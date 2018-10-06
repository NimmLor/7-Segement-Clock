#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/* ----------------------------------------------SETTINGS---------------------------------------------- */
const char* ssid = "YOURWIFINAME";                  // Your WiFi SSID
const char* password = "YOURPASSWORD";           // Your WiFi password
const char* mqtt_server = "192.168.0.107";    // Enter the IP-Address of your Raspberry Pi


#define mqtt_auth 0           // Set this to 0 to disable authentication
#define mqtt_user "YOURUSERNAME"      // Username for mqtt, not required if auth is disabled
#define mqtt_password "YOURPASSWORD" // Password for mqtt, not required if auth is disabled

#define mqtt_topic "miniclock"    // here you have to set the topic for mqtt

#define PIN 2                 // Pin of the led strip, default 2 (that is D4 on the wemos)
/* ----------------------------------------------SETTINGS END---------------------------------------------- */


/* ----------------------------------------------GLOBALS---------------------------------------------- */
int mqttdata = 0;
WiFiClient espClient;
PubSubClient client(espClient);
int hours = 0;
int mins = 0;

// Timer stuff
int T_hours = 0;
int T_mins = 0;
int T_secs = 0;
unsigned long timermillis = 0;
unsigned long weathermillis;
unsigned long alarmmillis;

int weathersecs = 0;

int alarmstate = 0;
int alarmcounter = 0;

int pt1 = 0;
int pt2 = 0;

// State of the dots
int dot1 = 0;
int dot2 = 0;

unsigned long timemillis;

int newData = 0;
const byte numChars = 50;
char receivedChars[numChars]; // an array to store the received data

// Numbers for the digits
int d1 = 99;
int d2 = 99;
int d3 = 99;
int d4 = 99;

// Color of the digits
int cd[] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };

// Color of the alarm digits
int fadespeed = 0;
int fadeamount = 0;
int ad = 0;
int ar = 255;
int ag = 255;
int ab = 255;

// Colors of the dots
int cdo[] = { 255, 255, 255, 255, 255, 255 };

int wr = 255;
int wg = 255;
int wb = 255;

char type = '0';  // current mode
char mymode2 = '0';
char old_type = '0';  // current mode

char vals[14][6] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
/* ----------------------------------------------GLOBALS END---------------------------------------------- */


/* ----------------------------------------------LED STRIP CONFIG---------------------------------------------- */
#define NUMPIXELS      30
//offsets
#define Digit1 0
#define Digit2 7
#define Digit3 16
#define Digit4 23

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
/* ----------------------------------------------LED STRIP CONFIG END---------------------------------------------- */


/* ----------------------------------------------FUNCTIONS START---------------------------------------------- */

/*
 * Function: serialNew
 * Used to process new data
 * Called when a new message arrives
 * Parameters: none
 */
void serialNew() {
	displayData();
	GetMode();
	CallMode();
	newData = false;
	mqttdata = 0;
}

/*
 * Function: displayData
 * Prints the message into serial console for debugging
 * Called when a new message arrives
 * Parameters: none
 */
void displayData()
{
	if (newData == true) {
		Serial.print("This just in ... ");
		Serial.println(receivedChars);
	}
}

/*
 * Function: GetMode
 * Extracts the type from the first character of the message
 * Called when a new message arrives
 * Parameters: none
 */
void GetMode()
{
	old_type = type;
	if (old_type == '0')pixels.setBrightness(40);
	type = receivedChars[0];
}

/*
 * Function: CallMode
 * Function calls the corresponding function to the type character
 * Called when a new message arrives
 * Parameters: none
 */
void CallMode()
{
	char mymode = type;
	if (mymode == 's')SetTime();      // s;2;2;5;8
	if (mymode == 'd')SetDots();      // d;1;0
	if (mymode == 'e')SetDotColors(); // e;255;0;0;0;255;0
	if (mymode == 'h')Set1DotColor(); // h;1;255;0;0
	if (mymode == 'i')Set1Dot();      // i;1
	if (mymode == 'f')Set1Color();    // f;1;8;255;34
	if (mymode == 'g')SetColors();    // g;255;10;3;100;255;200;255;10;3;100;255;200
	if (mymode == 'c')ModeClock();    // ignore
	if (mymode == 'z')CustomValues(); // z;0;2;0;4
	if (mymode == 'a')SetMode();      // a;c
	if (mymode == 't')SetTimerDyn();  // t;01;15;30
	if (mymode == 'b')SetBrightness();//b;80
	if (mymode == '!')Alarm();        // !
	if (mymode == '0')Off();          // a;0
	if (mymode == 'w')Weather();      // w;1;9;+;1;5
	if (mymode == '*') {
		mymode2 = '*';
		SetFadeSpeed();
	}
	if (mymode == '#')mymode2 = '0';
	if (mymode == '*' || mymode == '#' || mymode == '0' || mymode == '!')ResetAlarmLeds();
	pixels.show();
}


