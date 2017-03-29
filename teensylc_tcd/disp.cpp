#include "disp.h"

SoftwareSerial S7Seg(RX1, TX1); //RX pin, TX pin
uint16_t update_count = 0;

void initialize_disp() {
	//Talk to the Serial7Segment at 9600 bps
	S7Seg.begin(9600);
	// clear display
	CLEAR_DISP();
	// Brightness control command
	S7Seg.write(0x7A);
	S7Seg.write((unsigned char) 200);  // 0 is dimmest, 255 is brightest
}

void num2disp(uint16_t num) {
	if (num < 100) {
		S7Seg.write("  ");
		S7Seg.print(num);
	} else if (num < 1000) {
		S7Seg.write(" ");
		S7Seg.print(num);
	}  else {
		S7Seg.print(num);
	}
}

void update_disp(uint16_t num) {
	if (update_count++ >= REFRESH_RATE) {
		update_count = 0;
		CLEAR_DISP();
		num2disp(num);
	}
}

float index2mm(uint16_t num){
	return (SLOPE * (float) num) + OFFSET;
}