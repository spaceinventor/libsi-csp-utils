project('si-csp-utils', ['c'], version: '0.0.1')

dependencies = dependency('csp', required: true)

si_csp_utils_inc = include_directories('include')
install_subdir('include', install_dir : '.', install_tag: 'devel')

si_csp_utils_lib = library(meson.project_name(),
    ['src/cli.c'],
    include_directories: si_csp_utils_inc,
    dependencies: dependencies,
    soversion: meson.project_version(),
    install: true
)
pkg = import('pkgconfig')
pkg.generate(
    name: meson.project_name(),
    version: meson.project_version(),
    libraries: [si_csp_utils_lib],
    description: 'Library of C functions implementing common/tedious CSP related features'
)

si_csp_utils_dep = declare_dependency(include_directories : si_csp_utils_inc, link_with : si_csp_utils_lib)
