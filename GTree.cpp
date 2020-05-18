//GTree.cpp
#include "GTree.h"

/*
 Note: I'm not sure what BG or FG stand for, but we want BG.
 
 Name            BG  FG
 Black           30  40
 Red             31  41
 Green           32  42
 Yellow          33  43
 Blue            34  44
 Magenta         35  45
 Cyan            36  46
 White           37  47
 Bright Black    90  100
 Bright Red      91  101
 Bright Green    92  102
 Bright Yellow   93  103
 Bright Blue     94  104
 Bright Magenta  95  105
 Bright Cyan     96  106
 Bright White    97  107
*/

#define RED "\033[91m"
#define BLACK "\033[90m"
//#define BLACK "\033[3;47;30m"
#define NORMAL "\033[0m"

//GNode Constructor

//Constructor
GTree::GTree() {
	head = new GNode();
	resetCurrent();
	//longestPathLen = 0;
}

GTree::~GTree() {
	delete[] head;
	delete[] current;
}

int GTree::getCurrentToken() {
	return current->token;
}

int GTree::getLeftToken() {
	return current->left->token;
}

int GTree::getRightToken() {
	return current->right->token;
}

bool GTree::currentIsRed() {
	return current->isRed;
}

bool GTree::currentIsBlack() {
	return !current->isRed;
}

bool GTree::leftIsRed(){
	return current->left->isRed;
}

bool GTree::leftIsBlack() {
	return !current->left->isRed;
}

bool GTree::rightIsRed(){
	return current->right->isRed;
}

bool GTree::rightIsBlack() {
	return !current->right->isRed;
}

bool GTree::headIsEmpty() {
	//If the current token is -1, that means that it hasn't been assigned a value yet, and is thus considered null
	//If current is just straight up NULL then yeah, it's gonna be considered null.
	if (head == NULL || head->token == -1) 
		return true;
	else 
		return false;
}

bool GTree::leftIsEmpty() {
	//If the current token is -1, that means that it hasn't been assigned a value yet, and is thus considered null
	//If current is just straight up NULL then yeah, it's gonna be considered null.
	if (current->left == NULL || current->left->token == -1) 
		return true;
	else 
		return false;
}

bool GTree::rightIsEmpty() {
	//If the current token is -1, that means that it hasn't been assigned a value yet, and is thus considered null
	//If current is just straight up NULL then yeah, it's gonna be considered null.
	if (current->right == NULL || current->right->token == -1) 
		return true;
	else 
		return false;
}

void GTree::setHeadBlack() {
	head->isRed = false;
}

void GTree::setHeadToken(int value) {
	head->token = value;
}

void GTree::setLeftToken(int value) {
	GNode* newNode = new GNode();
	newNode->token = value;
	setLeft(newNode);
	//current->left = newNode;
}

void GTree::setRightToken(int value) {
	GNode* newNode = new GNode();
	newNode->token = value;
	setRight(newNode);
	//current->right = newNode;
}

/*
void GTree::setLeftAddress(char* path) {
	current->left->path = path;
}

void GTree::setRightAddress(char* path) {
	current->right->path = path;
}
*/

void GTree::setLeftRed() {
	current->left->isRed = true;
}

void GTree::setLeftBlack() {
	current->left->isRed = false;
}

void GTree::setRightRed() {
	current->right->isRed = true;
}

void GTree::setRightBlack() {
	current->right->isRed = false;
}

void GTree::invertCurrentColor() {
	current->isRed = !current->isRed;
}

void GTree::updateLeftParent() {
	current->left->parent = current; //Set the parent pointer of the left child to be current
}

void GTree::updateRightParent() {
	current->right->parent = current; //Set the parent pointer of the right child to be current
}

void GTree::moveLeft() {
	current = current->left;
}

void GTree::moveRight() {
	current = current->right;
}

void GTree::resetCurrent() {
	current = head;
}

void GTree::add(int newToken) {
	resetCurrent(); //Set current to the very top
	
	int currentPathLen = 0; //This represents the node's current path length. Will get incrementally updated.
	char* path = new char[LEN]; //This represents the node's path
	for (int i = 0; i < LEN; i++) {
		path[i] = (char)0;
	}
	
	//If the head is undefined
	if (headIsEmpty()) {
		//Set the head to be the new token
		setHeadToken(newToken);
		
		/*
		//No need to update current path because we already know that this path is the longest yet, being the first one and all
		longestPathLen = 1; //Update longest path to 1 because if head was undefined, that means that head will be the only node in the tree, meaning that generation 1 is the highest generation. Because head is the only node, that also means we don't need to worry about different amounts of black nodes per leaf on this iteration.
		*/
	}
	
	//If the head is all set and ready to roll
	else {
		//Go through the tree and stick the new node in purely based on its number value. We are not worrying about the colors yet.
		while (true) {
			/*
			//Update path length tallies if current is not red
			if (currentIsBlack()) {
				currentPathLen++;
			}
			*/
			
			//If the new token is smaller than current's token
			if (newToken < getCurrentToken()) {
				//If the left child is defined
				if (!leftIsEmpty()) {
					strcat(path, "0");
					moveLeft(); //Move current one spot to the left
				}
				//If the left child is not defined
				else {
					strcat(path, "0");
					setLeftToken(newToken); //Set the left child to be the new token
					//setLeftAddress(path); //Set the left address to be what it is
					moveLeft(); //Move left so that checkCases() looks at the correct node
					
					break;
				}
			}
			//If the new token is larger than or equal to current's token
			if (newToken >= getCurrentToken()) {
				//If the right child is defined
				if (!rightIsEmpty()) {
					strcat(path, "1");
					moveRight(); //Move current one spot to the right
				}
				//If the right child is not defined
				else {
					strcat(path, "1");
					setRightToken(newToken); //Set the right child to be the new token
					//setRightAddress(path); //Set the right address to be what it is
					moveRight(); //Move right so that checkCases() looks at the correct node
					break;
				}
			}
		}
	}
	
	//Now make the tree satisfy the rules of the Red-Black Tree gods
	//cout << "- Checking cases start -" << endl;
	//cout << "New node: " << getCurrentToken() << endl;
	checkCases(current);
	
	return;
}

