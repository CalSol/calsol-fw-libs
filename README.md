# calsol-fw-libs
CalSol's open-source, shared libraries for firmware running on the mbed platform.

## Repository Structure
This is meant to be [submoduled](https://git-scm.com/book/en/v2/Git-Tools-Submodules) into a higher-level project.

- [drivers]: driver code for external ICs, dependent on the mbed API
- [utils]: utility code and classes, like RGB LEDs and long tiemrs, dependent on the mbed API
- [hal]: HAL (hardware abstraction layer) extensions to mbed

## Building
A SConscript ([SCons](http://scons.org/) build fragment) is included in this and can be invoked from a higher-level SCons script. This modifies the `env` passed in so `CPPPATH` includes the header locations and `LIBS` includes the built static library.
