#include <iostream>
#include <string>

#include <irrXML.h>

#include "pht/unordered_tree.h"
#include "pht/farzan_munro.h"
#include "xml_reader.h"
#include "pht/hypersuccinct_tree_factory.h"
#include "pht/hypersuccinct_tree.h"
#include "pht/bitvector_utils.h"

std::shared_ptr<pht::UnorderedTree<char>> createTestTree();

int main() {
    //todo: Needs complete restructuring

    std::shared_ptr<pht::UnorderedTree<char>> tree = createTestTree();

    std::vector<std::shared_ptr<pht::UnorderedTree<char>>> componentSubtrees = pht::FarzanMunro<char>::decompose(tree, 5);


    uint32_t size = ceil(pow(log2(tree->getSize()), 2.0));
    std::cout << size << std::endl;
    std::vector<std::shared_ptr<pht::UnorderedTree<char>>> componentMinitrees = (pht::FarzanMunro<char>::decompose(tree, size));
    componentMinitrees = pht::ListUtils::reverse(componentMinitrees);
    size = ceil((log2(tree->getSize()))/8.0);
    std::cout << size << std::endl;
    std::cout << std::endl << std::endl;
    for(int i = 0; i < componentMinitrees.size(); i++) {
        std::cout << (i==0?"":"\n") << *componentMinitrees.at(i);
    }
    for(std::shared_ptr<pht::UnorderedTree<char>> miniTree : componentMinitrees) {
        std::vector<std::shared_ptr<pht::UnorderedTree<char>>> fmMicroTrees = pht::FarzanMunro<char>::decompose(miniTree,size);
        fmMicroTrees = pht::ListUtils::reverse(fmMicroTrees);
        std::cout << std::endl << std::endl;
        for(int i = 0; i < fmMicroTrees.size(); i++) {
            std::cout << (i==0?"":"\n") << *fmMicroTrees.at(i);
        }
    }
    std::cout << std::endl;
    componentSubtrees = pht::ListUtils::reverse(componentSubtrees);
    componentMinitrees = pht::ListUtils::reverse(componentMinitrees);
    pht::MiniTree miniTree;
    pht::HypersuccinctTree<char> hypersuccinctTree;
    pht::Bitvector_Utils::createMicroInterconnections(tree,componentSubtrees,5,miniTree);
    pht::Bitvector_Utils::createMiniInterconnections(tree,componentMinitrees,10,hypersuccinctTree);
    std::vector<bool> microTrees = pht::Bitvector_Utils::createBitVectorforMicroTrees(componentSubtrees);


    //Couts are necessary like this
    std::cout << "Original tree:\n" << *tree << "\n\n";
    std::cout << "Component trees:\n";
    for(int i = 0; i < componentSubtrees.size(); i++) {
        std::cout << (i==0?"":"\n") << *componentSubtrees.at(i);
    }
    std::cout << "\nBitvector:\n";
    for(bool bit: microTrees) {
        std::cout << bit;
    }
    std::cout << std::endl;
    for(bool bit: miniTree.FIDs) {
        std::cout << bit;
    }
    std::cout << std::endl;
    for(bool bit: miniTree.typeVectors) {
        std::cout << bit;
    }
    std::cout << std::endl;
    for(bool bit: miniTree.dummys) {
        std::cout << bit;
    }
    std::cout << std::endl << std::endl;
    for(bool bit: hypersuccinctTree.FIDs) {
        std::cout << bit;
    }
    std::cout << std::endl;
    for(bool bit: hypersuccinctTree.typeVectors) {
        std::cout << bit;
    }
    std::cout << std::endl;
    std::cout << std::endl;


    //todo: restructure so it can be used for testing
    //std::shared_ptr<pht::UnorderedTree<std::string>> xmlTree = pht::XMLReader::read("D:\\Nutzerdaten\\Dokumente\\Studium_Informatik\\Projektgruppe TheoInf\\ProjektSuccinctTrees\\cmake-build-debug\\example_service\\1998shortstats.xml");

    /*std::cout << "\n\nXML tree:\n" << xmlTree->toString();
    std::vector<std::shared_ptr<pht::UnorderedTree<std::string>>> componentSubtrees2 = pht::FarzanMunro<std::string>::decompose(xmlTree, 5);
    std::cout << "\n\nComponent trees:\n";
    for(int i = 0; i < componentSubtrees2.size(); i++) {
        std::cout << (i==0?"":"\n") << *componentSubtrees2.at(i);
    }*/

    //pht::HypersuccinctTree<std::string> hst = pht::HypersuccinctTreeFactory::create(xmlTree);
    return 0;
}