void GTree::remove(int newToken) {
	/*
	Just a quick bit of information:
	For the sake of efficiency, I'm going to refer to a thing's original state as _state.
	For example, the original node before it was replaced would be called _node, and the original left child before it was replaced with something else will be referred to as _left.
	All non-underline-prefaced things refer to what the thing is at that particular moment.
	Okay, now onto the code! Enjoy!
	*/
	
	//Find the node that we're going to delete
	GNode* node = kidnapChild(head, newToken);
	
	//Is the node to remove the head????
	if (isHead(node)) {
		cout << "- Node is Head! -" << endl << endl;
	}
	
	//If the kidnapChild() function returned NULL, then that means that no instances of newToken exist within the tree
	if (node == NULL) {
		cout << "No instances of the number \"" << newToken << "\" were found." << endl << endl;
		return;
	}
	
	//Record node's color
	bool wasRed = isRed(node);
	//Record the new node
	GNode* replacement = NULL;
	
	//Go through initial steps
	
	//Initial step 1
	cout << "- Initial Step 1 -" << endl << endl;
	//If node has 2 NULL children
	//Replace node with a NULL leaf
	if (getLeft(node) == NULL && getRight(node) == NULL) {
		cout << "- 0 Children -" << endl;
		//If node is a left child
		if (isHead(node)) {
			head = NULL;
		}
		else if (isLeft(node)) {
			getParent(node, 1)->left = NULL;
		}
		//If node is a right child
		else {
			getParent(node, 1)->right = NULL;
		}
	}
	
	//If node has only 1 NULL child
	//Replace node with the non-NULL child
	else if (getLeft(node) == NULL || getRight(node) == NULL) {
		cout << "- 1 Child -" << endl;
		//If the non-aborted child is the left child
		if (getLeft(node) != NULL) {
			//If node is the head
			if (isHead(node)) {
				//Replace node with _left
				replacement = node->left;
				head = replacement;
			}
			//If node is a left child
			else if (isLeft(node)) {
				//Replace node with _left
				replacement = node->left;
				current = getParent(node, 1);
				setLeft(replacement);
			}
			//If node is a right child
			else {
				//Replace node with _left
				replacement = node->left;
				current = getParent(node, 1);
				setRight(replacement);
			}
		}
		//If the non-aborted child is the right child
		else {
			//If node is the head
			if (isHead(node)) {
				//Replace node with _right
				replacement = node->right;
				head = replacement;
			}
			//If node is a left child
			else if (isLeft(node)) {
				//Replace node with _right
				replacement = node->right;
				current = getParent(node, 1);
				setLeft(replacement);
			}
			//If node is a right child
			else {
				//Replace node with _right
				replacement = node->right;
				current = getParent(node, 1);
				setRight(replacement);
			}
		}
	}
	
	//If node has 2 non-NULL children
	//Replace node with the right child's left child, or, if the right child doesn't have a left child, just replace node with the right child, then put the left child back in place
	else {
		cout << "- 2 Children -" << endl;
		//Keep track of the children's addresses because we are going to need them in a moment here
		GNode* _left = node->left;
		GNode* _right = node->right;
		GNode* _rightleft = node->right->left;
		
		//If node's right child has no left child
		if (node->right->left == NULL) {
			//If node is the head
			if (isHead(node)) {
                cout << "- 2 Children A -" << endl;
				//Replace node with _right
				replacement = _right;
				head = replacement;
			}
			//If node is a left child
			else if (isLeft(node)) {
                cout << "- 2 Children B -" << endl;
				//Replace node with _right
				replacement = _right;
				current = getParent(node, 1);
				setLeft(replacement);
			}
			//If node is a right child
			else {
                cout << "- 2 Children C -" << endl;
				//Replace node with _right
				replacement = _right;
				current = getParent(node, 1);
				setRight(replacement);
			}
			//Stick _left back on to node
			_right->left = _left;
		}
		//If node's right child has a left child
		else {
			//If node is the head
			if (isHead(node)) {
                cout << "- 2 Children D -" << endl;
				//Replace node with _rightleft
				replacement = _rightleft;
				head = replacement;
			}
			//If node is a left child
			else if (isLeft(node)) {
                cout << "- 2 Children E -" << endl;
				//Replace node with _rightleft
				replacement = _rightleft;
				current = getParent(node, 1);
				setLeft(replacement);
			}
			//If node is a right child
			else {
                cout << "- 2 Children F -" << endl;
				//Replace node with _rightleft
				replacement = _rightleft;
				current = getParent(node, 1);
				setRight(replacement);
			}
			
			//Force _right to disown its left child (node->right->left) to avoid turning the tree into a black hole
			_right->left = NULL;
			
			//Stick _left and _right back onto node
			_rightleft->left = _left;
			_rightleft->right = _right;
			
		}
	}
	
	cout << endl;
	
	//Initial step 2
	cout << "- Initial Step 2 -" << endl << endl;
	
	//If _node was red and replacement is red or NULL
	//We are done
	if (wasRed &&
	(replacement == NULL ||
	isRed(replacement))) {
		cout << "- Node is red, rep is red/NULL -" << endl;
		//We are done
		return;
	}
	
	//If _node was red and replacement is black but not NULL
	//Color replacement red, and proceed to cases
	if (wasRed &&
	replacement != NULL &&
	isBlack(replacement)) {
		cout << "- Node is red, rep is black -" << endl;
		//Change rep. to red
		invertColor(replacement);
		//Proceed to cases
		checkCasesRemove(replacement);
	}
	
	//If _node was black and replacement is red
	//Color replacement black, and then we are done
	if (!wasRed &&
	isRed(replacement)) {
		cout << "- Node is black, rep is red -" << endl;
		//Change rep. to black
		invertColor(replacement);
		//We are done
		return;
	}
	
	//If _node was black and replacement is NULL or black (my isBlack() function returns true for NULL nodes as well as non-NULL black nodes)
	//Do nothing, and proceed to cases
	if (!wasRed &&
	isBlack(replacement)) {
		cout << "- Node is black, rep is black/NULL -" << endl;
		//Proceed to cases
		checkCasesRemove(replacement);
	}
}

