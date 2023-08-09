#include "listener.h"
#include <iostream>
#include <cstdlib>
//#include <cstdint>
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
std::uint16_t getBytesAsUint16(uint8_t first, uint8_t second)
{
	return ((std::uint16_t) first << 8) | second; 
}
void parseData(const char *data)
{
//	std::system("clear");
	angle = getBytesAsUint16(data[2], data[3]);
	frequency = int(data[5]);
	distance = (uint8_t(data[4]) == 1);
	std::cout << angle << ' ' << distance << ' ' << frequency << '\n';
//	printRawData(data);
//	std::cout << "Angle:     " << angle << '\n';
//	std::cout << "Distance:  " << ((distance) ? ("under AUV") : ("Far from AUV")) << '\n';
//	std::cout << "Frequency: " << frequency << '\n';
}
uint16_t getAngle()
{
	return angle;
}
uint getFrequency()
{
	return frequency;
}
bool getDistace()
{
	return distance;
}
void printRawData(const char *data)
{
	for(int8_t i = 0; i < 9; ++i)
	{
		std::cout << int(data[i]) << ' ';//db of received row data//possible uint8_t(...)
	}
	std::cout << '\n';
}

void proccessData(const char *data)
{
	if(!checkCRC(data))
	{
		std::cout << "CRC failed\n";
		return;
	}
	parseData(data);
//	printRawData(data);

}
std::uint16_t calculateCRC(const uint8_t *data, std::uint16_t size)//char to uint?
{
	std::uint16_t crc16 = 0x8005; 
	std::uint16_t out = 0;
	int bits_read = 0;
	int bit_flag;
	if (data == 0)	return 0;
	while (size > 0) 
	{
		bit_flag = out >> 15;
		out <<= 1;
		out |= (*data >> (7 - bits_read)) & 1;
		bits_read++;
		if(bits_read > 7) 
		{
			bits_read = 0;
			data++;
			size--;
		}
		if(bit_flag)
		{
			out ^= crc16;
		}
	}
	return out;
}
bool checkCRC(const char *data)
{
	return (calculateCRC(reinterpret_cast<uint8_t *>(const_cast<char *>(data)), 7) == getBytesAsUint16(data[7], data[8])) ? 1 : 0;//const and size?
	
}
int main(int argc, char **argv) 
{

  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
	std::string comString{"/dev/ttyUSB0"} ;
	comString = ((argc == 2) ? (argv[1]) : (comString));
//	std::cout << argc << '\n';
	int serial_port = open(comString.c_str(), O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) 
  {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B57600);
  cfsetospeed(&tty, B57600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) 
  {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }



  //write(serial_port, msg, sizeof(msg));


  char read_buf[9];
  memset(&read_buf, '\0', sizeof(read_buf));
  for(;;)
  {
  	int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
  	if (num_bytes < 0) 
  	{
      		printf("Error reading: %s", strerror(errno));
      		return 1;
	}
	proccessData(read_buf);
  }
  close(serial_port);
  return 0; // success
};
