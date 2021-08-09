[![EN](https://user-images.githubusercontent.com/9499881/33184537-7be87e86-d096-11e7-89bb-f3286f752bc6.png)](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/blob/master/README.md)
[![RU](https://user-images.githubusercontent.com/9499881/27683795-5b0fbac6-5cd8-11e7-929c-057833e01fb1.png)](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/blob/master/README.RU.md) 
# OpenVR Additional Arduino Buttons
OpenVR драйвер, позволяющий дублировать кнопки контроллера в VR, с помощью устройства на базе Arduino. 

## Настройка драйвера
1. Прошейте Arduino прошивкой.
2. Извлеките "OpenVR.Arduino.Btns.SteamVR.zip" в папку `...\Steam\steamapps\common\SteamVR\drivers`.
3. Измените номер COM-порта, в конфигурационном файле `\SteamVR\drivers\arduinobuttons\resources\settings\default.vrsettings`.
4. (Опционально) Включите `DebugMode` (режим отладки), изменить его значение на `true`, чтобы проверить работу драйвера без Arduino.

Режим отладки позволяет проверить драйвер, без Arduino, с помощью клавиатуры. Стрелки изменяют положение стика VR контроллера, Numpad стрелки (8, 2, 4, 6) изменяют положение тачпада, а "Numpad 7" и "Numpad 9" нажимают кнопки "A" и "B".

## Настройка SteamVR
Проверить работу драйвера можно зайдя в настройки SteamVR, включив расширенные настройки, выбрав "Контроллеры" и нажав кнопку "Проверить контроллера". Там нужно выбрать `#/USER/TREADMILL` и нажать по нему.



Для того, чтобы игры понимали, что делать по нажатию клавиш, необходимы привязки кнопок. Для этого идём в "Настройки SteamVR" -> "Расширенные настройки" -> Показать -> "Контроллеры" -> "Показать старую раскладку" или можно [перейти по ссылке](http://localhost:27062/dashboard/controllerbinding.html) и настраивать в браузере. Далаее выбираем нужную запущенную игру, добавляем действия на кнопки, действия на стики или тачпад, после чего сохраняем привязку клавиш и играем. Также можно задать действия по умолчанию для всех игр изменив настройки для SteamVR.

## Загрузка
>Версия для x64.<br>
**[Загрузить](https://github.com/r57zone/OpenVR-Additional-Arduino-Btns/releases)**

## Обратная связь
`r57zone[собака]gmail.com`