cmake_minimum_required(VERSION 3.15)

set(EXEC_NAME nscan)

add_executable(${EXEC_NAME})

set_target_properties(${EXEC_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/${EXEC_NAME}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/${EXEC_NAME}

  AUTOMOC ON
)

set(SCANNER_SRC include/scanner/scanner.hpp src/scanner/scanner.cpp)

set(NMAP_SRC
  include/nmap/definitions.hpp src/nmap/definitions.cpp
  include/nmap/parser.hpp src/nmap/parser.cpp

  include/nmap/detail/definitions_impl.hpp
  include/nmap/detail/parser_impl.hpp src/nmap/detail/parser_impl.cpp
)

set(SRC
  src/main.cpp
  ${NMAP_SRC}
  ${SCANNER_SRC}
)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Boost REQUIRED)

target_include_directories(${EXEC_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)

target_sources(${EXEC_NAME} PRIVATE ${SRC})

target_link_libraries(${EXEC_NAME}
  PRIVATE
    Qt5::Core
    ${Boost_LIBRARIES}
)