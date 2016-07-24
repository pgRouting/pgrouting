import os
import subprocess

files = sorted(os.listdir('.'))

command = 'psql -Atf %s > %s.result'

for file in files:
    if 'test.sql' in file and 'edgedisjoint' not in file:
        p = subprocess.Popen(command % (file, file.replace('.test.sql', '')), shell=True)
        #p.wait()