#include "enemy.h"
#include "game-map.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
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

  int rows = swarm->count / 100;

  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < 100; i++) {
      swarm->positions[j * 100 + i].x = -22 + i * radius * 2;
      swarm->positions[j * 100 + i].y = 12 - j * radius * 2;
    }
  }

  swarm->mesh = GenMeshSphere(radius, 16, 16);

  swarm->shader = LoadShader("assets/shaders/lighting_instancing.vs",
                             "assets/shaders/lighting.fs");

  swarm->material = LoadMaterialDefault();
  swarm->material.shader = swarm->shader;
  swarm->material.maps[MATERIAL_MAP_DIFFUSE].color = RED;

  // transform matrix
  swarm->transforms = calloc(capacity, sizeof(Matrix));
  if (swarm->transforms == NULL) {
    UnloadMesh(swarm->mesh);
    UnloadMaterial(swarm->material);
    free(swarm->positions);
    swarm->capacity = 0;
    swarm->count = 0;
    return -1;
  }
  for (int i = 0; i < swarm->capacity; i++) {
    swarm->transforms[i] = MatrixIdentity();
  }

  return 0;
}

void enemySwarmFree(EnemySwarm *swarm) {
  if (swarm != NULL) {
    if (swarm->positions != NULL) {
      free(swarm->positions);
      swarm->positions = NULL;
    }

    if (swarm->transforms != NULL) {
      free(swarm->transforms);
      swarm->transforms = NULL;
    }

    UnloadMesh(swarm->mesh);
    UnloadMaterial(swarm->material);

    swarm->count = 0;
    swarm->capacity = 0;
  }
}

void enemySwarmUpdate(EnemySwarm *swarm, FlowField *flowField, float dt) {
  // move enemies through flowfield
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

  // resolve collisions
  float minDist = swarm->radius * 2;
  float minDistSquared = minDist * minDist;

  for (int i = 0; i < swarm->count; i++) {
    // check neighbors (only forward to avoid double-processing)
    for (int offset = 1; offset <= 20; offset++) {
      int j = i + offset;

      // skip items out of bounds
      if (j >= swarm->count) {
        break;
      }

      float dx = swarm->positions[i].x - swarm->positions[j].x;
      float dy = swarm->positions[i].y - swarm->positions[j].y;

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
        swarm->positions[i].x += separationX;
        swarm->positions[i].y += separationY;
        swarm->positions[j].x -= separationX;
        swarm->positions[j].y -= separationY;
      }
    }
  }

  // update transform matrix
  for (int i = 0; i < swarm->count; i++) {
    swarm->transforms[i] =
        MatrixTranslate(swarm->positions[i].x, 0.1, swarm->positions[i].y);
  }
}

void enemySwarmRender(EnemySwarm *swarm) {
  DrawMeshInstanced(swarm->mesh, swarm->material, swarm->transforms,
                    swarm->count);
  // for (int i = 0; i < swarm->count; i++) {
  //   DrawMesh(swarm->mesh, swarm->material, swarm->transforms[i]);
  // }
}
