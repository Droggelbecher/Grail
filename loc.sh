#!/bin/sh

LOC_LIB=$(cat $(find lib/ -name '*.cc' -or -name '*.h')|wc -l)
LOC_LIB=$(cat $(find runtime/ -name '*.cc' -or -name '*.h')|wc -l)

