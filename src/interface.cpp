#include "../include/interface.h"

Interface::Interface() : game() {}

void Interface::run_game() {
    game.draw_game();
}
