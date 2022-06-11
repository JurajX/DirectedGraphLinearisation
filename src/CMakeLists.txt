set(
    HDRS
    ${CMAKE_SOURCE_DIR}/src/undirected_graph.hpp
    ${CMAKE_SOURCE_DIR}/src/double_graph.hpp
)

install(
    FILES ${HDRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT development
)

add_library(
    graph SHARED
    ${CMAKE_SOURCE_DIR}/src/undirected_graph.cpp
    ${CMAKE_SOURCE_DIR}/src/double_graph.cpp
    ${CMAKE_SOURCE_DIR}/src/undirected_graph-impl.hpp
    ${CMAKE_SOURCE_DIR}/src/double_graph-impl.hpp
    ${HDRS}
)
add_compile_options(
    TARGET graph
    OPTIM_OPTIONS "-O3"
)

set_target_properties(
    graph PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    graph
    PUBLIC
    fmt::fmt
)

target_include_directories(
    graph PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
)
