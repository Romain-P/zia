## httpd server

### Compiled

Ready-to-use binaries

 * Windows: [1.0_win64_X86.zip](https://github.com/Romain-P/httpd/releases/download/1.0/1.0_win64_X86.zip)
 * Linux: [1.0_linux64_X86.zip](https://github.com/Romain-P/httpd/releases/download/1.0/1.0_linux64_X86.zip)

### How to compile
#### Prerequisites
 * Cmake >= 3.13
 * C++17 / GCC >= 8.0
 * Conan
 * For Unix: Makefile
 * For Windows: Visual Studio 15

#### Installation

 * Clone this repository
 * On Unix System `cd install && ./install.sh`
 * On Windows System `cd install && start install.bat`
 * Project binaries built at `/generated`

#### Misc

Install Cmake 3.13 on Linux Distributions
 * Download https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4-Linux-x86_64.sh
 * Execute the script
 * In your `.bashrc` add 
 ```sh
PATH="/path/to/cmake/bin:$PATH"
export PATH
```