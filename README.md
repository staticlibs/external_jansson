Jansson JSON library build for Staticlibs
=========================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [Jansson library](https://github.com/akheron/jansson). 
Wrapper uses Jansson's own CMake project through `add_subdirectory`, supports toolchain specification
with `STATICLIB_TOOLCHAIN` option and exports Jansson headers to be used from dependent projects.

Jansson GitHub repository is used as a git submodule of this project. Jannson is pinned to version 2.6.

Link to [API documentation](https://jansson.readthedocs.org/en/2.6/).

How to build
------------

[CMake](http://cmake.org/) is required for building.

[TODO]

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Changelog
---------

**2015-07-09**

 * version 2.6.0.2 - toolchains update

**2015-06-30**

 * version 2.6.0.1 - toolchains update

**2015-05-20**

 * 2.6.0, initial public version
