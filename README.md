# Water Temperature Sensors

This project aims at building a network of internet-connected water temperature
sensors around the Zürichsee using the Particle Electron.

The prototype is a regular indoor temperature sensor using the Particle Photon.


## Photon Setup

Install the Particle CLI: https://docs.particle.io/core/cli/

Log in to your Particle account:

    $ make login

Set up your device:

    $ make setup


## Flashing

To flash the firmware to a Particle Photon, first list the available Photons:

    $ make list

Then flash the current directory through the cloud:

    $ make DEVICE=<0123456789ABCDEFGHI> flash-ota

...or via USB:

    $ make DEVICE=<0123456789ABCDEFGHI> flash-usb

(Replace the device id with the id found in the first command output.)


## Viewing temperature

To display the measured temperature:

    $ make DEVICE=<0123456789ABCDEFGHI> temp


## Wiring

To wire a DS18B20 sensor to the Photon:

- VCC (usually red) -> 3.3V
- GND (usually black or blue) -> GND
- Data (usually yellow or white) -> D0
- 4.7k resistor from Data to VCC


## Links

- DS18B20 Datasheet: http://neutrino.phys.ksu.edu/~gahs/doublechooz/DC_SlowMRS/DS/DS18B20.pdf
- Particle Firmware: https://github.com/spark/firmware/tree/latest
