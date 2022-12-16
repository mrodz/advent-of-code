from typing import Optional

DEBUG = False


class CargoBay:
    def __init__(self):
        self.inner = dict()

    def add_box(self, column: int, box: str, building=False):
        if column not in self.inner:
            self.inner[column] = [box]
        else:
            self.inner[column].insert(
                0 if building else len(self.inner[column]), box)

    def pop_box(self, column: int) -> Optional[str]:
        if column not in self.inner or len(self.inner[column]) == 0:
            return None

        last = self.inner[column].pop()
        return last

    def shift(self, from_column: int, to: int):
        if (old := self.pop_box(from_column)) is not None:
            self.add_box(to, old)

    def __str__(self) -> str:
        result = ""
        for key in sorted(self.inner.keys()):
            result += f"[{key}] -> {self.inner[key]}\n"
        return result

    def top_chars(self):
        for key in sorted(self.inner.keys()):
            arr = self.inner[key]
            yield arr[len(arr) - 1]


class Instruction:
    def __init__(self, amount, base, destination):
        self.amount = amount
        self.base = base
        self.destination = destination

    def from_str(string: str):
        split = string.split(' ')
        return Instruction(int(split[1]), int(split[3]), int(split[5]))

    def __str__(self) -> str:
        return f"move {self.amount} from {self.base} to {self.destination}"


boxes = CargoBay()


def process_instruction(instruction: Instruction):
    for _ in range(instruction.amount):
        boxes.shift(instruction.base, instruction.destination)
    # for i in (boxes[instruction.base])[:instruction.amount:-1]:
    #     boxes.add_box(instruction.destination, i)


def row_of_boxes(line: str):
    for i, symbol in enumerate(line[1::4]):  # get the important symbols
        if symbol != ' ':
            boxes.add_box(i + 1, symbol, building=True)


def main():
    with open("./input.txt") as f:
        file_iter = f.__iter__()

        for line in file_iter:
            if line == '\n' or line[1].isdigit():
                file_iter.__next__()
                break

            row_of_boxes(line)

        if DEBUG:
            print(boxes)

        for line in file_iter:
            process_instruction(Instruction.from_str(line))

            if DEBUG:
                print(line[:-1], boxes, sep=":\n")

        for char in boxes.top_chars():
            print(char, end="")


if __name__ == "__main__":
    main()
