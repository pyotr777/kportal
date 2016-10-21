import numpy as np
from vtk.util.numpy_support import vtk_to_numpy
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from scipy.interpolate import griddata
import vtk
from mpl_toolkits.mplot3d import Axes3D
import os


def getData_vtk(filename):
    # load a vtk file as input
    reader = vtk.vtkUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    print reader.GetOutput().GetPoints()
    # Get the coordinates of nodes in the mesh
    nodes_vtk_array= reader.GetOutput().GetPoints().GetData()
    pressure_vtk_array = reader.GetOutput().GetPointData().GetArray(0)
    speed_vtk_array = reader.GetOutput().GetPointData().GetArray(1)
    nodes = vtk_to_numpy(nodes_vtk_array)
    p = vtk_to_numpy(pressure_vtk_array)
    U  = vtk_to_numpy(speed_vtk_array)
    return nodes, p, U

def extractFlatData(nodes, p, U):
    merged = np.empty([len(nodes),6])
    merged[:,0] = p
    merged[:,1:3] = U[:,:2]
    merged[:,3:] = nodes
    # Merged z0 and z1 arrays should be same because of
    # the model symmetry along z-axis
    merged_z0 = merged[np.where(merged[:,5] == 0)]
    merged_z1 = merged[np.where(merged[:,5] != 0)]
    # Extract nodes, p and U arrays from z0 array
    p = merged_z0[:,0]
    U = merged_z0[:,1:3]
    nodes = merged_z0[:,3:5]
    return nodes, p, U

def plot2D(nodes, a, vmin, vmax):
    # 2d plotting
    fig = plt.figure(figsize=(8,6))
    color_map = plt.cm.get_cmap('viridis')
    axes = plt.gca()
    #axes.set_xlim([-.1,2.1])
    #axes.set_ylim([-.01,0.11])
    #plt.axis('off')
    sc = plt.scatter(nodes[:,0],nodes[:,1],
                     s=320,
#                     color=cmap(a / a_max),
                     c = a,
                     cmap = color_map,
                     vmin = vmin,
                     vmax = vmax,
                     linewidth=0,
                     marker="s")
    plt.colorbar(sc)
    fig.tight_layout()

def plotVector(X, Y, UN, VN, U, vmin, vmax, filename=""):
    plot1 = plt.figure(figsize=(14,10))
    plt.quiver(X, Y, UN, VN,        # data
               U,                   # colour the arrows based on this array
               cmap=plt.cm.plasma,     # colour map
               norm=mpl.colors.Normalize(vmin=vmin,vmax=vmax), # Colour range fix
               headlength=4,
               width=0.007,
               scale_units='xy',
               scale=9,
               linewidths=1,
               edgecolors='k'
               )

    plt.colorbar()                  # adds the colour bar
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Velocity vector')
    if (filename != ""):
        plot1.tight_layout()
        plt.savefig(filename,bbox_inches='tight')
    else:
        plt.show(plot1)                 # display the plot
    plt.close(plot1)

vtkdir = "VTK"
vmin = 0
vmax = 1
imdir = "export"

def make_images():
    try:
        os.stat(imdir)
    except:
        os.mkdir(imdir)

    n = 0
    for i in range(0,501,50):
        print i
        filename = vtkdir+"/cavity_"+str(i) + ".vtk"
        im_filename = imdir + "/" + "cavity_" + str(n).zfill(4) + ".png"
        print "From {} to {}".format(filename, im_filename)
        n += 1

        nodes, p , U   = getData_vtk(filename)
        nodes_half, p_half, U_half = extractFlatData(nodes, p, U)
        Un_half = np.sqrt(U_half[:,0]**2 + U_half[:,1]**2) #Randomisation:  + np.random.rand(U_half.shape[0])
        print(np.amin(Un_half),np.amax(Un_half))

        plotVector(nodes_half[:,0], nodes_half[:,1], U_half[:,0], U_half[:,1],
                   Un_half, vmin, vmax, filename = im_filename)

if __name__ == "__main__":
    make_images()
