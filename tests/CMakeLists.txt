add_exe(
    TEST
    NAME test_undirected_graph
    SRCS ${CMAKE_CURRENT_SOURCE_DIR}/resources/test_data.hpp
    LINK_LIBRARIES graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)

add_exe(
    TEST
    NAME test_double_graph
    SRCS ${CMAKE_CURRENT_SOURCE_DIR}/resources/test_data.hpp
    LINK_LIBRARIES graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)
