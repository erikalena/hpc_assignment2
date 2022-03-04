import numpy as np
import matplotlib
from matplotlib import pyplot as plt

class Node:
    def __init__(self, x, y, axis, level):
        self.l = None
        self.r = None
        self.x = x
        self.y = y
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
        else:
            cmp_val = node.y < root.y
            
        if cmp_val:
            if root.l is not None:
                self._insert(node, root.l, (axis+1)%2)
            else:
                root.l = node
        else:
            if root.r is not None:
                self._insert(node, root.r, (axis+1)%2)
            else:
                root.r = node

  
    def viewTree(self, xmin, xmax, ymin,ymax):
        if self.root is not None:
            self._viewTree(self.root, xmin, xmax, ymin, ymax)

    def _viewTree(self, node, xmin, xmax, ymin, ymax):
        if node is not None:
            if(node.axis == 0):
                plt.vlines(x=node.x, ymin = ymin, ymax = ymax, color='#eb348c', linestyle='-')
                self._viewTree(node.l, xmin, node.x, ymin, ymax)
                self._viewTree(node.r, node.x, xmax, ymin, ymax)
            else:
                plt.hlines(y=node.y, xmin = xmin, xmax = xmax, color='#eb348c', linestyle='-')
                self._viewTree(node.l, xmin, xmax, ymin, node.y)
                self._viewTree(node.r, xmin, xmax, node.y, ymax)
            

if __name__ == "__main__":

    root = Tree()
    x = []
    y = []
    
    with open('../final_kdtree.txt') as kdtree:
        for l in kdtree:
            level = l.count('\t')
            axis = level%2
            val = float(l.split(',')[axis].strip().replace('(','').replace(')',''))
            xp = float(l.split(',')[0].strip().replace('(',''))
            yp = float(l.split(',')[1].strip().replace(')',''))
            
            x.append(xp)
            y.append(yp)
            
            n = Node(xp,yp,axis,level)
            root.insert(n, 0)
    
           
    root.viewTree(min(x), max(x), min(y), max(y))
    plt.scatter(x,y,zorder=2)
    plt.show()
