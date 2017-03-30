#include "img_sensor.h"
#include "disp.h"

// Teensy LC has the LED on pin 13
#define LED 13
#define LED_ON() digitalWriteFast(LED,HIGH)
#define LED_OFF() digitalWriteFast(LED,LOW)

volatile bool calibrating = false;

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
    pinMode(B_RIGHT, INPUT);

    attachInterrupt(B_RIGHT, calibrate_start, RISING);

    // configure pwm pins 2,3,4 to set frequency
    analogWriteFrequency(pwm_out, master_clock_freq);
    // configure pwm resolution to allow up to 1.5 MHz
    analogWriteResolution(5);

    // ensure ADC is off when unused
    digitalWriteFast(ADC_SWITCH, LOW);
    // configure ICG, SH states, start master clock
    initialize_sensor();
    // intialize and configure 7 seg display
    initialize_disp();

    // begin usb serial
    Serial.begin(9600);
}

void calibrate_start()
{
    cli();
    if (calibrating == false) {
        LED_ON();
        calibrating = true;
    }
    sei();
}

// Main Loop
void loop() {

    // calibrate
    if (calibrating == true) {
        initial = calibration(30);
        Serial.print("CALIBRATED ");
        Serial.println(initial);
        // stop calibration
        update_disp(initial);
        calibrating = false;
        LED_OFF();
    }
    delay(500);
}

