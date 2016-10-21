from flask import Flask, url_for
from flask import request
from flask import send_file
import os
import tarfile,sys
from subprocess import call
import make_images
app = Flask(__name__)

version="0.04b"
print "Flask VTK->images->movie server version " + version

@app.route('/')
def api_root():
    return "Hello! This is Flask VTK->images->movie server version " + version + "."


if __name__ == '__main__':
    app.run()


@app.route('/files_', methods = ['POST'])
def receive_files_():
    if request.headers['Content-Type'] == 'application/octet-stream' :
        files = ""
        for f in request.files:
            files += str(f) + ", "
        f = open("./binary",'wb')
        f.write(request.data)
        f.close()
        return "File ./binary written. Files: " + files
    else:
        return "415 Unsupported Media Type"

@app.route('/files', methods = ['POST'])
def receive_files():
    response = ""
    if request.files is not None:
        file = request.files['file']
        filename = file.filename
        print "Accessed /files endpoint. Request files written =" + filename
        file.save(filename)
        untar(filename)
        print "Calling make_images.py"
        make_images.make_images()
        print "export directory should be created."
        print "Read images with pattern export/cavity_%04d.png"
        call("./make_movie.sh")
        movie_fname = "out.mp4"
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
