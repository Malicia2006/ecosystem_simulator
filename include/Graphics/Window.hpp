#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "../Core/Structs.hpp"

namespace Ecosystem {
namespace Graphics {

class Window {
private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    int mWidth;
    int mHeight;
    bool mIsInitialized;
    std::string mTitle;
public:
    Window(const std::string& title, int width, int height);
    ~Window();
    bool Initialize();
    void Shutdown();
    void Clear(const Core::Color& color = Core::Color(30,30,30));
    void Present();
    SDL_Renderer* GetRenderer() const { return mRenderer; }
    bool IsInitialized() const { return mIsInitialized; }
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
};
} // namespace Graphics
} // namespace Ecosystem
