#include "img_sensor.h"

IntervalTimer sh_up, sh_down;

void initialize_sensor() {
	// stop interval timers
	SH_HIGH_STOP();
	SH_DOWN_STOP();
	// set initial ICG/SH conditions
	ICG_HIGH();
	SH_LOW();
	analogWrite(pwm_out, 15); // start PWM at 50% duty cycle
}

void handshake() {
	uint8_t t1_pulse = T1_PULSE_COUNT;
	uint16_t t3_pulse_length = SH_LOW_TIMER - SH_HIGH_TIMER;

	wait_for_rising_edge();
	ICG_LOW(); // t2 start

	// digital write used since it takes approx. 830 ns.
	digitalWrite(SH, HIGH); //  t3 start
	sh_down.begin(sh_first_pulse_down, t3_pulse_length);
	
	// wait for some pulses before starting measurement
	wait_clock_rising_counts(t1_pulse);
}

void capture() {
	handshake();

	// start measurements
	wait_for_rising_edge();
	ICG_HIGH();

	// element readout time
	wait_while_reading(1546); // 1546 elements

	// reset conditions
	initialize_sensor();
	// last integration time should be slightly longer to allow resynchronization
	wait_clock_rising_counts(SYNC_PULSE_COUNT);
}

void wait_for_rising_edge() {
	while (digitalReadFast(pwm_in) == LOW) {}
}

void wait_for_falling_edge() {
	while (digitalReadFast(pwm_in) == HIGH) {}
}

// Wait for a certain number of rising edges. Waits for period T = 1/F
void wait_clock_rising_counts(int n) {
	for (uint16_t c = 0; c < n; c++) {
		// wait one period
		wait_for_rising_edge();
		wait_for_falling_edge();
	}
}

int wait_while_reading(int n) {
	for (uint16_t c = 0; c < n; c++) {
		// read value
		// int value = analogRead(OS);
		// Serial.println(value);
		// wait two periods
		wait_clock_rising_counts(2);
	}
}

/* ISRs */
void sh_first_pulse_down() {
	SH_LOW();
	SH_HIGH_STOP();
	SH_HIGH_START();
	SH_LOW_START();
}

void sh_up_wrapper() {
	SH_HIGH();
	SH_HIGH_STOP();
}

void sh_down_wrapper() {
	SH_LOW();
	SH_HIGH_START();
}