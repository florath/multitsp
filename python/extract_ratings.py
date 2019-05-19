import json

def main():
    with open("results/vos01-final.json", "r") as fd:
        for line in fd:
            j = json.loads(line)
            print("%d,%d" % (j['rating']['length-of-stay'], j['rating']['tour-length']))

if __name__ == '__main__':
    main()
