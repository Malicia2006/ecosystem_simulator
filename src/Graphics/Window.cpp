#include "Graphics/Window.hpp"
#include <iostream>

namespace Ecosystem {
namespace Graphics {

Window::Window(const std::string& title, int width, int height)
    : mTitle(title), mWidth(width), mHeight(height), mWindow(nullptr), mRenderer(nullptr), mIsInitialized(false) {}

Window::~Window() { Shutdown(); }

bool Window::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return false;
    }
    mWindow = SDL_CreateWindow(mTitle.c_str(), mWidth, mHeight, 0);
    if (!mWindow) { std::cerr << "CreateWindow error: " << SDL_GetError() << "\n"; SDL_Quit(); return false; }
    mRenderer = SDL_CreateRenderer(mWindow, nullptr);
    if (!mRenderer) { std::cerr << "CreateRenderer error: " << SDL_GetError() << "\n"; SDL_DestroyWindow(mWindow); SDL_Quit(); return false; }
    mIsInitialized = true;
    std::cout << "Window initialized: " << mTitle << "\n";
    return true;
}

void Window::Shutdown() {
    if (mRenderer) { SDL_DestroyRenderer(mRenderer); mRenderer = nullptr; }
    if (mWindow) { SDL_DestroyWindow(mWindow); mWindow = nullptr; }
    if (mIsInitialized) SDL_Quit();
    mIsInitialized = false;
    std::cout << "Window shutdown\n";
}

void Window::Clear(const Core::Color& color) {
    if (!mRenderer) return;
    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(mRenderer);
}

void Window::Present() {
    if (mRenderer) SDL_RenderPresent(mRenderer);
}

} // namespace Graphics
} // namespace Ecosystem
