#include <cassert>
#include <thread>

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
        if(isTicking)
        {
            stopTicks();
        }
    };

    size_t GameLoop::submitTickFunctionRaw(TickFunction func)
    {
        std::lock_guard lock(editMutex);
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
        tickFunctions.emplace(id, std::move(func));
        return id;
    }

    bool GameLoop::removeTickFunctionRaw(int id)
    {
        std::lock_guard lock(editMutex);
        auto it = tickFunctions.find(static_cast<size_t>(id));
        if(it != tickFunctions.end())
        {
            tickFunctions.erase(it);
            deadIds.push_back(static_cast<size_t>(id));
            return true;
        }
        return false;
    }

    bool GameLoop::runTicks()
    {
        if(isTicking)
        {
            return false;
        }

        std::chrono::steady_clock clock;
        auto lastTime = clock.now();
        float deltaTime = targetFrameTime;

        while(isTicking)
        {
            tick(isFixedTimeStep? targetFrameTime : deltaTime);

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

    }
}