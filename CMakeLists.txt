cmake_minimum_required(VERSION 3.15)
project(ADS_Project1)

set(CMAKE_CXX_STANDARD 11)
## lsh
add_executable(ADS_Project1 src/mainLsh.cpp inc/FunctionH.h src/util2.cpp
        inc/util2.h src/util.cpp inc/util.h inc/LSH_HT.h inc/LSH.h inc/KNeighborsClassifier.h)
#cube
#add_executable(ADS_Project1 src/mainHypercube.cpp inc/FunctionH.h src/util2.cpp
#        inc/util2.h src/util.cpp inc/util.h inc/Hypercube_HT.h inc/Hypercube.h inc/KNeighborsClassifier.h)
#curve Projection Cube
#add_executable(ADS_Project1 src/mainCurveProjectionHypercube.cpp inc/ColorMod.h inc/FunctionH.h src/util2.cpp
#        inc/util2.h src/util.cpp inc/util.h inc/Hypercube_HT.h inc/Hypercube.h inc/KNeighborsClassifier.h src/Pathfinder.cpp)
##curve Projection LSH
#add_executable(ADS_Project1 src/mainCurveProjectionLsh inc/FunctionH.h src/util2.cpp
#        inc/util2.h src/util.cpp inc/util.h inc/Hypercube_HT.h inc/Hypercube.h inc/KNeighborsClassifier.h)
##curve grid Cube
#add_executable(ADS_Project1 src/mainCurveGridHypercube.cpp inc/FunctionH.h src/util2.cpp
#        inc/util2.h src/util.cpp inc/util.h inc/LSH_HT.h inc/LSH.h inc/KNeighborsClassifier.h)
###curve grid LSH
#add_executable(ADS_Project1 src/mainCurveGridLsh.cpp inc/FunctionH.h src/util2.cpp
#        inc/util2.h src/util.cpp inc/util.h inc/LSH_HT.h inc/LSH.h inc/KNeighborsClassifier.h)

