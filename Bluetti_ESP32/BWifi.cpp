/* -- BWifi.cpp -- MD0.0.2----------------------------------------------------------------------*/
#include "BluettiConfig.h"
#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "index.h"  //Web page header file
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
#include <AsyncTCP.h> // https://github.com/me-no-dev/AsyncTCP/archive/master.zip
#include <ESPmDNS.h>
#include <ElegantOTA.h> // https://github.com/ayushsharma82/AsyncElegantOTA/archive/master.zip
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#ifdef USE_DISPLAY
    #include "display.h"
  #endif

////char mqtt_server[40] = "127.0.0.1";
//char mqtt_server[40] = "10.0.0.111";  // md 0.1.0
//char mqtt_port[6]  = "1883";
//char api_token[45] = "wdZs0Hfij/xLLguSQ4EA5O6gtlIH0As5So1YfjBh8xA=";
////default custom static IP
//char static_ip[16] = "10.0.0.";
//strcat(static_ip, DEV_IP3);
//char static_gw[16] = "10.0.0.139";
//char static_sn[16] = "255.255.0.0";

//flag for saving data
bool shouldSaveConfig = false;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTimeWebUpdate = 0;

String lastMsg = "";

bool msgViewerDetails = false;

/* defined in BWifi.h
  //char bluetti_device_id[40] = "e.g. ACXXXYYYYYYYY";
  #if (AC300_ACTIVE > 0)
      char bluetti_device_id[40] = "AC3002235000574654";  // md 0.1.0
    #endif
  #if (AP300_ACTIVE > 0)
      char bluetti_device_id[40] = "AP3002519000710250";  // md 0.1.0
    #endif
  #if (HANDSFREE_ACTIVE > 0)
      char bluetti_device_id[40] = "Handsfree22446000191559";  // md 0.1.0
    #endif
 */
ESPBluettiSettings wifiConfig;
ESPBluettiSettings wifiConfigDefault;
const int16_t      EEPROM_SIZE = sizeof(ESPBluettiSettings);
const char*        CONFIG_SAVED_VALUE = "5A";

WiFiManager        wifiManager;
int                wifiReconnectCounter = 0;
bool               portalRunning        = false;
bool               startAP              = false; // start AP and webserver if true, else start only webserver
//WiFiManagerParameter custom_bluetti_device("bluetti", "Bluetti Bluetooth ID", bluetti_device_id, 40);

WiFiManagerParameter custom_mqtt_server       ("server",       "MQTT Server Address",wifiConfig.mqtt_server,       40);
WiFiManagerParameter custom_mqtt_port         ("port",         "MQTT Server Port",   wifiConfig.mqtt_port,         6);
WiFiManagerParameter custom_mqtt_username     ("username",     "MQTT Username",      wifiConfig.mqtt_username,     40);
WiFiManagerParameter custom_mqtt_password     ("password",     "MQTT Password",      wifiConfig.mqtt_password,     40, "type=password");
WiFiManagerParameter custom_api_token         ("api_token",    "API Token",          wifiConfig.api_token,         45);
#if (HANDSFREE_ACTIVE > 0)
    WiFiManagerParameter custom_bluetti_device("bluetti",      "Handsfree2",         wifiConfig.bluetti_device_id, 40);
  #endif
#if (AP300_ACTIVE > 0)
    WiFiManagerParameter custom_bluetti_device("bluetti",      "AP300",              wifiConfig.bluetti_device_id, 40);
  #endif
#if (AC300_ACTIVE > 0)
    WiFiManagerParameter custom_bluetti_device("bluetti",      "AC300",              wifiConfig.bluetti_device_id, 40);
  #endif
