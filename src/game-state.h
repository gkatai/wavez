#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "features/enemy.h"
#include "features/game-map.h"

typedef struct {
  GameMap gameMap;
  EnemySwarm swarm;
} GameState;

GameState *gameStateInit(void);
void gameStateFree(GameState *gameState);

#endif
