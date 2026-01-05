# Install script for directory: /workspace

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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "dlpack-LICENSE" FILES "/workspace/3rdparty/dlpack/LICENSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippicv-readme.htm" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/icv/readme.htm")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippicv-EULA.txt" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/EULA.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippicv-third-party-programs.txt" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/third-party-programs.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippiw-support.txt" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/icv/../iw/../support.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippiw-third-party-programs.txt" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/icv/../iw/../third-party-programs.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ippiw-EULA.txt" FILES "/workspace/build-pr-check/3rdparty/ippicv/ippicv_lnx/icv/../iw/../EULA.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "flatbuffers-LICENSE.txt" FILES "/workspace/3rdparty/flatbuffers/LICENSE.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "opencl-headers-LICENSE.txt" FILES "/workspace/3rdparty/include/opencl/LICENSE.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "licenses" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/opencv4" TYPE FILE RENAME "ade-LICENSE" FILES "/workspace/build-pr-check/3rdparty/ade/ade-0.1.2e/LICENSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv4/opencv2" TYPE FILE FILES "/workspace/build-pr-check/cvconfig.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv4/opencv2" TYPE FILE FILES "/workspace/build-pr-check/opencv2/opencv_modules.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4/OpenCVModules.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4/OpenCVModules.cmake"
         "/workspace/build-pr-check/CMakeFiles/Export/51ea738ee2ea68756d9122094dacc2a4/OpenCVModules.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4/OpenCVModules-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4/OpenCVModules.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4" TYPE FILE FILES "/workspace/build-pr-check/CMakeFiles/Export/51ea738ee2ea68756d9122094dacc2a4/OpenCVModules.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4" TYPE FILE FILES "/workspace/build-pr-check/CMakeFiles/Export/51ea738ee2ea68756d9122094dacc2a4/OpenCVModules-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/opencv4" TYPE FILE FILES
    "/workspace/build-pr-check/unix-install/OpenCVConfig-version.cmake"
    "/workspace/build-pr-check/unix-install/OpenCVConfig.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "scripts" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/workspace/build-pr-check/CMakeFiles/install/setup_vars_opencv4.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencv4" TYPE FILE FILES
    "/workspace/platforms/scripts/valgrind.supp"
    "/workspace/platforms/scripts/valgrind_3rdparty.supp"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/workspace/build-pr-check/3rdparty/libjpeg-turbo/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/libtiff/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/libwebp/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/openjpeg/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/libpng/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/openexr/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/ippiw/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/protobuf/cmake_install.cmake")
  include("/workspace/build-pr-check/hal/ipp/cmake_install.cmake")
  include("/workspace/build-pr-check/3rdparty/ittnotify/cmake_install.cmake")
  include("/workspace/build-pr-check/include/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/calib3d/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/core/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/dnn/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/features2d/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/flann/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/gapi/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/highgui/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/imgcodecs/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/imgproc/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/java/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/js/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/ml/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/objc/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/objdetect/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/photo/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/python/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/stitching/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/ts/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/video/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/videoio/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/.firstpass/world/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/core/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/flann/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/imgproc/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/ml/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/photo/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/python_tests/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/dnn/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/features2d/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/imgcodecs/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/videoio/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/calib3d/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/highgui/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/objdetect/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/stitching/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/video/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/gapi/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/java_bindings_generator/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/js_bindings_generator/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/objc_bindings_generator/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/python_bindings_generator/cmake_install.cmake")
  include("/workspace/build-pr-check/modules/java/cmake_install.cmake")
  include("/workspace/build-pr-check/doc/cmake_install.cmake")
  include("/workspace/build-pr-check/data/cmake_install.cmake")
  include("/workspace/build-pr-check/apps/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/workspace/build-pr-check/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
