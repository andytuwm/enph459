#ifndef IMG_SENSOR_H
#define IMG_SENSOR_H

#include <Arduino.h>

/* TCD Pins */

// PWM frequency
#define master_clock_freq 400000 // 2500 ns per period (400 kHz)

// pwm pin
#define pwm_out 3
// pin measuring pwm
#define pwm_in 4
// integration clear gate and shift
#define ICG 5
#define SH 6

// ADC pins
#define MSB 14
#define B7 15
#define B6 16
#define B5 17
#define B4 18
#define B3 19
#define B2 20
#define LSB 21

// Buttons
#define B_LEFT 22
#define B_RIGHT 23

// SH_LOW_TIMER determines total exposure time
// No shutter exposure time ~ 7500us
// 80us EXPOSURE 50 80, t1pulse 18, syncpulse 25
// 100us EXPOSURE 70 100, t1pulse 20, syncpulse 30
// 120us EXPOSURE 80 120, with 25 t1pulse, 30 end wait pulse
// 200us EXPOSURE  150 200, t1pulse 35, syncpulse 50
#define SH_HIGH_TIMER 50
#define SH_LOW_TIMER 80
#define T1_PULSE_COUNT 18
#define SYNC_PULSE_COUNT 23

#define AVG_APLHA 0.1 //prev value 0.07

#define ICG_HIGH() digitalWriteFast(ICG,HIGH)
#define ICG_LOW() digitalWriteFast(ICG,LOW)
#define SH_HIGH() digitalWriteFast(SH,HIGH)
#define SH_LOW() digitalWriteFast(SH,LOW)

extern IntervalTimer sh_up;
extern IntervalTimer sh_down;

void initialize_sensor();
void handshake();
uint16_t capture();
void wait_for_rising_edge();
void wait_for_falling_edge();
void wait_clock_rising_counts(int n);
uint16_t wait_while_reading(int n);
void sh_first_pulse_down();
void sh_up_wrapper();
void sh_down_wrapper();

#define SH_HIGH_START() sh_up.begin(sh_up_wrapper, SH_HIGH_TIMER)
#define SH_HIGH_STOP() sh_up.end()
#define SH_LOW_START() sh_down.begin(sh_down_wrapper, SH_LOW_TIMER)
#define SH_DOWN_STOP() sh_down.end()

#endif
