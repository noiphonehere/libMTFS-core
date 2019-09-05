# libMTFS
This is a collection of SDL and OpenGL Compatibility Profile C++ classes
that I wrote for my own purposes many years ago. Most of them are utilized for
display of user interface elements or simple 3D models using OpenGL.

The code is messy and doesn't have a Makefile, much less a configure script
or CMakeLists file. It needs SDL, OpenGL, and SDL\_Image.

## Status
This code is poorly documented. It's known to be a bit buggy (though hopefully
not too much at this point). It also hasn't really been worked on in a long
time. It's also not as simple to use as it should be, and its quirks are not
documented.

## How Do I Use This?
If you really want to use it, make sure you have the necessary dependencies
and run build.sh. Then, make sure the needed headers from the include folder
are referenced in the appropriate places in your project, and that the
resulting libMTFS-core.so is linked in.
To build for SDL 2, you need to add -DUSE\_SDL2 to the end of the command in
build.sh.

## Will You be Working on This Anymore?
Probably not. It's horribly obsolete, not leastly because it uses the OpenGL
Compatibility Profile.

## License
Licensed under the Expat license, taken from: https://directory.fsf.org/wiki/License:Expat
Copyright 2010-2012, 2019 Carlo Retta
