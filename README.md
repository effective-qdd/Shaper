# Shaper
This is a framework of image processing and make full use of [Elder](https://github.com/effective-qdd/Elder). It supports multi core image processing. 
Currently it works on windows only. You can load the shaper dynamically.
It also support python. You can call the functions of shaper by the python warp.

### Features
* Buffer - Automatically manage memory and identify with ID
* File - Currently support TIF
* Process - Do image processing
* Processor - Manager image processing algorithms
* Operator - Do image basic operator
* Display - Show image with OpenGL
### Usage
To get the pointer of features above, you must initialize the shaper. You can load the dll file like this:
```c++
#include "Shaper.h"

typedef std::shared_ptr<SHAPER::CShaper> (*ShaperInstanceFunc)();

HINSTANCE shaperLib = LoadLibrary("Shaper.dll");
if (!shaperLib) return;

auto shaperInstFunc = (ShaperInstanceFunc)GetProcAddress(shaperLib, "ShaperInstance");
if(!shaperInstFunc) return;

auto shaper = shaperInstFunc();
shaper->Initialize();

auto file = shaper->File();
auto buffer = shaper->Buffer();
auto process = shaper->Process();
auto processor = shaper->Processor();
auto operate = shaper->Operator();
auto display = shaper->Display();
```
To enable multi core processing, just pass the arguments to the Initialize function of Shaper and then you can use the multi core functions in process.
```C++
shaper->Initialize(true);
```
### Dependency
* C++ 17 - Visual Studio 2017
* Elder
* Intel IPP
* Intel TBB
* OpenGL - GLFW and GLAD
* SWIG and python3 - If you want compile python warp
### TODO
* Add more file format
* Add more image processing algorithms
* Add an configuration file which can be json 
