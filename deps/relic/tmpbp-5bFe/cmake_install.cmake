# Install script for directory: /my/openabe-master/deps/relic/relic-toolkit-0.5.0

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/my/openabe-master/deps/root")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/relic" TYPE FILE FILES
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_arch.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_bc.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_bench.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_bn.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_core.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_cp.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_dv.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_eb.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_ec.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_ed.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_ep.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_epx.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_err.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_fb.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_fbx.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_fp.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_fpx.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_label.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_md.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_pc.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_pp.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_rand.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_test.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_trace.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_types.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/relic_util.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/relic/low" TYPE FILE FILES
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/low/relic_bn_low.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/low/relic_dv_low.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/low/relic_fb_low.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/low/relic_fp_low.h"
    "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/include/low/relic_fpx_low.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/relic" TYPE DIRECTORY FILES "/my/openabe-master/deps/relic/tmpbp-5bFe/include/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/my/openabe-master/deps/relic/relic-toolkit-0.5.0/cmake/relic-config.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/my/openabe-master/deps/relic/tmpbp-5bFe/src/cmake_install.cmake")
  include("/my/openabe-master/deps/relic/tmpbp-5bFe/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/my/openabe-master/deps/relic/tmpbp-5bFe/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
