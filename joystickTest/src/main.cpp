#include "SDL.h"
#include <list>
#include <utility>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
void sync(uint8_t);
void handleEvents(SDL_Event);
void printControls();
void desync(uint8_t);
void spin(SDL_Event);
std::shared_ptr<std::vector<int16_t>> getRightVector(std::list<std::pair<std::vector<int16_t>, uint8_t>>, uint8_t);
std::vector<SDL_Joystick*> joy_ptr_vctr; 
int16_t wtf = 0;
uint8_t count = 0;
bool curseSet = 0;//fuck you 
//SDL_Event event;
int8_t commandPrint_g = -1;
int8_t idPrint_g = -1;
bool alarm_f = 0;
bool something_f = 0;
bool stupidCunt_f = 0;
bool stupidCunt2_f = 0;
bool exit_f = 0;
std::vector<std::vector<int16_t>> initialState_vctr;
std::vector<std::vector<int16_t>> axesValues_vctr;
std::vector<std::vector<int16_t>> buttonsValues_vctr;

int main(int argc, char **argv)
{
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);
  SDL_Event event;
  spin(event);
  return 0;
}

void sync(uint8_t id)
{
  SDL_Joystick *it = SDL_JoystickOpen(id);

  joy_ptr_vctr.push_back(it);
  initialState_vctr.push_back(std::vector<int16_t>());
  axesValues_vctr.push_back(std::vector<int16_t>());
  buttonsValues_vctr.push_back(std::vector<int16_t>());
  axesValues_vctr[id].resize(SDL_JoystickNumAxes(it));
  buttonsValues_vctr[id].resize(SDL_JoystickNumButtons(it));
  initialState_vctr[id].resize(SDL_JoystickNumAxes(it));


//  getRightVector(axesValues_vctr, id)->resize(SDL_JoystickNumAxes(joy_ptr), 0);
//  getRightVector(buttonsValues_vctr, id)->resize(SDL_JoystickNumButtons(joy_ptr), 0);
}

std::shared_ptr<std::vector<int16_t>> getRightVector(std::list<std::pair<std::vector<int16_t>, uint8_t>> struct_, uint8_t id)
{
  for(auto it : struct_)
  {
    std::cout << "id: " << uint16_t(it.second) << '\n';
    if(it.second == id)
    {
      std::cout << "nice?\n";
      return std::make_shared<std::vector<int16_t>>(it.first);
    }
  }
  std::cout << "not nice\n";
  return std::make_shared<std::vector<int16_t>>(struct_.front().first);
}

void desync(uint8_t id)
{
  joy_ptr_vctr.erase(joy_ptr_vctr.begin() + id );
  axesValues_vctr.erase(axesValues_vctr.begin() + id);
  buttonsValues_vctr.erase(buttonsValues_vctr.begin() + id);



}

