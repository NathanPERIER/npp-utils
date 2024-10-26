
npputils_parser_sources = [
	'string/lexer.cc'
]

npputils_parser_lib = static_library(
    'npputils_parser_static', npputils_parser_sources,
    dependencies: [npputils_core_dep],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: npputils_perser_headers_dir,
    install: false,
    build_by_default: true
)

npputils_parser_dep = declare_dependency(
    link_with : npputils_parser_lib,
    dependencies: [npputils_core_dep],
    include_directories : npputils_perser_headers_dir
)
