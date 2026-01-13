#pragma once

#include <mutex>
#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional> // using std::function<returnType(inputTypes)>

namespace spiel
{
    using TickEvent = std::function<void(float deltaTime)>;
    using SimpleEvent = std::function<void()>;

    class GameLoop
    {
    public:
        GameLoop();
        GameLoop(const GameLoop&) = delete;
        GameLoop& operator=(const GameLoop&) = delete;
        GameLoop(GameLoop&&) = delete;
        GameLoop& operator=(GameLoop&&) = delete;
        ~GameLoop();

        // never use this method in the tick function, it will lead to dead lock. U can do that in queue (addToQueue)
        size_t subscribeTickFunction(TickEvent func);
        // never use this method in the tick function, it will lead to dead lock. U can do that in queue (addToQueue)
        bool removeTickFunction(int id);

        // u can use this method in addToQueue, it will not lead to dead lock
        void addToQueue(SimpleEvent func);

        float getTimeScale() const;
        void setTimeScale(float value);

        // returns false if failed to run
        // does not return untill stopTicks is called
        // for example, call stopTicks in submited function to stop the loop.
        bool runTicks();

        // result is not immediate
        bool stopTicks();

        void tick(float deltaTime);
        void executeQueue();

    public:

        std::ostream* debugOs = nullptr;
    private:

        // dont change map into array, U tried once, and got problems with subscribtion ID's
        std::mutex substribtionMutex;
        std::unordered_map<size_t,TickEvent> subscribedFunctions;
        std::vector<size_t> deadIds;
        size_t nextId = 0;

        std::mutex queueMutex;
        std::vector<SimpleEvent> queuedEvents;

        std::atomic<float> timeScale = 1.0f;
        float targetFrameTime = 1.0f / 60.0f;
        bool isFixedTimeStep = true;
        bool isParallelExecutionPolicy = true;

        std::mutex loopLaunchMutex;
        bool isTicking = false;
        std::atomic<bool> stopTickingFlag = false;
    };
}