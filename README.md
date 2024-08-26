# Wolverine-Engine

Wolverine Engine is a 2D scriptable game engine written in C++.
It is based heavily off of the Unity engine, and composition is the development method of the engine.

In order to create a game you must have a resources folder in the root of the engine. It should be structured like this:

.resources
├── actor_templates
│   └── Enemy.template
├── audio
│   └── GameMusic.mp3
├── component_types
│   └── GameScript.lua
├── fonts
│   └── BasicFont.ttf
├── images
│   └── GameImage.png
├── scenes
│    └── basic.scene
├── rendering.config
└── game.config

All of your resources (images, audio, fonts, components, actor templates, and scenes) need to be stored in the respective folders for the engine to recognize them.

The actor templates, scenes, and config files are all JSON, the custom file extension helps the engine decide which is which.
The config files should just be lists of rendering or game variables.

Some important rendering variables include:
x_resolution: the number of pixels across that your game window will be
y_resolution: the number of pixels tall that your game window will be
draw_debug: set to true to have the engine draw the bounds of your colliders and triggers

Some important game variables include:
game_title: the name of your game
initial_scene: the first scene that will be loaded when your game is opened

If you want to take a look at the engine itself to make changes or just to understand it better, the documentation is under docs/html/index.html.
The documentation was written to help people understand the engine to make changes to it better, but it could help you if your a developer and want to know how
to make things in it as well.


