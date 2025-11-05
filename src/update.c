#include "update.h"
#include "features/enemy.h"

void update(GameState *gameState, float dt) {
  // debug on/off
  if (IsKeyPressed(KEY_D)) {
    gameState->gameMap.debug = !gameState->gameMap.debug;
  }

  enemySwarmUpdate(&(gameState->swarm), &(gameState->gameMap.flowField), dt);
}
