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

char lcd_print[50];


// calibration parameters
float  volts_per_div;
float  amps_per_div;
uint16_t volts_per_div_uint16_t;
uint16_t amps_per_div_uint16_t;
uint16_t zero_volts;
uint16_t zero_amps;
float  voltage_result;
float  current_result;
float  power_result;

uint16_t  test_v;
float test_vf;
float volts_per_div_2;
float amps_per_div_2;

//current readings
__IO uint16_t voltage_reading;
__IO uint16_t current_reading;

// attemp to convert between float and uint16_t, not working
uint16_t FloatToUint(float n) {
  return (uint16_t)(*(uint16_t*)&n);
}

float UintToFloat(uint16_t n){
  return (float)(*(float*)&n);
}


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
  eeprom_read(10, &zero_volts);
  volts_per_div = CAL_VOLTS / ((float) voltage_reading - (float) zero_volts);
  //The following line is for testing 'FloatToUint()' function. Not working as intented.
  volts_per_div_uint16_t = FloatToUint(volts_per_div);

  //Store values in EEPROM
  eeprom_write(30, volts_per_div_uint16_t);
}


/**
 * @brief Updates calibration for the standard current
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_current() {  
  //Code to calculate amps_per_div
  eeprom_init();
  eeprom_read(20, &zero_amps);
  amps_per_div = CAL_CURR / ((float) current_reading - (float) zero_amps);
  amps_per_div_uint16_t = amps_per_div;

  //Store values in EEPROM
  eeprom_write(40, amps_per_div_uint16_t);
}


/**
 * @brief Initialize the energy meter
 * @details Reads the calibration values from the EEPROM
 */
void meter_init() {  
  //Read in calibration constants from EEPROM

  eeprom_init();

  //volts_per_div = .0025;
  //amps_per_div = .001;

  eeprom_read(10, &zero_volts);
  eeprom_read(20, &zero_amps);
  eeprom_read(30, &volts_per_div_uint16_t);
  eeprom_read(40, &amps_per_div_uint16_t);  
}


/**
 * @brief Displays energy meter data
 * @details Replace with code to update the display with
 * your own
 */
void meter_display() {  
  //Code here

  //eeprom_init();

  /*
  eeprom_read(0, &zero_volts);
  eeprom_read(2, &zero_amps);
  eeprom_read(4, &volts_per_div_uint16_t);
  eeprom_read(8, &amps_per_div_uint16_t);  
  */
  /*
  volts_per_div = (float) volts_per_div_uint16_t;
  amps_per_div = (float) amps_per_div_uint16_t;
  */
  /*
  voltage_result = ((float) voltage_reading - (float) zero_volts) * volts_per_div;
  current_result = ((float) current_reading - (float) zero_amps) * amps_per_div;
  power_result = voltage_result * current_result;
  */

  // test
  //eeprom_read(10, &test_v);
  //test_vf = (test_v << 5);

  lcd_clear();
  lcd_goto(0, 0);
  snprintf(lcd_print, 50, "Power: %.5f, V: %.5f, I: %.5f", power_result, voltage_result, current_result);
  //snprintf(lcd_print, 50, "Vpd_16: %05d, Vpd: %.5f, vpd2: %.5f", volts_per_div_uint16_t,volts_per_div,volts_per_div_2);
  //snprintf(lcd_print, 50, "V0_16: %05d, Vreading: %05d", zero_volts,voltage_reading);
  //snprintf(lcd_print, 50, "test_vf: %.5f, testV: %05d, vpd2: %.5f", test_vf,test_v,volts_per_div_2);
  lcd_puts(lcd_print);
}


/**
 * @brief Callback at end of ADC conversion
 * @details Called at the end of the ADC conversions
 */
void my_adc_callback(uint16_t *data) {
  //eeprom_init();
  voltage_reading = (uint16_t) data[0];
  current_reading = (uint16_t) data[1];
  
  eeprom_read(10, &zero_volts);
  eeprom_read(20, &zero_amps);
  eeprom_read(30, &volts_per_div_uint16_t);
  eeprom_read(40, &amps_per_div_uint16_t);  
  
  // Not working as intended
  volts_per_div = (float) volts_per_div_uint16_t;
  amps_per_div = (float) amps_per_div_uint16_t;
 
  voltage_result = ((float) voltage_reading - (float) zero_volts) * volts_per_div;
  current_result = ((float) current_reading - (float) zero_amps) * amps_per_div;

  /* Test eeprom read and write function: by comparing 'volts_per_div' (before storing) and 'volts_per_div_2'(read from memory)

  volts_per_div_2 = UintToFloat(volts_per_div_uint16_t);
  amps_per_div_2 = (float) amps_per_div_uint16_t / 10000000.0;
  
  voltage_result = ((float) voltage_reading - (float) zero_volts) * volts_per_div_2;
  current_result = ((float) current_reading - (float) zero_amps) * amps_per_div_2;
  */

  power_result = voltage_result * current_result;
  
}
