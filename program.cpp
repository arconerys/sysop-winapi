#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

using namespace std;

HANDLE create_file();
DWORD read_file(HANDLE, char[], int);

int main(int argc, char* argv[])
{
	HANDLE out;
	
	do {
		cout << "SPINLOCK" << endl;
		out = create_file();
	} while(out == INVALID_HANDLE_VALUE);
	
	cout << "File has been loaded." << endl << endl;
	
	DWORD byteSize = GetFileSize(out, NULL);
	
	int size = byteSize/sizeof(char) + 1;
	char toRead[size] {0};
	
	DWORD bytesRead = read_file(out, &toRead[0], byteSize);
	
	cout << "Loaded " << bytesRead << " bytes from file." << endl;
	
	istringstream ss(toRead);
	string number;

	vector<string> allNumbers;
	while(getline(ss, number, '\t')) {
		allNumbers.push_back(number);
	}

	vector<int> myNumbers;
	int sizeVectorString = allNumbers.size();
	for(int i = 0; i < sizeVectorString; i++){
		myNumbers.push_back(atoi((allNumbers.at(i)).c_str()));
		cout << myNumbers.at(i) << "\t";
	}

	int sizeVectorInt = myNumbers.size();
	float sum = 0;
	for(int i = 0; i < sizeVectorInt; i++) {
		sum += myNumbers.at(i);
	}

	float result = sum/sizeVectorInt;
	
	cout<<"\nThe average is: "<< result <<endl;
	
	return 0;
}

HANDLE create_file() {
	
	HANDLE file = CreateFile(
		TEXT("out.txt"), 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);
		
	return file;
}

DWORD read_file(HANDLE out, char data[], int size) {
	
	DWORD bytesRead = 0;
	ReadFile(out, &data[0], size, &bytesRead, NULL);
	
	return bytesRead;
}

