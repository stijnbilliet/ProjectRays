set(target "Game")

#search/group source file
file(GLOB_RECURSE SB_GAME_HEADER_FILES "*.h")
file(GLOB_RECURSE SB_GAME_SOURCE_FILES "*.[ch]pp")

#create target
add_executable(${target} ${SB_GAME_HEADER_FILES} ${SB_GAME_SOURCE_FILES})
add_dependencies(${target} Framework)
target_link_libraries(${target} PUBLIC Framework)

#link libraries
target_link_libraries(${target} PUBLIC ${ASSIMP_LIBRARIES})
target_link_libraries(${target} PUBLIC ${OpenCL_LIBRARIES})
target_link_libraries(${target} PUBLIC ${SDL_LIBRARIES})
target_link_libraries(${target} PUBLIC ${GLM_LIBRARIES})

target_precompile_headers(${target} PUBLIC "GamePCH.h")

if ( MSVC )
    include(sourcegroupbyfolder)
    source_group_by_folder(SB_GAME_HEADER_FILES SB_GAME_SOURCE_FILES ${target})

    set_target_properties( ${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/_Temp/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )
    set_target_properties( ${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/Binaries/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )
    set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/Binaries/${target}/${CMAKE_SYSTEM_NAME}/${CMAKE_GENERATOR_PLATFORM}" )

    set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${target})
endif ( MSVC )
