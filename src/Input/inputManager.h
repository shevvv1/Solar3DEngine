#pragma once

#include <SDL3/SDL_events.h>
#include <functional>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "utils.h"

class InputManager {
public:
  enum class KeyState { RELEASED, PRESSED, JUST_PRESSED, JUST_RELEASED };
  enum class MouseButton { LEFT, RIGHT, MIDDLE };

  InputManager();

  void Update();

  bool isKeyPressed(int keycode);
  bool isKeyJustPressed(int keycode);
  bool isKeyReleased(int keycode);

  bool isMouseButtonPressed(MouseButton button);
  glm::vec2 getMousePosition();
  glm::vec2 getMouseDelta();
  float getMouseScroll();

  void bindKey(int keycode, const std::string &actionName,
               std::function<void()> func);
  bool isActionPressed(const std::string &action);
  int getActionKey(const std::string &action);
  std::string getKeyAction(int key);

  void processInputSDL(SDL_Event &event);

private:
  struct KeyData {
    KeyState state;
    bool pressedLastFrame;
  };

  std::unordered_map<int, KeyData> m_keyboardState;
  std::unordered_map<int, KeyData> m_mouseButtonsState;

  Utils::BiMap<int, std::string> m_keyMaps;
  std::unordered_map<std::string, std::function<void()>> m_actionMaps;

  glm::vec2 m_mousePosition;
  glm::vec2 m_lastMousePosition;
  glm::vec2 m_mouseDelta;
  float m_mouseScroll;
  bool m_firstMouseMove;

  void onKeyEvent(int keycode, bool pressed);
  void onMouseButtonEvent(int button_code, bool pressed);
  void onMouseMove(float x, float y);
  void onMouseScroll(float y);
};
