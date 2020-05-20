import sqlite3
import sys
import csv

# check if input valid commend-line argument
if len(sys.argv) != 2:
    sys.exit("Usage: python import.py file.csv")

# save file name
file_name = sys.argv[1]

if not (file_name.endswith(".csv")):
    sys.exit("Usage: python import.py file.csv")

# access to the database of students.db
student_db = sqlite3.connect('students.db')

# open csv file
with open(file_name, "r") as infile:

    # create DictReader
    reader = csv.DictReader(infile)

    # Iterate over csv file
    for row in reader:
        name = []
        for part in row["name"].split(" "):
            name.append(part)

        # Insert students by substituting values into each ? placeholder
        if len(name) == 2:
            student_db.execute("INSERT INTO students (first, last, house, birth) VALUES(?, ?, ?, ?)", (name[0], name[1],
                                                                                                       row["house"], row["birth"]))

        if len(name) == 3:
            student_db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", (name[0],
                                                                                                                  name[1], name[2], row["house"], row["birth"]))

student_db.commit()
student_db.close()

