# hackernews-badge

![IMG_2677 (Small)](https://user-images.githubusercontent.com/2049284/224446330-af63d740-84ce-49f4-be5b-74564d4e1c9e.png)

With just one off-the-shelf part and some programming, you can have a standalone gadget that displays the latest HackerNews story, and notifies you of changes to your karma!

https://user-images.githubusercontent.com/2049284/224445784-05fca88a-b30a-4433-946f-9851b481c9ec.MOV

This project is perfect for electronics beginners with some experience in software; this walk-through was written with the assumption that you are starting from scratch.

Features: 
- Displays the newest or toppest HackerNews story
- Scannable QR code that takes you to the comment section of the story
- Notifies you when your karma changes

![IMG_2676 (Small)](https://user-images.githubusercontent.com/2049284/224446385-f626e094-f939-4d77-b4bd-ac511933d8d0.png)

Pre-requisites:
- ESP32 Development Board with LCD Screen ( I tested these steps using https://www.amazon.com/HiLetgo-Display-Bluetooth-Internet-Development/dp/B07X1W16QS )

Installation:
- Install the Arduino 1.8.19 IDE: https://www.arduino.cc/en/software#legacy-ide-18x
- Start the Arduino IDE, open File -> Preferences, and add this URL to enable the ESP32 toolchain: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

![Capture](https://user-images.githubusercontent.com/2049284/224449066-8c2c4e1d-d168-4ded-bb41-326c49a603e2.PNG)

- Open Tools -> Board Manager and install the ESP32 toolchain

![Capture](https://user-images.githubusercontent.com/2049284/224449369-8af38650-f677-4bfb-9611-d913aca25824.PNG)

- Open Tools -> Boards and choose ESP32 Dev Module

![Untitled](https://user-images.githubusercontent.com/2049284/224452102-f1253e1a-b841-4ffa-a499-ce4b85d4e3d2.png)

- Connect the ESP32 board to your computer, and choose the Port it is using from Tools -> Ports (if in doubt, note the Ports, close the Tools Menu, then unplug the board, and check the Ports again to see which one disappeared)

![Untitled](https://user-images.githubusercontent.com/2049284/224452216-5a6c96f7-ce43-4d8a-ba2b-3be3c7784299.png)

- Open Tools -> Manage Libraries and install ArduinoJSON, TFT_eSPI, QRcode_eSPI (and dependency), and wordwrap

![Untitled](https://user-images.githubusercontent.com/2049284/224450706-7328fa40-18a3-4a7d-9a73-5cdc8b3038c9.png)

- Open File -> Preferences and note your Sketchbook location. Using your favorite text editor, open the TFT_eSPI setup file found at `<sketchbook_location>/libraries/TFT_eSPI/User_Setup_Select.h`, add `//` to the beginning of the line (comment out) `#include <User_Setup.h>` and remove the `//` (uncomment) from the line `#include <User_Setups/Setup25_TTGO_T_Display.h>`, and save the file

![Untitled](https://user-images.githubusercontent.com/2049284/224458325-f04c1f52-9802-4572-9a28-6077206705de.png)

- Copy the hackernews-badge.ino Arduino Sketch into the IDE and edit the Options at the top of the source code to match your Access Point settings

![Capture](https://user-images.githubusercontent.com/2049284/224452356-b659c364-9a02-4e1e-9ec9-2b8005c2ccd1.PNG)

- Click the Upload button, and enjoy! To check on the gadget after the code has been uploaded, open Tools -> Serial Monitor

![Capture](https://user-images.githubusercontent.com/2049284/224451816-bc6e901a-72fd-4745-b839-da2a272be285.PNG)

Potential Improvements:
- Connect a vibrating motor / speaker to alert on notifications
- Enumerate device as USB Keyboard and open links on attached computer using button
- Send a link to the article to your phone over Bluetooth using another button
