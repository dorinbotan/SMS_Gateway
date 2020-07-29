# -*- coding: utf-8 -*-
"""
Created on Mon Apr 10 15:01:16 2017
"""
import os
import sys
import zipfile

path = sys.argv[1]

if '/' in path:
    if os.path.exists(path):
        dir_path = path.split('.')[:-1][0]
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)
        else:
            os.system("rm -rf " + dir_path)
            os.makedirs(dir_path)

        zip_ref = zipfile.ZipFile(path, 'r')
        zip_ref.extractall(dir_path)
        zip_ref.close()

        files = os.listdir(dir_path)
        for file in files:
            os.rename(os.path.join(dir_path, file), os.path.join(dir_path, file+'.gz'))
            os.system("gzip -d " + os.path.join(dir_path, file+'.gz'))
        organized_files = [None] * (len(files))
        for file in files:
            organized_files[int(file[-1])] = file

        log_file = open((dir_path + '/' + "log_file.txt"), 'w')
        for index, file in enumerate(organized_files):
            with open(dir_path + '/' + organized_files[index]) as link:
                for line in link:
                    log_file.write(line)
        log_file.close()
    else:
       print("path to fill doesn't exist")
else:
    print("no path in argument")
