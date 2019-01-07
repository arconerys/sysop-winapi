#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc == 1) {

		cout << "Program needs arguments" << endl;
	} else {

        int first = atoi(argv[1]);
		int second = atoi(argv[2]);
		int third = atoi(argv[3]);

		cout << "First argument is " << first << endl;
		cout << "Second argument is " << second << endl;
		cout << "Third argument is " << third << endl;
	}

	return 0;
}

