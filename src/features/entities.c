#include "entities.h"
#include "raymath.h"
#include "swarm.h"
#include <math.h>

void entitiesInit(Entities *entities, Swarm *swarms, int swarmCount) {
  // Initialize spatial grid
  entities->gridWidth = SPATIAL_GRID_WIDTH;
  entities->gridHeight = SPATIAL_GRID_HEIGHT;
  entities->cellSize = SPATIAL_GRID_CELL_SIZE;

  // Clear all grid cells
  for (int i = 0; i < SPATIAL_GRID_TOTAL_CELLS; i++) {
    entities->cells[i].count = 0;
  }

  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    Swarm *swarm = &(swarms[currentSwarmIndex]);

    if (currentSwarmIndex == 0) {
      int rows = swarm->count / 50;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x =
              -22 + i * swarm->radius * 4;
          entities->positions[currentEntityIndex].y =
              12 - j * swarm->radius * 4;
        }
      }
    }

    if (currentSwarmIndex == 1) {
      int rows = swarm->count / 50;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x =
              -22 + i * swarm->radius * 4;
          entities->positions[currentEntityIndex].y =
              -12 - j * swarm->radius * 4;
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

  // Clear spatial grid
  for (int i = 0; i < SPATIAL_GRID_TOTAL_CELLS; i++) {
    entities->cells[i].count = 0;
  }

  // Bin entities into spatial grid cells
  for (int i = 0; i < totalEntities; i++) {
    // Convert world position to grid coordinates
    int gridX = (int)((entities->positions[i].x + 25.0f) / entities->cellSize);
    int gridY = (int)((entities->positions[i].y + 25.0f) / entities->cellSize);

    // Clamp to grid bounds
    if (gridX < 0)
      gridX = 0;
    if (gridX >= entities->gridWidth)
      gridX = entities->gridWidth - 1;
    if (gridY < 0)
      gridY = 0;
    if (gridY >= entities->gridHeight)
      gridY = entities->gridHeight - 1;

    int cellIndex = gridY * entities->gridWidth + gridX;
    GridCell *cell = &entities->cells[cellIndex];

    // Add entity to cell if there's space
    if (cell->count < MAX_ENTITIES_PER_CELL) {
      cell->entityIndices[cell->count++] = i;
    }
  }

  // Initialize corrections array
  Vector2 corrections[MAX_ENTITIES] = {0};

  // Spatial grid collision detection
  for (int i = 0; i < totalEntities; i++) {
    // Get entity's grid cell
    int gridX = (int)((entities->positions[i].x + 25.0f) / entities->cellSize);
    int gridY = (int)((entities->positions[i].y + 25.0f) / entities->cellSize);

    // Clamp to grid bounds
    if (gridX < 0)
      gridX = 0;
    if (gridX >= entities->gridWidth)
      gridX = entities->gridWidth - 1;
    if (gridY < 0)
      gridY = 0;
    if (gridY >= entities->gridHeight)
      gridY = entities->gridHeight - 1;

    // Check current cell and 8 neighboring cells
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
        int neighborX = gridX + dx;
        int neighborY = gridY + dy;

        // Skip out of bounds neighbors
        if (neighborX < 0 || neighborX >= entities->gridWidth ||
            neighborY < 0 || neighborY >= entities->gridHeight) {
          continue;
        }

        int cellIndex = neighborY * entities->gridWidth + neighborX;
        GridCell *cell = &entities->cells[cellIndex];

        // Check collisions with all entities in this cell
        for (int k = 0; k < cell->count; k++) {
          int j = cell->entityIndices[k];

          // Skip self and already-checked pairs
          if (j <= i)
            continue;

          float minDist = entities->radii[i] + entities->radii[j];
          float minDistSquared = minDist * minDist;

          float dx_pos = entities->positions[i].x - entities->positions[j].x;
          float dy_pos = entities->positions[i].y - entities->positions[j].y;

          float distSquared = dx_pos * dx_pos + dy_pos * dy_pos;

          // collision
          if (distSquared < minDistSquared) {
            float distance = sqrtf(distSquared);

            // don't divide by zero
            if (distance < 0.0001) {
              continue;
            }

            float overlap = minDist - distance;

            // normalized directions
            float nx = dx_pos / distance;
            float ny = dy_pos / distance;

            // separation
            float separationX = nx * overlap * 0.5;
            float separationY = ny * overlap * 0.5;

            // accumulate corrections
            corrections[i].x += separationX;
            corrections[i].y += separationY;
            corrections[j].x -= separationX;
            corrections[j].y -= separationY;
          }
        }
      }
    }
  }

  // Apply accumulated corrections with damping
  float dampingFactor = 0.3f;
  for (int i = 0; i < totalEntities; i++) {
    entities->positions[i].x += corrections[i].x * dampingFactor;
    entities->positions[i].y += corrections[i].y * dampingFactor;
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
