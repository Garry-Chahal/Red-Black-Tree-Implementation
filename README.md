# Red-Black-Tree-Implementation
Red Black Tree Implementation (Template Class) in C++

The testing framework used to test the Red Black Tree Implementation is Catch2.
More details available here: https://github.com/catchorg/Catch2

Red Black Tree Method Descriptions:

default constructor – creates an empty tree whose root is a null pointer.
copy constructor – a constructor that creates a deep copy of its RedBlackTree reference parameter.
operator= – overloads the assignment operator for RedBlackTree objects.
destructor – deletes dynamic memory allocated by the tree.
insert – if the tree does not contain the method's single template parameter, inserts the parameter and returns true; otherwise does not insert the parameter and returns false.
remove – removes the method's template parameter from the tree and returns true; if the tree does not contain the parameter returns false.
search – searches the tree for the method's single template parameter and returns true if it is found and false otherwise.
search – returns a vector that contains all of the values between the method's first and second template parameters, including both parameter values if they are in the tree.
closestLess - returns the largest value stored in the tree that is less than the method's single template parameter; returns the value of the parameter if there is no such value.
closestGreater - returns the smallest value stored in the tree that is greater than the method's single template parameter; returns the value of the parameter if there is no such value.
values – returns a vector that contains all of the values in the tree; the contents of the vector are in ascending order.
size – returns the number of values stored in the tree

The Red Black Tree Implementation also includes an additional Statistics function, which reads doubles and prints analysis of the provided input upon insertion into the Red Black Tree.
