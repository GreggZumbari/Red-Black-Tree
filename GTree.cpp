//GTree.cpp
//Changes cout text color: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
#include <windows.h>

#include "GTree.h"

//Constructor
GTree::GTree() {
	head = new GNode();
	resetCurrent();
	highestGeneration = 0;
}

GTree::~GTree() {
	delete[] head;
	delete[] current;
	delete[] treeGuts;
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
	current->left = newNode;
}

void GTree::setRightToken(int value) {
	GNode* newNode = new GNode();
	newNode->token = value;
	current->right = newNode;
}

void GTree::setLeftAddress(char* path) {
	current->left->path = path;
}

void GTree::setRightAddress(char* path) {
	current->right->path = path;
}

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
	setHeadBlack(); //Just always set head to be black so that there is no way anybody can ever make head accidentally not black
	resetCurrent(); //Set current to the very top
	
	char* path = new char[LEN]; //This represents the node's path
	for (int i = 0; i < LEN; i++) {
		path[i] = (char)0;
	}
	
	//If the head is undefined
	if (headIsEmpty()) {
		//Set the head to be the new token
		setHeadToken(newToken);
		setHeadBlack();
	}
	//If the head is all set and ready to roll
	else {
		//Go through the tree and stick the new node in purely based on its number value. We are not worrying about the colors yet.
		while (true) {
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
					updateLeftParent(); //Update the left child to show current as the parent
					setLeftAddress(path); //Set the left address to be what it is
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
					updateRightParent(); //Update the right child to show current as the parent
					setRightAddress(path); //Set the right address to be what it is
					break;
				}
			}
		}
	}
	
	//Now make the tree satisfy the rules of the Red-Black Tree gods
	if (currentIsRed()) {
		if (!leftIsEmpty() && leftIsRed()) {
			setLeftBlack();
		}
		if (!rightIsEmpty() && rightIsRed()) {
			setRightBlack();
		}
	}
	
	//One more time for extra measure, also because somebody might have actually changed it back to red like an absolute egghead
	setHeadBlack();
	
	return;
}

void GTree::printTree() {
	/*
	All colors pulled from the c++ api for reference

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
	highestGeneration = 0;
	
	/*
	//Initialize pureTreeGuts
	pureTreeGuts = new GNode[LEN];
	for (int i = 0; i < LEN; i++) {
		pureTreeGuts[i].token = -1; //Clear the list
	}
	*/
	
	cout << "Tree: " << endl << "---" << endl;
	//Make sure that there is something in the tree before we go and print it
	if (!headIsEmpty()) {
		resetCurrent();
		
		//If head is red, which means Greggory Hickman did something incorrectly
		//The head should only ever be black, but in case it ever isn't for some reason, we want the program to show that
		if (currentIsRed()) {
			//Set all text after this point to be red
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //Set text to red
		}
		//If head is black, which it should be always
		else {
			//Set all text after this point to be dark grey
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //Set text to gray
		}
		
		cout << "Generation 1: " << head->token << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
		
		checkChildren(head, generation);
		
		/*
		//Pretty looking tree technique, commence!
		for (int i = 0; i < highestGeneration; i++) {
			//Epic tree printout here
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

//Private functions only after this point

bool GTree::isLeft(GNode* node) {
	//If parent is null return false
	if (node->parent == NULL) {
		return false;
	}
	//If parent's left child is node, return true
	if (node->parent->left == node) {
		return true;
	}
	//Otherwise, return false
	return false;
}

bool GTree::isRight(GNode* node) {
	//If parent is null return false
	if (node->parent == NULL) {
		return false;
	}
	//If parent's right child is node, return true
	if (node->parent->right == node) {
		return true;
	}
	//Otherwise, return false
	return false;
}

GTree::GNode* GTree::getLeft(GNode* node) {
	return node->left;
}

GTree::GNode* GTree::getRight(GNode* node) {
	return node->right;
}

GTree::GNode* GTree::getParent(GNode* node, int generations) {
	GNode* buff = node;
	for (int i = 0; i < generations; i++) {
		//If buff has no parent, aka is the head
		if (buff->parent == NULL) {
			buff == NULL;
			break;
		}
		//We shouldn't need an else because the above if has a break in it, but I'm putting this here anyway just in case
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
	
	return buff;
}

//Recursives only after this point

void GTree::checkChildren(GNode*& node, int generation) {
	
	/*
	//Put current token in pureTreeGuts
	for (int i = 0; i < BIGLEN; i++) {
		if (pureTreeGuts[i].token == -1) {
			pureTreeGuts[i] = *node;
			break;
		}
	}
	*/
	
	//If the left child isn't NULL, check both of their children
	if (node->left != NULL) {
		//Print out the next node's token now so that we can specify which direction that it is in relation to the current node
		//If the node is red
		if (node->left->isRed) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //Set text to red
			cout << "Generation " << ++generation << " (Left): " << node->left->token << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
		}
		//If the node is black
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //Set text to gray (I would do black but then you couldnt see the text against the background)
			cout << "Generation " << ++generation << " (Left): " << node->left->token << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
		}

		//Update highest if needed
		if (generation > highestGeneration) highestGeneration = generation;
		
		//Check the children of the left node
		checkChildren(node->left, generation); //Check the next generation
		generation--;
	}
	//If the right child isn't NULL, check both of their children
	if (node->right != NULL) {
		//Print out the next node's token now so that we can specify which direction that it is in relation to the current node
		//If the node is red
		if (node->right->isRed) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //Set text to red
			cout << "Generation " << ++generation << " (Right): " << node->right->token << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to white
		}
		//If the node is black
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //Set text to gray (I would do black but then you couldnt see the text against the background)
			cout << "Generation " << ++generation << " (Right): " << node->right->token << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
		}
		
		//Update highest if needed
		if (generation > highestGeneration) highestGeneration = generation;
		
		//Check the children of the right node
		checkChildren(node->right, generation);
		generation--;
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
	
	delete[] node->path;
	delete[] node; //Down the toilet this node goes!
}