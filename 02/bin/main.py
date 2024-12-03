Report = list[int]


def get_reports_from_input() -> list[Report]:
    with open("input.txt", "r") as f:
        return [
            [int(level) for level in report.split()]
            for report in f.readlines()
        ]


# DANGER, code is at this point not correct, the fact it gives correct
# result on my input was a highly unlikely coicidence
def is_report_safe(report: Report, can_be_dampened: bool) -> bool:
    is_increasing: bool | None = None
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
        elif can_be_dampened:
            can_be_dampened = False
        else:
            return False
    return True


if __name__ == "__main__":
    safe_reports: int = 0
    dampened_reports: int = 0
    for report in get_reports_from_input():
        safe_reports += 1 if is_report_safe(report, False) else 0
        dampened_reports += 1 if is_report_safe(report, True) else 0
    print("Total amount of safe reports:", safe_reports)
    print("Dampened amount of safe reports:", dampened_reports)
