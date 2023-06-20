#include <iostream>
#include <windows.h>
#include <iomanip>
//using namespace std;

FARPROC createprocessaddress = NULL;
SIZE_T bytesWritten = 0;
char createprocessoriginalbytes[6] = {};



int hookedCreateprocess() {
	STARTUPINFOA cifH;
	ZeroMemory(&cifH, sizeof(STARTUPINFO));
	PROCESS_INFORMATION piH;

	std::cout << "We are now in foreign function" << std::endl;
	std::cin.ignore();

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)createprocessaddress, createprocessoriginalbytes, sizeof(createprocessoriginalbytes), &bytesWritten); //unhook function back to execute createprocess



	return CreateProcessA("C:\\Windows\\System32\\calc.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &cifH, &piH);
	std::cout << "Notepad is called but foreign function is executed as calc.exe" << std::endl;
	std::cin.ignore();
}



int main()
{
	STARTUPINFOA cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;



	CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi);

	
	std::cout << "Original process notepad.exe created" << std::endl;
	std::cin.ignore();

	HINSTANCE library = LoadLibraryA("Kernel32.dll");
	SIZE_T bytesRead = 0;
	createprocessaddress = GetProcAddress(library, "CreateProcessA");

	std::cout << "CreateProcessA instance loaded by addr: " << createprocessaddress << std::endl;
	std::cin.ignore();

	ReadProcessMemory(GetCurrentProcess(), createprocessaddress, createprocessoriginalbytes, 6, &bytesRead);

	std::cout << "Content of function is placed to variable, its content = " << createprocessoriginalbytes << std::endl;
	std::cin.ignore();

	void* hookedCreateProcessAddress = &hookedCreateprocess;  //pointer where createprocess function addres is stored


	char patch[6] = { 0 };
	memcpy_s(patch, 1, "\x68", 1); //call function byte. x68 means call somethingcalled with given address of func.
	memcpy_s(patch + 1, 4, &hookedCreateProcessAddress, 4);
	memcpy_s(patch + 5, 1, "\xC3", 1);  //ret function byte.

	std::cout << "Patch content with foreign content if placed to variable = " << patch << std::endl;
	std::cin.ignore();

	//std::cout << createprocessaddress << " - Address of original function CreateProcessA" << std::endl;
	//std::cout << createprocessoriginalbytes << " - buffer with original CreateProcessA function content" << std::endl;
	//std::cout << hookedCreateProcessAddress << " - address of function to replace original" << std::endl;
	//std::cout << patch << " - new function content" << std::endl;

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)createprocessaddress, patch, sizeof(patch), &bytesWritten);

	std::cout << "Original function is rewrited with foreign" << std::endl;
	std::cin.ignore();

	CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi);

	std::cout << "Call CreateProcess again" << std::endl;
	std::cin.ignore();


	STARTUPINFOA cif1;
	ZeroMemory(&cif1, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi1;
	CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &cif1, &pi1);



	TerminateProcess(pi.hProcess, NO_ERROR);
}
