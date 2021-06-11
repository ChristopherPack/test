//
// Created by Christopher Pack on 15.01.2021.
//

#ifndef PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_FACTORY_H
#define PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_FACTORY_H

#include <iostream>
#include <iterator>
#include <map>
#include <set>

#include "huffman.h"
#include "hypersuccinct_tree.h"
#include "hst_output.h"

namespace pht {
    /**
     * Creates Hypersuccinct Code for a given Unordered Tree utilizing the Farzan Munro Algorithm.
     */
    class HypersuccinctTreeFactory {
    public:
        /**
         * Creates Hypersuccinct Code for a given Unordered Tree utilizing the Farzan Munro Algorithm.
         * TODO: I don't think this class can be restructured majorly, but one could add comments for structure
         * @tparam T Class implemented in UnorderedTree.
         * @param tree the UnorderedTree to be encoded.
         * @return HypersuccinctTree class representing the Hypersuccinct code todo: see Class
         */
        template<class T> static HypersuccinctTree create(const std::shared_ptr<UnorderedTree<T>> tree, bool huffman = false) {
            HypersuccinctTree hypersuccinctTree;
            hypersuccinctTree.huffmanFlag = false;

            #ifdef PHT_TEST
            uint32_t sizeMini = 12;
            uint32_t sizeMicro = 4;
            #else
            uint32_t sizeMini = ceil(pow(log2(tree->getSize()), 2.0));
            uint32_t sizeMicro = ceil((log2(tree->getSize())) / 8.0);
            #endif

            pht::Bitvector_Utils::encodeNumber(std::inserter(hypersuccinctTree.size,hypersuccinctTree.size.begin()),tree->getSize(),Bitvector_Utils::NumberEncoding::BINARY);
            pht::Bitvector_Utils::encodeNumber(std::inserter(hypersuccinctTree.miniSize, hypersuccinctTree.miniSize.begin()), sizeMini,Bitvector_Utils::NumberEncoding::BINARY);
            std::vector<std::shared_ptr<pht::UnorderedTree<T>>> fmMiniTrees = pht::FarzanMunro<T>::decompose(tree, sizeMini);

            /*for(std::shared_ptr<pht::UnorderedTree<std::string>>& fmMiniTree : fmMiniTrees) {

                std::cout << "Size of MiniTree: " << fmMiniTree->getSize() << "\n";
                std::cout << "Root of MiniTree: " << fmMiniTree->getRoot()->getValue() << "\n";
                std::cout << "Nodes of MiniTree: " << *fmMiniTree << "\n";
            }*/

            pht::Bitvector_Utils::encodeNumber(std::inserter(hypersuccinctTree.microSize, hypersuccinctTree.microSize.begin()), sizeMicro,Bitvector_Utils::NumberEncoding::BINARY);

            std::tuple<Bitvector,Bitvector> miniIntercon = create1_2_Interconnections(tree,fmMiniTrees,sizeMini);
            hypersuccinctTree.miniFIDs = std::get<0>(miniIntercon);
            hypersuccinctTree.miniTypeVectors = std::get<1>(miniIntercon);

            Bitvector miniDummys = createDummyInterconnections(tree,fmMiniTrees,sizeMini);
            hypersuccinctTree.miniDummys = miniDummys;

            std::map<std::vector<bool>,uint32_t> bpsAndOccurrences;
            std::vector<std::shared_ptr<pht::Node<T>>> orderedMinis;

            std::cout << "Output of Factory:  " << fmMiniTrees.size() << "\n";
            std::cout << "Amount of MiniTrees: " << fmMiniTrees.size() << "\n";

            uint32_t miniTreeNum = 0;

            for(std::shared_ptr<pht::UnorderedTree<T>> fmMiniTree : fmMiniTrees) {
                fmMiniTree->getRoot()->setMiniTree(miniTreeNum);
                miniTreeNum++;
            }

            for(std::shared_ptr<pht::UnorderedTree<T>> fmMiniTree : fmMiniTrees) {
                std::vector<std::shared_ptr<pht::UnorderedTree<T>>> fmMicroTrees = pht::FarzanMunro<T>::decompose(fmMiniTree, sizeMicro);
                MiniTree miniTree = MiniTree();

                orderedMinis.push_back(fmMiniTree->getRoot());

                if(fmMiniTree->hasDummy()) {
                    for(const std::shared_ptr<pht::UnorderedTree<T>>& fmMicroTree : fmMicroTrees) {
                        miniTree.rootAncestors.push_back(fmMiniTree->isAncestor(fmMiniTree->getDummy(), fmMicroTree->getRoot()));
                    }
                }

                std::tuple<Bitvector,Bitvector> microIntercon = create1_2_Interconnections(fmMiniTree,fmMicroTrees,sizeMicro);
                miniTree.FIDs = std::get<0>(microIntercon);
                miniTree.typeVectors = std::get<1>(microIntercon);

                Bitvector dummys = createDummyInterconnections(fmMiniTree, fmMicroTrees, sizeMicro);
                miniTree.dummys = dummys;

                Bitvector_Utils::encodeNumber(miniTree.miniDepth, tree->getDepth(fmMiniTree->getRoot()), Bitvector_Utils::NumberEncoding::BINARY);
                Bitvector_Utils::encodeNumber(miniTree.miniHeight, tree->getHeight(fmMiniTree->getRoot()), Bitvector_Utils::NumberEncoding::BINARY);
                Bitvector_Utils::encodeNumber(miniTree.miniLeaves, tree->getLeafSize(fmMiniTree->getRoot()),Bitvector_Utils::NumberEncoding::BINARY);

                Bitvector miniDummyTree;
                Bitvector miniDummyIndex;

                uint32_t microTreeNum = 0;
                for(const std::shared_ptr<pht::UnorderedTree<T>>& fmMicroTree : fmMicroTrees) {
                    if(fmMicroTree->contains(fmMiniTree->getDummy())) {
                        auto iter = std::find(fmMicroTrees.begin(),fmMicroTrees.end(), fmMicroTree);
                        uint32_t dist = std::distance(fmMicroTrees.begin(), iter);
                        Bitvector_Utils::encodeNumber(miniDummyTree,dist,Bitvector_Utils::NumberEncoding::BINARY);
                        miniTree.miniDummyTree = miniDummyTree;
                        std::vector<std::shared_ptr<Node<T>>> nodes = fmMicroTree->getNodes();
                        auto iter1 = std::find(nodes.begin(),nodes.end(), fmMiniTree->getDummy());
                        dist = std::distance(nodes.begin(), iter1);
                        Bitvector_Utils::encodeNumber(miniDummyIndex,dist,Bitvector_Utils::NumberEncoding::BINARY);
                        miniTree.miniDummyIndex = miniDummyIndex;
                    }
                    fmMicroTree->getRoot()->setMicroTree(microTreeNum);
                    microTreeNum++;
                }

                if(fmMiniTree->hasDummy()) {
                    for(const std::shared_ptr<pht::UnorderedTree<T>>& fmMicroTree : fmMicroTrees) {
                        if(fmMicroTree->hasDummy()) {
                            miniTree.dummyAncestors.push_back(fmMiniTree->isAncestor(fmMiniTree->getDummy(), fmMicroTree->getDummy()));
                        } else {
                            miniTree.dummyAncestors.push_back(false);
                        }
                    }

                    std::shared_ptr<pht::Node<T>> dummyPoint = tree->getDirectDescendants(fmMiniTree->getDummy()).at(0);
                    uint32_t miniTreePointer = dummyPoint->getMiniTree();
                    Bitvector_Utils::encodeNumber(miniTree.miniDummyPointer, miniTreePointer, Bitvector_Utils::NumberEncoding::BINARY);

                    Bitvector_Utils::encodeNumber(miniTree.miniDummyDepth, tree->getDepth(dummyPoint), Bitvector_Utils::NumberEncoding::BINARY);
                    Bitvector_Utils::encodeNumber(miniTree.miniDummyHeight, tree->getHeight(dummyPoint), Bitvector_Utils::NumberEncoding::BINARY);

                }

                std::vector<std::shared_ptr<pht::Node<T>>> orderedMicros;

                for(std::shared_ptr<pht::UnorderedTree<T>> fmMicroTree : fmMicroTrees) {

                    orderedMicros.push_back(fmMicroTree->getRoot());

                    Bitvector bp = fmMicroTree->toBalancedParenthesis();
                    if(huffman) {
                        hypersuccinctTree.huffmanFlag = true;
                        if(bpsAndOccurrences.find(bp) == bpsAndOccurrences.end()) {
                            bpsAndOccurrences.insert({bp, 0});
                        }
                        bpsAndOccurrences.at(bp)++;
                    }

                    Bitvector_Utils::encodeNumber(miniTree.rootDepths, fmMiniTree->getDepth(fmMicroTree->getRoot())+1, Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                    Bitvector_Utils::encodeNumber(miniTree.rootHeights, fmMiniTree->getHeight(fmMicroTree->getRoot())+1, Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                    Bitvector_Utils::encodeNumber(miniTree.microLeaves, fmMiniTree->getLeafSize(fmMicroTree->getRoot()), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

                    Bitvector matrix;
                    Bitvector degree;
                    Bitvector subTree;
                    Bitvector nodeDepth;
                    Bitvector nodeHeight;
                    Bitvector leave;
                    //std::vector<std::shared_ptr<pht::Node<T>>> orderedNodes = fmMicroTree->getNodes();
                    //ListUtils::sort<std::shared_ptr<pht::Node<T>>>(orderedNodes, [&fmMicroTree](std::shared_ptr<pht::Node<T>> a, std::shared_ptr<pht::Node<T>> b){ return fmMicroTree->enumerate(a) < fmMicroTree->enumerate(b); });
                    for(std::shared_ptr<pht::Node<T>> node1 : fmMicroTree->getNodes()) {
                        for(std::shared_ptr<pht::Node<T>> node2 : fmMicroTree->getNodes()) {
                            matrix.push_back(fmMicroTree->isAncestor(node2, node1));
                        }

                        uint32_t degreeNum = fmMicroTree->getDegree(node1)+1;
                        Bitvector_Utils::encodeNumber(degree, degreeNum,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

                        uint32_t subTreeNum = fmMicroTree->getSize(node1,true)+1;
                        Bitvector_Utils::encodeNumber(subTree,subTreeNum,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

                        uint32_t nodeDepthNum = fmMicroTree->getDepth(node1, true) +1;
                        Bitvector_Utils::encodeNumber(nodeDepth,nodeDepthNum,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

                        uint32_t nodeHeightNum = fmMicroTree->getHeight(node1, true) +1;
                        Bitvector_Utils::encodeNumber(nodeHeight,nodeDepthNum,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

                        uint32_t leaveNum = fmMicroTree->getLeafSize(node1);
                        Bitvector_Utils::encodeNumber(leave,leaveNum,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                    }

                    if(fmMicroTree->hasDummy()) {
                        std::shared_ptr<pht::Node<T>> dummyPoint = fmMiniTree->getDirectDescendants(fmMicroTree->getDummy()).at(0);
                        uint32_t microTreePointer = dummyPoint->getMicroTree();
                        Bitvector_Utils::encodeNumber(miniTree.microDummyPointers, microTreePointer, Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                    }

                    LookupTableEntry microTreeData(bp, matrix);
                    microTreeData.degree = degree;
                    microTreeData.subTrees = subTree;
                    microTreeData.nodeDepths = nodeDepth;
                    microTreeData.nodeHeights = nodeHeight;
                    microTreeData.leaves = leave;
                    if(!ListUtils::containsAny(hypersuccinctTree.lookupTable, {microTreeData})) {
                        hypersuccinctTree.lookupTable.push_back(microTreeData);
                    }

                    Bitvector_Utils::encodeNumber(miniTree.microSubTrees, fmMiniTree->getSize(fmMicroTree->getRoot(),false),Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                }

                Bitvector_Utils::encodeNumber(miniTree.subTree, tree->getSize(fmMiniTree->getRoot(),false),Bitvector_Utils::NumberEncoding::BINARY);

                ListUtils::sort<std::shared_ptr<pht::Node<T>>>(orderedMicros, [&fmMiniTree](std::shared_ptr<pht::Node<T>> a, std::shared_ptr<pht::Node<T>> b){ return fmMiniTree->enumerate(a) < fmMiniTree->enumerate(b); });
                for(std::shared_ptr<pht::Node<T>> node1 : orderedMicros) {
                    for(std::shared_ptr<pht::Node<T>> node2 : orderedMicros) {
                        miniTree.ancMatrix.push_back(fmMiniTree->isAncestor(node2, node1));
                    }
                }

                miniTree.microTrees = createBitVectorforMicroTrees(fmMicroTrees);
                hypersuccinctTree.miniTrees.push_back(miniTree);

                std::cout << "Size of MiniTree: " << fmMiniTree->getSize() << "\n";
                std::cout << "Root of MiniTree: " << fmMiniTree->getRoot()->getValue() << "\n";
                std::cout << "Nodes of MiniTree: " << fmMiniTree->toNewickString() << "\n";

                std::cout << "Amount of MicroTrees: " << fmMicroTrees.size() << "\n";

                for(std::shared_ptr<pht::UnorderedTree<std::string>>& fmMicroTree : fmMicroTrees) {
                    std::cout << "Size of MicroTree: " << fmMicroTree->getSize() << "\n";
                    std::cout << "Root of MicroTree: " << fmMicroTree->getRoot()->getValue() << "\n";
                    std::cout << "Nodes of MicroTree: " << fmMicroTree->toNewickString() << "\n";
                }
                std::cout << std::endl;
            }

            ListUtils::sort<std::shared_ptr<pht::Node<T>>>(orderedMinis, [&tree](std::shared_ptr<pht::Node<T>> a, std::shared_ptr<pht::Node<T>> b){ return tree->enumerate(a) < tree->enumerate(b); });
            for(std::shared_ptr<pht::Node<T>> node1 : orderedMinis) {
                for(std::shared_ptr<pht::Node<T>> node2 : orderedMinis) {
                    hypersuccinctTree.miniAncMatrix.push_back(tree->isAncestor(node2, node1));
                }
            }

            if(huffman) {
                std::map<std::vector<bool>,std::vector<bool>> huffmanTable = Huffman::generateTable<std::vector<bool>>(bpsAndOccurrences);
                convertToHuffman(hypersuccinctTree, huffmanTable);
            }

            return hypersuccinctTree;
        }

        /**
         * Creates a HypersuccinctTree from a given full Bitvector
         * WARNING: Function is very sensitive to badly formatted bitvectors (see write to File for formatting)
         * TODO: Need error handling for bad bitvectors
         * @param fullBitvector the bitvector
         * @return Hypersuccinct Tree
         */
        static HypersuccinctTree createFromFile(Bitvector fullBitvector) {
            HypersuccinctTree hst;
            auto iter = fullBitvector.begin();
            hst.huffmanFlag = *iter;
            iter++;
            uint32_t treeSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            uint32_t miniSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            uint32_t microSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            Bitvector_Utils::encodeNumber(hst.size, treeSize, Bitvector_Utils::NumberEncoding::BINARY);
            Bitvector_Utils::encodeNumber(hst.miniSize, miniSize, Bitvector_Utils::NumberEncoding::BINARY);
            Bitvector_Utils::encodeNumber(hst.microSize, microSize, Bitvector_Utils::NumberEncoding::BINARY);
            uint32_t miniTreesSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            uint32_t lookupTableSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);

            uint32_t tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            for(uint32_t i=0; i<tempSize; i++) {
                hst.miniFIDs.push_back(*iter);
                iter++;
            }

            tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            for(uint32_t i=0; i<tempSize; i++) {
                hst.miniTypeVectors.push_back(*iter);
                iter++;
            }

            tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
            for(uint32_t i=0; i<tempSize; i++) {
                hst.miniDummys.push_back(*iter);
                iter++;
            }

            for(uint32_t j=0; j<miniTreesSize; j++) {
                MiniTree mini;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.FIDs.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.typeVectors.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.dummys.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.microTrees.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.rootAncestors.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.dummyAncestors.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.miniDummyTree.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.miniDummyIndex.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.miniDummyPointer.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.miniAnc.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.subTree.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.microSubTrees.push_back(*iter);
                    iter++;
                }
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    mini.microDummyPointers.push_back(*iter);
                    iter++;
                }
                /*uint32_t miniDummyTreeNum = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                uint32_t miniDummyIndexNum = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                Bitvector_Utils::encodeNumber(mini.miniDummyTree, miniDummyTreeNum, Bitvector_Utils::NumberEncoding::BINARY);
                Bitvector_Utils::encodeNumber(mini.miniDummyIndex, miniDummyIndexNum, Bitvector_Utils::NumberEncoding::BINARY);*/
                hst.miniTrees.push_back(mini);
            }

            for(uint32_t j=0; j<lookupTableSize; j++) {
                Bitvector index;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    index.push_back(*iter);
                    iter++;
                }
                Bitvector bp;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    bp.push_back(*iter);
                    iter++;
                }
                Bitvector matrix;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    matrix.push_back(*iter);
                    iter++;
                }
                Bitvector degree;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    degree.push_back(*iter);
                    iter++;
                }
                Bitvector subTrees;
                tempSize = Bitvector_Utils::decodeNumber(iter, fullBitvector.cend(), Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(uint32_t i=0; i<tempSize; i++) {
                    subTrees.push_back(*iter);
                    iter++;
                }
                //TODO: Last Bit is SOMETIMES 0, should be 1; seems to be an encoding issue
                /**
                 * On TreeAlex:
                 * 1111111010110000100110001000000010000000100000001 correct bitvector
                 * 1111111010110000100110001000000010000000100000000 createFromFile
                 * 1111111010110000100110001000000010000000100000001 fileoutput
                 * 1111111010110000100110001000000010000000100000000000000 fileinput???
                 *
                 * HOWEVER:
                 * Behaves correctly on TreeNath!
                 */
                LookupTableEntry microTreeData(index, bp, matrix);
                microTreeData.degree = degree;
                microTreeData.subTrees = subTrees;
                hst.lookupTable.push_back(microTreeData);
            }
            return hst;
        }

        /**
         * This Class creates a Bitvector from a List of given UnorderedTrees
         * It is supposed to use a List of MicroTrees from the Farzan Munro Algorithm
         * Encoding consists of Elias Gamma Code of the size of the MicroTrees and their structure in Balanced Parenthesis form.
         *
         * @tparam T Class implemented in UnorderedTree
         * @param fmMicroTrees List of UnorderedTrees (MicroTrees)
         */
        template<class T> static Bitvector createBitVectorforMicroTrees(std::vector<std::shared_ptr<pht::UnorderedTree<T>>> fmMicroTrees) {
            //determine size of Bitvector
            Bitvector res;
            for(std::shared_ptr<pht::UnorderedTree<T>> fmMicroTree : fmMicroTrees) {
                int32_t size = fmMicroTree->getSize();
                //elias gamma code SIZE
                pht::Bitvector_Utils::encodeNumber(std::inserter(res, res.end()), size,Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                //BP FORM in bitform
                Bitvector bp = fmMicroTree->toBalancedParenthesis();
                pht::ListUtils::combine(res,bp);
            }
            return res;
        }

        static void convertToHuffman(HypersuccinctTree& tree, std::map<std::vector<bool>,std::vector<bool>> huffmanTable) {
            for(LookupTableEntry& x : tree.lookupTable) {
                x.bp = x.index;
                x.index = huffmanTable.at(x.index);
            }
            for(MiniTree& x : tree.miniTrees) {
                std::vector<bool> oldEncodedMicros = x.microTrees;
                x.microTrees.clear();
                uint32_t entryCount = Bitvector_Utils::getEntryCount(oldEncodedMicros.cbegin(), oldEncodedMicros.cend(), Bitvector_Utils::BitvectorEncoding::ELIAS_GAMMA, {Bitvector_Utils::nullIterator, Bitvector_Utils::nullIterator, 2, 0});
                for(uint32_t i = 0; i < entryCount; i++) {
                    auto iter = oldEncodedMicros.cbegin();
                    std::vector<bool> bp = Bitvector_Utils::getEntry(iter, i, oldEncodedMicros.cend(), Bitvector_Utils::BitvectorEncoding::ELIAS_GAMMA, {Bitvector_Utils::nullIterator, Bitvector_Utils::nullIterator, 2, 0});
                    ListUtils::combine(x.microTrees, huffmanTable.at(bp));
                }
                HypersuccinctTreeOutput::printBitvector(x.microTrees);
            }
        }

        /**
         *
         * @tparam T
         * @param baseTree
         * @param subtrees
         * @param size
         * @return
         */
        template<class T> static std::tuple<Bitvector,Bitvector> create1_2_Interconnections(std::shared_ptr<pht::UnorderedTree<T>> baseTree, std::vector<std::shared_ptr<pht::UnorderedTree<T>>> subtrees, uint32_t size) {
            Bitvector FIDs;
            Bitvector typeVectors;
            uint32_t dummySize = floor(log2(2*size+1))+1;
            std::vector<std::shared_ptr<pht::Node<T>>> rootNodes = ListUtils::mapped<std::shared_ptr<UnorderedTree<T>>, std::shared_ptr<pht::Node<T>>>(subtrees, [](std::shared_ptr<UnorderedTree<T>> x){return x -> getRoot();});
            std::vector<std::shared_ptr<pht::Node<T>>> distinctRootNodes = ListUtils::distincted(rootNodes);
            std::vector<std::shared_ptr<pht::Node<T>>> firstChildren;
            std::vector<std::shared_ptr<pht::UnorderedTree<T>>> filteredTrees = ListUtils::filtered<std::shared_ptr<UnorderedTree<T>>>(subtrees, [](std::shared_ptr<UnorderedTree<T>> x){return !(x -> isLeaf(x->getRoot()));});
            firstChildren = ListUtils::mapped<std::shared_ptr<UnorderedTree<T>>, std::shared_ptr<pht::Node<T>>>(filteredTrees, [](std::shared_ptr<UnorderedTree<T>> x){return x -> getDirectDescendants(x->getRoot()).at(0);});
            //zählung von firstChildren ist front - zählung in enumerate

            //FIDs und TypeVectors
            for(std::shared_ptr<pht::Node<T>> rootNode : distinctRootNodes) {
                std::vector<std::shared_ptr<pht::Node<T>>> children = baseTree->getDirectDescendants(rootNode);
                /*std::vector<std::pair<uint32_t,std::shared_ptr<pht::Node<T>>>> enumResult;
                enumResult = ListUtils::mapped<std::shared_ptr<pht::Node<T>>, std::pair<uint32_t,std::shared_ptr<pht::Node<T>>>>(children, [&baseTree](std::shared_ptr<pht::Node<T>> node){return std::pair<uint32_t,std::shared_ptr<pht::Node<T>>>(baseTree->enumerate(node),node);});
                ListUtils::sort<std::pair<uint32_t,std::shared_ptr<pht::Node<T>>>>(enumResult, [](std::pair<uint32_t,std::shared_ptr<pht::Node<T>>> nodeA, std::pair<uint32_t,std::shared_ptr<pht::Node<T>>> nodeB){return nodeA.first<nodeB.first;});
                children.clear();
                children = ListUtils::mapped<std::pair<uint32_t,std::shared_ptr<pht::Node<T>>>, std::shared_ptr<pht::Node<T>>>(enumResult, [](std::pair<uint32_t,std::shared_ptr<pht::Node<T>>> nodeB){return nodeB.second;});*/
                pht::Bitvector_Utils::encodeNumber(std::inserter(FIDs, FIDs.end()), children.size(),Bitvector_Utils::NumberEncoding::ELIAS_GAMMA);
                for(std::shared_ptr<pht::Node<T>> node : children) {
                    if(ListUtils::containsAny(rootNodes,{node})) {
                        FIDs.push_back(true);
                        typeVectors.push_back(false);
                    }
                    else if(ListUtils::containsAny(firstChildren,{node}))
                    {
                        FIDs.push_back(true);
                        typeVectors.push_back(true);
                    }
                    else
                    {
                        FIDs.push_back(false);
                    }
                }

            }
            return {FIDs, typeVectors};
        }

        /**
         *
         * @tparam T
         * @param baseTree
         * @param subtrees
         * @param size
         * @return
         */
        template<class T> static Bitvector createDummyInterconnections(std::shared_ptr<pht::UnorderedTree<T>> baseTree, std::vector<std::shared_ptr<pht::UnorderedTree<T>>> subtrees, uint32_t size) {
            Bitvector dummys;
            uint32_t dummySize = floor(log2(2*size+1))+1;
            //Dummy Nodes
            for(std::shared_ptr<pht::UnorderedTree<T>> fmMicroTree : subtrees) {
                bool hadDummy = false;
                for(std::shared_ptr<pht::Node<T>> node : fmMicroTree->getNodes()) {
                    if(node != fmMicroTree->getRoot()) {
                        std::vector<std::shared_ptr<pht::Node<T>>> children = baseTree->getDirectDescendants(node);
                        for(int ind = 0; ind < children.size(); ind++) {
                            if(!ListUtils::containsAny(fmMicroTree->getNodes(),{children.at(ind)})) {
                                std::shared_ptr<pht::Node<T>> dummyNode = std::make_shared<pht::Node<T>>(T());
                                dummyNode->setMiniDummy();
                                //Index für Tree order
                                fmMicroTree->insert(dummyNode, ind, node);
                                fmMicroTree->setDummy(dummyNode);
                                baseTree->insertBetween(dummyNode, children.at(ind), node);
                                Bitvector bp = fmMicroTree->toBalancedParenthesis();
                                Bitvector num;
                                pht::Bitvector_Utils::encodeNumber(std::inserter(num, num.end()), fmMicroTree->enumerate(dummyNode),Bitvector_Utils::NumberEncoding::BINARY);
                                for (int i = 0; i < dummySize-num.size(); i++) {
                                    dummys.push_back(false);
                                }
                                ListUtils::combine(dummys, num);
                                hadDummy = true;
                                break;
                            }
                        }
                        if(hadDummy) {
                            break;
                        }
                    }
                }
                if(!hadDummy) {
                    for (int i = 0; i < dummySize; i++) {
                        dummys.push_back(false);
                    }
                }
            }
            return dummys;
        }
    };
}

#endif //PROJEKTSUCCINCTTREES_HYPERSUCCINCT_TREE_FACTORY_H