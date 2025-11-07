#ifndef ENTITIES_H
#define ENTITIES_H

#include "game-map.h"
#include "raylib.h"
#include "swarm.h"

#define MAX_ENTITIES 5000

typedef struct {
  Vector2 positions[MAX_ENTITIES];
  Matrix transforms[MAX_ENTITIES];
  float radii[MAX_ENTITIES];
} Entities;

void entitiesInit(Entities *entities, Swarm *swarms, int swarmCount);
void entitiesUpdate(Entities *entities, FlowField *flowField, Swarm *swarms,
                    int swarmCount, float dt);
void entitiesRender(Entities *entities, Swarm *swarms, int swarmCount);

#endif
