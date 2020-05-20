from cs50 import get_float

while True:
    n = get_float("Change owed: ")
    if (n > 0):
        break

n = int(n * 100)
quarter = n // 25
quarter_r = n % 25
dime = quarter_r // 10
dime_r = quarter_r % 10
nickel = dime_r // 5
penny = dime_r % 5

print(f"{quarter + dime + nickel + penny}")