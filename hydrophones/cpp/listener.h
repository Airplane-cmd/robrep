#ifndef LISTENER_H
#define LISTENER_H
#include <cstdint>
uint16_t getBytesAsUint16(uint8_t first, uint8_t second);
void parseData(const char *data);
void printRawData(const char *data);
void proccessData(const char *data);
uint16_t calculateCRC(const uint8_t *data, uint16_t size);
bool checkCRC(const char *data);
uint16_t getAngele();
uint32_t getFrequency();
bool getDistance();
//void printData();
uint16_t angle;
int frequency;
bool distance; 
#endif

