# tiny tiny pascal

# Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

# Running

From directory ```build``` execute: ```Debug\tiny_tiny_pascal.exe```.

```bash
Usage: tiny_tiny_pascal [-h] [--scanner] [--parser] [--semantic] [--generation] --out PATH file

Positional arguments:
  file         	path to source file 

Optional arguments:
  -h, --help   	shows help message and exits 
  -v, --version	prints version information and exits 
  --scanner    	run scanner 
  --parser     	run parser 
  --semantic   	run semantic 
  --generation 	run generator 
  --out PATH   	path to outfile [default: "a.out"]
```

# Run tests

From directory ```build``` execute: ```Debug\tiny_tiny_pascal_tests.exe```.

# Requirements

- nasm
- gcc

# CATS

Компилятор языка программирования Pascal

Написан на языке программирования C++

Разработчик: студент ДВФУ группы Б9120-09.03.03пикд Варфоломеев Андрей