//main.cpp
/*
This program contains a class called "GTree", which gives the programmer an object to use, also called "GTree", which works as a regular Tree should.
A Tree is a kind of binary tree, made up of nodes. Each node points to two (2) other nodes, and each of those point to two (2) other nodes, etc.
The program first allows the user to enter up to 100 different number inputs, then it will create a GTree (like a Tree, except I made it) and sort the numbers such that each parent is bigger than their two children. The program can handle numbers from one (1) to one-thousand (1000).

The above description is nearly exactly copied over from my previous Tree project because this project does nearly exactly the same thing. 
This time, however, you are allowed to add, remove, and search for specific numbers throughout the Tree. Also, the medium for the inputs will now be Node structs (named GNode, of course) instead of an int array, sorted using some clever math.

In addition, the nodes are sorted using my iteration of the Red-Black Tree algorithm.

void addToTree(GTree*, int) - Put a number into the tree in the place that it should be in.
int* addFunction() - Get the user to put in their input(s) to add to the tree, and then print out the parsed version.
int searchFunction() - Get the user to input the number that they want to search the tree for.
void search(GTree*, int) - Print out the amount of times that the inputted number appears in the tree with some random UI stuff added in to make the info understandable.
bool confirmInput() - Simply asks the user for a yes or no response (y/n). Purely for modularity purposes.

@author Greggory Hickman, March 2020
@version 1.0
 */

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <math.h>

#include "BattlePack.h"
#include "GTree.h"

#define LEN 100
#define BIGLEN 3000
#define UNDERLINE "\033[4m"
#define STOPUNDERLINE "\033[0m"

using namespace std;

void sayProgramDescription();
void listCommands();
//void addToTree(GTree*, int);
int* fileFunction();
int* addFunction();
//int* removeFunction();
int searchFunction();
void search(GTree*, int);
bool confirmInput();

