# game with c++ and SFML

first installation process is dscribed.  
usage will be described (not yet) afrer installation  

# suggested file organisation:
the commands in this tutorial are runned from terminal (for ubuntu/linux) or from cmd.exe (for windows)
if u are not familliar with terminal/cmd, google about them.  
also google about cd (windows and linux), sudo (linux), apt (linux), path variables (windows and linux)
if you install components correctly, usually U should worry about path variables.

all comands suggested in this tutorial for setup are written to be executed from build bolder, related to context module.
comands like `cd <path>` or `mkdir build` are not listed in tutorial, but U should use them to execute command correctly
```
    <parent_path> (folder with libs and project. for me it is dev_cpp)
    ├SFML (cloned from github)
    │ ├build (created with "sfml -B build" comand)
    │ └include (include path)
    │   └SFML
    ├TGUI (cloned from github)
    │ └build (created with "sfml -B build" comand)
    ├BoostQVM (cloned from github)
    │ └include (include path)
    │  └boost
    └DasWerdeSpiel (the folder with this project)
      ├build (created with "sfml -B build" comand)
      ├resrc (resources, contains some files used in game)
      ├src (source code)
      ├CmakeLists.txt (filoe with project settings)
      └Readme.md (this readme file itself)
```

# requirements
-   first read suggested file organisation, it is above.  

-   developer's tools:
    git  
    cmake  
    some compiler:  
        g++/clang for linux/ubuntu  
        clang++/visual studio compiler for windows  
    some build system:  
        GNU make for linux/ubuntu  
        Visual studio or Ninja for windows  

-   SFML 3.0  
    the repository: https://github.com/SFML/SFML.git  
    the exact version: not defined yet, last for ~jan 2026  
    to download lib:  
    `git clone https://github.com/SFML/SFML.git ./SFML`  
    (executed from parent_path, outside this project)  
    or just download it manually without git  

    for Linux might require additional installing:  
    ```
    sudo apt update
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
    (this list of dependencies is taken from https://github.com/SFML/cmake-sfml-project )  

    to build lib:  
    or at least add it before `sfml_set_option`  

    (executed from SFML's build folder)  
    `cmake -B build -DBUILD_SHARED_LIBS=ON`  
    `cmake --build build` (not in case of Windows + visual studio)  

    for Windows if cmake is usind visual studio, U might need so specify release or debug version:  
    `cmake --build build --config=Release`  

    Then install the SFML:  
    ubuntu/linux:  
    `sudo cmake --install build --prefix <desired path>`  
    windows:  
    run cmd.exe as administrator, navigate to required path and run the comand:  
    `cmake --install build --prefix <desired path>`  
    for windows it is likely to fail if cmake used visual studio and u did not build release version.  

>   on failure dont hurry to delete all files in build folder  
>   if u dont want to uninstall SFML:  
>   manually delete all installed files.  
>   the files would be listed in `install_manifest.txt`  
>   this install manifest should be located at `SFML/build` after installation  
>   if u can not find `install_manifest.txt`, try to find folder SFML on Ur PC  
>   for me on windows it is `C:\Program Files (x86)\SFML`  
>   i did not test it for ubuntu yet.  

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
