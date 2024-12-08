
core_test_source_files = [
    'type_name.cc',
    'collections/pre_hashed.cc',
    'colour/conv.cc',
    'colour/rgb.cc',
    'conv/integer.cc',
    'format/csv.cc',
    'format/hexdump.cc',
    'math/complex.cc',
    'math/fraction.cc',
    'math/interval.cc',
    'math/vec2.cc',
    'memory/copy_ptr.cc',
    'memory/finally.cc',
    'program/arguments.cc',
    'string/diff.cc',
    'string/join.cc',
    'string/lazy.cc',
    'string/split.cc',
]


foreach main_path : core_test_source_files
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [npputils_core_dep, catch2_dep],
        install: false,
        build_by_default: true
    )
    test(test_name, test_exe)
endforeach
