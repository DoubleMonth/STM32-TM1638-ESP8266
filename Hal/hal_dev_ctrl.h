#ifndef __HAL_DEV_CTRL_
#define __HAL_DEV_CTRL_
#include "stdio.h"
#include "string.h"
#include <stdint.h>
#include "stm32f1xx_hal.h"

extern volatile uint32_t task_monitor;
#define DisableInterrupts() __disable_irq()
#define EnableInterrupts()  __enable_irq()
#define OS_CPU_SR  uint32_t
#define OS_ENTER_CRITICAL() do { \
        cpu_sr = __get_PRIMASK(); \
        DisableInterrupts(); \
} while (0)
#define OS_EXIT_CRITICAL()  __set_PRIMASK(cpu_sr)

#define set_bit_security(x, bit)     \
do {\
    OS_CPU_SR cpu_sr;\
    OS_ENTER_CRITICAL();\
    set_bit(x, bit);\
    OS_EXIT_CRITICAL();\
} while(0)
#define clr_bit_security(x, bit)     \
do {\
    OS_CPU_SR cpu_sr;\
    OS_ENTER_CRITICAL();\
    clr_bit(x, bit);\
    OS_EXIT_CRITICAL();\
} while(0)

//---------------------------------------------------------------------------------------
#define min(a, b) ((a)<(b) ? (a):(b))
#define max(a, b) ((a)>(b) ? (a):(b))
//#define bcd2bin(val) (((val) & 0x0f) + ((val) >> 4) * 10)
//#define bin2bcd(val) ((((val) / 10) << 4) + (val) % 10)
#define BCD2BIN(x)  ((((x)>>4)*10)+((x) & 0x0F))
#define BIN2BCD(x)  ((((unsigned char)(x)/10)<<4)+((unsigned char)(x) % 10))
//#define bin2bcd(x)  ((((((x/10)/10)<<4)+((x/10) % 10))<<4)+(((((x%10)/10)<<4)+((x%10) % 10))))
#define bcd2bin(x)  ((BCD2BIN((x)>>8)*100)+(BCD2BIN((x) & 0xFF)))
//---------------------------------------------------------------------------------------

#define is_bit_set(x, bit)     ((x) & (1 << (bit)))
#define clr_bit(x, bit) ((x) &= ~(1 << (bit)))
//---------------------------------------------------------------------------------------
#define set_bit(x, bit) ((x) |= 1 << (bit))
#define clr_bit(x, bit) ((x) &= ~(1 << (bit)))
#define tst_bit(x, bit) ((x) & (1 << (bit)))
#define get_bits(val,x1,x2)   (((val)>>(x1))&((1<<((x2)-(x1)+1))-1))
#define hole(val, start, end) ((val) & (~(((1 << ((end) - (start) + 1)) - 1) << (start))))
#define fill(set, start, end) (((set) & ((1 << ((end) - (start) + 1)) - 1))<< (start))
#define set_bits(val, set, start, end) (val = hole(val, start, end) | fill(set, start, end))
//---------------------------------------------------------------------------------------
#define array_size(array) (sizeof(array)/sizeof(*array))
//---------------------------------------------------------------------------------------
#define notify(task_bit)                 set_bit_security(task_monitor, task_bit)
#define is_task_set(task_bit)            tst_bit(task_monitor, task_bit)
#define is_task_always_alive(flags)      (flags & ALWAYS_ALIVE)
#define reset_task(task_bit)             clr_bit_security(task_monitor, task_bit)

enum
{
    EV_CLRDOG,
    EV_PLC,
    EV_DEBUG,
    EV_TICK,
    EV_20MS,
    EV_100MS,
    EV_SEC,
    EV_MIN,
    EV_KEY,
    EV_STATE
};
enum
{
    ALWAYS_ALIVE = 0x01
};
struct task
{
    uint8_t id;
    uint8_t flags;
    void *args;
    void (*handle) (void *args);
};
void task_handle(void);
#endif
