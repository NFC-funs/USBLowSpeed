# USBLowSpeed
Jieli AD15n For Low Speed USB Device 

1. Introduction
I want to design the lowest cost USB NFC Reader&Writer, then I found Jieli AD15n:

      https://github.com/Jieli-Tech/fw-AD15N

      https://gitee.com/Jieli-Tech/fw-AD15N

2. Background
Universal Serial Bus Specification Revision 1.1
7.1.11 Data Signaling Rate
The low-speed data rate is nominally 1.50Mb/s. The permitted data-rate tolerance for low-speed functions is ±1.5% (15,000ppm).

      AD15n's LSB default value is 80MHz, and it can be divided by n for LSB device(include IO&PWM).

      NFC readers use NFC frequency of about 13.56 MHz, and 13.56/9/1.5-1=0.444% < 1.5%.
      If the LSB can be overclocked to 81.36MHz(13.56*6), it will work for NFC and USB low speed device.

3. Test
Fortunately, I tuned the PLL of AD15n, it works.

      https://gitee.com/MCUer/AD15N-App/blob/master/02-USB/COMM/USB_CLK.C
