#!/bin/sh

set -e

check_program () {
    if [ -z "`which $1 2>/dev/null`" ]; then
        echo "Could not find $1 program: aptitude install $2"
        exit 1
    fi
}

mkdir -p m4

check_program automake automake
check_program autoreconf autoconf
check_program libtool libtool
check_program pkg-config pkg-config

echo "Setting up build system..."
autoreconf -f -i -s