void spin(SDL_Event event)
{
//    SDL_JoystickUpdate();
//    std::cout << "starting iteration in spin()\n";
  for(;;)
  {
    if(exit_f)	return;
    usleep(5000);
    handleEvents(event);
    printControls();
  }
}
void handleEvents(SDL_Event event)
{
//  SDL_JoystickUpdate();
//  std::cout << "Handling events?\n";
//  SDL_Event event;
while(SDL_PollEvent(&event) > 0)
{
  switch(event.type)
  {
    uint8_t id;
    case(SDL_JOYDEVICEADDED):
      id = event.jdevice.which;//print command if received
      commandPrint_g = 1;
      idPrint_g = id;
      alarm_f = 1;
      something_f = 1;
      std::cout << "[!] Joystick detected: " << id << '\n';
      sync(id);
      break;
    case(SDL_QUIT):
      std::cout << "[!] SDL_QUIT received: \n";//for destructor
      exit_f = 1;
      break;
    case(SDL_JOYAXISMOTION):
      {
	id = event.jaxis.which;
	uint8_t index = event.jaxis.axis;
	if(axesValues_vctr.at(id).size() > index) axesValues_vctr.at(id).at(index) = event.jaxis.value;
	else	stupidCunt_f = 1;
	break;
      }
    case(SDL_JOYBUTTONUP):
      {
	
	id = event.jbutton.which;
	uint8_t index = event.jbutton.button;
	if(buttonsValues_vctr.at(id).size() > index) buttonsValues_vctr.at(id).at(index) = 0;
	else	stupidCunt_f = 1;
	break;
      }
    case(SDL_JOYBUTTONDOWN):
      {
	std::cout << "[!] Button down\n";
	id = event.jbutton.which;
	uint8_t index = event.jbutton.button;
	if(buttonsValues_vctr.at(id).size() > index) buttonsValues_vctr.at(id).at(index) = 1;
	else	stupidCunt_f = 0;
	break;
      }
   case(SDL_JOYDEVICEREMOVED):
      if(SDL_NumJoysticks() == 0)
      {
	something_f = 0;
	alarm_f = 0;
      }
      id = event.jdevice.which;
      desync(id);
      break;
    dafault:
      std::cout <<"wtf?\n";
      break;
  }
}
}
void printControls()
{
  if(!something_f && !alarm_f)
  {
    std::system("clear");
    std::cout << "[~] No joysticks attached\n";
    alarm_f = 1;
  }
  if(!something_f)  return;
  std::system("clear");
//  for(SDL_Joystick *it : joy_ptr_lst)
  for(uint8_t i = 0; i < joy_ptr_vctr.size(); ++i)
  {
    if(i)  std::cout << "\n\n";
    SDL_Joystick* it = joy_ptr_vctr.at(i);
    uint8_t id = SDL_JoystickInstanceID(it);
    printf("[+] Opened Joystick %d\n", id);
    printf("Name: %s\n", SDL_JoystickNameForIndex(i));
    printf("Number of Axes: %d\n", SDL_JoystickNumAxes(it));
    printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(it));
    printf("Number of Balls: %d\n", SDL_JoystickNumBalls(it));
    std::cout << "Size of joystick vector: " << joy_ptr_vctr.size() << '\n';
    std::cout << "Size of axes list: " << buttonsValues_vctr.at(i).size() << '\n';
//    std::cout << "Size of it's vector: " << buttonsValues_vctr.front().first.size() << '\n';
    std::cout << "Size of axes vector: " << axesValues_vctr.at(i).size() << '\n'; 
    std::cout << "Size of buttons vector: " << buttonsValues_vctr.at(i).size() << '\n'; 
//    std::cout << "Are it the same vectors: " << buttonsValues_vctr, id).get() == &(buttonsValues_vctr.front().first)) << '\n';
    if(stupidCunt_f) std::cout << "Go fuck yourself\n"; 
    if(stupidCunt2_f)	std::cout << "ok, thats something\n";
    printf("Initial states:\n");
    for(uint8_t j = 0; j < SDL_JoystickNumAxes(it); ++j)
    {
      std::cout << int(j) << ": ";
      bool cursedAxis = SDL_JoystickGetAxisInitialState(it, j, &wtf);
      initialState_vctr.at(i).at(j) = wtf;//push_back(wtf);
      std::cout << cursedAxis << " " << initialState_vctr.at(i).at(j) << '\n';
    }
    std::cout << "Buttons: \n";
    for(uint8_t j = 0; j < SDL_JoystickNumButtons(it); ++j)
    {
      if(SDL_JoystickGetButton(it, int(j)) != 0 || 1)
      {
	std::cout << int(j) << ": ";
	std::cout << buttonsValues_vctr[i][j];
	std::cout << "\n";
      }
    }
    std::cout << "Axes: \n";
    for(uint8_t j = 0; j < SDL_JoystickNumAxes(it); ++j)
    {
      if(SDL_JoystickGetAxis(it, int(j)) != 0 || 1)
      {
	std::cout << int(j) << ": ";
	std::cout << axesValues_vctr[i][j];
	std::cout << "\n";
      }
    }
  }
}
  
