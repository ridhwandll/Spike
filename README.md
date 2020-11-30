
# LightEngine
![](https://github.com/FahimFuad/LightEngine/workflows/build/badge.svg)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/FahimFuad/LightEngine/blob/main/LICENSE)

![LightEngine](Resources/Branding/LightEngine.png)<br/>

Welcome to the **LightEngine**'s source code!<br/>
Its basically an application making engine written in **C++**<br/>
This can also be used an powerful **GameEngine**!<br/>

## :wrench: Project Setup :wrench:
### Windows
- Premake
    - Visual Studio 2019 is recommended on Windows<br/>
    - Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
    - Run the `GenProjects-windows.bat` file inside the scripts folder and Premake will generate project files for Visual Studio 2019.
    - If you want to generate for other IDE or versions of Visual Studio then in the project directory run your favourite premake [action](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
    - Once premake generate the project files then you can hit the run button and make the engine up and running!<br/>
- CMake
    - Again, start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git`.
    - Create a directory in which to build your project, e.g. 'build'
    - Run `cmake .. -G "Visual Studio 16 2019"` in the build directory to generate the solution.
    - Once the project files are generated, you should be able to import the project and run it.
### Mac & Linux
- Premake
    - Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
    - To genereate the project files for your IDE, select the actions of the premake and run them in the project directory. A list of actions of the premake can be found [here](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
    - The engine is now ready to light!
- CMake
    - Again, start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git`.
    - Create a directory in which to build your project, e.g. 'build'
    - Within your build directory, run `cmake ..` to generate makefiles or `cmake .. -G "CodeBlocks - Unix Makefiles"` to generate a Code::Blocks project.
    - To build the project, run `make` inside the build directory.
# Main features to come: 
- Fast 2D rendering (UI, particles, sprites, etc.)
- High-fidelity Physically-Based 3D rendering (this will be expanded later, 2D to come first)
- Support for Mac, Android and iOS
- Native rendering API support (DirectX, Vulkan, Metal)
- Fully featured viewer and editor applications
- Fully scripted interaction and behavior
- Integrated 3rd party 2D and 3D physics engine
- Procedural terrain and world generation
- Robust Artificial Intelligence(AI) system
- Audio system<br/>

## Contributing [![GitHub contributors](https://img.shields.io/github/contributors/FahimFuad/LightEngine.svg)](https://GitHub.com/FahimFuad/LightEngine/graphs/contributors/)
- For contributing, fork this repository first.
- Then made a feature branch in your fork and start adding stuff.
- When you're ready with your feature, you can send a Pull Request(PR).
- Then your code will be reviewed and if everything goes well, your commits will be mergerd!

This engine is inspired from **TheCherno's** [Hazel](https://github.com/TheCherno/Hazel) Engine.

[![Discord](https://img.shields.io/badge/LightEngine--red.svg?style=social&logo=discord)](https://discord.gg/65AvSfUeym)