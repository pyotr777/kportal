from flask import Flask, url_for
from flask import request
from flask import send_file
import os
import tarfile, sys, re
from subprocess import call
import make_images
app = Flask(__name__)

version="0.16b"
print "Flask VTK->images->movie server version " + version

movie_fname = "out.mp4"

@app.route('/')
def api_root():
    return "Hello! This is Flask VTK->images->movie server version " + version + "."


if __name__ == '__main__':
    app.run()


@app.route('/files', methods = ['POST'])
def receive_files():
    response = ""
    if request.files is not None:
        file = request.files['file']
        filename = file.filename
        print "Accessed /files endpoint. Request files written =" + filename
        file.save(filename)
        untar(filename)
        print "Remove tar file " + filename
        # os.remove(filename)
        print "Check that sample directory exists:"
        for f in os.listdir("sample/VTK"):
            print "sample/VTK/"+f
        #print "Rename files in sample/VTK:"
        #call("./rename_vtk_files.sh sample/VTK")
        #print "Calling make_images.py"
        #make_images.make_images()
        print "Check that export directory has been created."
        for f in os.listdir("export"):
            print "export/"+f
        print "Read images with pattern export/img_%04d.png"
        if os.path.isfile(movie_fname):
            os.remove(movie_fname)
        call("./make_movie.sh")
        print "Movie file "+ movie_fname +" created."
        return send_file(movie_fname, mimetype='video/mp4')
    else:
        return "415 Unsupported Media Type"



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

def untar(fname):
    if (fname.endswith("tar.gz") or fname.endswith("tar")):
        n  = 0  # Image file counter
        tar = tarfile.open(fname)
        for tarpath in tar.getmembers():
            # print "Looking at " + tarpath.name
            subdir = getsubdir_name(tarpath.name)
            # print "subdir is " + str(subdir)
            if subdir is None:
                basename = os.path.basename(tarpath.name)
                match = re.search( r'\d+\.vtk', basename)
                if match is not None:
                    # print "match: " + match.group(0)
                    newname = os.path.join(targetpath, targetname + match.group(0))
                    # print newname
                    try:
                        os.stat(targetpath)
                    except:
                        print "Creating  dir " + targetpath
                        os.makedirs(targetpath)
                    f=tar.extract(tarpath)
                    f=tar.extract(tarpath)
                    # print "Moving " + tarpath.name + " -> " + newname
                    call(["mv", tarpath.name, newname])
                    make_images.make_image(newname, n)
                    n += 1
        tar.close()



