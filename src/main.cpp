#include "Model/model.h"
#include "Render/GL_render.h"
#include "Render/shader.h"
#include "camera.h"
#include "glad/glad.h"
#include "skybox.h"
#include <SDL3/SDL_events.h>
#include <iostream>

int main(int argc, char *argv[]) {
  Render render(800, 600, "Sosat");
  Camera camera(800, 600, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0, 0.0, 1.f));
  Shader shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
  Shader shaderSky("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
  Model model("models/teapot.obj", glm::vec3(0.0, -0.3, 10));

  SkyBox skybox("models/skybox");

  glEnable(GL_FRAMEBUFFER_SRGB);
  SDL_Event e;
  float angle = 0;
  while (render.isRuning()) {
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        render.isRun = false;
      }
      if (e.type == SDL_EVENT_WINDOW_RESIZED) {
        int width, height;
        SDL_GetWindowSize(render.GetWindowID(), &width, &height);
        render.setScreenWidth(width);
        render.setScreenHeight(height);
        render.UpdateViewPort();
        camera.changeSize(width, height);
      }
      if (e.key.key == SDLK_R) {
        shader.HotReload("shaders/defaultVert.glsl",
                         "shaders/defaultFrag.glsl");
      }
    }
    glClearColor(0.01, 0.01, 0.01, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    shaderSky.Activate();
    camera.updateCamSkyBox(shaderSky);
    skybox.Draw(shaderSky);
    glDepthFunc(GL_LESS);

    shader.Activate(); // Model shader (ID 1)
    camera.updateCam(shader);
    model.Rotate(glm::vec3(0, angle, 0));
    angle++;
    model.Draw(shader);

    SDL_GL_SwapWindow(render.GetWindowID());
  }
  return 0;
}
