#include "STC15F2K60S2.h"
#include "sys_uart.h"
#include "string.h"

#define FOSC 11059200L          //ϵͳƵ��
#define BAUD 9600

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

void sys_uartRxHandler(void);

void sys_uartInit(void)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
    SCON = 0x50;                //8λ�ɱ䲨����
    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ���1�ж�
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
