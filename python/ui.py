import pht_hst

hst = pht_hst.PyHST("../resources/treeNath.xml", False, 12, 4)
print("hst.isDummyAncestorWithinMiniTree(1, 0, 0)): " + str(hst.isDummyAncestorWithinMiniTree(1, 0, 0)))
print("hst.isDummyAncestorWithinMicroTree(1, 0, 0): " + str(hst.isDummyAncestorWithinMicroTree(1, 0, 0)))
print("hst.child(1, 0, 0, 0): " + str(hst.child(1, 0, 0, 0)))
print("hst.childRank(1, 0, 0): " + str(hst.childRank(1, 0, 0)))
print("hst.getParent(1, 0, 0): " + str(hst.getParent(1, 0, 0)))
print("hst.degree(1, 0, 0): " + str(hst.degree(1, 0, 0)))
print("hst.subtreeSize(1, 0, 0): " + str(hst.subtreeSize(1, 0, 0)))
print("hst.depth(1, 0, 0): " + str(hst.depth(1, 0, 0)))
print("hst.height(1, 0, 0): " + str(hst.height(1, 0, 0)))
print("hst.leftmostLeaf(1, 0, 0): " + str(hst.leftmostLeaf(1, 0, 0)))
print("hst.rightmostLeaf(1, 0, 0): " + str(hst.rightmostLeaf(1, 0, 0)))
print("hst.leafSize(1, 0, 0): " + str(hst.leafSize(1, 0, 0)))
print("hst.leafRank(1, 0, 0): " + str(hst.leafRank(1, 0, 0)))