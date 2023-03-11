// ------------------- Options ------------------- //
// Your WiFi Access Point Name
#define MY_SSID "Replace Me"
// Your WiFi Access Point Passphrase
#define MY_PASS "ReplaceM3"
// Your Hacker News Username
#define MY_USER "replaceme"
// Your choice for showing the newest "new" Item or the toppest "top" item
#define MY_FEED "new"
// -- Please leave quotes around option values --- //

// Libraries for using the ESP32's WiFi features to connect to the HN API
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
// A popular JSON parsing libary optimized for microcontrollers
#include <ArduinoJson.h>
// Libraries for communicating with the TTGO display
#include <SPI.h>
#include <TFT_eSPI.h>
// Library for generating and displaying a QR code
#include "qrcodedisplay.h"
#include "qrencode.h"
// Library for wrapping on words on the display
#include <wordwrap.h>

// HEIGHT and WIDTH defined in TFT_eSPI/User_Setups/Setup25_TTGO_T_Display.h
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);

WiFiClientSecure *client = new WiFiClientSecure;

// Subclass QRcodeDisplay for our specific display and dimensions
class QRcodeTTGOdisplay : public QRcodeDisplay {
   private:
    TFT_eSPI *display;
    void drawPixel(int x, int y, int color);

   public:
    QRcodeTTGOdisplay(TFT_eSPI *display);
    void create(String message);
    void init();
    void screenwhite();
    void screenupdate();
    int margin;
} qrcode(&tft);

QRcodeTTGOdisplay::QRcodeTTGOdisplay(TFT_eSPI *display) { 
    this->display = display; 
}

void QRcodeTTGOdisplay::init() {
    // Screen H = 135, W = 240
    // QR Version 7 WD = 45
    // Multiply * 2 = 90
    this->screenwidth = 90;
    this->screenheight = 90;
    this->multiply = screenwidth / WD;
    // Add a margin around QR Code to help scanning
    this->margin = 2;
    // Align with bottom right of screen (rotation set to 1, flip h and w)
    this->offsetsX = TFT_HEIGHT - this->screenwidth - this->margin;
    this->offsetsY = TFT_WIDTH - this->screenheight - this->margin;
}

void QRcodeTTGOdisplay::create(String message) {
    display->startWrite();
    QRcodeDisplay::create(message);
    display->endWrite();
}

void QRcodeTTGOdisplay::screenwhite() {
    // Fills the background and margin area around the QR code with white
    display->fillRect(this->offsetsX - this->margin, 
                      this->offsetsY - this->margin,
                      this->screenwidth + (this->margin * 2),
                      this->screenheight + (this->margin * 2), 
                      TFT_WHITE);
}

void QRcodeTTGOdisplay::screenupdate() {}

void QRcodeTTGOdisplay::drawPixel(int x, int y, int color) {
    // translate color int to specific color macros used by our display
    color = color ? TFT_BLACK : TFT_WHITE;
    if (this->multiply > 1) {
        display->fillRect(x, y, multiply, multiply, color);
    } else {
        display->drawPixel(x, y, color);
    }
}

// Root certificate for https://hacker-news.firebaseio.com/
const char *rootCACertificate =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
    "-----END CERTIFICATE-----";

// Helper function to dump HN API Response
String callAPI(String url) {
    HTTPClient https;
    if (!https.begin(*client, url)) {
        Serial.println("[HTTPS] Unable to connect!");
        return "";
    }
    int httpCode = https.GET();
    String returnString;
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        returnString = https.getString();
    } else {
        String error_string = https.errorToString(httpCode).c_str();
        Serial.println("[HTTPS] GET... failed, error: " + error_string);
    }
    https.end();
    return returnString;
}

