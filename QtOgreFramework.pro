# This QMake file is not part of the official build system, and may
# not work. The official build system is based on CMake (not QMake). 
# Please see www.cmake.org and the CMakeLists.txt file.

# I can't support using the QMake system, but if you make any
# changes to improve it then I will be happy to include them.

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += QtOgre \
           Demo