/*
 * Function: ResetAlarmLeds
 * Resets alarm status to initial values
 * Called when alarm ends
 * Parameters: none
 */
void ResetAlarmLeds()
{
	ad = 0;
	ar = 255;
	ag = 255;
	ab = 255;
}

/*
 * Function: Weather
 * Extracts temperature readings and display duration from the message,
 * afterwards it draws the temperature symbol(°C) and the temperature
 * Called when a type is 'w'
 * Parameters: none
 */
void Weather()
{
	int t1 = receivedChars[2] - '0';
	int t2 = receivedChars[4] - '0';
	pt1 = t1;
	pt2 = t2;
	char vz = receivedChars[6];
	weathersecs = (receivedChars[8] - '0') * 10 + receivedChars[10] - '0';
	int temp = t1 * 10 + t2;
	if (vz == '-')temp = temp * (-1);

	for (int i = 0; i < NUMPIXELS; i++)pixels.setPixelColor(i, pixels.Color(0, 0, 0));

	//WeatherSymbols
	pixels.setPixelColor(Digit3 + 0, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit3 + 1, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit3 + 2, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit3 + 3, pixels.Color(61, 225, 255));

	pixels.setPixelColor(Digit4 + 2, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit4 + 3, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit4 + 4, pixels.Color(61, 225, 255));
	pixels.setPixelColor(Digit4 + 5, pixels.Color(61, 225, 255));

	weathermillis = millis();
	GetWeatherColor(temp);
	if (vz == '-' && t1 == '0') {
		pixels.setPixelColor(Digit3 + 0, pixels.Color(61, 225, 255));
		pixels.setPixelColor(Digit3 + 1, pixels.Color(61, 225, 255));
	}
	if (t1 != 0)
	{
		if (mymode2 == '*')DrawDigit(Digit1, ar, ag, ab, t1);
		else DrawDigit(Digit1, wr, wg, wb, t1);
	}
	if (mymode2 == '*')DrawDigit(Digit2, ar, ag, ab, t2);
	else DrawDigit(Digit2, ar, ag, ab, t2);
	pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
	pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
	pixels.show();
	Serial.printf("Showing the weather for %d seconds\n", weathersecs);


	if (t1 != 0)DrawDigit(Digit1, wr, wg, wb, t1);
	DrawDigit(Digit2, wr, wg, wb, t2);
	pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
	pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
	pixels.show();
	Serial.printf("Showing the weather for %d seconds\n", weathersecs);
}

/*
 * Function: GetWeatherColor
 * Used to get temperature color, high temperature = more yellow, low temperature = more blue
 * Called by function Weather
 * Parameters: temperature in °C
 */
void GetWeatherColor(int temp)
{
	int R = 255; int G = 255; int B = 255;
	String hex = "";
	if (temp < -40) {
		R = 0;
		G = 197;
		B = 229;
	}
	else if (temp < -30) {
		R = 28;
		G = 203;
		B = 204;
	}
	else if (temp < -20) {
		R = 56;
		G = 209;
		B = 180;
	}
	else if (temp < -10) {
		R = 84;
		G = 216;
		B = 156;
	}
	else if (temp < 0) {
		R = 112;
		G = 222;
		B = 131;
	}
	else if (temp < 4) {
		R = 140;
		G = 229;
		B = 107;
	}
	else if (temp < 8) {
		R = 168;
		G = 235;
		B = 83;
	}
	else if (temp < 14) {
		R = 196;
		G = 242;
		B = 58;
	}
	else if (temp < 18) {
		R = 224;
		G = 248;
		B = 34;
	}
	else if (temp < 22) {
		R = 253, G = 244, B = 10;
	}
	else if (temp < 26) {
		R = 253, G = 233, B = 10;
	}
	else if (temp < 30) {
		R = 254, G = 142, B = 10;
	}
	else if (temp < 34) {
		R = 254, G = 105, B = 10;
	}
	else if (temp > 40) {
		R = 255, G = 68, B = 10;
	}
	wr = R; wg = G; wb = B;
}

