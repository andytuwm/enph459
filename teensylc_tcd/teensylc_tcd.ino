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
    if (in_measurement == false) calibrating = true;
    sei();
}

void end_measurement()
{
    cli();
    LED_OFF();
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
        // Serial.println(last_measurement);
    }

    // Serial7Segment.write(0x76);
    // Serial7Segment.print((int)(20.68 * 100));
    // Serial7Segment.write(0x77);
    // Serial7Segment.write(0b000010);
    // delay(500);
    // Serial7Segment.write(0x76);
    // Serial.write(0x79); // Send the Move Cursor Command
    // Serial.write(0x01); // Send the data byte, with value 1
    // Serial7Segment.write(" ");
    // Serial7Segment.print((int) (2.68*100));
    // delay(500);

    // Serial.println("=================");
}

