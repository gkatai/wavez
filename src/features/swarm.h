#ifndef SWARM_H
#define SWARM_H

#include "raylib.h"

typedef struct {
  int count;
  int capacity;
  float radius;
  Mesh mesh;
  Material material;
  Shader shader;
} Swarm;

void swarmInit(Swarm *swarm, int capacity, float radius);

#endif
