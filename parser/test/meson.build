
parser_test_source_files = [
    'string/lexer.cc',
]


foreach main_path : parser_test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [npputils_parser_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
