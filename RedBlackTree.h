#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

// NodeT class
template <class T>
class NodeT
{
public:
    T data;
    NodeT<T> *left;
    NodeT<T> *right;
    NodeT<T> *parent;
    bool isBlack;

    // NodeT Constructor
    NodeT<T>(T value) : data(value), left(nullptr), right(nullptr), parent(nullptr), isBlack(false){};
};

template <class T>
class RedBlackTree
{
    // Private attributes and helper methods
private:
    NodeT<T> *root;
    int treeSize;
    NodeT<T> *copyTree(const NodeT<T> *treeNode);
    bool isEmpty() const;
    NodeT<T> *findNode(const T valueToSearch) const;
    void deleteTree(NodeT<T> *treeNode);
    vector<T> inOrderValues(const NodeT<T> *currentNode, vector<T> &treeValues) const;
    vector<T> rangeSearch(const NodeT<T> *currentNode, const T valueToSearch1,
                          const T valueToSearch2, vector<T> &treeValues) const;
    void rotateLeft(NodeT<T> *nodeToRotate);
    void rotateRight(NodeT<T> *nodeToRotate);
    NodeT<T> *BSTInsert(NodeT<T> *currentNode, NodeT<T> *nodeToStore);
    void RBInsert(NodeT<T> *nodeToStore);
    void removeFix(NodeT<T> *nodeToRemove, NodeT<T> *nodeParent, bool isLeftChild);
    NodeT<T> *predecessor(NodeT<T> *currentNode);

    // Public methods
public:
    RedBlackTree();
    RedBlackTree(const RedBlackTree<T> &treeParameter);
    RedBlackTree<T> &operator=(const RedBlackTree<T> &treeParameter);
    ~RedBlackTree();
    bool insert(const T valueToStore);
    bool remove(const T valueToRemove);
    bool search(const T valueToSearch) const;
    vector<T> search(const T valueToSearch1, const T valueToSearch2) const;
    T closestLess(const T valueToCompare) const;
    T closestGreater(const T valueToCompare) const;
    vector<T> values() const;
    int size() const;
    template <class Tjwme>
    friend NodeT<Tjwme> *getTreeRoot(const RedBlackTree<Tjwme> &rbt);
};

// Constructor
template <class T>
RedBlackTree<T>::RedBlackTree()
{
    // Create an empty tree
    root = nullptr;
    treeSize = 0;
}

// Copy constructor
template <class T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<T> &treeParameter)
{
    // Deep copies its constant RedBlackTree reference parameter
    root = copyTree(treeParameter.root);
    treeSize = treeParameter.treeSize;
}

// Overloads the assignment operator for RedBlackTree
template <class T>
RedBlackTree<T> &RedBlackTree<T>::operator=(const RedBlackTree<T> &treeParameter)
{
    // If the calling object is the parameter, the operator should not copy it
    if (this != &treeParameter)
    {
        // Dallocates dynamic memory associated with the original tree
        deleteTree(root);
        root = nullptr;

        // Deep copies its constant QueueT reference parameter
        root = copyTree(treeParameter.root);
        treeSize = treeParameter.treeSize;
    }
    // Returns a reference to the calling object
    return *this;
}

// Helper function to create a copy of the parameter
// onto the calling object tree
template <class T>
NodeT<T> *RedBlackTree<T>::copyTree(const NodeT<T> *treeNode)
{
    if (treeNode == nullptr)
    {
        return nullptr;
    }
    else
    {
        // Create a new node with the original properties
        NodeT<T> *newNode = new NodeT<T>(treeNode->data);
        newNode->isBlack = treeNode->isBlack;

        // Copy nodes in the left subtree
        newNode->left = copyTree(treeNode->left);

        // Assign parent pointers
        if (newNode->left != nullptr)
        {
            newNode->left->parent = newNode;
        }

        // Copy nodes in the right subtree
        newNode->right = copyTree(treeNode->right);
        if (newNode->right != nullptr)
        {
            newNode->right->parent = newNode;
        }

        return newNode;
    }
}

// Destructor
template <class T>
RedBlackTree<T>::~RedBlackTree()
{
    deleteTree(root);
    root = nullptr;
    treeSize = 0;
}

