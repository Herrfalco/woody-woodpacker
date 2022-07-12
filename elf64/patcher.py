#!/usr/bin/env python3

import os
import re

if __name__ == "__main__":
    REGEXP = re.compile(r'\s*[0-9a-f]+:\s+(?P<ops>([0-9a-f]{2}\s)*([0-9a-f]{2})*)')
    RESULT = []

    os.system('nasm -felf64 shell_code.asm')
    STREAM = os.popen('objdump -D shell_code.o')
    for line in STREAM:
        match = REGEXP.search(line)
        if match:
            RESULT.extend(match.group('ops').split())
    RESULT = ['\\x' + elem for elem in RESULT]
    with open('shell_code.txt', 'w') as output:
        print('"', file=output, end='')
        for i, op in enumerate(RESULT):
            if i != 0 and i % 5 == 0:
                print('" \\\n"', file=output, end='')
            print(op, file=output, end='')
        print('"', file=output)
        os.system('rm -rf shell_code.o')
