#include "entity-type.h"
#include "raylib.h"

void entityTypeInit(EntityType *entityType, Type type, int capacity,
                    float radius) {
  entityType->capacity = capacity;
  entityType->count = capacity;
  entityType->radius = radius;

  switch (type) {
  case BASIC_ENEMY:
    entityType->isStatic = false;
    entityType->mesh = GenMeshSphere(radius / 2, 16, 16);
    break;
  case WALL:
  default:
    entityType->isStatic = true;
    entityType->mesh = GenMeshCube(radius, radius, radius);
  }

  entityType->shader = LoadShader("assets/shaders/lighting_instancing.vs",
                                  "assets/shaders/lighting.fs");

  entityType->material = LoadMaterialDefault();
  entityType->material.shader = entityType->shader;
  entityType->material.maps[MATERIAL_MAP_DIFFUSE].color = RED;
}
