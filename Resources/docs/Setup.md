# Cloning Spike
Clone the repository with `git clone --recursive https://github.com/FahimFuad/Spike.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.You can use Git clients like Gitkraken, Fork etc. It is upto you how you clone it, just be sure to do a *recursive* clone to fetch the submodules.

# Windows Setup

![Windows64 supported](https://img.shields.io/badge/Windows64-Supported-green.svg)
![Windows32 Notsupported](https://img.shields.io/badge/Windows32-NotSupported-red.svg)

- Visual Studio 2019 is highly recommended on Windows
- Run the `WindowsGenProjects.bat` file inside the scripts folder and CMake will generate a `Spike.sln` file inside the `build` folder on the root directory. Open that file and hit the run button.
- If you want to generate for other IDE or versions of Visual Studio then in the project directory run your favourite CMake action.

# Mac & Linux Setup

![MacOS not supported](https://img.shields.io/badge/MacOS-NotSupported-red.svg)
![Linux not supported](https://img.shields.io/badge/Linux-NotSupported-red.svg)

- Make a folder named `build` in the root directory
- Go to `build` directory
- Run CMake on the `build` directory. After CMake generates project files, you can hit the run button of your IDE.


# Building the Example Project

![Windows64 supported](https://img.shields.io/badge/Windows64-Supported-green.svg)
![MacOS not supported](https://img.shields.io/badge/MacOS-NotSupported-red.svg)
![Linux not supported](https://img.shields.io/badge/Linux-NotSupported-red.svg)

- Enter the `ExampleApp` folder in the root directory
- Run `GenExample.bat` and it will generate an `ExampleApp.sln` inside the `build` folder of `ExampleApp`
