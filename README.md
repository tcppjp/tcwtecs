TCW for TECS
============

Experimental super-lightweight GUI toolkit for [TECS (TOPPERS Embedded Component System)](https://www.toppers.jp/tecs.html).

Building Example Project
------------------------

Before building the project, the directory that contains `tecsgen` (TECS generator) must be added to the `PATH` environment variable.

Development package of [SDL2 library](https://www.libsdl.org/) is required to build the project.

1. Move to the build directory by `cd test/build`
2. Run `make tecs` to generate TECS interface code from TECS CDL files. The `gen` directory will be created.
3. Run `make` to build the executable file.
4. RUn `./test` to start the example program.

When you update any dependent CDL files, you must re-run `make tecs` so that interface code is up-to-date.
