immature engine
===============
C++  
immature engine
Quick-and-dirty

Introduction:
-------------
A small game engine. 
The fundamental 3D rendering technique is derived from the textbook 
Introduction to 3D Game Programming with DirectX 11 by Frank Luna.
This engine development methodology is Quick-and-dirty, no traditional API design, casual style.
I suggest do "Mod" on this game engine, it will not waste too much time,
because the engine is on low completion status.

In Development:
---------------
The project is in development, there is less document and feature now.
The framework of the engine is instability, will be changed frequently.
Just guarantee the able to compile the project correctly.

Compile Environment:
--------------------
Win 7.1 / Win 8.x  
Visual Studio 2013  

Runtime Environment:
--------------------
Win 7.1 / Win 8.x / Win 10  
DirectX End-User Runtimes (June 2010)  
Visual C++ Redistributable Packages for Visual Studio 2013  
Update KB2670838 (Windows 7.1 situation)  
DirectX 11 capable GPU  
1024X768 minimum display resolution  

Dependent Librarie Files:
-------------------------
Those files should be in header_files_third_party\, 
notice corresponding x64/x86, Debug/Release version.
* **DirectXTK.lib**: DirectX Tool Kit
* **Effects11.lib**: Effects for Direct3D 11 (FX11)
* **lua.lib**: Lua 5.3.0
* **lua_static.lib**: Lua 5.3.0
* **XInput1_3.lib**: Its orgin name is XInput.lib, from DirectX SDK (June 2010)

How to Build:
-------------
In demo folder, every subfolder inlude a signle demo.
The compile option see bat_tool\1.bat.
* **init_d3d**: Basic windows draw empty.
* **simple_secene**: The current work (see Released Demo).
3D models and textures of this demo is absent in the repository, 
please obtain from the Released Demo or rebuild the asset.

Runtime Folder Tree:
--------------------
See minimum_root\, represents following tree.

	 |--ass_model
	 |--ass_shader
	 |--ass_texture
	 |--misc
	 |   |--input
	 |   |--output
	 |--script
	     |--cmd_util
	     |--library
	     |--scene


Asset and Tools:
----------------
* **immature Blender export**: 
Export Blender model data to .m3d file format for immature engine. 
https://github.com/endrollex/imm_blender_export
* **immature engine console util b3m tool**: 
Convert .m3d file to binary file, it can be found in game input mode,
"Alt+Delete" then type "help" for more info.

Released Demo:
--------------
* **x64**: http://pan.baidu.com/s/1c0yFrCc
* **x86**: http://pan.baidu.com/s/1mg5k7XY

Copyright and License:
----------------------
* Copyright 2015 Huang Yiting (http://endrollex.com)
* immature engine is distributed under the the terms of the BSD license
* Not include the files in floders of suffix "_third_party".