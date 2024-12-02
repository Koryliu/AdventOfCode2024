Report = list[int]


def get_reports_from_input() -> list[Report]:
    with open("input.txt", "r") as f:
        return [
            [int(level) for level in report.split()]
            for report in f.readlines()
        ]


def is_report_safe(report: Report) -> bool:
    is_increasing: bool | None = None
    is_problem_dampened: bool = False
    for i, level in enumerate(report):
        if i == len(report) - 1:
            continue

        next_level: int = report[i + 1]
        if (is_increasing is None or is_increasing) and\
           (level < next_level <= level + 3):
            is_increasing = True
        elif (is_increasing is None or not is_increasing) and\
             (next_level < level <= next_level + 3):
            is_increasing = False
        elif not is_problem_dampened:
            is_problem_dampened = True
        else:
            return False
    return True


if __name__ == "__main__":
    safe_reports: int = 0
    for report in get_reports_from_input():
        safe_reports += 1 if is_report_safe(report) else 0
    print("Total amount of safe reports:", safe_reports)
