#ifndef ENEMY_H
#define ENEMY_H

#include "game-map.h"
#include "raylib.h"

typedef struct {
  int count;
  int capacity;
  Vector2 *positions;
  float radius;
} EnemySwarm;

int enemySwarmInit(EnemySwarm *swarm, int capacity, float radius);
void enemySwarmFree(EnemySwarm *swarm);
void enemySwarmUpdate(EnemySwarm *swarm, FlowField *flowField, float dt);
void enemySwarmRender(EnemySwarm *swarm);

#endif
