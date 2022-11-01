#ifndef VFD_DISPLAY_H
#define VFD_DISPLAY_H

#include "Arduino.h"
#include "wifi_adapter.h"
#include "gpio_conf.h"
#include "math.h"

#define SEG_A_W				2
#define SEG_B				14
#define SEG_C_S				13
#define SEG_D				12
#define SEG_E				1
#define SEG_F				3
#define SEG_G_M				15
#define SEG_P				11

#define GRID_01				22
#define GRID_02				19
#define GRID_03				24
#define GRID_04				23
#define GRID_05				25
#define GRID_06				20
#define GRID_07				21
#define GRID_08				8
#define GRID_09				7
#define GRID_10				6
#define GRID_11				5
#define GRID_12				4
#define GRID_13				9
#define GRID_14				10

#define VFD_DIGIT_COUNT		14

#define VFD_DOT_INDEX		10
#define VFD_MINUS_INDEX		11
#define VFD_PIPE_INDEX		12

#define GET_BIT_VALUE(binary, pos) (((binary >> pos) & 1) == 1 ? 1 : 0)

//  AAA
// F   B
// F   B
//  GGG
// E   C
// E   C
//  DDD  PP
const uint32_t VFD_segmentMap[13] = {
	//GBCDP0000000FAE0        A B C D E F G
	0b0111000000001110, // 0: A B C D E F
	0b0110000000000000, // 1:   B C
	0b1101000000000110, // 2: A B   D E   G
	0b1111000000000100, // 3: A B C D     G
	0b1110000000001000, // 4:   B C     F G
	0b1011000000001100, // 5: A   C D   F G
	0b1011000000001110, // 6: A   C D E F G
	0b0110000000000100, // 7: A B C
	0b1111000000001110, // 8: A B C D E F G
	0b1111000000001100, // 9: A B C D   F G
	0b0000000000000100, // .: A
	0b1000000000000000, // -:             G
	0b0000100000000000, // |:     C
};

const uint8_t VFD_digitMap[14] = {
	GRID_01, GRID_02, GRID_03, GRID_04, GRID_05, GRID_06, GRID_07, GRID_08, GRID_09, GRID_10, GRID_11, GRID_12, GRID_13, GRID_14
};

void VFD_init();
void VFD_setDigit(uint8_t digit, int8_t value);
int8_t VFD_getDigit(uint8_t digit);
void VFD_setDots(uint16_t dots);
void VFD_setDot(uint8_t digit, int8_t hasDot);
void VFD_showBtcRate(BTC_rate *rate);
void VFD_refreshDisplayTask(void * pvParameters);

#endif