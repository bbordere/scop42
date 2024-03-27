# Scop
Simple obj file viewer with texture using OpenGL

## Overview
Scop is 42 project that involves the implementation of obj file viewer using OpenGL 3.3. \
The 3D model is displayed in perspective view, rotating on itself. A texture is also applied to it. 
No other libraries were used except for window management (GLFW).
## Installation

Clone the repository:

```shell
# Clone this repository
git clone https://github.com/bbordere/scop42.git

# Go into the repository
cd scop42
```

## Compile and Run
You simply have to run: 
```shell
make
```
To run the program: 
```shell
./scop <path_to_obj_file> <path_to_texture_file>
```
If no argument is provided, the program starts with default cube and default texture.
If one argument is provided, the programs starts with loaded model and default texture.

The texture file must be in ``bmp`` format ***uncompressed***. <br>
There are example models and textures in ``models`` and ``res`` folders.

## Features
  * ``Texture``: Apply texture on model
  * ``UV Mapping``: Only for objects that don't have it
  * ``Normals Computation``: Only for objects that don't have it
  * ``Mesh Triangulation``: Objects can be rendered regardless of its faces layout
  * ``Lighting and Shadows``: Phong model with shadow map
  * ``Reflection``: Object texture is replaced by chrome or reflective material
  * ``Landscape Skyboxes``: There are 4 landscapes available with different light settings
  * ``Wireframe``: Object can be rendered as a wireframe or a point cloud
  * ``Drag and Drop``: You can drag and drop models and textures inside window

## Controls
  * ``WASD``: Move the camera in world
  * ``Left/Right Arrow``: Rotate the camera
  * ``Ctrl-l/Shift``: Move down/up the camera
  * ``Esc``: Exit
  * ``F1-F4``: Change landscape
  * ``R``: Toggle Model Rotation
  * ``T``: Toggle Wireframe mode
  * ``U``: Toggle UV Sphere Projection
  * ``F``: Reset camera position
  * ``G``: Toggle Skybox
  * ``C``: Toggle Reflection Shader
  * ``B``: Toggle Bounding Box rendering
  * ``N``: Toggle Normals rendering
  * ``Space``: Toggle Triangle color mode

## Screenshots
![Screenshot from 2024-03-27 15-57-47](https://github.com/bbordere/scop42/assets/45495330/6acdaff5-1d91-46a2-ab1d-781c81f7b77a)
<br>
![Screenshot from 2024-03-27 15-59-06](https://github.com/bbordere/scop42/assets/45495330/2a3d3896-ab3b-4438-852e-ba83666cf10f)
<br>
![Screenshot from 2024-03-27 16-00-39](https://github.com/bbordere/scop42/assets/45495330/13de5b99-c1cf-48e2-ad58-c03d2c1a4302)
<br>
![Screenshot from 2024-03-2![Uploading Screenshot from 2024-03-27 16-03-29.png…]()
7 16-01-36](https://github.com/bbordere/scop42/assets/45495330/2e8fd9d0-72f8-4e3d-a5c5-a514600ad2ad)
<br>

![Screenshot from 2024-03-27 16-03-29](https://github.com/bbordere/scop42/assets/45495330/2c6709aa-78b0-4e7a-871e-f6d186d725ce)
