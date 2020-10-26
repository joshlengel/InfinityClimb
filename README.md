# InfinityClimb - Desktop

This is a 3D shooter game written in the C programming language. It uses OpenGL 3.3 and the cross platform windowing library GLFW and extention loading library GLAD.

## About

This game was inspired by many past attempts to code a larger-scale OpenGL project with object-oriented-languages such as C++ or Java to no avail. I made the realization that object-oriented languages were making me struggle to build easily usable and maintanable code, so I gave C a shot. This project does not have a concrete direction or end goal yet - I am just writing ideas down as I go and I try to implement whatever I can and try to make it look good.

## How to build

This project only supports Linux and Windows at the moment (Due to testing limitations). The binaries will be released as packages on the GitHub repository https://github.com/joshlengel/InfinityClimb. It better however, to build the project from source (which is not much work - so definitely recommended)

### Download binaries

Head over to https://github.com/joshlengel/InfinityClimb, click on the releases tab, download your favorite version and you are done!

### Build from source

1. Clone the repository. Go into the directory where you want InfinityClimb to reside and run the command `git clone "https://github.com/joshlengel/InfinityClimb"`
2. run `cd path/to/InfinityClimb/build`
3. install CMake if you have not already (Minimum version 3.12 is required) and run `cmake -DCMAKE_BUILD_TYPE=Release ..`
4. Locate the apropriate executable under bin/platform/Release and enjoy!

## Issues

If the game crashes or you have visual artifacts, either download the debug binary or build it from source with the flag `-DCMAKE_BUILD_TYPE=Debug` set. When you run the program, a directory logs should appear where you might find a more reliable output. If you are, however, unable to fix the problem using this aid create a GitHub issue (or fork if you find a solution) and I will hopefully be able to lend a hand.

## Gameplay

Now to the controls!

Move with the w, a, s and d keys and move your mouse to change the viewing direction. Jump with the space bar and descend with either of the shift keys (Only works with player in super mode). To switch perpectives, press the p key (This will toggle between first and third person cameras). To switch player modes, press the m key (This will toggle between super and normal mode).

Now have fun!