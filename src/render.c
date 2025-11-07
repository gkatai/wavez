#include "render.h"
#include "features/entities.h"
#include "game-state.h"

void render3D(GameState *gameState) {
  gameMapRender(&(gameState->gameMap));
  entitiesRender(&(gameState->entities), gameState->swarms, 1);
}