/*
 * Function: Off
 * Turns every pixel off and sets brightness to 0
 * Called when mode is '0'
 * Parameters: none
 */
void Off()
{
	Serial.println("Clock is turning off");
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(0, 0, 0));
	}
	pixels.setBrightness(0);
	pixels.show();
}

/*
 * Function: Set1Dot
 * Used to set the color of the dot
 * Called when a new message arrives
 * Parameters: none
 */
void Set1Dot()
{
	char temp = receivedChars[2];
	//Serial.print(temp);
	if (temp == '0') {
		pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
		dot1 = 0;
	}
	if (temp == '1') {
		pixels.setPixelColor(Digit3 - 1, pixels.Color(cdo[3], cdo[4], cdo[5]));
		dot1 = 1;
	}
	if (temp == '2') {
		pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
		dot2 = 0;
	}
	if (temp == '3') {
		pixels.setPixelColor(Digit3 - 2, pixels.Color(cdo[3], cdo[4], cdo[5]));
		dot2 = 1;
	}
	DrawDots();
	pixels.show();
	type = old_type;
}


/*
 * Function: SetTimerDyn
 * Used to read time of the message and start the timer
 * Called when mode is "t"
 * Parameters: none
 */
void SetTimerDyn()  //t;01;22;03
{
	int index = 2; int index2 = 0;
	int temp_h = 0; int temp_m = 0; int temp_s = 0;
	temp_h = receivedChars[2] - '0';
	temp_h *= 10;
	temp_h += receivedChars[3] - '0';

	temp_m = receivedChars[5] - '0';
	temp_m *= 10;
	temp_m += receivedChars[6] - '0';

	temp_s = receivedChars[8] - '0';
	temp_s *= 10;
	temp_s += receivedChars[9] - '0';

	T_hours = temp_h;
	T_mins = temp_m;
	T_secs = temp_s;
	Serial.printf("Timer Started: %d:%d:%d\n", T_hours, T_mins, T_secs);
	DrawTimer();
	timermillis = millis();
	type = 't';
	pixels.show();
}


/*
 * Function: SetMode
 * Used to set the mode manually and to print the mode into serial
 * Called when mode is "a"
 * Parameters: none
 */
void SetMode()
{
	type = receivedChars[2];
	Serial.printf("Mode updated: %c", type);
	if (type == '0')Off();
	if (type == 'c') {
		if (mymode2 != '*')DrawTime();
		if (mymode2 != '*')DrawDots();
	}
}

/*
 * Function: SetFadeSpeed
 * Used to determine the fade amount and the fade delay
 * Called when mode is "*"
 * Parameters: none
 * Note: untilize a switch statement instead of 10x if()
 */
void SetFadeSpeed()
{
	//*;5
	int value = receivedChars[2] - '0';
	if (value == 0) {
		fadeamount = 1;
		fadespeed = 1000;
	}
	if (value == 1) {
		fadeamount = 1;
		fadespeed = 500;
	}
	if (value == 2) {
		fadeamount = 1;
		fadespeed = 250;
	}
	if (value == 3) {
		fadeamount = 1;
		fadespeed = 100;
	}
	if (value == 4) {
		fadeamount = 1;
		fadespeed = 25;
	}
	if (value == 5) {
		fadeamount = 2;
		fadespeed = 25;
	}
	if (value == 6) {
		fadeamount = 2;
		fadespeed = 0;
	}
	if (value == 7) {
		fadeamount = 3;
		fadespeed = 0;
	}
	if (value == 8) {
		fadeamount = 4;
		fadespeed = 0;
	}
	if (value == 9) {
		fadeamount = 12;
		fadespeed = 0;
	}

}


/*
 * Function: SetDots
 * Used to read dot values and to draw the dots
 * Called when mode is "d"
 * Parameters: none
 */
void SetDots()
{
	dot1 = receivedChars[2] - '0';
	dot2 = receivedChars[4] - '0';
	DrawDots();
	pixels.show();
	type = old_type;
}

/*
 * Function: SetDotColors
 * Used to set both dot colors
 * Called when mode is "e"
 * Parameters: none
 */
