#ifndef JOYSTICK_HPP_
#define JOYSTICK_HPP_
#include "SDL.h"
#include <stdint.h>
class JoystickData
{
private:
  SDL_Joystick* m_joy_ptr_vctr;
  std::vector<int16_t> m_axes_vctr;
  std::vector<int16_t> m_buttons_vctr;
  std::vector<int16_t> m_initialState_vctr;
  std::vector<int16_t> m_additionalValues_vctr;
  void m_updateID();
public:
  JoystickData(SDL_Joystick* ptr);
  ~JoystickData();
};
#endif
