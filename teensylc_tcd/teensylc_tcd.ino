#include "img_sensor.h"

// Teensy LC has the LED on pin 13
#define LED 13
#define LED_ON() digitalWriteFast(LED,HIGH)
#define LED_OFF() digitalWriteFast(LED,LOW)

#include <SoftwareSerial.h>
#define RX1 0
#define TX1 1
SoftwareSerial Serial7Segment(RX1, TX1); //RX pin, TX pin

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

    attachInterrupt(B_LEFT, led_off, RISING);
    attachInterrupt(B_RIGHT, led_on, RISING);

    // configure pwm pins 2,3,4 to set frequency
    analogWriteFrequency(pwm_out, master_clock_freq);
    // configure pwm resolution to allow up to 1.5 MHz
    analogWriteResolution(5);

    // configure ICG, SH states, start master clock
    initialize_sensor();

    // begin serial
    Serial.begin(9600);

    //Talk to the Serial7Segment at 9600 bps
    Serial7Segment.begin(9600);
    // clear display
    Serial7Segment.write(0x76);
    // Brightness control command
    Serial7Segment.write(0x7A);
    Serial7Segment.write((byte) 120);  // 0 is dimmest, 255 is brightest
}


void led_on()
{
    cli();
    LED_ON();
    sei();
}

void led_off()
{
    cli();
    LED_OFF();
    sei();
}

float i = 0;
// Main Loop
void loop() {
    capture();

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

