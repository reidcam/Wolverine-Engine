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

## Config

The config files should just be lists of rendering or game variables.

Some important rendering variables include:
x_resolution: the number of pixels across that your game window will be
y_resolution: the number of pixels tall that your game window will be
draw_debug: set to true to have the engine draw the bounds of your colliders and triggers

Some important game variables include:
game_title: the name of your game
initial_scene: the first scene that will be loaded when your game is opened

## Scenes

The scenes should be structured like so:
{
	"actors": [
		{
			"name": "Player",
			"components": {
				"1": {
					"type": "Rigidbody",
					"body_type": "static"
				},
				"2": {
					"type": "SpriteRenderer",
					"sprite": "grass_platform_small"
				},
    				"3": {
					"type": "PlayerControllerExample"
				}
			}
		},
		{
			"name": "Enemy",
			"components": {
				"1": {
					"type": "Rigidbody"
				},
				"2": {
					"type": "EnemyController",
					"speed": 2
				}
			}
		}
	]
}

As you can see, they are functionally just a list of all the actors in the scene.
Each actor has a name and a components list. Each component in the list must have a key which CAN be anything but is 1 or 2 in our example.
It also must have a type which tells the engine the kind of component that it is. Then you can list all of the variable overrides that you want to preform on the component.
(i.e. If the default speed of an enemy is 1 but you want this enemies speed to be 2, tell the engine that here.)

If you want to take a look at the engine itself to make changes or just to understand it better, the documentation is under docs/html/index.html.
The documentation was written to help people understand the engine to make changes to it better, but it could help you if your a developer and want to know how
to make things in it as well.

Be sure to format it correctly for JSON!

## Actor Templates

Actor templates function the same way as "prefabs" in Unity. They are basically an easy way to store the information for an actor before running the game so you can
easily make multiple copies of it at runtime.

A template for our enemy from the above scene would look like this:
{
	"name": "Enemy",
	"components": {
		"1": {
			"type": "Rigidbody"
		},
		"2": {
			"type": "EnemyController",
			"speed": 2
		}
	}
}

Make sure to name the file Enemy.template and put it in the actor_templates folder so the engine can find it.

## Components

This engine comes with a handfull of native components that you can use to create your game. Things like the SpriteRenderer and Rigidbody components are there so you don't
have to create them yourself!

But inevitably you will want to create your own custom components for your game. These custom components must be made in Lua.

Similarly to Unity, there are built in lifecycle functions for you to use if there is logic that you want to run every frame, when the component is created, or when the component is deleted.
These functions are: "OnUpdate", "OnStart", and "OnDestroy" respectively. There is also an "OnLateUpdate" function that runs after update every frame.

Here is an example of a basic player controller that allows the player to move an actor left and right. It shows how to format a component and some uses the input system along with the native Rigidbody component that is on the player.

