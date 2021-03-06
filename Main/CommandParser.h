#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#include <Arduino.h>
#include <WString.h>
#include "Globals.h"
#include "TinyVector.h"


/*
 * Структура команд:
 * CT - префикс
 * SET или GET = тип команды
 * = после знака равно начинается текст команды
 * 
 * Каждая команда передается выбранному модулю.
 * Каждый аргумент в команде разделен через символ '|'.
 * Если символа '|' нет в Command.Arg - считается, что всё, переданное в Command.Arg  - это ID модуля.
 * Первый аргумент в Command.Arg  - это ID модуля (регистрозависимо!).
 * Следующие n аргументов - специфичны для каждого модуля.
 * 
 * Пример команды:
 * CTSET=PIN|13|T - инвертировать текущее состояние модуля PIN для пина номер 13
 * CTGET=PIN|13 - получить текущее состояние модуля PIN для пина номер 13
 */


typedef enum 
{ 
ctUNKNOWN, 
ctGET, 
ctSET,
} COMMAND_TYPE; // тип команды

typedef Vector<char*> CommandArgsVec;

class Command
{
  private:


    Stream* IncomingStream; // поток, из которого пришла команда
    CommandArgsVec arguments; // аргументы команды
    
    bool bIsInternal; // флаг того, что команда получена от другого зарегистрированного модуля
    uint8_t Type; // тип команды
    String ModuleID; // ID модуля

    void Clear();

 public:


    // устанавливает/возвращает поток для работы с командой
    void SetIncomingStream(Stream* s) {IncomingStream = s;}
    Stream* GetIncomingStream() const {return IncomingStream;}

    // флаг, что команда внутренняя, т.е. от одного модуля другому
    bool IsInternal() const {return bIsInternal;}
    void SetInternal(bool i) {bIsInternal = i;}
    
    void Construct(const char* moduleID,const char* rawArgs, uint8_t ct); // конструирует команду из переданных аргументов
    void Construct(const char* moduleID,const char* rawArgs, const char* ct); // конструирует команду из переданных аргументов


    // возвращает тип команды
    uint8_t GetType() const {return Type;}

    // возвращает ID программного модуля, которому адресована команда
    String GetTargetModuleID() const {return ModuleID;}

    // возвращает количество переданных аргументов
    size_t GetArgsCount() const;

    // возвращает аргумент по индексу
    const char* GetArg(size_t idx) const;
    
    Command();
    ~Command();
};


// парсер команд
class CommandParser
{
  private:
    
  public:
    CommandParser();

    void Clear();
    bool ParseCommand(const String& command, Command& outCommand);
};


#endif
