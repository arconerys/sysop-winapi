#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

HANDLE Create_File(const char*);
DWORD Read_File(HANDLE, char[], int);

int main(int argc, char* argv[])
{
	if(argc == 1) {

		cout << "Program needs arguments" << endl;
	} else {
		
		const char * FileName = argv[1];
		HANDLE out = Create_File(FileName);

		if (out == INVALID_HANDLE_VALUE) {
        	cout << "CreateFile error " << GetLastError() << "." << endl;
            getchar();
            return 1;
		}

		DWORD byteSize = GetFileSize(out, NULL);
		cout << "Loaded file contains: " << byteSize << " bytes." << endl;
		
		int size = byteSize/sizeof(char) + 1;
		char toRead[size];
		DWORD count = Read_File(out, &toRead[0], byteSize);
		cout << "Loaded " << count << " bytes from file." << endl;
		cout << "Data in file: " << endl << endl;
		cout << toRead << endl;
	}

	return 0;
}

HANDLE Create_File(const char* FileName) {
	
	HANDLE file = CreateFile(
		FileName, 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);
		
	return file;
}

DWORD Read_File(HANDLE out, char data[], int size) {
	
	DWORD bytesRead = 0;
	ReadFile(out, &data[0], size, &bytesRead, NULL);
	
	return bytesRead;
}