void SetDotColors()
{
	int index = 2; int index2 = 0;
	while (receivedChars[index] != ';') {
		vals[0][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[1][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[2][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[3][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[4][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != '\0') {
		vals[5][index2] = receivedChars[index];
		index++;
		index2++;
	}

	cdo[0] = atoi(vals[0]);
	cdo[1] = atoi(vals[1]);
	cdo[2] = atoi(vals[2]);
	cdo[3] = atoi(vals[3]);
	cdo[4] = atoi(vals[4]);
	cdo[5] = atoi(vals[5]);

	DrawDots();


	if (type == 'c') DrawTime();
	else if (type == 't') DrawTimer();
	pixels.show();
	type = old_type;
}

/*
 * Function: DrawDots
 * Used to draw the dots
 * Called when dots state or color gets changed
 * Parameters: none
 */
void DrawDots()
{
	if (dot1)pixels.setPixelColor(Digit3 - 1, pixels.Color(cdo[0], cdo[1], cdo[2]));
	else pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
	if (dot2)pixels.setPixelColor(Digit3 - 2, pixels.Color(cdo[3], cdo[4], cdo[5]));
	else pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
}

/*
 * Function: Set1DotColor
 * Used to set the color of a single dot
 * Called when mode is "h"
 * Parameters: none
 */
void Set1DotColor()
{
	int index = 4; int index2 = 0;
	char dotnr = receivedChars[2];
	while (receivedChars[index] != ';') {
		vals[0][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[1][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[2][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[3][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[4][index2] = receivedChars[index];
		index++;
		index2++;
	}
	index2 = 0; index++;
	while (receivedChars[index] != '\0') {
		vals[5][index2] = receivedChars[index];
		index++;
		index2++;
	}

	if (dotnr == '1')
	{
		cdo[0] = atoi(vals[0]);
		cdo[1] = atoi(vals[1]);
		cdo[2] = atoi(vals[2]);
	}
	if (dotnr == '2')
	{
		cdo[3] = atoi(vals[0]);
		cdo[4] = atoi(vals[1]);
		cdo[5] = atoi(vals[2]);
	}
	Serial.printf("Setting Dot Color of %c", dotnr);
	DrawDots();

	if (type == 'c') DrawTime();
	else if (type == 't') DrawTimer();

	type = old_type;
	pixels.show();
}

/*
 * Function: Set1DotColor
 * Used to set the color of a single dot
 * Called when mode is "h"
 * Parameters: none
 */
void Set1Color()
{
	int mydigit = receivedChars[2] - '0';
	int index = 4; int index2 = 0;
	while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
		vals[0][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[0][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
		vals[1][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[1][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
		vals[2][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[2][index2] = '\0';
	index2 = 0; index++;

	int t_r = atoi(vals[0]);
	int t_g = atoi(vals[1]);
	int t_b = atoi(vals[2]);
	Serial.printf("r:%d g:%d b:%d\n", t_r, t_g, t_b);

	if (mydigit == 1 || mydigit == 0) {
		cd[0] = t_r;
		cd[1] = t_g;
		cd[2] = t_b;
	};
	if (mydigit == 2 || mydigit == 0) {
		cd[3] = t_r;
		cd[4] = t_g;
		cd[5] = t_b;
	};
	if (mydigit == 3 || mydigit == 0) {
		cd[6] = t_r;
		cd[7] = t_g;
		cd[8] = t_b;
	};
	if (mydigit == 4 || mydigit == 0) {
		cd[9] = t_r;
		cd[10] = t_g;
		cd[11] = t_b;
	};
	type = old_type;
	if (type == 'c') DrawTime();
	else if (type == 't') DrawTimer();
	DrawDots();
	pixels.show();

}

/*
 * Function: SetColors
 * Used to set individual colors for all digits
 * Called when mode is "g"
 * Parameters: none
 */
void SetColors()
{
	int index = 2; int index2 = 0;
	while (receivedChars[index] != ';') {
		vals[0][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[0][index2] = '\0';

	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[1][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[1][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[2][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[2][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[3][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[3][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[4][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[4][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[5][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[5][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[6][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[6][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[7][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[7][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[8][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[8][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[9][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[9][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != ';') {
		vals[10][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[10][index2] = '\0';
	index2 = 0; index++;
	while (receivedChars[index] != '\0') {
		vals[11][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[12][index2] = '\0';
	index2 = 0; index++;
	cd[0] = atoi(vals[0]);
	cd[1] = atoi(vals[1]);
	cd[2] = atoi(vals[2]);
	cd[3] = atoi(vals[3]);
	cd[4] = atoi(vals[4]);
	cd[5] = atoi(vals[5]);
	cd[6] = atoi(vals[6]);
	cd[7] = atoi(vals[7]);
	cd[8] = atoi(vals[8]);
	cd[9] = atoi(vals[9]);
	cd[10] = atoi(vals[10]);
	cd[11] = atoi(vals[11]);

	Serial.printf("Setting Colors: r:%s g:%s b:%s r:%s g:%s b:%s r:%s g:%s b:%s r:%s g:%s b:%s  \n", vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7], vals[8], vals[9], vals[10], vals[11]);
	type = old_type;
	if (type == 'c') DrawTime();
	else if (type == 't') DrawTimer();
	DrawDots();
	pixels.show();
}

/*
 * Function: SetBrightness
 * Used to set the brightness of the clock
 * Called when mode is "b"
 * Parameters: none
 */
void SetBrightness()
{
	int index = 2; int index2 = 0;
	while (receivedChars[index] != '\0') {
		vals[0][index2] = receivedChars[index];
		index++;
		index2++;
	}
	vals[0][index2] = '\0';
	int brightness = atoi(vals[0]);
	Serial.printf("Setting brightness to %d%%\n", brightness);
	pixels.setBrightness(brightness);


	type = old_type;
	if (mymode2 != '*')DrawDots();
	pixels.show();


	if (type == 'c' && mymode2 != '*') DrawTime();
	else if (type == 't' && mymode2 != '*') DrawTimer();
}

/*
 * Function: ModeClock
 * Updates time if the time difference to the last update exceeds 60 seconds
 * Called in the loop
 * Parameters: none
 */
void ModeClock()
{
	if ((millis() - timemillis) >= 60000)
	{
		long minidiff = millis() - timemillis;
		timemillis = millis() - minidiff;
		mins++;
		if (mins >= 60) {
			mins = 0;
			hours++;
		}
		if (hours >= 24) hours = 0;
		if (type == 'c')
		{
			if (mymode2 != '*')DrawTime();
			if (mymode2 != '*')DrawDots();
			pixels.show();
		}
	}
}

/*
 * Function: ModeClock
 * Updates the timer if the time difference to the last update exceeds 1 second
 * Called in the loop if mode is "t"
 * Parameters: none
 */
void ModeTimerDyn()
{
	if (T_hours <= 0 && T_mins <= 0 && T_secs <= 0)type = '1';
	else
	{
		if ((millis() - timermillis) >= 1000)
		{
			int minidiff = millis() - timermillis;
			timermillis = millis() - minidiff;
			T_secs--;
			if (T_secs <= 0)
			{
				if (T_mins > 0 || T_hours > 0)T_secs = 59;
				T_mins--;
				if (T_mins <= 0)
				{
					if (T_hours > 0)T_mins = 59;
					T_hours--;
				}
			}
			DrawTimer();
		}
	}
}


/*
 * Function: ModeWeather
 * If the weather display duration is reached, then the mode will be set to clock
 * Called in the loop if mode is "w"
 * Parameters: none
 */
void ModeWeather()
{
	if (mymode2 == '*')
	{
		DrawDigit(Digit1, ar, ag, ab, pt1);
		DrawDigit(Digit2, ar, ag, ab, pt2);
	}
	if (((millis() - weathermillis) / 1000) > weathersecs)
	{
		Serial.println("Setting mode back to Clock");
		type = 'c';
		if (mymode2 != '*')DrawTime();
		if (mymode2 != '*')DrawDots();
		pixels.show();
	}
}

/*
 * Function: TimerAlarm
 * Flashes all digits on/off and fades the colors
 * Called when the timer runs out (mode is = "1")
 * Parameters: none
 */
void TimerAlarm()
{
	if (alarmstate)
	{
		if ((millis() - timermillis) >= 350)
		{
			Serial.println("ALARM!");
			timermillis = millis();
			alarmstate = 0; pixels.setBrightness(0); pixels.show();
		}
	}
	else
	{
		if ((millis() - timermillis) >= 100)
		{
			Serial.println("ALARM!");
			timermillis = millis();
			alarmstate = 1; pixels.setBrightness(255);
			ShiftAlarmLeds(51);
			DrawDigit(Digit1, ar, ag, ab, 0);
			DrawDigit(Digit2, ar, ag, ab, 0);

			pixels.setPixelColor(Digit3 - 1, pixels.Color(ar, ag, ab));
			pixels.setPixelColor(Digit3 - 2, pixels.Color(ar, ag, ab));

			DrawDigit(Digit3, ar, ag, ab, 0);
			DrawDigit(Digit4, ar, ag, ab, 0);
			pixels.show();
		}
	}

}

/*
 * Function: Alarm
 * Flashes all digits on/off and fades the colors
 * Called when mode is "!"
 * Parameters: none
 */
void Alarm()
{
	if (alarmstate)
	{
		if ((millis() - timermillis) >= 350)
		{
			Serial.println("ALARM!");
			timermillis = millis();
			alarmstate = 0; pixels.setBrightness(0); pixels.show();
		}
	}
	else
	{
		if ((millis() - timermillis) >= 100)
		{
			Serial.println("ALARM!");
			timermillis = millis();
			alarmstate = 1; pixels.setBrightness(255);
			ShiftAlarmLeds(51);

			DrawDigit(Digit1, ar, ag, ab, hours / 10); //Draw the first digit of the hour
			DrawDigit(Digit2, ar, ag, ab, hours - ((hours / 10) * 10)); //Draw the second digit of the hour

			DrawDigit(Digit3, ar, ag, ab, mins / 10); //Draw the first digit of the minute
			DrawDigit(Digit4, ar, ag, ab, mins - ((mins / 10) * 10)); //Draw the second digit of the minute
			pixels.show();
		}
	}
}

/*
 * Function: ModeFade
 * Fades the colors
 * Called when mode2 is "*"
 * Parameters: none
 */
void ModeFade()
{
	if ((millis() - timermillis) >= fadespeed)
	{
		Serial.printf("fade! d=%d r=%d g=%d b=%d\n", ad, ar, ag, ab);
		timermillis = millis();
		alarmstate = 1;
		ShiftAlarmLeds(fadeamount);

		if (type != 'w')
		{
			DrawDigit(Digit1, ar, ag, ab, hours / 10); //Draw the first digit of the hour
			DrawDigit(Digit2, ar, ag, ab, hours - ((hours / 10) * 10)); //Draw the second digit of the hour

			DrawDigit(Digit3, ar, ag, ab, mins / 10); //Draw the first digit of the minute
			DrawDigit(Digit4, ar, ag, ab, mins - ((mins / 10) * 10)); //Draw the second digit of the minute

			if (dot1)pixels.setPixelColor(Digit3 - 1, pixels.Color(ar, ag, ab));
			else pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
			if (dot2)pixels.setPixelColor(Digit3 - 2, pixels.Color(ar, ag, ab));
			else pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
		}
		else

			pixels.show();
	}
}


/*
 * Function: DrawTimer
 * Displays remaining time in HH:MM or MM:SS depending on time
 * Called in loop when mode is "w"
 * Parameters: none
 */
void DrawTimer()
{
	Serial.printf("Updating Timer: %d:%d:%d\n", T_hours, T_mins, T_secs);

	if (T_hours > 0)
	{
		DrawDigit(Digit1, cd[0], cd[1], cd[2], T_hours / 10);
		DrawDigit(Digit2, cd[3], cd[4], cd[5], T_hours - ((T_hours / 10) * 10));

		DrawDigit(Digit3, cd[6], cd[7], cd[8], T_mins / 10);
		DrawDigit(Digit4, cd[9], cd[10], cd[11], T_mins - ((T_mins / 10) * 10));
	}
	else
	{
		DrawDigit(Digit1, cd[0], cd[1], cd[2], T_mins / 10);
		DrawDigit(Digit2, cd[3], cd[4], cd[5], T_mins - ((T_mins / 10) * 10));

		DrawDigit(Digit3, cd[6], cd[7], cd[8], T_secs / 10);
		DrawDigit(Digit4, cd[9], cd[10], cd[11], T_secs - ((T_secs / 10) * 10));
	}
}

/*
 * Function: CustomValues
 * Displays custom values on the clock
 * Called when mode is "z"
 * Parameters: none
 */
void CustomValues()
{
	d1 = receivedChars[2] - '0';
	d2 = receivedChars[4] - '0';
	d3 = receivedChars[6] - '0';
	d4 = receivedChars[8] - '0';
	Serial.print("Setting custom values!");
	if (d1 < 10) DrawDigit(Digit1, cd[0], cd[1], cd[2], d1); //Draw the first digit of the hour
	else DisableDigit(1);
	if (d2 < 10) DrawDigit(Digit2, cd[3], cd[4], cd[5], d2); //Draw the second digit of the hour
	else DisableDigit(2);

	if (d3 < 10) DrawDigit(Digit3, cd[6], cd[7], cd[8], d3); //Draw the first digit of the minute
	else DisableDigit(3);
	if (d4 < 10) DrawDigit(Digit4, cd[9], cd[10], cd[11], d4); //Draw the second digit of the minute
	else DisableDigit(4);
	pixels.show();
	type = 'z';
}

/*
 * Function: DrawTime
 * Displays current time
 * Called in loop when mode is "w"
 * Parameters: none
 */
void DrawTime()
{
	Serial.printf("Updating Time: %d:%d\n", hours, mins);
	DrawDigit(Digit1, cd[0], cd[1], cd[2], hours / 10); //Draw the first digit of the hour
	DrawDigit(Digit2, cd[3], cd[4], cd[5], hours - ((hours / 10) * 10)); //Draw the second digit of the hour

	DrawDigit(Digit3, cd[6], cd[7], cd[8], mins / 10); //Draw the first digit of the minute
	DrawDigit(Digit4, cd[9], cd[10], cd[11], mins - ((mins / 10) * 10)); //Draw the second digit of the minute
}

/*
 * Function: SetTime
 * Updates local time
 * Called in loop when mode is "s"
 * Parameters: none
 */
void SetTime()
{
	timemillis = millis();
	Serial.print("\nSetting the time: ");
	hours = 0; mins = 0;
	hours = 10 * (receivedChars[2] - '0');
	hours = hours + (receivedChars[4] - '0');
	mins = 10 * (receivedChars[6] - '0');
	mins = mins + (receivedChars[8] - '0');

	Serial.print(hours);
	Serial.print(":");
	Serial.println(mins);
	if (old_type == 'c')DrawTime();
	type = old_type;
}

/*
 * Function: DisableDigit
 * Displays remaining time in HH:MM or MM:SS depending on time
 * Parameters: position on the clock (1-4)
 */
void DisableDigit(int mydigit)
{
	if (mydigit == 1)
	{
		for (int i = 0; i < 14; i++)
		{
			pixels.setPixelColor(Digit1 + i, pixels.Color(0, 0, 0));
		}
	}
	if (mydigit == 2)
	{
		for (int i = 0; i < 14; i++)
		{
			pixels.setPixelColor(Digit2 + i, pixels.Color(0, 0, 0));
		}
	}
	if (mydigit == 3)
	{
		for (int i = 0; i < 14; i++)
		{
			pixels.setPixelColor(Digit3 + i, pixels.Color(0, 0, 0));
		}
	}
	if (mydigit == 4)
	{
		for (int i = 0; i < 14; i++)
		{
			pixels.setPixelColor(Digit4 + i, pixels.Color(0, 0, 0));
		}
	}
	pixels.show();
}

/*
 * Function: DrawDigit
 * Lights up segments depending on the value
 * Parameters:
 * - offset: position on the clock (1-4)
 * - r: red component (0-255)
 * - g: green component (0-255)
 * - b: blue component (0-255)
 * - n: value to be drawn (0-9)
 */
void DrawDigit(int offset, int r, int g, int b, int n)
{

	if (n == 2 || n == 3 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9) //MIDDLE
	{
		pixels.setPixelColor(0 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(0 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 1 || n == 2 || n == 3 || n == 4 || n == 7 || n == 8 || n == 9) //TOP RIGHT
	{
		pixels.setPixelColor(1 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(1 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //TOP
	{
		pixels.setPixelColor(2 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(2 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9) //TOP LEFT
	{
		pixels.setPixelColor(3 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(3 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 2 || n == 6 || n == 8) //BOTTOM LEFT
	{
		pixels.setPixelColor(4 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(4 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 8 || n == 9) //BOTTOM
	{
		pixels.setPixelColor(5 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(5 + offset, pixels.Color(0, 0, 0));
	}
	if (n == 0 || n == 1 || n == 3 || n == 4 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //BOTTOM RIGHT
	{
		pixels.setPixelColor(6 + offset, pixels.Color(r, g, b));
	}
	else
	{
		pixels.setPixelColor(6 + offset, pixels.Color(0, 0, 0));
	}
	pixels.show();
}

/*
 * Function: ShiftAlarmLeds
 * Updates local time
 * Called by Alarm(), TimerAlarm(), ModeFade()
 * Parameters: amount: speed of the fade effect
 */
void ShiftAlarmLeds(int amount)
{
	if (ad == 0)
	{
		ag = ag - amount; ab = ab - amount; if (ag <= 0 && ab <= 0)ad++; if (ag < 0) {
			ag = 0;
			ab = 0;
		}
	}
	if (ad == 1)
	{
		ag = ag + amount; if (ag >= 255)ad++; if (ag > 255)ag = 255;
	}
	if (ad == 2)
	{
		ar = ar - amount; if (ar <= 0)ad++; if (ar < 0)ar = 0;
	}
	if (ad == 3)
	{
		ab = ab + amount; if (ab >= 255)ad++; if (ab > 255)ab = 255;
	}
	if (ad == 4)
	{
		ag = ag - amount; if (ag <= 0)ad++; if (ag < 0)ag = 0;
	}
	if (ad == 5)
	{
		ar = ar + amount; if (ar >= 255)ad++; if (ar > 255)ar = 255;
	}
	if (ad == 6)
	{
		ab = ab - amount; if (ab <= 0)ad++; if (ab < 0)ab = 0;
	}
	if (ad == 7)
	{
		ab = ab + amount; ag = ag + amount; if (ab >= 255)ad = 0; if (ab > 255) {
			ab = 255;
			ag = 255;
		}
	}
}
/* ----------------------------------------------FUNCTIONS END---------------------------------------------- */

/* ----------------------------------------------COMMUNICATION---------------------------------------------- */
void reconnect() {
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		if (mqtt_auth == 1)
		{
			if (client.connect("ClockMini", mqtt_user, mqtt_password)) {
				Serial.println("connected");
				client.subscribe(mqtt_topic);
			}
			else {
				Serial.print("failed, rc=");
				Serial.print(client.state());
				Serial.println(" try again in 5 seconds");
				delay(5000);
			}
		}
		else
		{
			if (client.connect("ClockMini")) {
				Serial.println("connected");
				client.subscribe(mqtt_topic);
			}
			else {
				Serial.print("failed, rc=");
				Serial.print(client.state());
				Serial.println(" try again in 5 seconds");
				delay(5000);
			}
		}
	}
}
/* ----------------------------------------------COMUNICATION END---------------------------------------------- */

/* ----------------------------------------------MQTT INTERRUPT---------------------------------------------- */
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	int i = 0;
	for (i = 0; i < length; i++) {
		receivedChars[i] = (char)payload[i];
	}
	receivedChars[i] = '\0';
	Serial.printf(": %s\n", receivedChars);
	mqttdata = 1;
}
/* ----------------------------------------------MQTT INTERRUPT END---------------------------------------------- */

/* ----------------------------------------------SETUP---------------------------------------------- */
void setup()
{
	Serial.begin(115200);
	delay(3000);
	Serial.println();
	Serial.println("Clock is booting up!");
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	// Wait until the connection has been confirmed before continuing
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	// Debugging - Output the IP Address of the ESP8266
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	delay(1000);
	Serial.println("Clock is now ready!");
	type = 'c';
	pixels.begin();

	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(255, 0, 0));
		pixels.show();
		delay(25);
	}
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(0, 255, 0));
		pixels.show();
		delay(25);
	}
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(0, 0, 255));
		pixels.show();
		delay(25);
	}
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(255, 255, 255));
		pixels.show();
		delay(25);
	}
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(0, 0, 0));
		pixels.show();
		delay(25);
	}


}
/* ----------------------------------------------SETUP END---------------------------------------------- */



/* ----------------------------------------------LOOP---------------------------------------------- */
void loop()
{
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
	ModeClock();
	if (mqttdata > 0)serialNew();
	else if (type == 't')ModeTimerDyn();
	else if (type == 'w')ModeWeather();
	else if (type == '1')TimerAlarm();
	else if (type == '!')Alarm();
	if (mymode2 == '*')ModeFade();;
}
/* ----------------------------------------------LOOP END---------------------------------------------- */
