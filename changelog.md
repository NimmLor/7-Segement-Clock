# Known issues:

- Entering a time into the timer field causes the time to be displayed in HH:MM format
- Leaving the clock on the maximum fade speed for a longer duration might cause a kernel panic on certain esp8266 boards, possible solution: adding a 100uF Capacitor between VCC and GND



## Changelog:

## Major Update: 24.09.2018

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
