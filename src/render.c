#include "render.h"
#include "game-state.h"

void render3D(GameState *gameState) { gameMapRender(&(gameState->gameMap)); }
