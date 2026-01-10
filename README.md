```
██╗     ██╗████████╗███╗   ███╗██╗   ██╗███████╗███╗   ███╗ █████╗ ████████╗██╗  ██╗
██║     ██║╚══██╔══╝████╗ ████║██║   ██║██╔════╝████╗ ████║██╔══██╗╚══██╔══╝██║  ██║
██║     ██║   ██║   ██╔████╔██║██║   ██║███████╗██╔████╔██║███████║   ██║   ███████║
██║     ██║   ██║   ██║╚██╔╝██║██║   ██║╚════██║██║╚██╔╝██║██╔══██║   ██║   ██╔══██║
███████╗██║   ██║   ██║ ╚═╝ ██║╚██████╔╝███████║██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║
╚══════╝╚═╝   ╚═╝   ╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
```                                                                                   
[![LitmusMath build and tests](https://github.com/abhimanyu232/LitmusMath/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/abhimanyu232/LitmusMath/actions/workflows/cmake.yml)
---

## A C++ header only library that implements and benchmarks various numerical math functions

**Dependencies and Requirements**
* Compiler : Tested with `g++-14` and above with `--std=c++23`
* * Makes uses `<print>`, `<span>`, and `<mdspan>` .
* MPI : OpenMPI or equivalent

Currently in development, not intended for use yet.  

**Build instructions:** 
---
1. Configure CMake: `cmake -B build -DTESTING=ON -DCMAKE_BUILD_TYPE={Release|RelWithDebInfo|Debug}`

2. Build and Compile: ` cmake --build build `

    *  Default CMAKE_BUILD_TYPE = RelWithDebInfo

    * Default Testing = OFF 


**Testing** 
---
Implemented using GoogleTest and is fetched by CMake during build. 

**Authors**
---
Dr. Abhimanyu Bhadauria
