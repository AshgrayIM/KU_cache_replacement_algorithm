#include "CacheAlg.h"
#include "string"
#include <ctime>
#include <cstdlib>

void main() {

	

	while (true) {

		cout << ">> y/n: ";
		string str;
		cin >> str;
		if (str == "n")
			return;
		else
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		CacheAlg c;
		c.play();
	}
}