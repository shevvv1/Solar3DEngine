#include "GL_render.h"
#include <SDL3/SDL_init.h>
#include <iostream>

void Render::sdl_gl_init() {

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    isRun = false;
  }
  window = SDL_CreateWindow("Test", screen_width, screen_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == nullptr) {
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
    SDL_Quit();
    isRun = false;
  }

  glContext = SDL_GL_CreateContext(window);
  if (glContext == nullptr) {
    SDL_Log("Failed to create OpenGL context:%s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    isRun = false;
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    isRun = false;
  }

  // Устанавливаем viewport OpenGL
  glViewport(0, 0, screen_width, screen_height);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

Render::Render()
    : screen_width{640}, screen_height{480}, name{"3DENG"}, isRun{true} {
  sdl_gl_init();
}

Render::Render(int width, int height, std::string nameOfWindow)
    : screen_width{width}, screen_height{height}, name{nameOfWindow},
      isRun{true} {
  sdl_gl_init();
}

Render::~Render() {
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Render::UpdateViewPort() { glViewport(0, 0, screen_width, screen_height); }

void Render::setScreenWidth(int width) { screen_width = width; }

void Render::setScreenHeight(int height) { screen_height = height; }