WiFiManagerParameter custom_bluetti_device_id ("bluetti_id",   "Bluetti Device ID",  wifiConfig.bluetti_device_id, 40);
WiFiManagerParameter custom_ota_username      ("ota_username", "OTA Username",       wifiConfig.ota_username,      40);
WiFiManagerParameter custom_ota_password      ("ota_password", "OTA Password",       wifiConfig.ota_password,      40, "type=password");
WiFiManagerParameter custom_static_ip         ("static_ip",    "Static IP",          wifiConfig.static_ip,         16);
WiFiManagerParameter custom_static_gw         ("static_gw",    "Static Gateway",     wifiConfig.static_gw,         16);
WiFiManagerParameter custom_static_sn         ("static_sn",    "Static Subnet",      wifiConfig.static_sn,         16);
WiFiManagerParameter custom_ap_static_ip      ("ap_static_ip", "AP Static IP",       wifiConfig.ap_static_ip,         16);
WiFiManagerParameter custom_ap_static_gw      ("ap_static_gw", "AP Static Gateway",  wifiConfig.ap_static_gw,         16);
WiFiManagerParameter custom_ap_static_sn      ("ap_static_sn", "AP Static Subnet",   wifiConfig.ap_static_sn,         16);
WiFiManagerParameter custom_wifi_ssid         ("wifi_ssid",    "WiFi SSID",          wifiConfig.wifi_ssid,         32);
WiFiManagerParameter custom_wifi_password     ("wifi_password","WiFi Password",      wifiConfig.wifi_password,     32, "type=password");
WiFiManagerParameter custom_config_saved      ("config_saved", "Config Saved",       wifiConfig.config_saved,      32, "type=password");
//-----------------------
//callback notifying us of the need to save config
void saveConfigCallback ()
  {
    Serial.println("set Flag shouldSaveConfig");
    shouldSaveConfig = true;
  }

ESPBluettiSettings get_esp32_bluetti_settings()
  {
    return wifiConfig;
    //return wifiConfig;
  }

bool isConfigValid()
  {
    if (strcmp(wifiConfig.config_saved, CONFIG_SAVED_VALUE) == 0)
      {
        Serial.println("  config_saved Flag is valid");
        return true;
      }
    else
      {
        Serial.println("  ERR config_saved Flag is not valid");
        return false;
      }
  }

void eeprom_saveconfig()
  {
    if (shouldSaveConfig) // update configfrom weberver (wifi manager)
      {
        Serial.println("update wifiConfig from WiFiManager");
        strlcpy(wifiConfig.mqtt_server,   custom_mqtt_server.getValue(),   40);
        strlcpy(wifiConfig.mqtt_port,     custom_mqtt_port.getValue(),     6);
        strlcpy(wifiConfig.mqtt_username, custom_mqtt_username.getValue(), 40);
        strlcpy(wifiConfig.mqtt_password, custom_mqtt_password.getValue(), 40);
        strlcpy(wifiConfig.api_token,     custom_api_token.getValue(),     45);
        strlcpy(wifiConfig.bluetti_device,custom_bluetti_device.getValue(),40);
        strlcpy(wifiConfig.ota_username,  custom_ota_username.getValue(),  40);
        strlcpy(wifiConfig.ota_password,  custom_ota_password.getValue(),  40);
        strlcpy(wifiConfig.static_ip,     custom_static_ip.getValue(),     16);
        strlcpy(wifiConfig.static_gw,     custom_static_gw.getValue(),     16);
        strlcpy(wifiConfig.static_sn,     custom_static_sn.getValue(),     16);
        strlcpy(wifiConfig.wifi_ssid,     custom_wifi_ssid.getValue(),     32);
        strlcpy(wifiConfig.wifi_password, custom_wifi_password.getValue(), 32);
      }
    else
      {
        Serial.println("wifiConfig not updated from WiFiManager");
      }
    strcpy(wifiConfig.config_saved, CONFIG_SAVED_VALUE); // mark config as saved

    #ifdef DEBUG_BWIFI
        Serial.println("  configuration to save:");
        Serial.print(F("  MQTT Server:       ")); Serial.println(wifiConfig.mqtt_server);
        Serial.print(F("  MQTT Port:         ")); Serial.println(wifiConfig.mqtt_port);
        Serial.print(F("  API Token:         ")); Serial.println(wifiConfig.api_token);
        Serial.print(F("  MQTT Username:     ")); Serial.println(wifiConfig.mqtt_username);
        Serial.print(F("  MQTT Password:     ")); Serial.println(wifiConfig.mqtt_password);
        Serial.print(F("  Bluetti Device:    ")); Serial.println(wifiConfig.bluetti_device);
        Serial.print(F("  Bluetti Device ID: ")); Serial.println(wifiConfig.bluetti_device_id);
        Serial.print(F("  OTA Username:      ")); Serial.println(wifiConfig.ota_username);
        Serial.print(F("  OTA Password:      ")); Serial.println(wifiConfig.ota_password);
        Serial.print(F("  Static IP:         ")); Serial.println(wifiConfig.static_ip);
        Serial.print(F("  Static GW:         ")); Serial.println(wifiConfig.static_gw);
        Serial.print(F("  Static SN:         ")); Serial.println(wifiConfig.static_sn);
        Serial.print(F("  WiFi SSID:         ")); Serial.println(wifiConfig.wifi_ssid);
        Serial.print(F("  WiFi Password:     ")); Serial.println(wifiConfig.wifi_password);
        Serial.print(F("  config_saved Flag: ")); Serial.println(wifiConfig.config_saved);
      #endif

    Serial.println(F("  Saving Values to EEPROM"));
    if (!EEPROM.begin(EEPROM_SIZE))
      {
        Serial.println("    ERR Failed to initialize EEPROM");
        return;
      }
    else;
      {
        EEPROM.put(0, wifiConfig);
        if (EEPROM.commit())
          {
            Serial.println("    SUCCESS save config");
          }
        else
          {
            Serial.println("    ERR failed to save Config");
          }
      }
    EEPROM.end();
  }
