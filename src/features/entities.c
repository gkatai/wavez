#include "entities.h"
#include "raymath.h"
#include <math.h>

void entitiesInit(Entities *entities, EntityType *swarms, int swarmCount) {
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
    EntityType *swarm = &(swarms[currentSwarmIndex]);

    if (currentSwarmIndex == 0) {
      int rows = swarm->count / 100;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x = -22 + i * swarm->radius;
          entities->positions[currentEntityIndex].y = 12 - j * swarm->radius;
        }
      }
    }

    if (currentSwarmIndex == 1) {
      int rows = swarm->count / 100;

      for (int j = 0; j < rows; j++) {
        for (int i = 0; i < 100; i++) {
          int currentEntityIndex = entityIndex + (j * 100 + i);
          entities->positions[currentEntityIndex].x = -22 + i * swarm->radius;
          entities->positions[currentEntityIndex].y = -12 - j * swarm->radius;
        }
      }
    }

    if (currentSwarmIndex == 2) {
      entities->positions[entityIndex].x = 5;
      entities->positions[entityIndex].y = 0;
      entities->positions[entityIndex + 1].x = 5;
      entities->positions[entityIndex + 1].y = 1;
      entities->positions[entityIndex + 2].x = 5;
      entities->positions[entityIndex + 2].y = 2;
    }

    if (currentSwarmIndex == 3) {
      entities->positions[entityIndex].x = 0;
      entities->positions[entityIndex].y = 0;
    }

    for (int i = 0; i < swarm->count; i++) {
      // set radius for all entities
      entities->radii[entityIndex + i] = swarm->radius;
      // set isStatic for all entities
      entities->isStatic[entityIndex + i] = swarm->isStatic;
      // set health for all entities
      entities->health[entityIndex + i] = swarm->health;
      // set isEnemy for all entities
      entities->isEnemy[entityIndex + i] = swarm->isEnemy;
      // set damage
      entities->damage[entityIndex + i] = swarm->damage;
      // set damage radius
      entities->damageRadius[entityIndex + i] = swarm->damageRadius;
      // set damage cooldown (0 = ready to damage immediately)
      entities->damageCooldowns[entityIndex + i] = 0.0f;
    }

    entityIndex += swarm->count;
  }
}

