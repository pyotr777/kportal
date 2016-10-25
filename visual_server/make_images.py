# Produce image series from simulation data in VTK format


import numpy as np
from vtk.util.numpy_support import vtk_to_numpy
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from scipy.interpolate import griddata
import vtk
from mpl_toolkits.mplot3d import Axes3D
import os

def getData(filename):
    # load a vtk file as input
    reader = vtk.vtkUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    vtk_arrays= reader.GetOutput().GetPointData()
    #print vtk_nodes
    #print vtk_arrays
    return vtk_arrays

def getNodes(filename):
    # load a vtk file as input
    reader = vtk.vtkUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    points = reader.GetOutput().GetPoints()
    if points is None:
        return None
#    print points
#    Get the coordinates of nodes in the mesh
    nodes_vtk_array= reader.GetOutput().GetPoints().GetData()
    nodes = vtk_to_numpy(nodes_vtk_array)
    return nodes

# Remove data for z != 0
def extractFlatData(nodes, a, U):
    merged = np.empty([len(nodes),6])
    merged[:,0] = a
    merged[:,1:3] = U[:,:2]
    merged[:,3:] = nodes
    # Merged z0 and z1 arrays should be same because of
    # the model symmetry along z-axis
    merged_z0 = merged[np.where(merged[:,5] == 0)]
    merged_z1 = merged[np.where(merged[:,5] != 0)]
    # Extract nodes, p and U arrays from z0 array
    a = merged_z0[:,0]
    U = merged_z0[:,1:3]
    nodes = merged_z0[:,3:5]
    return nodes, a, U

# Remove data for |U| == 0 and z != 0
def extractNonzeroData(nodes, a, U):
    merged = np.empty([len(nodes),7])
    print "Extrcting |U|!=0.\na:{} U:{} nodes:{}".format(a.shape, U.shape, nodes.shape)
    merged[:,0] = a
    merged[:,1:3] = U[:,:2]
    merged[:,3:6] = nodes
    # U length squared ^2
    merged[:, 6] = np.power(U[:,0],2) + np.power(U[:,1],2)
    merged_nonzero = merged[np.where(merged[:,6] > 0.01)]
    merged_nonzero = merged_nonzero[np.where(merged_nonzero[:,5] == 0)]
    print "Nonzero matrix shape: {}".format(merged_nonzero.shape)
    # Extract nodes, p and U arrays from z0 array
    a = merged_nonzero[:,0]
    U = merged_nonzero[:,1:3]
    nodes = merged_nonzero[:,3:5]
    return nodes, a, U

# Plot vector field on existing figure
def plotVector_combined(nodes, U, vmin, vmax, filename=""):
    X = nodes[:,0]
    Y = nodes[:,1]
    UN = U[:,0]
    VN = U[:,1]
    stride_elmts = 5
    Xsub = X[::stride_elmts]
    Ysub = Y[::stride_elmts]
    UNsub= UN[::stride_elmts]
    VNsub= VN[::stride_elmts]

    plt.quiver(Xsub, Ysub, UNsub, VNsub,        # data
               np.hypot(UNsub, VNsub),
 #              Usub,                   # colour the arrows based on this array
 #              color='#ffee77',
               cmap=plt.cm.YlOrRd,
               norm=mpl.colors.Normalize(vmin=vmin,vmax=vmax), # Colour range fix
               linewidth=0.5,
               scale=10
               )

    plt.colorbar()                  # adds the colour bar

# Plot 2D data on existing figure
def plot2D_combined(nodes, a, vmin, vmax):
    cmap = mpl.cm.seismic
    color_map = plt.cm.get_cmap('GnBu')
    sc = plt.scatter(nodes[:,0],nodes[:,1],
                     s=550,
                     c = a,
                     cmap = color_map,
                     vmin = vmin,
                     vmax = vmax,
                     linewidth=0,
                     marker="o")


# Generate source data file name and
# image file name for exporting visualisation frame.
def generateFilename(base, vtkdir, imdir, i, n):
    source = os.path.join(vtkdir, base + str(i) + ".vtk")
    img = os.path.join(imdir,"img_"+str(n).zfill(4)+".png")
    return source, img


vtkdir = "VTK"
imdir = "export"
filename_base = "mixerVessel2D_"
max_i = 500

# Main function that reads source files and produces images
def make_images():
    try:
        os.stat(imdir)
    except:
        os.mkdir(imdir)

    n = 0   # image counter
    for i in range(0,max_i+1):
        src_file, img_file = generateFilename(filename_base, vtkdir, imdir, i, n)
        # print "Looking for " + src_file
        if os.path.isfile(src_file):
            print "Found " + src_file
            n += 1
            filename = src_file
            vtk_data = getData(filename)
            nodes = getNodes(filename)
            alpha = vtk_to_numpy(vtk_data.GetArray(1))
            U = vtk_to_numpy(vtk_data.GetArray(3))
            a = alpha

            vmin = np.nanmin(a)
            vmax = np.nanmax(a)
            print "Alpha range: {} - {}, shape: {}".format(vmin,vmax, a.shape)

            nodes_half, a_half, U_half = extractFlatData(nodes, a, U)
            nodes_nonzero, a_nonzero, U_nonzero = extractNonzeroData(nodes, a, U)

            # Plot combined figure
            plt.figure(figsize=(14,12))
            axes = plt.gca()
            plt.axis([-.1, .1, -.1, .1])
            plt.xlabel('X')
            plt.ylabel('Y')
            plt.axis('off')

            plt.title('Alpha water and Velocity vector')
            plot2D_combined(nodes_half, a_half, vmin, vmax)
            plotVector_combined(nodes_nonzero, U_nonzero, 0, 0.4)

            plt.savefig(img_file,bbox_inches='tight')
            plt.close()


if __name__ == "__main__":
    make_images()


