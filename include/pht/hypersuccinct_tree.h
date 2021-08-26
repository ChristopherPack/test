#ifndef PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H
#define PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H

#include <iostream>
#include <functional>
#include <utility>

#include "unordered_tree.h"
#include "list_utils.h"
#include "farzan_munro.h"
#include "bitvector_utils.h"
#include "bit_vector.h"

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

namespace pht {
    typedef std::vector<bool> Bitvector;
    //node identification by: Minitree, MicroTree, NodeInMicroTree
    typedef std::tuple<uint32_t ,uint32_t ,uint32_t > HstNode;

    /**
     * MiniTree represents MiniTree of the HypersuccinctTree
     * It contains all information needed to query a single MiniTree.
     */
    struct __declspec(dllexport) MiniTree {
        //MicroFIDs
        Bitvector FIDs;
        succinct_bv::BitVector FIDsSupport;
        //MicroTypeVectors
        Bitvector typeVectors;
        succinct_bv::BitVector typeVectorsSupport;
        //MicroDummys: Static Size Encoding
        Bitvector dummys;
        succinct_bv::BitVector dummysSupport;
        //MircoTrees as encoded (BP if no encoding, huffman code if huffman encoding)
        Bitvector microTrees;
        succinct_bv::BitVector microTreesSupport;

        //MiniTree Top FID Index + 1
        Bitvector miniTopFIDIndex;
        succinct_bv::BitVector miniTopFIDIndexSupport;
        //MiniTree Low FID Index + 1
        Bitvector miniLowFIDIndex;
        succinct_bv::BitVector miniLowFIDIndexSupport;
        //MicroTree Top FID Indices + 1 + 1
        Bitvector microTopFIDIndices;
        succinct_bv::BitVector microTopFIDIndicesSupport;
        //MicroTree Low FID Indices + 1 + 1
        Bitvector microLowFIDIndices;
        succinct_bv::BitVector microLowFIDIndicesSupport;


        //Is MicroTree root ancestor of MiniTreeDummy? empty/0 if no MiniDummy exists
        Bitvector rootAncestors;
        succinct_bv::BitVector rootAncestorsSupport;
        //Is MicroTreeDummy ancestor of MiniTreeDummy? 0 per entry if no MicroDummy exists, empty/0 if no MiniDummy exists
        Bitvector dummyAncestors;
        succinct_bv::BitVector dummyAncestorsSupport;
        //If MiniTree has Dummy: Which MicroTree contains this Dummy?
        Bitvector miniDummyTree;
        succinct_bv::BitVector miniDummyTreeSupport;
        //If MiniTree has Dummy: Which Index within the MicroTree is this Dummy?
        Bitvector miniDummyIndex;
        succinct_bv::BitVector miniDummyIndexSupport;
        //If MiniTree has Dummy: To which Tree does the pointer lead?
        Bitvector miniDummyPointer;
        succinct_bv::BitVector miniDummyPointerSupport;
        //MicroTree Dummy Pointer: Static Size Encoding
        Bitvector microDummyPointers;
        succinct_bv::BitVector microDummyPointersSupport;
        //Ancestor of MiniTreeRoot
        Bitvector miniAnc;
        succinct_bv::BitVector miniAncSupport;
        //SubTree Size MiniTree
        Bitvector subTree;
        succinct_bv::BitVector subTreeSupport;
        //SubTree Size for MicroTree roots
        Bitvector microSubTrees;
        succinct_bv::BitVector microSubTreesSupport;
        //Depth of the MiniTree root
        Bitvector miniDepth;
        succinct_bv::BitVector miniDepthSupport;
        //Height of the MiniTree root
        Bitvector miniHeight;
        succinct_bv::BitVector miniHeightSupport;
        //Depth of the MiniTree Dummy
        Bitvector miniDummyDepth;
        succinct_bv::BitVector miniDummyDepthSupport;
        //Height of the MiniTree Dummy
        Bitvector miniDummyHeight;
        succinct_bv::BitVector miniDummyHeightSupport;
        //Depths for MicroTree roots + 1
        Bitvector rootDepths;
        succinct_bv::BitVector rootDepthsSupport;
        //Heights for MicroTree roots + 1
        Bitvector rootHeights;
        succinct_bv::BitVector rootHeightsSupport;
        //Amount of Leaves in MiniTree
        Bitvector miniLeaves;
        succinct_bv::BitVector miniLeavesSupport;
        //Amount of Leaves within MicroTrees
        Bitvector microLeaves;
        succinct_bv::BitVector microLeavesSupport;
        //Leftmost Leaf Pointer for MiniTree
        Bitvector miniTreeLeftmostLeafPointer;
        succinct_bv::BitVector miniTreeLeftmostLeafPointerSupport;
        //Rightmost Leaf Pointer for MiniTree
        Bitvector miniTreeRightmostLeafPointer;
        succinct_bv::BitVector miniTreeRightmostLeafPointerSupport;
        //Leftmost Leaf Pointers for MicroTrees
        Bitvector microTreeLeftmostLeafPointers;
        succinct_bv::BitVector microTreeLeftmostLeafPointersSupport;
        //Rightmost Leaf Pointers for MicroTrees
        Bitvector microTreeRightmostLeafPointers;
        succinct_bv::BitVector microTreeRightmostLeafPointersSupport;
        //Leaf Rank of MiniTree Root
        Bitvector miniRootLeafRank;
        succinct_bv::BitVector miniRootLeafRankSupport;
        //Leaf Rank of MiniTree Dummy
        Bitvector miniDummyLeafRank;
        succinct_bv::BitVector miniDummyLeafRankSupport;
        //Leaf Ranks of MicroTree Roots + 1
        Bitvector microRootLeafRanks;
        succinct_bv::BitVector microRootLeafRanksSupport;
    };

