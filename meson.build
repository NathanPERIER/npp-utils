project('cpp-utils', ['c','cpp'], default_options: ['cpp_std=c++2a'])

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

subdir('lib')
subdir('src')

# subdir('test')
