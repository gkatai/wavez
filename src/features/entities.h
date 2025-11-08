#ifndef ENTITIES_H
#define ENTITIES_H

#include "entity-type.h"
#include "game-map.h"
#include "raylib.h"

#define MAX_ENTITIES 5000
#define SPATIAL_GRID_CELL_SIZE 2.0f
#define SPATIAL_GRID_WIDTH 25
#define SPATIAL_GRID_HEIGHT 25
#define SPATIAL_GRID_TOTAL_CELLS 625
#define MAX_ENTITIES_PER_CELL 100
#define DAMAGE_COOLDOWN 1.0f

typedef struct {
  int entityIndices[MAX_ENTITIES_PER_CELL];
  int count;
} GridCell;

typedef struct {
  Vector2 positions[MAX_ENTITIES];
  Matrix transforms[MAX_ENTITIES];
  float radii[MAX_ENTITIES];
  bool isStatic[MAX_ENTITIES];
  bool isEnemy[MAX_ENTITIES];
  float health[MAX_ENTITIES];
  float damage[MAX_ENTITIES];
  float damageRadius[MAX_ENTITIES];
  float damageCooldowns[MAX_ENTITIES];
  Vector2 corrections[MAX_ENTITIES];
  GridCell cells[SPATIAL_GRID_TOTAL_CELLS];
  int gridWidth;
  int gridHeight;
  float cellSize;
} Entities;

void entitiesInit(Entities *entities, EntityType *swarms, int swarmCount);
void entitiesUpdate(Entities *entities, FlowField *flowField,
                    EntityType *swarms, int swarmCount, float dt);
void entitiesRender(Entities *entities, EntityType *swarms, int swarmCount);

#endif
