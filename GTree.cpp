//GTree.cpp
//Changes cout text color: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
#include <windows.h>

#include "GTree.h"

//Constructor
GTree::GTree() {
	head = new GNode();
	head->isRed = false;
	current = head;
	highestGeneration = 0;
}

GTree::~GTree() {
	delete[] head;
	delete[] current;
	delete[] treeGuts;
}

int GTree::getCurrent() {
	return current->token;
}

int GTree::getLeft() {
	return current->left->token;
}

int GTree::getRight() {
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
	if (current == NULL || current->token == -1) 
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

void GTree::setHead(int value) {
	head->token = value;
}

void GTree::setLeft(int value) {
	GNode* newNode = new GNode();
	newNode->token = value;
	current->left = newNode;
}

void GTree::setRight(int value) {
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
	resetCurrent(); //Set current to the very top
	
	char* path = new char[LEN]; //This represents the node's path
	for (int i = 0; i < LEN; i++) {
		path[i] = (char)0;
	}
	
	//If the head is undefined
	if (headIsEmpty()) {
		//Set the head to be the new token
		setHead(newToken);
	}
	//If the head is all set and ready to roll
	else {
		//Go through the tree and stick the new node in purely based on its number value. We are not worrying about the colors yet.
		while (true) {
			//If the new token is smaller than current's token
			if (newToken < getCurrent()) {
				//If the left child is defined
				if (!leftIsEmpty()) {
					strcat(path, "0");
					moveLeft(); //Move current one spot to the left
				}
				//If the left child is not defined
				else {
					strcat(path, "0");
					setLeft(newToken); //Set the left child to be the new token
					updateLeftParent(); //Update the left child to show current as the parent
					setLeftAddress(path); //Set the left address to be what it is
					break;
				}
			}
			//If the new token is larger than or equal to current's token
			if (newToken >= getCurrent()) {
				//If the right child is defined
				if (!rightIsEmpty()) {
					strcat(path, "1");
					moveRight(); //Move current one spot to the right
				}
				//If the right child is not defined
				else {
					strcat(path, "1");
					setRight(newToken); //Set the right child to be the new token
					updateRightParent(); //Update the right child to show current as the parent
					setRightAddress(path); //Set the right address to be what it is
					break;
				}
			}
		}
	}
	
	//Now make the tree satisfy the rules of the Red-Black Tree gods
	//If current is head and head is red
	cout << "Current, Head: " << current << ", " << head << endl;
	cout << "Red: " << current->isRed << endl;
	if (current == head && currentIsBlack()) {
		//That's illegal. Head must be black.
		cout << "Invert current color" << endl;
		invertCurrentColor();
	}
	if (currentIsRed()) {
		if (!leftIsEmpty() && leftIsRed()) {
			cout << "Set left black" << endl;
			setLeftBlack();
		}
		if (!rightIsEmpty() && rightIsRed()) {
			cout << "Set right black" << endl;
			setRightBlack();
		}
	}
	
	return;
}

void GTree::printTree() {
	//Print the tree
	/*
	cout << "Control Print: " << endl;
	cout << "Generation 1: " << head->token << endl;
	if (current->left != NULL) {
		cout << "Generation 2: " << getLeft();
	}
	if (current->right != NULL) {
		cout << ", " << getRight() << endl;
	}
	else {
		cout << endl;
	}
	*/
	//Starting out at the first generation
	int generation = 1;
	highestGeneration = 0;
	
	//Initialize pureTreeGuts
	pureTreeGuts = new GNode[LEN];
	for (int i = 0; i < LEN; i++) {
		pureTreeGuts[i].token = -1; //Clear the list
	}
	
	cout << "Tree: " << endl << "---" << endl;
	//Make sure that there is something in the tree before we go and print it
	if (!headIsEmpty()) {
		
		//Check both children of the head
		//The head should only ever be black, but in case it ever isn't for some reason, we want the program to show that
		if (currentIsRed()) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //Set text to red
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //Set text to red
		}
		
		cout << "Generation 1: " << head->token << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //Set text back to light gray
		
		checkChildren(head, generation);
		
		//Pretty looking tree technique, commence!
		for (int i = 0; i < highestGeneration; i++) {
			//Epic tree printout here
		}
		
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
	delete[] pureTreeGuts;
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

void GTree::binaryInsertionSort(GNode* in) {
	//CURRENTLY BENIGN. THIS DOES NOTHING RIGHT NOW.
	GNode* out = new GNode[LEN]; //Later, we are going to replace 
	for (int i = 0; i < LEN; i++) {
		pureTreeGuts[i].token = -1;
	}
}

/*
GNode* GTree::getParent(int generations) {
	GNode* buff = current;
	for (int i = 0; i < generations; i++) {
		//If buff has no parent, aka is the head
		if (buff->parent == NULL) {
			break;
		}
	}
	return buff;
}
*/

void GTree::checkChildren(GNode*& node, int generation) {
	
	//Put current token in pureTreeGuts
	for (int i = 0; i < BIGLEN; i++) {
		if (pureTreeGuts[i].token == -1) {
			pureTreeGuts[i] = *node;
			break;
		}
	}
	
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