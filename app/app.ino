// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Please use an Arduino IDE 1.6.8 or greater

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>

#include "config.h"

static bool messagePending = false;
static bool messageSending = true;

static char *connectionString = "HostName=uci-244-group6.azure-devices.net;DeviceId=MyNodeDevice;SharedAccessKey=9/9huZbKBFyC+eML/jHo/214zvdCFryOaFctoEE02IY=";
static char *ssid = "VenetoMCS";
static char *pass = "947Veneto";

static int interval = INTERVAL;

bool recording = false;
String noteString = "";
int prevButton = 0;
unsigned long startTime;

void blinkLED()
{
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
}

void initWifi()
{
    // Attempt to connect to Wifi network:
    Serial.printf("Attempting to connect to SSID: %s.\r\n", ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        // Get Mac Address and show it.
        // WiFi.macAddress(mac) save the mac address into a six length array, but the endian may be different. The huzzah board should
        // start from mac[0] to mac[5], but some other kinds of board run in the oppsite direction.
        uint8_t mac[6];
        WiFi.macAddress(mac);
        Serial.printf("You device with MAC address %02x:%02x:%02x:%02x:%02x:%02x connects to %s failed! Waiting 10 seconds to retry.\r\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ssid);
        WiFi.begin(ssid, pass);
        delay(10000);
    }
    Serial.printf("Connected to wifi %s.\r\n", ssid);
}

void initTime()
{
    time_t epochTime;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true)
    {
        epochTime = time(NULL);

        if (epochTime == 0)
        {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        }
        else
        {
            Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
            break;
        }
    }
}

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    initSerial();
    delay(2000);
    readCredentials();

    initWifi();
    initTime();

    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        Serial.println("Failed on IoTHubClient_CreateFromConnectionString.");
        while (1);
    }

    IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
    IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
    IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);
}

static int messageCount = 1;
void loop()
{
    if (digitalRead(BUTTON_PIN) == 1) {  // update recording state
        recording = !recording;
        if (recording) {
            startTime = millis();
            Serial.println("Start recording");
        } else {
            Serial.println("End recording");
        }
        delay(200);
  }
    /**
     * recording state
     */
    if (recording) {
        Serial.println(analogRead(A0));
        int button = getKey();
        playTone(button);
        if (button != prevButton) {
            prevButton = button;
            int noteTime = (millis() - startTime) / 10;
            startTime = millis();
            noteString = noteString + button + "," + noteTime + "#";
        }
    /**
     * uploading state
     */
    } else {
        if (!messagePending && messageSending) {
            char messagePayload[MESSAGE_MAX_LEN];
            String notes = "1,15#2,20#4,15#2,10#";
            int strLen = notes.length() + 1;
            char charBuff[strLen];
            notes.toCharArray(charBuff, strLen);
            bool temperatureAlert = readMessage(messageCount, messagePayload, charBuff);
            sendMessage(iotHubClientHandle, messagePayload, temperatureAlert);
            messageCount++;
            delay(interval);   // wait for upload finished
            Serial.println(noteString);
            delay(10);
            noteString = "";
        }
        prevButton = 0;
        IoTHubClient_LL_DoWork(iotHubClientHandle);
        delay(10);
    
  }
}