void entitiesUpdate(Entities *entities, FlowField *flowField,
                    EntityType *swarms, int swarmCount, float dt) {
  int entityIndex = 0;

  // Move all entities through flowfield
  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    EntityType *swarm = &(swarms[currentSwarmIndex]);

    for (int enemyIndex = 0; enemyIndex < swarm->count; enemyIndex++) {
      int currentEntityIndex = entityIndex + enemyIndex;

      // Skip dead entities
      if (entities->health[currentEntityIndex] <= 0) {
        continue;
      }

      // Skip movement for static entities
      if (entities->isStatic[currentEntityIndex]) {
        continue;
      }

      int gridX = (int)floorf(entities->positions[currentEntityIndex].x + 25.0f);
      int gridY = (int)floorf(entities->positions[currentEntityIndex].y + 25.0f);

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
    int gridX = (int)floorf((entities->positions[i].x + 25.0f) / entities->cellSize);
    int gridY = (int)floorf((entities->positions[i].y + 25.0f) / entities->cellSize);

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

  // Initialize corrections array to zero
  for (int i = 0; i < totalEntities; i++) {
    entities->corrections[i].x = 0.0f;
    entities->corrections[i].y = 0.0f;
  }

  // collision detection and damage application
  for (int i = 0; i < totalEntities; i++) {
    // Skip dead entities
    if (entities->health[i] <= 0)
      continue;

    // Check if this entity can deal damage
    bool canDamage =
        entities->isEnemy[i] && entities->damageCooldowns[i] <= 0.0f;
    bool damageDone = false;

    // Get entity's grid cell
    int gridX = (int)floorf((entities->positions[i].x + 25.0f) / entities->cellSize);
    int gridY = (int)floorf((entities->positions[i].y + 25.0f) / entities->cellSize);

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

        // Check all entities in this cell
        for (int k = 0; k < cell->count; k++) {
          int j = cell->entityIndices[k];

          // Skip self
          if (j == i)
            continue;

          // Skip dead entities
          if (entities->health[j] <= 0)
            continue;

          // Calculate distance once
          float dx_pos = entities->positions[i].x - entities->positions[j].x;
          float dy_pos = entities->positions[i].y - entities->positions[j].y;
          float distSquared = dx_pos * dx_pos + dy_pos * dy_pos;

          // Collision detection
          if (j > i && !(entities->isStatic[i] && entities->isStatic[j])) {
            float minDist = entities->radii[i] + entities->radii[j];
            float minDistSquared = minDist * minDist;

            if (distSquared < minDistSquared) {
              float distance = sqrtf(distSquared);

              // don't divide by zero
              if (distance >= 0.0001) {
                float overlap = minDist - distance;

                // normalized directions
                float nx = dx_pos / distance;
                float ny = dy_pos / distance;

                // separation
                float separationX = nx * overlap * 0.5;
                float separationY = ny * overlap * 0.5;

                // accumulate corrections
                entities->corrections[i].x += separationX;
                entities->corrections[i].y += separationY;
                entities->corrections[j].x -= separationX;
                entities->corrections[j].y -= separationY;
              }
            }
          }

          // Damage application (enemy -> non-enemy)
          if (canDamage && !damageDone && !entities->isEnemy[j]) {
            float damageRadiusSquared =
                entities->damageRadius[i] * entities->damageRadius[i];

            if (distSquared <= damageRadiusSquared) {
              entities->health[j] -= entities->damage[i];
              entities->damageCooldowns[i] = DAMAGE_COOLDOWN;
              damageDone = true;
              break; // Only damage one entity per cooldown cycle
            }
          }
        }
        if (damageDone)
          break;
      }
      if (damageDone)
        break;
    }
  }

  // Apply accumulated corrections with damping
  float dampingFactor = 0.3f;
  for (int i = 0; i < totalEntities; i++) {
    if (!entities->isStatic[i]) {
      entities->positions[i].x += entities->corrections[i].x * dampingFactor;
      entities->positions[i].y += entities->corrections[i].y * dampingFactor;
    }
  }

  // Decrement all damage cooldowns
  for (int i = 0; i < totalEntities; i++) {
    if (entities->damageCooldowns[i] > 0.0f) {
      entities->damageCooldowns[i] -= dt;
    }
  }

  // Update transform matrices for all entities
  entityIndex = 0;
  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    EntityType *swarm = &(swarms[currentSwarmIndex]);

    for (int enemyIndex = 0; enemyIndex < swarm->count; enemyIndex++) {
      int currentEntityIndex = entityIndex + enemyIndex;

      // Skip dead entities
      if (entities->health[currentEntityIndex] <= 0)
        continue;

      entities->transforms[currentEntityIndex] =
          MatrixTranslate(entities->positions[currentEntityIndex].x, 0.1,
                          entities->positions[currentEntityIndex].y);
    }

    entityIndex += swarm->count;
  }
}

void entitiesRender(Entities *entities, EntityType *swarms, int swarmCount) {
  int entityIndex = 0;

  for (int currentSwarmIndex = 0; currentSwarmIndex < swarmCount;
       currentSwarmIndex++) {
    EntityType *swarm = &(swarms[currentSwarmIndex]);

    // Build array of transforms for living entities only
    Matrix livingTransforms[swarm->count];
    int livingCount = 0;

    for (int i = 0; i < swarm->count; i++) {
      int currentEntityIndex = entityIndex + i;

      // Skip dead entities
      if (entities->health[currentEntityIndex] <= 0)
        continue;

      livingTransforms[livingCount++] =
          entities->transforms[currentEntityIndex];
    }

    // Only render if there are living entities
    if (livingCount > 0) {
      DrawMeshInstanced(swarm->mesh, swarm->material, livingTransforms,
                        livingCount);
    }

    entityIndex += swarm->count;
  }
}
