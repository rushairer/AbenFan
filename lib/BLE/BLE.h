#ifndef __BLE_H_
#define __BLE_H_

#include <functional>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLE : public BLEServerCallbacks, BLECharacteristicCallbacks
{
public:
    BLE(
        const char *name,
        const char *serviceUUID,
        const char *txCharacteristicUUID,
        const char *rxCharacteristicUUID);
    virtual ~BLE();

    void setup();
    void loop();

    void onConnect(BLEServer *server);
    void onDisconnect(BLEServer *server);

    void onWrite(BLECharacteristic *characteristic, esp_ble_gatts_cb_param_t *param);
    void onRead(BLECharacteristic *characteristic, esp_ble_gatts_cb_param_t *param);

protected:
    const char *_name;
    const char *_serviceUUID;
    const char *_txCharacteristicUUID;
    const char *_rxCharacteristicUUID;
    bool _connecting;
    bool _connected;
    BLEServer *_server;
    BLEService *_service;
    BLECharacteristic *_txCharacteristic;
    BLECharacteristic *_rxCharacteristic;
};

#endif // !__BLE_H_