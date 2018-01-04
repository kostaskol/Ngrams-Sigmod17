from sys import argv
from random import randint

if len(argv) != 2:
    print("One argument are required")

file = argv[1]

with open(file) as r:
    lines = []
    for line in r:
        if line != "\n":
            lines.append(line)

    with open(file, "w") as w:
        w.write("".join(lines))

