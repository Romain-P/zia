## httpd server

### Prerequisites
 * Cmake >= 3.13
 * C++17 / GCC >= 8.0
 * Conan
 * For Unix: Makefile
 * For Windows: Visual Studio 15

### Installation

 * Clone this repository
 * On Unix System `cd install && ./install.sh`
 * On Windows System `cd install && start install.bat`
 * Project binaries built at `/generated`

## Misc

Install Cmake 3.13 on Linux Distributions
 * Download https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4-Linux-x86_64.sh
 * Execute the script
 * In your `.bashrc` add 
 ```sh
PATH="/path/to/cmake/bin:$PATH"
export PATH
```