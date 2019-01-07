#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

HANDLE Create_File();
DWORD Write_File(HANDLE, int, int);

int main(int argc, char* argv[])
{
    if(argc == 1) {

		cout << "Program needs arguments" << endl;
	} else {

        int first = atoi(argv[1]);
		int second = atoi(argv[2]);	
		HANDLE out = Create_File();
		
		if (out == INVALID_HANDLE_VALUE) {
        	cout << "CreateFile error " << GetLastError() << "." << endl;
            getchar();
            return 1;
		}
		
		DWORD count = Write_File(out, first, second);
		cout << "There is " << count << " bytes." << endl;
	}

	return 0;
}

HANDLE Create_File() {
	
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

DWORD Write_File(HANDLE file, int first, int second) {
	
	string tab = "\t ";
	string toWrite = "First argument: " +
					to_string(first) +
					tab +
					"Second argument: " +
					to_string(second);
					
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

