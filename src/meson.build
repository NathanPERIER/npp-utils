
cpp_flags = [
    cpp_warning_flags
]

l_args = []


sources = [
	'string/lexer.cc'
]

headers = [
    cpputils_headers_dir
]


cpputils_lib = static_library(
    'cpputils_static', sources,
    dependencies: [fmt_dep],
    cpp_args: cpp_flags,
    link_args: l_args,
    include_directories: headers,
    install: false,
    build_by_default: true
)

cpputils_dep = declare_dependency(
    link_with : cpputils_lib,
    dependencies: [fmt_dep],
    include_directories : headers
)
