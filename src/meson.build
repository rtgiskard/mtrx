cmd = run_command('find . -name *.cc'.split(), check: true)
src = cmd.stdout().strip().split('\n')

# processed on project setup only, update of input will trigger setup
const_vars = configure_file(
  input: 'const.h.in',
  output: 'const.h.vcs.in',
  configuration: {
    'NAME': meson.project_name(),
  })

# make sure it's up to date on every build
const_vcs = vcs_tag(
  command: 'git describe --long --tags'.split(),
  fallback: meson.project_version(),
  input: const_vars,
  output: 'const.h',
  replace_string: '<VERSION>')

src += const_vcs

deps = [
  dependency('fmt'),
  dependency('tomlplusplus', version: '>=3.4'),
  dependency('argparse', version: '>=3.0'),
]

subdir('utils/')
subdir('udp2p/')

deps += utils_deps
deps += udp2p_deps

exe = executable('mtrx',
  sources: src,
  dependencies: deps,
  install: true)

test('main', exe, args: ['dump'])
