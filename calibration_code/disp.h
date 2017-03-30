#ifndef DISP_H
#define DISP_H

#include <SoftwareSerial.h>

#define RX1 0
#define TX1 1

#define REFRESH_RATE 8
#define SLOPE 1
#define OFFSET 0

extern SoftwareSerial S7Seg;

#define CLEAR_DISP() S7Seg.write(0x76)

void initialize_disp();
void num2disp(uint16_t num);
void update_disp(uint16_t num);
float index2mm(uint16_t num);

#endif