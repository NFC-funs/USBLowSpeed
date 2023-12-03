#ifndef _USBKEY_H__
#define _USBKEY_H__

//HIDKEY_KeyRepDesc[]
s8 HIDKEY_KeyRepDesc[] =
{
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	0x09, 0x06, // USAGE (Keyboard)
	0xa1, 0x01, // COLLECTION (Application)
	0x05, 0x07, // USAGE_PAGE (Keyboard/Keypad)

    //1--the first byte(L/R ctrl,alt,shift,win)
	0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00, //     LOGICAL_MINIMUM (0)
	0x25, 0x01, //     LOGICAL_MAXIMUM (1)
	0x95, 0x08, //     REPORT_COUNT (8)
	0x75, 0x01, //     REPORT_SIZE (1)
	0x81, 0x02, //     INPUT (Data,Var,Abs)

	//2--the sencond byte is reserved,so it's const(Cnst)
	0x95, 0x01, //     REPORT_COUNT (1)
	0x75, 0x08, //     REPORT_SIZE (8)
	0x81, 0x03, //     INPUT (Cnst,Var,Abs)

	//3--six bytes
	//bug?-0x15,0x00,//LOGICAL_MINIMUM (0) PS-20220627
	0x95, 0x06, //   REPORT_COUNT (6)
	0x75, 0x08, //   REPORT_SIZE (8)
	0x15, 0x00, //   LOGICAL_MINIMUM (0) PS-20221205
	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	0x05, 0x07, //   USAGE_PAGE (Keyboard/Keypad) it can be removed PS-20221205
	0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
	0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
	0x81, 0x00, //   INPUT (Data,Ary,Abs)

    //4--the output byte,LED,5 bits(1-5)
	0x25, 0x01, //   LOGICAL_MAXIMUM (1)
	0x95, 0x05, //   REPORT_COUNT (5)
	0x75, 0x01, //   REPORT_SIZE (1)
	0x05, 0x08, //   USAGE_PAGE (LEDs)
	0x19, 0x01, //   USAGE_MINIMUM (Num Lock) Num Lock -- Caps Lock -- Scroll Lock -- Compose -- Kana
	0x29, 0x05, //   USAGE_MAXIMUM (Kana)
	0x91, 0x02, //   OUTPUT (Data,Var,Abs)

	//5--the output byte,the other 3 bits
	0x95, 0x01, //   REPORT_COUNT (1)
	0x75, 0x03, //   REPORT_SIZE (3)
	0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)

	0xC0        //   END_COLLECTION
};
//composite -- HID keyboard & HID compliant device PS-20221204
//copy from CH559&558
s8 HIDCOMP_RepDesc[] =
{
	/* USER CODE BEGIN 0 */
	0x06,0xA0,0xFF,       //用法页(FFA0h, vendor defined)
	0x09, 0x01,           //用法(vendor defined)
	0xA1, 0x01,           //集合(Application)
	//0x09, 0x02 ,          //用法(vendor defined)
	//0xA1, 0x00,           //集合(Physical)
	//0x06,0xA1,0xFF,       //用法页(vendor defined)
	//输入报告
	//0x85,0x01,
	0x09, 0x03 ,          //用法(vendor defined)
	0x09, 0x04,           //用法(vendor defined)
	0x15, 0x00,//0x80,           //逻辑最小值(0x80 or -128)
	0x25, 0xFF,//0x7F,           //逻辑最大值(0x7F or 127)
	//0x35, 0x00,           //物理最小值(0)
	//0x45, 0xFF,           //物理最大值(255)
	0x75, 0x08,           //报告长度Report size (8位)
	0x95, 30,             //报告数值(64 fields)
	0x81, 0x02,           //输入(data, variable, absolute)
	//输出报告
	// 0x85, 0x02,
	0x09, 0x05,           //用法(vendor defined)
	0x09, 0x06,           //用法(vendor defined)
	0x15, 0x00,//0x80,           //逻辑最小值(0x80 or -128)
	0x25, 0xFF,//0x7F,           //逻辑最大值(0x7F or 127)
	//0x35, 0x00,           //物理最小值(0)
	//0x45, 0xFF,           //物理最大值(255)
	0x75, 0x08,           //报告长度(8位)
	0x95, 30,             //报告数值(64 fields)
	0x91, 0x02,           //输出(data, variable, absolute)
	0xC0	              //集合结束(Physical)
    /*
    0x06, 0x00,0xff,//Usage Page
    0x09, 0x01,     //Usage
    0xa1, 0x01,     // COLLECTION (Application)

    0x09, 0x02,     //Usage
    0x15, 0x00,     //Logical  Minimun
    0x26, 0x00,0xff,//Logical  Maximun
    0x75, 0x08,     //Report Size
    0x95, 0x40,     //Report Count
    0x81, 0x06,     //Input

    0x09, 0x02,     //Usage
    0x15, 0x00,     //Logical  Minimun
    0x26, 0x00,0xff,//Logical  Maximun
    0x75, 0x08,     //Report Size
    0x95, 0x40,     //Report Count
    0x91, 0x06,     //Output
    0xC0*/

	/*
    0x05,0x01, // USAGE_PAGE (Generic Desktop)

    0x09,0x00, // USAGE (undefine)
    0xa1,0x01, // COLLECTION (Application)

	0x09,0x02, //PS-20221215
	0x15,0x00, //   LOGICAL_MINIMUM (0)
    0x25,0xff, //   LOGICAL_MAXIMUM (255)
    //must include 0x19&29???  PS-20221215
    //add 0x09 0x02,dont need 0x19&0x29 PS-20221215
	//0x19,0x00,//0x01, //   USAGE_MINIMUM
    //0x29,0xff,//0x08, //   USAGE_MAXIMUM
    0x75,0x08,  //   REPORT_SIZE (8 bit)
    0x95,0x40,  //0x08,  //   REPORT_COUNT (8)
    0x81,0x02,
    //output report,Host send count bytes once time  //PS-20221213
    0x09,0x02,
    0x15,0x00,
    0x25,0xff,
    0x75,0x08,  //   REPORT_SIZE (8 bit)
    0x95,0x40,  //   REPORT_COUNT (8)
    0x91,0x02,  //0x06,  //  OUTPUT(0/1 Data/Cnst Ary/Var Abs/Rlv)  //PS-20221215
    0xC0
	*/
};

