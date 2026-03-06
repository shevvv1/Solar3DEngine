#include "GL_render.h"

#include <SDL3/SDL_init.h>

#include <iostream>

#include "SDL3/SDL_video.h"
#include "camera.h"

void Render::sdl_gl_init() {
  m_hideCursor = false;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    isRun = false;
  }
  m_window = SDL_CreateWindow("Test", m_screen_width, m_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (m_window == nullptr) {
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
    SDL_Quit();
    isRun = false;
  }

  m_glContext = SDL_GL_CreateContext(m_window);
  if (m_glContext == nullptr) {
    SDL_Log("Failed to create OpenGL context:%s\n", SDL_GetError());
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    isRun = false;
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    isRun = false;
  }

  SDL_SetWindowRelativeMouseMode(m_window, m_hideCursor);

  glViewport(0, 0, m_screen_width, m_screen_height);

  glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

Render::Render() : m_screen_width{640}, m_screen_height{480}, m_name{"NONAME"}, isRun{true} { sdl_gl_init(); }

Render::Render(int width, int height, std::string nameOfWindow)
    : m_screen_width{width}, m_screen_height{height}, m_name{nameOfWindow}, isRun{true} {
  sdl_gl_init();
}

Render::~Render() {
  SDL_GL_DestroyContext(m_glContext);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

bool Render::Run() {
  m_deltaTime.currentFrame = SDL_GetPerformanceCounter();
  m_deltaTime.delta_time =
      (float)((m_deltaTime.currentFrame - m_deltaTime.lastFrame) * 1000 / (float)SDL_GetPerformanceFrequency()) *
      0.001f;
  m_deltaTime.lastFrame = m_deltaTime.currentFrame;

  return isRun;
}

void Render::onEvent(SDL_Event* event, Camera& camera) {
  if (event->type == SDL_EVENT_QUIT) {
    isRun = false;
  }
  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    int width, height;
    SDL_GetWindowSize(getWindowID(), &width, &height);
    setScreenWidth(width);
    setScreenHeight(height);
    UpdateViewPort();
    camera.setSize(width, height);
  }
}
void Render::setCursorMode(bool mode) {
  m_hideCursor = mode;

  SDL_SetWindowRelativeMouseMode(m_window, m_hideCursor);
}

void Render::switchCursorMode() {
  m_hideCursor = !m_hideCursor;

  SDL_SetWindowRelativeMouseMode(m_window, m_hideCursor);
}

void Render::UpdateViewPort() { glViewport(0, 0, m_screen_width, m_screen_height); }

void Render::UpdateSDL_GLAttributes() {
  for (auto& it : m_SDL_GLAttr) {
    SDL_GL_SetAttribute(it.first, it.second);
  }
}

void Render::AddSDL_GLAttributes(SDL_GLAttr attr, int value) {
  if (m_SDL_GLAttr.find(attr) == m_SDL_GLAttr.end()) {
    std::clog << "This SDL attribute already exist:Rewrite\n";
  }
  m_SDL_GLAttr[attr] = value;
  UpdateSDL_GLAttributes();
}

void Render::ClearSDL_GLAttributes() { m_SDL_GLAttr.clear(); }
