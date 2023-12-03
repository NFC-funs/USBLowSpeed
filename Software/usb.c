#include "sfr.h"
#include "cpu.h"
#include "config.h"
#include "gpio.h"
#include "clock.h"
#include "common.h"

#include "usb.h"
#include "usbkey.h"

#include "pwmtimer.h"
#include "xcomm.h"

#include "irinput.h"  //PS-20220601

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[usb]"
#include "debug.h"

AT_RAM
u16 X_UsbCrc16(u8 *ptr,u8 len)
{
    u8 i;
    u16 crc = 0xFFFF;

    while(len--)
    {
        crc ^= *ptr++;
        for (i=0;i<8;++i)
        {
            if(crc&1)
                crc = (crc>>1)^0xA001;  //0x8005,swap MSB to LSB
            else
                crc = (crc>>1);
        }
    }

    return ~crc;
}

AT_RAM
void UsbDelay(u32 us)  //150 about 7us,set to 1.5us(gap of host send continuous two packets )
{
    while (us--) {

            asm("nop");
    }
}

AT_RAM
void X_UsbDeviceAck()  //0010-->01001011
{
    //SYNC,PID,EOP
    //K-J-K-J-K-J-K-K,J-J-K-J-J-K-K-K,0-0-J

    USB_SET_TO_OUTPUT;
    //only for test timming ,first must be J-status
    USB_SET_J;USB_OUTPUT_DIRECT;UsbDelay(100);
    /////////////////////
    //Delay Timer Start
    USB_DELAYTIMER_START;

    //SYNC K-J-K-J-K-J-K-K
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //PID--ACK J-J-K-J-J-K-K-K
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //EOP 0-0-J
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;


    //USB_DELAYTIMER_STOP;
    USB_SET_TO_INPUT;

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_HIGH;
    #endif // Debug_USB_Input_En

}
AT_RAM
void X_UsbDeviceNack()  //1010-->01011010
{
    //SYNC,PID,EOP
    //K-J-K-J-K-J-K-K,J-J-K-K-K-J-J-K,0-0-J

    USB_SET_TO_OUTPUT;
    //only for test timming ,first must be J-status
    USB_SET_J;USB_OUTPUT_DIRECT;UsbDelay(100);
    /////////////////////
    //Delay Timer Start
    USB_DELAYTIMER_START;

    //SYNC K-J-K-J-K-J-K-K
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //PID--NACK J-J-K-K-K-J-J-K
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //EOP 0-0-J
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;

    //USB_DELAYTIMER_STOP;
    USB_SET_TO_INPUT;

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_HIGH;
    #endif // Debug_USB_Input_En
}
AT_RAM
void X_UsbDeviceStall()  //1110-->01111000,PS-20220703
{
    //SYNC,PID,EOP
    //K-J-K-J-K-J-K-K,J-J-J-J-J-K-J-K,0-0-J

    USB_SET_TO_OUTPUT;
    //only for test timming ,first must be J-status
    USB_SET_J;USB_OUTPUT_DIRECT;UsbDelay(32);
    /////////////////////
    //Delay Timer Start
    USB_DELAYTIMER_START;

    //SYNC K-J-K-J-K-J-K-K
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //PID--STALL J-J-J-J-J-K-J-K
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;
    USB_SET_K;USB_OUTPUT_TO_IO;

    //EOP 0-0-J
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;

    //USB_DELAYTIMER_STOP;
    USB_SET_TO_INPUT;

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_HIGH;
    #endif // Debug_USB_Input_En
}

//81MHz/4 count 20250 times is 1ms PS-20220602
AT_RAM
void X_TunePll(u16 u16Trim)
{
    u32 pll_con1,pll_con1_temp1,pll_con1_temp2;

    local_irq_disable();

    pll_con1 = JL_PLL->CON1;
    pll_con1_temp1 = (pll_con1&0x00000FFF) + 2;

    pll_con1_temp2 = (pll_con1_temp1*20255)/u16Trim - 2;
    //if (((pll_con1_temp1*20250)%u16Trim) > 10125) pll_con1_temp2 = pll_con1_temp2 + 1;

    pll_con1 = (pll_con1&0xFFFFF000)|pll_con1_temp2;
    JL_PLL->CON1 = pll_con1;

    udelay(100);

    local_irq_enable();
}

static u16 u16LastBitStatus;
static u8 u8BitOneCounter;
static u8 u8BitSE0Counter;

