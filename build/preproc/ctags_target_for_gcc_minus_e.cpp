# 1 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"

/*



	Test sketch 

	Reading the bits (value >> bit) & 1



*/
# 11 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
# 12 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2
# 13 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2
# 14 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2
# 15 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2
# 16 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2
# 17 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 2

// SETTINGS



// OBJERCTS
SBuff SBuffer(64);
CMDHand CMDHandler(';', ',');
DHT dht(7, 11 /**< DHT TYPE 11 */);
RTC_DS3231 RTC;

// VARIABLES
CRGB segments[30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/];


void(*reset) (void) = 0;

void printTime()
{
 DateTime now = RTC.now();
 char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

 Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
 Serial.print(' ');
 Serial.print(now.day(), 10);
 Serial.print('.');
 Serial.print(now.month(), 10);
 Serial.print('.');
 Serial.print(now.year(), 10);
 Serial.print(". ");
 Serial.print(now.hour(), 10);
 Serial.print(':');
 Serial.print(now.minute(), 10);
 Serial.print(':');
 Serial.println(now.second(), 10);
}

void exeCMD(const char *commandSet)
{
 const char *cmd = CMDHandler.find(commandSet, 1);

 if (!strcmp(cmd, "rtc_adjust"))
 {
  if (CMDHandler.count(CMDHandler.getNext(1), 1) == 6)
  {
   uint8_t params[6]; // YEAR - 2000, MONTH, DAY, HOUR, MINUTE, SECOND
   for (uint8_t i = 0; i < 6; i++) params[i] = atoi(CMDHandler.find(nullptr, 1));

   RTC.adjust(DateTime(params[0] + 2000, params[1], params[2], params[3], params[4], params[5]));

   Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 67 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
               (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 67 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
               "New time is: "
# 67 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
               ); &__c[0];}))
# 67 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
               )));
   printTime();
  }
  else Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 70 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 70 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                     "rtc_adjust [Year - 2000],[Month],[Day],[Hour],[Minute],[Second]"
