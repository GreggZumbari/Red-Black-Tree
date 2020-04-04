//GTree.h
/*
This object is a tree of integers, starting with a single parent node. Each node branches off to two (2) other nodes, and could go on indefinetly (or at least while it hasn't run out of allocated memory).
By the way, when I say node, I'm not referring to a struct or anything like that. I'm referring to a single element in the tree, which as you can see, is an int array.
The algorithm for this int array, shown to me by Mr. Galbraith, is such that a parent's two children each reside at (parent's address * 2) and (parent's address * 2 + 1). For example, 4's children are at 8 and 9, and 9's children are at 18 and 19, etc. This works out so that nobody overrides anybody else, and it only uses a single array as storage.

Public functions:

int getCurrent() - Get the token of the current GNode.
int getLeft(int) - Returns the token of the current GNode's left child.
int getRight(int) - Returns the token of the current GNode's left child.
bool headIsEmpty() - Return true if head is undefined.
bool leftIsEmpty() - Returns true if the left child of current is NULL.
bool rightIsEmpty() - Returns true if the right child of current is NULL.
void setHead(int) - Set the token of the head.
void setLeft(int) - Set the token of the left child.
void setRight(int) - Set the token of the right child.
void setLeftAddress(char*) - Set the path of the left child. Requires that the left child already exists.
void setRightAddress(char*) - Set the path of the right child. Requires that the right child already exists.
void moveLeft() - Move current to the left child.
void moveRight() - Move current to the right child.
void resetCurrent() - Resets the position of current back to the head.
void add(int) - Adds the int to the GTree in the correct place.
void printTree() - Print out the whole tree to console in a visual format. It does so by checking the first node's children, and then by checking each of their children, and then by checking each of their children, etc. until you have reached the very bottom of the list, in which case instead of checking the new non-existant children, it will go on to print out the last node's token, and then it will recursively work its way backwards through each recursive function, printing out each parent as it goes, until it gets back to the very top, at which point it will simply print the head.
int numberCount(int) - Returns the amount of times that a specific number was found inside the tree.
int* flushTree() - Completely resets the GTree, returns all of the numbers in an int array that the tree previously had, in branch order from leftmost branch to rightmost branch, from the top of the branch to the bottom of the branch.

Private functions:

void binaryInsertionSort(GNode*) - Sorts a list of GNodes from left to right based on their path.
void checkChildren(GNode*&, int) - Call this function for any existing children of the inputted GNode, then print out the inputted GNode's token.
void searchChildren(GNode*&, int) - Same as checkChildren, except instead of printing out each node, it searches each node and finds out how many of a specified number that there is within the tree.
void flushChildren(GNode*&) - Same as checkChildren, except instead of just printing out the number, it records the number in int* treeGuts, which is a class variable, then deletes it.


@author Greggory Hickman, March 2020
@version 1.0
 */
 
#include <iostream>

#define LEN 100
#define BIGLEN 3000

using namespace std;

class GTree {
	public:
		//Constructor & Destructor
		GTree();
		~GTree();
		
		//Functions
		int getCurrent();
		//int getParent();
		int getLeft();
		int getRight();
		bool headIsEmpty();
		bool leftIsEmpty();
		bool rightIsEmpty();
		void setHead(int);
		void setLeft(int);
		void setRight(int);
		void setLeftAddress(char*);
		void setRightAddress(char*);
		void moveLeft();
		void moveRight();
		void resetCurrent();
		void add(int);
		void printTree();
		int numberCount(int);
		int* flushTree();
		
	private:
		struct GNode {
			bool isRed;
			int token;
			char* path; //This says where the node currently is in the tree right now. A zero means a left turn, a one means a right turn. For example, a path of "101011" means that to the get to the node, you must start at the head, then go right, then left, then right, then left, and then right two more times in order to find where the node is.
			GNode* parent;
			GNode* left;
			GNode* right;
			
			GNode() {
				isRed = true;
				token = -1;
				path = new char[LEN];
				parent = NULL;
				left = NULL;
				right = NULL;
				
				//Clear the path!
				for (int i = 0; i < LEN; i++) {
					path[i] = (char)0;
				}
			}
		};
		
		//Private functions
		void binaryInsertionSort(GNode*);
		//GNode* getParentNode();
		
		//Recursive functions which are also private
		void checkChildren(GNode*&, int);
		void searchChildren(GNode*&, int);
		void flushChildren(GNode*&);
	
		//Class variables
		int* treeGuts;
		GNode* pureTreeGuts;
		int highestGeneration;
		int count;
		GNode* head; //The head of the tree
		GNode* current; //The current GNode that the user is looking at.
		GNode* marker; //A marker to temporarily leave at a GNode
};