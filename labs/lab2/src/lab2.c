/**
 * @file  lab2.c
 * @brief Starter code for lab 2.
 * 
 * @details Modify this file to implement the power meter
 * for lab 2
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "lab2.h"

char lcd_print[16];


// calibration parameters
float volts_per_div;
float amps_per_div;
uint16_t zero_volts;
uint16_t zero_amps;

//current readings
__IO uint16_t voltage_reading;
__IO uint16_t current_reading;


/**
 * @brief Calibrate out the 0V and 0A offsets
 * @details Reads the ADC value at 0V and 0A and records
 * their offsets in the EEPROM
 */
void calibrate_offset() {
  zero_volts = voltage_reading;
  zero_amps = current_reading;

  //Store values in EEPROM
  eeprom_init();
  eeprom_write(0, zero_volts);
  eeprom_write(2, zero_amps);
}


/**
 * @brief Updates calibration for the standard voltage
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_voltage() {
  //Code to calculate volts_per_div
  eeprom_read(0, zero_volts);
  volts_per_div = CAL_VOLTS / (voltage_reading - zero_volts);

  //Store values in EEPROM
  eeprom_write(4, volts_per_div);
}


/**
 * @brief Updates calibration for the standard current
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_current() {
  //Code to calculate amps_per_div
  eeprom_read(2, zero_amps);
  amps_per_div = CAL_CURR / (current_reading - zero_amps);

  //Store values in EEPROM
  eeprom_write(8, amps_per_div);
}


/**
 * @brief Initialize the energy meter
 * @details Reads the calibration values from the EEPROM
 */
void meter_init() {
  //Read in calibration constants from EEPROM
  eeprom_read(0, zero_volts);
  eeprom_read(2, zero_amps);
  eeprom_read(4, volts_per_div);
  eeprom_read(8, amps_per_div);  
}


/**
 * @brief Displays energy meter data
 * @details Replace with code to update the display with
 * your own
 */
void meter_display() {
  //Code here
  voltage_result = (voltage_reading - zero_volts) * volts_per_div;
  current_result = (current_reading - zero_amps) * amps_per_div;
  power_result = voltage_result * current_result;

  lcd_clear();
  lcd_goto(0, 0);
  snprintf(lcd_print, 16, "Power: %.2f", power_result);
  lcd_puts(lcd_print);
}


/**
 * @brief Callback at end of ADC conversion
 * @details Called at the end of the ADC conversions
 */
void my_adc_callback(uint16_t *data) {
  voltage_reading = (uint16_t) data[0];
  current_reading = (uint16_t) data[1];
}
