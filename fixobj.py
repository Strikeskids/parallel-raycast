import sys
import operator

def readObj(f):
    for line in f:
        if line[-1] == '\n': line = line[:-1]
        if not line: continue
        if line[0] == 'v':
            yield list(map(float, line.split(' ')[1:]))
        else:
            print(line)

def getRange(l, i):
    return (min(map(operator.itemgetter(i), l)), max(map(operator.itemgetter(i), l)))

def sfactor(cur, desired):
    return desired / (cur[1]-cur[0])

def modify(vecs, i, scale, sr, c):
    for vec in vecs:
        n = vec[i] - (sr[1]+sr[0])/2
        n *= scale
        vec[i] = n+c

def main():
    objfile = sys.argv[1]
    print('Scaling obj',objfile)
    x, y, z = map(float, sys.argv[2:5])
    w, l, h = map(float, sys.argv[5:8])
    vs = None
    with open(objfile, 'r') as f:
        vs = list(readObj(f))

    xr, yr, zr = getRange(vs, 0), getRange(vs, 1), getRange(vs, 2)

    scale = min(sfactor(xr, w), sfactor(yr, l), sfactor(zr, h))

    modify(vs, 0, scale, xr, x)
    modify(vs, 1, scale, yr, y)
    modify(vs, 2, scale, zr, z)

    for v in vs:
        print("v %f %f %f"%tuple(v))
    

if __name__ == '__main__': main()
