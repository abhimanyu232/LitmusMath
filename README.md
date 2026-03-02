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
## A C++ header only library that implements and benchmarks various numerical methods

**Dependencies and Requirements**
------
* Compiler : Tested with `g++-14` and above, with `--std=c++23`
* *  uses `<print>`, `<span>`, and `<ranges>` .
* MPI : OpenMPI or equivalent

currently under development, not intended for use.  

**Build instructions** 
---
1. Configure CMake: `cmake -B build -DTESTING=ON -DCMAKE_BUILD_TYPE={Release|RelWithDebInfo|Debug}`

2. Build and Compile: ` cmake --build build `

    * Default CMAKE_BUILD_TYPE = RelWithDebInfo

    * Default Testing = OFF 

3.  Run Benchmarks: `./run_bench_mm <min_pow_2_size> <max_pow_2_size> <n_iterations>` </br>
	*	default config: `./run_bench_mm 2 9 100`

**Current Features**
---
- Matrix Multiplication
	- Naive implementation 
	- Transpose before multiplication for cache friendly behaviour  
	- Strassen's Algorithm

- Statistics
	-	Compute basic measures of central tendency

- Benchmarking and Timing 
	- Using `chrono::steady_clock`

- Testing
	- Implemented using GoogleTest and is fetched by CMake during build.

**Some Preliminary Benchmarks**
---
Hardware: AMD 5800X3D, 4.1GHz, 32GB DDR4

Compiler: `gcc15` <br>
Flags: `-std=c++23 -O2 -g ` 

![Matrix Multiplication [FP64]](/data/benchmarks/images/Matrix_Multiplication_[FP64].png "Double Precision Matrix Multiplication")

![Matrix Multiplication [FP34]](/data/benchmarks/images/Matrix_Multiplication_[FP32].png "Single Precision Matrix Multiplication")

**Planned Features** 
---
- Matrix algebra methods
- More robust benchmarking using RDTSC
- Parallel MPI implementations 
- SIMD implementations
- CUDA implementations 

**Authors**
---
Dr. Abhimanyu Bhadauria, Ph.D
