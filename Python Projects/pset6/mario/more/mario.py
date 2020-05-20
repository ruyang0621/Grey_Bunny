from cs50 import get_int

while True:
    n = get_int ("height: ")
    if ( 0 < n < 9):
        break

for x in range(n):
    print(" " * (n - x - 1) + "#" * (x + 1), end = "")
    print(" " * 2, end = "")
    print("#" * (x + 1))
    