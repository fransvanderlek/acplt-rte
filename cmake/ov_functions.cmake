
# Add a library target for an ACPLT/OV dynamic linkable library
# 
# Must be used together with ov_library_includes() to manage model include paths and add the custom_command for
# ov_codegen.
# 
# Arguments: 
#   * OV_LIBRARY_NAME: Name of the OV library (= name of the cmake target and name of the linked .so/.dll file)
#   * OV_CUSTOM_LIBOPEN (optional): If TRUE, the compiler define flag to override the default ov_library_open function
#       is added. It must be set, if a custom ov_library_open function is defined.
# 
# Usage:
#   add_ov_library(libname TRUE)
#   ov_library_includes(libname ov fb otherlibname)
function(add_ov_library OV_LIBRARY_NAME)

    # Define library target
    file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c)
    add_library(${OV_LIBRARY_NAME} SHARED
        ${SRC_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.c)

    target_include_directories(${OV_LIBRARY_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # For generated <library>.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/model>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:models>
        $<INSTALL_INTERFACE:include/${OV_LIBRARY_NAME}>)

    # Define option to let the user disable default build of this library
    option(BUILD_OV_LIBRARY_${OV_LIBRARY_NAME}
        "Compile ACPLT/OV library '${OV_LIBRARY_NAME}'"
        ON)
    # Remove targets from default build, if user wished
    if(NOT ${BUILD_OV_LIBRARY_${OV_LIBRARY_NAME}})
        set_target_properties(${OV_LIBRARY_NAME}
            PROPERTIES
                EXCLUDE_FROM_ALL 1
                EXCLUDE_FROM_DEFAULT_BUILD 1)
    endif()

    # Stop cmake from prepending "lib" to .so/.dll filename
    set_target_properties(${OV_LIBRARY_NAME} PROPERTIES PREFIX "")
    # Override definition for custom library_open function
    if(${ARGV1})
        target_compile_definitions(${OV_LIBRARY_NAME} PRIVATE -Dov_library_open_${OV_LIBRARY_NAME}=ov_library_open_${OV_LIBRARY_NAME}_old)
    endif()

    # If ov (the core library) is compiled within this cmake build, add this library's binary dir to its RPATH for dynamic linking
    if (TARGET ov)
        set_property(TARGET ov
            APPEND
            PROPERTY BUILD_RPATH ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    # Install rules
    install(TARGETS ${OV_LIBRARY_NAME}
        EXPORT ${PROJECT_NAME}
        # FIXME Don't install static libraries (.a) on Windows
        LIBRARY
            DESTINATION lib/
        RUNTIME
            DESTINATION lib/
        COMPONENT runtime
        OPTIONAL)

    # FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
    # install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.h
    #     DESTINATION include/${OV_LIBRARY_NAME}
    #     COMPONENT development
    # )
    # install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    #     DESTINATION include/${OV_LIBRARY_NAME}
    #     COMPONENT development
    #     FILES_MATCHING PATTERN *.h
    # )
    # install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/model/
    #     DESTINATION models
    #     COMPONENT development
    #     FILES_MATCHING PATTERN *.ovm PATTERN *.ovf PATTERN *.ovt
    # )
endfunction()

# Add OV library dependencies to an OV library, manage the library's model include paths and add the custom_command for
# ov_codegen
# 
# This command cannot be used for external libraries (like -lpthread). Use the usual `target_link_libraries` instead:
#   target_link_libraries(<libname> -lpthread)
# 
# Arguments:
#   * OV_LIBRARY_NAME: Name of the OV library
#   * [ARGN]: Names of OV libraries this library depends on
# 
# Usage:
#   add_ov_library(libname)
#   ov_library_includes(libname ov fb otherlibname)
function(ov_library_includes OV_LIBRARY_NAME)
    # Add dependencies as cmake link link_libraries
    target_link_libraries(${OV_LIBRARY_NAME} ${ARGN})

    # Gather model directories of dependency libraries
    foreach(_dependency ${ARGN})
        get_property(_model_path TARGET ${_dependency} PROPERTY OV_MODEL_INCLUDE_DIRECTORIES)
        list(APPEND OV_MODEL_PATHS ${_model_path})
    endforeach()
    list(APPEND OV_MODEL_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/model)
    # FIXME To fix importing of installed package, we need to use the following code for fixing the model paths upon
    # install:
    # list(APPEND OV_MODEL_PATHS
    #     $<INSTALL_INTERFACE:models>
    #     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/model>)
    # Unfortunately, this is forbidden by the EXPORT_PROPERTIES feature: https://gitlab.kitware.com/yrHeTaTeJlb/cmake/commit/6db61f072588bb097b16094290b0b415fa4905e5
    list(REMOVE_DUPLICATES OV_MODEL_PATHS)

    # Add custom command for generating the <library>.{h,c} files
    # For cross compilation, we expect the OV_CODEGEN_PATH variable to be set. Otherwise, we just take the ov_codegen binary that has been build (or imported) by this project
    set(OV_CODEGEN_PATH ""
        CACHE
        FILEPATH
        "Path of the ov_codegen binary (for cross compilation). If not given, the ov_codegen target of the RTE project is used.")
    if(OV_CODEGEN_PATH)
        add_custom_command (
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.h ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.c
            COMMAND "${OV_CODEGEN_PATH}" -f ${CMAKE_CURRENT_SOURCE_DIR}/model/${OV_LIBRARY_NAME}.ovm -o ${CMAKE_CURRENT_BINARY_DIR} -l ${OV_LIBRARY_NAME} "-I;$<JOIN:${OV_MODEL_PATHS},;-I;>"
            MAIN_DEPENDENCY model/${OV_LIBRARY_NAME}.ovm
            COMMENT "Generating OV library sources for library '${OV_LIBRARY_NAME}'"
            COMMAND_EXPAND_LISTS
        )
    else()
        add_custom_command (
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.h ${CMAKE_CURRENT_BINARY_DIR}/${OV_LIBRARY_NAME}.c
            COMMAND ov_codegen -f ${CMAKE_CURRENT_SOURCE_DIR}/model/${OV_LIBRARY_NAME}.ovm -o ${CMAKE_CURRENT_BINARY_DIR} -l ${OV_LIBRARY_NAME} "-I;$<JOIN:${OV_MODEL_PATHS},;-I;>"
            MAIN_DEPENDENCY model/${OV_LIBRARY_NAME}.ovm
            COMMENT "Generating OV library sources for library '${OV_LIBRARY_NAME}'"
            COMMAND_EXPAND_LISTS
        )
    endif()
    

    # Export (transitive) ov_codegen model include paths
    set_property(TARGET ${OV_LIBRARY_NAME}
        PROPERTY OV_MODEL_INCLUDE_DIRECTORIES ${OV_MODEL_PATHS})
    set_property(TARGET ${OV_LIBRARY_NAME}
        APPEND
        PROPERTY EXPORT_PROPERTIES OV_MODEL_INCLUDE_DIRECTORIES)
endfunction()