void eeprom_readconfig(bool resetWifi)
  {
    bool save_config = resetWifi;
    Serial.println(F("  Loading config from EEPROM"));
    if (!EEPROM.begin(EEPROM_SIZE))
      {
        Serial.println("    ERR Failed to initialize EEPROM");
        return;
      }
    else
      {
        EEPROM.get(0, wifiConfig);
        if (isConfigValid())
          {
            Serial.println("    SUCCESS load config");
          }
        else
          {
            Serial.println("    ERR corrupted config in EEPROM !! -> use default config");
            // use default values from wifiConfigDefault
            wifiConfig = wifiConfigDefault;
            save_config = true;
          }
      }
    if (save_config)
      {
        eeprom_saveconfig();
      }
    EEPROM.end();
    #ifdef DEBUG_BWIFI
        Serial.println("  actual configuration");
        Serial.print(F("  MQTT Server:       ")); Serial.println(wifiConfig.mqtt_server);
        Serial.print(F("  MQTT Port:         ")); Serial.println(wifiConfig.mqtt_port);
        Serial.print(F("  API Token:         ")); Serial.println(wifiConfig.api_token);
        Serial.print(F("  MQTT Username:     ")); Serial.println(wifiConfig.mqtt_username);
        Serial.print(F("  MQTT Password:     ")); Serial.println(wifiConfig.mqtt_password);
        Serial.print(F("  Bluetti Device:    ")); Serial.println(wifiConfig.bluetti_device);
        Serial.print(F("  Bluetti Device ID: ")); Serial.println(wifiConfig.bluetti_device_id);
        Serial.print(F("  OTA Username:      ")); Serial.println(wifiConfig.ota_username);
        Serial.print(F("  OTA Password:      ")); Serial.println(wifiConfig.ota_password);
        Serial.print(F("  Static IP:         ")); Serial.println(wifiConfig.static_ip);
        Serial.print(F("  Static GW:         ")); Serial.println(wifiConfig.static_gw);
        Serial.print(F("  Static SN:         ")); Serial.println(wifiConfig.static_sn);
        Serial.print(F("  AP_Static IP:      ")); Serial.println(wifiConfig.ap_static_ip);
        Serial.print(F("  AP_Static GW:      ")); Serial.println(wifiConfig.ap_static_gw);
        Serial.print(F("  AP_Static SN:      ")); Serial.println(wifiConfig.ap_static_sn);
        Serial.print(F("  WiFi SSID:         ")); Serial.println(wifiConfig.wifi_ssid);
        Serial.print(F("  WiFi Password:     ")); Serial.println(wifiConfig.wifi_password);
        Serial.print(F("  config_saved Flag: ")); Serial.println(wifiConfig.config_saved);
      #endif
  }

void setWiFiPowerSavingMode()
  {
    //esp_wifi_set_ps(WIFI_PS_MAX_MODEM); // maximum power saving, does not make sense here
    //esp_wifi_set_ps(WIFI_PS_NONE); // will cause kernel panic and reboot on my ESP32 (AlexBurghardt)
    esp_wifi_set_ps(WIFI_PS_MIN_MODEM); // default
  }
