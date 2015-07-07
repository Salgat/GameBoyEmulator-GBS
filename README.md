GBS - Game Boy Emulator
=================

![](https://github.com/Salgat/GameBoyEmulator/blob/master/doc/legendofzelda.PNG) ![](https://github.com/Salgat/GameBoyEmulator/blob/master/doc/pokemon.PNG) ![](https://github.com/Salgat/GameBoyEmulator/blob/master/doc/supermarioland2.PNG) ![](https://github.com/Salgat/GameBoyEmulator/blob/master/doc/instructiontest.PNG)

GBS is a free, open-source, and cross-platform implementation of the Game Boy console written in C++14 with a single library dependency of SFML.

Building the project
------------------------------------------
If using CMAKE gui, set the source code to "C:/your_directory/GameBoyEmulator" and where to build binaries to "C:/your_directory/GameBoyEmulator/bin/Release". After configuring and generating the Makefile, change directory to the /bin/Release folder and type "make" to build (be sure to install both CMake-GUI and download Make.exe for Windows).

GBS requires SFML. Libraries are available http://www.sfml-dev.org/download/sfml/2.3/ for your specific compiler. Extract the SFML header folder to /include and the libraries (.a) to /lib. Finally, extract the compiled library files (.dll) to your /bin/Release folder (I personally used "GCC 4.9.2 MinGW (SEH) - 64-bit", downloaded from MinGW-w64).

For those who want a more in depth tutorial on how to build a project like this, I created a detailed tutorial for a similar project that will also work for this at the below link.

https://github.com/Salgat/BubbleGrow/wiki/Building-from-source

Running the emulator
------------------------------------------
To run after building: GameBoyEmulator.exe -game="Super Mario Land 2.gb"

Controls
------------------------------------------
Controls for the emulator are currently hard-coded.
* Z = B
* X = A
* Enter = Start
* Shift = Select
* 1 through 9 = Choose save state slot
* S = Save game state for current slot
* L = Load game state for current slot
* P = Save screenshot

Note
------------------------------------------
GBS is very much in the early stages of development and is not an accurate emulator. Additionally, most of the features of other emulators are lacking. That being said, it still can play most Game Boy games.


