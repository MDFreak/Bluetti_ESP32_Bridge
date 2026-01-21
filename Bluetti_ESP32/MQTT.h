#ifndef MQTT_H
#define MQTT_H
#include "Arduino.h"
//#include "DeviceType.h"
#if (AP300_ACTIVE > 0)
    #include "DeviceType_AP300.h"
  #endif
#if (HANDSFREE_ACTIVE > 0)
    #include "DeviceType_Handsfree2.h"
  #endif
#if (AC300_ACTIVE > 0)
    #include "DeviceType_AC300.h"
  #endif

extern void publishTopic(enum field_names field_name, String value);
extern void publishHAConfig();
extern void publishDeviceState();
extern void publishDeviceStateStatus();
extern void deviceServoPress(int degree);
extern void handleMQTT();
extern void initMQTT();
extern bool isMQTTconnected();
extern int getPublishErrorCount();
unsigned long getLastMQTTMessageTime();
unsigned long getLastMQTTDeviceStateMessageTime();
unsigned long getLastMQTTDeviceStateStatusMessageTime();

#endif
/* - changelog --------------------------------------------------------------------------
 * MD0.0.1 - 2025-01-11 - md - initial version
 *
 * - change code format to MD format for better readability
 * ------------------------------------------------------------------------------------- */