void initBWifi(bool resetWifi)
  {
    shouldSaveConfig = resetWifi;
    IPAddress _ip, _gw, _sn;
    // implement static IP last item from platformio.ini to wifiConfigDefault
    Serial.println("\nInitializing WiFi...");
    String buf;
    Serial.print("  import last byte of 'static_ip' = "); Serial.println(DEV_IP3);
    buf = wifiConfigDefault.static_ip + String(DEV_IP3);
    Serial.println("  update wifiConfig*.static_ip, <wifi_param>.static_ip"); Serial.println(buf);
    strcpy(wifiConfigDefault.static_ip, buf.c_str());
    strcpy(wifiConfig.static_ip, buf.c_str());
    custom_static_ip.setValue(buf.c_str(), 16);
    Serial.print("  new default 'static_ip': "); Serial.println(custom_static_ip.getValue());

    // presave config, if necessary (corupted EEPROM, first start, resetWifi)
    eeprom_readconfig(resetWifi);

    Serial.println("  Configure WiFi");
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    wifiManager.setDebugOutput(true, WM_DEBUG_DEV);
    wifiManager.setConfigPortalTimeout(600); // 3 minutes to connect to wifi in config portal
    wifiManager.setConfigPortalBlocking(false);
    wifiManager.preloadWiFi(wifiConfig.wifi_ssid, wifiConfig.wifi_password);

    Serial.print("MD setSTAStaticIPConfig ... IP: "); Serial.print(wifiConfig.static_ip);
                                                      Serial.print(wifiConfig.static_gw);
                                                      Serial.println(wifiConfig.static_sn);
    _ip.fromString(wifiConfig.static_ip);
    _gw.fromString(wifiConfig.static_gw);
    _sn.fromString(wifiConfig.static_sn);
    wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

    Serial.print("MD setAPStaticIPConfig ... IP: "); Serial.println(wifiConfig.ap_static_ip);
                                                     Serial.print(wifiConfig.ap_static_gw);
                                                     Serial.println(wifiConfig.static_sn);
    _ip.fromString(wifiConfig.ap_static_ip);
    _gw.fromString(wifiConfig.ap_static_gw);
    _sn.fromString(wifiConfig.ap_static_sn);
    wifiManager.setAPStaticIPConfig(_ip, _gw, _sn);

    Serial.println("MD addParameter to WiFiManager");
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_username);
    wifiManager.addParameter(&custom_mqtt_password);
    wifiManager.addParameter(&custom_api_token);
    wifiManager.addParameter(&custom_bluetti_device);
    wifiManager.addParameter(&custom_bluetti_device_id);
    wifiManager.addParameter(&custom_ota_username);
    wifiManager.addParameter(&custom_ota_password);
    wifiManager.addParameter(&custom_static_ip);
    wifiManager.addParameter(&custom_static_gw);
    wifiManager.addParameter(&custom_static_sn);
    wifiManager.addParameter(&custom_ap_static_ip);
    wifiManager.addParameter(&custom_ap_static_gw);
    wifiManager.addParameter(&custom_ap_static_sn);
    wifiManager.addParameter(&custom_wifi_ssid);
    wifiManager.addParameter(&custom_wifi_password);

    Serial.println("MD setAPCallback   ...");
    wifiManager.setAPCallback([&](WiFiManager* wifiManager)
      {
    		Serial.printf("Entered config mode:ip=%s, ssid='%s'\n",
                      WiFi.softAPIP().toString().c_str(),
                      wifiManager->getConfigPortalSSID().c_str());
                      #ifdef USE_DISPLAY
                          #ifdef DISPLAYSSD1306
                              wrDisp_wifisignal(2); //AP mode
                              wrDisp_IP(WiFi.softAPIP().toString().c_str());
                              wrDisp_Status("Setup Wifi");
                            #endif
                        #endif
    	});
    //sets config for a static IP for login and AP
      /*
      #if (SET_STATIC_IP>0)
          //wifiManager.setAPStaticIPConfig(IPAddress(NET_IP0, NET_IP1, AP_IP2, AP_IP3), IPAddress(NET_IP0, NET_IP1, AP_IP2,AP_GW3), IPAddress(255,255,255,0));
          wifiManager.setSTAStaticIPConfig(IPAddress(NET_IP0, NET_IP1, DEV_IP2, DEV_IP3), IPAddress(NET_IP0, NET_IP1, DEV_GW2, DEV_GW3), IPAddress(255,255,0,0));
        #endif

       */ //if (!wifiManager.autoConnect("Bluetti_ESP32")) {
    Serial.println("MD autoConnect ...");
    if (!wifiManager.autoConnect("ESP32"))
      {
        Serial.println("MD ERR autoConnect ");
        ESP.restart();
      }
    if (shouldSaveConfig)
      {
        eeprom_saveconfig();
      }
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
      {
        // display will have blinking wifi signal until connected.
        #ifdef DISPLAYSSD1306
            disp_setPrevStateIcon(0);
            wrDisp_wifisignal(0);
            delay(200);
            Serial.print(".");
            disp_setPrevStateIcon(1);
            wrDisp_wifisignal(0);
          #else
            delay(500);
            Serial.print(".");
          #endif
      }

    WiFi.setAutoReconnect(true);

    Serial.println(F(""));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("MAC: "));
    Serial.println(WiFi.macAddress());
    #ifdef USE_DISPLAY
        #ifdef DISPLAYSSD1306
            wrDisp_IP(WiFi.localIP().toString().c_str());
            disp_setWifiSignal(1, WiFi.RSSI());
          #endif
      #endif
    if (MDNS.begin(DEVICE_NAME))
      {
        Serial.println(F("MDNS responder started"));
      }
    //setup web server handling
    #if MSG_VIEWER_DETAILS
        msgViewerDetails = true;
        Serial.println(F("webserver BT/MQTT variable logging enabled..."));
    #else
        msgViewerDetails = false;
        Serial.println(F("webserver BT/MQTT variable logging disabled..."));
      #endif
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
      {
        request->send_P(200, "text/html", index_html, processorWebsiteUpdates);
      });
    server.on("/switchLogging", HTTP_GET, [](AsyncWebServerRequest *request)
      {
        msgViewerDetails = !msgViewerDetails;
        if(msgViewerDetails)
          {
            Serial.println(F("webserver BT/MQTT variable logging enabled..."));
          }
          else
          {
            Serial.println(F("webserver BT/MQTT variable logging disabled..."));
          }
        request->send_P(200, "text/html", index_html, processorWebsiteUpdates);
      });
    server.on("/rebootDevice", [](AsyncWebServerRequest *request)
      {
        request->send(200, "text/plain", "reboot in 2sec");
        delay(2000);
        ESP.restart();
      });
    server.on("/resetConfig", [](AsyncWebServerRequest *request)
      {
        request->send(200, "text/plain", "reset Wifi and reboot in 2sec");
        delay(2000);
        initBWifi(true);
      });
    //setup web server events
    events.onConnect([](AsyncEventSourceClient *client)
      {
        if(client->lastId())
          {
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
          }
        client->send("hello my friend, I'm just your data feed!", NULL, millis(), 10000);
      });
    server.addHandler(&events);
    if (!wifiConfig.ota_username)
      {
        ElegantOTA.begin(&server);
      }
      else
      {
        ElegantOTA.begin(&server, wifiConfig.ota_username, wifiConfig.ota_password);
      }
    server.begin();
    Serial.println(F("HTTP server started"));
  }
