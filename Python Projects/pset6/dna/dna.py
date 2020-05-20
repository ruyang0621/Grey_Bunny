from sys import argv, exit
import csv

if len(argv) != 3:
    print("Usage: python dna.py CSVfile txtfile")
    exit()

# open CSV file and read content into memory
with open(argv[1]) as infile1:
    reader_dna = csv.reader(infile1)
    tmpcsv = []
    for row in reader_dna:
        tmpcsv.append(row)


STR = tmpcsv[0]
del STR[0]
sequences = []

# open txt file and read content into memory
with open(argv[2]) as infile2:
    dna_list = infile2.readlines()
    tmptxt = []
    for row in dna_list:
        tmptxt = row

for genestr in STR:
    length = len(genestr)
    counter = 0
    counter_max = 0
    for i in range(len(tmptxt)):
        if tmptxt[i: i + length] == genestr:
            counter += 1
            while tmptxt[i: i + length] == tmptxt[i + length: i + 2 * length]:
                counter += 1
                i += length

            if counter > counter_max:
                counter_max = counter

            counter = 0

    sequences.append(str(counter_max))

for i in range(1, len(tmpcsv)):
    if tmpcsv[i][1: len(STR) + 1] == sequences:
        print(tmpcsv[i][0])
        exit()
        
print("No Match")