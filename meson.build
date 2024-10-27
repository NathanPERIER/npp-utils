project('npp-utils', ['c','cpp'], default_options: ['cpp_std=c++2a'])

cmake = import('cmake')


cpp_warning_flags = [
  '-Wall',
  '-Wextra',
  '-pedantic-errors',
  '-Wdisabled-optimization',
  '-Werror=unused-result',
  '-Werror=return-type'
]

fmt_proj = cmake.subproject('fmt')
fmt_dep = fmt_proj.dependency('fmt')

catch2_proj = subproject('catch2', default_options: ['cpp_std=c++2a'])
catch2_dep = catch2_proj.get_variable('catch2_with_main_dep')


enable_parser_lib = get_option('parser_lib').enabled() or (get_option('parser_lib').auto() and not meson.is_subproject())
enable_json_lib   = get_option('json_lib').enabled() or (get_option('json_lib').auto() and not meson.is_subproject())
enable_unix_lib   = get_option('unix_lib').enabled() or (get_option('unix_lib').auto() and not meson.is_subproject() and target_machine.system() == 'linux')


subdir('core')

if enable_parser_lib
  subdir('parser')
endif

if enable_json_lib
  subdir('json')
endif

if enable_unix_lib
  subdir('unix')
endif

subdir('poc')
