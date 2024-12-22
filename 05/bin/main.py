from io import TextIOWrapper
import sys
sys.setrecursionlimit(3000)  # default is 1000

OrderRules = dict[int, set[int]]
Update = list[int]


def strip_whitespace(string: str) -> str:
    return string.lstrip().rstrip()


def get_page_ordering(file: TextIOWrapper) -> OrderRules:
    result: OrderRules = {}

    line: str = file.readline()
    while line != "\n":
        left, right = line.split('|')
        left, right = strip_whitespace(left), strip_whitespace(right)
        assert left.isdecimal() and right.isdecimal()
        left_n, right_n = int(left), int(right)

        if result.get(left_n) is None:
            result[left_n] = set()
        result[left_n].add(right_n)

        line = file.readline()
    return result


def get_updates(file: TextIOWrapper) -> list[Update]:
    result: list[Update] = []

    line: str = file.readline()
    while line != "":
        update: Update = []
        for number in line.split(','):
            number = strip_whitespace(number)
            assert number.isdecimal()
            update.append(int(number))
        result.append(update)
        line = file.readline()

    return result


def is_update_correct(update: Update, rules: OrderRules) -> bool:
    for i, val in enumerate(update):
        for prev_val in update[:i]:
            if prev_val in rules.get(val, set()):
                return False
    return True


def fix_update(update: Update, rules: OrderRules, idx: int) -> None:
    if idx == len(update):
        return

    val: int = update[idx]
    if rules.get(val) is not None:
        for prev_i in range(idx):
            if update[prev_i] in rules[val]:
                update[idx], update[prev_i] = update[prev_i], update[idx]
                idx = prev_i
                break
    fix_update(update, rules, idx + 1)


def example_values_test() -> None:
    with open("example.txt", "rt") as f:
        rules: OrderRules = get_page_ordering(f)
        updates: list[Update] = get_updates(f)

    for update in updates[:3]:
        assert is_update_correct(update, rules)

    expected_fixes: list[Update] = [
        [97, 75, 47, 61, 53],
        [61, 29, 13],
        [97, 75, 47, 29, 13],
    ]
    for i, update in enumerate(updates[3:]):
        assert not is_update_correct(update, rules)
        fix_update(update, rules, 0)
        assert update == expected_fixes[i]


def main() -> None:
    example_values_test()

    with open("input.txt", "rt") as f:
        rules: OrderRules = get_page_ordering(f)
        updates: list[Update] = get_updates(f)

    valid: int = 0
    fixed: int = 0
    for update in updates:
        if is_update_correct(update, rules):
            valid += update[(len(update) - 1) // 2]
        else:
            fix_update(update, rules, 0)
            fixed += update[(len(update) - 1) // 2]
    print("Middle page number sum of all valid pages is:", valid)
    print("Middle page number sum of all fixed pages is:", fixed)


if __name__ == "__main__":
    main()
