#include "SDL_handle.hpp"
SDL_Handle::SDL_Handle()
{
  m_wtf = 0;
  m_exit_f = 0;
  m_alarm_f = 0;
  m_something_f = 0;
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);
//	m_spin();
//  SDL_Event event;
	thread_pull.push_back(std::thread(&SDL_Handle::m_spin, this));
}
SDL_Handle::~SDL_Handle()
{
	for(std::thread &it : thread_pull)	it.join();
//	for(SDL_Joystick *it : m_joy_ptr_vctr)	delete it;
}
void SDL_Handle::m_sync(uint8_t id)
{
  SDL_Joystick *it = SDL_JoystickOpen(id);
  if(it == 0) return;
  m_joy_ptr_vctr.push_back(it);
  m_initialState_vctr.push_back(std::vector<int16_t>());
  m_axesValues_vctr.push_back(std::vector<int16_t>());
  m_buttonsValues_vctr.push_back(std::vector<int16_t>());
  m_axesValues_vctr[id].resize(SDL_JoystickNumAxes(it));
  m_buttonsValues_vctr[id].resize(SDL_JoystickNumButtons(it));
  m_initialState_vctr[id].resize(SDL_JoystickNumAxes(it));
  m_indexes_vctr.push_back(std::pair<uint8_t, uint8_t>(id, SDL_JoystickInstanceID(it)));
}
void SDL_Handle::m_desync(uint8_t id)
{
  m_joy_ptr_vctr.clear();
  m_initialState_vctr.clear();
  m_axesValues_vctr.clear();
  m_buttonsValues_vctr.clear();
  m_initialState_vctr.clear();
  m_indexes_vctr.clear();
  for(uint8_t i = 0; i < 255; ++i)	m_sync(i);
}
void SDL_Handle::m_spin()
{
  for(;;)
  {
    if(m_exit_f)	return;
    usleep(5000);
    m_handleEvents(m_event);
    m_printControls();
  }
}
void SDL_Handle::m_handleEvents(SDL_Event &event)
{
  while(SDL_PollEvent(&event) > 0)
  {
    switch(event.type)
    {
      uint8_t id;
      case(SDL_JOYDEVICEADDED):
			{
				id = event.jdevice.which;//print command if received
				m_alarm_f = 1;
				m_something_f = 1;
				std::cout << "[!] Joystick detected: " << id << '\n';
				m_sync(id);
				break;
			}
      case(SDL_QUIT):
				std::cout << "[!] SDL_QUIT received: \n";//for destructor
				m_exit_f = 1;
				break;
      case(SDL_JOYAXISMOTION):
			{
	  		id = event.jaxis.which;
	  		uint8_t index = event.jaxis.axis;
	  		if(m_axesValues_vctr.at(m_getNormalId(id)).size() > index)
	  		{
	    		std::cout << "[!] Axis motion received\n";
	    		m_axesValues_vctr.at(m_getNormalId(id)).at(index) = event.jaxis.value;
	  		}
	  		break;
			}
      case(SDL_JOYBUTTONUP):
			{
	  		id = event.jbutton.which;
	  		uint8_t index = event.jbutton.button;
	  		if(m_buttonsValues_vctr.at(m_getNormalId(id)).size() > index) m_buttonsValues_vctr.at(m_getNormalId(id)).at(index) = 0;
	  		break;
			}
      case(SDL_JOYBUTTONDOWN):
			{
	  		id = event.jbutton.which;
	  		uint8_t index = event.jbutton.button;
	  		if(m_buttonsValues_vctr.at(m_getNormalId(id)).size() > index) m_buttonsValues_vctr.at(m_getNormalId(id)).at(index) = 1;
	  		break;
			}
     	case(SDL_JOYDEVICEREMOVED):
			{
				if(SDL_NumJoysticks() == 0)
				{
	  			m_something_f = 0;
	  			m_alarm_f = 0;
				}
				id = event.jdevice.which;
				m_desync(id);
				break;
			}
      dafault:
			std::cout <<"wtf?\n";
			break;
    }
  }
}
void SDL_Handle::m_printControls()
{
  if(!m_something_f && !m_alarm_f)
  {
    std::system("clear");
    std::cout << "[~] No joysticks attached\n";
    m_alarm_f = 1;
  }
  if(!m_something_f)  return;
  std::system("clear");
//  for(SDL_Joystick *it : joy_ptr_lst)
  for(uint8_t i = 0; i < m_indexes_vctr.size(); ++i)//joy_ptr_vctr.size();
  {
    if(i)  std::cout << "\n\n";
    SDL_Joystick* it = m_joy_ptr_vctr.at(i);
    uint8_t id = m_indexes_vctr[i].first;//SDL_JoystickInstanceID(it);
    printf("[+] Opened Joystick %d\n", id);
    printf("Name: %s\n", SDL_JoystickNameForIndex(i));
    printf("Number of Axes: %d\n", SDL_JoystickNumAxes(it));
    printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(it));
    printf("Number of Balls: %d\n", SDL_JoystickNumBalls(it));
    std::cout << "Size of joystick vector: " << m_joy_ptr_vctr.size() << '\n';
    std::cout << "Size of axes list: " << m_buttonsValues_vctr.at(i).size() << '\n';
//    std::cout << "Size of it's vector: " << buttonsValues_vctr.front().first.size() << '\n';
    std::cout << "Size of axes vector: " << m_axesValues_vctr.at(i).size() << '\n'; 
    std::cout << "Size of buttons vector: " << m_buttonsValues_vctr.at(i).size() << '\n';
    std::cout << "Vector of ids: [ " ;
    for(std::pair<uint8_t, uint8_t> it : m_indexes_vctr)  std::cout << (uint16_t)it.first << ", " << (uint16_t)it.second << "; ";
    std::cout << " ]\n";
//    std::cout << "Are it the same vectors: " << buttonsValues_vctr, id).get() == &(buttonsValues_vctr.front().first)) << '\n';
    printf("Initial states:\n");
    for(uint8_t j = 0; j < SDL_JoystickNumAxes(it); ++j)
    {
      std::cout << int(j) << ": ";
      bool cursedAxis = SDL_JoystickGetAxisInitialState(it, j, &m_wtf);
      m_initialState_vctr.at(i).at(j) = m_wtf;//push_back(wtf);
      std::cout << cursedAxis << " " << m_initialState_vctr.at(i).at(j) << '\n';
    }
    std::cout << "Buttons: \n";
    for(uint8_t j = 0; j < SDL_JoystickNumButtons(it); ++j)
    {
      if(SDL_JoystickGetButton(it, int(j)) != 0 || 1)
      {
	std::cout << int(j) << ": ";
	std::cout << m_buttonsValues_vctr[i][j];
	std::cout << "\n";
      }
    }
    std::cout << "Axes: \n";
    for(uint8_t j = 0; j < SDL_JoystickNumAxes(it); ++j)
    {
      if(SDL_JoystickGetAxis(it, int(j)) != 0 || 1)
      {
	std::cout << int(j) << ": ";
	std::cout << m_axesValues_vctr[i][j];
	std::cout << "\n";
      }
    }
  }
}
uint8_t SDL_Handle::m_getNormalId(uint8_t cursedId)
{
  for(std::pair<uint8_t, uint8_t> &it : m_indexes_vctr)
  {
    if(it.second == cursedId)	return it.first;
  }
}