AT_RAM
void X_DecodeBitStuffing()
{
    if (u8BitOneCounter == 6)
    {
        USB_INPUT_FROM_IO;
        u16LastBitStatus = u16UsbIoStatus;
        u8BitOneCounter = 0;
    }
}

AT_RAM
u8 X_DecodeUsbData()
{
    u8 i,j;

    u8ArrayUsbRecvData[0] = 0x00;  //first bit must be zero
    u8BitOneCounter = 0;
    u8BitSE0Counter = 0;

    u16LastBitStatus = USB_BIT_IS_K;

    //log_info("usb recv data ...\n");

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_LOW;
    #endif // Debug_USB_Input_En

    //Delay Timer Start
    USB_DELAYTIMER_START;

    //SYNC MSB-0x80 LSB-0x01
    for (i=1;i<8;i++)
    {
        u8ArrayUsbRecvData[0] >>= 1;
        X_DecodeBitStuffing();
        USB_INPUT_FROM_IO;
        if (u16UsbIoStatus == u16LastBitStatus)
        {
            u8ArrayUsbRecvData[0] |= 0x80;
            u8BitOneCounter++;
            break;  //it's KK, SYNC-KJKJKJKK PS-20230102
        }
        else
        {
            u8ArrayUsbRecvData[0] &= 0x7F;
            u8BitOneCounter=0;
        }
        u16LastBitStatus = u16UsbIoStatus;
    }

    //#ifdef Debug_USB_Input_En
    //    DEBUG_USB_IN_HIGH;
    //#endif // Debug_USB_Input_En
    //log_info("u8ArrayUsbRecvData 0: %x",u8ArrayUsbRecvData[0]);

    for (j=1;j<13;j++)
    {
        for (i=0;i<8;i++)
        {
            u8ArrayUsbRecvData[j] >>= 1;
            X_DecodeBitStuffing();
            USB_INPUT_FROM_IO;
            if (u16UsbIoStatus == USB_BIT_IS_0)
            {
                #ifdef Debug_USB_Input_En
                    DEBUG_USB_IN_HIGH;
                #endif // Debug_USB_Input_En
                //if (j>8) log_info("j i u8ArrayUsbRecvData: %d,%d,%x,%x,%x,%x",j,i,u8ArrayUsbRecvData[0],u8ArrayUsbRecvData[1],u8ArrayUsbRecvData[2],u8ArrayUsbRecvData[3]);

                return j;
            }
            else
            {
               if (u16UsbIoStatus == u16LastBitStatus)
                {
                    u8ArrayUsbRecvData[j] |= 0x80;
                    u8BitOneCounter++;
                }
                else
                {
                    u8ArrayUsbRecvData[j] &= 0x7F;
                    u8BitOneCounter=0;
                }
                u16LastBitStatus = u16UsbIoStatus;
            }

        }
    }

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_HIGH;
    #endif // Debug_USB_Input_En

    return 0;
}
AT_RAM
void X_EncodeUsbData()
{
    u16 i,j,k;

    u8 u8CrcTemp[10];
    u16 u16CrcResult;

    u8ArrayUsbTransData[0] = 0x80;  //first byte must be SYNC

    //DATA0/DATA1
    if(u8LastDATA0D1Status == USB_PID_DATA0) u8ArrayUsbTransData[1] = USB_PID_DATA1;
    else  u8ArrayUsbTransData[1] = USB_PID_DATA0;
    u8LastDATA0D1Status = u8ArrayUsbTransData[1];


    if ((u8LengthUsbtransBuff - u8CounterUsbTrans) < 8) k = (u8LengthUsbtransBuff - u8CounterUsbTrans);
    else k = 8;


    for (i=0;i<k;i++)
    {
        u8CrcTemp[i] = u8ArrayUsbTransBuff[u8CounterUsbTrans + i];
    }
    u16CrcResult = X_UsbCrc16(u8CrcTemp,k);
    u8CrcTemp[k]   = (u16CrcResult&0x00FF);
    u8CrcTemp[k+1] = (u16CrcResult>>8);

    k=k+2;

    for(i=0;i<k;i++)
    {
       u8ArrayUsbTransData[i+2] =  u8CrcTemp[i];
    }

}
AT_RAM
void X_TransUsbData()
{
    u16 i,j,k;
    u8BitOneCounter = 0;

    //4--SYNC PID CRC16
    if ((u8LengthUsbtransBuff - u8CounterUsbTrans) <8 ) k = (u8LengthUsbtransBuff - u8CounterUsbTrans)+4;
    else k = 8+4;

    USB_SET_TO_OUTPUT;
    //only for test timming ,first must be J-status
    USB_SET_J;USB_OUTPUT_DIRECT;UsbDelay(100);
    /////////////////////
    //u16LastBitStatus = USB_BIT_IS_J;
    //Delay Timer Start
    USB_DELAYTIMER_START;

    //u16UsbIoStatus depend on gpio  PS-20221120
    for(i=0;i<k;i++)
    {
        //u8BitOneCounter = 0;  //remove it by PS-20220626
        for(j=0;j<8;j++)
        {
            //////////////BitStuffing///////////////
            //it's work PS-20220626
            if (u8BitOneCounter == 6)
            {
                //six 1 then insert 0
                if (u16UsbIoStatus & USB_BIT_IS_K) u16UsbIoStatus = u16UsbIoStatus + USB_BIT_IS_K;
                else  u16UsbIoStatus= u16UsbIoStatus - USB_BIT_IS_K;

                u8BitOneCounter = 0;
                USB_OUTPUT_TO_IO;
            }
            /////////////////////////////////////////

            if (u8ArrayUsbTransData[i]&0x01)
            {
                u8BitOneCounter ++;
                //do nothing PS-20220626
            }
            else
            {
                u8BitOneCounter = 0;
                if (u16UsbIoStatus & USB_BIT_IS_K) u16UsbIoStatus= u16UsbIoStatus + USB_BIT_IS_K;
                else  u16UsbIoStatus= u16UsbIoStatus - USB_BIT_IS_K;
            }
            USB_OUTPUT_TO_IO;
            u8ArrayUsbTransData[i] >>= 1;
            //if(u16UsbIoStatus & USB_BIT_IS_K) u16LastBitStatus = USB_BIT_IS_K;
            //else  u16LastBitStatus = USB_BIT_IS_J;
        }
    }

    //EOP 0-0-J
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_0;USB_OUTPUT_TO_IO;
    USB_SET_J;USB_OUTPUT_TO_IO;

    //USB_DELAYTIMER_STOP;
    USB_SET_TO_INPUT;

    #ifdef Debug_USB_Input_En
        DEBUG_USB_IN_HIGH;
    #endif // Debug_USB_Input_En
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////
/////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
___interrupt
static void timer1_reset_cap_isr(void)
{
    JL_TMR1->CON |= BIT(6);  //clear PND interrupt flag

    //log_info("the second reset...\n");
}

AT_RAM
___interrupt
static void timer0_usb_cap_isr(void)
{
    u8 u8DecodeDataLength,i;

    JL_TMR0->CON = BIT(6);//clear PND and stop timer

    //#ifdef Debug_USB_Input_En
    //    DEBUG_USB_IN_LOW;
    //#endif // Debug_USB_Input_En
    //log_info("usb d+ int ...\n");

    u8DecodeDataLength = X_DecodeUsbData();
    USB_INPUT_FROM_IO;//SE0
    USB_INPUT_FROM_IO;//J


    switch(u8ArrayUsbRecvData[USB_PACKET_PID])
    {
        case USB_PID_SETUP:  //Control Transfer,EP0
            //if ((u8ArrayUsbRecvData[2] >> 1) == u8UsbAddr)  //Addr
            {
                //if((u8ArrayUsbRecvData[2] >> 1) == 0)
                {
                  u8StatusTransData = USB_PID_SETUP;
                }
            }
            break;
        case USB_PID_IN:
            u8StatusTransData = USB_PID_IN;
            //Step 2:Only test usb receive data PS-20220619
            //log_info("USB_PID_IN\n");
            //log_info("USB_PID_IN u8ArrayUsbRecvData: %x,%x,%x,%x",u8ArrayUsbRecvData[0],u8ArrayUsbRecvData[1],u8ArrayUsbRecvData[2],u8ArrayUsbRecvData[3]);
            //device to host, 0--device 1--interface 2--endpoint
            //EP1--KEY EP0-Control  PS-20220703
            if (u8ArrayUsbRecvData[USB_PACKET_bmRequestType]&0x80)  //bit 0 is 1 -->EP1
            {
                //report key when key press  PS-20221019
                //Nack when no key press  PS-20221019
                //bit7 means PID_IN from EP1 PS-20230108
                u8StatusTransData |= 0x80;
                if (u8LengthUsbtransBuff == 0x00)
                {
                    X_UsbDeviceNack();
                }
                else
                {
                    if (u8UsbTransBuffForEp == 1) X_TransUsbData();
                    else X_UsbDeviceNack();
                }
            }
            else if (u8ArrayUsbRecvData[USB_PACKET_bRequest]&0x01)  //bit 1 is 1 -->EP2
            {
                if (u8UsbPidAck == 0x01)  //EP1,it means EP1 is transmitting data
                {
                    X_UsbDeviceNack();

                    u8UsbPidAck = 0x00;
                    //device transmit data to host is success
                    if ((u8LengthUsbtransBuff - u8CounterUsbTrans) <8 )  //complete transmition
                    {
                        u8LengthUsbtransBuff = 0;
                        u8CounterUsbTrans = 0;
                    }
                    else  //continue transmit data
                    {
                        u8CounterUsbTrans = u8CounterUsbTrans + 8;
                        //if (u8UsbTransBuffForEp != 1)X_EncodeUsbData();
                        X_EncodeUsbData();
                    }
                    //log_info("EP2_IN Nack EP1 is using");
                }
                else
                {
                    //EP2 transmit data to Host  ///////////////////////////////////////////////
                    if (u8LengthUsbtransBuff == 0x00)
                    {
                        X_UsbDeviceNack();

                        //log_info("u8Ep2RecvPacket:%d",u8Ep2RecvPacket);
                        if (u8UsbEPn == 2)
                        {
                            if (u8Ep2RecvPacket == 5)
                            {
                                u8Ep2RecvPacket = 0;

                                u8UsbTransBuffForEp = 2;  //For EP2
                                //transmit data length
                                u8LengthUsbtransBuff = 30;
                                //transmit data ready
                                u8CounterUsbTrans = 0;
                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                    u8ArrayUsbTransBuff[i] = u8ArrayEp2RecvTemp[i];//
                                X_EncodeUsbData();
                            }

                        }
                        u8UsbIdle = 0x01;
                        //log_info("EP2_IN Nack");
                    }
                    else
                    {
                        if (u8UsbTransBuffForEp == 2)
                        {
                            if(u8UsbPidAck == 0x02)  //trans data but no ack,its very important because usb will hangup without retry  PS-20230823
                            {
                                //X_UsbDeviceNack();
                                //resume USB_PID_DATA
                                if(u8LastDATA0D1Status == USB_PID_DATA0) u8LastDATA0D1Status = USB_PID_DATA1;
                                else u8LastDATA0D1Status = USB_PID_DATA0;
                                /*
                                //transmit data length
                                u8LengthUsbtransBuff = 30;
                                //transmit data ready
                                u8CounterUsbTrans = 0;
                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                    u8ArrayUsbTransBuff[i] = u8ArrayEp2RecvTemp[i];//
                                */
                                X_EncodeUsbData();
                                u8UsbPidAck = 0x00;
                                log_info("EP2_IN re-trans...%d",u8CounterUsbTrans);
                            }
                            else
                            {
                                X_TransUsbData();
                                u8UsbPidAck = 0x02;  //
                            }

                        }
                        else
                        {
                            X_UsbDeviceNack();
                            u8UsbIdle = 0x01;
                            //log_info("EP2_IN other Nack");
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////////////


                }
            }
            else X_TransUsbData();  //EP0
            break;
        case USB_PID_OUT:
            u8StatusTransData = USB_PID_OUT;
            if (u8ArrayUsbRecvData[USB_PACKET_bmRequestType]&0x80)  //bit 0 is 1 -->EP1
            {
                u8UsbEPn = 1;
            }
            else if (u8ArrayUsbRecvData[USB_PACKET_bRequest]&0x01)  //bit 1 is 1 -->EP2
            {
                u8UsbEPn = 2;
            }
            else u8UsbEPn = 0;
            break;
        case USB_PID_SOF:
            break;

        case USB_PID_DATA0:
            u8LastDATA0D1Status = USB_PID_DATA0;

            if (u8StatusTransData == USB_PID_SETUP)
            {
                //REQUEST_TYPE bmRequestType
                //if(u8ArrayUsbRecvData[2]&USBRQ_DIR_MASK)  //the following data,Device to Host
                //use switch--20220703
                switch(u8ArrayUsbRecvData[USB_PACKET_bmRequestType])
                {
                    case USBRQ_DIR_DEVICE_TO_HOST :  //0x80
                        switch(u8ArrayUsbRecvData[USB_PACKET_bRequest])
                        {
                            case USBRQ_GET_DESCRIPTOR:
                                switch(u8ArrayUsbRecvData[USB_PACKET_wValueH])
                                {
                                    case USB_DESCRIPTOR_DEVICE:  //STEP 1 STEP 3 STEP 9
                                        //Step 1:Only test usb receive data PS-20220619
                                        //USB_SET_TO_OUTPUT;
                                        #ifdef Debug_USB_Input_En
                                            DEBUG_USB_IN_LOW;
                                        #endif // Debug_USB_Input_En
                                        X_UsbDeviceAck();
                                        //transmit data length
                                        if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_DevDesc))
                                            u8LengthUsbtransBuff = sizeof(HIDKEY_DevDesc);
                                        else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                        //transmit data ready
                                        u8CounterUsbTrans = 0;
                                        for(i=0;i<u8LengthUsbtransBuff;i++)
                                            u8ArrayUsbTransBuff[i] = HIDKEY_DevDesc[i];
                                        X_EncodeUsbData();
                                        break;
                                    case USB_DESCRIPTOR_CONFIGURATION:  //STEP 4 STEP 10 STEP 11
                                        X_UsbDeviceAck();
                                        //transmit data length
                                        if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_CfgDesc))
                                            u8LengthUsbtransBuff = sizeof(HIDKEY_CfgDesc);
                                        else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                        //transmit data ready
                                        u8CounterUsbTrans = 0;
                                        for(i=0;i<u8LengthUsbtransBuff;i++)
                                            u8ArrayUsbTransBuff[i] = HIDKEY_CfgDesc[i];
                                        X_EncodeUsbData();
                                        break;
                                    case USB_DESCRIPTOR_STRING:
                                        X_UsbDeviceAck();
                                        switch (u8ArrayUsbRecvData[USB_PACKET_wValueL])
                                        {
                                            case 0:  //STEP 5
                                                //transmit data length
                                                if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_StringLangID))
                                                    u8LengthUsbtransBuff = sizeof(HIDKEY_StringLangID);
                                                else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                                //transmit data ready
                                                u8CounterUsbTrans = 0;
                                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                                    u8ArrayUsbTransBuff[i] = HIDKEY_StringLangID[i];
                                                break;
                                            case 2:  //STEP 6
                                                //transmit data length
                                                if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_StringProduct))
                                                    u8LengthUsbtransBuff = sizeof(HIDKEY_StringProduct);
                                                else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                                //transmit data ready
                                                u8CounterUsbTrans = 0;
                                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                                    u8ArrayUsbTransBuff[i] = HIDKEY_StringProduct[i];
                                                break;
                                            case 3:  //STEP 7 STEP 16 STEP 21
                                                //transmit data length
                                                if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_StringSerial))
                                                    u8LengthUsbtransBuff = sizeof(HIDKEY_StringSerial);
                                                else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                                //transmit data ready
                                                u8CounterUsbTrans = 0;
                                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                                    u8ArrayUsbTransBuff[i] = HIDKEY_StringSerial[i];
                                                break;
                                            case 1:  //STEP 15 STEP 20
                                                //transmit data length
                                                if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_StringManufacturer))
                                                    u8LengthUsbtransBuff = sizeof(HIDKEY_StringManufacturer);
                                                else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                                //transmit data ready
                                                u8CounterUsbTrans = 0;
                                                for(i=0;i<u8LengthUsbtransBuff;i++)
                                                    u8ArrayUsbTransBuff[i] = HIDKEY_StringManufacturer[i];
                                                break;

                                        }
                                        X_EncodeUsbData();

                                        //X_UsbDeviceAck();

                                        break;
                                    case USB_DESCRIPTOR_HID_REPORT:  //STEP 14
                                        X_UsbDeviceAck();
                                        if (u8ArrayUsbRecvData[USB_PACKET_wIndexL] == 0)  //intefae 0 report descriptor
                                        {
                                            //transmit data length
                                            if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_KeyRepDesc))
                                                u8LengthUsbtransBuff = sizeof(HIDKEY_KeyRepDesc);
                                            else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                            //transmit data ready
                                            u8CounterUsbTrans = 0;
                                            for(i=0;i<u8LengthUsbtransBuff;i++)
                                                u8ArrayUsbTransBuff[i] = HIDKEY_KeyRepDesc[i];
                                        }
                                        else //interface 1 report descriptor
                                        {
                                            //transmit data length
                                            if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDCOMP_RepDesc))
                                                u8LengthUsbtransBuff = sizeof(HIDCOMP_RepDesc);
                                            else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                            //transmit data ready
                                            u8CounterUsbTrans = 0;
                                            for(i=0;i<u8LengthUsbtransBuff;i++)
                                                u8ArrayUsbTransBuff[i] = HIDCOMP_RepDesc[i];
                                        }
                                        X_EncodeUsbData();
                                        break;
                                    case USB_DESCRIPTOR_DEVICE_QUALIFIER:  //STEP 8 STEP 18
                                        X_UsbDeviceStall();
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case USBRQ_GET_CONFIGURATION:
                                break;
                            case USBRQ_CLEAR_FEATURE:
                                break;
                            case USBRQ_GET_STATUS:
                                break;
                            default:
                                break;
                        }
                        break;
                    case (USBRQ_DIR_DEVICE_TO_HOST | USBRQ_DIR_DEVICE_TO_HOST_INTERFACE):  //0x81
                        {
                            switch(u8ArrayUsbRecvData[USB_PACKET_wValueH])
                                {
                                    case USB_DESCRIPTOR_HID_REPORT:  //STEP 14
                                        X_UsbDeviceAck();
                                        //transmit data length

                                        if (u8ArrayUsbRecvData[USB_PACKET_wIndexL] == 0)  //intefae 0 report descriptor
                                        {
                                            //transmit data length
                                            if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDKEY_KeyRepDesc))
                                                u8LengthUsbtransBuff = sizeof(HIDKEY_KeyRepDesc);
                                            else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                            //transmit data ready
                                            u8CounterUsbTrans = 0;
                                            for(i=0;i<u8LengthUsbtransBuff;i++)
                                                u8ArrayUsbTransBuff[i] = HIDKEY_KeyRepDesc[i];
                                        }
                                        else //interface 1 report descriptor
                                        {
                                            //transmit data length
                                            if (u8ArrayUsbRecvData[USB_PACKET_wLengthL] > sizeof(HIDCOMP_RepDesc))
                                                u8LengthUsbtransBuff = sizeof(HIDCOMP_RepDesc);
                                            else u8LengthUsbtransBuff = u8ArrayUsbRecvData[USB_PACKET_wLengthL];
                                            //transmit data ready
                                            u8CounterUsbTrans = 0;
                                            for(i=0;i<u8LengthUsbtransBuff;i++)
                                                u8ArrayUsbTransBuff[i] = HIDCOMP_RepDesc[i];
                                        }

                                        X_EncodeUsbData();
                                        break;
                                    default:
                                        break;
                                }
                        }
                        break;
                    case USBRQ_DIR_HOST_TO_DEVICE:  //0x00
                        switch(u8ArrayUsbRecvData[USB_PACKET_bRequest])
                        {

                            case USBRQ_SET_ADDRESS:  //STEP 2
                                //log_info("USBRQ_SET_ADDRESS\n");
                                X_UsbDeviceAck();
                                u8UsbAddr = u8ArrayUsbRecvData[USB_PACKET_wValueL];
                                //transmit data ready,empty packet
                                u8LengthUsbtransBuff = 0;
                                u8CounterUsbTrans = 0;
                                X_EncodeUsbData();
                                break;
                            case USBRQ_SET_CONFIGURATION:  //STEP 12
                                //transmit data ready,empty packet
                                u8LengthUsbtransBuff = 0;
                                u8CounterUsbTrans = 0;
                                X_EncodeUsbData();
                                X_UsbDeviceAck();
                                break;
                            case USBRQ_CLEAR_FEATURE:
                                break;
                            case USBRQ_SET_FEATURE:
                                break;
                            default:
                                break;
                        }
                        break;
                    case USB_DESCRIPTOR_HID:  //0x21  //STEP 13 STEP 17 STEP19 STEP 22
                        //Class request OUT (0x09) 21 09 00 02 00 00 01 00, The last requrst--Class request OUT,PS-20220912
                        X_UsbDeviceAck();
                        //if (u8ArrayUsbRecvData[3]==0x09) u8UsbIdle = 0x80;  //usb hid enumeration process end
                        //transmit data ready,empty packet
                        u8LengthUsbtransBuff = 0;
                        u8CounterUsbTrans = 0;
                        X_EncodeUsbData();
                        break;
                    default:
                        break;
                }
            }
            else// if (u8StatusTransData == USB_PID_OUT)  //USB_PID_OUT://Data
            {
                if (u8UsbEPn == 2)  //only for test PS-20221214
                {
                    if (u8DecodeDataLength == 10)  //the last packet only include 6 bytes data  PS-20230101
                    {
                        for (i=0;i<6;i++)   u8ArrayEp2RecvTemp[u8Ep2RecvPacket*8 + i] = u8ArrayUsbRecvData[i+2];
                        u8Ep2RecvPacket = 4;
                    }
                    else
                    {
                        for (i=0;i<8;i++)   u8ArrayEp2RecvTemp[u8Ep2RecvPacket*8 + i] = u8ArrayUsbRecvData[i+2];
                        u8Ep2RecvPacket ++;
                    }
                    X_UsbDeviceAck();
                    //log_info("U-D0:%d",u8Ep2RecvPacket);

                }
                else
                {
                    //LED STATUS  PS-20220703
                    X_UsbDeviceAck();
                    //set led means enumeration process end PS-20220912
                    u8Ep2RecvPacket = 0;
                }
            }
            break;
        case USB_PID_DATA1:
            u8LastDATA0D1Status = USB_PID_DATA1;
            if (u8StatusTransData == USB_PID_OUT)
            {
                if (u8UsbEPn == 2)  //only for test PS-20221214
                {
                    if (u8DecodeDataLength == 10)  //the last packet only include 6 bytes data  PS-20230101
                    {
                        for (i=0;i<6;i++)   u8ArrayEp2RecvTemp[u8Ep2RecvPacket*8 + i] = u8ArrayUsbRecvData[i+2];
                        u8Ep2RecvPacket = 4;
                    }
                    else
                    {
                        for (i=0;i<8;i++)   u8ArrayEp2RecvTemp[u8Ep2RecvPacket*8 + i] = u8ArrayUsbRecvData[i+2];
                        u8Ep2RecvPacket ++;
                    }

                    X_UsbDeviceAck();
                    //log_info("U-D1:%d",u8Ep2RecvPacket);
                }
                else
                {
                    //LED STATUS  PS-20220703
                    X_UsbDeviceAck();
                    //set led means enumeration process end PS-20220912
                    u8Ep2RecvPacket = 0;
                }
            }
            break;
        case USB_PID_ACK:
            //Step 3:Only test usb receive data PS-20220619
            //log_info("USB_PID_ACK\n");
            if (u8StatusTransData == USB_PID_IN)  //EP0 & EP2
            {
              u8UsbPidAck = 0x00;
              //device transmit data to host is success
              if ((u8LengthUsbtransBuff - u8CounterUsbTrans) < 8 )  //complete transmition
              {
                    //if (u8UsbEPn == 2) log_info("U-tr-c: %d %d",u8LengthUsbtransBuff,u8CounterUsbTrans);
                    u8LengthUsbtransBuff = 0;
                    u8CounterUsbTrans = 0;
              }
              else  //continue transmit data
              {
                    u8CounterUsbTrans = u8CounterUsbTrans + 8;
                    //if (u8UsbTransBuffForEp != 1)X_EncodeUsbData();
                    X_EncodeUsbData();
                    //if (u8UsbEPn == 2) log_info("U-tr: %d %d",u8LengthUsbtransBuff,u8CounterUsbTrans);
              }
            }
            else if (u8StatusTransData == (USB_PID_IN|0x80))  //EP1
            {
                u8UsbPidAck = 0x01;
                log_info("USB_PID_ACK:EP1");
            }
            break;
        case USB_PID_NACK:
            //device transmit data to host is fail,retry
            break;
        default:
            break;
    }


    JL_TMR0->CON = BIT(1);
}

