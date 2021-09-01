#!/bin/sh

rm -f /dev/mymodule*

mknod /dev/mymodule0 c $1 0
chmod 666 /dev/mymodule0

mknod /dev/mymodule1 c $1 1
chmod 666 /dev/mymodule1

mknod /dev/mymodule2 c $1 2
chmod 666 /dev/mymodule2