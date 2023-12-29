/*
 * global.h
 *
 *  Created on: Dec 29, 2023
 *      Author: quoct
 */

#pragma once
#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "sensor.h"
#include "buzzer.h"
#include "uart.h"

#define OFF 0
#define ON 1
#define INIT 0
#define NORMAL 1
#define WARNING 2

extern float get_Voltage, get_Temperature, get_Light, get_Current, get_Power, get_Humid;
extern uint16_t get_Potentiometer;
extern uint8_t count_adc;
extern uint8_t count_buzzer;
extern uint8_t status;
extern uint8_t status_buzzer;

void initFunc();
void get_Adc();
void display_Adc();
void fsm_Buzzer();
void updateTime();
void time_and_comms();

#endif /* INC_GLOBAL_H_ */
