#include "inputManager.h"
#include <functional>
#include <iostream>
#include <string>

InputManager::InputManager() {
  m_mousePosition = glm::vec2(0.0f);
  m_lastMousePosition = glm::vec2(0.0f);
  m_mouseDelta = glm::vec2(0.0f);
  m_mouseScroll = 0.0f;
  m_firstMouseMove = true;
  //  m_actionMaps[""] = []() {};
}

void InputManager::Update() {
  for (auto &[key, data] : m_keyboardState) {
    if (data.state == KeyState::JUST_PRESSED) {
      data.state = KeyState::PRESSED;
    } else if (data.state == KeyState::JUST_RELEASED) {
      data.state = KeyState::RELEASED;
    }
    data.pressedLastFrame = (data.state == KeyState::PRESSED ||
                             data.state == KeyState::JUST_PRESSED);
  }

  m_mouseDelta = m_mousePosition - m_lastMousePosition;
  m_lastMousePosition = m_mousePosition;
  m_mouseScroll = 0.0f;
}

void InputManager::onKeyEvent(int keycode, bool pressed) {
  auto it = m_keyboardState.find(keycode);

  if (it == m_keyboardState.end()) {
    m_keyboardState[keycode] = {
        pressed ? KeyState::JUST_PRESSED : KeyState::JUST_RELEASED, false};
  } else {
    if (pressed && it->second.state != KeyState::PRESSED) {
      it->second.state = KeyState::JUST_PRESSED;
    } else if (!pressed && it->second.state != KeyState::RELEASED) {
      it->second.state = KeyState::JUST_RELEASED;
    }
  }
}

void InputManager::onMouseButtonEvent(int button_code, bool pressed) {
  auto it = m_mouseButtonsState.find(button_code);

  if (it == m_mouseButtonsState.end()) {
    m_mouseButtonsState[button_code] = {
        pressed ? KeyState::JUST_PRESSED : KeyState::JUST_RELEASED, false};
  } else {
    if (pressed && it->second.state != KeyState::PRESSED) {
      it->second.state = KeyState::JUST_PRESSED;
    } else if (!pressed && it->second.state != KeyState::RELEASED) {
      it->second.state = KeyState::JUST_RELEASED;
    }
  }
}

void InputManager::onMouseMove(float x, float y) {
  m_mousePosition.x = x;
  m_mousePosition.x = y;
}

void InputManager::onMouseScroll(float y) { m_mouseScroll = y; }

bool InputManager::isKeyPressed(int keycode) {
  auto it = m_keyboardState.find(keycode);
  if (it == m_keyboardState.end())
    return false;
  return it->second.state == KeyState::PRESSED ||
         it->second.state == KeyState::JUST_PRESSED;
}

bool InputManager::isKeyJustPressed(int keycode) {
  auto it = m_keyboardState.find(keycode);
  if (it == m_keyboardState.end())
    return false;
  return it->second.state == KeyState::JUST_PRESSED;
}

void InputManager::bindKey(int keycode, const std::string &actionName,
                           std::function<void()> func) {
  if (m_keyMaps.hasKey(keycode)) {
    std::cerr << "Keycode: " << keycode
              << "is already ocupied by:" << m_keyMaps.getValue(keycode)
              << std::endl;
    return;
  }
  if (m_keyMaps.hasValue(actionName)) {
    std::cerr << "Action: " << actionName
              << "is already ocupied by:" << m_keyMaps.getKey(actionName)
              << std::endl;
    return;
  }
  m_keyMaps.insert(keycode, actionName);
  m_actionMaps[actionName] = func;
}

bool InputManager::isActionPressed(const std::string &action) {
  if (m_keyMaps.hasValue(action)) {
    return isKeyPressed(m_keyMaps.getKey(action));
  }
  return false;
}

std::string InputManager::getKeyAction(int key) {
  if (m_keyMaps.hasKey(key)) {
    return m_keyMaps.getValue(key);
  }
  return "";
}

void InputManager::processInputSDL(SDL_Event &event) {
  std::string action;
  switch (event.type) {
  case SDL_EVENT_KEY_DOWN:
    onKeyEvent(event.key.key, true);
    action = getKeyAction(event.key.key);
    break;
  case SDL_EVENT_KEY_UP:
    onKeyEvent(event.key.key, false);
    action = getKeyAction(event.key.key);
    break;
  case SDL_EVENT_MOUSE_MOTION:
    onMouseMove(event.motion.x, event.motion.y);
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    onMouseButtonEvent(event.button.button, true);
    action = getKeyAction(event.button.button);
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    onMouseButtonEvent(event.button.button, false);
    action = getKeyAction(event.button.button);
    break;
  case SDL_EVENT_MOUSE_WHEEL:
    onMouseScroll(event.wheel.y);
    break;
  }
  if (isActionPressed(action)) {
    m_actionMaps[action]();
  }
  Update();
}
