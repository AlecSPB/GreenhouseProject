#ifndef _TEMP_SENSORS_H
#define _TEMP_SENSORS_H

#include "AbstractModule.h"

typedef enum
{
  wmAutomatic, // автоматический режим управления окнами
  wmManual // мануальный режим управления окнами
  
} WindowWorkMode;

typedef enum
{
  dirNOTHING,
  dirOPEN,
  dirCLOSE
  
} DIRECTION;

class WindowState
{
 private:
 
  unsigned int CurrentPosition; // текущая позиция фрамуги
  unsigned int RequestedPosition; // какую позицию запросили
  bool OnMyWay; // флаг того, что фрамуга в процессе открытия/закрытия
  unsigned long TimerInterval; // сколько работать фрамуге?
  unsigned long TimerTicks; // сколько проработали уже?
  DIRECTION Direction;

  void SwitchRelays(uint16_t rel1State = SHORT_CIRQUIT_STATE, uint16_t rel2State = SHORT_CIRQUIT_STATE);

  uint8_t RelayChannel1;
  uint8_t RelayChannel2;
  ModuleState* RelayStateHolder;

public:

  bool IsBusy() {return OnMyWay;} // заняты или нет?
  
  bool ChangePosition(DIRECTION dir, unsigned int newPos); // меняет позицию
  
  unsigned int GetCurrentPosition() {return CurrentPosition;}
  unsigned int GetRequestedPosition() {return RequestedPosition;}
  DIRECTION GetDirection() {return Direction;}

  void UpdateState(uint16_t dt); // обновляет состояние фрамуги
  
  void Setup(ModuleState* state, uint8_t relay1, uint8_t relay2); // настраиваем перед пуском


  WindowState() 
  {
    CurrentPosition = 0;
    RequestedPosition = 0;
    OnMyWay = false;
    TimerInterval = 0;
    TimerTicks = 0;
    RelayChannel1 = 0;
    RelayChannel2 = 0;
    RelayStateHolder = NULL;
  }  
  
  
};

class TempSensors : public AbstractModule // модуль опроса температурных датчиков и управления фрамугами
{
  private:
  
    uint16_t lastUpdateCall;

    WindowState Windows[SUPPORTED_WINDOWS];
    void SetupWindows();


    WindowWorkMode workMode; // текущий режим работы (автоматический или ручной)

    void BlinkWorkMode(uint16_t blinkInterval = 0);

    uint16_t lastBlinkInterval;
    
  public:
    TempSensors() : AbstractModule(F("STATE")){}

    bool ExecCommand(const Command& command);
    void Setup();
    void Update(uint16_t dt);

    WindowWorkMode GetWorkMode() {return workMode;}
    void SetWorkMode(WindowWorkMode m) {workMode = m;}

};

#endif
