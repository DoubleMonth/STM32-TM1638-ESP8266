#include "hal_dev_ctrl.h"
#include "stddef.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "usart.h"

volatile uint32_t deltms = 0;//任务切换计数用
volatile uint32_t task_monitor;


static void sys_tick(void *args)
{
    static uint8_t _msec = 0, _sec = 0, tick_cnt = 0;

	
    if (deltms >= 20)
    {
        deltms -= 20;
        notify(EV_20MS);
        if (++tick_cnt < 5)
            return;
        tick_cnt = 0;
        notify(EV_100MS);
        if (++_msec < 10)
            return;
        _msec = 0;
        notify(EV_SEC);
        if (++_sec < 60)
            return;
        _sec = 0;
        notify(EV_MIN);
    }
}
uint32_t sys_counter;
static void task_20ms(void *args)
{
		
}


static void task_100ms(void *args)
{
		
}
static void task_sec(void *args)
{
	sys_counter++;
	if(sys_counter>5000)
		{
			sys_counter=0;
			printf ("task_100ms test!\r\n");
		}
}

struct task tasks[] =
{
//    { EV_PLC,    0,            NULL, on_uart_rxchar  },
//    { EV_DEBUG,  0,            NULL, on_debug_rxchar },
//    { EV_CLRDOG, ALWAYS_ALIVE, NULL, clr_watchdog    },
    { EV_TICK,   0,            NULL, sys_tick        },
    { EV_20MS,   0,            NULL, task_20ms       },
    { EV_100MS,  0,            NULL, task_100ms      },
    { EV_SEC,    0,            NULL, task_sec        },
//    { EV_MIN,    0,            NULL, task_min        },
//    { EV_KEY,    0,            NULL, task_key        },
//    { EV_STATE,  0,            NULL, state_machine   },
};

void task_handle(void)
{
    uint8_t i;

    for (i = 0; i < array_size(tasks); ++i)
    { 
        struct task *t = &tasks[i];

        if (is_task_set(t->id) || is_task_always_alive(t->flags))
        {
            reset_task(t->id);
            t->handle(t->args);
        }
    }
}
