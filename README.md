Jansson JSON library build for Staticlibs
=========================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [Jansson library](https://github.com/akheron/jansson). 

Jansson GitHub repository is used as a git submodule of this project. Jannson is pinned to version 2.6.

Link to the [API documentation](https://jansson.readthedocs.org/en/2.6/).

How to build
------------

[CMake](http://cmake.org/) is required for building.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone --recursive https://github.com/staticlibs/external_jansson.git
    cd external_jansson
    mkdir build
    cd build
    cmake ..
    msbuild external_jansson.sln

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2016-01-17**

 * version 2.6.0.5
 * minor CMake changes

**2015-11-17**

 * version 2.6.0.4
 * custom cmake script
 * deplibs cache support

**2015-11-01**

 * version 2.6.0.3 - pkg-config integration

**2015-07-09**

 * version 2.6.0.2 - toolchains update

**2015-06-30**

 * version 2.6.0.1 - toolchains update

**2015-05-20**

 * 2.6.0, initial public version
