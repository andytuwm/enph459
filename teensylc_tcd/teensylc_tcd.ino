#include "img_sensor.h"

// Teensy LC has the LED on pin 13
#define LED 13
#define LED_ON() digitalWriteFast(LED,HIGH)
#define LED_OFF() digitalWriteFast(LED,LOW)

// Setup
void setup() {
    // initialize digital pins
    pinMode(pwm_out, OUTPUT);
    pinMode(pwm_in, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(ICG, OUTPUT);
    pinMode(SH, OUTPUT);

    // configure pwm pins 2,3,4 to set frequency
    analogWriteFrequency(pwm_out, master_clock_freq);
    // configure pwm resolution to allow up to 1.5 MHz
    analogWriteResolution(5);

    // configure ICG, SH states, start master clock
    initialize_sensor();

    // begin serial
    Serial.begin(9600);
}

// Main Loop
void loop() {
    // while (digitalReadFast(pwm_in) == LOW) {}
    // Serial.println(digitalReadFast(pwm_in));
    capture();
    Serial.println("=================");
}

