#ifndef __OTA_UPDATE_H
#define __OTA_UPDATE_H

#define NO_GLOBAL_ARDUINOOTA
#include <ArduinoOTA.h>

class OTAUpdateClass: public ArduinoOTAClass
{

public:
    OTAUpdateClass(/* args */);
    ~OTAUpdateClass();

    //Starts the OTAUpdate service
    void begin();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_OTAUPDATE)
extern OTAUpdateClass OTAUpdate;
#endif

#endif /* __OTA_UPDATE_H */