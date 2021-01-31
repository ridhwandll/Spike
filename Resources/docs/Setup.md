## Spike engine has C# scripting, so you need mono on your system to make Spike work and run (We don't have a installer yet that will do this automatically for you)

Go to the mono [downloads page](https://www.mono-project.com/download/stable/) and grab the latest version for your OS. After you have installed mono on your system you can proceed to the next OS specific setups.

# Windows Setup

![Windows64 supported](https://img.shields.io/badge/Windows64-Supported-green.svg)
![Windows32 Notsupported](https://img.shields.io/badge/Windows32-NotSupported-red.svg)

- Visual Studio 2019 is highly recommended in Windows
- Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/Spike.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.

## Premake

- Run the `WindowsGenProjects.bat` file inside the scripts folder and Premake will generate project files for Visual Studio 2019.
- If you want to generate for other IDE or versions of Visual Studio then in the project directory run your favourite premake [action](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
- Once premake generate the project files then you can hit the run button and make the engine up and running!

## CMake

- You need to have CMake in your machine! Go [here](https://cmake.org/download/) to download the latest CMake and be sure to add CMake to your environment variable in your OS. We recommend downloading the installer and checking the option `Add to Path`
- With CMake, CLion is the recommended IDE but Visual Studio 2019 will also work fine!

- CLion
  - In CLion you can add this repository as a project. And hit the play button!

- Visual Studio 2019
  - Run the `WindowsGenProjectsCMake.bat` file inside the scripts folder to generate project files for Visual Studio 2019 inside the build folder in the root directory.Open the `Spike.sln` and hit the play button to get the engine running!

- Others

  - If you want to generate for other IDE or versions of Visual Studio then in the project directory then run the following code :arrow_down:

  ```cmake
  # makes a directory named build in the root directory
  mkdir build

  # Enters the build directory
  cd build

  # You CMake action
  cmake <Your-CMake-action>
  ```

  - Once CMake generates the project files then you can hit the run button and make the engine up and running!

# Mac & Linux Setup

![MacOS not supported](https://img.shields.io/badge/MacOS-NotSupported-red.svg)
![Linux not supported](https://img.shields.io/badge/Linux-NotSupported-red.svg)

## Premake

- Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/Spike.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
- To genereate the project files for your IDE, select the actions of the premake and run them in the project directory. A list of actions of the premake can be found [here](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
- The engine is now ready to run!

## CMake

- You need to have CMake in your machine! Go [here](https://cmake.org/download/) to download the latest CMake and be sure to add CMake to your environment variable in your OS. We recommend downloading the installer and checking the option `Add to Path`
- With CMake, CLion is the recommended IDE in Mac & Linux

- CLion
  - In CLion you can add this repository as a project. And hit the play button!

- Others
  - If you want to generate for other IDE then you should make a directory in the project root directory called **`build`** and generate CMake files with other CMake actions

  - Once CMake generates the project files then you can hit the run button and make the engine up and running!
