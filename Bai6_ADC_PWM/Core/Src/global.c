/*
 * global.c
 *
 *  Created on: Dec 29, 2023
 *      Author: quoct
 */

#include "global.h"

float get_Voltage, get_Temperature, get_Light, get_Current, get_Power, get_Humid;
uint16_t get_Potentiometer;
uint8_t count_adc = 0;
uint8_t count_buzzer = 0;
uint8_t status = 0;
uint8_t status_buzzer = 0;

void initFunc()
{
    get_Voltage = 0.0;
    get_Temperature = 0.0;
    get_Light = 0.0;
    get_Current = 0.0;
    get_Power = 0.0;
    get_Humid = 0.0;
    get_Potentiometer = 0;
    count_adc = 0;
    count_buzzer = 0;
    status = INIT;
    status_buzzer = OFF;
}

void display_Adc()
{
    lcd_ShowStr(10, 100, "Voltage:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 100, get_Voltage, 4, RED, BLACK, 16);
    lcd_ShowStr(10, 120, "Current:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 120, get_Current, 5, RED, BLACK, 16);
    lcd_ShowStr(10, 140, "Power:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 140, get_Power, 5, RED, BLACK, 16);
    lcd_ShowStr(10, 160, "Light:", RED, BLACK, 16, 0);
    if (get_Light > 3500)
    {
        lcd_ShowStr(30, 160, "STRONG", RED, BLACK, 16, 0);
    }
    else
        lcd_ShowStr(30, 160, "WEAK", RED, BLACK, 16, 0);
    // lcd_ShowIntNum(130, 160, get_Light, 4, RED, BLACK, 16);
    lcd_ShowStr(10, 180, "Potentiometer:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 180, get_Potentiometer, 5, RED, BLACK, 16);
    lcd_ShowStr(10, 200, "Humidity:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 200, get_Humid, 5, RED, BLACK, 16);
    lcd_ShowStr(10, 220, "Temperature:", RED, BLACK, 16, 0);
    lcd_ShowFloatNum(130, 220, get_Temperature, 4, RED, BLACK, 16);
}

void get_Adc()
{
    count_adc = (count_adc + 1) % 100;
    if (count_adc == 0)
    {
        sensor_Read();
        get_Voltage = sensor_GetVoltage();
        get_Current = sensor_GetCurrent();
        get_Light = sensor_GetLight();
        get_Potentiometer = sensor_GetPotentiometer();
        get_Temperature = sensor_GetTemperature();
        get_Power = get_Voltage * (get_Current / 1000.0);
        get_Humid = ((float)get_Potentiometer / 4095.0) * 100;
        display_Adc();
        uart_Rs232SendString("Power: ");
        uart_Rs232SendNum(get_Power);
        uart_Rs232SendString("\n");
        uart_Rs232SendString("Luminosity: ");
        if (get_Light > 3500)
        {
            uart_Rs232SendString("STRONG");
        }
        else
            uart_Rs232SendString("WEAK");
        uart_Rs232SendString("\n");
        uart_Rs232SendString("Power: ");
        uart_Rs232SendNum(get_Power);
        uart_Rs232SendString("\n");
        uart_Rs232SendString("Temperature: ");
        uart_Rs232SendNum(get_Temperature);
        uart_Rs232SendString("\n");
        uart_Rs232SendString("Humid: ");
        uart_Rs232SendNum(get_Humid);
        uart_Rs232SendString("\n");
    }
}

void fsm_Buzzer()
{
    switch (status)
    {
    case INIT:
        status = NORMAL;
        break;
    case NORMAL:
        if (get_Humid >= 70.0)
        {
            status = WARNING;
        }
        break;
    case WARNING:
        count_buzzer = (count_buzzer + 1) % 20;
        if (count_buzzer == 0)
        {
            if (status_buzzer == ON)
            {
                status_buzzer = OFF;
                buzzer_SetVolume(0);
            }
            else
            {
                status_buzzer = ON;
                buzzer_SetVolume(75);
                uart_Rs232SendString("!WARNING#");
            }
        }
        if (get_Humid < 70)
        {
            status = NORMAL;
            count_buzzer = 0;
            buzzer_SetVolume(0);
        }
        break;
    default:
        break;
    }
}

void updateTime()
{
    ds3231_Write(ADDRESS_YEAR, 23);
    ds3231_Write(ADDRESS_MONTH, 10);
    ds3231_Write(ADDRESS_DATE, 20);
    ds3231_Write(ADDRESS_DAY, 6);
    ds3231_Write(ADDRESS_HOUR, 20);
    ds3231_Write(ADDRESS_MIN, 11);
    ds3231_Write(ADDRESS_SEC, 23);
}

void time_and_comms()
{
    ds3231_ReadTime();
    led7_SetDigit(ds3231_hours / 10, 0, 0);
    led7_SetDigit(ds3231_hours % 10, 1, 0);
    led7_SetDigit(ds3231_min / 10, 2, 0);
    led7_SetDigit(ds3231_min % 10, 3, 0);
    led7_Scan();
}
