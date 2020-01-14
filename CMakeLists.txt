cmake_minimum_required(VERSION 3.8)

project(ThousandParty)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THOUSAND_PARTY_PATH ${CMAKE_CURRENT_LIST_DIR})
set(THOUSAND_PARTY_SOURCE_PATH ${THOUSAND_PARTY_PATH}/Source)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-x objective-c++")

function (init_thousand_party)
    set(GLOB_AGRUMENTS Source/*.cpp Source/*.c Source/*.h)
    file(GLOB_RECURSE SRC_PATH_LIST ${GLOB_AGRUMENTS})

    if (WIN32)
        add_executable(ThousandParty WIN32 ${SRC_PATH_LIST})
    else ()
        add_executable(ThousandParty ${SRC_PATH_LIST})
    endif ()
    source_group(TREE ${THOUSAND_PARTY_PATH} FILES ${THOUSAND_PARTY_SOURCE_PATH})
    init_thousand_party_properties()
endfunction ()

function (init_thousand_party_properties)
    set_target_properties(ThousandParty PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        FOLDER ThousandParty
    )
endfunction ()

function (init_tgon)
    add_subdirectory(../TGON "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TGON")
endfunction ()

init_thousand_party()
init_tgon()

target_link_libraries(ThousandParty PUBLIC TGON)
