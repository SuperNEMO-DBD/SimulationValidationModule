# SimValidationModule readme


Y. Ramachers (Warwick)
Last updated July 8th 2019

SimValidationModule is a Falaise pipeline module to process a selection of basic validation data and output a ROOT ntuple file, to be used for standard plots and metrics to validate simulation data quality.  

## Files:

- SimValidationModule.cpp
- SimValidationModule.h
- CMakeLists.txt
- SimValidationModuleExample.conf.in


## Description

Add to an flreconstruct pipeline to generate a ROOT ntuple file with some pertinent branches. To build it, do

``` console
$ ls
CMakeLists.txt                   SimValidationModule.h
README.md                        SimValidationModuleExample.conf.in
SimValidationModule.cpp
$ mkdir build
$ cd build
$ cmake -DCMAKE_PREFIX_PATH=<pathtoyourfalaiseinstall> ..
...
$ make
...
... If you are developing the module, you can test it by doing ...
$ make test
```

Note: if you get a QT5 error, you may need to specify the QT5 path when you run the cmake line, as given by `brew --prefix qt5-base`. For example, you can run:
``` console
$ cmake -DCMAKE_PREFIX_PATH="$(brew --prefix qt5-base);$(brew --prefix)" ..
``` 

The build will create the `libSimValidationModule` shared library plus the example `flreconstruct` pipeline
script `SimValidationModuleExample.conf`. Assuming that you have an `input.brio` file that contains
the `SD` data bank from `flsimulate`, this can be run as:

``` console
... Assume we run in the build dir ...
$ ls
CMakeCache.txt                SimValidationModuleExample.conf
CMakeFiles                    cmake_install.cmake
Makefile
...
$ flreconstruct -i /path/to/input.brio -p SimValidationModuleExample.conf
...
$ ls
CMakeCache.txt                SimValidationModuleExample.conf
CMakeFiles                    cmake_install.cmake
Makefile                      SimValidation.root
```

The output file will by default be called `SimValidation.root` so don’t run it multiple times concurrently in the same directory
or you will overwrite the previous file! Use the falaise flreconstruct pipeline instructions to see how to integrate this module in your pipeline.

There is now the option to configure the output filename in the module configuration file.
The final two lines of the configuration file must read:

[name="processing" type="SimValidationModule"]
filename_out : string[1] = "my_filename.root"

## Types of branch

