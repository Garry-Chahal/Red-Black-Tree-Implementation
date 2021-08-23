#include "catch.hpp"

// This test suite also includes a test case for the leftRotate method for diagnostic purposes
// This is a private method and in order for the test to work the method must be named "leftRotate" and take in a NodeT<T>* param
// To enable this test case uncomment the line below
// ----------------------------------------------
// #define ENABLE_PRIVATE_METHOD_LEFT_ROTATE_TEST

#ifdef ENABLE_PRIVATE_METHOD_LEFT_ROTATE_TEST
#define private public
#endif

#include "RedBlackTree.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

template <class Tjwme>
NodeT<Tjwme> *getTreeRoot(const RedBlackTree<Tjwme> &rbt)
{
    return rbt.root;
}

// Credit to @thebisqq for finding and adapting this function
template <class T>
static int computeBlackHeight(NodeT<T> *currNode)
{
    // For an empty subtree the answer is obvious
    if (currNode == nullptr)
        return 0;
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackHeight(currNode->left);
    int rightHeight = computeBlackHeight(currNode->right);
    int add = currNode->isBlack == true ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1;
    else
        return leftHeight + add;
}

template <class T>
static bool verifyRedNodeChildrenProperty(NodeT<T> *node)
{
    if (node != nullptr)
    {
        if (!node->isBlack)
        {
            if (node->left != nullptr && !node->left->isBlack)
                return false;
            if (node->right != nullptr && !node->right->isBlack)
                return false;
        }
        return verifyRedNodeChildrenProperty(node->left) && verifyRedNodeChildrenProperty(node->right);
    }
    return true;
}

template <class T>
static void validateInOrder(RedBlackTree<T> &rbt)
{
    vector<T> values = rbt.values();
    int prev = -1;
    for (int value : values)
    {
        CHECK(prev < value); // validate is in order
        prev = value;
    }
}

TEST_CASE("simple test (from canvas)", "[RBT]")
{
    // Int Tree Tests
    RedBlackTree<int> rb1;
    CHECK(rb1.insert(42) == true);
    CHECK(rb1.insert(71) == true);
    CHECK(rb1.insert(13) == true);
    RedBlackTree<int> rb2(rb1);

    CHECK(rb1.remove(13) == true);
    CHECK(rb1.search(42) == true);

    CHECK(rb1.search(50, 100) == (vector<int>){71});
    CHECK(rb1.values() == (vector<int>){42, 71});

    CHECK(rb1.closestLess(12) == 12);
    CHECK(rb1.closestGreater(91) == 91);

    CHECK(rb1.size() == 2);
}

TEST_CASE("basic insert and remove test", "[RBT]")
{
    RedBlackTree<char> rbt;
    CHECK(rbt.insert('l') == true);
    CHECK(rbt.insert('m') == true);
    CHECK(rbt.insert('m') == false);
    CHECK(rbt.insert('m') == false);
    CHECK(rbt.insert('a') == true);
    CHECK(rbt.insert('o') == true);
    CHECK(rbt.insert('o') == false);
    CHECK(rbt.insert('l') == false);
    CHECK(rbt.size() == 4);

    CHECK(rbt.search('l') == true);
    CHECK(rbt.search('m') == true);
    CHECK(rbt.search('a') == true);
    CHECK(rbt.search('o') == true);
    CHECK(rbt.search('f') == false);
    CHECK(rbt.search('g') == false);
    CHECK(rbt.search('b') == false);
    CHECK(rbt.search('n') == false);

    CHECK(rbt.remove('m') == true);
    CHECK(rbt.remove('m') == false);
    CHECK(rbt.remove('n') == false);
    CHECK(rbt.remove('a') == true);
    CHECK(rbt.remove('m') == false);
    CHECK(rbt.remove('n') == false);
    CHECK(rbt.remove('q') == false);
    CHECK(rbt.size() == 2);

    CHECK(rbt.search('l') == true);
    CHECK(rbt.search('o') == true);

    CHECK(rbt.insert('l') == false);
    CHECK(rbt.insert('o') == false);
    CHECK(rbt.insert('m') == true);
    CHECK(rbt.insert('a') == true);
    CHECK(rbt.insert('a') == false);
    CHECK(rbt.size() == 4);

    CHECK(rbt.remove('l') == true);
    CHECK(rbt.remove('m') == true);
    CHECK(rbt.remove('a') == true);
    CHECK(rbt.remove('o') == true);
    CHECK(rbt.size() == 0);

    CHECK(rbt.search('l') == false);
    CHECK(rbt.search('m') == false);
    CHECK(rbt.search('a') == false);
    CHECK(rbt.search('o') == false);

    CHECK(rbt.remove('l') == false);
    CHECK(rbt.remove('m') == false);
    CHECK(rbt.remove('a') == false);
    CHECK(rbt.remove('o') == false);
}