// The setup() function runs once on startup
void setup() {
    Serial.begin(9600);
    delay(2000);  // Wait for UART port to connect
    Serial.println();

    Serial.println("Starting App Code");
    WiFi.mode(WIFI_STA);
    WiFi.begin(MY_SSID, MY_PASS);

    Serial.print("Starting WiFi Connection");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("Initializing Screen and setting Drawing options");
    tft.init();
    analogWrite(TFT_BL, 64);  // Backlight level
    tft.setRotation(1);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);

    Serial.println("Initializing WiFi Client");
    if (!client) Serial.println("Unable to create client!");
    client->setCACert(rootCACertificate);
}

// The loop() function loops as long as there is power
void loop() {
    String userChoice = MY_USER;
    Serial.println("Fetching user details for " + userChoice);
    String userDetailsURL = "https://hacker-news.firebaseio.com/v0/user/";
    userDetailsURL += userChoice + ".json";
    String userDetails = callAPI(userDetailsURL);

    Serial.println("User details fetched, parsing karma");
    DynamicJsonDocument userDetailsJSON(16384);
    DeserializationError userDetailsError = deserializeJson(userDetailsJSON, userDetails);
    static int previousKarma = 0;
    int userKarma = userDetailsJSON["karma"];
    bool karmaShown = false;
    if (previousKarma != userKarma) {
      Serial.println("Karma changed");
      karmaShown = true;
      if (previousKarma > userKarma) {
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_WHITE, TFT_RED);
      } else {
        tft.fillScreen(TFT_GREEN);
        tft.setTextColor(TFT_BLACK, TFT_GREEN);
      }
      tft.setTextDatum(MC_DATUM);
      tft.drawString(String(userKarma), 120, 67);
      previousKarma = userKarma;
      delay(2000);
    }
  
    String feedChoice = MY_FEED;
    Serial.println("Fetching " + feedChoice + " story list");
    String feedURL = "https://hacker-news.firebaseio.com/v0/";
    feedURL += feedChoice + "stories.json";
    String storyList = callAPI(feedURL);

    Serial.println("Story list fetched, parsing list");
    DynamicJsonDocument storyListJSON(16384);
    DeserializationError storyListError = deserializeJson(storyListJSON, storyList);
    static long previousStoryID = 0;
    long firstStoryID = storyListJSON[0];
    if (firstStoryID == previousStoryID && !karmaShown) {
        Serial.println("Story unchanged, waiting 60 seconds");
        delay(60000);
        Serial.println("-------------------------");
        return;
    }
    previousStoryID = firstStoryID;
    Serial.println("Fetching details for " + String(firstStoryID));
    String storyDetailsURL = "https://hacker-news.firebaseio.com/v0/item/";
    storyDetailsURL += String(firstStoryID) + ".json";
    String storyDetails = callAPI(storyDetailsURL);

    Serial.println("Story details fetched, parsing details");
    DynamicJsonDocument storyDetailsJSON(16384);
    DeserializationError storyDetailsError = deserializeJson(storyDetailsJSON, storyDetails);
    const char *storyBy = storyDetailsJSON["by"];
    int storyScore = storyDetailsJSON["score"];
    const char *storyTitle = storyDetailsJSON["title"];
    
    String storyURL = "https://news.ycombinator.com/item?id=";
    storyURL += String(firstStoryID);
    Serial.println("Rendering QR Code for " + storyURL);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    qrcode.init();
    qrcode.create(storyURL);

    Serial.println("Printing Story Title " + String(storyTitle));
    // 20 columns, 2 rows, then 12 columns, 6 rows
    wordwrap.ellipsis = false;
    String storyTitle_1 = wordwrap.box(storyTitle, 2, 20);
    tft.println(storyTitle_1);
    if (wordwrap.overflowed) {
        wordwrap.ellipsis = true;
        String storyTitle_2 = String(storyTitle).substring(storyTitle_1.length());
        tft.println(wordwrap.box(storyTitle_2, 6, 12));
    } else {
        tft.println();
        tft.println("Score: " + String(storyScore));
        tft.println();
        tft.println(wordwrap.box(String(storyBy), 3, 12));
    }

    Serial.println("Waiting 60 seconds before checking again");
    delay(60000);
    Serial.println("-------------------------");
}
