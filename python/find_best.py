#!/usr/bin/env python3

import collections
import json
import os

class Results:

    def __add_result_file(self, rfile, only_final):
        with open(rfile, "r") as rfd:
            for jstr in rfd:
                j = json.loads(jstr)
                if j['comment'] == 'final':
                    self.__results.append(j)

    def __init__(self, directory, only_final=True):
        self.__results = []
        
        results = os.listdir(directory)
        for result in results:
            self.__add_result_file(os.path.join(directory, result),
                                   only_final)

    def sort_length_of_stay(self):
        self.__results.sort(
            key = lambda k: k['rating']['length-of-stay'])

    def print_head(self, cnt=30):
        for xidx in range(cnt):
            print(json.dumps(self.__results[xidx]))

    def generate_tour_statistics(self, cnt=10):
        stats = {}
        for xidx in range(cnt):
            for tour in self.__results[xidx]['tours']:
                idtuple = tuple(tour['ids'])
                if idtuple not in stats:
                    stats[idtuple] = 0
                stats[idtuple] = stats[idtuple] + 1
        return stats

def main():
    results = Results("results")
    results.sort_length_of_stay()
    results.print_head(10)

    stats = results.generate_tour_statistics(5)
    ostats = collections.OrderedDict(sorted(stats.items(), key=lambda k: k[1]))
#    print(ostats)
      


if __name__ == '__main__':
    main()
