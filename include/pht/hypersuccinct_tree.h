#ifndef PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H
#define PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H

#include <iostream>
#include <functional>
#include <utility>

#include "unordered_tree.h"
#include "list_utils.h"
#include "farzan_munro.h"
#include "bitvector_utils.h"

namespace pht {
    typedef std::vector<bool> Bitvector;
    //node identification by: Minitree, MicroTree, NodeInMicroTree
    typedef std::tuple<uint32_t ,uint32_t ,uint32_t > HstNode;

    enum class TreeTypes {
        MINI, MICRO
    };

    /**
     * MiniTree represents MiniTree of the HypersuccinctTree
     * It contains all information needed to query a single MiniTree.
     */
    struct MiniTree {
        //MircoTrees as encoded (BP if no encoding, huffman code if huffman encoding)
        Bitvector microTrees;
        //MicroFIDs
        Bitvector FIDs;
        //MicroTypeVectors
        Bitvector typeVectors;
        //MicroDummys
        Bitvector dummys;
        //Is MicroTree root ancestor of MiniTreeDummy? empty/0 if no MiniDummy exists
        Bitvector rootAncestors;
        //Is MicroTreeDummy ancestor of MiniTreeDummy? 0 per entry if no MicroDummy exists, empty/0 if no MiniDummy exists
        Bitvector dummyAncestors;
        //If MiniTree has Dummy: Which MicroTree contains this Dummy?
        Bitvector miniDummyTree;
        //If MiniTree has Dummy: Which Index within the MicroTree is this Dummy?
        Bitvector miniDummyIndex;
        //If MiniTree has Dummy: To which Tree does the pointer lead?
        Bitvector miniDummyPointer;
        //Ancestor matrix for MicroTrees
        //MicroTree Dummy Pointer
        Bitvector microDummyPointers;
        //Ancestor of MiniTreeRoot
        Bitvector miniAnc;
        //SubTree Size MiniTree
        Bitvector subTree;
        //SubTree Size for MicroTree roots
        Bitvector microSubTrees;
        //Depth of the MiniTree root
        Bitvector miniDepth;
        //Height of the MiniTree root
        Bitvector miniHeight;
        //Depth of the MiniTree Dummy
        Bitvector miniDummyDepth;
        //Height of the MiniTree Dummy
        Bitvector miniDummyHeight;
        //Depths for MicroTree roots + 1
        Bitvector rootDepths;
        //Heights for MicroTree roots + 1
        Bitvector rootHeights;

        //TODO:
        //Leftmost Leaf Pointer for MiniTree
        Bitvector miniTreeLeftmostPointer;
        //Rightmost Leaf Pointer for MiniTree
        Bitvector miniTreeRightmostPointer;
        //Leftmost Leaf Pointers for MicroTrees
        Bitvector microTreeLeftmostPointers;
        //Rightmost Leaf Pointers for MicroTrees
        Bitvector microTreeRightmostPointers;

        //Amount of Leaves in MiniTree
        Bitvector miniLeaves;
        //Amount of Leaves within MicroTrees
        Bitvector microLeaves;


        //TODO: Remove
        Bitvector ancMatrix;
    };

    /**
     * LookupTableEntry represents a single Entry of the Hypersuccinct Tree's Lookp Table.
     * It is indexed by the MicroTrees Balanced Parenthesis form (if no encoding is chosen) or by their Huffman code (if Huffman encoding is chosen)
     * It contains all fields necessary to satisfy the query's need for structural information
     */
    struct LookupTableEntry {
        //Index of the LookupTableEntry
        Bitvector index;
        //BP of the Entry. Empty if index is BP
        Bitvector bp;
        //Ancestor matrix
        Bitvector matrix;
        //degree for every node + 1
        Bitvector degree;
        //subTree for every node within MicroTree (at least 1)
        Bitvector subTrees;
        //Depths of nodes + 1
        Bitvector nodeDepths;
        //Heights of nodes + 1
        Bitvector nodeHeights;
        //Amount of Leaves for every node within MicroTree (at least 1)
        Bitvector leaves;


        //TODO: This constructor is specifically for HypersuccinctTreeFactory - could be removed
        LookupTableEntry(const Bitvector& index, const Bitvector& matrix) : index(index), matrix(matrix) {}
        //TODO: Extend constructor as more fields are added!
        LookupTableEntry(const Bitvector& index, const Bitvector& bp, const Bitvector& matrix) : index(index), bp(bp), matrix(matrix) {}
        bool operator==(const LookupTableEntry& mtd) const {
            return index == mtd.index;
        }
    };