// Helper Function for Destroying tree
// Deallocates dynamic memory allocated by the tree
template <class T>
void RedBlackTree<T>::deleteTree(NodeT<T> *treeNode)
{
    if (treeNode != nullptr)
    {
        // Destroy the left and right subtrees
        deleteTree(treeNode->left);
        deleteTree(treeNode->right);
        delete treeNode;
        treeNode = nullptr;
    }
}

// Inserts the value parameter into the Red-Black tree
// Returns true on success or false if the node is already present
template <class T>
bool RedBlackTree<T>::insert(const T valueToStore)
{
    // If the value is already in the tree, return false
    if (search(valueToStore) == false)
    {
        // If the value is not present, create a new node
        NodeT<T> *nodeToStore = new NodeT<T>(valueToStore);

        // Insert the node normally and "fix" it for the Red-Black Tree
        root = BSTInsert(root, nodeToStore);
        RBInsert(nodeToStore);
        treeSize++;
        return true;
    }
    return false;
}

// Fixes the Red-Black Tree after the insertion of a node
template <class T>
void RedBlackTree<T>::RBInsert(NodeT<T> *nodeToStore)
{
    // Continue looping if the node or its parent is red, or until the root isn't reached
    while (nodeToStore != root && nodeToStore->isBlack == false && nodeToStore->parent->isBlack == false)
    {
        NodeT<T> *nodeParent = nodeToStore->parent;
        NodeT<T> *nodeGrandParent = nodeParent->parent;

        // If the node's parent is a left child
        if (nodeParent == nodeGrandParent->left)
        {
            NodeT<T> *nodeUncle = nodeGrandParent->right;
            // The uncle and parent are red, so make them black and move towards the grandparent
            if (nodeUncle != nullptr && nodeUncle->isBlack == false)
            {
                nodeParent->isBlack = true;
                nodeUncle->isBlack = true;
                nodeGrandParent->isBlack = false;

                // Set the current node to the grandparent.
                nodeToStore = nodeGrandParent;
            }
            else
            {
                // The uncle is black
                if (nodeToStore == nodeParent->right)
                {
                    rotateLeft(nodeParent);

                    // Set the current node to the parent
                    nodeToStore = nodeParent;
                    nodeParent = nodeToStore->parent;
                }

                // Arrange the nodes in a line and rotate the grandparent to balance the tree
                nodeParent->isBlack = true;
                nodeGrandParent->isBlack = false;
                rotateRight(nodeGrandParent);
                nodeToStore = nodeParent;
            }
        }
        else // If the node's parent is a right child
        {
            // Symmetric to the above
            NodeT<T> *nodeUncle = nodeGrandParent->left;
            if (nodeUncle != nullptr && nodeUncle->isBlack == false)
            {
                nodeParent->isBlack = true;
                nodeUncle->isBlack = true;
                nodeGrandParent->isBlack = false;
                nodeToStore = nodeGrandParent;
            }
            else
            {
                if (nodeToStore == nodeParent->left)
                {
                    rotateRight(nodeParent);
                    nodeToStore = nodeParent;
                    nodeParent = nodeToStore->parent;
                }
                rotateLeft(nodeGrandParent);
                nodeParent->isBlack = true;
                nodeGrandParent->isBlack = false;
                nodeToStore = nodeParent;
            }
        }
    }

    // Set the root to black.
    root->isBlack = true;
}

// Recursively finds the appropriate place to insert the nodeToStore parameter
template <class T>
NodeT<T> *RedBlackTree<T>::BSTInsert(NodeT<T> *currentNode, NodeT<T> *nodeToStore)
{
    // We have found the place to insert the node
    if (currentNode == nullptr)
    {
        return nodeToStore;
    }

    // If the parameter value is less than the current node, search the left subtree
    if (nodeToStore->data < currentNode->data)
    {
        currentNode->left = BSTInsert(currentNode->left, nodeToStore);
        currentNode->left->parent = currentNode;
    }

    // If the parameter value is greater than the current node, search the right subtree
    else if (nodeToStore->data > currentNode->data)
    {
        currentNode->right = BSTInsert(currentNode->right, nodeToStore);
        currentNode->right->parent = currentNode;
    }

    return currentNode;
}

// Finds the predecessor of the given node parameter
template <class T>
NodeT<T> *RedBlackTree<T>::predecessor(NodeT<T> *currentNode)
{
    NodeT<T> *nodePredecessor = currentNode;

    // Predecessor is the largest (right most) node in the left subtree of the given node
    nodePredecessor = currentNode->left;
    while (nodePredecessor->right != nullptr)
    {
        nodePredecessor = nodePredecessor->right;
    }

    return nodePredecessor;
}

