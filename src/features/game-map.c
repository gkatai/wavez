#include "game-map.h"
#include <math.h>

void gameMapInit(GameMap *gameMap) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      float dx = CENTER_X - x;
      float dy = CENTER_Y - y;
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
