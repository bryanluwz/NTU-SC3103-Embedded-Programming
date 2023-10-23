#include "show.h"

int show_timer_count  = 0;

void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();
   while(1)
    {	

			vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));

			oled_show();    
    }
}  

void oled_show(void)
{  
     //To DO
	uint8_t namae[20] = "Bryan Lu O-O";
	uint8_t timer[20] = "";
	 
	 //call some function in oled to display something
	OLED_ShowString(0, 10, namae);
	
	sprintf(timer, "Timer: %2dm %2ds", show_timer_count / 60, show_timer_count % 60);
	show_timer_count++;

	OLED_ShowString(0, 40, timer);
	
	 OLED_Refresh_Gram(); //refresh the OLED RAM

	  		vTaskDelay(1000);
	}