//STEP 1
//reset
//setup
//Host -- > Device
//USBbmRequestType = 0x80    Get_Configuaration
//USBbRequest = 0x06         Get_Descriptor
//USBwValue = 0x01           Device Descriptor
//USBwIndex = 0x00
//USBwLength = 0x40          Max Bytes
//Device -- > Host
//HIDKEY_DevDesc[18]
/*Standard Device Descriptor*/
#if 1
s8 HIDKEY_DevDesc[18] = {
    0x12,                /*bLength,Device Descriptor 18 bytes*/
    0x01,                /*bDescriptorType,01--Device Descriptor*/
    0x00,0x01,           /*bcdUSB,0100--USB 1.0*/
    0x00,                /*bDeviceClass,00-Device*/
    0x00,                /*bDeviceSubClass,00*/
    0x00,                /*bDeviceProtocol,00*/
    0x08,                /*bMaxPacketSize0,8*/
    0x6F,0x54  //0x89,0x11,//0x83,0x04,           /*dVendor*/
    0x79,0x6E  //0x90,0x88,//0x10,0x57,           /*idProduct*/
    0x20,0x06,//0x00,0x01,           /*bcdDevice Ver-1.00*/
    0x01,                /*index of string descriptor describing Manufacturer--iManufacturer*/
    0x02,                /*index of string descriptor describing Product--iProduct*/
    0x03,                /*index of string descriptor describing SerialNumber--iSerialNumber*/
    0x01                 /*bNumConfigurations*/
    };
#endif

//STEP 2
//reset
//setup
//Host -- > Device
//USBbmRequestType = 0x00    Set_Address
//USBbRequest = 0x05
//USBwValue = 0x0004         Address = 0x04
//USBwIndex = 0x00
//USBwLength = 0x00
//ACK & 0 data packet-PS-20220703


//STEP 3
//setup
//Host -- > Device
//USBbmRequestType = 0x80    Get_Configuaration
//USBbRequest = 0x06		   Get_Descriptor
//USBwValue = 0x01 		   Device Descriptor
//USBwIndex = 0x00
//USBwLength = 0x12
//Device -- > Host
//HIDKEY_DevDesc[18]

