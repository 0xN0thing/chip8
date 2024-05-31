cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("glad")

add_library(glad STATIC "src/gl.c")

target_include_directories(glad PUBLIC "inc/")