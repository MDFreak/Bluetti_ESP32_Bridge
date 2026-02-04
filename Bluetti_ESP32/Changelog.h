// - changelog --------------------------------------------------------------------------
#define CHANGELOG_H
/* MD0.1.3 - 2026-01-31 - improve WiFi connection
 * KNOWN ISSUES
 * - Config page will not start, because config is always valid
 *   Change of configuration only via recompile
 *   - option is actually not necessary for me
 * --------------------------------------------------------------------------------------
 * TODO
 * - complete and test new SW parts for support new Bluetti APEX300 and Handsfree2
 * - fix issue above to ESP usable in different systems without recompile
 * --------------------------------------------------------------------------------------
 * Functional changes
 * --------------------------------------------------------------------------------------
 * - ESP8266 not supported anymore -> will be removed
 *
 * - system change in wifi management and static IP configuration
 *   - flash storage use is changed and stabilized using static IP configuration
 *     wifi config and access point use local net 10.0.x.x
 *     Device static IP: 10.0.0.<last byte from platformio.ini DEV_IP3>
 *                   GW: 10.0.139
 *                   SN: 255.255.255.0
 *     AP SSID: Bluetti_ESP32_AP / PW: ElaNanniRalf3
 *                AP IP: 10.0.1.1
 *            config IP: 10.0.1.2
 *                   SN: 255.255.255.0
 *
 *   - struct ESPBluettiSettings - typedef is moved from BWifi.h to config.h
 *     - defines all items of WiFi and configPortal
 *     - initialized default values
 *       exeption: last part of 'static_ip' is omitted -> is to be defined from platformio.ini
 *     - all items will be saved in EEPROM (-> 'static_ip' complete)
 *
 *   - fork of WiFiManager(tzapu) to WiFiManager_MD(MDFreak = me) to implement necessary functions
 *
 * - introduce switching AC300 versus AP300 (=APEX300) and Handsfree 2 (both Bluetti BMS models)
 *   - file names changed DeviceType.h -> DeviceType_AC300.h, DeviceType_AP300.h, DeviceType_Handsfree2.h
 *
 *   TODO needs to be completed

 * - files changed platformio.ini, BWifi.h, BWifi.cpp, BTooth.cpp, config.h, BluettiConfig.h, MQTT.cpp
 *                 PayloadParser.h, MQTT.h, Bluetti_ESP32.ino, Changelog.h, WiFiManager.h, WiFiManager.cpp
 *                 del DeviceType.h,
 *                 add DeviceType_AC300.h, DeviceType_AP300.h, DeviceType_Handsfree2.h
 */// -----------------------------------------------------------------------------------
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
