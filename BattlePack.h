//BattlePack.h
/*
Hi. I'm Greggory Hickman, and these are some helpful methods to use in your C++ code.
*/

#include <cstring>
#include <math.h>

#define LEN 100
#define BIGLEN 3000

/*
Clears all data from a cstring. Can be used to empty out a previously used cstring, or to clear any random RAM still present inside.
@author Greggory Hickman, February 2020
*/
void clearCString(char*& in, int len) {
	for (int i = 0; i < len; i++) {
		in[i] = (char)0;
	}
}

/*
Clears all data from an int array. Can be used to empty out a previously used array, or to clear any random RAM still present inside.
@author Greggory Hickman, February 2020
*/
void clearIntArray(int*& in, int len) {
	for (int i = 0; i < len; i++) {
		in[i] = 0;
	}
}

/*
Converts a character terminated cstring of numbers to an array of intergers. Ignores things other than numbers, making them safe to input.
The character that separates each number can be specified. For example, for a traditional zero terminated string, separator should be '0' or (char)48.
This method uses the clearCString method which can be seen above. Please keep that in mind when using this code.
@author Greggory Hickman, February 2020
*/
int* parseZTCString(char* charList, char separator) {
	char* current = new char[LEN];
	int* numList = new int[LEN];
	
	//Clear pesky leftover ram from pointer arrays
	clearCString(current, LEN);
	clearIntArray(numList, LEN);
	
	//cout << "Starting charList: " << charList << endl;
	//cout << "Starting current: " << current << endl;
	
	//Parse the input
	int j = 0; //Iterator for current
	int k = 0; //Iterator for numList
	for (int i = 0; i < BIGLEN + 1; i++) {
		//If we have hit the 100 number max limit
		if (k >= LEN - 1) {
			break;
		}
		//If is number
		if (charList[i] >= 48 && charList[i] <= 57) {
			//Add the digit to the end of the current multi-digit number
			current[j] = charList[i];
			j++;
		}
		//If is space
		else if (charList[i] == separator) {
			if (j != 0) {
				//cout << "Space: " << current << endl;
				//current contains the number that we want to convert to an integer
				int next = 0;
				
				//Iterate backwards through current
				for (int l = j - 1; l >= 0; l--) {
					//Convert the char* to int one unit place at a time by adding each value * 10^place. For example, when in the hundred's place, place = 2.
					next += (current[l] - 48) * round(pow(10, j - l - 1));
				}
				//cout << "Next: " << next << endl;
				numList[k] = next;
				j = 0;
				k++;
				
				clearCString(current, LEN);
			}
		}
		//If is null
		else if (charList[i] == 0) {
			if (j != 0) {
				//cout << "Null: " << current << endl;
				//current contains the number that we want to convert to an integer
				int next = 0;
				
				//Iterate backwards through current
				for (int l = j - 1; l >= 0; l--) {
					//Convert the char* to int one unit place at a time by adding each value * 10^place. For example, when in the hundred's place, place = 2.
					next += (current[l] - 48) * round(pow(10, j - l - 1));
				}
				numList[k] = next;
				break;
			}
		}
	}
	delete(current);
	
	return numList;
}