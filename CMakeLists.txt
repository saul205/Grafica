cmake_minimum_required(VERSION 3.10)
project(PathTracer CXX)

set(CMAKE_BUILD_TYPE DEBUG)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -pthread")

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/lib/Basics
                    ${PROJECT_SOURCE_DIR}/lib/GeometricFigures
                    ${PROJECT_SOURCE_DIR}/lib/Imaging
                    ${PROJECT_SOURCE_DIR}/lib/Sensor
                    ${PROJECT_SOURCE_DIR}/lib/Structures
                    )

set(SRCS_PATHTRACER_MODULES
   lib/Basics/DotDir.cpp
   lib/Basics/Ray.cpp
   lib/Basics/Transformation.cpp
   lib/GeometricFigures/Figure.cpp
   lib/GeometricFigures/LightSource.cpp
   lib/GeometricFigures/Plane.cpp
   lib/GeometricFigures/Sphere.cpp
   lib/GeometricFigures/Triangle.cpp
   lib/GeometricFigures/TriangleMesh.cpp
   lib/Imaging/Image.cpp
   lib/Imaging/Lab.cpp
   lib/Imaging/Rgb.cpp
   lib/Imaging/ToneMapper.cpp
   lib/Sensor/Sensor.cpp
   lib/Structures/BoundingBox.cpp
   lib/Structures/BoundingVolume.cpp
   lib/Structures/ConcurrentQueue.cpp
   lib/Structures/Scene.cpp
)

add_executable(pathTracer ${PROJECT_SOURCE_DIR}/practica3/main.cpp ${SRCS_PATHTRACER_MODULES})

