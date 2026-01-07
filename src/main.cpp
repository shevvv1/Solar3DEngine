#include "Input/inputManager.h"
#include "Model/model.h"
#include "Render/GL_render.h"
#include "Render/shader.h"
#include "SDL3/SDL_events.h"
#include "camera.h"
#include "glad/glad.h"
#include "skybox.h"

void inputInit(InputManager &input, Camera &camera, Shader &shader);

void inputProcess(InputManager &input, SDL_Event &e);

int main(int argc, char *argv[]) {
  Render render(1200, 860, "Sosat");
  Camera camera(800, 600, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0, 0.0, 1.f));
  Shader shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
  Shader shaderSky("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
  Model model("models/teapot.obj", glm::vec3(0.0, -0.3, 10));
  SkyBox skybox("models/skybox");
  InputManager input;
  inputInit(input, camera, shader);

  glEnable(GL_FRAMEBUFFER_SRGB);
  float angle = 0;
  SDL_Event e;
  while (render.isRuning()) {
    if (SDL_PollEvent(&e)) {
      render.onEvent(&e, camera);
      inputProcess(input, e);
    }

    glClearColor(0.01, 0.01, 0.01, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    shaderSky.Activate();
    camera.updateCamSkyBox(shaderSky);
    skybox.Draw(shaderSky);
    glDepthFunc(GL_LESS);

    shader.Activate();
    camera.updateCam(shader);
    model.Rotate(glm::vec3(0, angle, 0));
    angle++;
    model.Draw(shader);

    SDL_GL_SwapWindow(render.GetWindowID());
  }
  return 0;
}

void inputInit(InputManager &input, Camera &camera, Shader &shader) {
  input.bindKey(SDLK_W, "change.FOV.90", [&]() { camera.changeFOV(90); });
  input.bindKey(SDLK_S, "change.FOV.120", [&]() { camera.changeFOV(120); });
  input.bindKey(SDLK_R, "shader.hot_reload", [&]() {
    shader.HotReload("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
  });
}

void inputProcess(InputManager &input, SDL_Event &e) {
  input.processInputSDL(e);
}
