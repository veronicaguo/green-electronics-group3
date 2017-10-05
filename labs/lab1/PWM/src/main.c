/**
 * @file  main.c
 * @brief Hello World demo for the Green Electronics libraries.
 * 
 * @details Demos the basic functionality of the Green Electronics
 * libraries.
 * 
 * @author Ned Danyliw
 * @date  09.2016
 */
#include "ge_libs.h"
#include <stdio.h>

char lcd_print[16];

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // Initialize library
  ge_init();

  // Initialize PBTN1 to increase duty cycle by 0.05
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);

  // Initialize PBTN2 to decrease duty cycle by 0.05
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);

  // set pwm level for PWM demo
  float pwm_level = 0.0;

  // // enable PWM library
  pwm_init();
  pwm_freq(10000.0);

  // enable pwm pins
  pwm_set_pin(PA9);

  /* Infinite loop */
  /**
   * Handles the user interface state machine
   */
  while (1) {   

    if (!gpio_read_pin(GE_PBTN1)) {
      // increase duty cycle by 0.05
      pwm_level += 0.05;   
    }

    if (!gpio_read_pin(GE_PBTN2)) {
      // decrease duty cycle by 0.05
      pwm_level -= 0.05;   
    }

    if (pwm_level > 1.0) pwm_level = 1.0;
    if (pwm_level < 0.0) pwm_level = 0.0;

    pwm_set(PWM_CHAN2, pwm_level);
 
    // LCD display
    lcd_clear();
    lcd_goto(0, 0);
    snprintf(lcd_print, 16, "PWM level: %.2f", pwm_level);
    lcd_puts(lcd_print);

    delay_ms(50);
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