    /**
     * LookupTableEntry represents a single Entry of the Hypersuccinct Tree's Lookp Table.
     * It is indexed by the MicroTrees Balanced Parenthesis form (if no encoding is chosen) or by their Huffman code (if Huffman encoding is chosen)
     * It contains all fields necessary to satisfy the query's need for structural information
     */
    struct __declspec(dllexport) LookupTableEntry {
        //Index of the LookupTableEntry
        Bitvector index;
        succinct_bv::BitVector indexSupport;
        //BP of the Entry. Empty if index is BP
        Bitvector bp;
        succinct_bv::BitVector bpSupport;
        //Ancestor Matrix
        Bitvector ancestorMatrix;
        succinct_bv::BitVector ancestorMatrixSupport;
        //Child Matrix
        Bitvector childMatrix;
        succinct_bv::BitVector childMatrixSupport;
        //degree for every node + 1
        Bitvector degree;
        succinct_bv::BitVector degreeSupport;
        //subTree for every node within MicroTree (at least 1)
        Bitvector subTrees;
        succinct_bv::BitVector subTreesSupport;
        //Depths of nodes + 1
        Bitvector nodeDepths;
        succinct_bv::BitVector nodeDepthsSupport;
        //Heights of nodes + 1
        Bitvector nodeHeights;
        succinct_bv::BitVector nodeHeightsSupport;
        //Amount of Leaves for every node within MicroTree (at least 1)
        Bitvector leaves;
        succinct_bv::BitVector leavesSupport;
        //Rightmost leaves for every node within MicroTree
        Bitvector leftmost_leaf;
        succinct_bv::BitVector leftmost_leafSupport;
        //Leftmose leaves for every node within MicroTree
        Bitvector rightmost_leaf;
        succinct_bv::BitVector rightmost_leafSupport;
        //Leaf Rank for every node within MicroTree + 1
        Bitvector leafRank;
        succinct_bv::BitVector leafRankSupport;


        //TODO: This constructor is specifically for HypersuccinctTreeFactory - could be removed
        LookupTableEntry(const Bitvector& index) : index(index) {}
        //TODO: Extend constructor as more fields are added!
        LookupTableEntry(const Bitvector& index, const Bitvector& bp) : index(index), bp(bp) {}
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
    class __declspec(dllexport) HypersuccinctTree {
        friend class HypersuccinctTreeFactory;
    public:

        /**
         * Returns the MiniTree at the given index
         * O(1)
         *
         * @param index The Index of the MiniTree as integer
         * @return MiniTree as MiniTree
         */
        MiniTree& getMiniTree(uint32_t index ) {
            return miniTrees.at(index);
        }

        /**
         * Checks if the Tree is encoded with Huffman encoding
         * O(1)
         *
         * @return value of huffmanFlag
         */
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

        uint32_t getMicroTreeCount(MiniTree& miniTree);

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
         * Returns the MicroDummy Pointer Entry for a given MicroTree
         *
         * @param miniTree The MiniTree of the Dummy
         * @param index The index of the MicroTree
         * @return The Dummy Pointed MicroTree as int
         */
        Bitvector getMicroDummyPointers(MiniTree& miniTree, uint32_t index);

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
        bool lookupTableAncestorMatrixComparison(const LookupTableEntry& entry, uint32_t anc, uint32_t node2Index);

        /**
         * Returns if node1 is child of node2 with the given LookupTable Entry
         * Both nodes only need their index inside the MicroTree (HstNode index 2)
         *
         * @param entry The LookupTable Entry
         * @param anc Index of Node 1
         * @param node2Index Index of Node 2
         * @return if node1 is ancestor of node2 as bool
         */
        bool lookupTableChildMatrixComparison(const LookupTableEntry& entry, uint32_t child, uint32_t node2Index);

