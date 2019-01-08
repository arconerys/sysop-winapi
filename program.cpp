#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <algorithm>
#define MAX_THREADS 3

using namespace std;

DWORD INVALID_SET_FILE = 0xFFFFFFFF;

typedef struct MyData {
	vector<int> myNumbers;
	float average = -1;
	int max = -1;
	int min = 100;
	
} MYDATA, *PMYDATA;

HANDLE create_file();
DWORD read_file(HANDLE, char[], int);
void update_result(HANDLE, MyData*, int);
DWORD WINAPI Min(LPVOID);
DWORD WINAPI Max(LPVOID);
DWORD WINAPI Average(LPVOID);

int main(int argc, char* argv[])
{
	int frequency = atoi(argv[1]);
	HANDLE out;
	
	do {
		cout << "SPINLOCK" << endl;
		out = create_file();
	} while(out == INVALID_HANDLE_VALUE);
	
	cout << "File has been loaded." << endl;
	
	HANDLE resultFile = CreateFile(TEXT("result.txt"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(resultFile == INVALID_HANDLE_VALUE) {
        printf("CreateFile error %d.\n", GetLastError());
        getchar();
	}
	
	DWORD Pointer = SetFilePointer(resultFile, NULL, NULL, FILE_END);
	if(Pointer == INVALID_SET_FILE) {
		cout << "SetFilePointer error " << GetLastError() << endl;
    	getchar();
		return 1;
	}
	
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
	PMYDATA StructOfDataForThread;
	
	HANDLE Heap = GetProcessHeap();
    if(Heap == NULL) {
        cout << "Heaping Heap error: " << GetLastError() << endl;
        ExitProcess(2);
    }
    
    StructOfDataForThread = (PMYDATA) HeapAlloc(Heap, HEAP_ZERO_MEMORY, sizeof(MYDATA));
    if (StructOfDataForThread == NULL)	ExitProcess(2);
    StructOfDataForThread->myNumbers = myNumbers;

	ThreadArray[0] = CreateThread(NULL, 0, Max, StructOfDataForThread, CREATE_SUSPENDED, &ID[0]);
	ThreadArray[1] = CreateThread(NULL, 0, Min, StructOfDataForThread, CREATE_SUSPENDED, &ID[1]);
	ThreadArray[2] = CreateThread(NULL, 0, Average, StructOfDataForThread, CREATE_SUSPENDED, &ID[2]);
	
	for (int i = 0; i < MAX_THREADS; i++) {
		ResumeThread(ThreadArray[i]);
		WaitForSingleObject(ThreadArray[i], INFINITE);
		CloseHandle(ThreadArray[i]);
	}
	
	update_result(resultFile, StructOfDataForThread, frequency);
	if (!(HeapFree(Heap, 0, StructOfDataForThread)))	cout << "Failed to free allocation from Heap." << endl;
	
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

DWORD WINAPI Max(LPVOID value) {

	PMYDATA DataForThread = (PMYDATA)value;
	vector<int> myNumbers = DataForThread->myNumbers;
	vector<int>::iterator it = max_element(myNumbers.begin(), myNumbers.end());

	cout << "\nMaximum value is: " << *it << endl;
	DataForThread->max = int(*it);

	return 0;
}

DWORD WINAPI Min(LPVOID value) {

	PMYDATA DataForThread = (PMYDATA)value;
	vector<int> myNumbers = DataForThread->myNumbers;
	vector<int>::iterator it = min_element(myNumbers.begin(), myNumbers.end());

	cout << "Minimum value is: " << *it << endl;
	DataForThread->min = int(*it);
	return 0;
}

DWORD WINAPI Average(LPVOID value) {

	PMYDATA DataForThread = (PMYDATA)value;
	vector<int> myNumbers = DataForThread->myNumbers;
	
	int size = myNumbers.size();
	float sum = 0;
	for (int i=0; i < size; i++) {
		sum += myNumbers.at(i);
	}
	
	float result = sum/size;

	cout << "The average is: " << result << endl;
	DataForThread->average = result;
	return 0;
}

void update_result(HANDLE file, MyData* data, int frequency) {
	string tabulator = "\t ";
	string enter = "\r\n";
	string toWrite = to_string(frequency)+tabulator+to_string(data->average)+tabulator+to_string(data->min)+tabulator+to_string(data->max)+enter;
	
	DWORD bytesWritten = 0;
	bool isWrittenSuccees = WriteFile(file, &toWrite[0], sizeof(char)*toWrite.length(), &bytesWritten, NULL);
										
	if(!isWrittenSuccees)	cout << "There was not posibility to write to file." << endl;
}
