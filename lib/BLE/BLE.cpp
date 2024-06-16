#include "BLE.h"

BLE::BLE(
    const char *name,
    const char *serviceUUID,
    const char *txCharacteristicUUID,
    const char *rxCharacteristicUUID)
    : _name(name),
      _serviceUUID(serviceUUID),
      _txCharacteristicUUID(txCharacteristicUUID),
      _rxCharacteristicUUID(rxCharacteristicUUID),
      _connecting(false),
      _connected(false)
{
}

BLE::~BLE()
{
}

void BLE::setup()
{
    Serial.println("Starting BLE work!");
    BLEDevice::init(_name);

    _server = BLEDevice::createServer();
    _server->setCallbacks(this);

    _service = _server->createService(_serviceUUID);

    _txCharacteristic = _service->createCharacteristic(
        _txCharacteristicUUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    _txCharacteristic->addDescriptor(new BLE2902());

    _rxCharacteristic = _service->createCharacteristic(
        _rxCharacteristicUUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    _rxCharacteristic->setCallbacks(this);
    _rxCharacteristic->addDescriptor(new BLE2902());

    _service->start();
    _server->getAdvertising()->setScanResponse(true);
    _server->getAdvertising()->setMinPreferred(0x06);
    _server->getAdvertising()->setMinPreferred(0x12);
    _server->getAdvertising()->start();
}

void BLE::loop()
{
    if (_connecting)
    {
        _txCharacteristic->setValue("Hi");
        _txCharacteristic->notify();
        delay(100);
    }
    if (!_connecting && _connected)
    {
        delay(500);
        _server->startAdvertising();
        Serial.println("Start Advertising");
        _connected = _connecting;
    }

    if (_connecting && !_connected)
    {
        _connected = _connecting;
    }
}

void BLE::onConnect(BLEServer *server)
{
    _connecting = true;
}

void BLE::onDisconnect(BLEServer *server)
{
    _connecting = false;
}

void BLE::onWrite(BLECharacteristic *characteristic, esp_ble_gatts_cb_param_t *param)
{
    std::string value = characteristic->getValue();

    if (value.length() > 0)
    {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
        {
            Serial.print(value[i]);
        }

        Serial.println();
        Serial.println("*********");

        characteristic->setValue("received");
        characteristic->notify();
    }
}

void BLE::onRead(BLECharacteristic *characteristic, esp_ble_gatts_cb_param_t *param)
{
    // characteristic->setValue("received");
    // characteristic->notify();
}