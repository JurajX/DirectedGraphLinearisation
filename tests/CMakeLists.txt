add_exe(
    TEST
    NAME test_undirected_graph
    LINK_LIBRARIES graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)

add_exe(
    TEST
    NAME test_double_graph
    LINK_LIBRARIES graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)