// Removes the value parameter from the Red-Black tree
// Returns true on success or false if the node is not present
template <class T>
bool RedBlackTree<T>::remove(const T valueToRemove)
{
    if (search(valueToRemove))
    {
        // Find the node with the value to remove
        NodeT<T> *nodeToRemove = findNode(valueToRemove);
        if (nodeToRemove != nullptr)
        {
            NodeT<T> *nodeToReplace;
            NodeT<T> *nodeChild;
            bool isLeftChild;

            // The node has one or no children
            if (nodeToRemove->left == nullptr || nodeToRemove->right == nullptr)
            {
                nodeToReplace = nodeToRemove;
            }
            else // The node has 2 children
            {
                nodeToReplace = predecessor(nodeToRemove);
            }

            // Identify if the child of nodeToReplace is a left or right one
            if (nodeToReplace->left != nullptr)
            {
                nodeChild = nodeToReplace->left;
            }
            else
            {
                nodeChild = nodeToReplace->right;
            }

            // If nodeChild is not null, detach it from nodeToReplace
            if (nodeChild != nullptr)
            {
                nodeChild->parent = nodeToReplace->parent;
            }

            // nodeToReplace is a root, so set a new root
            if (nodeToReplace->parent == nullptr)
            {
                root = nodeChild;

                if (root != nullptr)
                {
                    root->parent = nullptr;
                }
            }
            else
            {
                // nodeToReplace is not a root, so attach nodeChild to nodeToReplace's parent
                if (nodeToReplace == nodeToReplace->parent->left)
                {
                    nodeToReplace->parent->left = nodeChild;

                    // nodeChild is a left child
                    isLeftChild = true;
                }
                else
                {
                    nodeToReplace->parent->right = nodeChild;

                    // nodeChild is a right child
                    isLeftChild = false;
                }
            }

            // nodeToReplace is not nodeToRemove (predecessor), so replace its data
            if (nodeToReplace != nodeToRemove)
            {
                nodeToRemove->data = nodeToReplace->data;
            }

            // If we delete a black node, we need to fix the tree's black height
            if (nodeToReplace->isBlack == true)
            {
                removeFix(nodeChild, nodeToReplace->parent, isLeftChild);
            }
            delete nodeToReplace;
            nodeToReplace = nullptr;
            treeSize--;
            return true;
        }
    }
    return false;
}

