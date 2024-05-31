cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("libtinyfd")

add_library(libtinyfd STATIC "src/tinyfiledialogs.c")

target_include_directories(libtinyfd PUBLIC "inc/")