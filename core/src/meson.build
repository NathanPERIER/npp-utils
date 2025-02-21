
npputils_core_sources = [
	'colour/conv.cc',
	'colour/rgb.cc',
	'format/base64.cc',
	'format/templater.cc',
	'io/file.cc',
	'string/diff.cc'
]

npputils_core_lib = static_library(
    'npputils_core_static', npputils_core_sources,
    dependencies: [fmt_dep],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: npputils_core_headers_dir,
    install: false,
    build_by_default: true
)

npputils_core_dep = declare_dependency(
    link_with : npputils_core_lib,
    dependencies: [fmt_dep],
    include_directories : npputils_core_headers_dir
)
