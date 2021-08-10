[![EN](https://user-images.githubusercontent.com/9499881/33184537-7be87e86-d096-11e7-89bb-f3286f752bc6.png)](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/blob/master/README.md)
[![RU](https://user-images.githubusercontent.com/9499881/27683795-5b0fbac6-5cd8-11e7-929c-057833e01fb1.png)](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/blob/master/README.RU.md) 
# OpenVR Additional Arduino Buttons
OpenVR driver that allows to duplicate the controller buttons in VR, using an Arduino based device.

## Driver setup
1. Flash the firmware to Arduino.
2. Unpack "OpenVR.Arduino.Btns.SteamVR.zip" в папку `...\Steam\steamapps\common\SteamVR\drivers`.
3. Change the COM port number in the configuration file `\SteamVR\drivers\arduinobuttons\resources\settings\default.vrsettings`.
4. (Optional) Turn on `DebugMode`, change its value to `true`, to check the work of the driver without Arduino.

Debugging mode allows to check the driver, without Arduino, using the keyboard. The arrows change the position of the VR controller stick, the Numpad arrows (8, 2, 4, 6) change the position of the touchpad, and the "Numpad 7" and "Numpad 9" press the "A" and "B" buttons.

## SteamVR Setup
Can check the operation of the driver by going to the SteamVR settings, enabling advanced settings, selecting "Controllers" and clicking the "Test controller" button. There you need to select `#/USER/TREADMILL` and click on it.



In order for the games to understand what to do by pressing the keys, button bindings are necessary. To do this, go to "SteamVR Settings" -> "Advanced Settings" -> Show -> "Controllers" -> "Show the old layout" or you can [follow the link](http://localhost:27062/dashboard/controllerbinding.html) and configure it in the browser. Then we select the desired running game, add actions to the buttons, actions to the sticks or touchpad, then save the key binding and play. Can also set default actions for all games by changing the settings for SteamVR.

## Download
>Version for x64.<br>
**[Download](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/releases)**

## Feedback
`r57zone[at]gmail.com`