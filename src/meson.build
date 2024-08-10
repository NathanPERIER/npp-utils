
cpp_flags = [
    cpp_warning_flags
]

l_args = []


sources = [
	'string/diff.cc',
	'string/lexer.cc',
	'system/environ.cc',
	'system/errno.cc'
]

headers = [
    npputils_headers_dir
]


npputils_lib = static_library(
    'npputils_static', sources,
    dependencies: [fmt_dep],
    cpp_args: cpp_flags,
    link_args: l_args,
    include_directories: headers,
    install: false,
    build_by_default: true
)

npputils_dep = declare_dependency(
    link_with : npputils_lib,
    dependencies: [fmt_dep],
    include_directories : headers
)
