# RenderLite

my first cpu rasterizer
implement some basic technique

## Features
- programmable pipeline, easy to switch shader
- right hand coordinate, like OpenGL
- perspective correct
- face culling 
- cvv clip
- homogeneous clip
- orbit camera
- skybox support
- ASSIMP integrated，enable to load various model
- STB_IMAGE integrated, enable to load picture（texture）
- simple math class(vector, matrix)
- MSAA4x(deprecated)
- some extra 2d stuff in Draw2d
- wireframe or triangle topology

## Limitations
- Windows only
- a few implementation may not easy to read
- performance relatively low(release build prefer), generally for only one model

## Dependencies
- stb_image (included already)
- assimp 5.0.1 (NEED to config VS project settings; only use some basic functions, so maybe is ok to make a difference)


## Example
![](https://s2.loli.net/2022/01/13/W8JOG9IFoQtTpng.png)
![](https://s2.loli.net/2022/01/13/BVHzdfvrlnwGXUj.png)
![](https://s2.loli.net/2022/01/13/H8VPUFnsTm2cbXB.png)
![](https://s2.loli.net/2022/01/13/7aMgxlNYSjcXUQP.png)
![](https://i.loli.net/2021/10/06/yvRCPYsD4NwQ13L.png)

