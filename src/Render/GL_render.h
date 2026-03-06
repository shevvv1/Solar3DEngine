#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>

#include <string>
#include <unordered_map>

#include "SDL3/SDL_video.h"
#include "camera.h"

class Render {
 public:
  Render();
  Render(int width, int height, std::string nameOfWindow);
  ~Render();

  bool Run();
  void onEvent(SDL_Event* event, Camera& camera);
  void UpdateViewPort();

  void UpdateSDL_GLAttributes();
  void AddSDL_GLAttributes(SDL_GLAttr attr, int value);
  void ClearSDL_GLAttributes();

  void setScreenWidth(int width) { m_screen_width = width; }
  void setScreenHeight(int height) { m_screen_height = height; }
  void getSDL_GLAtrrMap(std::unordered_map<SDL_GLAttr, int> new_map) { m_SDL_GLAttr = new_map; }

  void setCursorMode(bool mode);
  void switchCursorMode();

  int getScreenWidth() { return m_screen_width; }
  int getScreenHeight() { return m_screen_height; }
  SDL_Window* getWindowID() { return m_window; }
  float getDeltaTime() { return m_deltaTime.delta_time; }
  bool getCursorMode() { return m_hideCursor; }
  std::unordered_map<SDL_GLAttr, int> getSDL_GLAtrrMap() { return m_SDL_GLAttr; }

  bool isRun;

 private:
  SDL_Window* m_window;
  SDL_GLContext m_glContext;

  int m_screen_width;
  int m_screen_height;
  std::string m_name;

  bool m_hideCursor;

  struct deltaTime {
    float currentFrame = 0.f;
    float lastFrame = 0.f;
    float delta_time = 0.f;
  } m_deltaTime;

  std::unordered_map<SDL_GLAttr, int> m_SDL_GLAttr = {{SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
                                                      {SDL_GL_CONTEXT_MAJOR_VERSION, 4},
                                                      {SDL_GL_CONTEXT_MINOR_VERSION, 4}};

  void sdl_gl_init();
};
