#ifndef ENTITIES_H
#define ENTITIES_H

#include "game-map.h"
#include "raylib.h"
#include "swarm.h"

#define MAX_ENTITIES 5000
#define SPATIAL_GRID_CELL_SIZE 2.0f
#define SPATIAL_GRID_WIDTH 25
#define SPATIAL_GRID_HEIGHT 25
#define SPATIAL_GRID_TOTAL_CELLS 625
#define MAX_ENTITIES_PER_CELL 100

typedef struct {
  int entityIndices[MAX_ENTITIES_PER_CELL];
  int count;
} GridCell;

typedef struct {
  Vector2 positions[MAX_ENTITIES];
  Matrix transforms[MAX_ENTITIES];
  float radii[MAX_ENTITIES];
  GridCell cells[SPATIAL_GRID_TOTAL_CELLS];
  int gridWidth;
  int gridHeight;
  float cellSize;
} Entities;

void entitiesInit(Entities *entities, Swarm *swarms, int swarmCount);
void entitiesUpdate(Entities *entities, FlowField *flowField, Swarm *swarms,
                    int swarmCount, float dt);
void entitiesRender(Entities *entities, Swarm *swarms, int swarmCount);

#endif
