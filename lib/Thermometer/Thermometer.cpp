#include "Thermometer.h"
#include <stdio.h>
#include <arduino-timer.h>

auto timer = timer_create_default();

Thermometer::Thermometer(AHT20 *aht20,
                         ThermometerValue *value,
                         float temperatureOffset = 0.0,
                         float humidityOffset = 0.0)
    : _aht20(aht20),
      _value(value),
      _temperatureOffset(temperatureOffset),
      _humidityOffset(humidityOffset)
{
    if (aht20 == nullptr)
    {
        printf("error: aht20 is null \n");
    }
    if (value == nullptr)
    {
        printf("error: thermometer value is null \n");
    }
}

Thermometer::Thermometer(AHT20 *aht20,
                         float temperatureOffset = 0.0,
                         float humidityOffset = 0.0)
    : _aht20(aht20),
      _temperatureOffset(temperatureOffset),
      _humidityOffset(humidityOffset)
{
    if (aht20 == nullptr)
    {
        printf("error: aht20 is null \n");
    }

    _value = new ThermometerValue;
}

Thermometer::~Thermometer()
{
    _aht20 = nullptr;
    _value = nullptr;
    _temperatureOffset = 0;
    _humidityOffset = 0;
}

void Thermometer::setup()
{

    if (_aht20->begin() == false)
    {
        Serial.println("AHT20 not detected. Please check wiring. Freezing.");
    }
    else
    {
        _value->temperature = _aht20->getTemperature() + _temperatureOffset;
        _value->humidity = _aht20->getHumidity() + _humidityOffset;
    }

    timer.every(60000, &Thermometer::refreshAHT20, this);
}

void Thermometer::loop()
{
    timer.tick();
}

float Thermometer::getTemperature()
{
    return _value->temperature;
}

float Thermometer::getHumidity()
{
    return _value->humidity;
}

bool Thermometer::refreshAHT20(void *p)
{
    Thermometer *ptr = (Thermometer *)p;
    ptr->_value->temperature = ptr->_aht20->getTemperature() + ptr->_temperatureOffset;
    ptr->_value->humidity = ptr->_aht20->getHumidity() + ptr->_humidityOffset;
    return true;
}