    /**
     * This class represents the full hypersuccinct code for any given tree, created by HypersuccinctTreeFactory
     * as specified in 'A Uniform Paradigm to Succinctly Encode Various Families of Trees' by Arash Farzan; J. Ian Munro.
     * All code is represented as Bitvectors
     * It can be encoded with huffman encoding for MicroTrees
     * TODO: Need Complexity for all functions
     *
     * It contains:
     * The FarzanMunro Algorithm Sizes of MiniTrees and MicroTrees
     * The MiniTrees as a vector of MiniTree
     * The MiniTree FIDs, TypeVectors, and Dummys
     * The LookupTable as a vector of LookUpTableEntry
     *
     * This class implements:
     * Get functions for all Bitvectors
     * Queries as specified in 'A Uniform Paradigm to Succinctly Encode Various Families of Trees' by Arash Farzan; J. Ian Munro
     */
    class HypersuccinctTree {
        friend class HypersuccinctTreeFactory;
    public:

        /**
         * Returns the MiniTree at the given index
         *
         * @param index The Index of the MiniTree as integer
         * @return MiniTree as MiniTree
         */
        MiniTree getMiniTree(uint32_t index ) {
            return miniTrees.at(index);
        }

        bool isHuffman() {
            return huffmanFlag;
        }

        Bitvector getSize() {
            return size;
        }

        Bitvector getMicroSize() {
            return microSize;
        }

        Bitvector getMiniSize() {
            return miniSize;
        }

        std::vector<MiniTree> getMiniTrees() {
            return miniTrees;
        }

        Bitvector getMiniFIDs() {
            return miniFIDs;
        }

        Bitvector getMiniTypeVectors() {
            return miniTypeVectors;
        }

        Bitvector getMiniDummys() {
            return miniDummys;
        }

        Bitvector getMiniAncMatrix() {
            return miniAncMatrix;
        }

        std::vector<LookupTableEntry> getLookupTable() {
            return lookupTable;
        }

        /**
         * Returns the MicroTree Entry of the given MiniTree at the given MicroTree Index
         * Always returns Balanced parenthesis form of the MicroTree, regardless of encoding (Huffman or not)
         *
         * @param miniTree The MiniTree of the MicroTree
         * @param index The index of the MicroTree as int
         * @return the MicroTree in Balanced Parenthesis form as bitvector
         */
        Bitvector getMicroTree(MiniTree& miniTree,uint32_t index);

        /**
         * Returns the MicroFID Entry of the given MiniTree at the given MicroTree Index
         *
         * @param miniTree The MiniTree of the MicroFID
         * @param index The index of the MicroTree as int
         * @return the FID Entry as bitvector
         */
        Bitvector getMicroFID(MiniTree& miniTree,uint32_t index);

        /**
         * Returns the TypeVector Entry of the given MiniTree at the given MicroTree Index
         *
         * @param miniTree The MiniTree of the TypeVector
         * @param index The index of the MicroTree es int
         * @return the Typevector Entry as bitvector
         */
        Bitvector getMicroTypeVector(MiniTree& miniTree , uint32_t index);

        /**
         * Returns the MicroDummy Entry from the perspective of the MicroTree
         *
         * @param miniTree The MiniTree of the Dummy
         * @param index The index of the MicroTree as int
         * @return the Dummy Entry as bitvector
         */
        Bitvector getMicroDummys(MiniTree& miniTree, uint32_t index);

        /**
         * Returns the MiniDummy Entry from the perspective of the MiniTree
         *
         * @param index The index of the MiniTree as int
         * @return The Dummy Entry as bitvector
         */
        Bitvector getMiniDummy(uint32_t index);

        /**
         * Returns the LookupTable Entry at the given index
         *
         * @param index The index as int
         * @return LookupTable Entry as LookupTableEntry
         */
        LookupTableEntry getLookupTableEntry(uint32_t index) {
            return lookupTable.at(index);
        }

        /**
         * Returns the LookupTable Entry of the given MicroTree
         *
         * @param indexV The MicroTree as bitvector from a miniTree
         * @return LookupTable Entry of the MicroTree as LookupTableEntry
         */
        LookupTableEntry getLookupTableEntry(Bitvector indexV);

