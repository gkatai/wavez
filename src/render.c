#include "render.h"
#include "game-state.h"
#include "features/game-map.h"
#include "raylib.h"

void render3D(GameState *gameState) {

  DrawGrid(50, 1.0f);

  // Draw flow field arrows for debugging
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      // Base position (centered around origin, slightly above ground)
      Vector3 basePos = {x - 25.0f, 0.1f, y - 25.0f};

      Vector2 dir = gameState->gameMap.flowField.directions[y][x];

      float arrowLength = 0.7f;
      Vector3 tipPos = {
          basePos.x + dir.x * arrowLength, 0.1f, basePos.z + dir.y * arrowLength};

      DrawLine3D(basePos, tipPos, BLUE);
    }
  }
}
