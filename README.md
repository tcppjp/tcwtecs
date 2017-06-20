TCW for TECS
============

Experimental super-lightweight GUI toolkit for [TECS (TOPPERS Embedded Component System)](https://www.toppers.jp/tecs.html).

Building Example Project
------------------------

Before building the project, the directory that contains `tecsgen` (TECS generator) must be added to the `PATH` environment variable.

### SDL2 Example

Development package of [SDL2 library](https://www.libsdl.org/) is required to build the project.

1. Move to the build directory by `cd test/sdl-basic/build`
2. Run `make tecs` to generate TECS interface code from TECS CDL files. The `gen` directory will be created.
3. Run `make` to build the executable file.
4. RUn `./test` to start the example program.

When you update any dependent CDL files, you must re-run `make tecs` so that interface code is up-to-date.

### GR-PEACH + mbed-cli

You need to install [mbed-cli](https://github.com/ARMmbed/mbed-cli) before building this project. Using [pipenv](https://github.com/kennethreitz/pipenv) is recommended to manage Python environments. Start the pipenv environment by running `pipenv shell` at the repository root.

1. Move to the build directory by `cd test/sdl-basic/build`
2. Run `make tecs` to generate TECS interface code from TECS CDL files. The `gen` directory will be created.
3. Run `make` to build the library file.
4. `cd ../mbed-app`
5. `cp mbed_settings_example.py mbed_settings.py` and edit `mbed_settings.py` to match your building environment
6. `mbed update` (This might take a minute or an hour)
7. `mbed compile`

### STM32F746G-Discovery

A sample project for [STM32F746G-Discovery](http://www.st.com/en/evaluation-tools/32f746gdiscovery.html) can be found in the directory `test/stm32f7d-stopwatch`.

