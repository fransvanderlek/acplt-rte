# General OV preprocessor definitions for OV and all libs
if (WIN32)
    add_definitions(-DOV_SYSTEM_NT=1 -DPLT_SYSTEM_NT=1 -DFD_SETSIZE=128 -DWIN32)
elseif(APPLE)
    message(FATAL_ERROR "Building ACPLT/OV on Mac OS is not supported.")
elseif(CYGWIN)
    add_definitions(-DOV_SYSTEM_CYGWIN=1)
else()
    add_definitions(-DOV_SYSTEM_LINUX=1)
endif()
add_compile_options($<$<CONFIG:Debug>:-DOV_DEBUG=1>)

# General compiler options for OV and all libs
add_compile_options(-Wall)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wno-unused-parameter -Winit-self  -Wno-attributes)
endif()
if(OV_STATIC_LIBRARIES)
    add_compile_definitions(OV_STATIC_LIBRARIES)
else()
    add_compile_definitions(OV_DYNAMIC_LIBRARIES)
endif()
