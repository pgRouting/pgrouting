import os
import subprocess

TEST_DIR='./src/max_flow/test'
files = sorted(os.listdir(TEST_DIR))

command = 'psql -qAtf {0}/{1} > {0}/{2}'

for file in files:
    if 'test.sql' in file:
        print "Processing %s" % file
        p = subprocess.Popen(command.format(TEST_DIR, file, file.replace('.test.sql', '.result')), shell=True)
        p.wait()