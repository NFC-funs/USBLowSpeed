#ifndef _USB_H__
#define _USB_H__


/* USB Token values */
//(upper 4-bits) = ~(lower 4-bits)
#define USB_PID_OUT    0xE1
#define USB_PID_IN     0x69
#define USB_PID_SOF    0xA5
#define USB_PID_SETUP  0x2D

#define USB_PID_PRE    0x3C

#define USB_PID_DATA0  0xC3
#define USB_PID_DATA1  0x4B

#define USB_PID_ACK    0xD2
#define USB_PID_NACK   0x5A
#define USB_PID_STALL  0x1E

/* USB direction values: */
#define USBRQ_DIR_MASK              0x80
#define USBRQ_DIR_HOST_TO_DEVICE    (0<<7)
#define USBRQ_DIR_DEVICE_TO_HOST    (1<<7)
#define USBRQ_DIR_DEVICE_TO_HOST_INTERFACE 0x01
#define USBRQ_DIR_DEVICE_TO_HOST_ENDPOINT  0x02

/* USB Standard Requests */
#define USBRQ_GET_STATUS              0
#define USBRQ_CLEAR_FEATURE           1
#define USBRQ_SET_FEATURE             3
#define USBRQ_SET_ADDRESS             5
#define USBRQ_GET_DESCRIPTOR          6
#define USBRQ_SET_DESCRIPTOR          7
#define USBRQ_GET_CONFIGURATION       8
#define USBRQ_SET_CONFIGURATION       9
#define USBRQ_GET_INTERFACE          10
#define USBRQ_SET_INTERFACE          11
#define USBRQ_SYNCH_FRAME            12

/* USB descriptor constants */
#define USB_DESCRIPTOR_DEVICE                         1
#define USB_DESCRIPTOR_CONFIGURATION                  2
#define USB_DESCRIPTOR_STRING                         3
#define USB_DESCRIPTOR_INTERFACE                      4
#define USB_DESCRIPTOR_ENDPOINT                       5
#define USB_DESCRIPTOR_DEVICE_QUALIFIER               6
#define USB_DESCRIPTOR_OTHER_SPEED_CONFIGURATION      7
#define USB_DESCRIPTOR_INTERFACE_POWER1               8
#define USB_DESCRIPTOR_HID                         0x21
#define USB_DESCRIPTOR_HID_REPORT                  0x22
#define USB_DESCRIPTOR_HID_PHYS                    0x23

/* USB HID Requests */
#define USBRQ_HID_GET_REPORT    0x01
#define USBRQ_HID_GET_IDLE      0x02
#define USBRQ_HID_GET_PROTOCOL  0x03
#define USBRQ_HID_SET_REPORT    0x09
#define USBRQ_HID_SET_IDLE      0x0a
#define USBRQ_HID_SET_PROTOCOL  0x0b

//usb packet description PS-20221116
#define USB_PACKET_SYNC             0
#define USB_PACKET_PID              1
#define USB_PACKET_bmRequestType    2
#define USB_PACKET_bRequest         3
#define USB_PACKET_wValueL          4
#define USB_PACKET_wValueH          5
#define USB_PACKET_wIndexL          6
#define USB_PACKET_wIndexH          7
#define USB_PACKET_wLengthL         8
#define USB_PACKET_wLengthH         9


//12 = sync(1 byte) + pid(1 byte) + data(8 bytes)+crc16(2 bytes)
//4  = sync(1 byte) + pid(1 byte) + Addr(7bit) + EP(4 bit) + CRC5(5 bit)
//usb packet recv&trans////////////////////////////////////////////////
u8 u8ArrayUsbRecvData[13];
u8 u8ArrayUsbTransData[13];
///////////////////////////////////////////////////////////////////////

//PS-20221019
//release + 5bytes ID to 10key press and release + enter press and release
//Personal ID use first 6bytes
#define USB_HID_KEY_MAX_LEN  (8 + 16*16*2 + 16)
u8 u8ArrayUsbTransBuff[USB_HID_KEY_MAX_LEN];//Personal ID 8bytes to 16 key --PS 20221019
//u8-->u16 for 16 keys PS-20230108
u16 u8LengthUsbtransBuff;
u16 u8CounterUsbTrans;
//////////////////////////////////

u8 u8StatusTransData;
u8 u8LastDATA0D1Status;
////////////////////////////////////////////////////////////////////////////


//PS-20220622,init Addr&EP
//EPn recv&trans///////////////////////////////////////////////////////////
u8 u8UsbAddr;// = 0;  //
u8 u8UsbEPn;// = 0;  //

//hid(EP2) communicate 30 bytes for host receive,it include 4 packet
u8 u8ArrayEp2RecvTemp[32];//[64];  //PS-20221218
//u8 u8ArrayEp2TransTemp[32];//[64];  //PS-20221218
u8 u8Ep2RecvPacket;  //PS-20221218
u8 u8UsbTransBuffForEp;  //For EP1/EP2 PS-20230102
u8 u8UsbPidAck;  //PS-20230108
///////////////////////////////////////////////////////////////////////////

//PS-20220912,it means mcu can do others
u8 u8UsbIdle;
//bit0 idle
u8 u8USBLoop;  //main loop counter


void X_UsbInit();
void X_EncodeUsbData();


u8 u8ArrayUsbReaderCfg[8];

u8 X_XorSetConfig();
void X_CryptSetConfig();
void X_UsbEp2Func();

#endif
