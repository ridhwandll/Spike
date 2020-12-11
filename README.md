
# LightEngine

![workflow](https://github.com/FahimFuad/LightEngine/workflows/build/badge.svg) [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/FahimFuad/LightEngine/blob/main/LICENSE)

![LightEngine](Resources/Branding/LightEngine.png)

## :zap: Welcome to the **LightEngine**'s source code

It is a **Game engine** written in **C++**

# :wrench: Project Setup

## Windows

![Windows supported](https://img.shields.io/badge/Windows64-Suppoeted-green.svg)

- Visual Studio 2019 is highly recommended in Windows
- Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
- Run the `GenProjects-windows.bat` file inside the scripts folder and Premake will generate project files for Visual Studio 2019.
- If you want to generate for other IDE or versions of Visual Studio then in the project directory run your favourite premake [action](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
- Once premake generate the project files then you can hit the run button and make the engine up and running!

## Mac & Linux

![MacOS not supported](https://img.shields.io/badge/MacOS-NotSupported-red.svg)
![Linux not supported](https://img.shields.io/badge/Linux-NotSupported-red.svg)

- Start by cloning the repository with `git clone --recursive https://github.com/FahimFuad/LightEngine.git` to clone along with the submodules. If you have already cloned and you want to initialize the submodules, type `git submodule init` and to update the submodules type `git submodule update` in your project directory.
- To genereate the project files for your IDE, select the actions of the premake and run them in the project directory. A list of actions of the premake can be found [here](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files)
- The engine is now ready to light!

# :zap: Screenshot

![LightEngine](Resources/Screenshots/Demo.png)

# :zap: In Action

![LE](https://s8.gifyu.com/images/Demoaa758ed2fadc5ba8.gif)

# :zap: Main features to come

- Fast 2D rendering (UI, particles, sprites, etc.)
- High-fidelity Physically-Based 3D rendering (this will be expanded later, 2D to come first)
- Support for Mac, Android and iOS
- Native rendering API support (DirectX, Vulkan, Metal)
- Fully featured viewer and editor applications
- Fully scripted interaction and behavior
- Integrated 3rd party 2D and 3D physics engine
- Procedural terrain and world generation
- Robust Artificial Intelligence(AI) system
- Audio system

# :zap: Implemented Features

- Console Logging
- Internal Key and Mouse Codes
- Input Polling
- LayerStack and Layers
- Event System and Events
- 2D Renderer
- Fast and easy to use API for 2D Rendering
- Entity Component System(ECS)
- Native C++ Scripting
- GUI Editor (Lightning-Editor)
- Deltatime
- Scene system
- Scene Serialization and Deserialization using YAML
- On Screen Gizmos to control the Transform of the object
  - Translation, rotation and scale
- Built in editor console

## Contributing [![GitHub contributors](https://img.shields.io/github/contributors/FahimFuad/LightEngine.svg)](https://GitHub.com/FahimFuad/LightEngine/graphs/contributors/)

- For contributing, fork this repository first.
- Then made a feature branch in your fork and start adding stuff.
- When you're ready with your feature, you can send a Pull Request(PR).
- Then your code will be reviewed and if everything goes well, your commits will be mergerd!

This engine is inspired from **TheCherno's** [Hazel](https://github.com/TheCherno/Hazel) Engine.

[![Discord](https://img.shields.io/badge/LightEngine--red.svg?style=social&logo=discord)](https://discord.gg/65AvSfUeym)
