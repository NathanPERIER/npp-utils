
npputils_solver_sources = []

npputils_solver_lib = static_library(
    'npputils_solver_static', npputils_solver_sources,
    dependencies: [npputils_core_dep],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: npputils_solver_headers_dir,
    install: false,
    build_by_default: true
)

npputils_solver_dep = declare_dependency(
    link_with : npputils_solver_lib,
    dependencies: [npputils_core_dep],
    include_directories : npputils_solver_headers_dir
)
