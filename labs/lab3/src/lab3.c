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

#include "lab3.h"

char lcd_print[50];


// calibration parameters
float  volts_per_div;
float  amps_per_div;
uint16_t volts_per_div_uint16_t;
uint16_t amps_per_div_uint16_t;
uint16_t zero_volts;
uint16_t zero_amps;
uint16_t voltage_reading_10v;
uint16_t current_reading_3a;
float  voltage_result;
uint16_t  voltage_reading_previous;
uint16_t  voltage_reading_filtered;
float  current_result;
uint16_t  current_reading_previous;
uint16_t  current_reading_filtered;
float power_result;
int  shift;


// MPPT calculation parameters
float df = 0.5;
float delta_df = 0.01;
float old_df = 0.5;
float old_power = 0.0;


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
  eeprom_write(10, zero_volts);
  eeprom_write(20, zero_amps);
}


/**
 * @brief Updates calibration for the standard voltage
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_voltage() {  
  //Code to calculate volts_per_div
  eeprom_init();
  voltage_reading_10v = voltage_reading;
  eeprom_write(30, voltage_reading_10v);
}


/**
 * @brief Updates calibration for the standard current
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_current() {  
  //Code to calculate amps_per_div
  eeprom_init();
  current_reading_3a = current_reading;
  //Store values in EEPROM
  eeprom_write(40, current_reading_3a);
}


/**
 * @brief Initialize the energy meter
 * @details Reads the calibration values from the EEPROM
 */
void meter_init() {  
  //Read in calibration constants from EEPROM

  eeprom_init();

  eeprom_read(10, &zero_volts);
  eeprom_read(20, &zero_amps);
  eeprom_read(30, &voltage_reading_10v);
  eeprom_read(40, &current_reading_3a);   

  voltage_reading_previous = 0.0;
  current_reading_previous = 0.0;
  shift = 9.0;
}


/**
 * @brief Displays energy meter data
 * @details Replace with code to update the display with
 * your own
 */
void meter_display() {  
  //Code here

  eeprom_init();
  eeprom_read(10, &zero_volts);
  eeprom_read(20, &zero_amps);
  eeprom_read(30, &voltage_reading_10v);
  eeprom_read(40, &current_reading_3a); 

  // enable PWM library
  pwm_init();
  pwm_freq(100000.0);

  // enable pwm pins
  pwm_set_pin(PA9);
    
  volts_per_div = CAL_VOLTS / ((float) voltage_reading_10v - (float) zero_volts);
  amps_per_div = CAL_CURR / ((float) current_reading_3a - (float) zero_amps);

  while (1){
    df = df + delta_df;
    pwm_set(PWM_CHAN2, df);

    // voltage, current, power calculation and filter
    voltage_reading_filtered = (voltage_reading_previous - (voltage_reading_previous >> shift) + voltage_reading) >> 1;
    voltage_result = ((float) voltage_reading_filtered - (float) zero_volts) * volts_per_div;  
  
    current_reading_filtered = (current_reading_previous - (current_reading_previous >> shift) + current_reading) >> 1;
    current_result = ((float) current_reading_filtered - (float) zero_amps) * amps_per_div;
  
    power_result = voltage_result * current_result;

    voltage_reading_previous = voltage_reading_filtered;
    current_reading_previous = current_reading_filtered;

    if (power_result > old_power){
      old_power = power_result;
      old_df = df;
    }
    else {
      df = old_df;
      delta_df = - delta_df;
    }

    // Display
    lcd_clear();
    lcd_goto(0, 0);
    snprintf(lcd_print, 50, "Power: %.5f\nV: %.5f\nI: %.5f\nD: %.2f", power_result, voltage_result, current_result, df);
    lcd_puts(lcd_print);
  }

  
}


/**
 * @brief Callback at end of ADC conversion
 * @details Called at the end of the ADC conversions
 */
void my_adc_callback(uint16_t *data) {
  //eeprom_init();
  voltage_reading = (uint16_t) data[0];
  current_reading = (uint16_t) data[1]; 
}
