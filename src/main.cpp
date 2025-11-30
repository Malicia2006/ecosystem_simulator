#include "Core/GameEngine.hpp"
#include <iostream>
int main(int argc, char* argv[]) {
    Ecosystem::Core::GameEngine engine("Ecosystem Simulator", 800, 600);
    if (!engine.Initialize()) {
        std::cerr << "Failed to initialize engine\n";
        return 1;
    }
    engine.Run();
    engine.Shutdown();
    return 0;
}
