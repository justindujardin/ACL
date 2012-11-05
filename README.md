# Application Core Library
[![Build Status](https://secure.travis-ci.org/justindujardin/ACL.png?branch=master)](http://travis-ci.org/justindujardin/ACL)

- [Overview](#overview)
- Systems
 - [Core](#core)
 - [Platform](#platform)
 - [Plugin](#plugin)
- [Credits](#credits)
- [License](#license)

### <a id="overview">Overview</a>

Application Core Library is built from a stripped-down version of the utility classes used in the Torque family of engines.  It includes basic cross-platform types and classes for building applications on top of.  The main function of the ACL is to provide a solid basis for developing applications in a cross-platform way.  In addition to the code reused from Torque, the ACL includes a generic C API plugin system, that can be used to implement dynamic library plugins.

### <a id="core">Core</a>

When writing any substantial C++ application, solid core classes are essential.  The ACL provides an impressive suite of core classes for building your application on top of, e.g.

- [Compiler independent types](https://github.com/justindujardin/ACL/blob/master/acl/core/types.h), e.g. 32-bit numbers are `S32` signed int, `U32` unsigned int, `F32` float
- [Compiler independent string functions](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/stringFunctions.h), e.g. dStrcpy, dStrcmp, etc.
- [String class](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/str.h) with ultra-fast hashtable support for [pointer string comparisons](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/str.h#L166).
- Filesystem [Path class](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/path.h)
- [Unicode string support](https://github.com/justindujardin/ACL/blob/master/acl/core/strings/unicode.h)
- [A full complement of Stream classes](https://github.com/justindujardin/ACL/tree/master/acl/core/stream)
- [Container Classes](https://github.com/justindujardin/ACL/tree/master/acl/core/containers), e.g. Vector, List, Dictionary, Pair
- [Compiler independent aligned memory allocation functions](https://github.com/justindujardin/ACL/blob/master/acl/core/memoryFunctions.h)
- [Ultra-fast delegates](https://github.com/justindujardin/ACL/blob/master/acl/core/util/delegate.h) and [multi-cast delegates](https://github.com/justindujardin/ACL/blob/master/acl/core/util/tSignal.h)
- Utilities, e.g. [endian conversions](https://github.com/justindujardin/ACL/blob/master/acl/core/util/endian.h), [crc functions](https://github.com/justindujardin/ACL/blob/master/acl/core/crc.h), [compiler intrinsics](https://github.com/justindujardin/ACL/blob/master/acl/core/intrinsics.h), [compile-time and runtime assertions](https://github.com/justindujardin/ACL/blob/master/acl/core/assert.h).

### <a id="platform">Platform</a>

*TODO*

### <a id="plugin">Plugin</a>

*TODO*

### <a id="credits">Credits</a>

The ACL shares roots with the [Torque](https://github.com/GarageGames/Torque3D) family of game engines from [GarageGames](http://www.garagegames.com), and as such, it would never have been possible without the many people who contributed to Torque over the years.  I am eternally grateful for all the hard work they have put forward, and how it has made this library so brutally good at what it does.

### <a id="license">License</a>

Application Core Library is freely distributable under the MIT license

Copyright (c) 2009-2012 DuJardin Consulting, LLC  
Copyright (c) 2009 GarageGames, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.