TEST_CASE("closestGreater and closestLess test", "[RBT]")
{
    RedBlackTree<short> rbt;
    rbt.insert(-44);
    rbt.insert(-10);
    rbt.insert(0);
    rbt.insert(4);
    rbt.insert(36);
    rbt.insert(78);
    rbt.insert(92);

    CHECK(rbt.closestLess(-123) == -123);
    CHECK(rbt.closestLess(-50) == -50);
    CHECK(rbt.closestLess(-44) == -44);
    CHECK(rbt.closestLess(-20) == -44);
    CHECK(rbt.closestLess(-11) == -44);
    CHECK(rbt.closestLess(-10) == -44);
    CHECK(rbt.closestLess(-9) == -10);
    CHECK(rbt.closestLess(0) == -10);
    CHECK(rbt.closestLess(1) == 0);
    CHECK(rbt.closestLess(3) == 0);
    CHECK(rbt.closestLess(4) == 0);
    CHECK(rbt.closestLess(7) == 4);
    CHECK(rbt.closestLess(44) == 36);
    CHECK(rbt.closestLess(80) == 78);
    CHECK(rbt.closestLess(111) == 92);
    CHECK(rbt.closestLess(127) == 92);

    CHECK(rbt.closestGreater(-123) == -44);
    CHECK(rbt.closestGreater(-50) == -44);
    CHECK(rbt.closestGreater(-44) == -10);
    CHECK(rbt.closestGreater(-20) == -10);
    CHECK(rbt.closestGreater(-11) == -10);
    CHECK(rbt.closestGreater(-10) == 0);
    CHECK(rbt.closestGreater(-9) == 0);
    CHECK(rbt.closestGreater(0) == 4);
    CHECK(rbt.closestGreater(1) == 4);
    CHECK(rbt.closestGreater(3) == 4);
    CHECK(rbt.closestGreater(4) == 36);
    CHECK(rbt.closestGreater(7) == 36);
    CHECK(rbt.closestGreater(44) == 78);
    CHECK(rbt.closestGreater(77) == 78);
    CHECK(rbt.closestGreater(78) == 92);
    CHECK(rbt.closestGreater(80) == 92);
    CHECK(rbt.closestGreater(92) == 92);
    CHECK(rbt.closestGreater(111) == 111);
    CHECK(rbt.closestGreater(127) == 127);

    rbt = RedBlackTree<short>();
    CHECK(rbt.size() == 0);
    CHECK(rbt.closestLess(-123) == -123);
    CHECK(rbt.closestLess(55) == 55);
    CHECK(rbt.closestLess(0) == 0);

    CHECK(rbt.closestGreater(0) == 0);
    CHECK(rbt.closestGreater(-46) == -46);
    CHECK(rbt.closestGreater(90) == 90);
}

TEST_CASE("bounded search test", "[RBT]")
{
    RedBlackTree<int> rbt;
    rbt.insert(-32);
    rbt.insert(-15);
    rbt.insert(-6);
    rbt.insert(-2);
    rbt.insert(0);
    rbt.insert(5);
    rbt.insert(23);
    rbt.insert(59);
    rbt.insert(102);

    CHECK(rbt.search(-10, 10) == (vector<int>){-6, -2, 0, 5});
    CHECK(rbt.search(10, -10) == (vector<int>){-6, -2, 0, 5});
    CHECK(rbt.search(0, 40) == (vector<int>){0, 5, 23});
    CHECK(rbt.search(59, -6) == (vector<int>){-6, -2, 0, 5, 23, 59});
    CHECK(rbt.search(-1000, -10) == (vector<int>){-32, -15});
    CHECK(rbt.search(-6, -500) == (vector<int>){-32, -15, -6});
    CHECK(rbt.search(50, 2000) == (vector<int>){59, 102});
    CHECK(rbt.search(400, 100) == (vector<int>){102});
    CHECK(rbt.search(23, 23) == (vector<int>){23});
    CHECK(rbt.search(-15, -15) == (vector<int>){-15});
    CHECK(rbt.search(102, 102) == (vector<int>){102});
    CHECK(rbt.search(400, 500) == (vector<int>){});
    CHECK(rbt.search(500, 400) == (vector<int>){});
    CHECK(rbt.search(-500, -400) == (vector<int>){});
    CHECK(rbt.search(-400, -500) == (vector<int>){});
    CHECK(rbt.search(-5, -3) == (vector<int>){});
    CHECK(rbt.search(7, 20) == (vector<int>){});
    CHECK(rbt.search(100, 60) == (vector<int>){});
}

