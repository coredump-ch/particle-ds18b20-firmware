// Water Temperature Sensor Project.
//
// This firmware supports DS18B20 and DS1822 temperature sensors.
// Tested oly with DS18B20.
//
// Code is based on example code by Joe Goggins, see `LICENSE.md` for more
// information.

#include "OneWire.h"

OneWire ds(D0);  // on pin D0 (a 4.7K pull up resistor is necessary)

// Variable to hold raw data
String rawHexData = "uninitialized";

// Variable to hold temperature in celsius
double celsius;

// Buffer to read address
byte addr[8];

void setup(void) {
    // Initialize the serial console
    Serial.begin(57600);

    // Expose some variables
    Particle.variable("temperature", &celsius, DOUBLE);
    Particle.variable("raw", &rawHexData, STRING);

    // Log initialization
    Particle.publish("photon-initialized", NULL, 60, PRIVATE);

    // Write configuration (11 bit resolution)
    while (ds.search(addr)) {
        ds.reset();
        ds.select(addr);
        ds.write(0x4E); // Write scratchpad
        ds.write(0x55); // Set T_H to 85°C
        ds.write(0xFF); // Set T_L to -0.5°C
        //ds.write(0x1F | (0 << 5)); // Set resolution to 9 bit
        //ds.write(0x1F | (1 << 5)); // Set resolution to 10 bit
        //ds.write(0x1F | (2 << 5)); // Set resolution to 11 bit
        ds.write(0x1F | (3 << 5)); // Set resolution to 12 bit
    }

}

void loop(void) {

    // Counter variable
    byte i;
    // Whether or not the sensor is present
    byte present = 0;
    // Whether or not the sensor reading was successful
    byte success = 0;
    // Buffer to read data
    byte data[12];
    // CRC value
    uint8_t crc;

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
        case 0x28:
            Serial.println("  Chip = DS18B20");
            break;
        case 0x22:
            Serial.println("  Chip = DS1822");
            break;
        default:
            Serial.println("Device is not a DS18x20 family device.");
            return;
    }

    ds.reset();
    ds.select(addr);

    // Start conversion
    ds.write(0x44);

    // Maybe 750ms is enough, maybe not. Let's do 1000ms.
    // We might do a `ds.depower()` here, but the reset will take care of it.
    delay(1000);

    // Send a 1-Wire reset cycle. Returns 1 if a device responds with a
    // presence pulse. Returns 0 if there is no device or the bus is shorted
    // or otherwise held low for more than 250uS.
    present = ds.reset();

    // Request data from scratchpad memory
    ds.select(addr);
    ds.write(0xBE);

    // Read returned bytes
    ds.read_bytes(data, 9);

    // Calculate CRC value
    crc = OneWire::crc8(data, 8);

    // Set rawHexData variable
    rawHexData = String::format("Data = %02x %02x %02x %02x %02x %02x %02x %02x %02x CRC = %02x",
                                data[0], data[1], data[2], data[3],
                                data[4], data[5], data[6], data[7],
                                data[8], crc);

    // Format of the data (page 7 of datasheet):
    //
    //  data[0] -> Temperature LSB
    //  data[1] -> Temperature MSB
    //  data[2] -> T_H register or user byte 1
    //  data[3] -> T_L register or user byte 2
    //  data[4] -> Configuration register
    //  data[5] -> Reserved (FFh)
    //  data[6] -> Reserved (0Ch)
    //  data[7] -> Reserved (10h)
    //  data[8] -> CRC

    // Verify CRC code
    if (data[8] != crc) {
        Particle.publish("crc-error", "scratchpad", 300, PRIVATE);
        delay(1000);
        return;
    }

    // Convert the data to actual temperature.
    // Because the result is a 16 bit signed integer, it should be stored to an
    // "int16_t" type, which is always 16 bits even when compiled on a 32 bit
    // processor.
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);

    // At lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) { // 9 bit resolution, 93.75 ms conversion time
        // Clear the lowest 3 bits
        raw = raw & ~7;
    } else if (cfg == 0x20) { // 10 bit res, 187.5 ms conversion time
        // Clear the lowest 2 bits
        raw = raw & ~3;
    } else if (cfg == 0x40) { // 11 bit res, 375 ms conversion time
        // Clear the lowest bit
        raw = raw & ~1;
    }
    // default is 12 bit resolution, 750 ms conversion time

    // Show temperature
    if (raw == 0x550) {
        Serial.println("  Temperature reading error.");
        success = 0;
    } else {
        celsius = (double)raw / 16.0;
        Serial.print("  Temperature = ");
        Serial.print(celsius);
        Serial.println(" Celsius");
        success = 1;
    }

    // Sleep
    if (success) {
        delay(5000);
    } else {
        // Try again immediately
    }
}
