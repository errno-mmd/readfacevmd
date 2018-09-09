cmake_minimum_required(VERSION 2.8)

# modify if you installed OpenFace into a different directory
include_directories(/usr/local/include/OpenFace)
link_directories(/usr/local/lib)

find_package(BLAS REQUIRED)
include_directories(${BLAS_INCLUDE_DIRS})
link_directories(${BLAS_LIBRARY_DIRS})

find_package(OpenCV 3.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost 1.5.9 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS}/boost)
link_directories(${Boost_LIBRARY_DIRS})	

find_package(ICU REQUIRED COMPONENTS i18n uc)
# MESSAGE("ICU_INCLUDE_DIRS: ${ICU_INCLUDE_DIRS}")
# MESSAGE("ICU_LIBRARIES: ${ICU_LIBRARIES}")
# MESSAGE("ICU_LIBRARY_DIRS: ${ICU_LIBRARY_DIRS}")
include_directories(${ICU_INCLUDE_DIRS})
link_directories(${ICU_LIBRARY_DIRS})

find_package(PkgConfig REQUIRED)

pkg_check_modules(TBB tbb REQUIRED)
include_directories(${TBB_INCLUDE_DIRS})
link_directories(${TBB_LIBRARY_DIRS})

pkg_check_modules(Eigen3 eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

add_executable(readfacevmd readfacevmd.cc MMDFileIOUtil.cc VMD.cc smooth_reduce.cc smoothvmd.cc reducevmd.cc)
target_link_libraries(readfacevmd LandmarkDetector)
target_link_libraries(readfacevmd FaceAnalyser)
target_link_libraries(readfacevmd GazeAnalyser)
target_link_libraries(readfacevmd Utilities)
target_link_libraries(readfacevmd dlib)
target_link_libraries(readfacevmd ${OpenCV_LIBRARIES})
target_link_libraries(readfacevmd ${Boost_LIBRARIES})
target_link_libraries(readfacevmd ${TBB_LIBRARIES})
target_link_libraries(readfacevmd ${BLAS_LIBRARIES})
target_link_libraries(readfacevmd ${ICU_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

