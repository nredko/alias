/***************************************************************************************
Alias is a little tool which allows to run programs from "Ubuntu for Windows 10" in default windows command line.

For example, to run git in CMD:

1. Install git in Ubuntu subsystem
	$ sudo apt-get install git
2. Copy or link alias.exe to any folder in PATH with name git.exe
	> copy alias.exe c:\bin\git.exe
3. Copy bash.exe to the same folder
	> copy c:\windows\system32\bash.exe c:\bin\
4. Run git.exe with required arguments. It will execute command bash -c 'git %ARGS%'
The following commands are the same:
    > bash -c 'git status'
    > git status

 (c) 2016, nredko
***************************************************************************************/



#include <SDKDDKVer.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |	FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US), (LPTSTR)&lpMsgBuf, 0, NULL);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"),	lpszFunction, dw, lpMsgBuf);
	//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
	_tprintf((LPCTSTR)lpDisplayBuf);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}


void _tmain(int argc, TCHAR *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	LPTSTR cmdl = GetCommandLine();
	while (*cmdl != ' ') cmdl++; //command line without program name

	LPTSTR lpBuf = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)cmdl) + 40) * sizeof(TCHAR));
	LPTSTR command = argv[0] + lstrlen(argv[0]) - 1;
	while (*command != '\\') command--;
	command++; //command is file name without extension
	LPTSTR lpCmd = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, lstrlen((LPCTSTR)command) * sizeof(TCHAR));
	TCHAR* ptr; 
	TCHAR* ptr1 = lpCmd;
	for (ptr = command; *ptr != '.'; *ptr1++ = *ptr++);
	*ptr = 0;
	StringCchPrintf((LPTSTR)lpBuf,	LocalSize(lpBuf) / sizeof(TCHAR), TEXT("bash -c \"%s%s\""),	lpCmd, cmdl);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcess(
		NULL,			// No module name (use command line)
		lpBuf,          // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		ErrorExit(_T("CreateProcess"));
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exit_code;
	if (FALSE == GetExitCodeProcess(pi.hProcess, &exit_code))
	{
		ErrorExit(_T("GetExitCodeProcess"));
	}

	LocalFree(lpBuf);
	LocalFree(lpCmd);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	ExitProcess(exit_code);
}