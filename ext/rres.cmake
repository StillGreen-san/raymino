cmake_minimum_required(VERSION 3.20)

project(rres C)

add_library(rres)
target_sources(rres PRIVATE
        rres/src/rres-raylib.h
        rres/src/external/aes.c
        rres/src/external/lz4.c
        rres/src/external/monocypher.c
        )
target_compile_definitions(rres PRIVATE
        RRES_RAYLIB_IMPLEMENTATION
        RRES_SUPPORT_COMPRESSION_LZ4
        RRES_SUPPORT_ENCRYPTION_AES
        RRES_SUPPORT_ENCRYPTION_XCHACHA20
        )
target_include_directories(rres PUBLIC rres/src)
target_link_libraries(rres PUBLIC raylib)