void GTree::printTree() {
	
	/*
	//Credit to Fedele for showing how to do colored text on https://stackoverflow.com/questions/24281603/c-underline-output
    //Fedele is great and all but their method only works for Windows, and I find myself needed to code on a Mac, so I will be switching to the g++ native method. My Dad showed me how to do this one, and he originally found out about it from someone on Stack Overflow, so I'm not exactly sure who to credit here. We'll just pretend that I got it straight from the API.

	*/
	
	/*
	All color ids pulled from the c++ api for reference

	0   BLACK
	1   BLUE
	2   GREEN
	3   CYAN
	4   RED
	5   MAGENTA
	6   BROWN
	7   LIGHTGRAY
	8   DARKGRAY
	9   LIGHTBLUE
	10  LIGHTGREEN
	11  LIGHTCYAN
	12  LIGHTRED
	13  LIGHTMAGENTA
	14  YELLOW
	15  WHITE
	*/
	
	//Print the tree
	//Starting out at the first generation
	int generation = 1;
	//longestPathLen = 0;
	
	cout << "Tree: " << endl << "---" << endl;
	//Make sure that there is something in the tree before we go and print it
	if (!headIsEmpty()) {
		resetCurrent();
		
		//If head is red, which means Greggory Hickman did something incorrectly
		//The head should only ever be black, but in case it ever isn't for some reason, we want the program to show that
		if (currentIsRed()) {
			//Set all text after this point to be red
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //Set text to red
            cout << RED; //Set future text to be red
		}
		//If head is black, which it should be always
		else {
			//Set all text after this point to be dark grey
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //Set text to gray
            cout << BLACK; //Set future text to be dark gray, which is our "black"
		}
		
		cout << "head: " << head->token << " (Generation 1)" << endl;
		
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
        cout << NORMAL; //Set future text to normal, which is light gray
        
		char* dynamicPath = new char[LEN]; //This will contain the dynamic path
		strcpy(dynamicPath, "head");
		checkChildren(head, generation, dynamicPath);
		
		//Pretty looking tree technique, commence!
		/*
		char progressivePath[LEN];
		//Cycle through each generation
		longestPathLen = 5;
		for (int i = 0; i < longestPathLen; i++) {
			//Cycle through each possible slot in that generation
			for (int j = 0; j < pow(2, i); j++) {
				cout << j << endl;
			}
		}
		*/
		
	}
	else {
		cout << "EMPTY" << endl;
	}
	
	//Print out the paths of all of the numbers
	/*
	for (int i = 0; i < LEN; i++) {
		if (pureTreeGuts[i].token == -1) break;
		cout << pureTreeGuts[i].token << ", " << pureTreeGuts[i].path << endl;
	}
	*/
	
	cout << "---" << endl << endl;
	//delete[] pureTreeGuts;
}

int GTree::numberCount(int value) {
	count = 0;
	//Check the entire tree starting from head
	searchChildren(head, value);
	return count;
}

