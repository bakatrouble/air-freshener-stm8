cmake_minimum_required(VERSION 3.0)

project(stm8test C)
add_subdirectory(StdPeriph/STM8S/Libraries/STM8S_StdPeriph_Driver)

add_executable(${CMAKE_PROJECT_NAME}
        src/main.c
        src/time.c
        src/motor.c
        src/console_io.c
        src/config.c
        )
target_link_libraries(${CMAKE_PROJECT_NAME}
        STM8S_SPL
        STM8S_SPL::gpio
        STM8S_SPL::tim2
        STM8S_SPL::exti
        STM8S_SPL::clk
        STM8S_SPL::uart1
        STM8S_SPL::adc1
        STM8S_SPL::flash
        )
add_definitions(-DSTM8S003)

add_custom_target(
        Flash ALL
        COMMAND ./flash.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(Flash ${CMAKE_PROJECT_NAME})
