#include "update.h"
#include "features/enemy.h"

void update(GameState *gameState, float dt) {
  enemySwarmUpdate(&(gameState->swarm), &(gameState->gameMap.flowField), dt);
}
