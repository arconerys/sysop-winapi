#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

HANDLE create_file();
DWORD read_file(HANDLE, char[], int);
DWORD WINAPI AllOperation(LPVOID);

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

	DWORD ID;
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	HANDLE thread = CreateThread(NULL, 0, AllOperation, &myNumbers, 0, &ID);
	QueryPerformanceFrequency(&Frequency); 
	QueryPerformanceCounter(&StartingTime);
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	cout << "The thread was made in " << ElapsedMicroseconds.QuadPart << "ms." << endl;
	cout << "\nThe thread has been closed successfully." << endl;

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

DWORD WINAPI AllOperation(LPVOID value){
	
	vector<int> myNumbers = *((vector<int>*)value);
	vector<int>::iterator it1 = max_element(myNumbers.begin(), myNumbers.end());
	vector<int>::iterator it2 = min_element(myNumbers.begin(), myNumbers.end());
	
	int size = myNumbers.size();
	float sum = 0;
	for (int i=0; i < size; i++) {
		sum += myNumbers.at(i);
	}
	
	float result = sum/size;

	cout << "\n\nThe average is: " << result << endl;
	cout << "Maximum value is: " << *it1 << endl;
	cout << "Minimum value is: " << *it2 << endl;
	
	return 0;
}

