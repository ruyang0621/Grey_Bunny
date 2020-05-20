from cs50 import get_int

n = get_int("Number: ")

tmp = n
total = 0
digit_p = 1
while tmp > 0:
    remainder = tmp % 10
    tmp = tmp // 10
    if digit_p % 2 == 0:
        total = total + 2 * remainder // 10 + 2 * remainder % 10
    else:
        total = total + remainder
    digit_p += 1

start_2d = n // 10 ** (digit_p - 3)
start_1d = n // 10 ** (digit_p - 2)
if total % 10 == 0 and start_2d in [34, 37] and digit_p - 1 == 15:
    print("AMEX")
elif total % 10 == 0 and start_2d in range(51, 56, 1) and digit_p - 1 == 16:
    print("MASTERCARD")
elif total % 10 == 0 and start_1d == 4 and digit_p - 1 in [13, 16]:
    print("VISA")
else:
    print("INVALID")