// Fixes the Red-Black Tree when a black node is removed
template <class T>
void RedBlackTree<T>::removeFix(NodeT<T> *nodeToRemove, NodeT<T> *nodeParent, bool isLeftChild)
{
    NodeT<T> *nodeSibling;
    // A black node has been removed, so loop until black height has been fixed
    while (nodeToRemove != root && (nodeToRemove == nullptr || nodeToRemove->isBlack == true))
    {
        // The node removed is a left child
        if (isLeftChild == true)
        {
            // The node removed is a left child, so its sibling is the parent's right child
            nodeSibling = nodeParent->right;
            if (nodeSibling == nullptr)
            {
                break;
            }

            // The sibling is red
            // Make the node's sibling black or push problem up the tree
            if (nodeSibling != nullptr && nodeSibling->isBlack == false)
            {
                nodeSibling->isBlack = true;
                nodeParent->isBlack = false;
                rotateLeft(nodeParent);

                // Update the node sibling
                nodeSibling = nodeParent->right;
            }

            // If the sibling's children are both black
            // Make the sibling red to make the sibling's subtree the same black height
            if ((nodeSibling->left == nullptr || nodeSibling->left->isBlack == true) && (nodeSibling->right == nullptr || nodeSibling->right->isBlack == true))
            {
                nodeSibling->isBlack = false;
                nodeToRemove = nodeParent;
                nodeParent = nodeToRemove->parent;

                // nodeToRemove's parent is not nullptr
                if (nodeToRemove != root)
                {
                    // Update the isLeftChild boolean since nodeToRemove has changed
                    if (nodeToRemove == nodeParent->left)
                    {
                        isLeftChild = true;
                    }
                    else
                    {
                        isLeftChild = false;
                    }
                }
            }
            else // The sibling has 1 child
            {
                // Make the sibling's right child red
                if (nodeSibling->right == nullptr || nodeSibling->right->isBlack == true)
                {
                    nodeSibling->left->isBlack = true;
                    nodeSibling->isBlack = false;
                    rotateRight(nodeSibling);

                    // Update the node sibling
                    nodeSibling = nodeParent->right;
                }

                // Update the colours of the sibling and parent
                nodeSibling->isBlack = nodeParent->isBlack;
                nodeParent->isBlack = true;
                nodeSibling->right->isBlack = true;
                rotateLeft(nodeParent);
                nodeToRemove = root;
            }
        }
        else // The node removed is a right child
        {
            // The node removed is a right child, so its sibling is the parent's left child
            nodeSibling = nodeParent->left;
            if (nodeSibling == nullptr)
            {
                break;
            }

            // The sibling is red
            // Make the node's sibling black or push problem up the tree
            if (nodeSibling != nullptr && nodeSibling->isBlack == false)
            {
                nodeSibling->isBlack = true;
                nodeParent->isBlack = false;
                rotateRight(nodeParent);

                // Update the node sibling
                nodeSibling = nodeParent->left;
            }

            // If the sibling's children are both black
            // Make the sibling red to make the sibling's subtree the same black height
            if ((nodeSibling->left == nullptr || nodeSibling->left->isBlack == true) && (nodeSibling->right == nullptr || nodeSibling->right->isBlack == true))
            {
                nodeSibling->isBlack = false;
                nodeToRemove = nodeParent;
                nodeParent = nodeToRemove->parent;

                // nodeToRemove's parent is not nullptr
                if (nodeToRemove != root)
                {

                    // Update the isLeftChild boolean since nodeToRemove has changed
                    if (nodeToRemove == nodeParent->left)
                    {
                        isLeftChild = true;
                    }
                    else
                    {
                        isLeftChild = false;
                    }
                }
            }
            else // The sibling has 1 child
            {
                // Make the sibling's right child red
                if (nodeSibling->left == nullptr || nodeSibling->left->isBlack == true)
                {
                    nodeSibling->right->isBlack = true;
                    nodeSibling->isBlack = false;
                    rotateLeft(nodeSibling);

                    // Update the node sibling
                    nodeSibling = nodeParent->left;
                }

                // Update the colours of the sibling and parent
                nodeSibling->isBlack = nodeParent->isBlack;
                nodeParent->isBlack = true;
                nodeSibling->left->isBlack = true;
                rotateRight(nodeParent);
                nodeToRemove = root;
            }
        }
    }

    // A red node has been found, so make it black to fix black height
    if (nodeToRemove != nullptr)
    {
        nodeToRemove->isBlack = true;
    }
}

// Performs a left rotation on the given node parameter
template <class T>
void RedBlackTree<T>::rotateLeft(NodeT<T> *nodeToRotate)
{
    // The right node's left child is attached to the original node
    NodeT<T> *childNode = nodeToRotate->right;
    nodeToRotate->right = childNode->left;

    // Update the parent references
    if (childNode->left != nullptr)
    {
        childNode->left->parent = nodeToRotate;
    }

    // childNode's parent used to be nodeToRotate's parent
    childNode->parent = nodeToRotate->parent;

    // nodeToRotate is a root
    if (nodeToRotate->parent == nullptr)
    {
        root = childNode;
    }
    else if (nodeToRotate == nodeToRotate->parent->left)
    {
        nodeToRotate->parent->left = childNode;
    }
    else
    {
        nodeToRotate->parent->right = childNode;
    }

    // Make nodeToRotate childNode's left child and update the parent pointer
    childNode->left = nodeToRotate;
    nodeToRotate->parent = childNode;
}

// Performs a right rotation on the given node parameter
template <class T>
void RedBlackTree<T>::rotateRight(NodeT<T> *nodeToRotate)
{
    // The left node's right child is attached to the original node
    NodeT<T> *childNode = nodeToRotate->left;
    nodeToRotate->left = childNode->right;

    // Update the parent references
    if (childNode->right != nullptr)
    {
        childNode->right->parent = nodeToRotate;
    }

    // childNode's parent used to be nodeToRotate's parent
    childNode->parent = nodeToRotate->parent;

    // nodeToRotate is a root
    if (nodeToRotate->parent == nullptr)
    {
        root = childNode;
    }
    else if (nodeToRotate == nodeToRotate->parent->right)
    {
        nodeToRotate->parent->right = childNode;
    }
    else
    {
        nodeToRotate->parent->left = childNode;
    }

    // Make nodeToRotate childNode's left child and update the parent pointer
    childNode->right = nodeToRotate;
    nodeToRotate->parent = childNode;
}

