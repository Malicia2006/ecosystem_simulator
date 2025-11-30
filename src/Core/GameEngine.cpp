#include "Core/GameEngine.hpp"
#include <iostream>
#include <SDL3/SDL.h>
#include <sstream>

namespace Ecosystem {
namespace Core {

GameEngine::GameEngine(const std::string& title, int width, int height)
    : mWindow(title, width, height), mEcosystem(static_cast<float>(width), static_cast<float>(height), 500),
      mIsRunning(false), mIsPaused(false), mTimeScale(1.0f) {}

bool GameEngine::Initialize() {
    if (!mWindow.Initialize()) return false;
    mEcosystem.Initialize(20, 5, 30);
    mIsRunning = true;
    mLastUpdateTime = std::chrono::high_resolution_clock::now();
    std::cout << "GameEngine initialized\n";
    return true;
}

void GameEngine::Run() {
    std::cout << "Starting loop\n";
    while (mIsRunning) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - mLastUpdateTime;
        mLastUpdateTime = now;
        float dt = elapsed.count();
        HandleEvents();
        if (!mIsPaused) Update(dt * mTimeScale);
        Render();
        SDL_Delay(16); // ~60fps
    }
}

void GameEngine::Shutdown() {
    mIsRunning = false;
    mWindow.Shutdown();
    std::cout << "Engine shutdown\n";
}

void GameEngine::HandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) mIsRunning = false;
        else if (e.type == SDL_EVENT_KEY_DOWN) HandleInput(e.key.keysym.sym);
    }
}

void GameEngine::HandleInput(SDL_Keycode key) {
    switch (key) {
        case SDLK_ESCAPE: mIsRunning = false; break;
        case SDLK_SPACE: mIsPaused = !mIsPaused; std::cout << (mIsPaused ? "Paused\n" : "Resumed\n"); break;
        case 'r': mEcosystem.Initialize(20,5,30); std::cout << "Reset\n"; break;
        case 'f': mEcosystem.SpawnFood(10); std::cout << "Added food\n"; break;
        case SDLK_UP: mTimeScale *= 1.5f; std::cout << "Speed x" << mTimeScale << "\n"; break;
        case SDLK_DOWN: mTimeScale /= 1.5f; std::cout << "Speed x" << mTimeScale << "\n"; break;
        default: break; 
    }
}

void GameEngine::Update(float deltaTime) {
    mEcosystem.Update(deltaTime);
    static float statsTimer = 0.0f;
    statsTimer += deltaTime;
    if (statsTimer >= 2.0f) {
        auto s = mEcosystem.GetStatistics();
        std::cout << "Stats - Herbivores:" << s.totalHerbivores << " Carnivores:" << s.totalCarnivores
                  << " Plants:" << s.totalPlants << " Food:" << s.totalFood
                  << " Births:" << s.birthsToday << " Deaths:" << s.deathsToday << "\n";
        statsTimer = 0.0f;
    }
}

void GameEngine::Render() {
    mWindow.Clear();
    mEcosystem.Render(mWindow.GetRenderer());
    RenderUI();
    mWindow.Present();
}

void GameEngine::RenderUI() {
    // For simplicity, console-based UI; could draw text on renderer if SDL_ttf added.
}

} // namespace Core
} // namespace Ecosystem
