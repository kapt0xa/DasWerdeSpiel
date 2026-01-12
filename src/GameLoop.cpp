#include <cassert>
#include <thread>
#include <algorithm>
#include <execution>

#include "GameLoop.h"

namespace spiel
{
    std::vector<GameLoop> GameLoop::globalLoops = {GameLoop()};

    GameLoop::GameLoop() = default;

    GameLoop::GameLoop(GameLoop&& other) = default;
    //     : debugOs(other.debugOs),
    //       tickFunctions(std::move(other.tickFunctions)),
    //       deadIds(std::move(other.deadIds)),
    //       nextId(other.nextId),
    //       targetFrameTime(other.targetFrameTime),
    //       isFixedTimeStep(other.isFixedTimeStep),
    //       timeScale(other.timeScale)
    // {
    // }

    GameLoop& GameLoop::operator=(GameLoop&& other) = default;
    // {
    //     if(this != &other)
    //     {
    //         this->~GameLoop();
    //         new (this) GameLoop(std::move(other));
    //     }
    //     return *this;
    // }

    GameLoop::~GameLoop()
    {
        stopTicks();
    };

    size_t GameLoop::subscribeTickFunction(TickEvent func)
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
        subscribedFunctions.emplace(id, std::move(func));
        return id;
    }

    bool GameLoop::removeTickFunction(int id)
    {
        std::lock_guard lock(substribtionMutex);
        auto it = subscribedFunctions.find(static_cast<size_t>(id));
        if(it != subscribedFunctions.end())
        {
            subscribedFunctions.erase(it);
            deadIds.push_back(static_cast<size_t>(id));
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
        std::lock_guard<std::mutex> lock(loopLaunchMutex);
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
            std::for_each(std::execution::par, subscribedFunctions.begin(), subscribedFunctions.end(),
                            [deltaTime](const auto& entry)
                            {
                                entry.second(deltaTime);
                            });
        }
        else
        {
            std::for_each(std::execution::seq, subscribedFunctions.begin(), subscribedFunctions.end(),
                            [deltaTime](const auto& entry)
                            {
                                entry.second(deltaTime);
                            });
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
}