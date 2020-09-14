// Copyright SeppPenner 2020
// MIT License
//
// TelegramBot library
// Based on https://github.com/CasaJasmina/TelegramBot-Library
// https://github.com/SeppPenner/TelegramBot-Library

#ifndef TelegramKeyboard_h
#define TelegramKeyboard_h
#define ROWSLIMIT 10

#include <Arduino.h>

struct Button {
    const char* text;
    Button* next;
};

class TelegramKeyboard {
    public:
        TelegramKeyboard();
        void addRows(const char* rows[], int length);
        String getButton(int nrow, int button);
        int length();
        int rowSize(int nrow);

    private:
        int numberOfRows = 0;
        Button* rows[ROWSLIMIT];
        int lengthOfRows[ROWSLIMIT];

};
#endif