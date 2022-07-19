#!/usr/bin/env python3

import os
import re
import sys

if __name__ == "__main__":
    REGEXP = re.compile(r'\s*[0-9a-f]+:\s+(?P<ops>([0-9a-f]{2}\s)*([0-9a-f]{2})*)')
    RESULT = []

    if len(sys.argv) != 2:
        print('Error: ./get_shellcode.py ASM_FILE', file=sys.stderr)
        sys.exit(1)
    os.system('nasm -felf64 ' + sys.argv[1])
    STREAM = os.popen('objdump -D ' + sys.argv[1][:-4] + '.o')
    for line in STREAM:
        match = REGEXP.search(line)
        if match:
            RESULT.extend(match.group('ops').split())
    print(len(RESULT))
    os.system('rm -rf ' + sys.argv[1][:-4] + '.o')
