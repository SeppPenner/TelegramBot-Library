// Copyright SeppPenner 2020
// MIT License
//
// TelegramBot library
// Based on https://github.com/CasaJasmina/TelegramBot-Library
// https://github.com/SeppPenner/TelegramBot-Library

#include "TelegramBot.h"

/**
    The constructor of the Telegram bot class.
    @param token The Telegram token.
    @param client The used Wifi client.
    @return A new instance of the Telegram bot.
*/
TelegramBot::TelegramBot(const char* token, WifiClientSecure &client) {
    this -> client = &client;
    this -> token = token;
}

/**
    Starts the client and connects it to Telegram.
    @return Nothing.
*/
void TelegramBot::begin() {
    if (!client -> connected()) {
        client -> setInsecure();

        if (!client -> connect(HOST, SSLPORT)) {
            Serial.println("Connection failed.");
        } else {
            Serial.println("Connected.");
        }
    }
}

/**
    Gets updates from the Telegram messages in the given channel.
    @return A new instance of the deserialized Telegram message.
*/
Message TelegramBot::getUpdates() {
    begin();

    client -> println("GET /bot" + String(token) + "/getUpdates?limit=1&offset=" + String(lastMessageReceived) + " HTTP/1.1");
    setDefaultHeaders();
    client -> println();

    String payload = readPayload();
    if (payload != "") {
        Message m;
        StaticJsonDocument < JSONBUFFERSIZE > jsonBuffer;
        DeserializationError err = deserializeJson(jsonBuffer, payload);

        if (err == DeserializationError::Ok) {
            int updateId = jsonBuffer["result"][0]["update_id"];
            updateId = updateId + 1;

            if (lastMessageReceived != updateId) {
                String sender = jsonBuffer["result"][0]["message"]["from"]["username"];
                String text = jsonBuffer["result"][0]["message"]["text"];
                String chatId = jsonBuffer["result"][0]["message"]["chat"]["id"];
                String date = jsonBuffer["result"][0]["message"]["date"];

                m.sender = sender;
                m.text = text;
                m.chatId = chatId;
                m.date = date;
                lastMessageReceived = updateId;
                return m;
            } else {
                m.chatId = "";
                return m;
            }
        } else {
            Serial.println("");
            Serial.println("Message too long, skipped.");
            Serial.println("");
            int updateIdFirstDigit = 0;
            int updateIdLastDigit = 0;

            for (int a = 0; a < 3; a++) {
                updateIdFirstDigit = payload.indexOf(':', updateIdFirstDigit + 1);
            }

            for (int a = 0; a < 2; a++) {
                updateIdLastDigit = payload.indexOf(',', updateIdLastDigit + 1);
            }

            lastMessageReceived = payload.substring(updateIdFirstDigit + 1, updateIdLastDigit).toInt() + 1;
        }
    }
}

/**
    Sends a simple message to a chat.
    @param chatId The chat identifier.
    @param text The text to send.
    @return The posted message as String.
*/
String TelegramBot::sendMessage(String chatId, String text) {
    if (chatId != "0" && chatId != "") {
        StaticJsonDocument < JSONBUFFERSIZE > jsonBuffer;
        jsonBuffer["chat_id"] = chatId;
        jsonBuffer["text"] = text;
        String message;
        serializeJson(jsonBuffer, message);
        return postMessage(message);
    } else {
        Serial.println("Chat identifier not defined.");
    }
}

/**
    Sends a simple message to a chat with a reply markup.
    @param chatId The chat identifier.
    @param text The text to send.
    @param keyboardMarkup The keyboard markup.
    @param oneTimeKeyboard A bool indicating whether a one time keyboard is used.
    @param resizeKeyboard A bool indicating whether a resize keyboard is used.
    @return The posted message as String.
*/
String TelegramBot::sendMessage(String chatId, String text, TelegramKeyboard & keyboardMarkup, bool oneTimeKeyboard, bool resizeKeyboard) {
    if (chatId != "0" && chatId != "") {
        StaticJsonDocument < JSONBUFFERSIZE > jsonBuffer;
        jsonBuffer["chat_id"] = chatId;
        jsonBuffer["text"] = text;

        JsonObject replyMarkup = jsonBuffer.createNestedObject("reply_markup");
        JsonArray keyboard = replyMarkup.createNestedArray("keyboard");

        for (int a = 1; a <= keyboardMarkup.length(); a++) {
            JsonArray row = keyboard.createNestedArray();
            for (int b = 1; b <= keyboardMarkup.rowSize(a); b++) {
                row.add(keyboardMarkup.getButton(a, b));
            }
        }

        replyMarkup["one_time_keyboard"].set(oneTimeKeyboard);
        replyMarkup["resize_keyboard"].set(resizeKeyboard);
        replyMarkup["selective"].set(false);

        String message;
        serializeJson(jsonBuffer, message);
        return postMessage(message);
    } else {
        Serial.println("Chat identifier not defined.");
    }
}

/**
    Posts a simple message to a chat.
    @param message The message to send.
    @return The payload as String.
*/
String TelegramBot::postMessage(String message) {
    begin();
    client -> println("POST /bot" + String(token) + "/sendMessage HTTP/1.1");
    setDefaultHeaders();
    client -> println(message.length());
    client -> println();
    client -> println(message);
    return readPayload();
}

/**
    Posts a simple sticker to a chat.
    @param sticker The sticker to send.
    @return The payload as String.
*/
String TelegramBot::postSticker(String sticker) {
    begin();
    client -> println("POST /bot" + String(token) + "/sendSticker HTTP/1.1");
    setDefaultHeaders();
    client -> println(sticker.length());
    client -> println();
    client -> println(sticker);
    return readPayload();
}

/**
    Reads the payload from the Telegram server.
    @return The payload as String.
*/
String TelegramBot::readPayload() {
    char c;
    String payload = "";
    //Read the answer and save it in String payload
    while (client -> connected()) {
        payload = client -> readStringUntil('\n');
        if (payload == "\r") {
            break;
        }
    }
    payload = client -> readStringUntil('\r');
    return payload;
}

/**
    Sets the default headers.
    @return Nothing.
*/
void TelegramBot::setDefaultHeaders() {
    client -> println("Host: " + String(HOST));
    client -> println("Content-Type: application/json");
    client -> println("User-Agent: Arduino");
    client -> println("Connection: close");
    client -> print("Content-Length: ");
    client -> println("Accept: */*");
}