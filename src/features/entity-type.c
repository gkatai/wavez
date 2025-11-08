#include "entity-type.h"
#include "raylib.h"

void entityTypeInit(EntityType *entityType, Type type, int capacity,
                    float radius) {
  entityType->capacity = capacity;
  entityType->count = capacity;
  entityType->radius = radius;

  switch (type) {
  case SPIRE:
    entityType->isStatic = true;
    entityType->isEnemy = false;
    entityType->health = 50;
    entityType->damage = 0;
    entityType->damageRadius = 0;
    entityType->mesh = GenMeshSphere(radius / 2, 16, 16);
    break;
  case BASIC_ENEMY:
    entityType->isStatic = false;
    entityType->isEnemy = true;
    entityType->health = 10;
    entityType->damage = 0.1;
    entityType->damageRadius = 1.5;
    entityType->mesh = GenMeshSphere(radius / 2, 16, 16);
    break;
  case WALL:
  default:
    entityType->isStatic = true;
    entityType->isEnemy = false;
    entityType->health = 5;
    entityType->damage = 0;
    entityType->damageRadius = 0;
    entityType->mesh = GenMeshCube(radius, radius, radius);
  }

  entityType->shader = LoadShader("assets/shaders/lighting_instancing.vs",
                                  "assets/shaders/lighting.fs");

  entityType->material = LoadMaterialDefault();
  entityType->material.shader = entityType->shader;
  entityType->material.maps[MATERIAL_MAP_DIFFUSE].color = RED;
}
