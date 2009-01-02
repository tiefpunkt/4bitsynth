#!/bin/bash

make -f Makefile.square clean
make -f Makefile.triangle clean
make -f Makefile.noise clean

make -f Makefile.square hex
make -f Makefile.triangle hex
make -f Makefile.noise hex
