# Install script for directory: /Users/chajunho/Desktop/Programming/Git/GitHub/TGON/Source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/openal-soft-1.20.1/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/gainput-1.0.0/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/fmt-6.1.2/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/freetype-2.10.1/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/glew-2.1.0/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/icu4c-64_2/icu/cmake_install.cmake")
  include("/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/Source/ThirdParty/zlib-1.2.11/cmake_install.cmake")

endif()

