
json_test_source_files = [
    'json/convert_addons.cc',
    'json/convert_base.cc',
    'json/reader.cc'
]


foreach main_path : json_test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [npputils_json_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
