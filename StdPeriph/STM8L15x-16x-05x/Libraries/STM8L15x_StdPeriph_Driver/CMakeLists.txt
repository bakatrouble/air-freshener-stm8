add_library(STM8L15X_SPL INTERFACE)
target_include_directories(STM8L15X_SPL INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/inc
        )

file(GLOB STM8L15X_SPL_src "${CMAKE_CURRENT_LIST_DIR}/src/stm8l15x_*.c")
foreach(src_file ${STM8L15X_SPL_src})
    string(REGEX REPLACE "^.*_(.*)\\.c$" "\\1" component ${src_file})
    add_library(STM8L15X_SPL_${component} INTERFACE)
    target_sources(STM8L15X_SPL_${component} INTERFACE ${src_file})
    add_library(STM8L15X_SPL::${component} ALIAS STM8L15X_SPL_${component})
endforeach()
