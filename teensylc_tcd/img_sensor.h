#ifndef IMG_SENSOR_H
#define IMG_SENSOR_H

#include <Arduino.h>	

/* TCD Pins */

// PWM frequency
#define master_clock_freq 400000 // roughly 2500 ns per period (400 kHz)

// pwm pin
#define pwm_out 3
// pin measuring pwm
#define pwm_in 7
// integration clear gate and shift
#define ICG 8
#define SH 9
// Output
#define OS 1

// SH_LOW_TIMER determines total exposure time
// No shutter exposure time ~ 7500us
// 120us EXPOSURE 80 120, with 25 t1pulse, 30 end wait pulse
// 200us EXPOSURE  150 200, t1pulse 35, syncpulse 50
#define SH_HIGH_TIMER 150
#define SH_LOW_TIMER 200
#define T1_PULSE_COUNT 35
#define SYNC_PULSE_COUNT 50

#define ICG_HIGH() digitalWriteFast(ICG,HIGH)
#define ICG_LOW() digitalWriteFast(ICG,LOW)
#define SH_HIGH() digitalWriteFast(SH,HIGH)
#define SH_LOW() digitalWriteFast(SH,LOW)

extern IntervalTimer sh_up;
extern IntervalTimer sh_down;

void initialize_sensor();
void handshake();
void capture();
void wait_for_rising_edge();
void wait_for_falling_edge();
void wait_clock_rising_counts(int n);
int wait_while_reading(int n);
void sh_first_pulse_down();
void sh_up_wrapper();
void sh_down_wrapper();

#define SH_HIGH_START() sh_up.begin(sh_up_wrapper, SH_HIGH_TIMER)
#define SH_HIGH_STOP() sh_up.end()
#define SH_LOW_START() sh_down.begin(sh_down_wrapper, SH_LOW_TIMER)
#define SH_DOWN_STOP() sh_down.end()

#endif
