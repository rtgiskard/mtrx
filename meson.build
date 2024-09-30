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

  # spdlog: turn on/off logging statements at compile time
  # make sure this is defined before include spdlog.h
add_project_arguments(
  '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG',
  language : 'cpp')


subdir('src/')
subdir('data/')

if get_option('test')
  subdir('test/')
endif
