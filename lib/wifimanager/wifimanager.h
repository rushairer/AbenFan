#ifndef __WIFIMANAGER_H_
#define __WIFIMANAGER_H_

#include <WiFi.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <WebServer.h>

class WifiManager;

class WifiManager
{
public:
    WifiManager(
        char *hostName,
        WiFiClass *wifi,
        MDNSResponder *mdns,
        HWCDC *serial,
        int serverPort = 8081);
    virtual ~WifiManager();

    void setup();
    void loop();

protected:
    Preferences _preferences;
    WiFiClass *_wifi;
    MDNSResponder *_mdns;
    HWCDC *_serial;

    WebServer _webServer;

    void autoConnect();
    int tryConnect();
    void createAP();

private:
    void handleIndex();
    void handleSavePassword();
    void handleClearPassword();

    char *_hostName;
};

#endif