#!/usr/bin/env python3

import json
import os

MAXID=90

def check_tour(line):
    res = {}
    for i in range(MAXID):
        res[i+1] = 0
        
    jtour = json.loads(line)
    print(jtour)

    for tour in jtour['tours']:
        for i in tour['ids']:
            res[i] += 1

    for i in range(MAXID):
        assert res[i+1] == 1

def check_file(filename):
    print(filename)
    with open(filename, "r") as cfd:
        for line in cfd:
            check_tour(line)

def main():
    #    result_files = os.listdir("results")
    result_files = os.listdir("r")
    for rfile in result_files:
        check_file(os.path.join("r", rfile))

if __name__ == '__main__':
    main()
