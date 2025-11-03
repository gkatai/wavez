#include "game-state.h"
#include <stdlib.h>

GameState *gameStateInit(void) {
  GameState *gameState = malloc(sizeof(GameState));
  gameMapInit(&(gameState->gameMap));

  return gameState;
}

void gameStateFree(GameState *gameState) { free(gameState); }