/*
int* GTree::flushTree() {
	count = 0;
	//We are going to use the class variable "treeGuts" to store our tokens after the tree is completely emptied
	treeGuts = new int[BIGLEN];
	for (int i = 0; i < BIGLEN; i++) {
		treeGuts[i] = -1;
	}
	if (!headIsEmpty()) {
		//Record head
		treeGuts[count] = head->token;
		count++;
		//Flush the entire tree
		flushChildren(head);
	}
	head = new GNode();
	resetCurrent();
	//Return the complete list of tokens that we recorded along the way
	return treeGuts;
}	
*/

//Private functions only after this point

bool GTree::isHead(GNode* node) {
	//If node is the head return true
	if (node == head) return true;
	//Otherwise return false
	return false;
}

bool GTree::isLeft(GNode* node) {
	//If node is NULL return false
	if (node == NULL) return false;
	//If parent is null return false
	if (node->parent == NULL) return false;
	//If parent's left child is node and not NULL, return true
	if (node->parent->left != NULL && node->parent->left == node) return true;
	//Otherwise, return false
	return false;
}

bool GTree::isRight(GNode* node) {
	//If node is NULL return false
	if (node == NULL) return false;
	//If parent is null return false
	if (node->parent == NULL) return false;
	//If parent's right child is node and not NULL, return true
	if (node->parent->right != NULL && node->parent->right == node) return true;
	//Otherwise, return false
	return false;
}

bool GTree::isRed(GNode* node) {
	if (node == NULL) {
		return false; //Because NULL is black
	}
	return node->isRed;
}

bool GTree::isBlack(GNode* node) {
	if (node == NULL) {
		return true; //Because NULL is black
	}
	return !node->isRed;
}

void GTree::setRed(GNode* node) {
	node->isRed = true;
}

void GTree::setBlack(GNode* node) {
	node->isRed = false;
}

void GTree::invertColor(GNode* node) {
	node->isRed = !node->isRed;
}

void GTree::setLeft(GNode* node) {
	current->left = node;
	if (node != NULL) {
		node->parent = current;
	}
}
		
void GTree::setRight(GNode* node) {
	current->right = node;
	if (node != NULL) {
		node->parent = current;
	}
}

void GTree::rotateLeft(GNode* _parent) {
	//If parent is a left child
	if (isLeft(_parent)) {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->right;
		GNode* _rightleft = _parent->right->left;
		
		current = _parent; //Set current to point at parent
		setRight(_rightleft); //Set parent's right child to node's left child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setLeft(_node); //Set grandparent's left child to node, replacing parent
		
		current = _node;
		setLeft(_parent); //Set node's left child to parent, replacing node's old left child
	}
	//If parent is a right child
	else if (isRight(_parent)) {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->right;
		GNode* _rightleft = _parent->right->left;
		
		current = _parent; //Set current to point at parent
		setRight(_rightleft); //Set parent's right child to node's left child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setRight(_node); //Set grandparent's right child to node, replacing parent
		
		current = _node;
		setLeft(_parent); //Set node's left child to parent, replacing node's old left child
	}
	//If parent is head
	else {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->right;
		GNode* _rightleft = _parent->right->left;
		
		current = _parent; //Set current to point at parent
		setRight(_rightleft); //Set parent's right child to node's left child, replacing node
		
		head = _node; //Set grandparent's right child to node, replacing parent
		
		current = _node;
		setLeft(_parent); //Set node's left child to parent, replacing node's old left child
	}
}
		
void GTree::rotateRight(GNode* _parent) {
	//If parent is a left child
	if (isLeft(_parent)) {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->left;
		GNode* _leftright = _parent->left->right;
		
		current = _parent; //Set current to point at parent
		setLeft(_leftright); //Set parent's left child to node's right child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setLeft(_node); //Set grandparent's left child to node, replacing parent
		
		current = _node; //Set current to point at node
		setRight(_parent); //Set node's right child to parent, replacing node's old right child
	}
	//If parent is a right child
	else if (isRight(_parent)) {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->left;
		GNode* _leftright = _parent->left->right;
		
		current = _parent; //Set current to point at parent
		setLeft(_leftright); //Set parent's left child to node's right child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setLeft(_node); //Set grandparent's left child to node, replacing parent
		
		current = _node; //Set current to point at node
		setRight(_parent); //Set node's right child to parent, replacing node's old right child
	}
	//If parent is head
	else {
		//Create a temporary variable to hold all of the addresses
		GNode* _node = _parent->left;
		GNode* _leftright = _parent->left->right;
		
		current = _parent; //Set current to point at parent
		setLeft(_leftright); //Set parent's left child to node's right child, replacing node
		
		head = _node; //Set grandparent's left child to node, replacing parent
		
		current = _node; //Set current to point at node
		setRight(_parent); //Set node's right child to parent, replacing node's old right child
	}
}

GTree::GNode* GTree::getLeft(GNode* node) {
	return node->left;
}

GTree::GNode* GTree::getRight(GNode* node) {
	return node->right;
}

