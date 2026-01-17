#include "GL_render.h"
#include "camera.h"
#include <SDL3/SDL_init.h>
#include <iostream>

void Render::sdl_gl_init() {

  m_showCursor = false;

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

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);

  SDL_SetWindowRelativeMouseMode(window, m_showCursor);

  glViewport(0, 0, screen_width, screen_height);
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

bool Render::Run() {
  m_deltaTime.currentFrame = SDL_GetPerformanceCounter();
  m_deltaTime.delta_time =
      (float)((m_deltaTime.currentFrame - m_deltaTime.lastFrame) * 1000 /
              (float)SDL_GetPerformanceFrequency()) *
      0.001f;
  m_deltaTime.lastFrame = m_deltaTime.currentFrame;

  return isRun;
}

void Render::onEvent(SDL_Event *event, Camera &camera) {
  if (event->type == SDL_EVENT_QUIT) {
    isRun = false;
  }
  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    int width, height;
    SDL_GetWindowSize(GetWindowID(), &width, &height);
    setScreenWidth(width);
    setScreenHeight(height);
    UpdateViewPort();
    camera.setSize(width, height);
  }
}
void Render::setCursorMode(bool mode) {
  m_showCursor = mode;

  SDL_SetWindowRelativeMouseMode(window, m_showCursor);
}

void Render::switchCursorMode() {
  m_showCursor = !m_showCursor;

  SDL_SetWindowRelativeMouseMode(window, m_showCursor);
}

void Render::UpdateViewPort() { glViewport(0, 0, screen_width, screen_height); }

void Render::setScreenWidth(int width) { screen_width = width; }

void Render::setScreenHeight(int height) { screen_height = height; }
