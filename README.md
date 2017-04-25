# Homotica
This repository contains the sketch and the library required by "Homotica" Android app.
You can find the app at the following link: https://play.google.com/store/apps/details?id=com.vetru.app.homotica

## INSTRUCTIONS:
  1 - download the repository, and unzip it.
  
  2 - move the folder named "homotica" to your Arduino libraries folder (usually under C:\Users\your_user_name\Documents\Arduino\libraries).
  
  3 - open the Arduino sketch "homotica_sketch" contained in the namesake folder.
  
  4 - copile and upload it using an Arduino IDE.
  
  5 - you are done!

Why do I need to use a custom library in order to get this sketch to work? - the library can set timers (up to 15 at the same time by default) without using the delay() function, which would stop the Arduino loop; this way it can accept different commands at the same time and be reactive to connection checks or other stuff.
