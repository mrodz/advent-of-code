from typing import Optional, Callable


class Cranes:
    def model_9000(cargo, from_column: int, to: int, n: int = 1):
        for _ in range(n):
            if (old := cargo.pop_box(from_column)) is not None:
                cargo.add_box(to, old)

    def model_9001(cargo, from_column: int, to: int, n: int = 1):
        cargo.inner[to] += cargo.inner[from_column][-n:]
        for _ in range(n):
            cargo.inner[from_column].pop()


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

    def shift(self, from_column: int, to: int, n: int = 1, crane_model: Callable[[any, int, int, int], None] = Cranes.model_9000):
        crane_model(self, from_column, to, n)

    def __str__(self) -> str:
        result = ""
        for key in sorted(self.inner.keys()):
            result += f"[{key}] -> {self.inner[key]}\n"
        return result

    def top_chars(self):
        for key in sorted(self.inner.keys()):
            arr = self.inner[key]
            if len(arr) != 0:
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


def process_instruction(instruction: Instruction, crane):
    boxes.shift(instruction.base, instruction.destination,
                n=instruction.amount, crane_model=crane)


def row_of_boxes(line: str):
    for i, symbol in enumerate(line[1::4]):  # get the important symbols
        if symbol != ' ':
            boxes.add_box(i + 1, symbol, building=True)


def work_with_crane(crane):
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
            process_instruction(Instruction.from_str(line), crane=crane)

            if DEBUG:
                print(line[:-1], boxes, sep=":\n")

        for char in boxes.top_chars():
            print(char, end="")


def main():
    print("PART ONE:", end="\n\t")
    work_with_crane(Cranes.model_9000)
    print("\nPART TWO:", end="\n\t")
    work_with_crane(Cranes.model_9001)


if __name__ == "__main__":
    main()
