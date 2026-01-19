#include <cassert>
#include <thread>
#include <algorithm>
#include <execution>
#include <iostream>

#include "GameLoop.h"

namespace spiel
{
    GameLoop::GameLoop() = default;

    GameLoop::~GameLoop()
    {
        stopTicks();
    };

    size_t GameLoop::subscribeTickFunctionRaw(TickEvent func, ptrdiff_t executionOrder)
    {
        std::lock_guard lock(substribtionMutex);
        size_t id;
        if(!deadIds.empty())
        {
            id = deadIds.back();
            deadIds.pop_back();
        }
        else
        {
            id = nextId++;
        }
        auto& subscribedFunctions = subscribedFunctionsOrdered[executionOrder];
        subscribedFunctions.emplace(id, std::move(func));
        return id;
    }

    bool GameLoop::removeTickFunction(size_t id)
    {
        std::lock_guard lock(substribtionMutex);
        auto& subscribedFunctions = subscribedFunctionsOrdered[subscribtionOrder[id]];
        auto it = subscribedFunctions.find(id);
        if(it != subscribedFunctions.end())
        {
            subscribedFunctions.erase(it);
            deadIds.push_back(id);
            return true;
        }
        return false;
    }

    bool GameLoop::runTicks()
    {
        {
            std::lock_guard<std::mutex> lock(loopLaunchMutex);
            if(isTicking)
            {
                return false;
            }

            isTicking = true;
        }

        std::chrono::steady_clock clock;
        auto lastTime = clock.now();
        float deltaTime = targetFrameTime;

        while(!stopTickingFlag)
        {
            tick(deltaTime);
            executeQueue();

            auto newTime = clock.now();
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(newTime - lastTime).count();

            if(deltaTime < targetFrameTime)
            {
                std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime-deltaTime));
                newTime = clock.now();
                deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(newTime - lastTime).count();
            }

            lastTime = newTime;
        }

        {
            std::lock_guard<std::mutex> lock(loopLaunchMutex);
            stopTickingFlag = false;
            assert(isTicking);
            isTicking = false;
        }

        return true;
    }

    bool GameLoop::stopTicks()
    {
        std::lock_guard<std::mutex> lock(loopLaunchMutex);
        if(! isTicking)
        {
            return false;
        }
        if(stopTickingFlag)
        {
            return false;
        }

        stopTickingFlag = true;
        return true;
    }

    void GameLoop::addToQueue(SimpleEvent func)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        queuedEvents.push_back(std::move(func));
    }

    void GameLoop::tick(float deltaTime)
    {
        if(isFixedTimeStep)
        {
            deltaTime = targetFrameTime;
        }

        deltaTime *= timeScale;

        std::lock_guard<std::mutex> lock(substribtionMutex);
        if(isParallelExecutionPolicy)
        {
            for(auto& [order, subscribedFunctions] : subscribedFunctionsOrdered)
            {
                std::for_each(std::execution::par, subscribedFunctions.begin(), subscribedFunctions.end(),
                                [deltaTime](const auto& entry)
                                {
                                    entry.second(deltaTime);
                                });
            }
        }
        else
        {
            for(auto& [order, subscribedFunctions] : subscribedFunctionsOrdered)
            {
                std::for_each(std::execution::seq, subscribedFunctions.begin(), subscribedFunctions.end(),
                                [deltaTime](const auto& entry)
                                {
                                    entry.second(deltaTime);
                                });
            }
        }
    }

    void GameLoop::executeQueue()
    {
        std::vector<SimpleEvent> buf;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            std::swap(buf, queuedEvents);
        }

        if(isParallelExecutionPolicy)
        {
            std::for_each(std::execution::par, buf.begin(), buf.end(),
                            [](const auto& entry) { entry(); });
        }
        else
        {
            std::for_each(std::execution::seq, buf.begin(), buf.end(),
                            [](const auto& entry) { entry(); });
        }
    }

    float GameLoop::getTimeScale() const
    {
        return timeScale;
    }

    void GameLoop::setTimeScale(float value)
    {
        timeScale = value;
    }

    SubscribtionGuard::SubscribtionGuard(GameLoop& loopRef, size_t idVal)
        : loop(&loopRef), id(idVal) {}
    
    SubscribtionGuard::~SubscribtionGuard()
    {
        unsubscribe();
    }

    bool SubscribtionGuard::unsubscribe()
    {
        if(loop)
        {
            bool result = loop->removeTickFunction(static_cast<int>(id));
            loop = nullptr;
            return result;
        }
        return false;
    }

    SubscribtionGuard GameLoop::subscribeTickFunction(TickEvent func)
    {
        size_t id = subscribeTickFunctionRaw(std::move(func));
        return SubscribtionGuard(*this, id);
    }

    SubscribtionGuard::SubscribtionGuard(SubscribtionGuard&& other) noexcept
        : loop(other.loop), id(other.id)
    {
        other.loop = nullptr;
    }

    SubscribtionGuard& SubscribtionGuard::operator=(SubscribtionGuard&& other) noexcept
    {
        if(this != &other)
        {
            unsubscribe();
            loop = other.loop;
            id = other.id;
            other.loop = nullptr;
        }
        return *this;
    }
}