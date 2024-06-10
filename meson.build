project('mtrx', 'cpp',
  version: 'v0.1',
  meson_version: '>=1.4.0',
  default_options: [
    'cpp_std=c++23',
    'warning_level=2',
    # 'buildtype=release',
    # 'optimization=2',
    # 'b_lto=true',
    # 'b_pie=true',
  ])

subdir('src/')
subdir('data/')

if get_option('test')
  subdir('test/')
endif