void handleWebserver()
  {
    //Serial.println(F("DEBUG handleWebserver"));
    if ((millis() - lastTimeWebUpdate) > MSG_VIEWER_REFRESH_CYCLE*1000)
      {
        // check wifi status every MSG_VIEWER_REFRESH_CYCLE and set display
        if (WiFi.status() != WL_CONNECTED)
          {
            Serial.println(F("WiFi is disconnected, try to reconnect..."));
            #ifdef USE_DISPLAY
                #ifdef DISPLAYSSD1306
                    disp_setWifiMode(0);
                    disp_setStatus("Wifi err..");
                  #endif
              #endif
            WiFi.disconnect();
            WiFi.reconnect();
            AddtoMsgView(String(millis()) + ": WLAN ERROR! try to reconnect");
            wifiReconnectCounter++;
            //delay(1000); no delay as we only check every 5 seconds. Removing 1 second blocking of the program in the loop.
          }
          else
          {
            #ifdef USE_DISPLAY
                #ifdef DISPLAYSSD1306
                    disp_setWifiSignal(1,WiFi.RSSI());
                    if (wifiReconnectCounter > 0)
                      {
                        //only update display ones after wifi is recovered.
                        disp_setStatus("Running!");
                        wifiReconnectCounter = 0;
                      }
                  #endif
              #endif
          }
        #ifdef USE_DISPLAY
            #ifdef DISPLAYSSD1306
                // update display
                disp_setBlueTooth(isBTconnected());
                disp_setMqttStatus(isMQTTconnected());
              #endif
          #endif
        // Send Events to the Web Server with current data
        events.send("ping",NULL,millis());
        events.send(String(millis()).c_str(),"runtime",millis());
        events.send(String(WiFi.RSSI()).c_str(),"rssi",millis());
        events.send(String(isMQTTconnected()).c_str(),"mqtt_connected",millis());
        events.send(String(getLastMQTTMessageTime()).c_str(),"mqtt_last_msg_time",millis());
        events.send(String(isBTconnected()).c_str(),"bt_connected",millis());
        events.send(String(getLastBTMessageTime()).c_str(),"bt_last_msg_time",millis());
        if(msgViewerDetails)
          {
            events.send(lastMsg.c_str(),"last_msg",millis());
          }
        lastTimeWebUpdate = millis();
      }
  }