//STEP 4
//setup
//Host -- > Device
//USBbmRequestType = 0x80    Get_Configuaration
//USBbRequest = 0x06 		   Get_Descriptor
//USBwValue = 0x02           Configuaration + Interface + String + Endpoint Descriptor
//USBwIndex = 0x00
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_CfgDesc[41]
#if 1
#define USB_INTERVAL_DEFAULT    32
s8 HIDKEY_CfgDesc[9+32+32] =
{
    0x09,                /*bLength,Configuration Descriptor 9 bytes*/
    0x02,                /*bDescriptorType,02--Configuration Descriptor*/
    (9+32+32),0x00,           /*wTotalLength,9+25+32 bytes*/
    0x02,                /*bNumInterfaces*/ //2 interfaces
    0x01,                /*bConfigurationValue*/
    0x00,                /*iConfiguration*/
    0x80,                /*bmAttributes,Bit7-Bus power Bit6-Self power Bit5-Remote wakeup*/
    0x32,                /*MaxPower,*2mA*/
    //
    0x09,                /*bLength,Interface Descriptor 9 bytes*/
    0x04,                /*bDescriptorType,04--Interface Descriptor*/
    0x00,                /*bInterfaceNumber*/  //interface 0
    0x00,                /*bAlternateSetting*/
    0x02,                /*bNumEndpoints*/  //PS-20220626 0x01-->0x02
    0x03,                /*bInterfaceClass,03-HID*/
    0x01,                /*bInterfaceSubClass,00-no boot 01-boot*/
    0x01,                /*bInterfaceProtocol,0-no 1-keyboard 02-mouse*/
    0x00,                /*iInterface*/
    //
    0x09,                /*bLength,HID Descriptor 9 bytes*/
    0x21,                /*bDescriptorType,21--HID Descriptor*/
    0x00,0x01,           /*bcdHID,0100--Ver1.00*/
    0x00,                /*bCountryCode*/
    0x01,                /*bNumDescriptors*/
    0x22,                /*bDescriptorType,22--HID REPORT DESC*/
    sizeof(HIDKEY_KeyRepDesc),0x00,           /*wDescriptorLength,Report Descritor length*/
    //
    0x07,                /*bLength,Endpoint Descriptor 7 bytes*/
    0x05,                /*bDescriptorType,05--Endpoint Descriptor*/
    0x81,                /*bEndpointAddress,bit7(0-OUT 1-IN);bit3:0 the endpoint numer*/
    0x03,                /*bmAttributes,03-Interrupt endpoint(00-control 01--isochronous 10-bulk 11-interrupt)*/
    0x08,0x00,           /*bMaxPacketSize0,8*/
    USB_INTERVAL_DEFAULT,//0x20,                /*bInterval,*1ms*/
    //must include OUT Endpoint PS-20220626
    0x07,                /*bLength,Endpoint Descriptor 7 bytes*/
    0x05,                /*bDescriptorType,05--Endpoint Descriptor*/
    0x01,                /*bEndpointAddress,bit7(0-OUT 1-IN);bit3:0 the endpoint numer*/
    0x03,                /*bmAttributes,03-Interrupt endpoint(00-control 01--isochronous 10-bulk 11-interrupt)*/
    0x01,0x00,           /*bMaxPacketSize0,1*/
    USB_INTERVAL_DEFAULT,//0x20                 /*bInterval,*1ms*/

    //compliant device PS-20221204
    0x09,0x04,0x01,0x00,0x02,0x03,0x00,0x00,0x00,//interface 1
    0x09,0x21,0x00,0x01,0x00,0x01,0x22,sizeof(HIDCOMP_RepDesc),0x00,
    0x07,0x05,0x82,0x03,0x08,0x00,USB_INTERVAL_DEFAULT,
    0x07,0x05,0x02,0x03,0x08,0x00,USB_INTERVAL_DEFAULT
};
#endif

//STEP 5
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x6
//USBwValue = 0x0300         03:String_descripotr 00:Index
//USBwIndex = 0x00
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_StringLangID[4]
s8 HIDKEY_StringLangID[4] =
{
    0x04,                /*bLength,String Descriptor 4 bytes*/
    0x03,                /*bDescriptorType,03--String Descriptor*/
    0x09,0x04  //0409--US English
};

