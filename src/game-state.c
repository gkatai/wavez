#include "game-state.h"
#include "features/entities.h"
#include "features/entity-type.h"
#include <stdlib.h>

GameState *gameStateInit(void) {
  GameState *gameState = malloc(sizeof(GameState));
  if (gameState == NULL) {
    return NULL;
  }

  gameMapInit(&(gameState->gameMap));
  entityTypeInit(&(gameState->swarms[0]), BASIC_ENEMY, 1000, 0.4);
  entityTypeInit(&(gameState->swarms[1]), BASIC_ENEMY, 1000, 0.4);
  entityTypeInit(&(gameState->swarms[2]), WALL, 3, 1);
  entitiesInit(&(gameState->entities), gameState->swarms, 3);

  return gameState;
}

void gameStateFree(GameState *gameState) {
  if (gameState == NULL) {
    return;
  }

  free(gameState);
}