int main() {
	
	GTree tree;
	//Define some variables and objects that we will need throughout the program
	bool quit = false;
	int* numbersToAdd; //Going to be the current number(s) to add to the tree
	
	//Epic ASCII art... In order to print out a backslash, I need to type a double-backslash. Each double-backslash actually comes out as just 1 backslash, so this actually looks fine when you run the program.
	cout << "-----------------------------------------------------------" << endl;
	cout << " ____           ____       _______   ____    _____   _____ " << endl;
	cout << "|  _ \\         |  _ \\     |___ ___| |  _ \\  |  ___| |  ___|" << endl;
	cout << "| | \\ \\        | | \\ |       | |    | | \\ \\ | |     | |    " << endl;
	cout << "| |_/ |  ____  | |_/ /       | |    | |_/ | | |__   | |__  " << endl;
	cout << "|   _/  |____| |  _ |        | |    |   _/  |  __|  |  __| " << endl;
	cout << "|   \\          | | \\ \\       | |    |   \\   | |     | |    " << endl;
	cout << "| |\\ \\         | |_/ /       | |    | |\\ \\  | |___  | |___ " << endl;
	cout << "|_| \\_\\        |____/        |_|    |_| \\_\\ |_____| |_____|" << endl;
	cout << "                                                           " << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << endl;
	cout << "By Greggory Hickman" << endl;
	cout << "Made in March and April of 2020" << endl;
	cout << endl;
	
	cout << "Type \"help\" or \"?\" for help" << endl;
	
	//The loop that the majority of the program takes place in
	while (!quit) {
		char cmdin[LEN];
	
		cout << "> ";
		cin >> cmdin; cin.clear(); cin.ignore(LEN, '\n');
		
		cout << endl;
		bool haveInput = false;
		
		//Stay in this section until we have the input
		while (!haveInput) {
			//If the user typed "add"
			if (strcmp(cmdin, "add") == 0 || strcmp(cmdin, "a") == 0 || strcmp(cmdin, "A") == 0) {
				//Ask which method of adding the user would like to use
				cout << "Would you like the type your numbers directly into the console, or would you like to use some numbers stored in a file, separated by spaces?" << endl <<
				"Type \"" << UNDERLINE << "c" << STOPUNDERLINE << "onsole\" for console, or \"" << UNDERLINE << "f" << STOPUNDERLINE << "ile\" for file:" << endl;
				
				char userResponse[LEN]; //This will contain the user's reply to the previous question
				
				//Put the user's response into userResponse. Yes, that's right, i'm getting better at naming variables
				cout << "> ";
				cin >> userResponse; cin.clear(); cin.ignore(LEN, '\n');
				cout << endl;
				
				//Add using the console method
				if (strcmp(userResponse, "console") == 0 || strcmp(userResponse, "c") == 0 || strcmp(userResponse, "C") == 0) {
					//Call the add function
					numbersToAdd = addFunction();
					//Confirm the input
					haveInput = confirmInput();
				}
				//Add using the file method
				else if (strcmp(userResponse, "file") == 0 || strcmp(userResponse, "f") == 0 || strcmp(userResponse, "F") == 0) {
					//Call the file function
					numbersToAdd = fileFunction();
					//Confirm the input
					haveInput = confirmInput();
				}
				else {
					cout << "Input method \"" << cmdin << "\" not recognized" << endl;
				}
			}
			//If the user typed "search"
			else if (strcmp(cmdin, "search") == 0 || strcmp(cmdin, "s") == 0 || strcmp(cmdin, "S") == 0) {
				//Call the search function
				int numberToSearchFor = searchFunction();
				//Tell the user how many times that number appears in the tree
				search(&tree, numberToSearchFor);
				haveInput = true;
			}
			//If the user typed "print"
			else if (strcmp(cmdin, "print") == 0 || strcmp(cmdin, "p") == 0 || strcmp(cmdin, "P") == 0) {
				//Print the tree
				tree.printTree();
				break;
			}
			//If the user typed "about"
			else if (strcmp(cmdin, "about") == 0 || strcmp(cmdin, "b") == 0 || strcmp(cmdin, "B") == 0) {
				//Say program description
				sayProgramDescription();
				break;
			}
			//If the user typed "help"
			else if (strcmp(cmdin, "help") == 0 || strcmp(cmdin, "?") == 0 || /*We want to encourage good grammar here*/ strcmp(cmdin, "Help") == 0) {
				//Print commands
				listCommands();
				break;
			}
			//If the user typed "exit"
			else if (strcmp(cmdin, "exit") == 0 || strcmp(cmdin, "e") == 0 || strcmp(cmdin, "E") == 0) {
				//Exit the program
				cout << "Closing program..." << endl;
				return 0;
			}
			//If the user typed an invalid command
			else {
				cout << "Command \"" << cmdin << "\" not recognized" << endl;
				break;
			}
			cout << endl;
		}
		
		//Now, update the tree
		//If the user previously told the program to add (a) number(s)
		if (strcmp(cmdin, "add") == 0 || strcmp(cmdin, "a") == 0 || strcmp(cmdin, "A") == 0) {
			//Sort each number into the tree one by one
			for (int i = 0; numbersToAdd[i] > 0; i++) {
				//Set the current pointer back to the head
				tree.add(numbersToAdd[i]);
			}
		}
		/*
		//If the user previously told the program to remove (a) number(s)
		else if (strcmp(cmdin, "remove") == 0 || strcmp(cmdin, "r") == 0 || strcmp(cmdin, "R") == 0) {
			//Sort each number into the tree one by one
			for (int i = 0; numbersToAdd[i] > 0; i++) {
				//Set the current pointer back to the head
				int* treeGuts = tree.flushTree();
				//for (int i = 0; treeGuts[i] != 0; i++) cout << treeGuts[i] << ", ";
				for (int j = 0; j < BIGLEN; j++) {
					if (treeGuts[j] == numbersToAdd[i]) {
						treeGuts[j] = -1;
					}
				}
				for (int j = 0; j < BIGLEN; j++) {
					if (treeGuts[j] != -1) {
						addToTree(&tree, treeGuts[j]);
					}
				}
				cout << endl;
			}
		}
		*/
	}
	
	return 0;
}

void sayProgramDescription() {
	//Greetings, and an introduction to what a tree is in case the user doesn't know
	cout << "Welcome to Gregg\'s fabulous Red-Black Tree program!" << endl <<
	"This program is a kind of number sorter which sorts the numbers given to it in a tree formation." << endl <<
	"Each number will always have a lower number branching off to the left, and a higher number branching off to the right." << endl;
}

void listCommands() {
	//Ask how the user wants to input the numbers
	//Credit to r3mainer on https://stackoverflow.com/questions/24281603/c-underline-output for showing how to do the underlines
	//For reference, the underlined letter is a shortcut for that command
	cout << "Type \"" << UNDERLINE << "a" << STOPUNDERLINE << "dd\" to add a number" << endl << 
	"Type \"" << UNDERLINE << "r" << STOPUNDERLINE << "emove\" to remove a number" << endl << 
	"Type \"" << UNDERLINE << "s" << STOPUNDERLINE << "earch\" to find out how many times a number appears within the tree" << endl << 
	"Type \"" << UNDERLINE << "p" << STOPUNDERLINE << "rint\" to print out the tree into console" << endl << 
	"Type \"a" << UNDERLINE << "b" << STOPUNDERLINE << "out\" for a short description on what this program does" << endl <<
	"Type \"help\" or \"" << UNDERLINE << "?" << STOPUNDERLINE << "\" for help" << endl <<
	"Type \"" << UNDERLINE << "e" << STOPUNDERLINE << "xit\" to kill the program" << endl;
}

void search(GTree* tree, int numberToSearchFor) {
	cout << tree->numberCount(numberToSearchFor) << " instances of the number \"" << numberToSearchFor << "\" found." << endl << endl;
	return;
}

