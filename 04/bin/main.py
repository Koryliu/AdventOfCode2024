Pattern = dict[str, set[str]]


class Searcher:
    XMAS_MAP: Pattern =\
        {'X': {'M'}, 'M': {'X', 'A'}, 'A': {'M', 'S'}, 'S': {'A'}}
    MAS_MAP: Pattern = {'M': {'A'}, 'A': {'M', 'S'}, 'S': {'A'}}

    def __init__(self, puzzle: list[str]):
        self.puzzle: list[str] = puzzle

    def get_letter(self, x: int, y: int) -> str:
        if (0 <= y < len(self.puzzle)) and (0 <= x < len(self.puzzle[y])):
            return self.puzzle[y][x]
        return ''

    def print_square(self, x: int, y: int) -> None:
        for col in range(-1, 2):
            for row in range(-1, 2):
                print(self.get_letter(x + row, y + col), end='')
            print()  # newline

    def ceres_search(self) -> int:
        total: int = 0
        for y in range(len(self.puzzle)):
            for x in range(len(self.puzzle[y])):
                if self.get_letter(x, y) not in "XMAS":
                    continue

                for dx, dy in [(0, 1), (1, 1), (1, 0), (1, -1)]:
                    if is_pattern(
                       self.find_pattern("", Searcher.XMAS_MAP,
                                         x, y, dx, dy),
                       "XMAS"):
                        total += 1
        return total

    def xmas_search(self) -> int:
        total: int = 0
        for y in range(len(self.puzzle)):
            for x in range(len(self.puzzle[y])):
                if self.get_letter(x, y) != 'A':
                    continue

                if is_pattern(
                    self.find_pattern("", Searcher.MAS_MAP,
                                      x - 1, y + 1, 1, -1),
                    "MAS") and\
                   is_pattern(
                    self.find_pattern("", Searcher.MAS_MAP,
                                      x + 1, y + 1, -1, -1),
                   "MAS"):
                    total += 1
        return total

    def find_pattern(self, word: str, pattern: Pattern,
                     x: int, y: int, dx: int, dy: int,) -> str | None:
        if len(word) == len(pattern):
            return word

        letter: str = self.get_letter(x, y)
        if (word != '' and letter in pattern[word[-1]]) or\
           (letter in pattern.keys()):
            return self.find_pattern(word + letter, pattern,
                                     x + dx, y + dy, dx, dy)
        return None


def is_pattern(test: str | None, pattern: str) -> bool:
    if test is None:
        return False

    return test == pattern or test == pattern[::-1]


def get_input() -> list[str]:
    result: list[str] = []
    with open("input.txt", "rt") as f:
        result = f.readlines()
    return result


if __name__ == "__main__":
    searcher = Searcher(get_input())
    print("XMAS occured", searcher.ceres_search(), "times")
    print("X-MAS occured", searcher.xmas_search(), "times")
