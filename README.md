# homotica
This repository contains the sketch and the library required by "Homotica" Android app.

You can find the app at the following link: 

INSTRUCTIONS:

1 - download the repository, and unzip it

2 - move the folder named "homotica" to your Arduino libraries folder (usually under C:\Users\your_user_name\Documents\Arduino)

3 - open the Arduino sketch "homotica_sketch" contained in the namesake folder

4 - copile and upload it using an Arduino IDE

5 - you are done!

Why do I need to use a custom library in order to get this sketch to work? - the library can set timers (up to 15 at the same time by default) without using the delay() function, which would stop the Arduino loop; this way it can accept different commands at the same time and be reative to connection checks or other stuff.
