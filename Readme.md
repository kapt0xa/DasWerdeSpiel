# game with c++ and SFML

# requirements
-   SFML 3.0 
    the repository https://github.com/SFML/cmake-sfml-project
    the exact version https://github.com/SFML/cmake-sfml-project/tree/98e2d5c3d5f0906a89606044b42f1b756fd7fd4b

this is offline version. once U downloaded all requirements, U dont need internet connection to rebuild the project with cmake. read Cmakelists.txt to understand how to setup external dependencies (ctrl+F "GIT").

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

 for comrortable code editing:

``` c_cpp_properties.json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/build/_deps/sfml-src/include"
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
