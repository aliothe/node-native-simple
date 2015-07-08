node-native-simple
==================

A simple example of writing a native node module

Prerequisites
=============
* node js - https://github.com/joyent/node http://nodejs.org
* c++ compiler that groks c++11 - see below for windows and linux
* Note: This branch is for node v0.12.x - see master branch for node v0.10.x.

Building
========
# Windows
* Works with visual studio express 2013 and node js v0.10.29
* You still need to have python, [windows-python-v2.7.3], installed
* You should also install jasmine-node globally ( IE npm install jasmine-node)

```
npm install
npm test 
```

# Linux
You need to have a version of g++ that supports the c++11 features used in this project.

g++-4.6.3 works.

You also need to have python and make installed as well as jasmine-node installed globally.

``` bash
$ npm install 
```

For clang, on linux, you may want to try something like
``` bash
$ GYP_DEFINES=clang=1 CC=clang CXX=clang++ npm install
```

``` bash
$ npm test 
```

Notes
=====
Yes the actual example task in this module is stupid and I will try to find something more interesting. 

Still, it does show the basic usage of libuv.

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
