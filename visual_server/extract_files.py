# Extract files from tar archive to
# sample/VTK/sample_NNN.vtk files.

import os
import tarfile,sys
import re
from subprocess import call
import make_images

def getsubdir_name(path):
    dir_ind = 2
    sep = "/"
    dirs = path.split(sep)
    # print "Extracted dirs has length " + str(len(dirs)) + ". Split by " + str(sep)
    if len(dirs) > dir_ind+1:
        return dirs[dir_ind]
    else:
        return None

targetpath = "sample/VTK/"
targetname = "sample_"
fname = "VTK.tar.gz"
tar = tarfile.open(fname)
n = 0
for tarpath in tar.getmembers():
    print "Looking at " + tarpath.name
    subdir = getsubdir_name(tarpath.name)
    # print "subdir is " + str(subdir)
    if subdir is None:
        basename = os.path.basename(tarpath.name)
        match = re.search( r'\d+\.vtk', basename)
        if match is not None:
            # print "match: " + match.group(0)
            newname = os.path.join(targetpath, targetname + match.group(0))
            print newname
            try:
                os.stat(targetpath)
            except:
                print "Creating  dir " + targetpath
                os.makedirs(targetpath)
            f=tar.extract(tarpath)
            print "Moving " + tarpath.name + " -> " + newname
            call(["mv", tarpath.name, newname])
            make_images.make_image(newname, n)
            n += 1
tar.close()


