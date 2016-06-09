/*
The USB HID report descriptor is partially obtained from an existing UPS, except the vendor specific sections have been removed
Some sections are also commented out to simplify things, less reports to handle
*/

PROGMEM const uint8_t usbHidReportDescriptor[] = { /* USB report descriptor, size must match usbconfig.h */
    0x05, 0x84,        // Usage Page (Power Pages) // Power Device
    0x09, 0x04,        // Usage (0x04) // UPS
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x24,        //   Usage (0x24) // PowerSummary
    0xA1, 0x00,        //   Collection (Physical)
    0x85, 0x01,        //     Report ID (1)
    0x09, 0xFE,        //     Usage (0xFE) // iProduct
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x02,        //     Report ID (2)
    0x09, 0xFF,        //     Usage (0xFF) // iserialNumber
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x03,        //     Report ID (3)
    0x05, 0x85,        //     Usage Page (Power Pages) // Battery System
    0x09, 0x89,        //     Usage (0x89) // iDeviceChemistery
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x04,        //     Report ID (4)
    0x09, 0x8F,        //     Usage (0x8F) // iOEMInformation
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x05,        //     Report ID (5)
    0x09, 0x8B,        //     Usage (0x8B) // Rechargable
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x06,        //     Report ID (6)
    0x09, 0x2C,        //     Usage (0x2C) // CapacityMode
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x07,        //     Report ID (7)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x06,        //     Report Count (6)
    0x25, 0x64,        //     Logical Maximum (100)
    0x09, 0x83,        //     Usage (0x83) // DesignCapacity
    0x09, 0x8D,        //     Usage (0x8D) // CapacityGranularity1
    0x09, 0x8E,        //     Usage (0x8E) // CapacityGranularity2
    0x09, 0x8C,        //     Usage (0x8C) // WarningCapacityLimit
    0x09, 0x29,        //     Usage (0x29) // RemainingTimeLimit
    0x09, 0x67,        //     Usage (0x67) // FullChargeCapacity
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x08,        //     Report ID (8)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x65, 0x00,        //     Unit (None)
    0x09, 0x66,        //     Usage (0x66) // RemainingCapacity
    0x81, 0x23,        //     Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x09, 0x66,        //     Usage (0x66) // RemainingCapacity
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x68,        //     Usage (0x68) // RunTimeToEmpty
    0x75, 0x10,        //     Report Size (16)
    0x27, 0xFF, 0xFF, 0x00, 0x00,  //     Logical Maximum (65535)
    0x66, 0x01, 0x10,  //     Unit (System: SI Linear, Time: Seconds)
    0x81, 0x23,        //     Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x09, 0x68,        //     Usage (0x68) // RunTimeToEmpty
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x2A,        //     Usage (0x2A) // RemainingTimeLimit 
    0x26, 0x58, 0x02,  //     Logical Maximum (600)
    0x81, 0x23,        //     Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x09, 0x2A,        //     Usage (0x2A) // RemainingTimeLimit 
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x85, 0x09,        //     Report ID (9)
    0x75, 0x08,        //     Report Size (8)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x05, 0x84,        //     Usage Page (Power Pages)
    0x09, 0x40,        //     Usage (0x40)
    0x67, 0x21, 0xD1, 0xF0, 0x00,  //     Unit (System: SI Linear, Length: Radians, Mass: Gram)
    0x55, 0x06,        //     Unit Exponent (6)
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x0A,        //     Report ID (10)
    0x09, 0x30,        //     Usage (0x30)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x02,        //     Usage (0x02)
    0xA1, 0x02,        //     Collection (Logical)
    0x65, 0x00,        //       Unit (None)
    0x55, 0x00,        //       Unit Exponent (0)
    0x85, 0x0B,        //       Report ID (11)
    0x75, 0x01,        //       Report Size (1)
    0x95, 0x06,        //       Report Count (6)
    0x25, 0x01,        //       Logical Maximum (1)
    0x05, 0x85,        //       Usage Page (Power Pages) // Actually Battery System
    0x09, 0xD0,        //       Usage (0xD0) // ACPresent
    0x09, 0x44,        //       Usage (0x44) // Charging
    0x09, 0x45,        //       Usage (0x45) // Discharging
    0x09, 0x42,        //       Usage (0x42) // BelowRemainingCapacityLimit
    0x09, 0x46,        //       Usage (0x46) // FullyCharged
    0x09, 0x43,        //       Usage (0x43) // RemainingTimeLimitExpired
    0x81, 0x23,        //       Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x09, 0xD0,        //       Usage (0xD0) // ACPresent
    0x09, 0x44,        //       Usage (0x44) // Charging
    0x09, 0x45,        //       Usage (0x45) // Discharging
    0x09, 0x42,        //       Usage (0x42) // BelowRemainingCapacityLimit
    0x09, 0x46,        //       Usage (0x46) // FullyCharged
    0x09, 0x43,        //       Usage (0x43) // RemainingTimeLimitExpired
    0xB1, 0xA3,        //       Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x75, 0x02,        //       Report Size (2)
    0x95, 0x01,        //       Report Count (1)
    0x81, 0x01,        //       Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xB1, 0x01,        //       Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              //     End Collection
    0x85, 0x0C,        //     Report ID (12)
    0x05, 0x84,        //     Usage Page (Power Pages)
    0x09, 0x5A,        //     Usage (0x5A) // AudibleAlarmControl
    0x75, 0x08,        //     Report Size (8)
    0x15, 0x01,        //     Logical Minimum (1)
    0x25, 0x03,        //     Logical Maximum (3)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x5A,        //     Usage (0x5A) // AudibleAlarmControl
    0x81, 0x22,        //     Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x85, 0x0D,        //     Report ID (13)
    0x09, 0xFD,        //     Usage (0xFD)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0xC0,              //   End Collection
    0x05, 0x84,        //   Usage Page (Power Pages)
    0x09, 0x1A,        //   Usage (0x1A) // Input
    0xA1, 0x00,        //   Collection (Physical)
    0x85, 0x0E,        //     Report ID (14)
    0x05, 0x84,        //     Usage Page (Power Pages)
    0x09, 0x40,        //     Usage (0x40)
    0x75, 0x08,        //     Report Size (8)
    0x67, 0x21, 0xD1, 0xF0, 0x00,  //     Unit (System: SI Linear, Length: Radians, Mass: Gram)
    0x55, 0x07,        //     Unit Exponent (7)
    0xB1, 0x23,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
    0x85, 0x0F,        //     Report ID (15)
    0x75, 0x10,        //     Report Size (16)
    0x09, 0x30,        //     Usage (0x30)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    /*
    0x85, 0x10,        //     Report ID (16)
    0x09, 0x53,        //     Usage (0x53)
    0x15, 0x4E,        //     Logical Minimum (78)
    0x25, 0x58,        //     Logical Maximum (88)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x53,        //     Usage (0x53)
    0x81, 0x23,        //     Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x09, 0x54,        //     Usage (0x54)
    0x16, 0x88, 0x00,  //     Logical Minimum (136)
    0x26, 0x8E, 0x00,  //     Logical Maximum (142)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x54,        //     Usage (0x54)
    0x81, 0x23,        //     Input (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    //*/
    0xC0,              //   End Collection
    0x09, 0x1C,        //   Usage (0x1C) // Output
    0xA1, 0x00,        //   Collection (Physical)
    0x85, 0x12,        //     Report ID (18)
    0x09, 0x30,        //     Usage (0x30)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x75, 0x08,        //     Report Size (8)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x65, 0x00,        //     Unit (None)
    0x55, 0x00,        //     Unit Exponent (0)
    0x85, 0x13,        //     Report ID (19)
    0x09, 0x35,        //     Usage (0x35)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    /*
    0x85, 0x14,        //     Report ID (20)
    0x09, 0x58,        //     Usage (0x58)
    0x25, 0x06,        //     Logical Maximum (6)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x58,        //     Usage (0x58)
    0x81, 0x22,        //     Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x85, 0x15,        //     Report ID (21)
    0x09, 0x57,        //     Usage (0x57)
    0x75, 0x10,        //     Report Size (16)
    0x15, 0xFF,        //     Logical Minimum (255)
    0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    0x35, 0xC4,        //     Physical Minimum (196)
    0x47, 0xC4, 0xFF, 0x1D, 0x00,  //     Physical Maximum (1966020)
    0x66, 0x01, 0x10,  //     Unit (System: SI Linear, Time: Seconds)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    //*/
    /*
    0x85, 0x16,        //     Report ID (22)
    0x09, 0x56,        //     Usage (0x56)
    0xB1, 0xA2,        //     Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x85, 0x17,        //     Report ID (23)
    0x09, 0x6E,        //     Usage (0x6E)
    0x75, 0x01,        //     Report Size (1)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x35, 0x00,        //     Physical Minimum (0)
    0x45, 0x00,        //     Physical Maximum (0)
    0x65, 0x00,        //     Unit (None)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x09, 0x65,        //     Usage (0x65)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    0x75, 0x06,        //     Report Size (6)
    0xB1, 0x01,        //     Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x18,        //     Report ID (24)
    0x75, 0x10,        //     Report Size (16)
    0x09, 0x44,        //     Usage (0x44)
    0x26, 0x84, 0x03,  //     Logical Maximum (900)
    0x66, 0x21, 0xD1,  //     Unit (System: SI Linear, Length: Radians, Mass: Gram)
    0x55, 0x07,        //     Unit Exponent (7)
    0xB1, 0xA3,        //     Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
    //*/
    0xC0,              //   End Collection
    0xC0,              // End Collection
};