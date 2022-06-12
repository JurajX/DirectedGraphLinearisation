add_exe(
    NAME solution
    LINK_LIBRARIES fmt::fmt graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)

add_exe(
    NAME example_undirected_graph
    LINK_LIBRARIES fmt::fmt graph
    OPTIM_OPTIONS "-O3"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS_OFF
)