TEST_CASE("values test", "[RBT]")
{
    RedBlackTree<int> rbt;
    rbt.insert(7);
    rbt.insert(3);
    rbt.insert(8);
    rbt.insert(4);
    rbt.insert(5);
    rbt.insert(1);
    rbt.insert(6);
    rbt.insert(2);

    CHECK(rbt.values() == (vector<int>){1, 2, 3, 4, 5, 6, 7, 8});
    rbt = RedBlackTree<int>();
    CHECK(rbt.values() == (vector<int>){});
}

TEST_CASE("copy constructor test", "[RBT]")
{
    RedBlackTree<string> rbt1;

    rbt1.insert("Resistor");
    rbt1.insert("Capacitor");
    rbt1.insert("Diode");
    rbt1.insert("Potentiometer");
    CHECK(rbt1.size() == 4);

    RedBlackTree<string> rbt2(rbt1);

    CHECK(rbt2.size() == 4);
    CHECK(rbt2.search("Resistor"));
    CHECK(rbt2.search("Capacitor"));
    CHECK(rbt2.search("Diode"));
    CHECK(rbt2.search("Potentiometer"));

    CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt2)));
    CHECK(computeBlackHeight(getTreeRoot(rbt2)) != -1);

    CHECK(rbt2.remove("Capacitor"));
    CHECK(rbt2.remove("Diode"));
    CHECK(rbt2.remove("Potentiometer"));
    CHECK(rbt2.remove("Resistor"));
    CHECK(rbt2.size() == 0);

    CHECK(rbt1.size() == 4);
    CHECK(rbt1.remove("Diode"));
    CHECK(rbt1.remove("Resistor"));
    CHECK(rbt1.remove("Potentiometer"));
    CHECK(rbt1.remove("Capacitor"));
    CHECK(rbt1.size() == 0);

    RedBlackTree<string> rbt3(rbt1);
    CHECK(rbt3.size() == 0);

    CHECK(rbt3.insert("Solenoid"));
    CHECK(rbt3.insert("Transistor"));
    CHECK(rbt3.size() == 2);
    CHECK(rbt1.size() == 0);
    CHECK(rbt3.remove("Solenoid"));
    CHECK(rbt3.remove("Transistor"));
    CHECK(rbt3.size() == 0);
}

TEST_CASE("assignment operator test", "[RBT]")
{
    RedBlackTree<string> rbt1;
    RedBlackTree<string> rbt2;

    rbt2 = rbt1;

    rbt2.insert("Resistor");
    rbt2.insert("Capacitor");
    CHECK(rbt2.size() == 2);

    rbt1.insert("OpAmp");
    rbt1.insert("Transistor");
    rbt1.insert("Fuse");
    rbt1.insert("Solenoid");
    rbt1.insert("Inductor");
    CHECK(rbt1.size() == 5);

    rbt1 = rbt1;
    CHECK(rbt1.size() == 5);
    rbt2 = rbt1;
    CHECK(rbt2.size() == 5);

    CHECK(rbt2.search("OpAmp"));
    CHECK(rbt2.search("Transistor"));
    CHECK(rbt2.search("Fuse"));
    CHECK(rbt2.search("Solenoid"));
    CHECK(rbt2.search("Inductor"));
    // verify that all copied tree still has valid properties (isBlack was copied as well)
    CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt2)));
    CHECK(computeBlackHeight(getTreeRoot(rbt2)) != -1);

    // vefify tree is still functional (all node pointers including parent were copied)
    CHECK(rbt2.remove("Fuse"));
    CHECK(rbt2.remove("Solenoid"));

    CHECK(rbt2.search("Fuse") == false);
    CHECK(rbt2.search("Solenoid") == false);
    CHECK(rbt1.search("Fuse"));
    CHECK(rbt1.search("Solenoid"));
    CHECK(rbt1.size() == 5);

    rbt1 = rbt2;
    CHECK(rbt1.size() == 3);
    CHECK(rbt1.remove("OpAmp"));
    CHECK(rbt1.remove("Transistor"));
    CHECK(rbt1.remove("Inductor"));
    CHECK(rbt1.size() == 0);

    CHECK(rbt2.size() == 3);
    CHECK(rbt2.remove("OpAmp"));
    CHECK(rbt2.remove("Transistor"));
    CHECK(rbt2.remove("Inductor"));
    CHECK(rbt2.size() == 0);
}

