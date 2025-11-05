#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "raylib.h"

#define MAP_WIDTH 50
#define MAP_HEIGHT 50

typedef struct {
  Vector2 directions[MAP_HEIGHT][MAP_WIDTH];
} FlowField;

typedef struct {
  Vector2 size;
  Vector2 center;
  FlowField flowField;
  bool debug;
} GameMap;

void gameMapInit(GameMap *gameMap);
void gameMapRender(GameMap *gameMap);

#endif
