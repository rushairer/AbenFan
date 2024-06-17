#ifndef __ACTIONS_H_
#define __ACTIONS_H_

#include <functional>

void ActionStartDinoRun();
void ActionStartDinoUltramanRun();
void ActionTurnOff();
void ActionToggleRGBLight(uint8_t on);
void ActionChangeTemperatureOffset(uint8_t offset);
void ActionChangeHumidityOffset(uint8_t offset);

#endif // !__ACTIONS_H_