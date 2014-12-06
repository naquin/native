'''
This script wraps the double-conversion library for easier integration with
`native`.
'''

import os

THIS_PATH             = os.path.dirname(os.path.realpath(__file__))
DOUBLE_CONVERSION_SRC = os.path.join(THIS_PATH, 'submodule/src')
ROOT                  = os.path.abspath(os.path.join(THIS_PATH, '../..'))
INCLUDE_PATH          = os.path.join(ROOT, 'include/native/detail/double-conversion')
SRC_PATH              = os.path.join(ROOT, 'src')

GENERATED_MESSAGE = '''// !!!
// generated from /native/deps/double-conversion/setup.py
// !!!
// easy compile for double-conversion


'''

def wrap_source(filename, expected_namespace='double_conversion'):
    lines = []
    with open(filename) as f:
        lines = f.readlines()

    wrapped = ['// ** {0} **\n'.format(os.path.basename(filename))]
    for line in lines:
        if line.startswith('namespace {0} {{'.format(expected_namespace)):
            wrapped.append('namespace native {\n')
            wrapped.append(line)
        elif line.startswith('}}  // namespace {0}'.format(expected_namespace)):
            wrapped.append(line)
            wrapped.append('}  // namespace native\n')
        elif line.startswith('#include "'):
            wrapped.append('#include "native/detail/double-conversion/{0}'.format(line[len('#include "'):]))
        else:
            wrapped.append(line)

    wrapped.append('// ** /{0} **\n'.format(os.path.basename(filename)))

    return ''.join(wrapped)


def write_source_amalgamation(sources):
    with open(os.path.join(SRC_PATH, 'double-conversion.cpp'), 'w') as f:
        f.write(GENERATED_MESSAGE)
        for filename in sources:
            f.write(wrap_source(os.path.join(DOUBLE_CONVERSION_SRC, filename)))


def wrap_headers(headers):
    for filename in headers:
        with open(os.path.join(INCLUDE_PATH, filename), 'w') as f:
            f.write(GENERATED_MESSAGE)
            f.write(wrap_source(os.path.join(DOUBLE_CONVERSION_SRC, filename)))

def main():
    files = os.listdir(DOUBLE_CONVERSION_SRC)

    sources = []
    headers = []
    for filename in files:
        ext = os.path.splitext(filename)[1]
        if ext in ['.cc']:
            sources.append(filename)
        elif ext in ['.h']:
            headers.append(filename)

    write_source_amalgamation(sources)
    wrap_headers(headers)


if __name__ == '__main__':
    main()
