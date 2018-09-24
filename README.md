This is a fully 3D printed clock that was designed in Fusion 360. The project is based on [Ivan Miranda's Big Digital Clock](https://ivanmiranda.com/diy-big-digital-clock) and was completely re-designed.

## If you have any questions or issues feel free contact me 

#### *Mini version* of the clock coming soon :)


## Changelog:

## Major Software Update: 24.09.2018
**Make sure to update all your libraries (3 dashes → manage palette → update)**
- **LOTS OF BUG FIXES**
  - Clock does no longer light up every 6 hours even when turned of
  - Fixed a bug when clock mode was selected at 00:00 - 00:59 that it would made it show e.g. 24:15
  - Fixed random reboots
  - Fixed that alarm wouldn't trigger at the correct time
  - Fixed a bug that occured when the API-key of *openweathermap* wasn't set
  - Fixed error spamming in debug console of Node-red
  - Fixed that timer would skip 1 second every minute
  - Fixed bugs that would result in a crash of the clock
  - and much more...
- Removed unnecessary buttons in the UI
- Added a **settings tab** to the UI, the *settings* section in the ui got moved over there
- Clock is now **more precise**
- Timer is now capable of displaying HH:MM and MM:SS format, depending on the remaining time
- **SOURCE CODE OVERHAUL**
  - Functions are now **modular**
  - Added tons of **comments**
  - Made the code **readable**
  - **New** functions
  - Clock/Timer works now more precise (the clock shouldn't be off time more than 2 secs)
  - Functions utilize much less global variables making the code easier to modify
  - Weather algorithm got fixed
  - Fade works more stable now and shouldn't cause a crash anymore
  - Time is now set in **HH:MM:SS** before it was HH:MM
- Clock now request the time on startup and after connection loss
- Custom values now allow to **disable digits**
- Fixed fade-speed levels
- Node-red flows are now much more modular, this allows controlling additional clocks
- **Debug Serial** mode for the clock -> debugging can be disabled -> more stability
- Brightness slider is now logarithmic
- Fixed some typos
- Easy way to control more clock implemented
- Time offset should now be persistent
- Added **Settings** page
- Improved **Alarm** Creation
  - Fixed tons of bugs
  - Added alternative input formats
  - Added input check → the user gets informed if the values aren't correct
  - Adapted the form to the new dashboard plugin update
  - The alarm triggers now at the correct time :D
- and more...



## Hotfix: 14.07.2018

- Fixed a bug in the arduino code that would cause the shutdown of the clock if the weather gets displayed
- Due to an update of the dashboard library some code had to be changed for the timer

## Hotfix: 05.07.2018

- Updated **diffuser_white_28x**, fixed the tiny hole on the bottom






#### All files and photos can be found at: [Github](https://github.com/NimmLor/7-Segement-Clock)


### Tested devices
 - Raspberry Pi 3B
 - Raspberry Pi B+
 - Wemos D1 mini
 - Generic ESP8266 (Led Strip Pin must be changed)


## Features

- Responsive **webinterface**

![Webinterface](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/new/Screenshot_2018-09-24-22-28-06.png?raw=true)

- Create **alarms**

![Webinterface alarm](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/new/Screenshot_2018-09-24-22-32-30.png?raw=true)

- Show current temperature in your region

![Webinterface weather](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/new/Screenshot_2018-09-24-22-43-28.png?raw=true)

- Configure settings

![Webinterface settings](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/new/Screenshot_2018-09-24-22-32-51.png?raw=true)

- Display local time
- Set **individual colors** of each digit
- Custom **scoreboard** mode 
- Control brightness
- Save **custom colors**
- **Fade** colors



# Bill of materials
- 1x [Raspberry Pi](https://www.amazon.de/Raspberry-Pi-Model-ARM-Cortex-A53-Bluetooth/dp/B01CD5VC92/ref=sr_1_5?s=computers&ie=UTF8&qid=1529245480&sr=1-5&keywords=raspberry+pi) or any linux machine
- 1x [Wemos d1 mini](http://s.click.aliexpress.com/e/MzRFIIE)
- 1x [1m of WS2812 LED strip with 60 LED's](http://s.click.aliexpress.com/e/EiMfaA2)
- 1x [Power supply 5V, 4A](https://de.aliexpress.com/item/-/32729613841.html?aff_platform=aaf&cpt=1529246193632&sk=2v37IQv&aff_trace_key=4a8485e23f644e5ea714a9afe5dbc424-1529246193632-06076-2v37IQv&terminal_id=a501ccaf1cb041b3a86c8852e0b0a15e)
- 16x [M3 * 10-16mm](http://s.click.aliexpress.com/e/zRNRrrz)
- 16x [M3 Nuts](http://s.click.aliexpress.com/e/a2JeIEQ)
- 1x [Power jack adapter](http://s.click.aliexpress.com/e/Jyvj2vb)
- 2m [Wire](http://s.click.aliexpress.com/e/rzvNFUb)
- 4x [Dupont connectors](http://s.click.aliexpress.com/e/6UvBeYZ)
- ##### Estimated total price: 25€ not including the raspberry pi

## Tools

- Soldering iron
- 3D Printer
- Pliers
- Hot glue gun




# How to build it

## 1. 3D-Print


#### Print Settings

##### Attention: diffuser and dot_diffuser must be printed at 100% infill!

| Part   | frame  | dot_frame | back_cover | dot_back_cover | diffuser | dot_diffuser |
| :----- | ------ | --------- | ---------- | -------------- | -------- | ------------ |
| Speed  | 45mm/s | 45mm/s    | 45mm/s     | 45mm/s         | 25mm/s   | 25mm/s       |
| Infill | 15%    | 15%       | 15%        | 15%            | **100%** | **100%**     |
| Layer  | 0.2mm  | 0.2mm     | 0.2mm      | 0.2mm          | 0.2mm    | 0.2mm        |
| Color  | Black  | Black     | Black      | Black          | White    | White        |
| Amount | 4      | 1         | 4          | 1              | 28       | 2            |



## 2. Soldering of the LED-Strips

First the LED strips have to be cut into pieces of 2 LEDs each, then the ends have to be bent until the solder pads are exposed. It is recommended to tape the strips onto a soldering mat or a table and solder the wires afterwards. 

![Wiring](https://github.com/NimmLor/7-Segement-Clock/blob/master/WIRING1.jpg?raw=true)

![full wiring](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/full_wiring.jpg?raw=true)

Be careful when wiring the dots, only **one** LED was used for these pieces. 

## 3. Testing the LEDs

Upload the *clock_mqtt.ino* sketch onto the ESP8266, if the leds don't show *0:0:0:1* after one minute, then you made a mistake or have loose wires somewhere.

## 4. Assembling the frame

Push in all 30 diffusers, use the 16 M3 screws and nuts to assemble the frame. This step should be self explainatory.

## 5. Glue in the LEDs 

A hot glue gun needs to be used to glue on all LED strips. The start of the LED strip and the ESP8266 must be located on the left when looking at the clock from the front. Afterwards also glue on the microcontroller and the power jack once everything works the way it should. Finally attach the back covers.



# Setup

## 1. Raspberry Pi
- You could also install this on any linux machine

In case help with Raspberry Pi is needed, click [here](https://www.imore.com/how-get-started-using-raspberry-pi).

#### 1. Install Node-RED

`bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered)`

#### 2. Install npm - this might be already installed

`sudo apt-get install nodejs npm -y`

#### 3. Install dashboard, openweathermap and mysql

`cd $HOME/.node-red`
`npm install node-red-dashboard`

- if you get an error try: `npm install  --unsafe-perm node-red-dashboard`

`npm install node-red-node-openweathermap`
`npm install node-red-node-mysql`

#### 4. Install mosquitto

`sudo apt-get install mosquitto -y`
(optional) [Setup mosquitto authentication](http://www.steves-internet-guide.com/mqtt-username-password-example/)

#### 5. Install mysql

`sudo apt-get install mysql-server -y`

   - Just hit **OK** or **RETURN** during installation

#### 6. Setup a static ip for your raspberry pi

   - (recommended) Setup a static ip on your router
   - [or on your Raspberry Pi](http://www.circuitbasics.com/how-to-set-up-a-static-ip-on-the-raspberry-pi/)

#### 7. Create Database clock and account
You will have to create a database named **clock** to be able to save alarms. Just type into your shell:

`mysql -uroot`

`create database clock;`

`CREATE USER 'admin'@'localhost' IDENTIFIED BY 'raspberry';`

`GRANT ALL PRIVILEGES ON clock.* TO 'admin'@'localhost';`

`FLUSH PRIVILEGES;`

`quit`

## 2. Node-RED

#### 1. Start node-red
- `sudo node-red-start`

#### 2. Open node-red
- <http://yourRaspberryIP:1880>

#### 3. Import the flows
- Click on the 3 dashes in the top right corner → import → clipboard
- Enter the code snippet from **all_flows_v2.txt** and click **import**

#### 4. Head to the Thingiverse_Settings flow
#### 5. Edit the mqtt node
   - Set topic to **clock**
   - Edit **broker** and enter **localhost** in the IP field
     - If you had set up authentication before, the credentials must be entered in the **Security** tab 

#### 6. Edit the mysql node
   - Edit the mysql connection
     - IP: **127.0.0.1**
     - Username: **admin**
     - Password: **raspberry**
     - Database: **clock**

![mysql node settings](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/mysql.PNG?raw=true)

#### 7. Click on the latch of the node *CREATE TABLE*
#### 8. Double click on the node *TIME OFFSET*
   - Enter the offset of the time, Germany and Austria would be = 1

![Time offset](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/time_offset.PNG?raw=true)

#### 9.  Get local temperatures from *OpenWeatherMap*

- If temperature output is not needed, just delete the flow *Thingiverse_weather* and the weather section in the *Thingiverse_clock* flow.
- Head over to [OpenWeatherMap](https://home.openweathermap.org/users/sign_up) and create an account
- Click on API-Keys and copy the value

![API-Key](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/api_key.PNG?raw=true)

- Afterwards copy that key into the *openweathermap* node in the *Thingiverse_weather* flow.
- Either pick geo-coordinates or city to get the temperatures.

![owm node](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/weather.PNG?raw=true)

#### 10.Connect to the webinterface

- Hit **DEPLOY**
- Now you can connect to the UI via: http://yourRaspberryIP:1880/ui

#### 11. Customize the webinterface

- The webinterface is designed to fit the whole screen of the *Fire HD 8* Tablet, to fit your phone you have to change the dimensions of the elements. 
- You can change the position by dragging the elements in the dashboard list.

![Dashboard element list](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/mod.jpg?raw=true)

#### 12. Change theme of the webinterface
- You can either choose one of the two default themes or set your own colors.

![customize](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/colors.PNG?raw=true)

## 3. ESP8266

#### 1. Install Arduino IDE
   - https://www.arduino.cc/en/main/software

#### 2. Add ESP8266 boards to the Arduino IDE

      1. Click on preferences
      2. Enter under *Additional Board Manager URLs*:
      http://arduino.esp8266.com/stable/package_esp8266com_index.json
      3. Now head to tools → Board → Boards-Manager
      - Search for **esp8266** and install the package
      4. Select **WeMos D1 R2 & mini** from the boards list

#### 3. Install libraries

      1. Click on sketch → include libary → manage labraries
      2. Install *PubSubClient*
      3. Install *Adafruit Neopixel*


#### 4. Now open the file *clock_mqtt_v2.ino*
#### 5. Edit the code
      1. Set your WiFi SSID and WiFi password
      2. Set **mqtt_server** to your Raspberry Pi's IP-Address
      3. If you had set up authentication before, change **mqtt_auth** to 1 and enter your credentials below, otherwise set it to 0

![arduino code](https://github.com/NimmLor/7-Segement-Clock/blob/master/images/new/settings.png?raw=true)

#### 6. Choose the correct COM-Port of the ESP8266 under tools → Port
#### 7. Hit **Upload**

# Congratulations you made it, have fun! ☺



## 4. My setup 

The table i'm using is an **Amazon Fire HD 8** with an app called **One Page Web Browser**, this app can be found [here](https://play.google.com/store/apps/details?id=cz.mivazlin.onepagewebbrowser).



