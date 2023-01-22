# Pi
Ce repertoire contient le code du raspberry pi pour le projet AGRUM. Voici les étapes pour set-up votre environnement, build le code et programmer le rasperry pi pico W. Je me suis beaucoup inspiré de [ce site web](https://www.hackster.io/lawrence-wiznet-io/how-to-setup-raspberry-pi-pico-c-c-sdk-in-window10-f2b816)

## Set-up environnement Windows

### 1. ARM GCC Compiler

Ce compilateur est utilisé pour build le code afin de programmer le Pi.

Lien direct : https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-mingw-w64-i686-arm-none-eabi.exe?rev=4eb1b321a6f44ca78be67eb9cef8b37a&hash=AD35FDA3E92F4D716B9C4FCF21A9B27F

Arm download web page : https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

Option d'installation : Set `Default Destination folder path` et cocher les options `Add path to environment variable` et `Add registry information`

### 2. CMake

CMake est utilisé pour compiler le code C/C++.

Direct link : https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-windows-x86_64.msi

Download page : https://cmake.org/download/

Cocher l'option `Add CMake to the system PATH for all the users` et set `Default Destination folder path`

### 3. Build tools for Visual Studio 2022

Necessaire afin d'utiliser CMake.

Direct link : https://aka.ms/vs/17/release/vs_BuildTools.exe

Cocher l'option `Desktop development with C++`

### 3. Github

Si vous etes à l'aise avec les lignes de commande git : https://git-scm.com/download/win

Sinon, utilisez Github Desktop : https://central.github.com/deployments/desktop/desktop/latest/win32

### 4. Visual Studio Code

Finalement, pour ecrire du code, je recommande VS Code pour toute les extensions qui facilite la vie.

Direct link : https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user

Set `Default Destination folder path` et cocher `Add to PATH`

Si vous etes rendu ici, poker moi pour que je finisse ce ReadMe!



