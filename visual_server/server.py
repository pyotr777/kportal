from flask import Flask, url_for
from flask import request
from flask import send_file
app = Flask(__name__)

@app.route('/')
def api_root():
    return "Hello!"


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
        file.save(filename)
    print "Accessed /files endpoint. Request files written =" + filename
    return send_file(filename, mimetype='image/png')

