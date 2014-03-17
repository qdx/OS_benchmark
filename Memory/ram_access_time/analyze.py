import subprocess
import numpy
import sys


args = sys.argv
command = args[1]
output = args[2]
times = int(args[3])

p = subprocess.Popen([command], stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE)
out, err = p.communicate()
split = out[0:-1].split('\n')
length = len(split)
data = [[0 for x in xrange(times)] for x in xrange(length)]
for i in range(length):
    data[i][0] = float(split[i])

for i in range(1, times):
    p = subprocess.Popen([command], stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE)
    out, err = p.communicate()
    split = out[0:-1].split('\n')
    for k in range(length):
        data[k][i] = float(split[k])

f = open(output, 'w')
x = ""
big_x = ""
y = ""
big_y = ""

for i in range(length):
    x = str(numpy.mean(data[i], axis = 0))
    f.write("mean:" + x + "\n")
    big_x += x + "\n"
    y = str(numpy.std(data[i], axis = 0));
    f.write("std deviation:" + y + "\n")
    big_y += y + "\n"

f.write(big_x + "\n"+ "\n"+ "\n");
f.write(big_y + "\n"+ "\n"+ "\n");
f.close()
