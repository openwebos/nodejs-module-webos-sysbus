# @@@LICENSE
#
#      Copyright (c) 2013 LG Electronics
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# LICENSE@@@
{
  "variables" : {
    'sysroot%': ''
    },
  "targets": [
    {
      'target_name': "webos-sysbus",
      'include_dirs': ['<(sysroot)/usr/include/glib-2.0',
                       '<(sysroot)/usr/lib/glib-2.0/include'],
      'sources': [ 'src/node_ls2.cpp',
                   'src/node_ls2_base.cpp',
                   'src/node_ls2_call.cpp',
                   'src/node_ls2_error_wrapper.cpp',
                   'src/node_ls2_handle.cpp',
                   'src/node_ls2_message.cpp',
                   'src/node_ls2_utils.cpp' ],
      'link_settings': {
          'libraries': [
              '-lglib-2.0',
              '-lluna-service2',
              '-lpthread'
          ]
      },
      'cflags!': [ '-fno-exceptions' ],
      'cflags': [ '-g' ],
      'cflags_cc': [ '-g' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'ldflags': [ '-pthread' ]
    }
  ]
}
