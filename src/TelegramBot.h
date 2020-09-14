// Copyright SeppPenner 2020
// MIT License
//
// TelegramBot library
// Based on https://github.com/CasaJasmina/TelegramBot-Library
// https://github.com/SeppPenner/TelegramBot-Library

#ifndef TelegramBot_h
#define TelegramBot_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WifiClientSecure.h>
#include <TelegramKeyboard.h>

#define HOST "api.telegram.org"
#define SSLPORT 443

#ifndef JSONBUFFERSIZE
#ifdef ESP8266
#define JSONBUFFERSIZE 1000
#else
#define JSONBUFFERSIZE 10000
#endif
#endif

struct Message {
    String text;
    String chatId;
    String sender;
    String date;
};

class TelegramBot {
    public:
        TelegramBot(const char* token, WifiClientSecure &client);
        void begin();
        Message getUpdates();
        String sendMessage(String chatId, String text);
        String sendMessage(String chatId, String text, TelegramKeyboard &keyboardMarkup, bool oneTimeKeyboard = true, bool resizeKeyboard = true);
        String sendSticker(String chatId, String sticker);

    private:
        const char* token;
        int lastMessageReceived;
        WifiClientSecure* client;
        String postMessage(String message);
        String postSticker(String sticker);
        String readPayload();
        void setDefaultHeaders();
};

#endif