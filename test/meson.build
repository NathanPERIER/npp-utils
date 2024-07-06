
test_source_files = [
    'hexdump.cc',
    'type_name.cc',
    'format/csv.cc',
    'string/split.cc'
]


foreach main_path : test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [cpputils_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
