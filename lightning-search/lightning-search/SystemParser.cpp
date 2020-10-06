#include "SystemParser.h"

struct ScanDriveData {
    SystemParser* systemParser;
    std::wstring drive;
};

DWORD WINAPI scanDrive(void* param) {
    ScanDriveData* scanDriveData = (ScanDriveData*)param;
    SystemParser* systemParser = scanDriveData->systemParser;
    std::wstring drive = scanDriveData->drive;
    std::wcout << L"Scanning drive " << drive << L"\n";
    systemParser->FindFile(drive);
    std::wcout << L"Done scanning drive " << drive << L"\n";
    return 0;
}

DWORD WINAPI runSystemParser(void* param) {
    SystemParser* systemParser = (SystemParser*)param;
    std::cout << "Finding Steam Games\n";
    systemParser->indexFiles();
    return 0;
}

void SystemParser::run() {
    CreateThread(NULL, 0, runSystemParser, this, 0, NULL);
}

void SystemParser::indexFiles() {
    std::vector<std::wstring> drives({ L"A:", L"B:", L"C:", L"D:", L"E:", L"F:", L"G:", L"H:", L"I:", L"J:", L"K:", L"L:", L"M:", L"N:", L"O:", L"P:", L"Q:", L"R:", L"S:", L"T:", L"U:", L"V:", L"W:", L"X:", L"Y:", L"Z:" });
    // std::vector<std::wstring> drives({ L"A:", L"B:", L"C:", L"D:", L"E:", L"F:" }); // For testing
    const int MAX_THREADS = 26;
    int actual_threads = drives.size();
    DWORD dwThreadArray[MAX_THREADS];
    HANDLE hThreadArray[MAX_THREADS];
    ScanDriveData driveData[MAX_THREADS];
    for (int i = 0; i < drives.size(); i++) {
        driveData[i].drive = drives[i];
        driveData[i].systemParser = this;
        hThreadArray[i] = CreateThread(NULL, 0, scanDrive, &driveData[i], 0, &dwThreadArray[i]);
    }
    // Wait for game scanning threads to return
    DWORD ret = WaitForMultipleObjects(actual_threads, hThreadArray, TRUE, INFINITE);
    std::cout << GetLastError() << "\n";
    std::cout << "WaitForMultipleObjects response: " << ret << "\n";
}

void SystemParser::FindFile(const std::wstring& directory) {
    std::wstring tmp = directory + L"\\*";
    WIN32_FIND_DATA file;
    HANDLE search_handle = FindFirstFile(tmp.c_str(), &file);
    if (search_handle != INVALID_HANDLE_VALUE)
    {
        std::vector<std::wstring> directories;

        do
        {
            std::wstring filename(file.cFileName);
            // TODO: Index files
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (filename.compare(L".") != 0 && filename.compare(L"..") != 0 && filename.find(L"$") == std::wstring::npos) {
                    tmp = directory + L"\\" + std::wstring(file.cFileName);
                    directories.push_back(tmp);
                }

            }
        } while (FindNextFileW(search_handle, &file));

        FindClose(search_handle);

        for (std::vector<std::wstring>::iterator iter = directories.begin(), end = directories.end(); iter != end; ++iter) {
            FindFile(*iter);
        }
    }
}