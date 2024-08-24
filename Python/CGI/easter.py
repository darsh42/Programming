import datetime

def easter(y):
    a = y % 19
    b = y // 100
    c = y % 100
    d = b // 4
    e = b % 4
    g = (8 * b + 13) // 25
    h = (19 * a + b - d - g + 15) % 30
    j = c // 4
    k = c % 4
    m = (a + 11 * h) // 319
    r = (2 * e + 2 * j - k - h + m + 32) % 7
    n = (h - m + r + 90) // 25
    p = (h - m + r + n + 19) % 32
    return datetime.datetime(year=y, month=n, day=p)

def suffix(day):
    if day % 10 == 1 and day != 11:
        return "st"
    elif day % 10 == 2 and day != 12:
        return "nd"
    elif day % 10 == 3 and day != 13:
        return "rd"
    else:
        return "th"

date = easter(int(input("year")))
suffix = suffix(int(date.strftime('%d')))
print(f"{date.strftime('%d')}<sup>{suffix}</sup> {date.strftime('%B')} {date.strftime('%Y')}")

