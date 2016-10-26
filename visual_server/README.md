# Visualisation server for K-portal

## API

Send HTTP POST request to "/files" with VTK.tar.gz file.

Recieve movie in MP4 format.

### Sample request

```$curl -F file=@"VTK.tar.gz" http://52.199.34.102:8080/files > movie.mp4```


## Workflow


<table>
  <tr>
    <th></th>
    <th>#</th>
    <th>Place</th>
    <th>Action</th>
    <th>Files</th>
    <th>notes</th>
  </tr>
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-portal</td>
  	<td></td>
  	<td>App files</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>0.</td>
  	<td>↓</td>
  	<td>Submit. Staging Step 1.</td>
  	<td></td>
  	<td>Submitting K-portal job.</td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-login</td>
  	<td></td>
  	<td>App files</td>
  	<td>Have to add +x permission to ./makeMesh, because permissions lost after stage-in Step 1. <br></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>1.</td>
  	<td>↓</td>
  	<td>pjsub. stage-in. Staging Step 2.</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-compute</td>
  	<td></td>
  	<td>/0<br>/1<br>...</td>
  	<td>Dirs created by OpenFOAM in directory sample/.</td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>2.</td>
  	<td>↓</td>
  	<td>foamToVTK</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-compute</td>
  	<td></td>
  	<td>/VTK<br>/0<br>/1<br>...</td>  	
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>3.</td>
  	<td>↓</td>
  	<td>stage-out. Step 2.</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-login</td>
  	<td></td>
  	<td>/VTK<br>...</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>4.</td>
  	<td>↓</td>
  	<td>post-job command. tar -czvf ...</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>K-login</td>
  	<td></td>
  	<td>VTK.tar.gz</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>5.</td>
  	<td>↓</td>
  	<td>post- command. curl -F...</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>vis server</td>
  	<td></td>
  	<td>VTK.tar.gz</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>6.</td>
  	<td>↓</td>
  	<td>server.py. untar.</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>vis server</td>
  	<td></td>
  	<td>/VTK</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>7.</td>
  	<td>↓</td>
  	<td>server.py. make_images.py.</td>
  	<td></td>
  	<td></td>
  </tr>
  
  <tr>
  	<td></td>
  	<td></td>
  	<td>vis server</td>
  	<td></td>
  	<td>/export<br>img1.png<br>...</td>
  	<td></td>
  </tr>
  
  <tr>
  	<td>◯</td>
  	<td>8.</td>
  	<td>↓</td>
  	<td>sever.py. make_movie.sh.</td>
  	<td></td>
  	<td></td>
  </tr>
  
    <tr>
  	<td></td>
  	<td></td>
  	<td>vis server</td>
  	<td></td>
  	<td>out.mp4</td>
  	<td></td>
  </tr>
  
    <tr>
  	<td>◯</td>
  	<td>9.</td>
  	<td>↓</td>
  	<td>server respond</td>
  	<td></td>
  	<td></td>
  </tr>
  
    <tr>
  	<td></td>
  	<td></td>
  	<td>K-login</td>
  	<td></td>
  	<td>out.mp4</td>
  	<td></td>
  </tr>
  
    <tr>
  	<td>◯</td>
  	<td>10.</td>
  	<td>↓</td>
  	<td>post- command. mailx -a out.mp4...</td>
  	<td></td>
  	<td></td>
  </tr>
  
    <tr>
  	<td>◯</td>
  	<td></td>
  	<td>K-login</td>
  	<td></td>
  	<td>job files<br>out.mp4</td>
  	<td>After job finished takes some time to download all files to K-portal.<br>
  	<a href="https://github.com/pyotr777/kportal/issues/12">issue #12</a></td>
  </tr>
  
    <tr>
  	<td></td>
  	<td>11.</td>
  	<td>↓</td>
  	<td>Staging step 1.</td>
  	<td></td>
  	<td></td>
  </tr>
  
    <tr>
  	<td></td>
  	<td></td>
  	<td>K-portal</td>
  	<td></td>
  	<td>job files<br>out.mp4</td>
  	<td></td>
  </tr>
</table>

