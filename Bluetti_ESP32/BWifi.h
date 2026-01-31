/* -- BWifi.h -- MD0.0.1----------------------------------------------------------------------*/
#ifndef BWIFI_H
  #define BWIFI_H

  #include "Arduino.h"
  #include "config.h"
//char bluetti_device_id[40] = "e.g. ACXXXYYYYYYYY";

  extern ESPBluettiSettings get_esp32_bluetti_settings();
  extern void initBWifi(bool resetWifi = false);
  extern void handleWebserver();
  String processorWebsiteUpdates(const String& var);
  extern void AddtoMsgView(String data);
#endif
/* MD0.1.3 - 2025-12-13 - add Bluetti APEX300 support
 * - struct ESPBluettiSettings - typedef is moved from BWifi.h to config.h
 *   - defines all items of WiFi and configPortal
 *   - initialized default values
 *     exeption: last part of 'static_ip' is omitted -> is to be defined from platformio.ini
 *   - all items will be saved in EEPROM (-> 'static_ip' complete)
 */// -----------------------------------------------------------------------------------
/* MD0.0.1 - 2025-01-11 - md - initial version
 * - new define USE_DISPLAY (-> platform.ini)
 *   ndef USE_DISPLAY = no display implemented
 * - change code format to MD format for better readability
 *///------------------------------------------------------------------------------------
