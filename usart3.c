#include "stm32f4xx_conf.h"
#include <stdarg.h>
#include <string.h>

/* function name: str_reverse()
 * description: reverse string
 *
 * @args: a null-terminated string
 * @return: nothing
 * @result: the characters in str are str_reverse
 * @refrence: https://wirejungle.wordpress.com/2011/08/06/displaying-floating-point-numbers/
 */
void str_reverse(char* begin, char* end)
{
	char tmp;
	while(end > begin)
		tmp=*end, *end--=*begin, *begin++=tmp;
}


/*
 * function name: 	itoa
 * description:		integer to ascii
 * 
 * @args:			value, str
 * @return:			the last position of string
 * @result:			convert integer to ascii store in str
*/
char *itoa(long value, char *str)
{
	char *p = str;
	static char digit[] = "0123456789";

	//Add sign if needed
	if(value < 0) *(p++)='-';

	//Work on unsigned
	//value = ABS(value);

	//Conversion. Number is reversed.
	do{
		const int tmp = value/10;
		*(p++) = digit[value - (tmp * 10)];
		value = tmp;
	}while(value);

	*p=' ';
	str_reverse(str, p - 1);
	return p;
}


void ltoa(char *buf, unsigned long i, int base)
{
        char *s;
        #define LEN 25
        int rem;
        char rev[LEN+1];

        if (i == 0)
                s = "0";
        else
                {
                rev[LEN] = 0;
                s = &rev[LEN];
                while (i)
                        {
                        rem = i % base;
                        if (rem < 10)
                                *--s = rem + '0';
                        else if (base == 16)
                                *--s = "abcdef"[rem - 10];
                        i /= base;
                        }
                }
        strcpy(buf, s);
}

typedef union {
long L;
float F;
} LF_t;

char *ftoa(float f) //, int *status)
{
        long mantissa, int_part, frac_part;
        short exp2;
        LF_t x;
        char *p;
        static char outbuf[15];

        //*status = 0;
        if (f == 0.0)
        {
                outbuf[0] = '0';
                outbuf[1] = '.';
                outbuf[2] = '0';
                outbuf[3] = 0;
                return outbuf;
        }
        x.F = f;

        exp2 = (unsigned char)(x.L >> 23) - 127;
        mantissa = (x.L & 0xFFFFFF) | 0x800000;
        frac_part = 0;
        int_part = 0;

        if (exp2 >= 31)
        {
                //*status = _FTOA_TOO_LARGE
				//outbuf = 0;return;
                return 0;
        }
        else if (exp2 < -23)
        {
                //*status = _FTOA_TOO_SMALL;
				//outbuf = 0;return;
                return 0;
        }
        else if (exp2 >= 23)
        int_part = mantissa << (exp2 - 23);
        else if (exp2 >= 0)
        {
                int_part = mantissa >> (23 - exp2);
                frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
        }
        else /* if (exp2 < 0) */
        frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);

        p = outbuf;

        if (x.L < 0)
                *p++ = '-';

        if (int_part == 0)
                *p++ = '0';
        else
        {
                ltoa(p, int_part, 10);
                while (*p)
                p++;
        }
        *p++ = '.';

        if (frac_part == 0)
                *p++ = '0';
        else
        {
                char m, max;

                max = sizeof (outbuf) - (p - outbuf) - 1;
                if (max > 7)
                        max = 7;
                /* print BCD */
                for (m = 0; m < max; m++)
                {
                        /* frac_part *= 10; */
                        frac_part = (frac_part << 3) + (frac_part << 1);

                        *p++ = (frac_part >> 24) + '0';
                        frac_part &= 0xFFFFFF;
                }
                /* delete ending zeroes */
                for (--p; p[0] == '0' && p[-1] != '.'; --p)
                        ;
                        ++p;
        }
        *p = 0;

        return outbuf;
}


/*
 * function name: 	USART3_Config
 * description:		USART3 GPIO configure, 9600 8-N-1
 * input:			none
 * output:			none
 * refrence from:	http://lolikitty.pixnet.net/blog/post/143466068
*/
void USART3_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

   /******** 啟用 GPIOC、USART3 的 RCC 時鐘 ********/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	/******** 將 PC10、PC11 連接至 USART3 ********/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	/******** 設定 PC10 為 Tx 覆用  ********/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 使用推挽式輸出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 使用上拉電阻
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 設置為覆用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // 設定第 10 腳
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 設定 GPIO 速度為 50 MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** 設定 PC11 為 Rx 覆用  ********/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 設置為覆用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // 設定第 11 腳
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** USART 基本參數設定 ********/
	USART_InitStructure.USART_BaudRate = 9600; // 設定 USART 包率 (每秒位元數) 為 9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 設定 USART 傳輸的資料位元為 8
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // 設定 USART 停止位元為 1
	USART_InitStructure.USART_Parity = USART_Parity_No; // 不使用同位元檢查
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用流量控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // 設定 USART 模式為 Rx (接收) 、 Tx (傳送)
	USART_Init(USART3, &USART_InitStructure); // 套用以上 USART 設置，並初始化UART3

	/******** 啟用 USART3 ********/
	USART_Cmd(USART3, ENABLE);
}

char USART3_printf(char *string){
    while(*string){
        /* 傳送訊息至 USART3 */
        USART_SendData(USART3, (unsigned short int) *string++);
 
        /* 等待訊息傳送完畢 */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
}

/*
 * function name: 	printf
 * description:		format and print data
 * input:			none
 * output:			none
 * refrence from:	http://lolikitty.pixnet.net/blog/post/143466068
*/
void printf(uint8_t *Data,...)
{
	const char *s;
	int d;   
	double f;
	char buf[16]={0};
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     //判斷是否到達string結束位元
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':
					USART_SendData(USART3, 0x0d);
					Data ++;
					break;
		
				case 'n':
					USART_SendData(USART3, 0x0a);	
					Data ++;
					break;
		
				default:
					Data ++;
					break;
			}			 
		}
		else if ( *Data == '%')
		{									 
			switch ( *++Data )
			{				
				case 's':	
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(USART3,*s);
						while( USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
					}
					Data++;
					break;
	
					case 'd':										//十進制
						d = va_arg(ap, int);
						itoa(d, buf);
						for (s = buf; *s; s++) 
						{
							USART_SendData(USART3,*s);
							while( USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
						}
						Data++;
						break;
					case 'f':
						f = va_arg(ap, double);
						strcat(buf, ftoa(f));
						for(s = buf; *s; s++)
						{
							USART_SendData(USART3, *s);
							while( USART_GetFlagStatus(USART3, USART_FLAG_TC == RESET));
						}
						Data++;
						break;
					default:
						Data++;
						break;
			}		 
		} /* end of else if */
		else USART_SendData(USART3, *Data++);
			while( USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
	}
}


