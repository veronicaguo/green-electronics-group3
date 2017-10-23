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

char lcd_print_power[50];
char lcd_print_voltage[50];
char lcd_print_current[50];
char lcd_print_df[50];


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

// filter parameters
//float voltage_stored[50];
//float current_stored[50];
//int i;
//float error_vol;
//float error_cur;
float voltage_result_filtered;
float current_result_filtered;
float voltage_result_filtered_old;
float current_result_filtered_old;
//float error_th;


// MPPT calculation parameters
float df = INITIAL_DUTY_FACTOR;
float delta_df = DELTA_DF;
float old_df = 0;
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

  voltage_reading_previous = 2090.0;
  current_reading_previous = 2048.0;
  shift = 5.0;
}

/*
float average(float *arr){
  int i;
  float sum, avg;
  int n = 50;
  //int n = sizeof(arr)/sizeof(arr[0]);
  //loop through each element of the array
  for (i = 0; i<n; ++i){
    sum += arr[i];
  }
  avg = sum/n;
  return avg;
}
*/

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

  while (true){

  //df = df + delta_df;
    pwm_set(PWM_CHAN2, df);

  // voltage, current, power calculation and filter

    voltage_reading_filtered = voltage_reading_previous - (voltage_reading_previous >> shift) + (voltage_reading >> shift);
    voltage_result = ((float) voltage_reading_filtered - (float) zero_volts) * volts_per_div;  

    current_reading_filtered = current_reading_previous - (current_reading_previous >> shift) + (current_reading >> shift);
    current_result = ((float) current_reading_filtered - (float) zero_amps) * amps_per_div;
    
    power_result = voltage_result * current_result;

    voltage_reading_previous = voltage_reading_filtered;
    current_reading_previous = current_reading_filtered; 

  // MPPT

  //lcd_clear();
  
    delay_ms(50); 
    // Display
    lcd_goto(0, 0);
    snprintf(lcd_print_power, 50, "Power: %.5f", power_result);
    lcd_puts(lcd_print_power);
    lcd_goto(0, 1);
    snprintf(lcd_print_voltage, 50, "Voltage: %.5f", voltage_result);
    lcd_puts(lcd_print_voltage);
    lcd_goto(0, 2);
    snprintf(lcd_print_current, 50, "Current: %.5f", current_result);
    lcd_puts(lcd_print_current);
    lcd_goto(0, 3);
    snprintf(lcd_print_df, 50, "Duty: %.5f", df);
    lcd_puts(lcd_print_df);
    
    // old_power starts at 0
    // old_df = 0
    
    if (power_result > old_power){
      old_power = power_result;
      old_df = df;
      df = df + delta_df;
      if (df > 1.0) {
        df = 1.0;
      }
    }
    else {
      df = old_df;
      df = df - delta_df;
      old_power = 0;
      
      if (df < 0.05) {
        df = 0.05;
      }
      
    }
    

    if (!gpio_read_pin(GE_PBTN1)) break;
    
  }
  
  /*
  lcd_goto(0, 3);
  snprintf(lcd_print_df, 50, "Duty: %.5f", df);
  lcd_puts(lcd_print_df);
  */
  //df = df + delta_df;
  //delay_ms(200);
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