GTree::GNode* GTree::getParent(GNode* node, int generations) {
	//If node is NULL, I don't know what you want from me. Return NULL.
	if (node == NULL) {
		return NULL;
	}
	GNode* buff = node;
	for (int i = 0; i < generations; i++) {
		//If buff has no parent, aka is the head
		if (buff->parent == NULL) {
			return NULL;
		}
		else {
			buff = buff->parent;
		}
	}
	return buff;
}

GTree::GNode* GTree::getSibling(GNode* node) {
	GNode* buff = node;
	
	//If current is a left child
	if (isLeft(node)) {
		buff = getRight(getParent(buff, 1));
	}
	//If current is a right child
	if (isRight(node)) {
		buff = getLeft(getParent(buff, 1));
	}
	
	//If sibling is NULL, that's okay. Then, we just return NULL.
	return buff;
}

GTree::GNode* GTree::getUncle(GNode* node) {
	//If node is NULL, I don't know what you want from me. Return NULL.
	if (node == NULL) {
		return NULL;
	}
	GNode* buff = node;
	//If parent is NULL, there isn't an uncle either, so return NULL
	if (buff->parent == NULL) {
		return NULL;
	}
	//If parent is left, return grandparent's right child, which must be uncle
	else if (isLeft(buff->parent)) {
		return getRight(getParent(buff, 2));
	}
	//If parent is right, return grandparent's left child, which must be uncle
	else if (isRight(buff->parent)) {
		return getLeft(getParent(buff, 2));
	}
	//If the parent is neither a left, nor a right child, and isn't NULL, that means either something went horribly wrong, or parent is the head, in which case, there is no uncle. So, return NULL.
	//By the way, isLeft() and isRight() both return false if parent is NULL. If only have the check at the beginning to make this code more readable
	else {
		return NULL;
	}
}

//Recursives only after this point

void GTree::checkCasesRemove(GNode* node) {
	cout << "- Cases yink yoink -" << endl << endl;
	//Go through cases
	
	//Case 1: Node is red
	//Color node black. We are done.
	if (isRed(node)) {
		cout << "- Case 1 -" << endl;
		//Color node black
		setBlack(node);
		//We are done
		return;
	}
	
	//Case 2: Node is black and sibling is red
	//Color _sibling black 
	//Then color _parent red
	//Then rotate _parent (if node is a left child, rotate parent left, vice versa for right) 
	//Then continue on with the rest of the cases
	if (isBlack(node) && 
	isRed(getSibling(node))) {
		cout << "- Case 2 -" << endl;
		//Color sibling black
		setBlack(getSibling(node));
		//Color parent red
		setRed(getParent(node, 1));
		//Rotate through parent
		if (isLeft(node)) {
			//If node is left, rotate parent left
			rotateLeft(getParent(node, 1));
		}
		else if (isRight(node)) {
			//If node is right, rotate parent right
			rotateLeft(getParent(node, 1));
		}
	}
	
	//Case 3: Node is black and sibling is black and both of sibling's children are black
	//Color _sibling red
	//Set node to _parent
	//If new node is red, color node black, and then we are done
	//If new node is black, run all cases on it *again*
	if (isBlack(node) && 
	isBlack(getSibling(node)) && 
	isBlack(getLeft(getSibling(node))) && 
	isBlack(getRight(getSibling(node)))) {
		cout << "- Case 3 -" << endl;
		//Set sibling red
		setRed(getSibling(node));
		//Set node to _parent
		node = getParent(node, 1);
		//If node is red
		if (isRed(node)) {
			//Color node black
			setBlack(node);
			//We are done
			return;
		}
		//If node is black
		else {
			//Check Cases 2: Electric Boogaloo
			checkCasesRemove(node);
			//Return so that we don't run cases 4 and 5 on node again
			return;
		}
	}
	
	//Case 4: Node is black and its sibling is black and...
	if (isBlack(node) && 
	isBlack(getSibling(node))) {
		GNode* _sibling = getSibling(node); //Yay, efficiency!
		
		//If node is the left child, sibling's left child is red and sibling's right child is black
		//Color sibling's left child black
		//Color sibling red
		//Rotate right through sibling
		//Proceed to case 5
		if (isLeft(node) && 
		isRed(getLeft(getSibling(node))) && 
		isBlack(getRight(getSibling(node)))) {
			cout << "- Case 4L -" << endl;
			//Color sib's left child black
			setBlack(getLeft(_sibling));
			//Color sib red
			setRed(_sibling);
			//Rotate right through sib
			rotateRight(_sibling);
			//Proceed to case 5
		}
		
		//If node is the right child, sibling's right child is red and sibling's left child is black
		//Color sibling's right child black
		//Color sibling red
		//Rotate left through sibling
		//Proceed to case 5
		else if (isRight(node) && 
		isRed(getRight(getSibling(node))) && 
		isBlack(getLeft(getSibling(node)))) {
			cout << "- Case 4R -" << endl;
			//Color sib's right child black
			setBlack(getRight(_sibling));
			//Color sib red
			setRed(_sibling);
			//Rotate left through sib
			rotateLeft(_sibling);
			//Proceed to case 5
		}
	}
	
	//Case 5: Node is black and its sibling is black and...
	if (isBlack(node) &&
	isBlack(getSibling(node))) {
		GNode* _sibling = getSibling(node);
		GNode* _parent = getParent(node, 1);
		
		//If node is the left child and sibling's right child is red
		//Color _sibling to be the same color as _parent
		//Color _parent black
		//Color _sibling's right child black
		//Rotate left through parent
		//We are done
		if (isLeft(node) && 
		isRed(getRight(getSibling(node)))) {
			cout << "- Case 5L -" << endl;
			//Color sibling to be the same color as parent
			_sibling->isRed = _parent->isRed;
			//Color parent black
			setBlack(_parent);
			//Color _sibling's right child black
			setBlack(getRight(_sibling));
			//Rotate left through padre
			rotateLeft(_parent);
			//We are done!!!! Booyah
			return;
		}
		
		//If node is the right child and sibling's left child is red
		//Color _sibling to be the same color as _parent
		//Color _parent black
		//Color _sibling's left child black
		//Rotate right through parent
		//We are done
		if (isRight(node) && 
		isRed(getLeft(getSibling(node)))) {
			cout << "- Case 5R -" << endl;
			//Color sibling to be the same color as parent
			_sibling->isRed = _parent->isRed;
			//Color parent black
			setBlack(_parent);
			//Color _sibling's left child black
			setBlack(getLeft(_sibling));
			//Rotate right through padre
			rotateRight(_parent);
			//We are done!!!! Booyah
			return;
		}
	}
	
	//Done!
	return;
}
	