String processorWebsiteUpdates(const String& var)
  {
    if(var == "IP")
      {
        return String(WiFi.localIP().toString());
      }
    else if(var == "RSSI")
      {
        return String(WiFi.RSSI());
      }
    else if(var == "SSID")
      {
        return String(WiFi.SSID());
      }
    else if(var == "MAC")
      {
        return String(WiFi.macAddress());
      }
    else if(var == "RUNTIME")
      {
        return String(millis());
      }
    else if(var == "MQTT_IP")
      {
        char msg[40];
        if (strlen(wifiConfig.mqtt_server) == 0)
          {
            strlcpy(msg, "No MQTT server configured", 40);
          }
          else
          {
            strlcpy(msg, wifiConfig.mqtt_server, 40);
          }
        return msg;
      }
    else if(var == "MQTT_PORT")
      {
        char msg[6];
        strlcpy(msg, wifiConfig.mqtt_port, 6);
        return msg;
      }
    else if(var == "MQTT_CONNECTED")
      {
        return String(isMQTTconnected());
      }
    else if(var == "LAST_MQTT_MSG_TIME")
      {
        return String(getLastMQTTMessageTime());
      }
    else if(var == "DEVICE_ID")
      {
        char msg[40];
        strlcpy(msg, wifiConfig.bluetti_device, 40);
        return msg;
      }
    else if(var == "BT_CONNECTED")
      {
        return String(isBTconnected());
      }
    else if(var == "LAST_BT_MSG_TIME")
      {
        return String(getLastBTMessageTime());
      }
    else if(var == "BT_ERROR")
      {
        return String(getPublishErrorCount());
      }
    else if(var == "LAST_MSG")
      {
        if (msgViewerDetails)
          {
            return String("...waiting for data...");
          }
          else
          {
            return String("...disabled...");
          }
      }
    else //return something, else this if then else will crash in case calles without VAR set....
      {
        return String("");
      }
  }
void AddtoMsgView(String data)
  {
    String tempMsg = "";
    int firstPos = lastMsg.indexOf("</p>");
    int nextPos = firstPos;
    int numEntry = 0;
    while(nextPos > 0)
      {
        nextPos = lastMsg.indexOf("</p>",nextPos+4);
        if (nextPos > 0)
          {
            numEntry++;
          }
      }
    if (numEntry > MSG_VIEWER_ENTRY_COUNT-2)
      {
        tempMsg = lastMsg.substring(firstPos+4);
        lastMsg = tempMsg + "<p>" + data + "</p>";
      }
      else
      {
        lastMsg = lastMsg + "<p>" + data + "</p>";
      }
  }
// - changelog --------------------------------------------------------------------------
/* MD0.1.2 - 2025-10-28 - add scanning of unknown modbus adresses of AC300
 * - add static IP config for device login and access point to BWifi.cpp
 *   move details as define to platformio.ini
/* MD0.0.2 - 2025-01-13 - simuting Bluetti data for MQTT
 * - set default data for connections
 * -
 * ------------------------------------------------------------------------------------- */
/* MD0.0.1 - 2025-01-11 - md - initial version
 * - new define USE_DISPLAY (-> platform.ini)
 *   ndef USE_DISPLAY = no display implemented
 * - change code format to MD format for better readability
 * ------------------------------------------------------------------------------------- */
