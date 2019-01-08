#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <algorithm>

#define MAX_THREADS 4

using namespace std;

HANDLE create_file();
DWORD read_file(HANDLE, char[], int);
DWORD WINAPI AllOperation(LPVOID);
DWORD WINAPI Min(LPVOID);
DWORD WINAPI Max(LPVOID);
DWORD WINAPI Average(LPVOID);

int main(int argc, char* argv[])
{
	HANDLE out;
	
	do {
		cout << "SPINLOCK" << endl;
		out = create_file();
	} while(out == INVALID_HANDLE_VALUE);
	
	cout << "File has been loaded." << endl;
	
	DWORD byteSize = GetFileSize(out, NULL);
	
	int size = byteSize/sizeof(char);
	char toRead[size-1] {0};
	
	DWORD bytesRead = read_file(out, &toRead[0], byteSize);
	
	cout << "Loaded " << bytesRead << " bytes from file." << endl << endl;
	
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

	HANDLE ThreadArray[MAX_THREADS]; 
	DWORD ID[MAX_THREADS];
	ThreadArray[0] = CreateThread(NULL, 0, Max, &myNumbers, CREATE_SUSPENDED, &ID[0]);
	ThreadArray[1] = CreateThread(NULL, 0, Min, &myNumbers, CREATE_SUSPENDED, &ID[1]);
	ThreadArray[2] = CreateThread(NULL, 0, Average, &myNumbers, CREATE_SUSPENDED, &ID[2]);
	ThreadArray[3] = CreateThread(NULL, 0, AllOperation, &myNumbers, CREATE_SUSPENDED, &ID[3]);
	
	for(int i = 1; i <= MAX_THREADS; i++) {
		ResumeThread(ThreadArray[i-1]);
		WaitForSingleObject(ThreadArray[i-1], INFINITE);
		CloseHandle(ThreadArray[i-1]);
		cout << "\nThe thread #" << i << " has been closed successfully." << endl;
	}

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

DWORD WINAPI AllOperation(LPVOID value) {
	
	vector<int> myNumbers = *((vector<int>*)value);
	vector<int>::iterator it1 = max_element(myNumbers.begin(), myNumbers.end());
	vector<int>::iterator it2 = min_element(myNumbers.begin(), myNumbers.end());
	
	int size = myNumbers.size();
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += myNumbers.at(i);
	}
	
	float result = sum/size;

	cout << "\n\nThe average value is: " << result << endl;
	cout << "Maximum value is: " << *it1 << endl;
	cout << "Minimum value is: " << *it2 << endl;
	
	return 0;
}

DWORD WINAPI Max(LPVOID value) {
	vector<int> myNumbers = *((vector<int>*)value);
	vector<int>::iterator it = max_element(myNumbers.begin(), myNumbers.end());
	cout << "\nMaximum value is: " << *it << endl;	
	
	return 0;
}

DWORD WINAPI Min(LPVOID value ) {
	vector<int> myNumbers = *((vector<int>*)value);
	vector<int>::iterator it = min_element(myNumbers.begin(), myNumbers.end());
	cout << "\nMinimum value is: " << *it << endl;
	
	return 0;
}

DWORD WINAPI Average(LPVOID value ) {
	vector<int> myNumbers = *((vector<int>*)value);
	int size = myNumbers.size();
	float sum = 0;
	for (int i=0; i < size; i++) {
		sum += myNumbers.at(i);
	}
	
	float result = sum/size;

	cout << "\nThe average value is: " << result << endl;
	
	return 0;
}

