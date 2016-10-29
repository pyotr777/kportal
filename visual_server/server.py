from flask import Flask, url_for
from flask import request
from flask import send_file
import os
import tarfile,sys
from subprocess import call
import make_images
app = Flask(__name__)

version="0.08b"
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
        print "Rename files in sample/VTK:"
        call("./rename_vtk_files.sh sample/VTK")
        print "Calling make_images.py"
        make_images.make_images()
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


def untar(fname):
    if (fname.endswith("tar.gz") or fname.endswith("tar")):
        tar = tarfile.open(fname)
        tar.extractall()
        tar.close()
        print "Extracted "+fname +" in " + os.getcwd()
    else:
        print "Not a tar.gz file: '%s '" % sys.argv[0]
