#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// settings
const char* ssid = "WIFI";                  // Your WiFi SSID
const char* password = "PASWWORD";           // Your WiFi password
const char* mqtt_server = "192.168.0.107";    // Enter the IP-Address of your Raspberry Pi


#define mqtt_auth 0           // Set this to 0 to disable authentication
#define mqtt_user ""      // Username for mqtt, not required if auth is disabled
#define mqtt_password "" // Password for mqtt, not required if auth is disabled

#define mqtt_topic ""    // here you have to set the topic for mqtt

#define PIN 2                 // Pin of the led strip, default 2 (that is D4 on the wemos)



// Globals
int mqttdata = 0;
WiFiClient espClient;
PubSubClient client(espClient);
int hours = 0;
int mins = 0;

// Timer stuff
int T_hours = 0;
int T_mins = 0;
int T_secs = 0;
unsigned long timermillis;
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
int r1 = 255;
int g1 = 255;
int b1 = 255;
int r2 = 255;
int g2 = 255;
int b2 = 255;
int r3 = 255;
int g3 = 255;
int b3 = 255;
int r4 = 255;
int g4 = 255;
int b4 = 255;

// Color of the alarm digits
int fadespeed = 0;
int fadeamount = 0;
int ad = 0;
int ar = 255;
int ag = 255;
int ab = 255;

// Colors of the dots
int dr1 = 255;
int dg1 = 255;
int db1 = 255;
int dr2 = 255;
int dg2 = 255;
int db2 = 255;

int wr = 255;
int wg = 255;
int wb = 255;

char type = '0';  // current mode
char mymode2 = '0';
char old_type = '0';  // current mode

char val1[10] = "";
char val2[10] = "";
char val3[10] = "";
char val4[10] = "";
char val5[10] = "";
char val6[10] = "";
char val7[10] = "";
char val8[10] = "";
char val9[10] = "";
char val10[10] = "";
char val11[10] = "";
char val12[10] = "";
char val13[10] = "";
char val14[10] = "";



//led strip

#define NUMPIXELS      58

//offsets
#define Digit1 0
#define Digit2 14
#define Digit3 30
#define Digit4 44

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



void serialNew() {
  //recvWithEndMarker();
  displayData();
  GetMode();
  CallMode();
  newData = false;
  mqttdata = 0;
}

void displayData()
{
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
  }
}


void GetMode()
{
  old_type = type;
  if (old_type == '0')pixels.setBrightness(40);
  type = receivedChars[0];
  //Serial.println(type);
}

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



void ResetAlarmLeds()
{
  ad = 0;
  ar = 255;
  ag = 255;
  ab = 255;
}


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
  pixels.setPixelColor(Digit3 + 4, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit3 + 5, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit3 + 6, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit3 + 7, pixels.Color(61, 225, 255));


  pixels.setPixelColor(Digit4 + 4, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 5, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 6, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 7, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 8, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 9, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 10, pixels.Color(61, 225, 255));
  pixels.setPixelColor(Digit4 + 11, pixels.Color(61, 225, 255));

  weathermillis = millis();
  GetWeatherColor(temp);
  if (vz == '-' && t1 == '0') {
    pixels.setPixelColor(Digit3 + 0, pixels.Color(61, 225, 255));
    pixels.setPixelColor(Digit3 + 1, pixels.Color(61, 225, 255));
  }
  if (t1 != 0)
  {
    if (mymode2 == '*')drawdigit(Digit1, ar, ag, ab, t1);
    else drawdigit(Digit1, wr, wg, wb, t1);
  }
  if (mymode2 == '*')drawdigit(Digit2, ar, ag, ab, t2);
  else drawdigit(Digit2, ar, ag, ab, t2);
  pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
  pixels.show();
  Serial.printf("Showing the weather for %d seconds\n", weathersecs);


  if (t1 != 0)drawdigit(Digit1, wr, wg, wb, t1);
  drawdigit(Digit2, wr, wg, wb, t2);
  pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
  pixels.show();
  Serial.printf("Showing the weather for %d seconds\n", weathersecs);
}

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

void Set1Dot()
{
  char temp = receivedChars[2];
  //Serial.print(temp);
  if (temp == '0') {
    pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
    dot1 = 0;
  }
  if (temp == '1') {
    pixels.setPixelColor(Digit3 - 1, pixels.Color(dr2, dg2, db2));
    dot1 = 1;
  }
  if (temp == '2') {
    pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
    dot2 = 0;
  }
  if (temp == '3') {
    pixels.setPixelColor(Digit3 - 2, pixels.Color(dr2, dg2, db2));
    dot2 = 1;
  }
  DrawDots();
  pixels.show();
  type = old_type;
}

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
    fadeamount = 4;
    fadespeed = 0;
  }
  if (value == 7) {
    fadeamount = 6;
    fadespeed = 0;
  }
  if (value == 8) {
    fadeamount = 12;
    fadespeed = 0;
  }
  if (value == 9) {
    fadeamount = 25;
    fadespeed = 0;
  }

}

