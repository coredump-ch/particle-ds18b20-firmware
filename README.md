# Water Temperature Sensors


## Flashing

To flash the firmware to a Particle Photon, first list the available Photons:

    $ particle list

Then flash the current directory:

    $ particle flash <0123456789ABCDEFGHI> .

(Replace the device id with the id found in the first command output.)


## Wiring

To wire a DS18B20 sensor to the Photon:

- VCC (usually red) -> 3.3V
- GND (usually black or blue) -> GND
- Data (usually yellow or white) -> D0
- 4.7k resistor from Data to VCC
