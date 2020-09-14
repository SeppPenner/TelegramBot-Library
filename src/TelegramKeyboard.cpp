// Copyright SeppPenner 2020
// MIT License
//
// TelegramBot library
// Based on https://github.com/CasaJasmina/TelegramBot-Library
// https://github.com/SeppPenner/TelegramBot-Library

#include "TelegramKeyboard.h"

/**
    The constructor of the Telegram keyboard class.
    @return A new instance of the Telegram keyboard.
*/
TelegramKeyboard::TelegramKeyboard() {}

/**
    Adds new rows.
    @param rows The rows.
    @param length The row length.
    @return Nothing.
*/
void TelegramKeyboard::addRows(const char* rows[], int length) {
    Button* first;
    first -> next = NULL;

    lengthOfRows[numberOfRows] = length;

    for (int i = length - 1; i >= 0; i--) {
        Button* temp;
        temp = new Button;
        temp -> text = rows[i];
        temp -> next = first;
        first = temp;
    }

    rows[numberOfRows] = first;
    numberOfRows++;
}

/**
    Gets the button from the keyboard.
    @param rowNumber The row number.
    @param buttonNumber The button number.
    @return The button as String.
*/
String TelegramKeyboard::getButton(int rowNumber, int buttonNumber) {
    if (rowNumber < 1) {
        return "Row number is less than 1.";
    }

    if (buttonNumber < 1) {
        return "Button number is less than 1.";
    }

    Button* first;
    first = rows[rowNumber - 1];

    for (int i = 0; i < buttonNumber - 1; i++) {
        first = first -> next;
    }

    if (first == NULL) {
        return "Overflow.";
    }

    return first -> text;
}

/**
    Gets the number of the rows.
    @return The number of rows as int.
*/
int TelegramKeyboard::length() {
    return numberOfRows;
}

/**
    Gets the row size.
    @return The row size as int.
*/
int TelegramKeyboard::rowSize(int nrow) {
    return lengthOfRows[nrow - 1];
}