#distutils: language=c++

from cython.operator cimport dereference
from libcpp.memory cimport unique_ptr

from pht_xml_reader cimport XMLReader
from pht_unordered_tree cimport UnorderedTree
from pht_hst_factory cimport HypersuccinctTreeFactory
from pht_hst cimport HypersuccinctTree

from pht_hst_node cimport HstNode

cdef class PyHST:
    cdef unique_ptr[HypersuccinctTree] hst

    def __cinit__(self, path, huffman=False, sizeMini=0, sizeMicro=0):
        self.hst = HypersuccinctTreeFactory.create(XMLReader.read(path.encode('UTF-8')), huffman, sizeMini, sizeMicro)

    def isDummyAncestorWithinMiniTree(self, mini, micro, node):
        try:
            return dereference(self.hst).isDummyAncestorWithinMiniTree(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def isDummyAncestorWithinMicroTree(self, mini, micro, node):
        try:
            return dereference(self.hst).isDummyAncestorWithinMicroTree(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def child(self, mini, micro, node, index):
        try:
            cpp_hst_node = dereference(self.hst).child(HstNode(mini, micro, node), index)
            return (cpp_hst_node.mini, cpp_hst_node.micro, cpp_hst_node.node)
        except IndexError:
            print("Vector Index out of Bounds")

    def childRank(self, mini, micro, node):
        try:
            return dereference(self.hst).childRank(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def getParent(self, mini, micro, node):
        try:
            cpp_hst_node = dereference(self.hst).getParent(HstNode(mini, micro, node))
            return (cpp_hst_node.mini, cpp_hst_node.micro, cpp_hst_node.node)
        except IndexError:
            print("Vector Index out of Bounds")

    def degree(self, mini, micro, node):
        try:
            return dereference(self.hst).degree(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def subtreeSize(self, mini, micro, node):
        try:
            return dereference(self.hst).subtreeSize(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def depth(self, mini, micro, node):
        try:
            return dereference(self.hst).depth(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def height(self, mini, micro, node):
        try:
            return dereference(self.hst).height(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def leftmostLeaf(self, mini, micro, node):
        try:
            cpp_hst_node = dereference(self.hst).leftmostLeaf(HstNode(mini, micro, node))
            return (cpp_hst_node.mini, cpp_hst_node.micro, cpp_hst_node.node)
        except IndexError:
            print("Vector Index out of Bounds")

    def rightmostLeaf(self, mini, micro, node):
        try:
            cpp_hst_node = dereference(self.hst).rightmostLeaf(HstNode(mini, micro, node))
            return (cpp_hst_node.mini, cpp_hst_node.micro, cpp_hst_node.node)
        except IndexError:
            print("Vector Index out of Bounds")

    def leafSize(self, mini, micro, node):
        try:
            return dereference(self.hst).leafSize(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

    def leafRank(self, mini, micro, node):
        try:
            return dereference(self.hst).leafRank(HstNode(mini, micro, node))
        except IndexError:
            print("Vector Index out of Bounds")

#   def levelAncestor(self, mini, micro, node, level):
#        return dereference(self.hst).levelAncestor(HstNode(mini, micro, node), level)