        /**
         * Returns the FID for a given MiniTree
         * This is more efficient than calling convert + getTrees for FID due to combining both into one loop
         *
         * @param treeNum Number of the Tree as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMiniTree(uint32_t treeNum);

        /**
         * Returns the FID for a given MicroTree within a MiniTree
         * This is more efficient than calling convert + getTrees for FID due to combining both into one loop
         *
         * @param miniTree The Minitree as Minitree
         * @param treeNum the Microtree number as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMicroTree(MiniTree &miniTree, uint32_t treeNum);

        /**
         * Returns the FID for a given MicroTree within a MiniTree
         * This is more efficient than calling convert + getTrees for FID due to combining both into one loop
         *
         * @param miniTree The Minitree number as int
         * @param treeNum the Microtree number as int
         * @return FID as Bitvector
         */
        Bitvector getFIDforMicroTree(uint32_t miniTree, uint32_t treeNum);

        /**
         * For a fiven FID index, finds all Tree Indices that belong to this FID
         * For MiniTrees
         *
         * @param index The index of the FID
         * @return The Type1 Tree Indices and Type2 Tree Indices in a Tuple of Vectors of uint32_t
         */
        std::pair< std::vector<uint32_t >,std::vector<uint32_t > > getTreesForFID(uint32_t index);

        /**
         * For a fiven FID index, finds all Tree Indices that belong to this FID
         * For MicroTrees
         *
         * @param miniTree The Minitree containing the FIDs
         * @param index The index of the FID
         * @return The Type1 Tree Indices and Type2 Tree Indices in a Tuple of Vectors of uint32_t
         */
        std::pair< std::vector<uint32_t >,std::vector<uint32_t > > getTreesForMicroFID(MiniTree &miniTree, uint32_t index);

        /**
         * Index conversion between tree indices and their fid inidces
         * For MiniTrees
         *
         * @param miniTree  The index of the miniTree
         * @return The indices of its Top and Low FID (if they exist)
         */
        std::pair<uint32_t ,uint32_t > convertTreeToFIDIndex(uint32_t miniTree);

        /**
         * Index conversion between tree indices and their fid inidces
         * For MicroTrees
         *
         * @param miniTree  the miniTree
         * @param microTree The index of the microTree
         * @return The indices of its Top and Low FID (if they exist)
         */
        std::pair<uint32_t ,uint32_t > convertMicroTreeToFIDIndex(MiniTree &miniTree, uint32_t microTree);



        ////////////////////////////////////////////////////////////////////////////
        // TODO: Test Methods in this Block
        //
        //
        ////////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////////
        //
        //
        //TODO: End Test Method Block
        ////////////////////////////////////////////////////////////////////////////

        /**
         * Returns if given Node is ancestor of Dummy within the Node's MiniTree
         * TODO: Look at the Micro Version - This can clearly be optimized
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
         * Returns the ith child of a given Node, if it exists
         *
         * @param parent The parent Node as HStNode
         * @param index The index as unint32_t
         * @return The ith child as HstNode
         */
        HstNode child(HstNode parent, uint32_t index);

        /**
         * Returns the Child Rank of a given Node
         *
         * @param node The Node as HstNode
         * @return The Child Rank as int
         */
        uint32_t child_rank(HstNode node);

        /**
         * Finds the direct Parent of the given Node
         * Very important helper function!
         * TODO: Could be private
         *
         * @param node The node as HSTNode
         * @return The parent as HstNode
         */
        HstNode getParent(HstNode node);

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
         * Returns the leftmost leaf of a given Node
         *
         * @param node The Node as HstNode
         * @return The leftmost leaf as HstNode
         */
        HstNode leftmost_leaf(HstNode node);

        /**
         * Returns the rightmost leaf of a given Node
         *
         * @param node The Node as HstNode
         * @return The rightmost leaf as HstNode
         */
        HstNode rightmost_leaf(HstNode node);

        /**
         * Returns the Leaf size of a given Node
         *
         * @param node The Node as HstNode
         * @return the Leaf Size as uint32_t
         */
        uint32_t leaf_size(HstNode node);

        /**
         * Returns the Leaf Rank of a given Node
         * Leaf Rank is the amount of Leaves coming before the Node, in node order
         *
         * @param node The Node as HstNode
         * @return the Leaf Size as uint32_t
         */
         uint32_t leaf_rank(HstNode node);

        /**
         * TODO: Unfinished
         *
         * @param level
         * @param node
         * @return
         */
        HstNode levelAncestor(uint32_t level, HstNode node);

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
        //LookupTable
        std::vector<LookupTableEntry> lookupTable;

    };
}

#undef DLL_API
#endif //PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_H