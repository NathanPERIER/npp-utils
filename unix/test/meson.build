
unix_test_source_files = [
    'program/environ.cc',
    'system/errno.cc',
    'system/path.cc'
]


foreach main_path : unix_test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [npputils_unix_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
