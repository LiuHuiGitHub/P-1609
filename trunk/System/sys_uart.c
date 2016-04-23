#include "STC15F2K60S2.h"
#include "sys_uart.h"
#include "string.h"

#define FOSC 11059200L          //系统频率
#define BAUD 9600

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

void sys_uartRxHandler(void);

void sys_uartInit(void)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
    SCON = 0x50;                //8位可变波特率
    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;
}

void sys_uartInterrupt() interrupt 4
{
    if(RI)
    {
        RI = 0;
        sys_uartRxHandler();
    }
    if(TI)
    {
        TI = 0;
    }
}

UINT8 RxNum = 0;
void sys_uartRxHandler(void)
{
    if(SBUF == 0x7F)        //0x7F auto download     boud 2400 0x7F at boud 9600 is 0xF8
    {
        RxNum++;
        if(RxNum >= 40)
        {
            IAP_CONTR = 0x60;
        }
    }
    else
    {
        RxNum = 0;
    }
}