void SetDots()
{
  dot1 = receivedChars[2] - '0';
  dot2 = receivedChars[4] - '0';
  //Serial.print(dot1);
  //Serial.print(dot2);
  //drawdigit(Digit3-2, dr1, dg1, db1, dot1); //Draw the first digit of the minute
  //drawdigit(Digit3-1, dr2, dg2, db2, dot2); //Draw the second digit of the minute

  DrawDots();

  pixels.show();

  type = old_type;
}

void SetDotColors()
{
  int index = 2; int index2 = 0;
  while (receivedChars[index] != ';') {
    val1[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val2[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val3[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val4[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val5[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != '\0') {
    val6[index2] = receivedChars[index];
    index++;
    index2++;
  }

  dr1 = atoi(val1);
  dg1 = atoi(val2);
  db1 = atoi(val3);
  dr2 = atoi(val4);
  dg2 = atoi(val5);
  db2 = atoi(val6);

  //Serial.printf("%d%d%d%d%d%d", dr1, dg1, db1, dr2, dg2, db2);
  DrawDots();


  if (type == 'c') DrawTime();
  else if (type == 't') DrawTimer();
  pixels.show();
  type = old_type;
}

void DrawDots()
{
  if (dot1)pixels.setPixelColor(Digit3 - 1, pixels.Color(dr1, dg1, db1));
  else pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
  if (dot2)pixels.setPixelColor(Digit3 - 2, pixels.Color(dr2, dg2, db2));
  else pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
}


void Set1DotColor()
{
  int index = 4; int index2 = 0;
  char dotnr = receivedChars[2];
  while (receivedChars[index] != ';') {
    val1[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val2[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val3[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val4[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val5[index2] = receivedChars[index];
    index++;
    index2++;
  }
  index2 = 0; index++;
  while (receivedChars[index] != '\0') {
    val6[index2] = receivedChars[index];
    index++;
    index2++;
  }

  if (dotnr == '1')
  {
    dr1 = atoi(val1);
    dg1 = atoi(val2);
    db1 = atoi(val3);
  }
  if (dotnr == '2')
  {
    dr2 = atoi(val1);
    dg2 = atoi(val2);
    db2 = atoi(val3);
  }
  //Serial.printf("%d%d%d%d%d%d", dr1, dg1, db1, dr2, dg2, db2);
  Serial.printf("Setting Dot Color of %c", dotnr);
  DrawDots();

  if (type == 'c') DrawTime();
  else if (type == 't') DrawTimer();

  type = old_type;
  pixels.show();
}


void Set1Color()
{
  int mydigit = receivedChars[2] - '0';
  //f;1;2
  int index = 4; int index2 = 0;
  while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
    val1[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val1[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
    val2[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val2[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != '\0' && receivedChars[index] != ';') {
    val3[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val3[index2] = '\0';
  index2 = 0; index++;

  int t_r = atoi(val1);
  int t_g = atoi(val2);
  int t_b = atoi(val3);
  Serial.printf("r:%d g:%d b:%d\n", t_r, t_g, t_b);

  if (mydigit == 1 || mydigit == 0) {
    r1 = t_r;
    g1 = t_g;
    b1 = t_b;
  };
  if (mydigit == 2 || mydigit == 0) {
    r2 = t_r;
    g2 = t_g;
    b2 = t_b;
  };
  if (mydigit == 3 || mydigit == 0) {
    r3 = t_r;
    g3 = t_g;
    b3 = t_b;
  };
  if (mydigit == 4 || mydigit == 0) {
    r4 = t_r;
    g4 = t_g;
    b4 = t_b;
  };
  type = old_type;
  if (type == 'c') DrawTime();
  else if (type == 't') DrawTimer();
  DrawDots();
  pixels.show();

}

void SetColors()
{
  int index = 2; int index2 = 0;
  while (receivedChars[index] != ';') {
    val1[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val1[index2] = '\0';

  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val2[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val2[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val3[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val3[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val4[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val4[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val5[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val5[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val6[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val6[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val7[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val7[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val8[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val8[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val9[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val9[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val10[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val10[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != ';') {
    val11[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val11[index2] = '\0';
  index2 = 0; index++;
  while (receivedChars[index] != '\0') {
    val12[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val12[index2] = '\0';
  index2 = 0; index++;
  r1 = atoi(val1);
  g1 = atoi(val2);
  b1 = atoi(val3);
  r2 = atoi(val4);
  g2 = atoi(val5);
  b2 = atoi(val6);
  r3 = atoi(val7);
  g3 = atoi(val8);
  b3 = atoi(val9);
  r4 = atoi(val10);
  g4 = atoi(val11);
  b4 = atoi(val12);

  Serial.printf("Setting Colors: r:%s g:%s b:%s r:%s g:%s b:%s r:%s g:%s b:%s r:%s g:%s b:%s  \n", val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12);
  //DrawTime();
  type = old_type;
  if (type == 'c') DrawTime();
  else if (type == 't') DrawTimer();
  DrawDots();
  pixels.show();
}

void SetBrightness()
{
  int index = 2; int index2 = 0;
  while (receivedChars[index] != '\0') {
    val1[index2] = receivedChars[index];
    index++;
    index2++;
  }
  val1[index2] = '\0';
  int brightness = atoi(val1);
  Serial.printf("Setting brightness to %d%%\n", brightness);
  pixels.setBrightness(brightness);


  type = old_type;
  if (mymode2 != '*')DrawDots();
  pixels.show();


  if (type == 'c' && mymode2 != '*') DrawTime();
  else if (type == 't' && mymode2 != '*') DrawTimer();
}


void ModeClock()
{

  if ((millis() - timemillis) >= 60000)
  {
    timemillis = millis();
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

void ModeTimerDyn()
{
  if (T_hours <= 0 && T_mins <= 0 && T_secs <= 0)type = '1';
  else
  {
    if ((millis() - timermillis) >= 1000)
    {
      timermillis = millis();
      T_secs--;
      if (T_secs <= 0)
      {
        if(T_mins > 0 || T_hours > 0)T_secs = 59;
        T_mins--;
        if (T_mins <= 0)
        {
          if(T_hours > 0)T_mins = 59;
          T_hours--;
        }
      }
      DrawTimer();
    }
  }


}



void ModeWeather()
{
  //Serial.println(millis());
  //Serial.println(weathermillis);
  //Serial.println(weathersecs * 1000);
  if (mymode2 == '*')
  {
    drawdigit(Digit1, ar, ag, ab, pt1);
    drawdigit(Digit2, ar, ag, ab, pt2);
  }
  if ( ((millis() - weathermillis) / 1000) > weathersecs )
  {

    Serial.println("Setting mode back to Clock");
    type = 'c';
    if (mymode2 != '*')DrawTime();
    if (mymode2 != '*')DrawDots();
    pixels.show();
  }
}

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
      shiftAlarmLeds(51);
      drawdigit(Digit1, ar, ag, ab, 0);
      drawdigit(Digit2, ar, ag, ab, 0);

      pixels.setPixelColor(Digit3 - 1, pixels.Color(ar, ag, ab));
      pixels.setPixelColor(Digit3 - 2, pixels.Color(ar, ag, ab));

      drawdigit(Digit3, ar, ag, ab, 0);
      drawdigit(Digit4, ar, ag, ab, 0);
      pixels.show();
    }
  }

}

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
      shiftAlarmLeds(51);

      drawdigit(Digit1, ar, ag, ab, hours / 10); //Draw the first digit of the hour
      drawdigit(Digit2, ar, ag, ab, hours - ((hours / 10) * 10)); //Draw the second digit of the hour

      drawdigit(Digit3, ar, ag, ab, mins / 10); //Draw the first digit of the minute
      drawdigit(Digit4, ar, ag, ab, mins - ((mins / 10) * 10)); //Draw the second digit of the minute
      pixels.show();
    }
  }
}


void ModeFade()
{
  if ((millis() - timermillis) >= fadespeed)
  {
    Serial.printf("fade! d=%d r=%d g=%d b=%d\n", ad, ar, ag, ab);
    timermillis = millis();
    alarmstate = 1;
    shiftAlarmLeds(fadeamount);

    if (type != 'w')
    {
      drawdigit(Digit1, ar, ag, ab, hours / 10); //Draw the first digit of the hour
      drawdigit(Digit2, ar, ag, ab, hours - ((hours / 10) * 10)); //Draw the second digit of the hour

      drawdigit(Digit3, ar, ag, ab, mins / 10); //Draw the first digit of the minute
      drawdigit(Digit4, ar, ag, ab, mins - ((mins / 10) * 10)); //Draw the second digit of the minute

      if (dot1)pixels.setPixelColor(Digit3 - 1, pixels.Color(ar, ag, ab));
      else pixels.setPixelColor(Digit3 - 1, pixels.Color(0, 0, 0));
      if (dot2)pixels.setPixelColor(Digit3 - 2, pixels.Color(ar, ag, ab));
      else pixels.setPixelColor(Digit3 - 2, pixels.Color(0, 0, 0));
    }
    else

      pixels.show();
  }
}



void DrawTimer()
{
  Serial.printf("Updating Timer: %d:%d:%d\n", T_hours, T_mins, T_secs);

  if (T_hours > 0)
  {
    drawdigit(Digit1, r1, g1, b1, T_hours / 10);
    drawdigit(Digit2, r2, g2, b2, T_hours - ((T_hours / 10) * 10));

    drawdigit(Digit3, r3, g3, b3, T_mins / 10);
    drawdigit(Digit4, r4, g4, b4, T_mins - ((T_mins / 10) * 10));
  }
  else
  {
    drawdigit(Digit1, r1, g1, b1, T_mins / 10);
    drawdigit(Digit2, r2, g2, b2, T_mins - ((T_mins / 10) * 10));

    drawdigit(Digit3, r3, g3, b3, T_secs / 10);
    drawdigit(Digit4, r4, g4, b4, T_secs - ((T_secs / 10) * 10));
  }
}


void CustomValues()
{
  d1 = receivedChars[2] - '0';
  d2 = receivedChars[4] - '0';
  d3 = receivedChars[6] - '0';
  d4 = receivedChars[8] - '0';
  Serial.print("Setting custom values!");
  if (d1 < 10) drawdigit(Digit1, r1, g1, b1, d1); //Draw the first digit of the hour
  else disabledigit(1);
  if (d2 < 10) drawdigit(Digit2, r2, g2, b2, d2); //Draw the second digit of the hour
  else disabledigit(2);

  if (d3 < 10) drawdigit(Digit3, r3, g3, b3, d3); //Draw the first digit of the minute
  else disabledigit(3);
  if (d4 < 10) drawdigit(Digit4, r4, g4, b4, d4); //Draw the second digit of the minute
  else disabledigit(4);
  pixels.show();
  type = 'z';
}

void DrawTime()
{
  Serial.printf("Updating Time: %d:%d\n", hours, mins);
  drawdigit(Digit1, r1, g1, b1, hours / 10); //Draw the first digit of the hour
  drawdigit(Digit2, r2, g2, b2, hours - ((hours / 10) * 10)); //Draw the second digit of the hour

  drawdigit(Digit3, r3, g3, b3, mins / 10); //Draw the first digit of the minute
  drawdigit(Digit4, r4, g4, b4, mins - ((mins / 10) * 10)); //Draw the second digit of the minute
}

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
  DrawTime();
  type = old_type;
}

void disabledigit(int mydigit)
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

void drawdigit(int offset, int r, int g, int b, int n)
{

  if (n == 2 || n == 3 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9 ) //MIDDLE
  {
    pixels.setPixelColor(0 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(1 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(0 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 1 || n == 2 || n == 3 || n == 4 || n == 7 || n == 8 || n == 9) //TOP RIGHT
  {
    pixels.setPixelColor(2 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(3 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(2 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //TOP
  {
    pixels.setPixelColor(4 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(5 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(4 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9 ) //TOP LEFT
  {
    pixels.setPixelColor(6 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(7 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(6 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(7 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 2 || n == 6 || n == 8) //BOTTOM LEFT
  {
    pixels.setPixelColor(8 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(9 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(8 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(9 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 8 || n == 9) //BOTTOM
  {
    pixels.setPixelColor(10 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(11 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(10 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(11 + offset, pixels.Color(0, 0, 0));
  }
  if (n == 0 || n == 1 || n == 3 || n == 4 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //BOTTOM RIGHT
  {
    pixels.setPixelColor(12 + offset, pixels.Color(r, g, b));
    pixels.setPixelColor(13 + offset, pixels.Color(r, g, b));
  }
  else
  {
    pixels.setPixelColor(12 + offset, pixels.Color(0, 0, 0));
    pixels.setPixelColor(13 + offset, pixels.Color(0, 0, 0));
  }
  pixels.show();
}


void shiftAlarmLeds(int amount)
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
    ab = ab - amount; if ( ab <= 0)ad++; if (ab < 0)ab = 0;
  }
  if (ad == 7)
  {
    ab = ab + amount; ag = ag + amount; if ( ab >= 255)ad = 0; if (ab > 255) {
      ab = 255;
      ag = 255;
    }
  }


}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_auth == 1)
    {
      if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
        Serial.println("connected");
        client.subscribe(mqtt_topic);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
    else
    {
      if (client.connect("ESP8266Client")) {
        Serial.println("connected");
        client.subscribe(mqtt_topic);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
}

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

//----------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
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
}
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------

