/**
 * @file  main.c
 * @brief Starter code for Lab 5
 * 
 * @details Measures the frequency of the signal on PB10 and prints it
 * to the LCD
 * 
 * @author Ned Danyliw
 * @date  10.2015
 */
#include "ge_libs.h"


int compare_function(const void* a, const void* b){
  return (*(float*)a - *(float*)b);
}


/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
  

int main(void)
{  
  //Initialize library
  ge_init();

  // Set maximum capture frequency
  ic_int_set_maxf(600.0);

  
  float setpoint = 0;
  float incre = 10;
  float duty = 0.5;
  float kp = 0.2;
  float kd = 1e-2;
  float ki = 1e-6;
  float error = 0;
  float error_prev = 0;
  float deriv = 0;
  float integ = 0;
  float dt = 1e-3;

  char lcd_print[50];
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);
  

  uint8_t filter_number = 11;

  float speed_buf[filter_number];
  float sorted_buf[filter_number];

  float filtered_speed = 0;

  uint8_t counter = 0;
  uint8_t index = 0;
  pwm_init();
  pwm_freq(100000.0);
  pwm_set_pin(PA9);

  while (1) {
    if (index>filter_number-1){
      index = 0;
    }
    float temp = ic_int_read_freq();
    speed_buf[index]=temp;
    index++;

    memcpy(sorted_buf, speed_buf, filter_number*sizeof(float)); //make copy so we can sort in place
    qsort(sorted_buf, filter_number, sizeof(float), compare_function);

    filtered_speed = sorted_buf[filter_number/2+1];


    if (counter % 20 == 0){
      counter = 0;
      char buf[10];
      //lcd_clear();
      sprintf(buf, "Current Speed: %.2f, Set Speed: %.2f, Duty: %.2f", filtered_speed, setpoint, duty);
      lcd_goto(0, 0);
      lcd_puts(buf);
    }

    // ADD CONTROLLER CODE HERE
    if (!gpio_read_pin(GE_PBTN1)) {
      setpoint = setpoint+incre;
      if (setpoint>=800) {
        setpoint = 800;
      }
      delay_ms(100);
    }

    if (!gpio_read_pin(GE_PBTN2)) {
      setpoint = setpoint-incre;
      if(setpoint<=0) {
        setpoint = 0;
      }
      delay_ms(100);
    }
    /*
    error = setpoint-filtered_speed;
    integ = integ + (error*dt);
    deriv = (error-error_prev)/dt;
    error_prev = error;

    duty = (error*kp)+(integ*ki)+(deriv*kd);
    if (duty<=0) {
      duty = 0;
    }
    if (duty>=1) {
      duty = 0.9;
    }
    */
    duty = 0.5;
    /*
    lcd_goto(0, 0);
    snprintf(lcd_print, 50, "Current Speed: %.2f, Set Speed: %.2f", filtered_speed, setpoint);
    lcd_puts(lcd_print);
    */
    pwm_set(PWM_CHAN2, duty);
    delay_ms(10);
    counter++;
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif