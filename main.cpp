/* main.cpp */
#include "Tetris.h"
#include "graphics.h"
#include "Blocks.h"

int main() {
    Tetris game;
    try {
        game.run();
    }
    catch (const std::exception& e) {
        closegraph();
        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}