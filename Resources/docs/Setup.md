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

# Mac & Linux Setup

![MacOS not supported](https://img.shields.io/badge/MacOS-NotSupported-red.svg)
![Linux not supported](https://img.shields.io/badge/Linux-NotSupported-red.svg)

## Premake

- Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/Spike.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
- To genereate the project files for your IDE, select the actions of the premake and run them in the project directory. A list of actions of the premake can be found [here](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
- The engine is now ready to run!

### **Note**: ***CMake*** support is currently not working due to some problem with the libraries. So we turned CMake CI off. The CMakeLists.txt files still exists in the codebase, incase we bring it back....