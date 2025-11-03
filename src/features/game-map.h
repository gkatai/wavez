#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "raylib.h"

#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define CENTER_X 20
#define CENTER_Y 20

typedef struct {
  Vector2 directions[MAP_HEIGHT][MAP_WIDTH];
} FlowField;

typedef struct {
  FlowField flowField;
} GameMap;

void gameMapInit(GameMap* gameMap);

#endif
