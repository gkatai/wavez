#include "swarm.h"
#include "raylib.h"

void swarmInit(Swarm *swarm, int capacity, float radius) {
  swarm->capacity = capacity;
  swarm->count = capacity;
  swarm->radius = radius;

  swarm->mesh = GenMeshSphere(radius / 2, 16, 16);

  swarm->shader = LoadShader("assets/shaders/lighting_instancing.vs",
                             "assets/shaders/lighting.fs");

  swarm->material = LoadMaterialDefault();
  swarm->material.shader = swarm->shader;
  swarm->material.maps[MATERIAL_MAP_DIFFUSE].color = RED;
}
