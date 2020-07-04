#include <Arduino.h>
#line 1 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"

/*

	Test sketch 
	Reading the bits (value >> bit) & 1

*/

#define FASTLED_INTERNAL

#include 								<SerialBuffer.h>
#include 								<CommandHandler.h>
#include 								<DHT.h>
#include 								<RTClib.h>
#include 								<Rotary.h>
#include 								<FastLED.h>

// SETTINGS
#define SEGMENTS 						30 // 4 * 4 VERTICAL SEGMENTS + 4 * 3 HORIZONTAL SEGMENTS + 2 DOT SEGMENTS
#define DATA_PIN						5

// OBJERCTS
SBuff SBuffer(64);
CMDHand CMDHandler(';', ',');
DHT dht(7, DHT11);
RTC_DS3231 RTC;

// VARIABLES
CRGB segments[SEGMENTS];


void(*reset) (void) = 0;

#line 34 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void printTime();
#line 54 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void exeCMD(const char *commandSet);
#line 170 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void parseCMDs();
#line 181 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void HM10CMD();
#line 195 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void setup();
#line 208 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void loop();
#line 34 "c:\\Users\\silvi\\OneDrive\\Projekti\\3D-Clock-SW\\Firmware\\Test\\Test.ino"
void printTime()
{
	DateTime now = RTC.now();
	char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
	Serial.print(' ');
	Serial.print(now.day(), DEC);
	Serial.print('.');
	Serial.print(now.month(), DEC);
	Serial.print('.');
	Serial.print(now.year(), DEC);
	Serial.print(". ");
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.println(now.second(), DEC);
}

void exeCMD(const char *commandSet)
{
	const char *cmd = CMDHandler.find(commandSet, CMD_PARAM);

	if (!strcmp(cmd, "rtc_adjust"))
	{
		if (CMDHandler.count(CMDHandler.getNext(CMD_PARAM), CMD_PARAM) == 6)
		{
			uint8_t params[6]; // YEAR - 2000, MONTH, DAY, HOUR, MINUTE, SECOND
			for (uint8_t i = 0; i < 6; i++) params[i] = atoi(CMDHandler.find(nullptr, CMD_PARAM));

			RTC.adjust(DateTime(params[0] + 2000, params[1], params[2], params[3], params[4], params[5]));

			Serial.print(F("New time is: "));
			printTime();
		}
		else Serial.println(F("rtc_adjust [Year - 2000],[Month],[Day],[Hour],[Minute],[Second]"));
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
		for (uint8_t segmentIdx = 0; segmentIdx < SEGMENTS; segmentIdx++) segments[segmentIdx].setRGB(255, 0, 0);
		FastLED.show();
		delay(3000);

		// GREEN COLOR
		for (uint8_t segmentIdx = 0; segmentIdx < SEGMENTS; segmentIdx++) segments[segmentIdx].setRGB(0, 255, 0);
		FastLED.show();
		delay(3000);

		// BLUE COLOR
		for (uint8_t segmentIdx = 0; segmentIdx < SEGMENTS; segmentIdx++) segments[segmentIdx].setRGB(0, 0, 255);
		FastLED.show();
		delay(3000);

		// OFF
		for (uint8_t segmentIdx = 0; segmentIdx < SEGMENTS; segmentIdx++) segments[segmentIdx].setRGB(0, 0, 0);
		FastLED.show();		
	}
	else if (!strcmp(cmd, "buzzer"))
	{
		uint8_t paramCount = CMDHandler.count(CMDHandler.getNext(CMD_PARAM), CMD_PARAM);

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
			tone(9, atoi(CMDHandler.find(nullptr, CMD_PARAM)), 1000);
			delay(1500);				
		}
		else
		{
			uint16_t freq = atoi(CMDHandler.find(nullptr, CMD_PARAM));
			uint16_t duration = atoi(CMDHandler.find(nullptr, CMD_PARAM));
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
		uint8_t paramCount = CMDHandler.count(CMDHandler.getNext(CMD_PARAM), CMD_PARAM);
		
		if (paramCount == 3) // ALL LEDS
		{
			uint8_t i = 0;
			uint8_t colors[3];

			for (; i < 3; i++) colors[i] = atoi(CMDHandler.find(nullptr, CMD_PARAM));
			for (i = 0; i < SEGMENTS; i++) segments[i].setRGB(colors[0], colors[1], colors[2]);
			FastLED.show();
		}
		else if (paramCount == 4) // SELECTED LED
		{
			uint8_t colors[3];
			uint8_t segmentIdx = atoi(CMDHandler.find(nullptr, CMD_PARAM));
			for (uint8_t i = 0; i < 3; i++) colors[i] = atoi(CMDHandler.find(nullptr, CMD_PARAM));
				
			segments[segmentIdx].setRGB(colors[0], colors[1], colors[2]);
			FastLED.show();
		}
		else Serial.println(F("Command needs 3/ or 4 params!"));
	}
	else if (!strcmp(cmd, "reset")) reset(); // NOT BEST WAY FOR SOFT RESET
	else if (!strcmp(cmd, "help"))
	{
		Serial.println(F("------ HELP ------\nrtc_adjust - Adjusts RTC time\nrtc - Prints current time and temperature\nsegments - Tests each color\nbuzzer - Tests buzzer with custom or hardcoded freq\ndht11 - Prints humidity and temperature\ncolors - Sets one or all LED to certian color\nreset - Soft reset"));
	}
	else
	{
		Serial.print(F("Command "));
		Serial.print(cmd);
		Serial.println(F(" does not exist!"));
	}
}

void parseCMDs()
{
	char *command = CMDHandler.find(SBuffer.buffer(), CMD_SET);

	while (command != nullptr)
	{
		exeCMD(command);
		command = CMDHandler.find(nullptr, CMD_SET);
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
	FastLED.addLeds<WS2812, DATA_PIN, GRB>(segments, SEGMENTS);

	Serial.println(F("3D Clock PCB test"));
	if (RTC.lostPower()) Serial.println(F("RTC lost power, set correct time!"));
}

void loop()
{
	while (Serial.available()) SBuffer.buff(Serial.read(), &parseCMDs);
	if (SBuffer.buffed() != 0 && (millis() - SBuffer.lastBuff()) > 250) SBuffer.end(HM10CMD); // POSSIBLE DATA FROM HM-10

	delay(1);
}
