#!/bin/bash
# A buildscript that is compatible with cate. Written by Latte
build() {
    for file in "$@"; do
        obj_name="${file%%.c*}"
        obj_name="${obj_name/\//"_"}.o"

        if [ $file -nt $build_folder/$obj_name ]; then
            $CC $file $cflags -c -o $build_folder/$obj_name &
        fi
    done
    wait
}

build_folder="build"
CC="cc"
cflags="-Iinclude"
out_name="piagn"
mkdir -p $build_folder 

build src/main.c src/etc.c src/instructions.c
wait
$CC $build_folder/*.o -o $out_name