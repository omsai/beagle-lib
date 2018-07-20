cc = meson.get_compiler('c')
cdata = configuration_data()
check_headers = [
  ['HAVE_CPUID_H', 'cpuid.h'],
  ['HAVE_DLFCN_H', 'dlfcn.h'],
  ['HAVE_INTTYPES_H', 'inttypes.h'],
  ['HAVE_MEMORY_H', 'memory.h'],
  ['HAVE_STDINT_H', 'stdint.h'],
  ['HAVE_STDLIB_H', 'stdlib.h'],
  ['HAVE_STRINGS_H', 'strings.h'],
  ['HAVE_STRING_H', 'string.h'],
  ['HAVE_SYS_STAT_H', 'sys/stat.h'],
  ['HAVE_SYS_TYPES_H', 'sys/types.h'],
  ['HAVE_UNISTD_H', 'unistd.h'],
]

foreach h : check_headers
  if cc.has_header(h.get(1))
    cdata.set(h.get(0), 1)
  endif
endforeach

check_functions = [
# check token ('HAVE_ABM',)
# check token ('HAVE_ADX',)
# check token ('HAVE_AES',)
# check token ('HAVE_ALTIVEC',)
# check token ('HAVE_AVX',)
# check token ('HAVE_AVX2',)
# check token ('HAVE_AVX512_BW',)
# check token ('HAVE_AVX512_CD',)
# check token ('HAVE_AVX512_DQ',)
# check token ('HAVE_AVX512_ER',)
# check token ('HAVE_AVX512_F',)
# check token ('HAVE_AVX512_IFMA',)
# check token ('HAVE_AVX512_PF',)
# check token ('HAVE_AVX512_VBMI',)
# check token ('HAVE_AVX512_VL',)
# check token ('HAVE_BMI1',)
# check token ('HAVE_BMI2',)
# check token ('HAVE_FMA3',)
# check token ('HAVE_FMA4',)
# check token ('HAVE_LIBLTDL',)
# check token ('HAVE_MMX',)
# check token ('HAVE_MPX',)
# check token ('HAVE_PREFETCHWT1',)
# check token ('HAVE_RDRND',)
# check token ('HAVE_SHA',)
# check token ('HAVE_SSE',)
# check token ('HAVE_SSE2',)
# check token ('HAVE_SSE3',)
# check token ('HAVE_SSE4_1',)
# check token ('HAVE_SSE4_2',)
# check token ('HAVE_SSE4a',)
# check token ('HAVE_SSSE3',)
# check token ('HAVE_XOP',)
]

foreach f : check_functions
  if cc.has_function(f.get(1), prefix : f.get(2))
    cdata.set(f.get(0), 1)
  endif
endforeach

cdata.set_quoted('PACKAGE_BUGREPORT', 'beagle-dev@googlegroups.com')
cdata.set_quoted('PACKAGE_NAME', meson.project_name())
cdata.set_quoted('PACKAGE_STRING', 'FIXME')
cdata.set_quoted('PACKAGE_TARNAME', 'FIXME')
cdata.set_quoted('PACKAGE_URL', 'FIXME')
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set('PLUGIN_VERSION', v_plugin)

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)
add_project_arguments('-DHAVE_CONFIG_H', language : 'cpp')

installdir_inc = meson.project_name() + '-' + v_api
install_headers(
  [
    'beagle.h',
    'platform.h'
  ],
  install_dir : join_paths('include', installdir_inc)
)
