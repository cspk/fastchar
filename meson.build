project('fastchar', 'c', version: '1.1', license: 'MIT',
        default_options: ['c_std=c11'])

src = ['src/main.c', 'src/cfg.c', 'src/ui.c', 'src/x11.c']

gtk3 = dependency('gtk+-3.0')
x11 = dependency('x11')
xtst = dependency('xtst')

executable('fastchar', src, dependencies: [gtk3, x11, xtst], install: true,
           c_args: '-DPROJECT_VERSION="@0@"'.format(meson.project_version()))
