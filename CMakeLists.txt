cmake_minimum_required(VERSION 3.12)

project(midnight)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include include/glad/include ${GLFW3_INCLUDE_DIRS})

set(GLAD_SRC include/glad/src/glad.c)
file(GLOB_RECURSE SOURCES "src/*.c")
set(ALL_SOURCES ${GLAD_SRC} ${SOURCES})

add_library(midnight STATIC ${ALL_SOURCES})
target_link_libraries(midnight PRIVATE ${GLFW3_LIBRARIES} OpenGL::OpenGL m)
target_compile_options(midnight PRIVATE -Wall -Wextra ${GLFW3_CFLAGS_OTHER})

file(GLOB DEMO_DIRS "demos/*")

foreach(DEMO_DIR ${DEMO_DIRS})
  if(IS_DIRECTORY ${DEMO_DIR})
    file(GLOB DEMO_SOURCES "${DEMO_DIR}/main.c")
    if(DEMO_SOURCES)
      get_filename_component(DEMO_NAME ${DEMO_DIR} NAME)
      add_executable(${DEMO_NAME} ${DEMO_SOURCES})
      target_link_libraries(${DEMO_NAME} midnight ${GLFW3_LIBRARIES} OpenGL::OpenGL m)
      target_compile_options(${DEMO_NAME} PRIVATE -Wall -Wextra ${GLFW3_CFLAGS_OTHER})
      include_directories(${DEMO_NAME} include include/glad/include ${GLFW3_INCLUDE_DIRS})

      file(GLOB DEMO_RESOURCES "${DEMO_DIR}/*")
      foreach(RESOURCE ${DEMO_RESOURCES})
        if(NOT IS_DIRECTORY ${RESOURCE} AND NOT "${RESOURCE}" MATCHES ".*main.c$")
          get_filename_component(RESOURCE_NAME ${RESOURCE} NAME)
          add_custom_command(
            TARGET ${DEMO_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RESOURCE}" $<TARGET_FILE_DIR:${DEMO_NAME}>/${RESOURCE_NAME}
            COMMENT "Copying ${RESOURCE_NAME} to $<TARGET_FILE_DIR:${DEMO_NAME}>"
          )
        endif()
      endforeach()

    endif()
  endif()
endforeach()
