import subprocess
import numpy
import sys


'''
This script is used to run multiple experiments, get their output,
then calculate the mean and standard deviation for thoes data.
Use this script like this:
    python analyze.py "./exec" output.txt 100
where "./exec" is the binary you are executing, output.txt is where
the script will write the result, 100 is how many times you want
to repeat the "./exec" command.

The output format accepted by this script is like:
    15.24
    266.878
    48765.7
with just floating numbers. This script will treat each line of out
put as one experiment, so average and standard deviation for each
experiment are calculated separately among lines, each line forming
their own data set.
'''


def prepare_subprocess(command):
    return subprocess.Popen(command, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)

args = sys.argv
command = args[1].split(" ")
output = args[2]
times = int(args[3])

print "processing:", command
print "output data to:", output
print "repeat for:", times, "times"
p = prepare_subprocess(command)
out, err = p.communicate()
print out
split = out[0:-1].split('\n')
length = len(split)
data = [[0 for x in xrange(times)] for x in xrange(length)]
for i in range(length):
    data[i][0] = float(split[i])

for i in range(1, times):
    print "processing", i + 1, "times"
    p = prepare_subprocess(command)
    out, err = p.communicate()
    print out
    split = out[0:-1].split('\n')
    for k in range(length):
        data[k][i] = float(split[k])

f = open(output, 'w')
for i in range(length):
    f.write("mean:" + str(numpy.mean(data[i], axis=0)) + "\n")
    f.write("std deviation:" + str(numpy.std(data[i], axis=0)) + "\n")
f.close()
