#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "features/entities.h"
#include "features/entity-type.h"
#include "features/game-map.h"

#define SWARM_COUNT 10

typedef struct {
  GameMap gameMap;
  EntityType swarms[SWARM_COUNT];
  Entities entities;
} GameState;

GameState *gameStateInit(void);
void gameStateFree(GameState *gameState);

#endif
