#ifndef __RGBLed
#include <Adafruit_NeoPixel.h>
#include <functional>

class RGBLed
{
public:
    RGBLed(Adafruit_NeoPixel *pixels);
    virtual ~RGBLed();

    void setup();

protected:
    Adafruit_NeoPixel *_pixels;
    uint32_t wheel(byte WheelPos);
    static void showInTask(void *);
};

#endif // !__RGBLed