void UsbCalibrateClk()
{
    u8 i;
    u16 u16Timer1Cap0,u16Timer1Cap1;
    //Ñ¡Ôñinput channel1ÊäÈë
    // IOMC2[11 : 6]
    // 0 ~ 15: 		PA0 ~ PA15
    //16 ~ 27: 		PB0 ~ PB11
    //28 ~ 32: 		PD0 ~ PD4
    //33 ~ 34: 		TMR2PWM1/0
    INPUT_CHANNLE1_SRC_SEL(USB_DM_INT_IN_PORT);
    gpio_set_direction(USB_DM_INT_IN_PORT, 1);
    gpio_set_die(USB_DM_INT_IN_PORT, 1);
	IRFLT_OUTPUT_TIMER_SEL(IRFLT_OS_TIMER1);

    JL_IRFLT->CON = 0;  //clear
    JL_IRFLT->CON = (IRFLT_PSEL_1 | IRFLT_TSRC_SYS | IRFLT_EN_EN);		//irflt config


    //filter 5 times 1ms KA,PS-20220703
    JL_TMR1->CON = BIT(6);  //clear interrupt
    JL_TMR1->PRD = 0;
    JL_TMR1->CNT = 0;
    JL_TMR1->CON = (TIMER1_PSET_DIV4 | TIMER1_SSEL_LSB | TIMER1_MODE_FALL);  //div4 + lsb + falling edge

    //check KA is right
    for (i=0;i<10;i++)
    {
         while(!(JL_TMR1->CON & BIT(7)));//wait PND
        JL_TMR1->CON |= BIT(6);
        u16Timer1Cap0 = JL_TMR1->PRD;
        while(!(JL_TMR1->CON & BIT(7)));//wait PND
        JL_TMR1->CON |= BIT(6);
        u16Timer1Cap1 = JL_TMR1->PRD;

        if ( ((u16Timer1Cap1-u16Timer1Cap0) > 19800) && ((u16Timer1Cap1-u16Timer1Cap0) < 20200) )
        break;
    }


    log_info("KA time: %d,%d",i,u16Timer1Cap1-u16Timer1Cap0);

    X_TunePll(u16Timer1Cap1-u16Timer1Cap0);

    //test USB KA after tune pll
    JL_TMR1->CON = BIT(6);  //clear interrupt
    JL_TMR1->PRD = 0;
    JL_TMR1->CNT = 0;
    JL_TMR1->CON = (TIMER1_PSET_DIV4 | TIMER1_SSEL_LSB | TIMER1_MODE_FALL);  //div4 + lsb + falling edge

    while(!(JL_TMR1->CON & BIT(7)));//wait PND
    JL_TMR1->CON |= BIT(6);
    u16Timer1Cap0 = JL_TMR1->PRD;
    while(!(JL_TMR1->CON & BIT(7)));//wait PND
    JL_TMR1->CON = BIT(6);//stop Timer1 capture
    u16Timer1Cap1 = JL_TMR1->PRD;

    log_info("KA time after trimming: %d",u16Timer1Cap1-u16Timer1Cap0);
    ////////////////////////////////////////////////////////////////////


    USB_DELAYTIMER_STOP;
}
//////////////////////
//PS-20220504 capture test--PA1
//1.5MHz capture OK 20220507
void UsbCapInit()
{
    u8UsbIdle = 0x00;


    JL_PORTA->DIE |= 0x0002;
    JL_PORTA->DIR |= 0x0002;
    //gpio_set_direction(IO_PORTA_01,1);  //PA1 input
    //gpio_set_die(IO_PORTA_01,1);  //must set digital input!!! PS-20220507

    SFR(JL_IOMC->IOMC2, 0, 5, 1);  //PA01  IN_CH0_SEL
    SFR(JL_IOMC->IOMC1, 4, 2, 1);  //Mux to timer0  1--timer0 2--timer1 3--timer2
    SFR(JL_IOMC->IOMC1, 6, 1, 0);  //0--rising edge 1-- falling edge

    //timer0 input,MUX-INV-->TMR0,NO MUX--FLT-->TMR0
    //JL_IRFLT->CON = 0;

    JL_TMR0->CON = BIT(6);
    JL_TMR0->PRD = 0;
    JL_TMR0->CNT = 0;

    //capture usb D+, lsb/1,raising edge
    JL_TMR0->CON = BIT(1);

    HWI_Install(IRQ_TIME0_IDX, (u32)timer0_usb_cap_isr, 6);
}

void X_UsbInit()
{
    USB_SET_TO_INPUT;

    UsbCalibrateClk();

    USB_DELAYTIMER_INIT;

    UsbCapInit();
}
