find_package(OpenCV REQUIRED)

add_executable(camera_sensor
        src/main.cpp
        src/camera_driver.cpp
)

target_include_directories(camera_sensor
        PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        include
)

target_link_libraries(camera_sensor
        ${OpenCV_LIBS}
)