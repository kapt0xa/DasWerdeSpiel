# game with c++ and SFML

first installation process is dscribed.  
usage will be described (not yet) afrer installation  

# suggested file organisation:
the commands in this tutorial are runned from terminal (for ubuntu/linux) or from cmd.exe (for windows)
if you are not familliar with terminal/cmd, google about them.  
also google about cd (windows and linux), sudo (linux), apt (linux), path variables (windows and linux)
if you install components correctly, usually you should worry about path variables.

the instruction is written for this file organisation:
the `<parent_path>` can be named anyway.
```
    <parent_path> (folder with libs and project. for me it is dev_cpp)
    ├SFML_SRC (source cloned from github)
    ├SFML (installed via cmake)
    │ ├bin
    │ ├include
    │ └lib
    ├TGUI (cloned from github)
    ├BoostQVM (cloned from github)
    │ └include (include path)
    │  └boost
    └DasWerdeSpiel (the folder with this project)
      ├build (created with "sfml -B build" comand)
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
    the exact version: not defined yet, if modern last version does not work, try to use last version of january 2026  
    ofitial SFML building instruction: https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/#building-sfml  
    download lib via git:  
    `cd <parent_path>`
    `git clone https://github.com/SFML/SFML.git ./SFML_SRC`
>   on windows if `<parent path>` is not located on main disk, you might need to use `<disk_name>:` to switch disk, like `D:`  
>   `git clone <url or path from> <path to>`  
>   paths are written with `/` but on windows you might need to use `\` instead of `/`
    or just download it manually without git and locate it in <parent path>  

    for Linux might require additional installing.  
    from SFML documentation https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/#introduction :  

```
    SFML depends on a few other libraries, which will be automatically built as part of the SFML project unless otherwise specified by the SFML_USE_SYSTEM_DEPS option (see below). On Linux, however, this option is disabled by default, so either enable it, or ensure the following packages are installed on your system:

    -freetype
    -harfbuzz
    -flac
    -ogg
    -vorbis
    -vorbisenc
    -vorbisfile
    -mbedtls

    On Linux there are also other system dependencies that must be installed in all cases:

    -x11
    -xrandr
    -xcursor
    -xi
    -udev
    -opengl
    -pthread

    The exact name of the packages may vary from distribution to distribution. Once those packages are installed, don't forget to install their development headers as well.
```

    for ubuntu this installations are recomended:  
```
    sudo apt update
    sudo apt install \
        libfreetype-dev \
        libharfbuzz-dev \
        libflac-dev \
        libogg-dev \
        libvorbis-dev \
        libmbedtls-dev \
        libx11-dev \
        libxrandr-dev \
        libxcursor-dev \
        libxi-dev \
        libudev-dev \
        libgl1-mesa-dev \
        libopenal-dev
```
>   yes, this list doesnt match well with SFML documentation.  
>   `libgl1-mesa-dev` - it is for opengl, might depend on videocard driver  
>   `libopenal-dev` - openAL is not listed in SFML documentation, but it is requires for audio  
>   `libvorbis-dev`- contains not just vorbis, but also vorbisenc and vorbisfile  

    `cd ./SFML_SRC`  
    `cmake -B build -DBUILD_SHARED_LIBS=ON`  
>   `cmake -B <path to build> -D<additional paraments>`  

    if not visual studio (if build files go into `SFML_SRC/build/lib`):  
    `cmake --build build`  
>   `cmake --build <path to build>`  
    if visual studio (if build files go into `SFML_SRC/build/lib/Debug`):  
    `cmake --build build --config=Release`  
>   `--config=Release` - without Release the `--install` comand will fail and visual studio builds Debug by default  

    Then install the SFML:  
    ubuntu/linux:  
    `sudo cmake --install build --prefix ../SFML`  
    windows:  
    run cmd.exe as administrator, navigate to required path and run the comand:  
    `cd <parent_path>/SFML`
    `cmake --install build --prefix ../SFML`  

>   `sudo` - linux analog to window's administrator access  
>   `cmake --install <path to build> --prefix <path to install>`

-   Boost QVM  
    to install it:  
    `git clone https://github.com/boostorg/qvm.git ./BoostQVM` (executed from parent folder, outside project)  

# build
## Ubuntu
- you require git, cmake, make, g++ installed also you require some SFML-specific modules, i dont know what. i just read errors, googled them and instlled required modules for SFML
- start folder is folder of project (if you print "$ ls" it should list some likes, one of them is this "Readme.md")
- you require to copy SFML's repository. read comments in CMakeLists.txt, ctrl+F "git clone"
- the cloned repository should be in the same folder the project is located. if you print "$ ls ..", it should list the SFML's repository. the exact name and location can be changed in CMakeLists.txt.
- than you create folder "build" and go into it: "$ mkdir build", "& cd build"
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
