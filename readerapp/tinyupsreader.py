#!/usr/bin/env python

'''
A simple script to obtain debug from the TinyUPS
The pywinusb implementation isn't reliable
The libusb implementation requires a libusb driver to be installed
'''

import pywinusb.hid as hid
import usb.core
import usb.util

def pywinusb_read_values():
    all_devices = hid.HidDeviceFilter().get_devices()

    if not all_devices:
        print("pywinusb can't find any non system HID device connected")
        return False
    else:
        for device in all_devices:
            try:
                device.open()
                usage_found = False
                for report in device.find_feature_reports():
                    target_usage = hid.get_full_usage_id(0xFFAB, 0x20)
                    if target_usage in report:
                        report.get()
                        print("RPT DEBUG: {0}".format(report.get_raw_data()))
                        usage_found = True
                        target_usage = hid.get_full_usage_id(0x85, 0x66)
                        if target_usage in report:
                            report.get()
                            print("RPT ID 0x08: {0}".format(report.get_raw_data()))
                            usage_found = True
                        target_usage = hid.get_full_usage_id(0x85, 0xD0)
                        if target_usage in report:
                            report.get()
                            print("RPT ID 0x0B: {0}".format(report.get_raw_data()))
                            usage_found = True
            finally:
                device.close()
        if not usage_found:
            print("pywinusb target device was found, but the requested usage does not exist!\n")
            return False
        return True

def libusb_read_values():
    dev = usb.core.find(idVendor=0x16C0, idProduct=0x03E8)
    if dev is None:
        print "libusb cannot find the device"
        return False
        dev.set_configuration()
    try:
        rpt11 = dev.ctrl_transfer(0xA1, 0x01, 0x030B, 0x0000, 2)
        rpt8 = dev.ctrl_transfer(0xA1, 0x01, 0x0308, 0x0000, 6)
        rpt32 = dev.ctrl_transfer(0xA1, 0x01, 0x0320, 0x0000, 7)
    except:
        print "libusb error during ctrl_transfer"
        return False
    print ''.join('{:02X} '.format(x) for x in rpt11)
    print ''.join('{:02X} '.format(x) for x in rpt8) + ": " + str(rpt8[1]) + " , " + str(rpt8[2] + (rpt8[3] << 8)) + " , " + str(rpt8[4] + (rpt8[5] << 8))
    print ''.join('{:02X} '.format(x) for x in rpt32) + ": " + str(rpt32[1] + (rpt32[2] << 8)) + " , " + str(rpt32[3] + (rpt32[4] << 8))
    return True

if __name__ == '__main__':
    #pywinusb_read_values()
    libusb_read_values()
