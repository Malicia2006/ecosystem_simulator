#pragma once
#include "../Graphics/Window.hpp"
#include "Ecosystem.hpp"
#include <chrono>

namespace Ecosystem {
namespace Core {

class GameEngine {
private:
    Graphics::Window mWindow;
    Ecosystem mEcosystem;
    bool mIsRunning;
    bool mIsPaused;
    float mTimeScale;
    std::chrono::high_resolution_clock::time_point mLastUpdateTime;
public:
    GameEngine(const std::string& title, int width, int height);
    bool Initialize();
    void Run();
    void Shutdown();
    void HandleEvents();
    void HandleInput(SDL_Keycode key);
private:
    void Update(float deltaTime);
    void Render();
    void RenderUI();
};
} // namespace Core
} // namespace Ecosystem