//STEP 6
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0302          03:String_descripotr 02:Index
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_StringProduct[32]
//unicode
s8 HIDKEY_StringProduct[] =
{
    //0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '5', 0, '9', 0
    /*
    0x12,
    0x03,
    'P',
    'S',
    ' ',
    'H',
    'I',
    'D',
    ' ',
    'R',
    'E',
    'A',
    'D',
    'E',
    'R',
    '-',
    '0',
    '2'
    */

    0x22,
    0x03,
    'P', 0,
    'S', 0,
    ' ', 0,
    'H', 0,
    'I', 0,
    'D', 0,
    ' ', 0,
    'W', 0,
    '&', 0,
    'R', 0,
    '-', 0,
    'M', 0,
    '1', 0,
    '-', 0,
    '0', 0,
    '2', 0

};

//STEP 7
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0303          03:String_descripotr 03:Index
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_StringSerial[]
//length error?08004300--PS-20220627
s8 HIDKEY_StringSerial[] =
{
    //0x0E,0x03,'k',0,'e',0,'y',0,'5',0,'5',0,'9',0

    0x1E,
    0x03,
    'P', 0,
    'S', 0,
    ' ', 0,
    'H', 0,
    'I', 0,
    'D', 0,
    '-', 0,
    '0', 0,
    '0', 0,
    '1', 0, //6
    '5', 0, //5
    '2', 0, //5
    '0', 0, //3
    '4', 0  //6

};

//STEP 8
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x06
//USBwIndex = 0x00
//USBwLength = 0xa
//Dont support,STALL--PS-20220703


//STEP 9
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x01
//USBwIndex = 0x00
//USBwLength = 0x12
//Device -- > Host
//HIDKEY_DevDesc[18]

//STEP 10
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x02
//USBwIndex = 0x00
//USBwLength = 0x09
//Device -- > Host
//HIDKEY_CfgDesc[] first 9 bytes

//STEP 11
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x02
//USBwIndex = 0x00
//USBwLength = 0x29
//Device -- > Host
//HIDKEY_CfgDesc[41]

//STEP 12
//setup
//Host -- > Device
//USBbmRequestType = 0x00
//USBbRequest = 0x09            set_configuration
//USBwValue = 0x0001            select configuration
//USBwIndex = 0x00              set current configuration = 0x01
//USBwLength = 0x00
//ACK&empty packet-PS-20220703

//STEP 13
//setup
//Host -- > Device
//USBbmRequestType = 0x21       HID
//USBbRequest = 0x0a            set_idle
//USBwValue = 0x00
//USBwIndex = 0x00
//USBwLength = 0x00
//ACK&empty packet-PS-20220703

//STEP 14
//setup
//Host -- > Device
//USBbmRequestType = 0x81      Interface_request
//USBbRequest = 0x06
//USBwValue = 0x22             Report_descriptor
//USBwIndex = 0x00
//USBwLength = 0x7d
//Device -- > Host
//HIDKEY_KeyRepDesc[]


//STEP 15
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0301
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
///*iManufacturer*/
s8 HIDKEY_StringManufacturer[] =
{
    //0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0

    0x10,
    0x03,
    'T', 0,
    'h', 0,
    'e', 0,
    'W', 0,
    'o', 0,
    'r', 0,
    'k', 0

};

//STEP 16
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0303
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_StringSerial[]

//STEP 17
//setup
//Host -- > Device
//USBbmRequestType = 0x21
//USBbRequest = 0x0a
//USBwValue = 0x00
//USBwIndex = 0x00
//USBwLength = 0x00

//STEP 18
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x06
//USBwIndex = 0x00
//USBwLength = 0x0a
//Dont support,STALL--PS-20220703

//STEP 19
//setup
//Host -- > Device
//USBbmRequestType = 0x21
//USBbRequest = 0x0a
//USBwValue = 0x00
//USBwIndex = 0x00
//USBwLength = 0x00

//STEP 20
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0301
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
///*iManufacturer*/

//STEP 21
//setup
//Host -- > Device
//USBbmRequestType = 0x80
//USBbRequest = 0x06
//USBwValue = 0x0303
//USBwIndex = 0x0409
//USBwLength = 0xff
//Device -- > Host
//HIDKEY_StringSerial[]

//STEP 22
//setup
//Host -- > Device
//USBbmRequestType = 0x21
//USBbRequest = 0x0a
//USBwValue = 0x00
//USBwIndex = 0x00
//USBwLength = 0x00

#endif