        /**
         * Returns if node1 is ancestor of node2 with the given LookupTable Entry
         * Both nodes only need their index inside the MicroTree (HstNode index 2)
         *
         * @param entry The LookupTable Entry
         * @param anc Index of Node 1
         * @param node2Index Index of Node 2
         * @return if node1 is ancestor of node2 as bool
         */
        bool lookupTableMatrixComparison(const LookupTableEntry& entry, uint32_t anc, uint32_t node2Index);

        /**
         * Returns the FID for a given MiniTree
         *
         * @param treeNum Number of the Tree as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMiniTree(uint32_t treeNum);

        /**
         * Returns the FID for a given MicroTree within a MiniTree
         *
         * @param miniTree The Minitree as Minitree
         * @param treeNum the Microtree number as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMicroTree(MiniTree &miniTree, uint32_t treeNum);

        /**
         * Returns the FID for a given MicroTree within a MiniTree
         *
         * @param miniTree The Minitree number as int
         * @param treeNum the Microtree number as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMicroTree(uint32_t miniTree, uint32_t treeNum);

        ////////////////////////////////////////////////////////////////////////////
        // Test Methods in this Block
        //
        //
        ////////////////////////////////////////////////////////////////////////////

        bool miniTreeAncMatrixComparison(uint32_t ancTree, uint32_t treeIndex2);

        bool microTreeAncMatrixComparison(const MiniTree& miniTree, uint32_t ancTree, uint32_t treeIndex2);

        /**
         * TODO: This method calls the others (like in bitvector utils)
         * @param type
         * @param ancestor
         * @param treeNum
         * @return
         */
        Bitvector getFidforTree(TreeTypes type, bool ancestor, uint32_t treeNum);

        Bitvector getParentFIDMiniTree(uint32_t treeNum);

        uint32_t getParentMiniTree(uint32_t treeNum);

        /**
         * TODO: Most likely unnecessary
         * @param node
         * @param anc
         * @return
         */
        bool isAncestor(HstNode node, HstNode anc);

        ////////////////////////////////////////////////////////////////////////////
        //
        //
        //End Test Method Block
        ////////////////////////////////////////////////////////////////////////////

        /**
         * Returns if given Node is ancestor of Dummy within the Node's MiniTree
         *
         * @param node The Node as HstNode
         * @return if Node is ancestor of MiniDummy as bool
         */
        bool isDummyAncestorWithinMiniTree(HstNode node);

        /**
         * Returns if  given Node is ancestor of Dummy within the Node's MicroTree
         *
         * @param node  The Node as HstNode
         * @return if Node is ancestor of MicroDummy as bool
         */
        bool isDummyAncestorWithinMicroTree(HstNode node);

        /**
         * Returns the degree of a given Node
         *
         * @param node The Node as HstNode
         * @return the degree as int
         */
        uint32_t degree(HstNode node);

        /**
         * Returns the Subtree size of a given Node
         *
         * @param node The Node as HstNode
         * @return Subtree size as int
         */
        uint32_t subtree_size(HstNode node);

        /**
         * Returns the Depth of a given Node
         *
         * @param node The Node as HstNode
         * @return The Depth as uint32_t
         */
        uint32_t depth(HstNode node);

        /**
         * Returns the Depth of a given Node
         *
         * @param node The Node as HstNode
         * @return The Depth as uint32_t
         */
        uint32_t height(HstNode node);

        /**
         * Returns the Leaf size of a given Node
         *
         * @param node The Node as HstNode
         * @return the Leaf Size as uint32_t
         */
        uint32_t leaf_size(HstNode node);

        /**
         * TODO: Unfinished
         *
         * @param level
         * @param node
         * @return
         */
        HstNode levelAncestor(uint32_t level, HstNode node);

        /**
         * TODO: unfinished
         * @param node
         * @return
         */
        uint32_t childRank(HstNode node);

    private:
        HypersuccinctTree() = default;
        bool huffmanFlag;
        //sizes
        std::vector<bool> size;
        std::vector<bool> microSize;
        std::vector<bool> miniSize;
        //miniTrees
        std::vector<MiniTree> miniTrees;
        std::vector<bool> miniFIDs;
        std::vector<bool> miniTypeVectors;
        std::vector<bool> miniDummys;
        //TODO: Remove
        std::vector<bool> miniAncMatrix;
        //LookupTable
        std::vector<LookupTableEntry> lookupTable;
    };
}

#endif //PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H