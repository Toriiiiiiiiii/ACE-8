#!/bin/python3
import os
import sys

opcodes = {'nop': '0',  'ldi': '1',  'lda': '2',  'ldd': '3',  'sta': '4',
           'std': '5',  'psh': '6',  'pop': '7',  'adi': '8',  'adr': '9',
           'sbi': '10', 'sbr': '11', 'mli': '12', 'mlr': '13', 'dvi': '14',
           'dvr': '15', 'mdi': '16', 'mdr': '17', 'ani': '18', 'anr': '19',
           'ori': '20', 'orr': '21', 'not': '22', 'cpi': '23', 'cpr': '24',
           'jmp': '25', 'jiz': '26', 'jnz': '27', 'jin': '28', 'jic': '29',
           'cll': '30', 'ret': '31'}

regs = {'a': '0', 'b': '1', 'c': '2', 'd': '3', 'e': '4', 'f': '5', 'g': '6', 'h': 7}

lines, lineinfo, lineadr, labels = [], [], [], {}
LINEINFO_NONE, LINEINFO_ORG = 0x00000, 0x10000
LINEINFO_BEGIN, LINEINFO_END = 0x20000, 0x40000

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <source file>")
    exit(1)

with open(sys.argv[1], "r") as f:
    lines = [line.strip() for line in f.readlines()]

def importAll():
    global lines

    for i in range(len(lines)-1, -1, -1):
        if lines[i].startswith("%import"):
            dirname = os.path.dirname(sys.argv[1])

            filename = dirname + "/" + lines[i][8:].lstrip('"').lstrip("'").rstrip('"').rstrip("'")
            with open(filename, "r") as f:
                lines = lines[:i] + [l.strip() for l in f.readlines()] + lines[i+1:]

            importAll()

# PASS 0: File Imports
importAll()

# PASS 1: Per-Line Replacements
for i in range(len(lines)):
    while lines[i].find("'") != -1:
        k = lines[i].find("'")
        l = lines[i].find("'", k+1)

        if k != -1 and l != -1:
            replaced = ''
            for c in lines[i][k+1:l]: 
                replaced += str(ord(c)) + ' '

            lines[i] = lines[i][0:k] + replaced + lines[i][l+1:]

        else:
            break

    while lines[i].find("\"") != -1:
        k = lines[i].find("\"")
        l = lines[i].find("\"", k+1)

        if k != -1 and l != -1:
            replaced = ''
            for c in lines[i][k+1:l]: 
                replaced += str(ord(c)) + ' '

            lines[i] = lines[i][0:k] + replaced + lines[i][l+1:]

        else:
            break

    if lines[i].find(";") != -1:
        lines[i] = lines[i][0:lines[i].find(";")]

    lines[i] = lines[i].replace(",", " ")

    lineinfo.append(LINEINFO_NONE)
    if lines[i].find("%begin") != -1: 
        lineinfo[i] |= LINEINFO_BEGIN
        lines[i] = lines[i].replace("%begin", "")

    if lines[i].find("%end") != -1:
        lineinfo[i] |= LINEINFO_END
        lines[i] = lines[i].replace("%end", "")

    k = lines[i].find("%org")
    if k != -1:
        s = lines[i][k:].split()
        lineinfo[i] |= LINEINFO_ORG + int(s[1], 0)
        lines[i] = lines[i][0:k].join(s[2:])

    if lines[i].find(":") != -1:
        labels[lines[i][:lines[i].find(":")]] = i
        lines[i] = lines[i][lines[i].find(":") + 1:]

    lines[i] = lines[i].split()

    for j in range(len(lines[i]) - 1, -1, -1):
        try: 
            regs[lines[i][j]] 
            lines[i][j] = regs[lines[i][j]]
        except: pass

        try: 
            lines[i][j] = str((int(opcodes[lines[i][j].lower()]) << 3) + int(lines[i][j+1], 0))
            lines[i].pop(j+1)
        except:
            try: lines[i][j] = str(int(opcodes[lines[i][j].lower()]) << 3)
            except:
                if lines[i][j].find("0x") == 0 and len(lines[i][j]) > 4:
                    val = int(lines[i][j], 16)
                    lines[i][j] = str(val & 0xff)
                    lines[i].insert(j+1, str((val >> 8) & 0xff))

# PASS 2: STARTING ADDRESS
adr = 0
for i in range(len(lines)):
    for j in range(len(lines[i])-1, -1, -1):
        e = lines[i][j]
        if e[0] == "<" or e[0] == ">": continue
        if e.find("+") != -1: e = e[0:e.find("+")]
        if e.find("-") != -1: e = e[0:e.find("-")]
        try:
            labels[e]; lines[i].insert(j+1, "0x@@")
        except: pass
            
    if lineinfo[i] & LINEINFO_ORG: adr = lineinfo[i] & 0xffff
    lineadr.append(adr)
    adr += len(lines[i])

for l in labels: labels[l] = lineadr[labels[l]]

# PASS 3: REPLACE REFERENCES TO MEMORY ADDRESSES
for i in range(len(lines)):
    for j in range(len(lines[i])):
        e = lines[i][j]
        off = 0
        pre = ""

        if e[0] == "<" or e[0] == ">":
            pre = e[0]
            e = e[1:]

        if e.find("+") != -1: 
            off += int(e[e.find("+")+1:])
            e = e[0:e.find("+")]

        if e.find("-") != -1:
            off -= int(e[e.find("-")-1:])
            e = e[0:e.find("-")]
    
        try:
            adr = labels[e] + off
            if pre == "<": lines[i][j] = str(adr & 0xff)
            elif pre == ">": lines[i][j] = str((adr >> 8) & 0xff)
            else:
                lines[i][j] = str(adr & 0xff)
                lines[i][j+1] = str((adr >> 8) & 0xff)
        
        except: pass

        try:
            int(lines[i][j], 0)
        except:
            print(f"ERROR IN LINE {str(i+1)}: Undefined Expression '{lines[i][j]}'")
            exit(1)

print("Assembly complete!\n")

print("Symbols:")
print("--------")

longest = 0
for label in labels:
    if len(label) > longest:
        longest = len(label)

for label in labels:
    print(f"{label.ljust(longest, ' ')}   0x%04.4x" % labels[label])

print("\nMachine Code:")
print("-------------")

for i in range(len(lines)):
    if len(lines[i]) == 0: continue
    s = ('%04.4x' % lineadr[i]) + ": "
    for e in lines[i]: s += ('%02x' % (int(e, 0) & 0xff)) + " "
    print(s)

# Write output file
outPath = input("Output file > ")

program = []
for i in range(len(lines)):
    for e in lines[i]: 
        program.append(int(e, 0))

with open(outPath, "wb") as f:
    f.write(bytearray(program))
