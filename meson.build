project('libhmsbeagle',
	['c', 'cpp', 'java'],
	default_options : ['c_std=c11', 'cpp_std=c++11'],
	version : '3.0.2',
        license : ['GPL3+', 'LGPL3+'])

# versions
v_major = '3'
v_minor = '0'
v_micro = '2'
v_api = '1'
v_revision = v_minor
# library version schema:
#
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed
v_current = meson.project_version().split('.').get(0)
v_age = '2'
v_library = v_current + ':' + v_revision + ':' + v_age
v_plugin = v_major + v_minor

# openmp
dep_openmp = dependency('openmp', required : false)

# optimization level
add_project_arguments('-O3', language : 'cpp')

# threads
dep_threads = dependency('threads')

# opencl
dep_opencl = dependency('OpenCL', required : false)

# cuda compiler doesn't look like it's supported yet in meson

# simd instruction checks require small test files
# cc = meson.get_compiler('c')
# simd = import('unstable_simd')
# rval = simd.check('simd',
# 		  sse : 'simd_sse.c',
# 		  sse2 : 'simd_sse2.c',
# 		  sse3 : 'simd_sse3.c',
# 		  ssse3 : 'simd_ssse3.c',
# 		  sse41 : 'simd_sse41.c',
# 		  sse42 : 'simd_sse42.c',
# 		  avx : 'simd_avx.c',
# 		  avx2 : 'simd_avx2.c',
# 		  compiler : cc)

inc = include_directories('.', 'libhmsbeagle', 'libhmsbeagle/plugin')

subdir('libhmsbeagle')

cpp = meson.get_compiler('cpp')
dl = cpp.find_library('dl')
libplugin = library('plugin',
		    [
		      'libhmsbeagle/plugin/Plugin.cpp',
		      'libhmsbeagle/plugin/UnixSharedLibrary.cpp',
		    ],
		    install : true,
		    dependencies : dl)
libdir = join_paths(get_option('prefix'), get_option('libdir'))
libhmsbeagle = library('hmsbeagle', 'libhmsbeagle/beagle.cpp',
		       install : true,
		       link_with : libplugin,
		       install_rpath : libdir)
dep_beagle = declare_dependency(include_directories : inc,
				link_with : libhmsbeagle)
