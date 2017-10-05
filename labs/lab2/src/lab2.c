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
#include "ge_system.h"
#include "ge_adc.h"


// calibration parameters
float volts_per_div;
float amps_per_div;
uint16_t zero_volts;
uint16_t zero_amps;

//current readings
__IO uint16_t voltage_reading;
__IO uint16_t current_reading;

//ADC callbacks
void calculate_power(uint16_t *data);

float power_result;

//initialize ADCs
ge_init();
adc_set_fs(10000);
adc_callback(&calculate_power);
//enable ADC channels
adc_enable_channels([GE_A1, GE_A3], 2);
adc_initialize_channels


/**
 * @brief Calibrate out the 0V and 0A offsets
 * @details Reads the ADC value at 0V and 0A and records
 * their offsets in the EEPROM
 */
void calibrate_offset() {
  zero_volts = voltage_reading;
  zero_amps = current_reading;

  //Store values in EEPROM

}


/**
 * @brief Updates calibration for the standard voltage
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_voltage() {
  //Code to calculate volts_per_div

  //Store values in EEPROM
}


/**
 * @brief Updates calibration for the standard current
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_current() {
  //Code to calculate amps_per_div

  //Store values in EEPROM
}


/**
 * @brief Initialize the energy meter
 * @details Reads the calibration values from the EEPROM
 */
void meter_init() {
  //Read in calibration constants from EEPROM

  
}


/**
 * @brief Displays energy meter data
 * @details Replace with code to update the display with
 * your own
 */
void meter_display() {
  //Code here
}


/**
 * @brief Callback at end of ADC conversion
 * @details Called at the end of the ADC conversions
 */
void my_adc_callback(uint16_t *data) {
  voltage_reading = (uint16_t) data[0];
  current_reading = (uint16_t) data[1];
}
