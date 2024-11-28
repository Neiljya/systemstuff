#include<Windows.h>
#include<iostream>
#include <string>
#include <algorithm>

using namespace std;

/**

This is a basic console app that sorts files by extension
given a source and target directory (target directory will be created if none specified)

Ex: .pdf, .png, .txt, etc

*/


// Method to remove whitespaces
string trim(const string str) {
	const string whitespace = " \t\n\r";
	size_t start = str.find_first_not_of(whitespace);

	if (start == string::npos)
		return ""; // empty
	
	size_t end = str.find_last_not_of(whitespace);
	return str.substr(start, end - start + 1);

}

// Method to get the file extension in lowercase removing the '.'
string getExtension(const string filename) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos == string::npos || dotPos == filename.length() - 1)
		return "no extension found";
	
	string ext = filename.substr(dotPos + 1);

	// convert to lowercase
	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	return ext;

}

int main() {
	string sourceDir;
	string targetDir;

	cout << "Enter the source directory path: ";
	getline(cin, sourceDir);
	sourceDir = trim(sourceDir);
	
	if (sourceDir.empty()) {
		cout << "Source directory cannot be empty.\n";
		return 1;
	}
	
	// add '\\' if not included already
	if (sourceDir.back() != '\\') {
		sourceDir += "\\";
	}	

	cout << "Enter the target directory path (leave blank to create a default folder)";
	
	// store input as targetDir
	getline(cin, targetDir);

	// remove any trailing whitespaces
	targetDir = trim(targetDir);

	// create default directory if no target is specified
	if (targetDir.empty()) {
		targetDir = sourceDir + "Sorted\\";
	}
	else {
	
		if (targetDir.back() != '\\') {
			targetDir += '\\';
		
		}
	}


	DWORD ftyp = GetFileAttributesA(sourceDir.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
		cout << "Source directory does not exist or is not a valid directory\n";
		return 1;
	
	}

	if (CreateDirectoryA(targetDir.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
	}
	else {
		cout << "Failed to create target directory.\n";
		return 1;
	}

	// Prepare search pattern
	string searchPattern = sourceDir + "*.*";

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "No files found in source directory";
		return 0;
	}

	do {

		// Skip directories
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}

		string fileName = findData.cFileName;
		string extension = getExtension(fileName);

		// Creates extension in target folder
		string extensionDir = targetDir + extension + "\\";
		if (!CreateDirectoryA(extensionDir.c_str(), NULL)) {
			if (GetLastError() != ERROR_ALREADY_EXISTS) {
				cout << "Failed to create directory: " << extensionDir << "\n";
				continue;
			
			}
		
		}

		string sourceFilePath = sourceDir + fileName;
		string targetFilePath = extensionDir + fileName;

		if (!MoveFileA(sourceFilePath.c_str(), targetFilePath.c_str())) {
			cout << "Failed to move file: " << fileName << "\n";
		}
		else {
			cout << "Successfully Moved: " << fileName << " -> " << extension << "\\\n";
		}
	} while (FindNextFileA(hFind, &findData) != 0);

	FindClose(hFind);

	cout << "Completed Sorting\n";
	return 0;


}