TEST_CASE("volume sequential insert and remove test", "[RBT]")
{
    const int max = 2000;

    RedBlackTree<int> rbt;

    for (int i = 0; i < max; ++i)
    {
        CHECK(rbt.insert(i) == true);
        CHECK(rbt.search(i) == true);
        CHECK(computeBlackHeight(getTreeRoot(rbt)) != -1);
        CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt)));
    }
    for (int i = 0; i < max; ++i)
        CHECK(rbt.search(i) == true);

    for (int i = 0; i < max; ++i)
        CHECK(rbt.closestLess(i) == (i - 1 >= 0 ? i - 1 : 0));
    for (int i = 0; i < max; ++i)
        CHECK(rbt.closestGreater(i) == (i + 1 <= max - 1 ? i + 1 : max - 1));

    CHECK(rbt.size() == max);
    vector<int> v = rbt.values();
    int prev = -1;
    for (int i = 0; i < max; ++i)
    {
        CHECK(prev < i);                               // validate is in order
        CHECK(find(v.begin(), v.end(), i) != v.end()); // check if item is in vector
        prev = i;
    }

    for (int i = 0; i < max; ++i)
    {
        CHECK(rbt.remove(i) == true);
        CHECK(rbt.search(i) == false);
        CHECK(computeBlackHeight(getTreeRoot(rbt)) != -1);
        CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt)));
    }
    for (int i = 0; i < max; ++i)
        CHECK(rbt.search(i) == false);

    CHECK(rbt.size() == 0);
}

TEST_CASE("volume random insert and remove test", "[RBT]")
{
    const int numberOfElements = 1000;
    const int maxElemValue = 2000;
    const int numberOfTrees = 10;
    for (int t = 0; t < numberOfTrees; ++t)
    {
        vector<int> v;
        RedBlackTree<int> rbt;

        for (int i = 0; i < numberOfElements; ++i)
        {
            int value = rand() % maxElemValue;
            bool inserted = rbt.insert(value);
            CHECK(inserted == !(find(v.begin(), v.end(), value) != v.end()));
            CHECK(computeBlackHeight(getTreeRoot(rbt)) != -1);
            CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt)));
            if (inserted)
                v.push_back(value);
        }
        for (int i : v)
            CHECK(rbt.search(i) == true);

        CHECK(rbt.size() == v.size());
        vector<int> values = rbt.values();
        CHECK(values.size() == v.size());
        validateInOrder(rbt);
        for (int i = 1; i < values.size(); ++i)
            CHECK(rbt.closestLess(values[i]) == *(--find(values.begin(), values.end(), values[i])));
        for (int i = 0; i < values.size() - 1; ++i)
            CHECK(rbt.closestGreater(values[i]) == *(++find(values.begin(), values.end(), values[i])));

        for (int i : v)
        {
            CHECK(rbt.remove(i) == true);
            CHECK(computeBlackHeight(getTreeRoot(rbt)) != -1);
            CHECK(verifyRedNodeChildrenProperty(getTreeRoot(rbt)));
            validateInOrder(rbt);
        }
        for (int i : v)
            CHECK(rbt.search(i) == false);

        CHECK(rbt.size() == 0);
        cout << "Ran full validation test for tree totaling " << v.size() << " random elements" << endl;
    }
}

