# game with c++ and SFML

first installation process is dscribed.  
usage will be described (not yet) afrer installation  

# suggested file organisation:

the instruction is written for this file organisation:
```
<parent_path>
  ├SFML_SRC (source cloned from github)
  ├SFML (installed via cmake)
  │ ├bin
  │ ├include
  │ └lib
  ├SFML_DBG (installed via cmake, optional)
  ├TGUI (cloned from github)
  ├BoostQVM (cloned from github)
  │ └include (include path)
  │  └boost
  └DasWerdeSpiel (the folder with this project)
    ├build (created with "sfml -B build" comand)
    └Readme.md (this readme file itself)
```

# requirements

-   developer's tools:  
    git  
    cmake  
    some compiler:  
    &emsp;g++/clang for linux/ubuntu  
    &emsp;clang++/visual studio compiler for windows  
    some build system:  
    &emsp;GNU make for linux/ubuntu  
    &emsp;Visual studio or Ninja for windows  
    (recomdnded but you might use any analog of theese tools)  
    .  
    for windows users:
    you might need use `\` instead of `/` in paths.  
    also if you have more than one disk, then pay attention on executing commands `cd <path>`.
    `cd` is for navigating through path and on windows it cant switch disks.
    use `<disk_name>:` to navigate to other disk, like `D:`  
    .  
    `visual studio` and `visual studio code` are not same.  
    `visual studio` - contains its own build system, available for windows and macos  
    `visual studio code` (or VScode) - another simplified but more flexible application  
    visual studio has its own issues with debug and release versions.  
    also for visual studio you should ensure that you have modules for c/c++  
    .  

-   short list of libs:  
    SFML 3 (download and install)  
    TGUI (not described yet)  
    BoostQVM (just download)  

-   SFML 3.0  
    .  
    the repository: https://github.com/SFML/SFML.git  
    the exact version: https://github.com/SFML/SFML/tree/e533db99c8a83a6b81c47105964e10cbab421a0d (january 2026)  
    ofitial SFML building instruction: https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/#building-sfml  
    .  
    download lib via git:  
    `cd <parent_path>`  
    `git clone --branch --single-branch 3.0.2 https://github.com/SFML/SFML.git ./SFML_SRC`
    `cd ./SFML_SRC` (navigate into the cloned repository arter downloading it)  
    (or just download it manually without git and locate it into `<parent_path>`)  
    .  
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
    (yes, this list doesnt qite fit SFML documentation.  
    `libvorbis-dev` is for 3 components - vorbis, vorbisenc and vorbisfile  
    `libgl1-mesa-dev` is for openGL and might depend on your videocard driver  
    `libopenal-dev` is fir OpenAL, is not listed in SFML documentation but is required
    others fit pattern `lib<name>-dev`)  
    .  
    build the lib:  
    ```
    cmake -B build/RLS \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON && \
    cmake -B build/DBG \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBUILD_SHARED_LIBS=ON && \
    cmake --build build/DBG --config=Debug --parallel && \
    cmake --build build/RLS --config=Release --parallel && \
    cmake --install build/DBG --prefix ../SFML --config=Debug && \
    cmake --install build/RLS --prefix ../SFML --config=Release
    ```
    (for windows replace `\` with `^`)  
    .  

-   TGUI
    .  
    the repository: https://github.com/texus/TGUI/  
    exact version: v1.12.0  (jan 2026)  
    documentation: https://tgui.eu/tutorials/latest-stable/  
    .  
    download lib via git:  
    `cd <parent_path>`  
    `git clone --branch v1.12.0 --single-branch https://github.com/texus/TGUI/ ./TGUI_SRC`  
    (or just download it manually without git and locate it into `<parent_path>`)  
    `cd TGUI_SRC`  
    .  
    build the lib:  
    (before pasting it, change `<parent_path>` into actual path, twice)
    ```
    cmake -B build/RLS \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -DTGUI_BACKEND=SFML_GRAPHICS \
        -DSFML_DIR=<parent_path>/SFML/lib/cmake/SFML && \
    cmake -B build/DBG \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBUILD_SHARED_LIBS=ON \
        -DTGUI_BACKEND=SFML_GRAPHICS \
        -DSFML_DIR=<parent_path>/SFML/lib/cmake/SFML && \
    cmake --build build/DBG --config=Debug --parallel && \
    cmake --build build/RLS --config=Release --parallel && \
    cmake --install build/DBG --prefix ../TGUI --config=Debug && \
    cmake --install build/RLS --prefix ../TGUI --config=Release
    ```
    (for windows replace `\` with `^`)  
    .  

-   Boost QVM  
    .  
    to install it:  
    `cd <parent_path>`  
    `git clone --branch  boost-1.90.0 --single-branch https://github.com/boostorg/qvm.git ./BoostQVM`  
    .  

# build the project
-   make sure you covered requirements - installed dependencies for linux, downloaded and built libraries.  
-   navigate into procect:  
    `cd <parent_path>/DasWerdeSpiel`  
-   build and run debug:  
    .  
    ```
    cmake -B build && \
    cmake --build build --config=Debug --parallel && \
    ./build/bin/DWS
    ```
    .  
    build and run release:  
    ```
    cmake -B build && \
    cmake --build build --config=Release --parallel && \
    ./build/bin/DWS
    ```
    or for visual studio:
    .  
    ```
    cmake -B build && \
    cmake --build build --config=Debug --parallel && \
    ./build/bin/Debug/DWS
    ```
    .  
    build and run release:  
    ```
    cmake -B build && \
    cmake --build build --config=Release --parallel && \
    ./build/bin/Release/DWS
    ```
    .  

# vscode setup:

-   for comfortable code editing.  
    c_cpp_properties.json:  
    (requires additional editing)
    ```
    {
        "configurations": [
            {
                "name": "Linux" or "Windows",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/../SFML/include",
                    "${workspaceFolder}/../TGUI/include",
                    "${workspaceFolder}/../BoostQVM/include"
                ],
                "compilerPath": "g++" or "clang++",
                "cStandard": "c17",
                "cppStandard": "c++20",
                "intelliSenseMode": "linux-gcc-x64" or "windows-clang-x64"
            }
        ],
        "version": 4
    }
    ```

# this repository:
https://github.com/kapt0xa/DasWerdeSpiel  
in vscode - shift+ctrl+V to view the .md file
