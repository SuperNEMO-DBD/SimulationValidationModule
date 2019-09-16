# SimValidationModule README

Yorck Ramachers (Warwick), Bartosz Pyszkowski (Warwick)
Last updated August 19th 2019

SimValidationModule is a Falaise pipeline module to process a selection of basic validation data and output a ROOT ntuple file, to be used for standard plots and metrics to validate simulation data quality.  

## Files:

- SimValidationModule.cpp
- SimValidationModule.h
- CMakeLists.txt
- SimValidationModule.conf.in


## Description

Add to an flreconstruct pipeline to generate a ROOT ntuple file with some pertinent branches. To build it, do

``` console
$ ls
CMakeLists.txt                   SimValidationModule.h
README.md                        SimValidationModule.conf.in
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
script `SimValidationModule.conf`. Note that the simulation validation benefits from simulated files with detailed output, i.e. 
the `__visu_tracks` are filled, producing rather large '.brio' files. Assuming that you have such an `input.brio` file that contains
the `SD` data bank from `flsimulate`, this can be run as:

``` console
... Assume we run in the build dir ...
$ ls
CMakeCache.txt                SimValidationModule.conf
CMakeFiles                    cmake_install.cmake
Makefile
...
$ flreconstruct -i /path/to/input.brio -p SimValidationModule.conf
...
$ ls
CMakeCache.txt                SimValidationModule.conf
CMakeFiles                    cmake_install.cmake
Makefile                      SimValidation.root
```

The output file will by default be called `SimValidation.root` so don’t run it multiple times concurrently in the same directory
or you will overwrite the previous file! Use the falaise flreconstruct pipeline instructions to see how to integrate this module in your pipeline.

There is the option to configure the output filename in the module configuration file.
The final two lines of the configuration file must read:

[name="processing" type="SimValidationModule"]

filename_out : string[1] = "my_filename.root"

If in doubt about the `__visu_tracks` content of the SD data bank, you can produce that output (careful, much larger output files) when specifying in the simulation configuration script the variant service option:

[name="flsimulate.variantService" type="flsimulate::section"]

settings : string[1] = "simulation:output_profile=all_details"
