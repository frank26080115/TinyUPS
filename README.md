TinyUPS
=======

A small 12V Uninterruptible Power Supply that implements a standard USB interface using an Atmel ATtiny85 microcontroller.

This is supposed to only serve as a bare minimum implementation. Plenty of future improvements can be made to this design to suite other needs.

Please see http://www.eleccelerator.com/TinyUPS-simple-backup-power-for-NAS/ for details about my implementation.

Project Goal
------------

 * Cleanly shutdown my NAS in the event of a power outage

Implicit Objectives:

 * Interface with a cheap lead acid battery charger to determine status of battery and status of mains power
 * Read voltage from a sealed lead acid battery to estimate battery life
 * Implement standard USB battery system according to official HID specs on power related usage pages
 * Ensure the USB implementation is compatible with the OS on my QNAP brand NAS
 * Must utilize V-USB from Objective Development https://www.obdev.at/products/vusb/

Secondary Objectives:

 * Compatibility with Windows OS, maybe Linux
 * Extensible code that can be easily ported to ATmega
 * Extensible code that can support additional features in the future, such as load measurement and more accurate predictions

Hardware Requirements
---------------------

A rough schematic is provided for the bare minimum implementation. Do not build the circuit verbatim.

 * Circuitry consisting of a ATtiny85 microcontroller, the passive components required by V-USB, and components to determine power status
   * Adafruit's Gemma or Trinket may work
 * Voltage divider with protection diode that converts 13.5V to 2.56V
 * Cheap lead acid battery charger that uses AC from the wall, has a sufficient rating for the NAS, and features LED indicators
 * Buck-Boost DC/DC converter that supplies a steady 12V to the NAS

License
-------

GNU GPL V3, please see License.txt in project root directory.

Possible Improvements
---------------------

 * Current measurement, perhaps even digital fuel gauge
 * Intelligent self calibration
 * Real time clock
 * Temperature readout, compensate battery capacity according to temperature
 * Charge and discharge control
 * Indicators and displays
 * Interface with solar systems
 * Multi-cell management
 * Port to LUFA or ARM Cortex M