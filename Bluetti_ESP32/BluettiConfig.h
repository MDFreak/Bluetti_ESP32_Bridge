/* -- BTooth.h -- MD0.0.1----------------------------------------------------------------------*/
#ifndef BLUETTI_CONFIG_H
  #define BLUETTI_CONFIG_H

  //#if POWER_STATION(AP300)
  #if (AP300_ACTIVE > 0)
      #include "DeviceType_AP300.h"
      #include "Device_AP300.h"
    #endif
  #if (HANDSFREE_ACTIVE > 0)
      #include "DeviceType_Handsfree2.h"
      #include "Device_Handsfree2.h"
    #endif
  #if (AC300_ACTIVE > 0)
      #include "DeviceType_AC300.h"
      #include "Device_AC300.h"
    #endif

  //#include "PowerStation.h"
  #include "config.h"

#endif
/* MD0.0.1 - 2025-01-11 - md - initial version
 * - new define USE_DISPLAY (-> platform.ini)
 *   ndef USE_DISPLAY = no display implemented
 * - change code format to MD format for better readability
 *///------------------------------------------------------------------------------------