// Searches the tree for the provided parameter
// Returns true if found, false otherwise
template <class T>
bool RedBlackTree<T>::search(const T valueToSearch) const
{
    // Begin the search from the root
    NodeT<T> *currentNode = root;
    while (currentNode != nullptr)
    {
        // Node with the value has been found
        if (valueToSearch == currentNode->data)
        {
            return true;
        }
        // If the parameter value is less than the current node, search the left subtree
        else if (valueToSearch < currentNode->data)
        {
            currentNode = currentNode->left;
        }
        // Search the right subtree
        else
        {
            currentNode = currentNode->right;
        }
    }
    return false;
}

// Similar to the search function, but returns the node rather than a boolean
template <class T>
NodeT<T> *RedBlackTree<T>::findNode(const T valueToSearch) const
{
    // Begin the search from the root
    NodeT<T> *currentNode = root;
    while (currentNode != nullptr)
    {
        // Node with the value has been found
        if (valueToSearch == currentNode->data)
        {
            return currentNode;
        }
        // If the parameter value is less than the current node, search the left subtree
        else if (valueToSearch < currentNode->data)
        {
            currentNode = currentNode->left;
        }
        // Search the right subtree
        else
        {
            currentNode = currentNode->right;
        }
    }
    return nullptr;
}

// Returns the largest value in the tree that is less than the parameter
template <class T>
T RedBlackTree<T>::closestLess(const T valueToCompare) const
{
    // If the tree is empty, we can't find a closest value
    if (isEmpty())
    {
        return valueToCompare;
    }

    // Find the smallest value in the entire tree
    NodeT<T> *smallestNodeValue = root;
    while (smallestNodeValue->left != nullptr)
    {
        smallestNodeValue = smallestNodeValue->left;
    }

    if (smallestNodeValue->data >= valueToCompare)
    {
        return valueToCompare;
    }

    // Get a vector of all values between the smallest value and the parameter
    vector<T> closestValues = search(smallestNodeValue->data, valueToCompare);

    // Iterate through the array backwards and return the first value less than the parameter
    T currentSmallestValue = smallestNodeValue->data;
    for (unsigned int i = closestValues.size() - 1; i >= 0; i--)
    {
        if (closestValues[i] < valueToCompare)
        {
            return closestValues[i];
        }
    }

    // There is no smaller closest value
    return currentSmallestValue;
}

// Returns the smallest value in the tree that is greater than the parameter
template <class T>
T RedBlackTree<T>::closestGreater(const T valueToCompare) const
{
    // If the tree is empty, we can't find a closest value
    if (isEmpty())
    {
        return valueToCompare;
    }

    // Find the greatest value in the entire tree
    NodeT<T> *greatestNodeValue = root;
    while (greatestNodeValue->right != nullptr)
    {
        greatestNodeValue = greatestNodeValue->right;
    }

    if (greatestNodeValue->data <= valueToCompare)
    {
        return valueToCompare;
    }

    // Get a vector of all values between the parameter and the greatest value
    vector<T> closestValues = search(valueToCompare, greatestNodeValue->data);

    // Iterate through the array and return the first value greater than the parameter
    T currentGreatestValue = greatestNodeValue->data;
    for (unsigned int i = 0; i < closestValues.size(); i++)
    {
        if (closestValues[i] > valueToCompare)
        {
            return closestValues[i];
        }
    }

    // There is no greater closest value
    return currentGreatestValue;
}

// Returns a vector containing all of the values in the tree
template <class T>
vector<T> RedBlackTree<T>::values() const
{
    vector<T> treeValues;
    inOrderValues(root, treeValues);
    return treeValues;
}

// Returns a vector containing values between the method's first and second parameters
// The vector is in ascending order
template <class T>
vector<T> RedBlackTree<T>::search(const T valueToSearch1, const T valueToSearch2) const
{
    vector<T> treeValuesInRange;

    // Determine which parameter value is lower and greater (bounds)
    T lowerValue = valueToSearch1;
    T higherValue = valueToSearch2;
    if (valueToSearch1 > valueToSearch2)
    {
        lowerValue = valueToSearch2;
        higherValue = valueToSearch1;
    }

    // Search for values between the low and high bounds
    rangeSearch(root, lowerValue, higherValue, treeValuesInRange);
    return treeValuesInRange;
}

