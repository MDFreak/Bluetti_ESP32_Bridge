/* -- BWifi.h -- MD0.0.1----------------------------------------------------------------------*/
#ifndef CONFIG_H
  #define CONFIG_H
  #include "Arduino.h"

  #define WIFI_SSID "MAMD-HomeG"
  #define WIFI_PASS "ElaNanniRalf3"

  // MDFREAK-0.0.1 moved to platform.ini
    #ifndef DEBUG
      #define DEBUG 1
    #endif
    // Display config section, comment DISPLAYSSD1306 to disable display
      //#define DISPLAYSSD1306 1
      //#define DEBUGDISP 1
      //#define DISPLAY_SCL_PORT 4
      //#define DISPLAY_SDA_PORT 5
    //Uncomment to toggle display reset on start, required for displays like LoRa TTGO v1.0
      //#define DISPLAY_RST_PORT 16
    // relais config
      //#define RELAISMODE 1
      //#define RELAIS_PIN 22
      //#define RELAIS_LOW LOW // = 0
      //#define RELAIS_HIGH HIGH // = 1
  //#define EEPROM_SALT 13374

  #define DEVICE_NAME "BLUETTI-MQTT" // MDFREAK-0.0.1
  #if (AP300_ACTIVE > 0)
      #define BLUETTI_TYPE AP300
    #endif
  #if (HANDSFREE_ACTIVE > 0)
      #define BLUETTI_TYPE HANDSFREE2
    #endif
  #if (AP300_ACTIVE > 0)
      #define BLUETTI_TYPE AC300
    #endif

  #ifndef SIM_BLUETTI
      #define BLUETOOTH_QUERY_MESSAGE_DELAY 5000
    #else
      #define BLUETOOTH_QUERY_MESSAGE_DELAY 10000
    #endif
  #define BLUETOOTH_MAX_RETRIES_BEFORE_REBOOT 10
  #define BLUETOOTH_SCAN_DURATION_IN_SECONDS 10
  #define BLUETOOTH_SCAN_INTERVAL_IN_SECONDS 10

  #define MAX_DISCONNECTED_TIME_UNTIL_REBOOT 5 //device will reboot when wlan/BT/MQTT is not connectet within x Minutes
  #define SLEEP_TIME_ON_BT_NOT_AVAIL 2 //device will sleep x minutes if restarted is triggered by bluetooth error
                                       //set to 0 to disable
  #define DEVICE_STATE_UPDATE  5
  #define MSG_VIEWER_DETAILS 0 //enable detailed BT/MQTT messages via WebUI by default, can be changed in WebUI
  #define DEVICE_STATE_STATUS_UPDATE  2.5 //Was 0.5 in original branc which is half the DEVICE_STATE_UPDATE value, kept the ratio
  #define MSG_VIEWER_ENTRY_COUNT 20 //number of lines for web message viewer
  #define MSG_VIEWER_REFRESH_CYCLE 5 //refresh time for website data in seconds

  typedef struct
    {
      //int  salt = EEPROM_SALT;
      //char mqtt_server[40] = "127.0.0.1";
      //char bluetti_device_id[40] = "AC300";
      char mqtt_server[40]       = "10.0.0.111";
      char mqtt_port[6]          = "1883";
      char mqtt_username[40]     = "MQTT";
      char mqtt_password[40]     = "MQTT-Tiny";
      char api_token[45]         = "wdZs0Hfij/xLLguSQ4EA5O6gtlIH0As5So1YfjBh8xA=";
      #if (AC300_ACTIVE > 0)
          char bluetti_device[40]    = "AC300";
          char bluetti_device_id[40] = "AC3002235000574654";
        #endif
      #if (AP300_ACTIVE > 0)
          char bluetti_device_id[40] = "AP300";
          char bluetti_device_id[40] = "AP3002519000710250";
        #endif
      #if (HANDSFREE_ACTIVE > 0)
          char bluetti_device_id[40] = "Handsfree";
          char bluetti_device_id[40] = "Handsfree 22446000191559";
        #endif
      char ota_username[40]      = "Tiny";
      char ota_password[40]      = "Tiny-OTA";
      char static_ip[16]         = "10.0.0.";
      char static_gw[16]         = "10.0.0.139";
      char static_sn[16]         = "255.255.255.0";
      char ap_static_ip[16]      = "10.0.1.2";
      char ap_static_gw[16]      = "10.0.1.1";
      char ap_static_sn[16]      = "255.255.255.0";
      char wifi_ssid[32]         = "MAMD-HomeG";
      char wifi_password[32]     = "ElaNanniRalf3";
      char config_saved[3]       = "01";
    } ESPBluettiSettings;

#endif
/* - changelog --------------------------------------------------------------------------
/* MD0.1.3 - 2025-12-13 - add Bluetti APEX300 support
 * - struct ESPBluettiSettings - typedef is moved from BWifi.h to config.h
 *   - defines all items of WiFi and configPortal
 *   - initialized default values
 *     exeption: last part of 'static_ip' is omitted -> is to be defined from platformio.ini
 *   - all items will be saved in EEPROM (-> 'static_ip' complete)
 */// -----------------------------------------------------------------------------------
/* MD0.0.1 - 2025-01-11 - md - initial version
 *
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
 */// -----------------------------------------------------------------------------------
