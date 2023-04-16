# Pi
Ce repertoire contient le code du raspberry pi pour le projet AGRUM.

Le projet étudiant AGRUM de l'Université de Sherbrooke est effectué dans le cadre du Projet majeur de Conception en énergie électrique. Ce projet s'étend sur plus d'un an et demi (Mai 2022 à Décembre 2023). AGRUM se veut une preuve de concept selon laquelle la production d'électricité et d'aliment est possible à même les villes, le tout occupant la même surface d'une toiture urbaine. La finalité du projet est un prototype d'agrivoltaïsme installer sur le toit de l'Université de Sherbrooke - Campus de Longueuil pendant une partie de l'été et de l'automne 2023

Ce répertoire contient le code du Raspberry Pi pico W (nommé Pi pour la suite) servant à controler l'ensemble du système du prototype, notamment la rotation des panneaux solaire, l'irrigation des cultures végétales, la prise de mesure de puissance électrique et de mesures environnementales ainsi que l'envoie de l'état du système sur une interface web. 

Pour toute question, communiquer à AGRUM@USherbrooke.ca ou Alexis.Gendron@USherbrooke.ca

## Table des matières

- [Environment Setup](#environment-setup)
- [Project architecture](#project-architecture)
- [Version](#versionknown-issues)
- [Known Issues](#versionknown-issues)

Voici les étapes pour set-up votre environnement, build le code et programmer le Pi. Je me suis beaucoup inspiré de [ce site web](https://www.hackster.io/lawrence-wiznet-io/how-to-setup-raspberry-pi-pico-c-c-sdk-in-window10-f2b816)

---

## Environment Setup

Vous aurez besoin d'environ 20 Gb d'espace sur votre disque.

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

### 4. Github

Si vous etes à l'aise avec les lignes de commande git : https://git-scm.com/download/win

Sinon, utilisez Github Desktop : https://central.github.com/deployments/desktop/desktop/latest/win32

### 5. Visual Studio Code

Finalement, pour ecrire du code, je recommande VS Code pour toute les extensions qui facilite la vie.

Direct link : https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user

Set `Default Destination folder path` et cocher `Add to PATH`

Si vous etes rendu ici, écrivez moi et je vous aiderai à finir le setup, ou suiver le guide dans l'hyperlien au début de cette section.

---

## Project architecture

Voici un apercu de l'architecture des dossiers et de ce qu'ils contiennent

### app
L'application contient les fonctions haut-niveau liées au système global. Elle contient : 
- Irrigation : Gère les capteurs environnementaux, l'irrigation des plantes et la récupération d'eau.
- Controle des PV (pv_management) : Gère la position des panneaux et la calibratin de ceux-ci.
- Gestion de l'énergie (energy_management) : S'occupe des décisions d'injection de puissance et gère la charge/décharge des batteries.

L'application se base sur l'interface **HAL** pour intéragir avec le matériel.

### hal
Fait la liaison entre l'application (**app**) et le matériel (**driver**). Contient des modules de niveau moyen (interface, motor, pump, sensors, timing, valve, weather) et utilise des fonction de l'interface **driver**.

### driver
Offre une interface simple pour interagir directement avec le matériel et des protocole de communication. Contient de nombreux wrapper (i2c, rtc, watchdog, MQTT, etc) afin de ne pas dépendre directement du microcontrolleur et des librairies utilisées.

### lib
Contient des librairies 3rd parties nécessaire au fonctionnement actuel du code, comme le rasberry pi pico C/C++ SDK qui contient à son tour d'autre librairies.

---

## Version
La version actuelle est v0.1.

- v0.0 : Premier test de la v0 sur l'Oasis avec machine à état d'irrigation et reconnexion au Thingsboard lors d'une déconnexion.
- v0.1 : Ajout d'envoie de l'état du système à ThingsBoard et des timestamp dans l'envoie de la télémetrie.

### v0.x
Les versions commencant par **v0** sont celles développées avant le mois de mai 2023. Elles sont surtout des tests et de la validation avant la **v1.0**

### v1.x
Les versions commencant par **v1** débutent au début du mois de mai, quand les étudiants partent en stage. Les versions supérieurs à **v1.0** sont des fix ou amélioration de la version **v1.0**.

Cette version, qui sera en fonction la majorité de l'été 2023, sert à mesurer humidité et température dans l'environnement de culture des plantes et envoyer les données sur l'interface web.

### v2.x
Les versions commencant par **v2** débutent dès le retour du stage. Les versions supérieurs à **v2.0** sont des fix ou amélioration de la version **v2.0**.

Cette version, qui sera en fonction la majorité de l'automne 2023, contient l'ensemble des fonctionnalitées du prototype.

---

## Known Issues
- V0.0 : 1. Le Pi se déconnecte et reboot après un peu plus de 5h de fonctionnement.
