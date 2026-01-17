#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include <string>

#include "camera.h"

class Render {
public:
  Render();
  Render(int width, int height, std::string nameOfWindow);
  ~Render();

  bool Run();
  void onEvent(SDL_Event *event, Camera &camera);
  void loadCamera(Camera *camera_m);
  void UpdateViewPort();

  void setScreenWidth(int width);
  void setScreenHeight(int height);
  void setCursorMode(bool mode);

  void switchCursorMode();

  int GetScreenWidth() { return screen_width; }
  int GetScreenHeight() { return screen_height; }
  SDL_Window *GetWindowID() { return window; }
  float GetDeltaTime() { return m_deltaTime.delta_time; }
  bool getCursorMode() { return m_showCursor; }

  bool isRun;

private:
  SDL_Window *window;
  SDL_GLContext glContext;
  Camera *camera;

  int screen_width;
  int screen_height;
  std::string name;

  bool m_showCursor;

  struct deltaTime {
    float currentFrame = 0.f;
    float lastFrame = 0.f;
    float delta_time = 0.f;
  } m_deltaTime;

  void sdl_gl_init();
};
