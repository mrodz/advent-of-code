with open('input.txt') as f:
    commands = f.readlines()


MAX_SIZE = 100_000
sizesmap = dict()
cwd = []


def elevatedcommand(command: str) -> bool:
    return command.startswith('$ ls') or command.startswith('dir')


for command in commands:
    if elevatedcommand(command):
        continue

    parts = command.split()

    if command.startswith('$ cd'):
        print(parts)
        if parts[2] == '..':
            cwd.pop()
        else:
            path = (cwd[-1] + '/' + parts[2]) if cwd else parts[2]
            cwd.append(path)
        continue

    else:
        for directory in cwd:
            if directory not in sizesmap:
                sizesmap[directory] = 0

            sizesmap[directory] += int(parts[0])

result = 0
for key, value in sizesmap.items():
    if value <= MAX_SIZE:
        result += value
        print(f'~{(key[1:])} ({value} bytes)')

print("PART ONE:", result)
