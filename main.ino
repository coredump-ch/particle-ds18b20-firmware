// Water Temperature Sensor Project.
//
// This firmware supports DS18S20, DS18B20 and DS1822 temperature sensors.
// Tested oly with DS18B20.
//
// Code is based on example code by Joe Goggins, see `LICENSE.md` for more
// information.

#include "OneWire.h"

OneWire ds(D0);  // on pin D0 (a 4.7K pull up resistor is necessary)

void setup(void) {
    Serial.begin(57600);
}

void loop(void) {

    // Counter variable
    byte i;
    // Whether or not the sensor is present
    byte present = 0;
    // The sensor type
    byte type_s;
    // Buffer to read data
    byte data[12];
    // Buffer to read address
    byte addr[8];
    // Variable to hold temperature in celsius
    float celsius;

    if (!ds.search(addr)) {
        Serial.println("No more addresses.");
        Serial.println();
        ds.reset_search();
        delay(250);
        return;
    }

    Serial.print("ROM =");
    for(i = 0; i < 8; i++) {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
    }

    if (OneWire::crc8(addr, 7) != addr[7]) {
          Serial.println("CRC is not valid!");
          return;
    }
    Serial.println();

    // the first ROM byte indicates which chip
    switch (addr[0]) {
        case 0x10:
            Serial.println("  Chip = DS18S20");  // or old DS1820
            type_s = 1;
            break;
        case 0x28:
            Serial.println("  Chip = DS18B20");
            type_s = 0;
            break;
        case 0x22:
            Serial.println("  Chip = DS1822");
            type_s = 0;
            break;
        default:
            Serial.println("Device is not a DS18x20 family device.");
            return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    // Maybe 750ms is enough, maybe not. Let's do 1000ms.
    // We might do a `ds.depower()` here, but the reset will take care of it.
    delay(1000);

    // Send a 1-Wire reset cycle. Returns 1 if a device responds with a
    // presence pulse. Returns 0 if there is no device or the bus is shorted
    // or otherwise held low for more than 250uS.
    present = ds.reset();

    ds.select(addr);
    ds.write(0xBE); // Read data from scratchpad memory

    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
    for (i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

    // Convert the data to actual temperature.
    // Because the result is a 16 bit signed integer, it should be stored to an
    // "int16_t" type, which is always 16 bits even when compiled on a 32 bit
    // processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        // default is 12 bit resolution, 750 ms conversion time
    }
    celsius = (float)raw / 16.0;
    Serial.print("  Temperature = ");
    Serial.print(celsius);
    Serial.println(" Celsius");
}
