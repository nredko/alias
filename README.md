# alias
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

