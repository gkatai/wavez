#include "entities.h"
#include "raymath.h"
#include "swarm.h"
#include <math.h>

void entitiesInit(Entities *entities, Swarm *swarms, int swarmCount) {
  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

    int rows = swarm->count / 100;

    for (int j = 0; j < rows; j++) {
      for (int i = 0; i < 100; i++) {
        int currentEntityIndex = entityIndex + (j * 100 + i);
        entities->positions[currentEntityIndex].x = -22 + i * swarm->radius * 2;
        entities->positions[currentEntityIndex].y = 12 - j * swarm->radius * 2;
      }
    }

    entityIndex += swarm->count;
  }
}

void entitiesUpdate(Entities *entities, FlowField *flowField, Swarm *swarms,
                    int swarmCount, float dt) {
  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

    // move enemies through flowfield
    for (int enemyIndex = 0; enemyIndex < swarm->count; enemyIndex++) {
      int currentEntityIndex = entityIndex + enemyIndex;
      int gridX = (int)(entities->positions[currentEntityIndex].x + 25.0);
      int gridY = (int)(entities->positions[currentEntityIndex].y + 25.0);

      if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        // enemy out of bounds, skip
        continue;
      }

      Vector2 direction = flowField->directions[gridY][gridX];

      float speed = 1;
      entities->positions[currentEntityIndex].x += direction.x * speed * dt;
      entities->positions[currentEntityIndex].y += direction.y * speed * dt;
    }

    float minDist = swarm->radius * 2;
    float minDistSquared = minDist * minDist;

    for (int enemyIndex = 0; enemyIndex < swarm->count; enemyIndex++) {
      int currentEntityIndex = entityIndex + enemyIndex;

      // check neighbors (only forward to avoid double-processing)
      for (int offset = 1; offset <= 20; offset++) {
        int nextEnemyIndex = enemyIndex + offset;

        // skip items out of bounds
        if (nextEnemyIndex >= swarm->count) {
          break;
        }

        int nextEntityIndex = entityIndex + nextEnemyIndex;

        float dx = entities->positions[currentEntityIndex].x -
                   entities->positions[nextEntityIndex].x;
        float dy = entities->positions[currentEntityIndex].y -
                   entities->positions[nextEntityIndex].y;

        float distSquared = dx * dx + dy * dy;

        // collision
        if (distSquared < minDistSquared) {
          float distance = sqrtf(distSquared);

          // don't divide by zero
          if (distance < 0.0001f) {
            continue;
          }

          float overlap = minDist - distance;

          // normalized directions
          float nx = dx / distance;
          float ny = dy / distance;

          // separation
          float separationX = nx * overlap * 0.5;
          float separationY = ny * overlap * 0.5;

          // push enemies apart
          entities->positions[currentEntityIndex].x += separationX;
          entities->positions[currentEntityIndex].y += separationY;
          entities->positions[nextEntityIndex].x -= separationX;
          entities->positions[nextEntityIndex].y -= separationY;
        }
      }
    }

    // update transform matrices for all entities in this swarm
    for (int enemyIndex = 0; enemyIndex < swarm->count; enemyIndex++) {
      int currentEntityIndex = entityIndex + enemyIndex;
      entities->transforms[currentEntityIndex] =
          MatrixTranslate(entities->positions[currentEntityIndex].x, 0.1,
                          entities->positions[currentEntityIndex].y);
    }

    entityIndex += swarm->count;
  }
}

void entitiesRender(Entities *entities, Swarm *swarms, int swarmCount) {
  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

    DrawMeshInstanced(swarm->mesh, swarm->material,
                      &entities->transforms[entityIndex], swarm->count);

    entityIndex += swarm->count;
  }
}
