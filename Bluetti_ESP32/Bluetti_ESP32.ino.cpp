/* -- BTooth.cpp -- MD0.0.2----------------------------------------------------------------------*/
#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "config.h"
#ifdef USE_DISPLAY
    #include "display.h"
  #endif
unsigned long lastTime1 = 0;
unsigned long timerDelay1 = 3000;

void setup()
  {
    Serial.begin(115200);
    // for development only - circle simulation of Bluetti comunication
    #ifdef SIM_BLUETTI
        Serial.print(millis());
        Serial.println(" start setup");
      #endif
    #ifdef RELAISMODE
        pinMode(RELAIS_PIN, OUTPUT);
        #ifdef DEBUG
            Serial.println(F("deactivate relais contact"));
          #endif
        digitalWrite(RELAIS_PIN, RELAIS_LOW);
      #endif
    #ifdef SLEEP_TIME_ON_BT_NOT_AVAIL
        esp_sleep_enable_timer_wakeup(SLEEP_TIME_ON_BT_NOT_AVAIL * 60 * 1000000ULL);
      #endif
    // for use of local display
    #ifdef USE_DISPLAY
        #ifdef DISPLAYSSD1306
            initDisplay();
          #endif
      #endif
    #ifdef DEBUG
        Serial.println(F("vor initBWifi"));
      #endif
    // initialize communication
    initBWifi(false);
    #ifdef DEBUG
        Serial.println(F("nach initBWifi"));
      #endif
    initBluetooth();
    initMQTT();
    #ifdef USE_DISPLAY
        #ifdef DISPLAYSSD1306
            wrDisp_Status("Running!");
          #endif
      #endif
    #ifdef SIM_BLUETTI
        Serial.print(millis());
        Serial.println(" end setup");
      #endif
  }
void loop()
  {
    #ifdef USE_DISPLAY
        #ifdef DISPLAYSSD1306
            handleDisplay();
          #endif
      #endif
    handleBluetooth();
    handleMQTT();
    handleWebserver();
  }

// - changelog --------------------------------------------------------------------------
  /* MD0.1.1 - 2025-10-28 - add scanning of unknown modbus adresses of AC300
   * - add fields with names including address - example ADR_0x0BF7_UINT
   *   files: Device_AC300.h, DeviceType.h, MQTT.cpp, BTooth.cpp
   *///------------------------------------------------------------------------------------
  /* MD0.1.0 - 2025-01-18 - md - extend functionality for AC300
   * - add and synchronize enums and fields in Device_AC300.h and DeviceType.h
   * - extend simulation in BTooth.cpp
   * - update and add evaluation of 'ENUM_FIELD' and 'DECIMAL_ARRAY'
   *///------------------------------------------------------------------------------------
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
   *///-----------------------------------------------------------------------------------
  /* MD0.0.1 - 2025-01-11 - md - initial version
   * - new define USE_DISPLAY (-> platform.ini)
   *   ndef USE_DISPLAY = no display implemented
   * - change code format to MD format for better readability
   *///------------------------------------------------------------------------------------
