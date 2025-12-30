#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include <string>

#include "camera.h"

class Render {
public:
  bool mouseMode;

  Render();
  Render(int width, int height, std::string nameOfWindow);
  ~Render();

  void onEvent(SDL_Event *event);
  void loadCamera(Camera *camera_m);
  void UpdateViewPort();

  void setScreenWidth(int width);
  void setScreenHeight(int height);

  int GetScreenWidth() { return screen_width; }
  int GetScreenHeight() { return screen_height; }
  SDL_Window *GetWindowID() { return window; }
  bool isRuning(bool run = true) { return isRun; }

  bool isRun;

private:
  SDL_Window *window;
  SDL_GLContext glContext;
  Camera *camera;

  int screen_width;
  int screen_height;
  std::string name;

  void sdl_gl_init();
};
