# Application Core Library
[![Build Status](https://secure.travis-ci.org/justindujardin/ACL.png?branch=master)](http://travis-ci.org/justindujardin/ACL)

## Overview

Application Core Library is a C++ library, built from a stripped-down version of the utility classes used in the Torque family of engines.  It includes basic cross-platform types and classes for building applications on top of.  

The main function of the ACL is to provide a solid basis for developing applications in a cross-platform way.  In addition to the code reused from Torque, the ACL includes a generic C API plugin system, that can be used to implement dynamic library plugins.

## Architecture

There are three general classifications of systems provided by the ACL.  The `core` path contains most of the classes for writing applications that you'll interact with.  The `platform` directory contains an abstracted platform layer, that provides access to OS-specific services (e.g. Threads, FileSystem, Dynamic Libraries).  The `plugin` directory contains a C-interface based plugin system skeleton, to allow compiler independent exporting of functions for use in other libraries.

### Core

When writing any substantial C++ application, solid core classes are essential.  The ACL provides an impressive suite of core classes for building your application on top of.

- [Compiler independent types](https://github.com/justindujardin/ACL/blob/master/acl/core/types.h)
- [Compiler independent string functions](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/stringFunctions.h)
- [String class](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/str.h) with support for [pointer string comparisons](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/str.h#L166).
- [Path class](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/path.h)
- [Unicode string support](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/unicode.h)
- [Stream classes](https://github.com/justindujardin/ACL/tree/master/acl/core/stream)
- [Container classes](https://github.com/justindujardin/ACL/tree/master/acl/core/containers), e.g. Vector, List, Dictionary, Pair
- [Compiler independent aligned memory allocation functions](https://github.com/justindujardin/ACL/blob/master/acl/core/memoryFunctions.h)
- [Ultra-fast delegates](https://github.com/justindujardin/ACL/blob/master/acl/core/util/delegate.h) and [multi-cast delegates](https://github.com/justindujardin/ACL/blob/master/acl/core/util/tSignal.h)
- [Reference-counted pointer classes](https://github.com/justindujardin/ACL/blob/master/acl/core/util/refBase.h)
- [Type rebinding](https://github.com/justindujardin/ACL/blob/master/acl/core/util/typeRebind.h) for implementation-specific abstracted class creation.
- Various Utilities: [endian conversion](https://github.com/justindujardin/ACL/blob/master/acl/core/util/endian.h), [crc calculation](https://github.com/justindujardin/ACL/blob/master/acl/core/crc.h), [compiler intrinsics](https://github.com/justindujardin/ACL/blob/master/acl/core/intrinsics.h), [compile-time and runtime assertions](https://github.com/justindujardin/ACL/blob/master/acl/core/assert.h), ...

### Platform

*TODO*

### Plugin

*TODO*

## Building and Testing

ACL uses [premake4](https://github.com/justindujardin/ACL/blob/master/premake4.lua) to generate Xcode, Visual Studio, and make projects, putting them in the `projects` subfolder. It geneates projects for the library, gtest, and the test-runner application.  

### Build Scripts

There are scripts in the root directory for generating projects on each platform, and for building and running the test-suite, e.g.

Generate make project on linux:

     ./generateLinux.sh

Build and run the test-suite on OSX:

     ./testOSX.sh

Generate Visual Studio projects on Windows:

     ./generateWindows.bat

### Continuous Integration Testing

ACL is setup to do continuous integration building and testing on Ubuntu linux, via travis-ci.  It uses the [gtest](https://github.com/justindujardin/ACL/tree/master/lib/gtest) library for testing the C++ classes.  The test-suite is still relatively small, but it's a good start.  The test-suite passes on OSX, Windows, and Linux platforms.

## Credits

The ACL shares roots with the [Torque](https://github.com/GarageGames/Torque3D) family of game engines from [GarageGames](http://www.garagegames.com), and as such, it would never have been possible without the many people who contributed to Torque over the years.  I am eternally grateful for all the hard work they have put forward, and how it has made this library so brutally good at what it does.

## License

Application Core Library is freely distributable under the MIT license

Copyright (c) 2009-2012 DuJardin Consulting, LLC  
Copyright (c) 2009 GarageGames, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.