std::shared_ptr<pht::UnorderedTree<char>> createTestTree() {
    std::shared_ptr<pht::UnorderedTree<char>> tree = std::make_shared<pht::UnorderedTree<char>>();
    std::shared_ptr<pht::Node<char>> a = std::make_shared<pht::Node<char>>('a');
    std::shared_ptr<pht::Node<char>> b = std::make_shared<pht::Node<char>>('b');
    std::shared_ptr<pht::Node<char>> c = std::make_shared<pht::Node<char>>('c');
    std::shared_ptr<pht::Node<char>> d = std::make_shared<pht::Node<char>>('d');
    std::shared_ptr<pht::Node<char>> e = std::make_shared<pht::Node<char>>('e');
    std::shared_ptr<pht::Node<char>> f = std::make_shared<pht::Node<char>>('f');
    std::shared_ptr<pht::Node<char>> g = std::make_shared<pht::Node<char>>('g');
    std::shared_ptr<pht::Node<char>> h = std::make_shared<pht::Node<char>>('h');
    std::shared_ptr<pht::Node<char>> i = std::make_shared<pht::Node<char>>('i');
    std::shared_ptr<pht::Node<char>> j = std::make_shared<pht::Node<char>>('j');
    std::shared_ptr<pht::Node<char>> k = std::make_shared<pht::Node<char>>('k');
    std::shared_ptr<pht::Node<char>> l = std::make_shared<pht::Node<char>>('l');
    std::shared_ptr<pht::Node<char>> m = std::make_shared<pht::Node<char>>('m');
    std::shared_ptr<pht::Node<char>> n = std::make_shared<pht::Node<char>>('n');
    std::shared_ptr<pht::Node<char>> o = std::make_shared<pht::Node<char>>('o');
    std::shared_ptr<pht::Node<char>> p = std::make_shared<pht::Node<char>>('p');
    std::shared_ptr<pht::Node<char>> q = std::make_shared<pht::Node<char>>('q');
    std::shared_ptr<pht::Node<char>> r = std::make_shared<pht::Node<char>>('r');
    std::shared_ptr<pht::Node<char>> s = std::make_shared<pht::Node<char>>('s');
    std::shared_ptr<pht::Node<char>> t = std::make_shared<pht::Node<char>>('t');
    std::shared_ptr<pht::Node<char>> u = std::make_shared<pht::Node<char>>('u');
    std::shared_ptr<pht::Node<char>> v = std::make_shared<pht::Node<char>>('v');
    std::shared_ptr<pht::Node<char>> w = std::make_shared<pht::Node<char>>('w');
    std::shared_ptr<pht::Node<char>> x = std::make_shared<pht::Node<char>>('x');
    std::shared_ptr<pht::Node<char>> y = std::make_shared<pht::Node<char>>('y');
    std::shared_ptr<pht::Node<char>> z = std::make_shared<pht::Node<char>>('z');
    std::shared_ptr<pht::Node<char>> A = std::make_shared<pht::Node<char>>('A');
    std::shared_ptr<pht::Node<char>> B = std::make_shared<pht::Node<char>>('B');

    tree->add(a);
    tree->add(b, a);
    tree->add(c, a);
    tree->add(d, a);
    tree->add(e, a);
    tree->add(f, a);
    tree->add(g, b);
    tree->add(h, b);
    tree->add(i, b);
    tree->add(j, b);
    tree->add(k, d);
    tree->add(l, g);
    tree->add(m, i);
    tree->add(n, k);
    tree->add(o, k);
    tree->add(p, k);
    tree->add(q, o);
    tree->add(r, o);
    tree->add(s, q);
    tree->add(t, q);
    tree->add(u, q);
    tree->add(v, q);
    tree->add(w, q);
    tree->add(x, r);
    tree->add(y, r);
    tree->add(z, r);
    tree->add(A, t);
    tree->add(B, x);

    return tree;
}