stm32_template
==============

stm32f4-discovery template

Ready for USART3, so you can connect rs-232 to the discovery board.

[Usage]
  rs232.tx->PC11; rs232.rx->PC10
  
And completed printf function by usart3.

The printf source code from 《stm32庫開發實踐指南》.
==============
main.c中加入printf功能，可以印出加速度計目前X Y Z軸的值。
