# LAN Game
========================

About
-----

An online network game that I created in my final year of university. The game is not optimised, crashes are possible and uses a very naive client-server implementation. 
The engine uses some third-party libraries to facilitate development.

You may have seen some similarities with other engines, and there are. I took inspiration for ideas from other engines such as Serious Engine and Id Tech 3, and yes, I didn't copy anything from there.

You might also have seen my nickname as Dark-Head - it was my old nickname when I was developing.

This project will most likely be an introduction to how online games and game engines work. I hope it will be useful for someone who wants to get acquainted with how games work.
As this is an alpha version of the game, I plan to improve both the engine and the game in the future.

Projects
--------

In total, we have 6 MS Visual Studio projects, only 3 of which are used for the game. Splitting the game into separate projects will give us the advantage of modularity, as we plan to support game modifications.

* `Editor` In this version, it is most likely a model converter. In the next versions there will be support for the level editor.
* `Engine` Game engine. It is responsible for graphics, networking, level geometry, and other components for the game's functioning.
* `Game` Game module, responsible for the execution of game logic, the game server session and the client. Game entities are also described here.
* `MakeResource` Used to generate textures, models and fonts.
* `Power` Game executable file.
* `Test` It is not used in the game and game engine at all. It is mainly used to test new features.

Building
--------

The game runs on the Windows operating system, so you need to install Microsoft Visual Studio 2022 or higher to compile the code, but there is a nuance...

As I said, the engine uses third-party libraries, but due to other licences, they were not included in the project except for the `Bullet Engine`, which is included in the `Engine` project itself, and GLM (OpenGL Mathematics).

To successfully compile and run the game, you need the following libraries:

* `SDL2` Input, window handling, openGL contex.
* `Libsodium` For encrypt/decrypt messages between client and server.

After you have installed the libraries, you need to place the header files and static libraries, as well as the dynamic libraries. There are two folders in the `Sources` folder: `Libs` and `Includes`. 
Place the header files with folders in `Includes`, and static libraries in `Libs`. IMPORTANT: The libsodium library is supplied in both a release and a debug version. 
Therefore, for the debug version, rename the name of the library with the letter “D” (Dlisodium.lib) and the dynamic library (Dlibsodium.dll). 
Add the dynamic libraries to the root folder of the game.

After you have arranged the header files and libraries in the folders, you should get something like this:

```
Source
├──Includes
|  ├──SDL2
|  |  └──headers
|  └──sodium
|     └──headers
└──Libs  
   └──.lib files
```

If everything is fine, you can compile the code and try to play :). Please note that the projects are ALL configured for the x86 platform.

And one more thing - do not use spaces in the names of directories and solution paths

Common problems
---------------

As I said, the game is not perfect, it can crash. Also, if you can't connect to the server on your local network, configure your firewall (Inbound rules) to allow UDP port 5910 for the game.

License
-------

This  game is licensed under the MIT license (see LICENSE file).

Below are the side libraries that are used:

* `SDL2` by Sam Lantinga and SDL Community (zlib license)
* `Libsodium` see credits in official documentation: https://doc.libsodium.org/#thanks (ISC license)
* `Bullet Engine` by Erwin Coumans (zlib license)
* `GLM` by Christophe Riccio (MIT license)
