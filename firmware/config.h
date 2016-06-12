#ifndef _CONFIG_H_
#define _CONFIG_H_

// define features here

#define ENABLE_UPS_REPORTS // must be enabled or else device won't be a UPS, disable only to test ADCs
//#define ENABLE_DEBUG_REPORTS // reports ADC values via a vendor specific report
#define ENABLE_TEST_SHUTDOWN // uses a spare pin to test if device will accept a low battery level to shutdown
//#define ALLOW_WRITE // allows the host to write values via SETUP OUT transactions
#define USE_SOF_FOR_OSC_CAL // use with ATtiny85
//#define USE_SOF_FOR_SCHEDULING // assumes SOF is 1ms apart
#define USE_TIMER_FOR_SCHEDULING // uses a AVR internal timer for scheduling
//#define USE_AUTO_SCALE // use only if you really don't want to actually calibrate
#define FAKE_CYBERPOWER // some devices need to be on an "approved list", enabling this will pretend to be a CyberPower CP850PFCLCD as much as possible
//#define FAKE_ALWAYS_FULL // only used for testing without the risk of unexpectedly shutting down the PC

// below are some hardware settings that can be changed without modifying main code

// ADC channels for the battery and status pins
#define BATT_CHAN 2
#define STATUS_CHAN 1

#define USB_CFG_IOPORTNAME      B
/* This is the port where the USB bus is connected. When you configure it to
 * "B", the registers PORTB, PINB and DDRB will be used.
**/
#define USB_CFG_DMINUS_BIT      0
/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
 * This may be any bit in the port.
**/
#define USB_CFG_DPLUS_BIT       1
/* This is the bit number in USB_CFG_IOPORT where the USB D+ line is connected.
 * This may be any bit in the port. Please note that D+ must also be connected
 * to interrupt pin INT0!
**/

/* The interrupt needs to be configured correctly
 * keeping in mind that USB_SOF_COUNT requires a different configuration
**/
#define USB_INTR_CFG            PCMSK
#define USB_INTR_CFG_SET        (1 << USB_CFG_DMINUS_BIT)
#define USB_INTR_CFG_CLR        0
#define USB_INTR_ENABLE         GIMSK
#define USB_INTR_ENABLE_BIT     PCIE
#define USB_INTR_PENDING        GIFR
#define USB_INTR_PENDING_BIT    PCIF
#define USB_INTR_VECTOR         PCINT0_vect

// USB identification parameters below

#ifdef FAKE_CYBERPOWER
#define  USB_CFG_VENDOR_ID       0x64, 0x07 /* CyberPower */
#else
#define  USB_CFG_VENDOR_ID       0xC0, 0x16 /* = 0x16c0 = 5824 = voti.nl */
/* USB vendor ID for the device, low byte first. If you have registered your
 * own Vendor ID, define it here. Otherwise you may use one of obdev's free
 * shared VID/PID pairs. Be sure to read USB-IDs-for-free.txt for rules!
 * *** IMPORTANT NOTE ***
 * This template uses obdev's shared VID/PID pair for Vendor Class devices
 * with libusb: 0x16c0/0x5dc.  Use this VID/PID pair ONLY if you understand
 * the implications!
**/
#endif

#ifdef FAKE_CYBERPOWER
#define  USB_CFG_DEVICE_ID       0x01, 0x05 /* CyberPower CP850PFCLCD */
#else
#define  USB_CFG_DEVICE_ID       0xE8, 0x03 /* VOTI's lab use PID */
/* This is the ID of the product, low byte first. It is interpreted in the
 * scope of the vendor ID. If you have registered your own VID with usb.org
 * or if you have licensed a PID from somebody else, define it here. Otherwise
 * you may use one of obdev's free shared VID/PID pairs. See the file
 * USB-IDs-for-free.txt for details!
 * *** IMPORTANT NOTE ***
 * This template uses obdev's shared VID/PID pair for Vendor Class devices
 * with libusb: 0x16c0/0x5dc.  Use this VID/PID pair ONLY if you understand
 * the implications!
**/
#endif
#define USB_CFG_DEVICE_VERSION  0x00, 0x01
/* Version number of the device: Minor number first, then major number.
**/
#ifdef FAKE_CYBERPOWER
#define USB_CFG_VENDOR_NAME     'C', 'P', 'S'
#define USB_CFG_VENDOR_NAME_LEN 3
#else
#define USB_CFG_VENDOR_NAME     'e', 'l', 'e', 'c', 'c', 'e', 'l', 'e', 'r', 'a', 't', 'o', 'r', '.', 'c', 'o', 'm'
#define USB_CFG_VENDOR_NAME_LEN 17
#endif
/* These two values define the vendor name returned by the USB device. The name
 * must be given as a list of characters under single quotes. The characters
 * are interpreted as Unicode (UTF-16) entities.
 * If you don't want a vendor name string, undefine these macros.
 * ALWAYS define a vendor name containing your Internet domain name if you use
 * obdev's free shared VID/PID pair. See the file USB-IDs-for-free.txt for
 * details.
**/
#ifdef FAKE_CYBERPOWER
#define USB_CFG_DEVICE_NAME     'C', 'P', '8', '5', '0', 'P', 'F', 'C', 'L', 'C', 'D'
#define USB_CFG_DEVICE_NAME_LEN 11
#else
#define USB_CFG_DEVICE_NAME     'U', 'P', 'S'
#define USB_CFG_DEVICE_NAME_LEN 3
#endif
/* Same as above for the device name. If you don't want a device name, undefine
 * the macros. See the file USB-IDs-for-free.txt before you assign a name if
 * you use a shared VID/PID.
**/
#ifdef FAKE_CYBERPOWER
#define USB_CFG_SERIAL_NUMBER   '0', '0', '0', '0', '0', '0', '0', '0'
#define USB_CFG_SERIAL_NUMBER_LEN   8
#else
#define USB_CFG_SERIAL_NUMBER   '0', '0', '0', '1'
#define USB_CFG_SERIAL_NUMBER_LEN   4
#endif
/* Same as above for the serial number. If you don't want a serial number,
 * undefine the macros.
 * It may be useful to provide the serial number through other means than at
 * compile time. See the section about descriptor properties below for how
 * to fine tune control over USB descriptors such as the string descriptor
 * for the serial number.
**/

#define USB_CFG_DEVICE_CHEMISTRY        'L', 'e', 'a', 'd', ' ', 'A', 'c', 'i', 'd'
#define USB_CFG_DEVICE_CHEMISTRY_LEN    9
#define USB_CFG_OEM_INFO                'L', 'e', 'a', 'd', ' ', 'A', 'c', 'i', 'd'
#define USB_CFG_OEM_INFO_LEN            9

// these are read only constants

#define CONSTANT_INPUT_VOLTAGE  110
#define CONSTANT_OUTPUT_VOLTAGE 110
#define CONSTANT_OUTPUT_PERCENT_LOAD 50 // TODO: this can be dynamic in a more advanced implementation
#define CONSTANT_OUTPUT_BATTERY_LOAD 0.3 // unit is in C, TODO: this can be dynamic in a more advanced implementation

#endif
