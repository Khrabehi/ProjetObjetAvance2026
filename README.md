# Project2024

## Setup

### QT
* Télécharger Qt depuis https://www.qt.io/download-qt-installer-oss?utm_referrer=https%3A%2F%2Fwww.qt.io%2Fdownload-open-source
* Installer Qt avec :
  * Installation personnalisé pour pouvoir choisir la suite.
  * Choisir une version de Qt et les binaires qui vont avec.

    ![choix de l'installation qt](./readme_ressources/choix_qt.png)

    * Je choisis MinGW par simplicité.
      Si vous avez Visual Studio (pas Code, le Visual Studio payant), vous pouvez prendre ces binaires.
    * Je choisis les Sources et les info' de Debug pour faciliter le deboguage.
    * J'ai choisi Qt Quick Timeline par erreur sur cette image. Il ne sert à rien pour nous.
  * Choisir les outils : cmake et MinGW à minima :

    ![choix des outils](./readme_ressources/tools.png)

    * J'ai choisi Qt Creator et son module de debug au cas où je souhaite utiliser cet IDE.
    * J'ai choisi les outils de debug pour permettre un debogage plus facile.
    * J'ai choisi MinGW pour disposer de la console pré-configurée.
    * J'ai choisi CMake pour ne pas avoir à le configurer par ailleurs et l'avoir dans la console pré-configurée.
    * J'ai choisi Ninja pour l'avoir à dispo' (mais je n'ai pas réussi à le faire fonctionner).

En cas d'erreur ou de modification à réaliser, Qt Maintenance Tool permet de reprendre l'installation :

![Qt Maintenance Tool](./readme_ressources/qt_maintenance_tool.png)

## Configuration
Une fois l'installation réalisée, une console pré-configurée doit être disponible :

![Qt console](./readme_ressources/console.png)

Elle permet de disposer de cmake, la toolchain de compilation (mingw dans mon cas) et Qt de disponible sans configuration.

Si vous ne voulez pas l'utiliser, il faut :
* Rajouter le répertoire bin du dossier d'installation de cmake au PATH (répertoire contenant `cmake.exe`).

  Ex : `set PATH=%PATH%;C:\Program Files\CMake\bin`

  L'alternative est d'appeler cmake est chemin absolu : `C:\Program Files\CMake\bin\cmake` dans mon cas à la place de `cmake` dans les commandes ci-dessous.

* Rajouter le répertoire bin du dossier d'installation de mingw au PATH (répertoire contenant `gcc.exe`).

  Ex : `set PATH=%PATH%;C:\tools\Qt\Tools\mingw1120_64\bin`

  L'alternative est de setter les variables `CMAKE_MAKE_PROGRAM` & co.

* Rajouter le répertoire bin du dossier d'installation de la librairie Qt (répertoire contenant `Qt6Core.dll`).

  Ex : `set PATH=%PATH%;C:\tools\Qt\6.6.1\mingw_64\bin`

  L'alternative est de setter la variable `CMAKE_PREFIX_PATH` à la configuration : `-DCMAKE_PREFIX_PATH="C:\tools\Qt\6.6.1\mingw_64\bin"`

Une fois la configuration terminée, pour compiler le projet, il est nécesaire de :
* Créer un répertoire de build : `mkdir build_debug`.

* Accèder à ce répertoire : `cd build_debug`.

* Configurer le projet : `cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..`.

  Dans le cas où vous n'avez pas rajouter le chemin vers l'installation MinGW et/ou Qt, c'est ici qu'il faut rajouter les paramètres de configuration.

  Ex pour Qt : `cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="C:\tools\Qt\6.6.1\mingw_64\bin -G "MinGW Makefiles" ..`.

* Builder le projet : `cmake --build .`.

En mode concis :
```console
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
cmake --build .
```

## Run

Une fois le projet buildé, deux exécutables sont disponibles :

* Les tests unitaires exécutable via `ctest` ou en appelant directement l'executable `.\bin\Debug\hello_test.exe`.

  Un test doit être exécuté avec succès.

* L'application graphique exécutable en appelant `.\bin\Debug\NomCool.exe`.

  Une fenêtre vide doit apparaitre et se fermer lors de l'appuie sur la croix.

En mode concis :
```console
ctest => one test successful
.\bin\Debug\NomCool.exe => UI shows
```
