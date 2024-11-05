#!/usr/bin/env python3

import subprocess
import sys



args = sys.argv[1:] 


finalL = set()

for i in args :
    aaa = subprocess.check_output(["pkg-config", "--cflags", i]) 
    l = list(aaa.decode("utf-8").split())
    #sanitize
    l = [x for x in l if x.startswith("-I")]
    l = list(map(lambda x: x[2:], l))

    finalL.update(list(map(lambda x: "\"" + x + "\"", l)))

print(",\n".join(list(finalL)))