TEST_CASE("zzz statistics test", "[RBT]")
{
    cout << endl
         << "===============================================" << endl
         << endl;

    cout << "EXPECTED VALUES: - MANUAL VERIFICATION REQUIRED" << endl;
    cout << "# of values:  5" << endl;
    cout << "average:      1593.4" << endl;
    cout << "median:       3.14159" << endl;
    cout << "closest < 42: 3.14159" << endl;
    cout << "closest > 42: 7917.5" << endl;
    cout << "-------------" << endl;
    statistics("testStatFile1.txt");

    cout << endl
         << "===============================================" << endl
         << endl;

    cout << "EXPECTED VALUES: - MANUAL VERIFICATION REQUIRED" << endl;
    cout << "# of values:  10" << endl;
    cout << "average:      45" << endl;
    cout << "median:       45" << endl;
    cout << "closest < 42: 40" << endl;
    cout << "closest > 42: 50" << endl;
    cout << "-------------" << endl;
    statistics("testStatFile2.txt");

    cout << endl
         << "===============================================" << endl
         << endl;

    cout << "EXPECTED VALUES: - MANUAL VERIFICATION REQUIRED" << endl;
    cout << "# of values:  2" << endl;
    cout << "average:      1.65" << endl;
    cout << "median:       1.65" << endl;
    cout << "closest < 42: 2.2" << endl;
    cout << "closest > 42: none" << endl;
    cout << "-------------" << endl;
    statistics("testStatFile3.txt");

    cout << endl
         << "===============================================" << endl
         << endl;

    cout << "EXPECTED VALUES: - MANUAL VERIFICATION REQUIRED" << endl;
    cout << "# of values:  36" << endl;
    cout << "average:      73.7222" << endl;
    cout << "median:       74.5" << endl;
    cout << "closest < 42: none" << endl;
    cout << "closest > 42: 50" << endl;
    cout << "-------------" << endl;
    statistics("testStatFile4.txt");

    cout << endl
         << "===============================================" << endl
         << endl;

    cout << "EXPECTED VALUES: - MANUAL VERIFICATION REQUIRED" << endl;
    cout << "# of values:  1" << endl;
    cout << "average:      0" << endl;
    cout << "median:       0" << endl;
    cout << "closest < 42: 0" << endl;
    cout << "closest > 42: none" << endl;
    cout << "-------------" << endl;
    statistics("testStatFile5.txt");
}

#ifdef ENABLE_PRIVATE_METHOD_LEFT_ROTATE_TEST

TEST_CASE("leftRotate test", "[RBT]")
{
    SECTION("minimal 3 node rotate")
    {
        RedBlackTree<int> rbt;
        rbt.insert(5);
        rbt.insert(2);
        rbt.insert(8);
        NodeT<int> *x;
        rbt.tryFind(5, x);
        rbt.leftRotate(x);

        NodeT<int> *root = getTreeRoot(rbt);
        CHECK(root->data == 8);
        CHECK(root->right == nullptr);
        CHECK(root->left->data == 5);
        CHECK(root->left->right == nullptr);
        CHECK(root->left->parent == root);
        CHECK(root->left->left->data == 2);
        CHECK(root->left->left->left == nullptr);
        CHECK(root->left->left->right == nullptr);
        CHECK(root->left->left->parent == root->left);
    }

    SECTION("not attached to root rotate")
    {
        RedBlackTree<int> rbt;
        rbt.rawInsert(10);
        rbt.rawInsert(5);
        rbt.rawInsert(2);
        rbt.rawInsert(8);
        NodeT<int> *x;
        rbt.tryFind(5, x);

        rbt.leftRotate(x);

        NodeT<int> *root = getTreeRoot(rbt);
        CHECK(root->data == 10);
        CHECK(root->right == nullptr);
        CHECK(root->left->data == 8);
        CHECK(root->left->right == nullptr);
        CHECK(root->left->parent == root);
        CHECK(root->left->left->data == 5);
        CHECK(root->left->left->right == nullptr);
        CHECK(root->left->left->parent == root->left);
        CHECK(root->left->left->left->data == 2);
        CHECK(root->left->left->left->left == nullptr);
        CHECK(root->left->left->left->right == nullptr);
        CHECK(root->left->left->left->parent == root->left->left);
    }

    SECTION("rotate with children/subtrees")
    {
        RedBlackTree<int> rbt;
        rbt.rawInsert(10);
        rbt.rawInsert(5);
        rbt.rawInsert(2);
        rbt.rawInsert(8);
        rbt.rawInsert(1);
        rbt.rawInsert(3);
        rbt.rawInsert(7);
        rbt.rawInsert(9);
        NodeT<int> *x;
        rbt.tryFind(5, x);
        rbt.leftRotate(x);

        NodeT<int> *root = getTreeRoot(rbt);
        CHECK(root->data == 10);
        CHECK(root->right == nullptr);
        CHECK(root->left->data == 8);
        CHECK(root->left->right->data == 9);
        CHECK(root->left->right->parent == root->left);
        CHECK(root->left->parent == root);

        CHECK(root->left->left->data == 5);
        CHECK(root->left->left->right->data == 7);
        CHECK(root->left->left->right->parent == root->left->left);
        CHECK(root->left->left->parent == root->left);

        CHECK(root->left->left->left->data == 2);
        CHECK(root->left->left->left->left->data == 1);
        CHECK(root->left->left->left->left->parent == root->left->left->left);
        CHECK(root->left->left->left->right->data == 3);
        CHECK(root->left->left->left->right->parent == root->left->left->left);
        CHECK(root->left->left->left->parent == root->left->left);
    }
}
#endif
