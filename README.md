USART3
==============
Ready for USART3, so you can connect rs-232 to the discovery board.

[Usage]
  rs232.tx->PC11; rs232.rx->PC10
  
And completed printf function by usart3.(can't print float)

The printf source code from 《stm32庫開發實踐指南》.



main
==============
已在main.c中加入usart3的printf功能，可以藉由rs232印出加速度計目前X Y Z軸的值到終端機上。
