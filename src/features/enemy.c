#include "enemy.h"
#include "game-map.h"
#include "raylib.h"
#include <stdlib.h>

int enemySwarmInit(EnemySwarm *swarm, int capacity, float radius) {
  swarm->capacity = capacity;
  swarm->count = capacity;
  swarm->radius = radius;

  swarm->positions = calloc(capacity, sizeof(Vector2));
  if (swarm->positions == NULL) {
    swarm->capacity = 0;
    swarm->count = 0;
    return -1;
  }

  for (int i = 0; i < capacity; i++) {
    swarm->positions[i].x = -22 + i * radius * 2;
    swarm->positions[i].y = 22;
  }

  return 0;
}

void enemySwarmFree(EnemySwarm *swarm) {
  if (swarm != NULL && swarm->positions != NULL) {
    free(swarm->positions);
    swarm->positions = NULL;
  }

  if (swarm != NULL) {
    swarm->count = 0;
    swarm->capacity = 0;
  }
}

void enemySwarmUpdate(EnemySwarm *swarm, FlowField *flowField, float dt) {
  for (int i = 0; i < swarm->count; i++) {
    int gridX = (int)(swarm->positions[i].x + 25.0);
    int gridY = (int)(swarm->positions[i].y + 25.0);

    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
      // enemy out of bounds, skip
      continue;
    }

    Vector2 direction = flowField->directions[gridY][gridX];

    float speed = 1;
    swarm->positions[i].x += direction.x * speed * dt;
    swarm->positions[i].y += direction.y * speed * dt;
  }
}

void enemySwarmRender(EnemySwarm *swarm) {
  for (int i = 0; i < swarm->count; i++) {
    DrawCircle3D((Vector3){.x = swarm->positions[i].x,
                           .y = 0.1,
                           .z = swarm->positions[i].y},
                 swarm->radius, (Vector3){1.0, 0.0, 0.0}, 90.0, RED);
  }
}
