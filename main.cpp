#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

const int MIN = 0;
const int MAX = 100;

HANDLE create_file();
DWORD write_file(HANDLE, string);
HANDLE new_process();
void random_numbers_generator();
void random_numbers(int*, int, int, int);
int single_random_number(int, int);
DWORD print_result(HANDLE, int*, int);

int main(int argc, char* argv[])
{
    if(argc == 1) {

		cout << "Program needs arguments" << endl;
	} else {

        int quantity = atoi(argv[1]);
        int* randomIntArray = new int[quantity];
        
        random_numbers_generator();
		random_numbers(randomIntArray, quantity, MIN, MAX);
        
		HANDLE out = create_file();
		
		if (out == INVALID_HANDLE_VALUE) {
        	cout << "CreateFile error " << GetLastError() << "." << endl;
            getchar();
            return 1;
		}
		
		HANDLE second_process = new_process();
		
		print_result(out, randomIntArray, quantity);
		
		CloseHandle(out);
		
		WaitForSingleObject(second_process, INFINITE);
		
		CloseHandle(second_process);
	}

	return 0;
}

HANDLE create_file() {
	
    HANDLE file = CreateFile(
		TEXT("out.txt"), 
		GENERIC_WRITE,
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);
    
    return file;
}

DWORD write_file(HANDLE file, string toWrite) {
	
	DWORD bytesWritten = 0;
	
	bool isWrittenSuccees = WriteFile(
							file, 
							&toWrite[0], 
							sizeof(char)*toWrite.length(), 
							&bytesWritten, 
							NULL);

	if(!isWrittenSuccees)	cout << "There was not posibility to write to file." << endl;
	
	return bytesWritten;
}

void random_numbers_generator() {
	srand(time(NULL));
}

void random_numbers(int *randomIntArray, int cnt, int MIN, int MAX) {
	
	for(int i = 0; i < cnt; i++) {
		randomIntArray[i] = single_random_number(MIN, MAX);
	}
}

int single_random_number(int MIN, int MAX) {
	
	int randomInt;
	randomInt = rand() % MAX + MIN;
	return randomInt;
}

HANDLE new_process() {
	
	STARTUPINFO startupinfo;
    PROCESS_INFORMATION process_information;

    ZeroMemory(&startupinfo, sizeof(startupinfo) );
    startupinfo.cb = sizeof(startupinfo);
    ZeroMemory(&process_information, sizeof(process_information));
    
	/**
	*	Creating a separate process
	**/
    if(!CreateProcess("program.exe", NULL, NULL, NULL, FALSE,0, NULL, NULL, &startupinfo, &process_information)) {
    	
        printf("CreateProcess failed (%d).\n", GetLastError());
	}

    return process_information.hProcess;
}

DWORD print_result(HANDLE hFile, int* array, int quantity) {
	
	string toWrite;
	
	for(int i = 0; i < quantity; i++) {
		
		toWrite += to_string(array[i]);
		if(i != quantity - 1) {
			toWrite += " \t";
		}
	}

	DWORD bytesWritten = write_file(hFile, toWrite);
	
	return bytesWritten;
}

