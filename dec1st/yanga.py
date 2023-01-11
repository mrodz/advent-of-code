with open('input.txt') as file:
    max = 0
    this = 0
    for line in file:
        if line != '\n':
            this += int(line)
            continue
        
        if this > max:
            max = this
    
        this = 0