add_library(STM8S_SPL INTERFACE)
target_include_directories(STM8S_SPL INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/inc
        )

file(GLOB STM8S_SPL_src "${CMAKE_CURRENT_LIST_DIR}/src/stm8s_*.c")
foreach(src_file ${STM8S_SPL_src})
    string(REGEX REPLACE "^.*_(.*)\\.c$" "\\1" component ${src_file})
    add_library(STM8S_SPL_${component} INTERFACE)
    target_sources(STM8S_SPL_${component} INTERFACE ${src_file})
    add_library(STM8S_SPL::${component} ALIAS STM8S_SPL_${component})
endforeach()
