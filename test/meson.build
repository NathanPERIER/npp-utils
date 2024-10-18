
test_source_files = [
    'hexdump.cc',
    'type_name.cc',
    'collections/pre_hashed.cc',
    'conv/integer.cc',
    'format/csv.cc',
    'math/complex.cc',
    'math/fraction.cc',
    'math/interval.cc',
    'math/vec2.cc',
    'memory/copy_ptr.cc',
    'memory/finally.cc',
    'program/arguments.cc',
    'program/environ.cc',
    'string/diff.cc',
    'string/join.cc',
    'string/lazy.cc',
    'string/lexer.cc',
    'string/split.cc',
    'system/errno.cc',
    'system/path.cc'
]


foreach main_path : test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [npputils_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
