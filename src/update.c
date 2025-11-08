#include "update.h"
#include "features/entities.h"

void update(GameState *gameState, float dt) {
  // debug on/off
  if (IsKeyPressed(KEY_D)) {
    gameState->gameMap.debug = !gameState->gameMap.debug;
  }

  entitiesUpdate(&(gameState->entities), &(gameState->gameMap.flowField),
                 gameState->swarms, 4, dt);
}
