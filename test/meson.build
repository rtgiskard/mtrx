cmd = run_command('find . -name *.cc'.split(), check: true)
src_test = cmd.stdout().strip().split('\n')

src_test += [
  '../src/utils/log.cc',
  '../src/utils/crypto.cc',
  '../src/utils/misc.cc',
]

deps_test = deps + [ dependency('doctest') ]

exe_test = executable('mtrx_test',
  sources: src_test,
  dependencies: deps_test,
  build_by_default: false)

test('tests', exe_test)
