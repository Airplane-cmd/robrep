#include "SDL.h"
#include <list>
#include <utility>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
void halt();
void sync();
std::shared_ptr<std::vector<int16_t>> getRightVector(std::list<std::pair<std::vector<int16_t>, uint8_t>>, uint8_t);
std::list<SDL_Joystick*> joy_ptr_lst; 
int16_t wtf = 0;
uint8_t count = 0;
bool curseSet = 0;//fuck you 

std::list<std::pair<std::vector<int16_t>, uint8_t>> initialState_vctr;
std::list<std::pair<std::vector<int16_t>, uint8_t>> axesValues_vctr;
std::list<std::pair<std::vector<int16_t>, uint8_t>> buttonsValues_vctr;
 
int main(int argc, char **argv)
{
  
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);
  SDL_Event event;
  bool exit = 0;
  uint8_t cursedAxesCount = 0;
  halt();
  sync();
  if(SDL_NumJoysticks() > 0) 
  {
      if(!exit)
      {
	while(SDL_PollEvent(&event) >= 0)
	{
	  //	  std::cout << "wtf?";
	  usleep(5000);
	  std::system("clear"); 
	  switch(event.type)
	  {
	    uint8_t id;
	    case(SDL_QUIT):
	      exit = 1;
	      break;
	    case(SDL_JOYAXISMOTION):
	      id = event.jaxis.which;
	      getRightVector(axesValues_vctr, id)->at(event.jaxis.axis) = event.jaxis.value - getRightVector(initialState_vctr, id)->at(event.jaxis.axis);
	      break;
	    case(SDL_JOYBUTTONUP):
	      id = event.button.which;
	      getRightVector(buttonsValues_vctr, id)->at(event.jbutton.button) = 0;
	      break;
	    case(SDL_JOYBUTTONDOWN):
	      id = event.button.which;
	      getRightVector(buttonsValues_vctr, id)->at(event.jbutton.button) = 1;
	      break;
	    case(SDL_JOYDEVICEREMOVED):
	      id = event.jdevice.which;
	      SDL_Joystick* ptr;
	      for(auto it : joy_ptr_lst)
	      {
		if(SDL_JoystickInstanceID(it) == id)	
		{
		  ptr = it;
		  break;
		}
	      }
	      if(SDL_JoystickGetAttached(ptr))	SDL_JoystickClose(ptr);
	      halt();
	      sync();
	      break;
	    dafault:
	      std::cout <<"wtf?\n";
	      break;
	  }
	  if(exit) break;

    	for(auto it : joy_ptr_lst)
	{
	  uint8_t id = SDL_JoystickInstanceID(it);
	  printf("[+] Opened Joystick %d\n", id);
	  printf("Name: %s\n", SDL_JoystickNameForIndex(0));
	  printf("Number of Axes: %d\n", SDL_JoystickNumAxes(it));
	  printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(it));
	  printf("Number of Balls: %d\n", SDL_JoystickNumBalls(it));

	 	  std::cout << "Buttons: \n";
	  for(uint8_t i = 0; i < SDL_JoystickNumButtons(it); ++i)
	  {
	    if(SDL_JoystickGetButton(it, int(i)) != 0 || 1)
	    {
	      std::cout << int(i) << ": ";
	      std::cout << (int)getRightVector(buttonsValues_vctr, id)->at(i);
	      std::cout << "\n";
	    }
	  }
	  std::cout << '\n';
	  std::cout << "Axes: \n";
	  for(uint8_t i = 0; i < SDL_JoystickNumAxes(it); ++i)
	  {
	    if(SDL_JoystickGetAxis(it, int(i)) != 0 || 1)
	    {
	      std::cout << int(i) << ": ";
	      std::cout << (int)getRightVector(axesValues_vctr, id)->at(i);
	      std::cout << "\n";
	    }
	  }	
	}
      }
    }

    }

  else 
  {
    printf("[-] Joystick is not detected\n");
    return -2;
  }
  return 0;
}
void halt()
{
 for(;;)
  {
    SDL_JoystickUpdate();
    count = SDL_NumJoysticks();
    if(count > 0)
    {
      std::cout << "[+] Joytick found\n";
      break;
    }
    else std::cout << "[-] Joystick is not connected\n";
    sleep(1);
//    break;
  }
}
void sync()
{
  for(uint8_t i = 0; i < count; ++i)	joy_ptr_lst.push_back(SDL_JoystickOpen(i));
  for(SDL_Joystick *it : joy_ptr_lst)
  {
    if(it) 
    {
      uint8_t id = SDL_JoystickInstanceID(it);
      printf("[+] Opened Joystick %d\n", id);
      printf("Name: %s\n", SDL_JoystickNameForIndex(0));
      printf("Number of Axes: %d\n", SDL_JoystickNumAxes(it));
      printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(it));
      printf("Number of Balls: %d\n", SDL_JoystickNumBalls(it));
      std::cout << "Axes initial states: \n";
//      std::pair<std::vector<int16_t>, uint8_t> pair;
//      pair.first = std::vector<int16_t>(SDL_JoystickNumAxes(it));
//      pair.second = id;
//      initialState_vctr.push_back(pair);
//      pair.fist =
      initialState_vctr.push_back(std::pair<std::vector<int16_t>, uint8_t>(std::vector<int16_t>(SDL_JoystickNumAxes(it)), id));

      axesValues_vctr.push_back(std::pair<std::vector<int16_t>, uint8_t>(std::vector<int16_t>(SDL_JoystickNumAxes(it)), id));
      buttonsValues_vctr.push_back(std::pair<std::vector<int16_t>, uint8_t>(std::vector<int16_t>(SDL_JoystickNumButtons(it)), id));


      for(uint8_t i = 0; i < SDL_JoystickNumAxes(it); ++i)
      {
	if(SDL_JoystickGetAxis(it, int(i)) != 0 || 1)
	{
	  std::cout << int(i) << ": ";
	  bool cursedAxis = SDL_JoystickGetAxisInitialState(it, i, &wtf);
	  if(!curseSet || 1)	getRightVector(initialState_vctr, id)->push_back(wtf);
	  std::cout << cursedAxis << " ";
	  if(cursedAxis)
	  {
	    std::cout << wtf;
	  }
	  std::cout << "\n";
	}
      }
      getRightVector(axesValues_vctr, id)->resize(SDL_JoystickNumAxes(it), 0);
      getRightVector(buttonsValues_vctr, id)->resize(SDL_JoystickNumButtons(it), 0);
    }
  }
}
std::shared_ptr<std::vector<int16_t>> getRightVector(std::list<std::pair<std::vector<int16_t>, uint8_t>> struct_, uint8_t id)
{
  for(auto it : struct_)
  {
    if(it.second == id)	return std::make_shared<std::vector<int16_t>>(it.first);
  }
}
