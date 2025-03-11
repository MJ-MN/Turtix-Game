# Turtix Game
Turtix is a 2D game developed by C++ and the SFML library.
![screenshot](./Turtix.png?raw=true)
## Tutorial
### Common requirements
Install the following packages from your package repository (if not installed):
- `build-essential` (on Debian-based distribution):

  ```sh
  $ sudo apt update && sudo apt install build-essential
  ```
- `g++ compiler`:

  Debian-based distribution:
  ```sh
  sudo apt-get install g++
  ```
  RPM-based distribution:
  ```sh
  sudo dnf install gcc-c++
  ```
- `7-Zip`:

  Debian-based distribution:
  ```sh
  sudo apt install p7zip-full
  ```
  RPM-based distribution:
  ```sh
  sudo dnf install p7zip-full
  ```
- `git`:

  Debian-based distribution:
  ```sh
  $ sudo apt install git-all
  ```
  RPM-based distribution:
  ```sh
  $ sudo dnf install git-all
  ```

- `SFML` library:

  Debian-based distribution:
  ```sh
  sudo apt install libsfml-dev
  ```
  RPM-based distribution:
  ```sh
  sudo dnf install libsfml-dev
  ```
- `Makefile`:

  Debian-based distribution:
  ```sh
  sudo apt install make
  ```
  RPM-based distribution:
  ```sh
  sudo dnf install make
  ```
### Installation
1. Open a new terminal console
2. Create a new destination folder at a place of your choice e.g. at `~/git`: `mkdir $HOME/git`
3. Change to this directory: `cd ~/git`
4. Fetch the project source files by running `git clone https://github.com/MJ-MN/Turtix-Game.git`
### Building
1. Change into the project source directory: `cd Turtix`
2. Run `make clean`
3. Run `make install` to install. This might require sudo permissions.
4. Run `make`to build the execution file.
5. Run `bin/Turtix.out` to play game.