int searchFunction() {
	char* inputString = new char[BIGLEN];
	
	cout << "Reading from console..." << endl <<
	"Enter the number you would like to search for." << endl;
	
	//Read in the input from console and put it into inputString
	cout << "Search> ";
	cin.getline(inputString, BIGLEN);
	
	//Parse the char* input into an int* with each number separated
	int* numberToSearchFor = parseZTCString(inputString, 32);
	delete[] inputString; //We don't need this anymore.

	//At this point, we have our number that we want to search for. We only want the first one. The rest of them must be incinerated.
	int out = numberToSearchFor[0];
	delete[] numberToSearchFor; //INCINERATE ALL OF THE UNNECESSARY NUMBERS! MWAHAHAHA!!!!
	return out;
}

bool confirmInput() {
	char confirm;
	bool haveInput = false;
	
	cout << "Confirm Input? (y/n)" << endl;
	cout << "> ";
	cin >> confirm; cin.clear(); cin.ignore(LEN, '\n');
		
	if (confirm == 'y') haveInput = true;
	
	return haveInput;
}

int* fileFunction() {
	char fileBuffer[LEN]; //This will contain the path to the file that the user would like to read from
	char* inputString; //This will contain the raw cstring of text from the file, if the user gives a valid file
	FILE* fin = NULL; //Credit to my dad for showing me the FILE libraries from ye olde C
	
	cout << "Reading from console..." << endl <<
	"Input the path to your file. It must be a text (.txt) file:" << endl;
	
	//Don't leave the while loop until the person puts in a real file with valid numbers
	while (true) {
		//Get address
		cout << "File> ";
		cin.getline(fileBuffer, LEN);
		
		//Get file from address
		fin = fopen(fileBuffer, "r"); //"r" for read
		
		if (fin != NULL) {
			//Now allocate the numList data
			inputString = new char[BIGLEN];
			cout << "Reading from file..." << endl;
			
			//Put the contents of the file into numList
			fgets(inputString, BIGLEN, fin);
			
			//Parse the char* input into an int* with each number separated
			int* numbersToAdd = parseZTCString(inputString, 32);
			delete[] inputString;
			
			//At this point, we have our input (inputString).
			cout << "Input: ";
			for (int i = 0; numbersToAdd[i] != 0; i++) {
				cout << numbersToAdd[i] << " "; //Print all int-ified inputs
			}
			cout << endl;
			
			return numbersToAdd;
		}
		else {
			cout << "There was an issue opening the file at \"" << fileBuffer << "\". (Psst! Did you forget to include the extension at the end?) Please try again..." << endl;
		}
	}
	fclose(fin);
	delete fin;
}

int* addFunction() {
	char* inputString = new char[BIGLEN];
	
	cout << "Reading from console..." << endl <<
	"Enter the number you would like to add. If you have multiple, separate them with spaces." << endl;
	
	//Read in the input from console and put it into inputString
	cout << "Add> ";
	cin.getline(inputString, BIGLEN);
	
	//Parse the char* input into an int* with each number separated
	int* numbersToAdd = parseZTCString(inputString, 32);
	delete[] inputString;

	//At this point, we have our input (inputString).
	cout << "Input: ";
	for (int i = 0; numbersToAdd[i] != 0; i++) {
		cout << numbersToAdd[i] << " "; //Print all int-ified inputs
	}
	cout << endl;
	
	return numbersToAdd;
}

/*
int* removeFunction() {
	char* inputString = new char[BIGLEN];
	
	cout << "Reading from console..." << endl <<
	"Enter the number you would like to remove. If you have multiple, separate them with spaces." << endl;
	
	//Read in the input from console and put it into inputString
	cout << "Remove> ";
	cin.getline(inputString, BIGLEN);

	//Parse the char* input into an int* with each number separated
	int* numbersToRemove = parseZTCString(inputString, 32);

	//At this point, we have our input (inputString).
	cout << "Input: ";
	for (int i = 0; numbersToRemove[i] != 0; i++) {
		cout << numbersToRemove[i] << " "; //Print all int-ified inputs
	}
		cout << endl;
	
	return numbersToRemove;
}
*/

/*
GTree maxTreeSort(int* input) {
	
	GTree tree();
	//For future reference, the "boss parent" is the very first node; the one which all others lead back to
	//
	//For every token in input...
	for (int i = 0; input[i] != 0; i++) { //i is iterator for input
		for (int j = 0; j < LEN; j++) { //j is iterator for tree
			//... and go through the tree from left to right until a token of lower magnitude than the current number from input is found.
			//At which point replace the new token with the old, then recursively sort down the tree from that point.
			if (input[i] >= tree.get(j)) {
				//Sort the new token and all of its children
				insertToken(&tree, j, input[i]);
				break;
			}
		}
	}
	
	return tree;
}
*/
