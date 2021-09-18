# This CMake macro exports SHADER_COMPILER if the program is found.
# Otherwise, the macro will raise an error and prevent the CMake configuration step from continuing.

macro(find_shader_compiler)
    find_program(SHADER_COMPILER NAMES glslc)
    if(NOT EXISTS ${SHADER_COMPILER})
        message(FATAL_ERROR "Cannot find shader compiler program: ${SHADER_COMPILER}")
    endif()
endmacro()
