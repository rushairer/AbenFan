#ifndef __THERMOMETER_H_

#include <AHT20.h>

typedef struct
{
    float temperature;
    float humidity;
} ThermometerValue;

class Thermometer
{
public:
    Thermometer(AHT20 *aht20, float temperatureOffset, float humidityOffset);
    Thermometer(AHT20 *aht20, ThermometerValue *value, float temperatureOffset, float humidityOffset);
    virtual ~Thermometer();

    void setup();
    void loop();

    float getTemperature();
    float getHumidity();

protected:
    AHT20 *_aht20;
    ThermometerValue *_value;
    float _temperatureOffset;
    float _humidityOffset;
    static bool refreshAHT20(void *);
};

#endif // !__THERMOMETER_H_