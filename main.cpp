#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

const int MIN = 0;
const int MAX = 100;

HANDLE create_file();
DWORD write_file(HANDLE, string);
HANDLE new_process(int);
void random_numbers_generator();
void random_numbers(int*, int, int, int);
int single_random_number(int, int);
DWORD print_result(HANDLE, int*, int);
HANDLE create_result(int, int, int);

int main(int argc, char* argv[])
{
    if(argc == 1) {

		cout << "Program needs arguments" << endl;
	} else {
		
		LARGE_INTEGER liDueTime;
        int quantity = atoi(argv[1]);
		int seconds = atoi(argv[2]);
		int counter = atoi(argv[3]);
        int* randomIntArray = new int[quantity];
        
        liDueTime.QuadPart = -(seconds * 10000000);
		HANDLE Timer = NULL;
		HANDLE result = create_result(quantity, counter, seconds);
		CloseHandle(result);
		
		int i = 0, frequency = 1;
        
        do {
	        random_numbers_generator();
			random_numbers(randomIntArray, quantity, MIN, MAX);
	        
			HANDLE out = create_file();
			
			if (out == INVALID_HANDLE_VALUE) {
	        	cout << "CreateFile error " << GetLastError() << "." << endl;
	            getchar();
	            return 1;
			}
			
			Timer = CreateWaitableTimer(NULL, TRUE, NULL);
			
			if(Timer == NULL) {
				cout << "CreateWaitableTimer failed " << GetLastError() << endl;
        		return 1;
			}
 			
    		cout << "\nWaiting for " << seconds << " seconds." << endl;	
    		
			if(!SetWaitableTimer(Timer, &liDueTime, 0, NULL, NULL, 0)) {
				cout << "SetWaitableTimer failed " << GetLastError() << endl;
				return 1;
    		}
    	
    		if(WaitForSingleObject(Timer, INFINITE) != WAIT_OBJECT_0) {
				cout << "WaitForSingleObject failed " << GetLastError() << endl;
			} else {
				cout << "\nTimer was signaled.\n" << endl;
			}
			
			HANDLE second_process = new_process(frequency);
			
			print_result(out, randomIntArray, quantity);
			
			CloseHandle(out);
			
			WaitForSingleObject(second_process, INFINITE);
			
			CloseHandle(second_process);
	
			i++;
			frequency++;
		} while(!(WaitForSingleObject(Timer, INFINITE) != WAIT_OBJECT_0) && (i != counter));
			
		free(randomIntArray);
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

HANDLE new_process(int frequency) {
	string one = "program.exe ";
	string two = to_string(frequency);
	string finally = one + two;
	const char * data = finally.c_str();
	
	STARTUPINFO startupinfo;
    PROCESS_INFORMATION process_information;

    ZeroMemory(&startupinfo, sizeof(startupinfo) );
    startupinfo.cb = sizeof(startupinfo);
    ZeroMemory(&process_information, sizeof(process_information));
    
	/**
	*	Creating a separate process
	**/
    if(!CreateProcess("program.exe", (char*)data, NULL, NULL, FALSE, 0, NULL, NULL, &startupinfo, &process_information)) {
    	
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

HANDLE create_result(int quantity, int replies, int pause) {
	
	HANDLE file = CreateFile(TEXT("result.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(file == INVALID_HANDLE_VALUE) {
        printf("CreateFile error %d.\n", GetLastError());
        getchar();
	} 
		
	_SYSTEM_INFO LPSYSTEM_INFO;
	GetSystemInfo(&LPSYSTEM_INFO);
	
	string tabulator = "\t ";
	string enter = "\r\n";
	string toWrite = "Total quantity: "+to_string(quantity)+" | Version: Multithread | Machine: "+to_string(LPSYSTEM_INFO.dwProcessorType)
		+enter+"Replies: "+to_string(replies)+tabulator+" | Pause: "+to_string(pause)+" sec"+enter+enter+"Nr."+tabulator+"Avg"+tabulator+tabulator+
		"Min"+tabulator+"Max"+enter;
		
	DWORD bytesWritten = 0;
	bool isWrittenSuccees = WriteFile(file, &toWrite[0], sizeof(char)*toWrite.length(), &bytesWritten, NULL);
										
	if(!isWrittenSuccees)	cout << "There was not posibility to write to file." << endl;
	
	return file;
}

