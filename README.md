# VISUAL PROJECT
This project is intended as a testing ground for usage of OpenGL (in C).
* This project is intended to run on Unix based OS' (Linux, Mac)

### Libraries
(List disincludes common C Standard libraries)
* OpenGL (v4.6)
    * GL Loading Lib: [GLEW](https://github.com/nigels-com/glew)
* [GLFW](https://www.glfw.org/) (v3)
* Nothing's [STB](https://github.com/nothings/stb)
* [CGLM](https://github.com/recp/cglm)

#### Windowing API Support
Only X11 is supported; however, you may run it on Wayland if you're using XWayland.

### Usage
Renders provided `.vpvox` files found in `VisualProject/Objects/` as cubes.
* Specified via the usage of `VP_LoadVox()`.

Step by Step (Linux):
* After `git clone`ing the repo, run `make -B` within the `VisualProject` folder.
* Then run (within the `VisualProject` Folder) `./.out/vinit.o`
