project('octocurl2', 'cpp', default_options : ['cpp_std=c++17'])

curldep = dependency('libcurl')
boostdep = dependency('boost', modules : 'program_options')
exe = executable('octocurl2', 'src/octocurl2.cpp', 'src/filename.cpp', 'src/curlwrap.cpp', 'src/download_task.cpp',
  dependencies : [curldep, boostdep], install : true)
