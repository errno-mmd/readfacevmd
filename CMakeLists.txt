cmake_minimum_required(VERSION 3.8)
project(readfacevmd)

set(CMAKE_CXX_STANDARD 17)

# modify if you installed OpenFace into a different directory
include_directories(/usr/local/include/OpenFace)
link_directories(/usr/local/lib)

find_package(OpenCV 4.1 REQUIRED COMPONENTS core imgproc calib3d highgui objdetect)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost 1.5.9 REQUIRED COMPONENTS filesystem system program_options)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS}/boost)
link_directories(${Boost_LIBRARY_DIRS})	

find_package(ICU REQUIRED COMPONENTS i18n uc)
include_directories(${ICU_INCLUDE_DIRS})
link_directories(${ICU_LIBRARY_DIRS})

find_package(PkgConfig REQUIRED)

pkg_check_modules(Eigen3 eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

pkg_check_modules(OpenBLAS openblas REQUIRED)
include_directories(${OpenBLAS_INCLUDE_DIRS})
link_directories(${OpenBLAS_LIBRARY_DIRS})

add_executable(readfacevmd readfacevmd_main.cc readfacevmd.cc MMDFileIOUtil.cc VMD.cc smooth_reduce.cc smoothvmd.cc reducevmd.cc morph_name.cc interpolate.cc fpschanger.cc refine.cc MyGazeEstimation.cc)
target_link_libraries(readfacevmd LandmarkDetector)
target_link_libraries(readfacevmd FaceAnalyser)
target_link_libraries(readfacevmd GazeAnalyser)
target_link_libraries(readfacevmd Utilities)
target_link_libraries(readfacevmd dlib)
target_link_libraries(readfacevmd pthread)
target_link_libraries(readfacevmd ${OpenCV_LIBRARIES})
target_link_libraries(readfacevmd ${Boost_LIBRARIES})
target_link_libraries(readfacevmd ${OpenBLAS_LIBRARIES})
target_link_libraries(readfacevmd ${ICU_LIBRARIES})

add_executable(mergevmd mergevmd.cc MMDFileIOUtil.cc VMD.cc reducevmd.cc interpolate.cc)
target_link_libraries(mergevmd ${ICU_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall")
