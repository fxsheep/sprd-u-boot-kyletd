#!/bin/bash
export CROSS_COMPILE=/opt/toolchains/arm-eabi-4.4.3/bin/arm-eabi-
export ARCH=arm
make distclean
make kyletd_config
make -j16
make -j16 fdl2
