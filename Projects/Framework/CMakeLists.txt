set(target "Framework")

file(GLOB_RECURSE SB_ENGINE_HEADER_FILES "*.h")
file(GLOB_RECURSE SB_ENGINE_SOURCE_FILES "*.[ch]pp")

add_library(${target} ${SB_ENGINE_HEADER_FILES} ${SB_ENGINE_SOURCE_FILES})
add_dependencies(${target} RadeonRaysSDK)
target_precompile_headers(${target} PUBLIC "FrameworkPCH.h")

if ( MSVC )
    include(sourcegroupbyfolder)
    source_group_by_folder(SB_ENGINE_HEADER_FILES SB_ENGINE_SOURCE_FILES ${target})
    set_target_properties( ${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/_Temp/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )
    set_target_properties( ${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )
    set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )
    # etc for the other available configuration types (MinSizeRel, RelWithDebInfo)
endif ( MSVC )
