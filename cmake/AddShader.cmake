# This CMake macro creates a target to compile shaders.
# The target will have file-level dependencies in the source-tree.
# The target will write the compiled shader output into ${CMAKE_CURRENT_BUILD_DIR}.
# The output file will be the name of the target with the .spv file extension added.
# The output file will be installed to ${CMAKE_INSTALL_PREFIX}/shaders/.
# Note: If two shader targets share the same name,
#       one of the output files will overwrite the other upon installation.
# Note: This macro requires that the shader_processor CMake executable is available.

include(${CMAKE_CURRENT_LIST_DIR}/FindShaderCompiler.cmake)

macro(add_shader)
    find_shader_compiler()
    set(target_name ${ARGV0})
    set(source_file ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1})
    set(output_source "${ARGV0}.cpp")
    set(output_header_dir "include/${target_name}")
    set(output_header "${output_header_dir}/${ARGV0}.hpp")
    add_custom_command(OUTPUT ${output_source} ${output_header}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${output_header_dir}
        COMMAND ${SHADER_COMPILER} ${source_file} -o ${target_name}.bin
        COMMAND shader_processor -i ${target_name}.bin -o ${output_source} -h ${output_header} -n ${target_name}
        DEPENDS shader_processor ${source_file})
    add_library(${target_name} ${output_source})
    target_include_directories(${target_name}
        PUBLIC $<INSTALL_INTERFACE:include>
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)
endmacro()
