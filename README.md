# gcsg

Master:
![Travis (.org) branch](https://img.shields.io/travis/GavinNL/gcsg/master.svg)
![AppVeyor branch](https://img.shields.io/appveyor/ci/GavinNL/gcsg/master.svg)
![Codecov branch](https://img.shields.io/codecov/c/github/GavinNL/gcsg/master.svg)
![Codacy branch grade](https://img.shields.io/codacy/grade/c19827f8039d4dd09b4f0671d8edb5bf/master.svg)

Dev:
![Travis (.org) branch](https://img.shields.io/travis/GavinNL/gcsg/dev.svg)
![AppVeyor branch](https://img.shields.io/appveyor/ci/GavinNL/gcsg/dev.svg)
![Codecov branch](https://img.shields.io/codecov/c/github/GavinNL/gcsg/dev.svg)
![Codacy branch grade](https://img.shields.io/codacy/grade/c19827f8039d4dd09b4f0671d8edb5bf/dev.svg)

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