# 70 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                     ); &__c[0];}))
# 70 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                     )));
 }
 else if (!strcmp(cmd, "rtc"))
 {
  printTime();
  Serial.print(RTC.getTemperature(), 2);
  Serial.println("°C");
 }
 else if (!strcmp(cmd, "segments"))
 {
  // RED COLOR
  for (uint8_t segmentIdx = 0; segmentIdx < 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/; segmentIdx++) segments[segmentIdx].setRGB(255, 0, 0);
  FastLED.show();
  delay(3000);

  // GREEN COLOR
  for (uint8_t segmentIdx = 0; segmentIdx < 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/; segmentIdx++) segments[segmentIdx].setRGB(0, 255, 0);
  FastLED.show();
  delay(3000);

  // BLUE COLOR
  for (uint8_t segmentIdx = 0; segmentIdx < 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/; segmentIdx++) segments[segmentIdx].setRGB(0, 0, 255);
  FastLED.show();
  delay(3000);

  // OFF
  for (uint8_t segmentIdx = 0; segmentIdx < 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/; segmentIdx++) segments[segmentIdx].setRGB(0, 0, 0);
  FastLED.show();
 }
 else if (!strcmp(cmd, "buzzer"))
 {
  uint8_t paramCount = CMDHandler.count(CMDHandler.getNext(1), 1);

  if (!paramCount)
  {
   uint16_t freq[7] = { 1500, 2000, 2500, 3000, 3500, 4000, 4500 };

   for (uint8_t i = 0; i < 7; i++)
   {
    tone(9, freq[i], 1000);
    delay(1500);
   }
  }
  else if (paramCount == 1)
  {
   tone(9, atoi(CMDHandler.find(nullptr, 1)), 1000);
   delay(1500);
  }
  else
  {
   uint16_t freq = atoi(CMDHandler.find(nullptr, 1));
   uint16_t duration = atoi(CMDHandler.find(nullptr, 1));
   tone(9, freq, duration);
   delay(duration);
  }
 }
 else if (!strcmp(cmd, "dht11"))
 {
  Serial.print(dht.readTemperature(), 2);
  Serial.print("°C | ");
  Serial.print(dht.readHumidity(), 2);
  Serial.println('%');
 }
 else if (!strcmp(cmd, "color"))
 {
  uint8_t paramCount = CMDHandler.count(CMDHandler.getNext(1), 1);

  if (paramCount == 3) // ALL LEDS
  {
   uint8_t i = 0;
   uint8_t colors[3];

   for (; i < 3; i++) colors[i] = atoi(CMDHandler.find(nullptr, 1));
   for (i = 0; i < 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/; i++) segments[i].setRGB(colors[0], colors[1], colors[2]);
   FastLED.show();
  }
  else if (paramCount == 4) // SELECTED LED
  {
   uint8_t colors[3];
   uint8_t segmentIdx = atoi(CMDHandler.find(nullptr, 1));
   for (uint8_t i = 0; i < 3; i++) colors[i] = atoi(CMDHandler.find(nullptr, 1));

   segments[segmentIdx].setRGB(colors[0], colors[1], colors[2]);
   FastLED.show();
  }
  else Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 155 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 155 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                     "Command needs 3/ or 4 params!"
# 155 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                     ); &__c[0];}))
# 155 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                     )));
 }
 else if (!strcmp(cmd, "reset")) reset(); // NOT BEST WAY FOR SOFT RESET
 else if (!strcmp(cmd, "help"))
 {
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 160 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 160 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                "------ HELP ------\nrtc_adjust - Adjusts RTC time\nrtc - Prints current time and temperature\nsegments - Tests each color\nbuzzer - Tests buzzer with custom or hardcoded freq\ndht11 - Prints humidity and temperature\ncolors - Sets one or all LED to certian color\nreset - Soft reset"
# 160 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                ); &__c[0];}))
# 160 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                )));
 }
 else
 {
  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 164 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 164 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
              "Command "
# 164 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
              ); &__c[0];}))
# 164 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
              )));
  Serial.print(cmd);
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 166 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 166 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                " does not exist!"
# 166 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                ); &__c[0];}))
# 166 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                )));
 }
}

void parseCMDs()
{
 char *command = CMDHandler.find(SBuffer.buffer(), 0);

 while (command != nullptr)
 {
  exeCMD(command);
  command = CMDHandler.find(nullptr, 0);
 }
}

void HM10CMD()
{
 if (strstr(SBuffer.buffer(), "OK+CONN") != nullptr || strstr(SBuffer.buffer(), "OK+LOST") != nullptr)
 {
  for (uint8_t i = 0; i < 30; i++) segments[i].setRGB(255, 255, 255);
  FastLED.show();
  tone(9, 3000, 100);
  delay(100);

  for (uint8_t i = 0; i < 30; i++) segments[i].setRGB(0, 0, 0);
  FastLED.show();
 }
}

void setup()
{
 delay(100);

 Serial.begin(115200);
 RTC.begin();
 dht.begin();
 FastLED.addLeds<WS2812, 5, GRB>(segments, 30 /* 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS*/);

 Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 204 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
               (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 204 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
               "3D Clock PCB test"
# 204 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
               ); &__c[0];}))
# 204 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
               )));
 if (RTC.lostPower()) Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 205 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 205 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                                    "RTC lost power, set correct time!"
# 205 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino" 3
                                    ); &__c[0];}))
# 205 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
                                    )));
}

void loop()
{
 while (Serial.available()) SBuffer.buff(Serial.read(), &parseCMDs);
 if (SBuffer.buffed() != 0 && (millis() - SBuffer.lastBuff()) > 250) SBuffer.end(HM10CMD); // POSSIBLE DATA FROM HM-10

 delay(1);
}