void GTree::checkCases(GNode* node) {
	//printTree();
	
	/*
	Some quick clarifying information:
	Most people call the very top node from which all other nodes branch off of the "root". This makes no sense to me, because roots spread outwards and are plentiful, and there's not just one of them usually. Instead, I'm calling it the "head".
	I'm still calling the NULL pointers at the very ends of the branches "leaves".
	I'm calling grandparent's parent "ancestor"
	*/
	
	//Case 2: If parent is black and node is red
	//Do nothing. Why does this case exist?
	//cout << "- Case 2 -" << endl;
	//printTree();
	
	//Case 3: If parent and uncle are both red (and also grandpa is black)
	//Make parent and uncle black and grandpa red, then, run through all 5 cases again on the grandparent
	if (isRed(getParent(node, 1)) && isRed(getUncle(node))) {
		setBlack(getParent(node, 1)); //Set parent black
		setBlack(getUncle(node)); //Set uncle black
		setRed(getParent(node, 2)); //Set grandparent red
		//cout << "- Checking grandparent from Case 3 -" << endl;
		//cout << "Grandparent: " << getParent(node, 2)->token << endl;
		//cout << "- Case 3: True -" << endl;
		
		checkCases(getParent(node, 2)); //Check grandpa
		//cout << "- Done checking on grandparent from Case 3 -" << endl;
	}
	else {
		//cout << "- Case 3: False -" << endl;
	}
	//printTree();
	
	//Case 4: If uncle is black, parent and node are both red, parent is left, and node is right. Or, if uncle is black, parent is right, and node is left
	//Rotate through parent
	if (isBlack(getUncle(node)) &&
	isRed(getParent(node, 1)) &&
	isRed(node) &&
	(getParent(node, 1) == NULL || isLeft(getParent(node, 1))) && /* If parent is NULL, it will return false for left always */
	isRight(node)) {
		
		GNode* _parent = getParent(node, 1); //Create a temporary variable to hold parent's address
		rotateLeft(_parent);
		/*
		current = _parent; //Set current to point at parent
		setRight(getLeft(node)); //Set parent's right child to node's left child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setLeft(node); //Set grandparent's left child to node, replacing parent
		
		current = node;
		setLeft(_parent); //Set node's left child to parent, replacing node's old left child
		*/
		
		//Update all parent pointers to be correct
		//node->parent = _parent->parent; //Set node's parent to grandparent
		//_parent->parent = node; //Set parent's parent to node
		
		node = _parent; //Parent is now the bottom-most node, thus, it should be treated as the new "node" now
		
		//cout << "- Case 4: True -" << endl;
	}
	else if (isBlack(getUncle(node)) && 
	isRed(getParent(node, 1)) &&
	isRed(node) &&
	(isRight(getParent(node, 1)) || getParent(node, 1) == NULL) && 
	isLeft(node)) {
		
		GNode* _parent = getParent(node, 1); //Create a temporary variable to hold parent's address
		rotateRight(_parent);
		
		/*
		current = _parent; //Set current to point at parent
		setLeft(getRight(node)); //Set parent's left child to node's right child, replacing node
		
		current = getParent(_parent, 1); //Get grandparent
		setRight(node); //Set grandparent's right child to node, replacing parent
		
		current = node;
		setRight(_parent); //Set node's right child to parent, replacing node's old right child
		
		//Update all parent pointers to be correct
		//node->parent = _parent->parent; //Set node's parent to grandparent
		//_parent->parent = node; //Set parent's parent to node
		*/
		
		node = _parent; //Parent is now the bottom-most node, thus, it should be treated as the new "node" now
		
		//cout << "- Case 4: True -" << endl;
	}
	else {
		//cout << "- Case 4: False -" << endl;
	}
	//printTree();
	
	//Case 5: If uncle is black, parent and node are both red, parent is left, and node is left. Or, if uncle is black, parent is right, and node is right
	//Rotate through grandparent, then switch the old parent and old grandparent's colors
	if (isBlack(getUncle(node)) && 
	isRed(getParent(node, 1)) &&
	isRed(node) &&
	isLeft(getParent(node, 1)) && 
	isLeft(node)) {
		GNode* _parent = getParent(node, 1); //Create a temporary pointer to the parent (also for convenience and efficiency)
		GNode* _grandparent = getParent(node, 2); //Create a temporary pointer to the grandparent (WE REALLY NEED THIS POINTER!! DON'T REMOVE IT UNLESS ITS AN EMERGENCY!!!!)

		rotateRight(_grandparent);
		
		/*
		//If grandparent is a left child
		if (isLeft(_grandparent)) {
			current = getParent(_grandparent, 1); //Set current to ancestor
			setLeft(getParent(node, 1)); //Set ancestor's left child to be parent
		}
		//If grandparent is a right child
		else if (isRight(_grandparent)) {
			current = getParent(_grandparent, 1); //Set current to ancestor
			setRight(getParent(node, 1)); //Set ancestor's left child to be parent
		} 
		//Otherwise, grandparent must be the head
		else {
			head = getParent(node, 1); //Set head to be parent
		}
		
		current = _grandparent; //Set current to grandparent
		setLeft(getRight(_parent)); //Set grandparent's left child to be parent's right child
		
		current = _parent; //Set current to parent
		setRight(_grandparent); //Set parent's right child to be grandparent
		*/
		
		//Swap the colors of grandparent and parent
		bool isGrandpaRed = isRed(_grandparent);
		_grandparent->isRed = _parent->isRed;
		_parent->isRed = isGrandpaRed;
		
		//No need for "node" to be updated because this is the last case, but I'll do it anyway
		node = getRight(_grandparent); //The node formerly known as grandparent is now the parent, so it's child should now be the new "node"
		
		//cout << "- Case 5: True -" << endl;
	}
	else if (isBlack(getUncle(node)) && 
	isRed(getParent(node, 1)) &&
	isRed(node) &&
	isRight(getParent(node, 1)) && 
	isRight(node)) {
		GNode* _parent = getParent(node, 1); //Create a temporary pointer to the parent (also for convenience and efficiency)
		GNode* _grandparent = getParent(node, 2); //Create a temporary pointer to the grandparent (WE REALLY NEED THIS POINTER!! DON'T REMOVE IT UNLESS ITS AN EMERGENCY!!!!)
		
		rotateLeft(_grandparent);
		
		/*
		//If grandparent is a left child
		if (isLeft(_grandparent)) {
			current = getParent(_grandparent, 1); //Set current to ancestor
			setLeft(getParent(node, 1)); //Set ancestor's left child to be parent
		}
		//If grandparent is a right child
		else if (isRight(_grandparent)) {
			current = getParent(_grandparent, 1); //Set current to ancestor
			setRight(getParent(node, 1)); //Set ancestor's left child to be parent
		} 
		//Otherwise, grandparent must be the head
		else {
			head = getParent(node, 1); //Set head to be parent
		}
		
		current = _grandparent; //Set current to grandparent
		setRight(getLeft(_parent)); //Set grandparent's right child to be parent's left child
		
		current = _parent; //Set current to parent
		setLeft(_grandparent); //Set parent's left child to be grandparent
		*/
		
		//Swap the colors of grandparent and parent
		bool isGrandpaRed = isRed(_grandparent);
		_grandparent->isRed = _parent->isRed;
		_parent->isRed = isGrandpaRed;
		
		//No need for "node" to be updated because this is the last case, but I'll do it anyway
		node = getRight(_grandparent); //The node formerly known as grandparent is now the parent, so it's child should now be the new "node"
		
		//cout << "- Case 5: True -" << endl;
	}
	else {
		//cout << "- Case 5: False -" << endl;
	}
	//printTree();
	
	//Case 1: If the node is being inserted at the head
	//~WARNING~ If you put this at the beginning like Lord Galbraith said to, then sometimes, the head ends up red rather than black because of a rotation. So, this needs to stay here at the end.
	//I think it's more efficient to just automatically set the head to black every single time rather than query about it every single time
	setHeadBlack();
	//cout << "- Case 1 -" << endl;
	//printTree();
}

