import numpy as np
import matplotlib
from matplotlib import cm
from matplotlib import pyplot as plt

class Node:
    def __init__(self, x, y, z, axis, level):
        self.l = None
        self.r = None
        self.x = x
        self.y = y
        self.z = z
        self.axis = axis
        self.level = level

class Tree:
    def __init__(self):
        self.root = None

    def getRoot(self):
        return self.root

    def insert(self, node, axis):
        if self.root is None:
            self.root = node
        else:
            self._insert(node, self.root, axis)

    def _insert(self, node, root, axis):
        if axis == 0:
            cmp_val = node.x < root.x
        elif axis == 1:
            cmp_val = node.y < root.y
        else:
            cmp_val = node.z < root.z
            
        if cmp_val:
            if root.l is not None:
                self._insert(node, root.l, (axis+1)%3)
            else:
                root.l = node
        else:
            if root.r is not None:
                self._insert(node, root.r, (axis+1)%3)
            else:
                root.r = node

  
    def viewTree(self, xmin, xmax, ymin,ymax,zmin,zmax):
        if self.root is not None:
            self._viewTree(self.root, xmin, xmax, ymin, ymax, zmin, zmax)

    def _viewTree(self, node, xmin, xmax, ymin, ymax, zmin, zmax):
        if node is not None:
            if(node.axis == 0):
                zz, yy = np.meshgrid(np.arange(zmin, zmax,0.01), np.arange(ymin, ymax,0.01))
                xx = (zz*0 + yy* 0 + node.x)
                ax.plot_surface(xx,yy,zz,alpha=0.5)
                self._viewTree(node.l, xmin, node.x, ymin, ymax, zmin, zmax)
                self._viewTree(node.r, node.x, xmax, ymin, ymax, zmin, zmax)
            elif(node.axis == 1):
                xx, zz = np.meshgrid(np.arange(xmin, xmax,0.01), np.arange(zmin, zmax,0.01))
                yy = (xx*0 + zz* 0 + node.y)
                ax.plot_surface(xx,yy,zz,alpha=0.5)
                self._viewTree(node.l, xmin, xmax, ymin, node.y,zmin, zmax)
                self._viewTree(node.r, xmin, xmax, node.y, ymax,zmin, zmax)
            else:
                xx, yy = np.meshgrid(np.arange(xmin, xmax,0.01), np.arange(ymin, ymax,0.01))
                zz = (xx*0 + yy* 0 + node.z)
                ax.plot_surface(xx,yy,zz,alpha=0.5)
                self._viewTree(node.l, xmin, xmax, ymin, ymax, zmin, node.z)
                self._viewTree(node.r, xmin, xmax, ymin, ymax, node.z, zmax)

if __name__ == "__main__":

    root = Tree()
    x = []
    y = []
    z = []

    with open('final_kdtree.txt') as kdtree:
        for l in kdtree:
            level = l.count('\t')
            axis = level%3
            val = float(l.split(',')[axis].strip().replace('(','').replace(')',''))
            xp = float(l.split(',')[0].strip().replace('(',''))
            yp = float(l.split(',')[1].strip())
            zp = float(l.split(',')[2].strip().replace(')',''))
            
            x.append(xp)
            y.append(yp)
            z.append(zp)
           
            n = Node(xp,yp, zp, axis,level)
            root.insert(n, 0)
    
           

    ax = plt.axes(projection='3d')	
    root.viewTree(min(x), max(x), min(y), max(y), min(z), max(z))
    ax.scatter3D(x,y,z, cmap='Greens')

    for i,j,k in zip(x,y,z):
        ax.text(i,j,k,f"({i},{j},{k})")
        
    plt.show()
