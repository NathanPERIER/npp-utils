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

subdir('lib')
subdir('src')

subdir('test')
