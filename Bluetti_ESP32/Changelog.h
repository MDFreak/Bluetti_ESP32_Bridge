// - changelog --------------------------------------------------------------------------

#define CHANGELOG_H
/* MD0.1.2 - 2025-10-28 - add scanning of unknown modbus adresses of AC300
 * - add static IP config for device login and access point to BWifi.cpp
 *   -> works only once - but not stored in EEPROM
 * - move details as define to platformio.ini
 */// -----------------------------------------------------------------------------------
/* MD0.1.1 - 2025-10-28 - add scanning of unknown modbus adresses of AC300
 * - add fields with names including address - example ADR_0x0BF7_UINT
 *   files: Device_AC300.h, DeviceType.h, MQTT.cpp, BTooth.cpp
 */// -----------------------------------------------------------------------------------
/* MD0.1.0 - 2025-01-18 - md - extend functionality for AC300
 * - add and synchronize enums and fields in Device_AC300.h and DeviceType.h
 * - extend simulation in BTooth.cpp
 * - update and add evaluation of 'ENUM_FIELD' and 'DECIMAL_ARRAY'
 */// -----------------------------------------------------------------------------------
/* MD0.0.2 - 2025-01-13 - simuting Bluetti data for MQTT
 * - introduce simulation for BT to implement MQTT without Bluetti
 *   - new define SIM_BLUETTI (-> platform.ini)
 *     used to block unused BT functions
 *     and activate simulation function
 *   - simulation starts in function 'handleBluetooth()' and uses
 *     new function 'sendSIM_data()' to publish data
 *     uses standard decoding methods
 *   - works with 8 items
 *   - add '#include "MQTT.h"' to Bluetooth.cpp
 * - set default data for connections
 * - introduce simulation for BT to implement MQTT without Bluetti
 */// -----------------------------------------------------------------------------------
/* MD0.0.1 - 2025-01-11 - md - initial version
 * - new define USE_DISPLAY (-> platform.ini)
 *   ndef USE_DISPLAY = no display implemented
 * - move defines to platform.ini
 *   DISPLAYSSD1306 1
 *   DEBUGDISP 1
 *   DISPLAY_SCL_PORT 4
 *   DISPLAY_SDA_PORT 5
 *   DISPLAY_RST_PORT 16
 *   RELAISMODE 1
 *   RELAIS_PIN 22
 *   RELAIS_LOW LOW   // = 0
 *   RELAIS_HIGH HIGH // = 1
 * - add defines for BLUETTI_TYPE and DEVICE_NAME
 * - new define USE_DISPLAY (-> platform.ini)
 *   ndef USE_DISPLAY = no display implemented
 * - change code format to MD format for better readability

 *///------------------------------------------------------------------------------------