void GTree::checkChildren(GNode*& node, int generation, char* inPath) {
	char* outPath1;
	char* outPath2;
	
	//If the left child isn't NULL, check both of their children
	if (node->left != NULL) {
		//Print out the next node's token now so that we can specify which direction that it is in relation to the current node
		outPath1 = new char[LEN];
		strcpy(outPath1, inPath);
		strcat(outPath1, "->left"); //Update dynamic path
		
		//If the node is red
		if (node->left->isRed) {
            cout << RED; //Set text to red
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			//cout << "Generation " << ++generation << " (Left): " << node->left->token << endl;
            
			cout << outPath1 << ": " << node->left->token << " (Generation " << ++generation << ")" << endl;
            
            cout << NORMAL; //Set text back to light gray
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
		//If the node is black
		else {
            cout << BLACK; //Set text to gray (I would do black but then you couldnt see the text against the background)
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			//cout << "Generation " << ++generation << " (Left): " << node->left->token << endl;
			
            cout << outPath1 << ": " << node->left->token << " (Generation " << ++generation << ")" << endl;
            
            cout << NORMAL; //Set text back to light gray
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}

		/*
		//Update highest if needed
		if (generation > longestPathLen) longestPathLen = generation;
		*/
		
		//Check the children of the left node
		checkChildren(node->left, generation, outPath1); //Check the next generation
		generation--;
        
        delete[] outPath1; //Clear up that space once we're done here
	}
	//If the right child isn't NULL, check both of their children
	if (node->right != NULL) {
		//Print out the next node's token now so that we can specify which direction that it is in relation to the current node
		outPath2 = new char[LEN];
		strcpy(outPath2, inPath);
		strcat(outPath2, "->right"); //Update dynamic path
		
		//If the node is red
		if (node->right->isRed) {
			cout << RED; //Set text to red
			//cout << "Generation " << ++generation << " (Right): " << node->right->token << endl;
			cout << outPath2 << ": " << node->right->token << " (Generation " << ++generation << ")" << endl;
			cout << NORMAL; //Set text back to light gray
		}
		//If the node is black
		else {
			cout << BLACK; //Set text to gray (I would do black but then you couldnt see the text against the background)
			//cout << "Generation " << ++generation << " (Right): " << node->right->token << endl;
			cout << outPath2 << ": " << node->right->token << " (Generation " << ++generation << ")" << endl;
			cout << NORMAL; //Set text back to light gray
		}
		
		/*
		//Update highest if needed
		if (generation > longestPathLen) longestPathLen = generation;
		*/
		
		//Check the children of the right node
		checkChildren(node->right, generation, outPath2);
		generation--;
        
        delete[] outPath2; //Clear up that space once we're done here
	}
}

void GTree::searchChildren(GNode*& node, int number) {
	//If the current token is the number that we're searching for
	if (node->token == number) {
		//Increment the counter
		count++;
	}
	//If the left child isn't NULL, check both of their children
	if (node->left != NULL) {
		//Check the children of the left node
		searchChildren(node->left, number); //Check the next generation
	}
	//If the right child isn't NULL, check both of their children
	if (node->right != NULL) {
		//Check the children of the right node
		searchChildren(node->right, number);
	}
}

GTree::GNode* GTree::kidnapChild(GNode*& node, int number) {
	//This is the variable that the GNode that we want is going to travel back up the recursion in
	GNode* childToKidnap = NULL;
	//If the current token is the number that we're searching for...
	//cout << "Node token: " << node->token << ", Number: " << number << endl;
	if (childToKidnap == NULL && node->token == number) {
		//...send it back up to the top of the recursive function!
		return node;
	}
	//If the left child isn't NULL, and we haven't found our child yet, check both of their children
	if (childToKidnap == NULL && node->left != NULL) {
		//Check the children of the left node
		childToKidnap = kidnapChild(node->left, number); //Check the next generation
	}
	//If the right child isn't NULL, and we haven't found our child yet, check both of their children
	if (childToKidnap == NULL && node->right != NULL) {
		//Check the children of the right node
		childToKidnap = kidnapChild(node->right, number);
	}
	return childToKidnap;
}

/*
void GTree::flushChildren(GNode*& node) {
	//If the left child isn't NULL, check both of their children
	if (node->left != NULL) {
		//Record the current token
		treeGuts[count] = node->left->token;
		count++;
		
		//Flush the children of the left node
		flushChildren(node->left); //Check the next generation
	}
	//If the right child isn't NULL, check both of their children
	if (node->right != NULL) {
		//Record the current token
		treeGuts[count] = node->right->token;
		count++;
		
		//Flush the children of the right node
		flushChildren(node->right);
	}
	
	delete[] node; //Down the toilet this node goes!
}
*/

/*
void GTree::updateParentPointers(GNode*& node) {
	//Kill head's parents if they exist
	head->parent = NULL;
	
	//Check left child
	if (node->left != NULL) {
		//Update left child's parent
		node->left->parent = &*node;
		//Check left's children
		updateParentPointers(node->left);
	}
	//Check right child
	if (node->right != NULL) {
		//Update right child's parent
		node->right->parent = &*node;
		//Check right's children
		updateParentPointers(node->right);
	}
}
*/

/*
void GTree::fillTreeGuts(GNode*& node) {
	//If the left child isn't NULL, check both of their children
	if (node->left != NULL) {
		//Notices you're black owo
		pureTreeGuts[count] = node->left;
		count++;
		
		//Check the children of the right node
		fillTreeGuts(node->left); //Check the next generation
	}
	//If the right child isn't NULL, check both of their children
	if (node->right != NULL) {
		//Record the current token
		pureTreeGuts[count] = node->right;
		count++;
		
		//Check the children of the right node
		fillTreeGuts(node->right);
	}
	return;
}
*/
