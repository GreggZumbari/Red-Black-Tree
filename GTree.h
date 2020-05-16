//GTree.h
/*
This object is a tree of integers, starting with a single parent node. Each node branches off to two (2) other nodes, and could go on indefinetly (or at least while it hasn't run out of allocated memory).
By the way, when I say node, I'm not referring to a struct or anything like that. I'm referring to a single element in the tree, which as you can see, is an int array.
The algorithm for this int array, shown to me by Mr. Galbraith, is such that a parent's two children each reside at (parent's address * 2) and (parent's address * 2 + 1). For example, 4's children are at 8 and 9, and 9's children are at 18 and 19, etc. This works out so that nobody overrides anybody else, and it only uses a single array as storage.

Public functions:

int getCurrentToken() - Get the token of the current GNode.
int getLeftToken() - Returns the token of the current GNode's left child.
int getRightToken() - Returns the token of the current GNode's left child.
bool currentIsRed() - Returns true if current is red.
bool currentIsBlack() - Returns true if current is black.
bool leftIsRed() - Returns true if left is red.
bool leftIsBlack() - Returns true if left is black.
bool rightIsRed() - Returns true if right is red.
bool rightIsBlack() - Returns true if right is black.
bool headIsEmpty() - Return true if head is undefined.
bool leftIsEmpty() - Returns true if the left child of current is NULL.
bool rightIsEmpty() - Returns true if the right child of current is NULL.
void setHeadBlack() - Sets the head to black.
void setHeadToken(int)  - Set the token of the head.
void setLeftToken(int) - Set the token of the left child.
void setRightToken(int) - Set the token of the right child.
void setLeftRed() - Change the color of the left child to red.
void setLeftBlack() - Change the color of the left child to black.
void setRightRed() - Change the color of the right child to red.
void setRightBlack() - Change the color of the right child to black.
void invertCurrentColor() - Invert the color of the current node. If it was black, it will be red after this function is called. If it was red, it will be black after this function is called.
void updateLeftParent() - Update's the left child's parent pointer to be current, as it should be.
void updateRightParent() - Update's the right child's parent pointer to be current, as it should be.
void moveLeft() - Move current to the left child.
void moveRight() - Move current to the right child.
void resetCurrent() - Resets the position of current back to the head.
void add(int) - Adds the int to the GTree in the correct place.
void printTree() - Print out the whole tree to console in a visual format. It does so by checking the first node's children, and then by checking each of their children, and then by checking each of their children, etc. until you have reached the very bottom of the list, in which case instead of checking the new non-existant children, it will go on to print out the last node's token, and then it will recursively work its way backwards through each recursive function, printing out each parent as it goes, until it gets back to the very top, at which point it will simply print the head.
int numberCount(int) - Returns the amount of times that a specific number was found inside the tree.

Private functions:

void checkCases(GNode*) - Makes sure that the node is passed in does not break the Red-Black Tree algorithm rules. If it does break the rules, which it probably will, it will rearrange the tree so that all of the rules are once again being followed.
void checkChildren(GNode*&, int) - Call this function for any existing children of the inputted GNode, then print out the inputted GNode's token.
void searchChildren(GNode*&, int) - Same as checkChildren, except instead of printing out each node, it searches each node and finds out how many of a specified number that there is within the tree.

@author Greggory Hickman, March-April 2020
*/
 
#include <iostream>
#include <math.h>
#include <windows.h>

#define LEN 100
#define BIGLEN 3000

using namespace std;

class GTree {
	public:
		//Constructor & Destructor
		GTree();
		~GTree();
		
		//Functions
		int getCurrentToken();
		//int getParent();
		int getLeftToken();
		int getRightToken();
		bool currentIsRed();
		bool currentIsBlack();
		bool leftIsRed();
		bool leftIsBlack();
		bool rightIsRed();
		bool rightIsBlack();
		bool headIsEmpty();
		bool leftIsEmpty();
		bool rightIsEmpty();
		void setHeadBlack();
		void setHeadToken(int);
		void setLeftToken(int);
		void setRightToken(int);
		//void setLeftAddress(char*);
		//void setRightAddress(char*);
		void setLeftRed();
		void setLeftBlack();
		void setRightRed();
		void setRightBlack();
		void invertCurrentColor();
		void updateLeftParent();
		void updateRightParent();
		void moveLeft();
		void moveRight();
		void resetCurrent();
		void add(int);
		void remove(int);
		void printTree();
		int numberCount(int);
		//int* flushTree();
		
	private:
		struct GNode {
			bool isRed;
			int token;
			//char* path; //This says where the node currently is in the tree right now. A zero means a left turn, a one means a right turn. For example, a path of "101011" means that to the get to the node, you must start at the head, then go right, then left, then right, then left, and then right two more times in order to find where the node is.
			GNode* parent;
			GNode* left;
			GNode* right;
			
			GNode() {
				isRed = true;
				token = -1;
				//path = new char[LEN];
				parent = NULL;
				left = NULL;
				right = NULL;
				
				/*
				//Clear the path!
				for (int i = 0; i < LEN; i++) {
					path[i] = (char)0;
				}
				*/
			}
		};
		
		//Private functions
		bool isHead(GNode*);
		bool isLeft(GNode*);
		bool isRight(GNode*);
		bool isRed(GNode*);
		bool isBlack(GNode*);
		void setRed(GNode*);
		void setBlack(GNode*);
		void invertColor(GNode*);
		void setLeft(GNode*);
		void setRight(GNode*);
		//void rotateLeft(GNode*);
		//void rotateRight(GNode*);
		void rotateLeft(GNode*);
		void rotateRight(GNode*);
		GNode* getLeft(GNode*);
		GNode* getRight(GNode*);
		GNode* getParent(GNode*, int);
		GNode* getSibling(GNode*);
		GNode* getUncle(GNode*);
		
		//Recursive functions which are also private
		void checkCasesRemove(GNode*);
		void checkCases(GNode*);
		void checkChildren(GNode*&, int, char*);
		void searchChildren(GNode*&, int);
		GNode* kidnapChild(GNode*&, int);
		//void fillTreeGuts(GNode*&);
		//void flushChildren(GNode*&);
		//void updateParentPointers(GNode*&);
	
		//Class variables
		int* treeGuts;
		//GNode** pureTreeGuts;
		//int longestPathLen;
		int count; //An iterator. You can basically ignore this variable with no consequence.
		GNode* head; //The head of the tree
		GNode* current; //The current GNode that the user is looking at.
};