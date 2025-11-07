#include "entities.h"
#include "raymath.h"
#include "swarm.h"
#include <math.h>

void entitiesInit(Entities *entities, Swarm *swarms, int swarmCount) {
  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

    if (currentSwarmIndex == 0) {
      int rows = swarm->count / 100;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x =
              -22 + i * swarm->radius * 2;
          entities->positions[currentEntityIndex].y =
              12 - j * swarm->radius * 2;
        }
      }
    }

    if (currentSwarmIndex == 1) {
      int rows = swarm->count / 100;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x =
              -22 + i * swarm->radius * 2;
          entities->positions[currentEntityIndex].y =
              -12 - j * swarm->radius * 2;
        }
      }
    }

    // set radius for all entities in this swarm
    for (int i = 0; i < swarm->count; i++) {
      entities->radii[entityIndex + i] = swarm->radius;
    }

    entityIndex += swarm->count;
  }
}

void entitiesUpdate(Entities *entities, FlowField *flowField, Swarm *swarms,
                    int swarmCount, float dt) {
  int entityIndex = 0;

  // Move all entities through flowfield
  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

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

    entityIndex += swarm->count;
  }

  // Calculate total entity count
  int totalEntities = 0;
  for (int s = 0; s < swarmCount; s++) {
    totalEntities += swarms[s].count;
  }

  // O(n²) collision detection - check every entity against every other
  for (int i = 0; i < totalEntities; i++) {
    for (int j = i + 1; j < totalEntities; j++) {
      float minDist = entities->radii[i] + entities->radii[j];
      float minDistSquared = minDist * minDist;

      float dx = entities->positions[i].x - entities->positions[j].x;
      float dy = entities->positions[i].y - entities->positions[j].y;

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
        entities->positions[i].x += separationX;
        entities->positions[i].y += separationY;
        entities->positions[j].x -= separationX;
        entities->positions[j].y -= separationY;
      }
    }
  }

  // Update transform matrices for all entities
  entityIndex = 0;
  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

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
