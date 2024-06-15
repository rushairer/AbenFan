#ifndef __MENU_H_
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <functional>

#define VERSION "1.0.0"

void MenuDinoRunFunc();
void MenuDinoUltramanRunFunc();
void MenuTurnOffFunc();
void MenuIsClosed();

void MenuSendIsRGBLightOn(uint8_t);
void MenuSetRGBLightOn(uint8_t);

void MenuSendTemperatureOffset(uint8_t offset);
void MenuSetTemperatureOffset(uint8_t offset);

void MenuSendHumidityOffset(uint8_t offset);
void MenuSetHumidityOffset(uint8_t offset);

class Menu
{
public:
    Menu(U8G2 *u8g2);
    virtual ~Menu();

    void setup();
    void loop(std::function<void()> whenFormInactiveFunc);

    void start();
    void gotoForm(uint8_t formId, uint8_t pos);
    void backtoForm();

protected:
    U8G2 *_u8g2;
    MUIU8G2 _mui;
    bool _isReDraw;
};

#endif // !__MENU_H_
