#include "img_sensor.h"
#include "disp.h"

// Teensy LC has the LED on pin 13
#define LED 13
#define LED_ON() digitalWriteFast(LED,HIGH)
#define LED_OFF() digitalWriteFast(LED,LOW)

volatile bool in_measurement = false;
volatile bool calibrating = false;
volatile uint16_t last_measurement = 0;

// Setup
void setup() {
    // initialize digital pins
    pinMode(pwm_out, OUTPUT);
    pinMode(pwm_in, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(ICG, OUTPUT);
    pinMode(SH, OUTPUT);
    pinMode(ADC_SWITCH, OUTPUT);
    pinMode(LSB, INPUT);
    pinMode(B2, INPUT);
    pinMode(B3, INPUT);
    pinMode(B4, INPUT);
    pinMode(B5, INPUT);
    pinMode(B6, INPUT);
    pinMode(B7, INPUT);
    pinMode(MSB, INPUT);
    pinMode(B_LEFT, INPUT);
    pinMode(B_RIGHT, INPUT);

    attachInterrupt(B_LEFT, start_measurement, RISING);
    attachInterrupt(B_RIGHT, end_measurement, RISING);

    // configure pwm pins 2,3,4 to set frequency
    analogWriteFrequency(pwm_out, master_clock_freq);
    // configure pwm resolution to allow up to 1.5 MHz
    analogWriteResolution(5);

    // ensure ADC is off when unused
    digitalWriteFast(ADC_SWITCH, HIGH);
    // configure ICG, SH states, start master clock
    initialize_sensor();
    // intialize and configure 7 seg display
    initialize_disp();

    // begin usb serial
    Serial.begin(9600);
}


void start_measurement()
{
    cli();
    LED_ON();
    if (in_measurement == false) {
        calibrating = true;
        // turn on ADC
        digitalWriteFast(ADC_SWITCH, LOW);
    }
    sei();
}

void end_measurement()
{
    cli();
    LED_OFF();
    // turn off ADC
    digitalWriteFast(ADC_SWITCH, HIGH);
    in_measurement = false;
    update_disp(last_measurement);
    sei();
}

// Main Loop
void loop() {

    // calibration step before making measurements
    if (calibrating == true) {
        initial = calibration();
        in_measurement = true;
        calibrating = false;

        Serial.print("CALIBRATED ");
        Serial.println(initial);
    }

    if (in_measurement == true) {
        last_measurement = capture();
    }
}

