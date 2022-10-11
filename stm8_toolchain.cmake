SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER sdcc)

SET(CMAKE_C_FLAGS_INIT "-mstm8 --opt-code-size --allow-unsafe-read")

SET(CMAKE_EXE_LINKER_FLAGS_INIT "")

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_SHARED_LIBRARY_PREFIX "")          # lib
set(CMAKE_SHARED_LIBRARY_SUFFIX ".lib")          # .so
set(CMAKE_IMPORT_LIBRARY_PREFIX )
set(CMAKE_IMPORT_LIBRARY_SUFFIX )
set(CMAKE_EXECUTABLE_SUFFIX ".ihx")          # intel hex file
set(CMAKE_LINK_LIBRARY_SUFFIX ".lib")
set(CMAKE_DL_LIBS "")

# find sdcclib as CMAKE_AR
# since cmake may already have searched for "ar", sdcclib has to
# be searched with a different variable name (SDCCLIB_EXECUTABLE)
# and must then be forced into the cache
get_filename_component(SDCC_LOCATION "${CMAKE_C_COMPILER}" PATH)
find_program(SDCCLIB_EXECUTABLE sdcclib PATHS "${SDCC_LOCATION}" NO_DEFAULT_PATH)
find_program(SDCCLIB_EXECUTABLE sdcclib)
set(CMAKE_AR "${SDCCLIB_EXECUTABLE}" CACHE FILEPATH "The sdcc librarian" FORCE)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_definitions(-D__SDCC__)
add_definitions(-D_SDCC_)

include_directories(/usr/share/sdcc/include)
