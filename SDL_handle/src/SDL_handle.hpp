#ifndef SDL_HANDLE_HPP_
#define SDL_HANDLE_HPP_
#include "SDL.h"
#include <list>
#include <utility>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <unistd.h>

class SDL_Handle
{
private:
	std::vector<std::thread> thread_pull;
  int16_t m_wtf = 0;
  bool m_exit_f = 0;
  bool m_alarm_f = 0;
  bool m_something_f = 0;
	SDL_Event m_event;
  std::vector<SDL_Joystick*> m_joy_ptr_vctr;
  std::vector<std::pair<uint8_t, uint8_t>> m_indexes_vctr;
  std::vector<std::vector<int16_t>> m_initialState_vctr;
  std::vector<std::vector<int16_t>> m_axesValues_vctr;
  std::vector<std::vector<int16_t>> m_buttonsValues_vctr;

  void m_sync(uint8_t);
  void m_handleEvents(SDL_Event &);
  void m_printControls();
  void m_desync(uint8_t);
  void m_spin();
  uint8_t m_getNormalId(uint8_t);

public:
  SDL_Handle();
  ~SDL_Handle();
};
#endif
