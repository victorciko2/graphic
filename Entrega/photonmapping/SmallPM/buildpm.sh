#!/bin/bash

g++ -std=c++11 src/main.cpp src/PhotonMapping.cpp src/RenderEngine.cpp smallrt/lib/smallrt.a -Ismallrt/include -Iinclude/ -o smallpm