// Returns a vector containing values between the method's first and second parameters
// Recursively calls itself and inserts the appropriate values into the treeValues vector
template <class T>
vector<T> RedBlackTree<T>::rangeSearch(const NodeT<T> *currentNode, const T valueToSearch1,
                                       const T valueToSearch2, vector<T> &treeValues) const
{
    if (currentNode == nullptr)
    {
        return treeValues;
    }

    // Search the left subtree
    if (valueToSearch1 < currentNode->data)
    {
        rangeSearch(currentNode->left, valueToSearch1, valueToSearch2, treeValues);
    }

    // If the current node value is between the first and second parameters,
    // insert the given value in the vector
    if (valueToSearch1 <= currentNode->data && valueToSearch2 >= currentNode->data)
    {
        treeValues.push_back(currentNode->data);
    }

    // Search the right subtree
    if (valueToSearch2 > currentNode->data)
    {
        rangeSearch(currentNode->right, valueToSearch1, valueToSearch2, treeValues);
    }

    return treeValues;
}

// Returns a vector containing all of the values in the tree
template <class T>
vector<T> RedBlackTree<T>::inOrderValues(const NodeT<T> *currentNode, vector<T> &treeValues) const
{
    if (currentNode == nullptr)
    {
        return treeValues;
    }

    // Go through the left subtree
    inOrderValues(currentNode->left, treeValues);

    treeValues.push_back(currentNode->data);

    // Go through the right subtree
    inOrderValues(currentNode->right, treeValues);

    return treeValues;
}

// Returns true if the tree is empty, false otherwise
template <class T>
bool RedBlackTree<T>::isEmpty() const
{
    return root == nullptr;
}

// Returns the size of the tree
template <class T>
int RedBlackTree<T>::size() const
{
    return treeSize;
}

void statistics(string filename)
{
    RedBlackTree<double> fileStatistics;

    double currentNumber = 0.0;
    double totalSumOfValues = 0.0; // Total sum of the unique values
    double medianOfValues = 0.0;   // Median of the unique values
    double averageOfValues = 0.0;  // Average of the unique values

    // Open the file parameter
    ifstream currentFile(filename);

    // Check if lines are processed correctly by reading the values into currentNumber
    while (currentFile >> currentNumber)
    {
        // Insert the values into the fileStatistics tree
        fileStatistics.insert(currentNumber);
    }

    // Get a vector of all the tree values
    vector<double> fileValues = fileStatistics.values();

    // The number of unique values are the size of the tree
    int numOfValues = fileStatistics.size();

    if (numOfValues != 0)
    {
        // Iterate through the vector to find the total sum of all values
        for (int i = 0; i < numOfValues; i++)
        {
            totalSumOfValues += fileValues[i];
        }

        // Average is the total sum divided by the number of values
        averageOfValues = totalSumOfValues / numOfValues;

        // If there are an odd number of values
        if (numOfValues % 2 != 0)
        {
            // The median is the middle element
            medianOfValues = fileValues.at(numOfValues / 2);
        }
        // Even number of values
        else
        {
            // The median is the average of the two central values
            medianOfValues = (fileValues.at((numOfValues - 1) / 2) + fileValues.at(numOfValues / 2)) / 2.0;
        }
        // Print the relevant information
        cout << "# of values: " << numOfValues << endl;
        cout << "average: " << averageOfValues << endl;
        cout << "median: " << medianOfValues << endl;

        // Find the greatest closest value less than 42.0
        if (fileStatistics.closestLess(42.0) != 42.0)
        {
            cout << "closest < 42: " << fileStatistics.closestLess(42.0) << endl;
        }
        else
        {
            cout << "closest < 42: None" << endl;
        }
        // Find the smallest closest value greater than 42.0
        if (fileStatistics.closestGreater(42.0) != 42.0)
        {
            cout << "closest > 42: " << fileStatistics.closestGreater(42.0) << endl;
        }
        else
        {
            cout << "closest > 42: None" << endl;
        }
    }
    else
    {
        // Tree size is 0, so the file does not contain any value
        cout << "The file is empty." << endl;
    }

    currentFile.close();
}