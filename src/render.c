#include "render.h"
#include "features/enemy.h"
#include "game-state.h"

void render3D(GameState *gameState) {
  gameMapRender(&(gameState->gameMap));
  enemySwarmRender(&(gameState->swarm));
}
