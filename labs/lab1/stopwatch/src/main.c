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

timer_id_t timer_id;

volatile int counter;
char timer_print[10];
int minute;
int second;
int millisecend;
//uint8_t timer_state;

//#define NUM_STATE 3
// enumerate states
/*
enum TIMER_STATE {
  TIMER_START, TIMER_STOP, TIMER_RESET
};
*/


// timer counter
void timer_counter(){
  counter += 1;
}


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

  // Initialize timer
  timer_init();

  // Initialize PBTN1 as START button
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);

  // Initialize PBTN2 as STOP button
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);

  // Initialize PBTN3 as RESET button
  gpio_setup_pin(GE_PBTN3, GPIO_INPUT, false, false);

  // Setup timer library
  // Set minimum timestep to 1ms (number of counts referecned to 
  // a 72MHz clock)
  timer_set_timestep(72000);
  // register callback for time counting every 1ms   
  //timer_id = timer_register(1, &timer_counter, GE_PERIODIC);
  
  counter = 0;

  /* Infinite loop */
  /**
   * Handles the user interface state machine
   */
  while (1) {       
    if (!gpio_read_pin(GE_PBTN1)) {   
      timer_deinit();
      timer_init();
      timer_id = timer_register(1, &timer_counter, GE_PERIODIC);
      timer_start(timer_id);
      //timer_state = TIMER_START;      
    }

    if (!gpio_read_pin(GE_PBTN2)) {
      // stop timer
      timer_stop(timer_id);
      //timer_state = TIMER_STOP;      
    }
    
    if (!gpio_read_pin(GE_PBTN3)) {
      // reset timer to 0
      timer_stop(timer_id);
      counter = 0;
      //timer_state = TIMER_RESET;      
    }
/*
    switch (timer_state){
      case TIMER_START:
        // start timer          
        timer_id = timer_register(1, &timer_counter, GE_PERIODIC);
        timer_start(timer_id);

      case TIMER_STOP:
        // stop timer
        timer_stop(timer_id);

      case TIMER_RESET:
        // reset timer to 0
        counter = 0;
    }
*/

    lcd_clear();
    lcd_goto(0, 0);
    minute = counter / 60000;
    second = (counter % 60000) / 1000;
    millisecend = counter % 1000;
    snprintf(timer_print, 10, "%02d:%02d.%03d", minute, second, millisecend);
    //snprintf(timer_print, 5, "%d", counter);
    lcd_puts(timer_print);

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
