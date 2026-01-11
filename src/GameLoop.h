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
    using TickFunction = std::function<void(float deltaTime)>;

    class GameLoop
    {
    public:
        GameLoop();
        GameLoop(const GameLoop&) = delete;
        GameLoop& operator=(const GameLoop&) = delete;
        GameLoop(GameLoop&&);
        GameLoop& operator=(GameLoop&&);
        ~GameLoop();

        size_t submitTickFunctionRaw(TickFunction func);
        bool removeTickFunctionRaw(int id);

        bool runTicks();
        bool stopTicks();

        void tick(float deltaTime);

    public:
        std::ostream* debugOs = nullptr;

        static std::vector<GameLoop> globalLoops;
    private:
        std::unordered_map<size_t,TickFunction> tickFunctions;
        std::vector<size_t> deadIds;
        size_t nextId = 0;

        float targetFrameTime = 1.0f / 60.0f;
        bool isFixedTimeStep = true;
        float timeScale = 1.0f;
        std::mutex editMutex;

        std::atomic<bool> isTicking = false;
    };
}