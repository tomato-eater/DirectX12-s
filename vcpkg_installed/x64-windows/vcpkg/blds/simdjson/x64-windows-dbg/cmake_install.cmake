# Install script for directory: C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/src/v4.6.7-5b0676bd97.clean

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/pkgs/simdjson_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "simdjson_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/src/v4.6.7-5b0676bd97.clean/singleheader/simdjson.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "simdjson_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/simdjson.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "simdjson_Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/simdjson.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "simdjson_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson" TYPE FILE FILES
    "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/simdjson-config.cmake"
    "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/simdjson-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "simdjson_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson/simdjsonTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson/simdjsonTargets.cmake"
         "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/CMakeFiles/Export/e9be1f3bf2ac05e81f5c4d20ad32d021/simdjsonTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson/simdjsonTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson/simdjsonTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson" TYPE FILE FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/CMakeFiles/Export/e9be1f3bf2ac05e81f5c4d20ad32d021/simdjsonTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/simdjson" TYPE FILE FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/CMakeFiles/Export/e9be1f3bf2ac05e81f5c4d20ad32d021/simdjsonTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/simdjson.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
  file(WRITE "C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/x64-windows-dbg/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
