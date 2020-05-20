import sqlite3
import sys
import csv

# check if input valid commend-line argument
if len(sys.argv) != 2:
    sys.exit("Usage: python import.py house")

# save input house
H = sys.argv[1].lower()

# access to the database of students.db
conn = sqlite3.connect('students.db')

# Creating a cursor object using the cursor() method
cursor = conn.cursor()

cursor.execute("SELECT first, middle, last, birth FROM students WHERE lower(house) = '{}' ORDER BY last, first;".format(H))

result = cursor.fetchall()

for x in result:
    if x[1] == None:
        print("{0} {1}, born {2}".format(x[0], x[2], x[3]))
    else:
        print("{0} {1} {2}, born {3}".format(x[0], x[1], x[2], x[3]))

conn.close()