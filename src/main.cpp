#include "Input/inputManager.h"
#include "Model/model.h"
#include "Render/GL_render.h"
#include "Render/shader.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "camera.h"
#include "glad/glad.h"
#include "skybox.h"

void inputInit(InputManager& input, Render& render, Camera& camera, Shader& shader);

int main(int argc, char* argv[]) {
  Render render(1200, 860, "UwU");
  Camera camera(800, 600, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0, 0.0, 1.f));
  Shader shader("shaders/Default/Vert.glsl", "shaders/Default/Frag.glsl");
  Shader shaderSky("shaders/Default/SkyboxVert.glsl", "shaders/Default/SkyboxFrag.glsl");
  Model model("models/teapot.obj", glm::vec3(0, 0, 0));
  SkyBox skybox("models/skybox");
  InputManager input;
  inputInit(input, render, camera, shader);

  glEnable(GL_FRAMEBUFFER_SRGB);

  float angle = 0;
  SDL_Event e;
  while (render.Run()) {
    while (SDL_PollEvent(&e)) {
      render.onEvent(&e, camera);
      input.onSDLEvent(e);
    }

    input.ProcessInput();

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
    angle += 0.1;
    model.Draw(shader);

    SDL_GL_SwapWindow(render.GetWindowID());
  }
  return 0;
}

void inputInit(InputManager& input, Render& render, Camera& camera, Shader& shader) {
  input.bindKey(SDLK_W, "camera.move.forward", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * camera.GetDirection());
  });

  input.bindKey(SDLK_S, "camera.move.back", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * (-camera.GetDirection()));
  });

  input.bindKey(SDLK_A, "camera.move.left", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * (-camera.GetRightVec()));
  });

  input.bindKey(SDLK_D, "camera.move.right", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * camera.GetRightVec());
  });

  input.bindKey(SDLK_LSHIFT, "camera.move.down", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * (-camera.GetUpVec()));
  });

  input.bindKey(SDLK_SPACE, "camera.move.up", true, [&]() {
    float delta = render.GetDeltaTime();
    float speed = 10 * delta;
    camera.offsetPosition(speed * camera.GetUpVec());
  });

  input.bindKey(SDLK_F5, "shader.hot_reload", false, [&]() { shader.HotReload(); });

  input.bindKey(SDLK_F1, "render.switch_cursor_mode", false, [&]() { render.switchCursorMode(); });

  input.bindKey(SDL_BUTTON_RIGHT, "shader.camera.move", true, [&]() {
    camera.yaw -= input.getMouseMotion().x / 2;
    camera.pitch += -input.getMouseMotion().y / 2;
  });
}
