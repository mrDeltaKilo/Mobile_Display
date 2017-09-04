/*************************
* Mobile_Display_TEST.h *
*************************/
#include <Arduino.h>

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "BlynkProvisioning.h"

WidgetTerminal terminal(V1);

MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed;    // default frame delay value
textEffect_t scrollEffect;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 4000; // in milliseconds
static int brightness;
static bool power;

// Global message buffers shared by Serial and Scrolling functions
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

void setup()
{
  delay(500);
  Serial.begin(SERIAL_BAUD);

  BlynkProvisioning.begin();

  MD_init();
}

void loop()
{
  BlynkProvisioning.run();

  MD_run();
}

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_RED       "#D3435C"

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V3, V4, V5);
}

BLYNK_APP_CONNECTED()
{
  terminal.print("\n [Mobile Scrolling Display] \n");
  terminal.print("\n Type a message for the scrolling display, ");
  terminal.print("\n or select a predefined message. \n");
  terminal.flush();
}

BLYNK_WRITE(V1)
{
  strcpy(newMessage, param.asStr());
  newMessageAvailable = true;
  SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
  terminal.print("\n Custom Message: ");
  terminal.write(param.getBuffer(), param.getLength());
  terminal.print(" \n");
  terminal.println();

  terminal.flush();
}

BLYNK_WRITE(V2)
{
  switch (param.asInt())
    {
    case 1:
      strcpy(newMessage, "// EVO");
      newMessageAvailable = true;
      SetParams(scrollSpeed * 3, 4000, PA_FADE);
      break;

    case 2:
      strcpy(newMessage, "Back off Asshole.");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    case 3:
      strcpy(newMessage, "Speed Limit is 30mph");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    case 4:
      strcpy(newMessage, "Speed Limit is 35mph");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    case 5:
      strcpy(newMessage, "Speed Limit is 45mph");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    case 6:
      strcpy(newMessage, "Speed Limit is 55mph");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    case 7:
      strcpy(newMessage, "CAUTION");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 4000, PA_OPENING_CURSOR);
      break;

    case 8:
      strcpy(newMessage, "Thank you for your service.");
      newMessageAvailable = true;
      SetParams(scrollSpeed, 0, PA_SCROLL_LEFT);
      break;

    default:
      break;
    }
}

BLYNK_WRITE(V3)
{
  if (param.asInt() != brightness)
    {
      brightness = param.asInt();
      P.setIntensity(brightness);
    }
}

BLYNK_WRITE(V4)
{
  if (param.asInt() != scrollSpeed)
    {
      scrollSpeed = param.asInt();
      P.setSpeed(scrollSpeed);
    }
}

BLYNK_WRITE(V5)
{
  power = param.asInt();
  if (!power)
    {
      P.displayShutdown(true);
      Blynk.setProperty(V5, "color", BLYNK_RED);
    }
  else
    {
      P.displayReset();
      P.displayClear();
      P.displayShutdown(false);
      Blynk.setProperty(V5, "color", BLYNK_GREEN);
    }
}

void readSerial(void)
{
  static char *cp = newMessage;

  while (Serial.available())
    {
      *cp = (char)Serial.read();
      if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE - 2))         // end of message character or full buffer
        {
          *cp = '\0';               // end the string
          // restart the index for next filling spree and flag we have a message waiting
          cp = newMessage;
          newMessageAvailable = true;
        }
      else           // move char pointer to next position
        cp++;
    }
}

void SetParams(uint8_t speed, uint16_t pause, textEffect_t effect)
{
  scrollSpeed = speed;
  scrollPause = pause;
  scrollEffect = effect;
}

void MD_init()
{
  Serial.print("\n[Mobile Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline\n");
  P.begin();
  P.displayClear();
  P.displaySuspend(false);

  SetParams(scrollSpeed * 3, 4000, PA_FADE);

  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

  strcpy(curMessage, "// EVO");
  newMessage[0] = '\0';
}

void MD_run()
{
  readSerial();
  if (P.displayAnimate())
    {
      if (newMessageAvailable)
        {
          strcpy(curMessage, newMessage);
          newMessageAvailable = false;
          P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
        }
      P.displayReset();
    }
}
