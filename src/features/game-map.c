#include "game-map.h"
#include <math.h>

void gameMapInit(GameMap *gameMap) {
  gameMap->size = (Vector2){.x = MAP_WIDTH, .y = MAP_HEIGHT};
  gameMap->center = (Vector2){.x = 25, .y = 25};

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      float dx = gameMap->center.x - x;
      float dy = gameMap->center.y - y;
      float len = sqrt(dx * dx + dy * dy);

      if (len > 0.01f) {
        gameMap->flowField.directions[y][x].x = dx / len;
        gameMap->flowField.directions[y][x].y = dy / len;
      } else {
        gameMap->flowField.directions[y][x].x = 0;
        gameMap->flowField.directions[y][x].y = 0;
      }
    }
  }
}

void gameMapRenderFlowFieldVectors(FlowField* flowField) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      Vector3 basePos = {x - 25.0f, 0.1f, y - 25.0f};

      Vector2 dir = flowField->directions[y][x];

      float arrowLength = 0.7f;
      Vector3 tipPos = {
          basePos.x + dir.x * arrowLength, 0.1f, basePos.z + dir.y * arrowLength};

      DrawLine3D(basePos, tipPos, BLUE);
    }
  }
}

void gameMapRender(GameMap* gameMap) {
  DrawGrid(50, 1.0f);
  gameMapRenderFlowFieldVectors(&(gameMap->flowField));
}
