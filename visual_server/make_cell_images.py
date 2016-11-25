# Produce image series from simulation data in VTK format


import numpy as np
from vtk.util.numpy_support import vtk_to_numpy
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from scipy.interpolate import griddata
import vtk
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import Polygon, Circle, RegularPolygon
from matplotlib.collections import PatchCollection
import os, sys


def getData_2(filename):
    # Read CELL_DATA from vtk file
    reader = vtk.vtkUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    vtk_points_arrays= reader.GetOutput().GetPointData()
    vtk_cell_arrays  = reader.GetOutput().GetCellData()
    return vtk_points_arrays, vtk_cell_arrays

def getNodes_2(filename):
    # Read CELL from vtk file
    reader = vtk.vtkUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    points = reader.GetOutput().GetPoints()
    cells  = reader.GetOutput().GetCells()
    if points is None:
        return None

#    Get the coordinates of nodes in the mesh
    points_vtk_array= points.GetData()
    cells_vtk_array = cells.GetData()
    points = vtk_to_numpy(points_vtk_array)
    cells  = vtk_to_numpy(cells_vtk_array)
    return points, cells

# Translate XYZ coordinates to XY plane
def projectToXY(point):
    x,y = point[:2]
    y += point[2]/5
    return [x,y]

# Translate 3D Hexahedron (8 points) to a poligon on XY plane
def hexahedronToXY(points):
    z0_points=[]
    z1_points=[]
    for point in points:
        xy_point = projectToXY(point)
        if point[2] > 0:
            z1_points.append(xy_point)
        else:
            z0_points.append(xy_point)
    z1 = [z1_points[0], z1_points[1], z1_points[3], z1_points[2]]
    z0 = [z0_points[0], z0_points[1], z0_points[3], z0_points[2]]
    return np.array(z0), np.array(z1)


def plot2D(patches, values, vmin, vmax):
    color_map = plt.cm.get_cmap('plasma_r')
    p = PatchCollection(patches, cmap=color_map, edgecolor="#ffffff", linewidth=0)
    colors = values
    p.set_array(np.array(colors))
    ax = plt.axes()
    ax.add_collection(p)
    #plt.colorbar(p)
    p.set_clim([vmin, vmax])


vtkdir = "sample/VTK"
imdir = "export"
filename_base = "sample_"
max_i = 800

def make_image_cells(vtk_fname, imn):
    try:
        os.stat(imdir)
    except:
        print "Creating export dir."
        os.mkdir(imdir)
    img_file = os.path.join(imdir,"img_"+str(imn).zfill(4)+".png")
    print "Making image "+ img_file + " from " + vtk_fname
    filename = vtk_fname
    try:
        vtk_points, vtk_cells = getData_2(filename)
        cells_idx = vtk_to_numpy(vtk_cells.GetArray(0))
        alpha = vtk_to_numpy(vtk_cells.GetArray(2))
        points, cells = getNodes_2(filename)
    except:
        print "Error parcing " + vtk_fname,  sys.exc_info()[0]
        return

    vmin = np.nanmin(alpha)
    vmax = np.nanmax(alpha)
    #print "Alpha range: {} - {}, shape: {}".format(vmin,vmax, alpha.shape)

    patches = []
    values = []

    N = len(cells)
    i = 0
    block = 0
    while i < N:
        block += 1
        n = cells[i]  # Number of points in this cell
        i += 1
        cell_points = np.take(points,cells[i:i+n],axis=0)
        i += n
        if n != 8:
            print "Not hexahedron! " + str(n)
            continue

        values.append(alpha[cells_idx[block-1]])
        face0, face1 = hexahedronToXY(cell_points)
        polygon1 = Polygon(face0, True)
        try:
            patches.append(polygon1)
        except:
            pass

    # Plot combined figure
    plt.figure(figsize=(4,4))
    axes = plt.gca()
    plt.axis([-.15, .15, -.15, .15])
    plt.xlabel('X (m)')
    plt.ylabel('Y (m)')
    #plt.axis('off')
    axes.spines['left'].set_color('white')
    axes.spines['right'].set_color('white')
    axes.spines['top'].set_color('white')
    axes.spines['bottom'].set_color('white')

    #plt.title('Liquid alpha')
    plot2D(patches, values, -0.2, +1.1)
    plt.savefig(img_file,bbox_inches='tight',dpi=150)
    plt.close()



# Main function that reads source files and produces images
def make_cell_images():
    try:
        os.stat(imdir)
    except:
        print "Creating export dir."
        os.mkdir(imdir)

    print "Files in " + vtkdir
    for f in os.listdir(vtkdir):
        print os.path.join(vtkdir,f)
    n = 0   # image counter
    for i in range(0,max_i+1):
        src_file, img_file = generateFilename(filename_base, vtkdir, imdir, i, n)
        #print "Looking for " + src_file
        if os.path.isfile(src_file):
            print "Found " + src_file
            make_image_cells(src_file, n)
            n += 1


if __name__ == "__main__":
    make_cell_images()


