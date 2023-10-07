# Camouflage Cubes openFrameworks project
## About
Recreation of an animation in openFrameworks with C++, originally created in Processing Java. 

[view tweet](https://twitter.com/jn3008/status/1699497083013640481) \| [view mp4](https://jn3008.com/processing/p079.mp4)

The animation is generative, that is, changing the random seed will result in a different outcome.
In the original version I experimented with many kinds of cube movements, but in the end I stuck with rolling and 'unsquishing' and have only included these in this implementation. To change the 'plot' of the animation, play around with the `submit_...` functions in `ofApp::setup` (in `ofApp.cpp`), or modify / create such functions in `Patterns.h`.

This project was created with some creative and technical input from [@Bleuje](https://www.github.com/Bleuje).

## Compile and run

You need to have openFrameworks installed to compile this project. Find setup guides for openFrameworks projects here: [https://openframeworks.cc/download/](https://openframeworks.cc/download/)

To compile and run: 
```
make -j6 && make run
```
or, for console and text output:
```
make -j6 && make run | tee output.txt
```

To enable running the program by double-clicking the exe file, copy the required DLLs:
```
make copy_dlls
```


