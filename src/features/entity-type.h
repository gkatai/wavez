#ifndef SWARM_H
#define SWARM_H

#include "raylib.h"

typedef enum { WALL, BASIC_TOWER, BASIC_ENEMY } Type;

typedef struct {
  int count;
  int capacity;
  float radius;
  Mesh mesh;
  Material material;
  Shader shader;
  Type type;
  bool isStatic;
} EntityType;

void entityTypeInit(EntityType *entityType, Type type, int capacity,
                    float radius);

#endif
