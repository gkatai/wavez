#include "game-state.h"
#include "features/enemy.h"
#include <stdlib.h>

GameState *gameStateInit(void) {
  GameState *gameState = malloc(sizeof(GameState));
  if (gameState == NULL) {
    return NULL;
  }

  gameMapInit(&(gameState->gameMap));
  if (enemySwarmInit(&gameState->swarm, 1000, 0.2)) {
    free(gameState);
    return NULL;
  }

  return gameState;
}

void gameStateFree(GameState *gameState) {
  if (gameState == NULL) {
    return;
  }

  enemySwarmFree(&(gameState->swarm));
  free(gameState);
}
