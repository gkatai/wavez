#include "game-state.h"
#include "features/entities.h"
#include "features/swarm.h"
#include <stdlib.h>

GameState *gameStateInit(void) {
  GameState *gameState = malloc(sizeof(GameState));
  if (gameState == NULL) {
    return NULL;
  }

  gameMapInit(&(gameState->gameMap));
  swarmInit(&(gameState->swarms[0]), 1000, 0.2);
  entitiesInit(&(gameState->entities), gameState->swarms, 1);

  return gameState;
}

void gameStateFree(GameState *gameState) {
  if (gameState == NULL) {
    return;
  }

  free(gameState);
}
