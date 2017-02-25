cmake_minimum_required(VERSION 2.8.3)
project(smtu_uauv)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
find_package(catkin REQUIRED COMPONENTS
  roscpp
  gazebo_ros
  std_msgs
  std_srvs
  gazebo_msgs
  geometry_msgs
  sensor_msgs
  urdf
  control_toolbox
  rostime
  nav_msgs
  joy
)

find_package(gazebo REQUIRED)
find_package(Eigen3)
if(NOT EIGEN3_FOUND)
  # Fallback to cmake_modules
  find_package(cmake_modules REQUIRED)
  find_package(Eigen REQUIRED)
  set(EIGEN3_INCLUDE_DIRS ${EIGEN_INCLUDE_DIRS})
else()
  set(EIGEN3_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
endif()

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES ${PROJECT_NAME} 
 CATKIN_DEPENDS
    roscpp
    std_msgs
    std_srvs
    gazebo_ros
    gazebo_msgs
    geometry_msgs
    sensor_msgs
    control_toolbox
    rostime
    nav_msgs
)

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})

# world plugin to simulate buoyancy and viscous force
add_library(smtu_freefloating_gazebo_fluid src/freefloating_gazebo_fluid.cpp include/freefloating_gazebo_fluid.h)
target_link_libraries(smtu_freefloating_gazebo_fluid ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

# model plugin subscribes to body and joint efforts and applies them in Gazebo
add_library(smtu_freefloating_gazebo_control src/freefloating_gazebo_control.cpp include/freefloating_gazebo_control.h)
target_link_libraries(smtu_freefloating_gazebo_control ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

# teleoperation node
add_executable(teleop src/teleop.cpp)
target_link_libraries(teleop ${catkin_LIBRARIES})
