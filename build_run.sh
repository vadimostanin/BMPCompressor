#!/bin/bash
cmake . && make
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/
./BMPCompressorUI -dir `pwd` -ext \*.bmp -ext \*.barch -ext \*.png
