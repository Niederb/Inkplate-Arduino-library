// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6 plus in the boards menu."
#endif

#include "Inkplate.h"

const char *ssid = "";     // Your WiFi SSID
const char *password = ""; // Your WiFi password

Inkplate display(INKPLATE_3BIT);

void setup()
{
    Serial.begin(115200);
    display.begin();

    // Turn off backlight
    display.backlight(true);
    display.setBacklight(0);

    // Join wifi
    display.joinAP(ssid, password);

    char url[256];
    imageUrl(url);

    Serial.println(display.drawImage(url, display.PNG, 0, 0));
    display.display();

    Serial.println("Going to sleep");
    delay(100);
    esp_sleep_enable_timer_wakeup(15ll * 60 * 1000 * 1000);
    esp_deep_sleep_start();
}

void loop()
{
    // Never here, as deepsleep restarts esp32
}

void imageUrl(char *a)
{
    String url;
    HTTPClient http;
    if (http.begin("https://source.unsplash.com/random/1024x768") && http.GET() > 0)
    {
        url = http.getString();

        int urlStart = url.indexOf("href=\"") + 6;
        int urlEnd = url.indexOf("\">", urlStart);

        url = url.substring(urlStart, urlEnd);
        url = url.substring(0, url.indexOf("?")) + "?crop=entropy&fit=crop&fm=png&h=768&w=1024";

        Serial.println(url);
        strcpy(a, url.c_str());
    }
    else
    {
        display.println("HTTP error");
        display.display();
    }
    http.end();
}