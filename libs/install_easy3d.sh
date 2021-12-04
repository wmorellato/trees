#!/bin/bash

git clone git@github.com:LiangliangNan/Easy3D.git
cd Easy3D
cmake -B ./build && cd build
cmake --build .
