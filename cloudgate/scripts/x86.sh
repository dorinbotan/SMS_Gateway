#!/bin/bash
scripts/config.sh --disable TARGET_imx28
scripts/config.sh --enable TARGET_x86
make defconfig
