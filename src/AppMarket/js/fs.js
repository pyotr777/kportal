<!DOCTYPE html> 
<html> 

<script>
function onInitFs(fs) {

  fs.root.getFile('log.txt', {create: false}, function(fileEntry) {

    // Create a FileWriter object for our FileEntry (log.txt).
    fileEntry.createWriter(function(fileWriter) {

      fileWriter.seek(fileWriter.length); // Start write position at EOF.
      fileWriter.truncate(1);

      // Create a new Blob and write it to log.txt.
      var bb = new BlobBuilder(); // Note: window.WebKitBlobBuilder in Chrome 12.
      bb.append('Hello World');
      fileWriter.write(bb.getBlob('text/plain'));

    }, errorHandler);

  }, errorHandler);

}
alert("hihi");

window.requestFileSystem(window.PERSISTENT, 1024*1024, onInitFs, errorHandler);
</script>
<button value="hihi"/>
</html> 