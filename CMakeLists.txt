cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(D4n1xOS_CYD_ESP32-2432S028_FreeRTOS)

# Add the gpio.h component from GitHub
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES lvgl lvgl_helpers XPT2046
    GIT_REPOSITORIES "https://github.com/espressif/esp-idf.git"
    GIT_TAG "v4.4"
    COMPONENTS "driver"
)