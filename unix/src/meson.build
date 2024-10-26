
npputils_unix_sources = [
	'program/environ.cc',
	'system/errno.cc',
	'system/path.cc',
	'system/subprocess.cc'
]

npputils_unix_lib = static_library(
    'npputils_unix_static', npputils_unix_sources,
    dependencies: [npputils_core_dep],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: npputils_unix_headers_dir,
    install: false,
    build_by_default: true
)

npputils_unix_dep = declare_dependency(
    link_with : npputils_unix_lib,
    dependencies: [npputils_core_dep],
    include_directories : npputils_unix_headers_dir
)
