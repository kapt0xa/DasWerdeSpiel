# game with c++ and SFML

requirements and installation process are described below  

# suggested file organisation:

the instruction is written for this file organisation:
```
<parent_path>
  ├SFML (source cloned from github)
  ├TGUI (source cloned from github)
  ├BoostQVM (source cloned from github)
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
    SFML 3  
    TGUI  
    BoostQVM  

-   SFML 3.0  
    .  
    the repository: https://github.com/SFML/SFML.git  
    the exact version: https://github.com/SFML/SFML/tree/e533db99c8a83a6b81c47105964e10cbab421a0d (january 2026)  
    ofitial SFML building instruction: https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/#building-sfml  
    .  
    download lib via git:  
    `cd <parent_path>`  
    `git clone https://github.com/SFML/SFML.git ./SFML`  
    (or just download it manually without git and locate it into `<parent path>`)  
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
    .  

-   TGUI
    .  
    the repository: https://github.com/texus/TGUI/  
    exact version: not defined yet  (jan 2026)  
    documentation: https://tgui.eu/tutorials/latest-stable/  
    .  
    download lib via git:  
    `cd <parent_path>`  
    `git clone https://github.com/texus/TGUI/ ./TGUI`  
    (or just download it manually without git and locate it into `<parent path>`)  
    .  

-   Boost QVM  
    .  
    the repository: https://github.com/boostorg/qvm.git  
    download lib via git:  
    `cd <parent_path>`  
    `git clone https://github.com/boostorg/qvm.git ./BoostQVM`  
    .  

# build the project
-   linux dependencies:  
    .  
    exact exact name of the packages may vary.  
    you can find link to documentation above.  
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
-   download libs and project via git:  
    .  
    `cd <parent_path>`  
    .  
    `git clone https://github.com/SFML/SFML.git ./SFML`  
    `git clone https://github.com/texus/TGUI/ ./TGUI`  
    `git clone https://github.com/boostorg/qvm.git ./BoostQVM`  
    `git clone https://github.com/kapt0xa/DasWerdeSpiel ./DasWerdeSpiel`  
    .  
-   Build project:  
    .  
    `cd DasWerdeSpiel`  
    ```
    cmake -B build && \
    cmake --build build --config=Debug --parallel
    ```
    (you can replace `Debug` with `Release`.  
    `--parallel` is optional, it makes building faster)  
    .  
-   Run the project:  
    .  
    `cd build` or `cd build/Debug` or `cd build/Release`  
    .  
    `./DWS.exe` or `./DWS` or `DWS`  
    .  
    as alternative, you can find the DWS executable and run it via fylesystem GUI  
    .  

# vscode setup:

-   for comfortable code editing.  
    c_cpp_properties.json:  
    for Linux:
    ```
    {
        "configurations": [
            {
                "name": "Linux",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/build/_deps/sfml-src/include",
                    "${workspaceFolder}/build/_deps/tgui-src/include",
                    "${workspaceFolder}/build/_deps/boostqvm-src/include"
                ],
                "compilerPath": "g++",
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
                    "${workspaceFolder}/build/_deps/tgui-src/include",
                    "${workspaceFolder}/build/_deps/boostqvm-src/include"
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

# this repository:
https://github.com/kapt0xa/DasWerdeSpiel  
in vscode - shift+ctrl+V to view the .md file
