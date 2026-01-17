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

  InputManager();

  void Update();

  bool isKeyPressed(int keycode);
  bool isKeyJustPressed(int keycode);
  bool isKeyReleased(int keycode);

  glm::vec2 getMousePosition();
  glm::vec2 getMouseMotion() { return m_mouseRelMotion; }
  glm::vec2 getMouseDelta() { return m_mouseDelta; }
  float getMouseScroll();

  void bindKey(int keycode, const std::string &actionName, bool longPress,
               std::function<void()> func);
  bool isActionPressed(const std::string &action);
  int getActionKey(const std::string &action);
  std::string getKeyAction(int key);

  void onSDLEvent(SDL_Event &event);
  void ProcessInput();

private:
  struct KeyData {
    KeyState state;
    bool pressedLastFrame;
    bool longPress;
  };

  std::unordered_map<int, KeyData> m_keyboardState;
  std::unordered_map<int, KeyData> m_mouseButtonsState;

  Utils::BiMap<int, std::string> m_keyMaps;
  std::unordered_map<std::string, std::function<void()>> m_actionMaps;

  glm::vec2 m_mousePosition;
  glm::vec2 m_mouseRelMotion;
  glm::vec2 m_lastMousePosition;
  glm::vec2 m_mouseDelta;
  float m_mouseScroll;
  bool m_firstMouseMove;

  void onKeyEvent(int keycode, bool pressed);
  void onMouseMove(float x, float y);
  void onMouseMoveRel(float xrel, float yrel);
  void onMouseScroll(float y);
  void execAction(const std::string &action);
};
