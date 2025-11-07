#include "game-state.h"
#include "raylib.h"
#include "render.h"
#include "update.h"
#include <stdlib.h>

int main(void) {
  const int screenWidth = 1000;
  const int screenHeight = 1000;

  InitWindow(screenWidth, screenHeight, "wavez");

  GameState *gameState = gameStateInit();
  if (gameState == NULL) {
    TraceLog(LOG_ERROR, "Failed to initialize game state!");
    CloseWindow();
    return 1;
  }

  Camera3D camera = {0};
  camera.position = (Vector3){0.0f, 65.0f, 5.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    update(gameState, dt);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    render3D(gameState);
    EndMode3D();

    DrawFPS(10, 10);

    EndDrawing();
  }

  gameStateFree(gameState);
  CloseWindow();

  return 0;
}
