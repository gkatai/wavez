#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "features/game-map.h"

typedef struct {
  GameMap gameMap;
} GameState;

GameState* gameStateInit(void);
void gameStateFree(GameState* gameState);

#endif
