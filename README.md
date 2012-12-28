node-native-simple
==================

A simple example of writing a native node module

Prerequisites
=============
* node js - https://github.com/joyent/node http://nodejs.org
* c++ compiler that groks c++11 - see below for windows and linux

Building
========
# Windows

You can not do npm install and expect it to work since node-gyp does not set up the project to use the 2012 November CTP compiler
that is needed for the c++11 features that is used in this code.
Thus you need to do something like:
``` bash
$ npm install -g node-gyp
```

You also need to install the dependencies in package.json since you can not do npm install.
``` bash
$ npm install underscore jasmine-node
```

Software needed besides node:
* Microsoft Visual Studio C++ 2012 for Windows Desktop ([msvc2012-express] version works well) 
* Visual C++ Compiler November 2012 CTP - http://www.microsoft.com/en-us/download/details.aspx?id=35515
* [windows-python] ([`v2.7.3`][windows-python-v2.7.3] recommended, `v3.x.x` is __*not*__ supported)
* [compiler-update-Windows-SDK-7.1]

Run 
``` bash
$ node-gyp configure
```

and make sure that it is ok.
Now open the project in visual studio 2012 and change Properties/General/Platform Toolset to the November CTP edition, compile for release.

npm test to verify that it all works as expected.

# Linux
You need to have g++ >= v4.5 for the c++11 features used in this project.
You also need to have python and make installed.

``` bash
$ npm install 
```

``` bash
$ npm test 
```

Notes
=====
Yes the actual example task in this module is stupid and I will try to find something more interesting. Still, it does show the basic usage of libuv.

License
-------

(The MIT License)

Copyright (c) 2012 Mathias Creutz &lt;mathiascreutz@yahoo.se&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[windows-python-v2.7.3]: http://www.python.org/download/releases/2.7.3#download
[msvc2012-express]: http://www.microsoft.com/en-us/download/details.aspx?id=34673
[win7sdk]: http://www.microsoft.com/en-us/download/details.aspx?id=8279
[compiler-update-Windows-SDK-7.1]: http://www.microsoft.com/en-us/download/details.aspx?id=4422
