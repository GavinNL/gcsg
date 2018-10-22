# gcsg

Master:
[![Build Status](https://travis-ci.org/GavinNL/gcsg.svg?branch=master)](https://travis-ci.org/GavinNL/gcsg)
[![Build status](https://ci.appveyor.com/api/projects/status/88di7b89gyauvivj/branch/master?svg=true)](https://ci.appveyor.com/project/GavinNL/gcsg/branch/master)
[![codecov](https://codecov.io/gh/GavinNL/gcsg/branch/master/graph/badge.svg)](https://codecov.io/gh/GavinNL/gcsg)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c19827f8039d4dd09b4f0671d8edb5bf)](https://www.codacy.com/app/GavinNL/gcsg?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=GavinNL/gcsg&amp;utm_campaign=Badge_Grade)

Devlop:
[![Build Status](https://travis-ci.org/GavinNL/gcsg.svg?branch=dev)](https://travis-ci.org/GavinNL/gcsg)
[![Build status](https://ci.appveyor.com/api/projects/status/88di7b89gyauvivj/branch/dev?svg=true)](https://ci.appveyor.com/project/GavinNL/gcsg/branch/dev)
[![codecov](https://codecov.io/gh/GavinNL/gcsg/branch/dev/graph/badge.svg)](https://codecov.io/gh/GavinNL/gcsg)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c19827f8039d4dd09b4f0671d8edb5bf)](https://www.codacy.com/app/GavinNL/gcsg?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=GavinNL/gcsg&amp;utm_campaign=Badge_Grade)

## Building

Requirements:
 *  The conan package manager must be installed on your system.
 *  matplotlib, and numpy must be installed to plot the points

```C++
git clone https://github.com/gavinNL/gcsg.git
cd gscg
mkdir build
cd build
cmake ..
make

cp ../plot_points.py .
./bin/2d > points
python3 points
```

## Examples

### 2D

To plot the output of the 2d examples, a python script is provided. You will need
to to have matplotlib installed.

```
./2d > shape.txt
python3 plot_points shape.txt
```

### 3D

The 3d example provided prints out the solid as an stl document.

You can pipe the output to a file
```
./3d > solid.stl
```

And then use an online STL viewer to view it. A good one is at: [http://piscis.github.io/webgl-3d-viewer/example/](http://piscis.github.io/webgl-3d-viewer/example/)
