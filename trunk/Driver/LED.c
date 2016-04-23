#include <STC15F2K60S2.H>
#include "LED.h"

sbit LED_CS0	= P2^6;//P3^7
sbit LED_CS1	= P2^7;//P3^6
sbit LED_CS2	= P2^0;//P3^5
sbit LED_CS3	= P5^5;//P3^2
sbit LED_CS4	= P5^4;//P3^3
sbit LED_CS5	= P3^6;//P3^4

//与显示相关变量
UINT8 gLedBuf[6] = {14,15,1,6,0,9}; // P-1609
UINT8 gCurLed = 0;//从左到右为0,1,2
UINT8 gShowDot = 0;
BOOL ledsw = 1;

void len_Init(void)
{
    P1M1 &= ~0xFF;      //推挽输出
    P1M0 |= 0xFF;
    P2M1 &= ~0xC1;      //推挽输出
    P2M0 |= 0xC1;
    P5M1 &= ~0x30;      //推挽输出
    P5M0 |= 0x30;
    P3M1 &= ~0x40;      //推挽输出
    P3M0 |= 0x40;
}

/* LED动态显示 */
void led_Handler1ms(void)
{
    if(ledsw)
    {
    	if (gCurLed<5)
    		gCurLed++;
    	else
    		gCurLed=0;
    	LED_CS0 = 1;
    	LED_CS1 = 1;
    	LED_CS2 = 1;
    	LED_CS3 = 1;
    	LED_CS4 = 1;
    	LED_CS5 = 1;
    	switch ( gLedBuf[gCurLed])		 //switch case 语句中 switch中的条件case都不满足就执行default
    	{
    	    case 0:	P1 = 0xeb; break;//0X88
    		case 1:	P1 = 0x88; break;//0XEB
    		case 2:	P1 = 0xB3; break;//0X92
    		case 3:	P1 = 0xba; break;//0XC2
    		case 4:	P1 = 0xd8; break;//0XE1
    		case 5:	P1 = 0x7a; break;//0XC4
    		case 6:	P1 = 0x7b; break;//0X84
    		case 7:	P1 = 0xa8; break;//0XEA
    		case 8:	P1 = 0xfb; break;//0X80
    		case 9:	P1 = 0xfa; break;//0XC0
    		case 10:P1 = 0xcb; break;//0XC0 U
    		case 11:P1 = 0xbe; break;//3.
    		case 12:P1 = 0x00; break;//空
    		case 13:P1 = 0x7e; break;//5.
            case 14:P1 = 0xf1;break;//P
            case 15:P1 = 0x10;break;//-
            case 16:P1 = 0xf9;break;//-
    		default:P1 = 0x00; break;
    	}
        if(gShowDot & (1<<gCurLed))//show dot
        {
     		P1 |= 0X04;
        }
    	switch (gCurLed)
    	{
    		case 0:
    			LED_CS0 = 0;
    			break;
    		case 1:
    			LED_CS1 = 0;
    			break;
    		case 2:
    			LED_CS2 = 0;
    			break;
    		case 3:
    			LED_CS3 = 0;
    			break;
    		case 4:
    			LED_CS4 = 0;
    			break;
    		case 5:
    			LED_CS5 = 0;
    			break;
    		default:
    			break;
    	}
    }
}

void led_ShowNumber(UINT16 L_num, UINT8 R_num, UINT8 DotPlace)
{
    if(L_num>9999)
    {
        L_num = 9999;
    }
    if(R_num>99)
    {
        R_num = 99;
    }
    gLedBuf[0] = L_num/1000;
    gLedBuf[1] = L_num/100%10;
    gLedBuf[2] = L_num/10%10;
    gLedBuf[3] = L_num%10;
    gLedBuf[4] = R_num/10%10;
    gLedBuf[5] = R_num%10;
    gShowDot = DotPlace;
}

void Led_ShowZero(void)
{
    gLedBuf[0] = 12;
    gLedBuf[1] = 12;
    gLedBuf[2] = 12;
    gLedBuf[3] = 12;
    gLedBuf[4] = 12;
    gLedBuf[5] = 0;
    gShowDot = 0;
}

void led_Switch(bit sw)
{
    ledsw = sw;
    if(ledsw==0)
    {
    	LED_CS0 = 1;
    	LED_CS1 = 1;
    	LED_CS2 = 1;
    	LED_CS3 = 1;
    	LED_CS4 = 1;
    	LED_CS5 = 1;
    }
}

