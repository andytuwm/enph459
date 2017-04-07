#ifndef DISP_H
#define DISP_H

#include <SoftwareSerial.h>
#include <Arduino.h>

#define RX1 0
#define TX1 1

#define REFRESH_RATE 8
#define SLOPE 0.0388712
#define OFFSET 0.9787

extern SoftwareSerial S7Seg;

#define CLEAR_DISP() S7Seg.write(0x76)

void initialize_disp();
void num2disp(uint16_t num);
void update_disp(uint16_t num);
float index2mm(uint16_t num);
void mm2disp(float mm);
void update_disp_mm(uint16_t num);
void update(uint16_t num, int calibrated_val);

#endif