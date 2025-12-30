#include "Model/model.h"
#include "Render/GL_render.h"
#include "Render/shader.h"
#include "camera.h"
#include "glad/glad.h"
#include <SDL3/SDL_events.h>

int main(int argc, char *argv[]) {
  Render render(800, 600, "Sosat");
  Camera camera(800, 600, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0, 0.0, 1.f));
  Shader shader("shaders/defaultVert.glsl", "shaders/phong.glsl");
  Model model("models/bw/scene.gltf", glm::vec3(0.0, -1, 2));
  Model model1("models/dragon/scene.gltf", glm::vec3(1, -0.7, 2));
  Model model2("models/f/scene.gltf", glm::vec3(0.0, -1, 2));
  Model model3("models/penger/penger-no-hull.obj", glm::vec3(-1, -1, 2));
  model1.Scale(glm::vec3(4));
  model.Scale(glm::vec3(0.01));
  model2.Scale(glm::vec3(2));

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
        shader.HotReload("shaders/defaultVert.glsl", "shaders/phong.glsl");
      }
    }
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.Activate();
    camera.updateCam(shader.GetId());
    model1.Rotate(glm::vec3(0, angle, 0));
    model2.Rotate(glm::vec3(0, angle, 0));
    model3.Rotate(glm::vec3(0, angle, 0));
    angle++;
    model3.Draw(shader);
    model.Draw(shader);
    model1.Draw(shader);
    model2.Draw(shader);

    SDL_GL_SwapWindow(render.GetWindowID());
  }
  return 0;
}
