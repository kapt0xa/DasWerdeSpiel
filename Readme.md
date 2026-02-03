# game with c++ and SFML

first installation process is dscribed.  
usage will be described (not yet) afrer installation  

# suggested file organisation:
all comands suggested in this tutorial for setup are written to be executed from build bolder, related to context module.
comands like `cd <path>` or `mkdir build` are not listed in tutorial, but U should use them to execute command correctly
    ```
    <parent_path> (folder with libs and project. for me it is dev_cpp)
    ├SFML (cloned from github)
    │ ├build (created manually, building of SFML happens from here)
    │ │└lib (created after building the SFML, should contain .lib files)
    │ │ └*.lib (read below about Windows)
    │ └include (include path)
    │   └SFML
    ├TGUI (cloned from github)
    │ └build (created manually, building of TGUI happens from here)
    ├BoostQVM (cloned from github)
    │ └include (include path)
    │  └boost
    └DasWerdeSpiel (the folder with this project)
      ├build (created manually, building of this project happens from here)
      ├resrc (resources, contains some files used in game)
      ├src (source code)
      ├CmakeLists.txt (filoe with project settings)
      └Readme.md (this readme file itself)
    ```

# requirements
-   first read suggested file organisation, it is above.  

-   git, cmake, some compiler (g++/clang for ubuntu, clang++/visual studio compiler for windows), some build system (GNU make for g++, LLVM for clang, visual studio for visual studio)  

-   SFML 3.0  
    the repository: https://github.com/SFML/SFML.git  
    the exact version: not defined yet, last for ~jan 2026  
    to download lib:  
    `git clone https://github.com/SFML/SFML.git ./SFML`  
    (executed from parent_path, outside this project)  
    or just download it manually without git  

    for Linux might require additional installing:  
    `sudo apt update`  
    ```
    sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libxi-dev \
        libudev-dev \
        libfreetype-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev \
        libfreetype-dev
    ```  
    (this list is taken from https://github.com/SFML/cmake-sfml-project )  

    to build lib:  
    `cmake ..` (executed from SFML's build folder)  
    `cmake --build ./`  

    for Windows U might need so specify release or debug version:
    `cmake --build ./ --config=Release`  
    `cmake --build ./ --config=Debug`  
    also microsoft visual studio locates the built fules in additional directory, which is unwanted.  
    make sure U move compiled files from `lib/Release` or `lib/Debug` to folder `lib`.  
    the project is cross-platform, but ubuntu is prioritised, so windows separation into debug and release is not supported. choose only one option or edit cmakelists.  

-   Boost QVM  
    to install it:  
    `git clone https://github.com/boostorg/qvm.git ../../BoostQVM` (executed from {project_root_path}/build)  

# build
## Ubuntu
- U require git, cmake, make, g++ installed also U require some SFML-specific modules, i dont know what. i just read errors, googled them and instlled required modules for SFML
- start folder is folder of project (if U print "$ ls" it should list some likes, one of them is this "Readme.md")
- U require to copy SFML's repository. read comments in CMakeLists.txt, ctrl+F "git clone"
- the cloned repository should be in the same folder the project is located. if u print "$ ls ..", it should list the SFML's repository. the exact name and location can be changed in CMakeLists.txt.
- than u create folder "build" and go into it: "$ mkdir build", "& cd build"
- build the project via cmake and make: "$ cmake ..", "$ make"
- run the executable. it is named "DWS"
- resources used for the executable should be located in path relative to executable "../resrc"

# vscode setup:

-   for comfortable code editing:  
    c_cpp_properties.json:
    ``` 
    {
        "configurations": [
            {
                "name": "Linux",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/build/_deps/sfml-src/include",
                    "${workspaceFolder}/../BoostQVM/include"
                ],
                "compilerPath": "/usr/bin/g++",
                "cStandard": "c17",
                "cppStandard": "c++20",
                "intelliSenseMode": "linux-gcc-x64" 
            }
        ],
        "version": 4
    }
    ```
    or for windows:
    ```
    {
        "configurations": [
            {
                "name": "Windows",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/build/_deps/sfml-src/include",
                    "${workspaceFolder}/../BoostQVM/include"
                ],
                "compilerPath": "clang++",
                "cStandard": "c17",
                "cppStandard": "c++20",
                "intelliSenseMode": "windows-clang-x64" 
            }
        ],
        "version": 4
    }
    ```

-   this repository: https://github.com/kapt0xa/DasWerdeSpiel 
