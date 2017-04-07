#include "img_sensor.h"
#include "disp.h"

IntervalTimer sh_up, sh_down;
uint16_t running_avg = 0;
int initial = 0;

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

uint16_t capture() {
	handshake();

	// start measurements
	wait_for_rising_edge();
	ICG_HIGH();

	// element readout time
	uint16_t val = wait_while_reading(1546); // 1546 elements

	// reset conditions
	initialize_sensor();
	// last integration time should be slightly longer to allow resynchronization
	wait_clock_rising_counts(SYNC_PULSE_COUNT);
	return val;
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

uint16_t wait_while_reading(int n) {
	uint8_t num = 255;
	uint8_t b1, b2, b3, b4, b5, b6, b7, b8;
	uint8_t values[n];
	uint16_t min_i;

	for (uint16_t c = 0; c < n; c++) {
		// wait two periods

		b1 = digitalReadFast(LSB);
		b2 = digitalReadFast(B2);
		b3 = digitalReadFast(B3);
		b4 = digitalReadFast(B4);
		b5 = digitalReadFast(B5);
		b6 = digitalReadFast(B6);
		
		wait_clock_rising_counts(1);

		b7 = digitalReadFast(B7);
		b8 = digitalReadFast(MSB);

		values[c] = b8 << 7 | b7 << 6 | b6 << 5 | b5 << 4 | b4 << 3 | b3 << 2 | b2 << 1 | b1;
		if (values[c] < num) {
			num = values[c];
			min_i = c;
		}

		wait_clock_rising_counts(1);
	}

	// get saturated peak's other side's index
	int k = n;
	for (int j = min_i; j < n; j++) {
		if (values[j] - values[min_i] > 2) {
			k = j;
			break;
		}
	}
	// compute the central index
	uint16_t min_avg = (k + min_i) / 2;

	// compute running avg of peak readings
	running_avg = (AVG_APLHA * min_avg) + (1.0 - AVG_APLHA) * running_avg;
	// display peak reading
	update_disp(running_avg);

	return running_avg;
}

int calibration() {
	int val = 0;
	for (uint8_t i = 0; i < CALIBRATION_N; i++) {
		val += capture();
	}
	return val / CALIBRATION_N;
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