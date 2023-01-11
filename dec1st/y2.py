fileHandler = open('input.txt')
currentElf = 0
prevElf = 0
for eachLine in fileHandler:
    eachLine = eachLine.rstrip()
    if eachLine != '':
        eachLine = int(eachLine)
        currentElf += eachLine
    else:
        if currentElf > prevElf:
            prevElf = currentElf
        currentElf = 0

print(prevElf)