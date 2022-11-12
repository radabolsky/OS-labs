#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

vector<LPCSTR> getDrives();

vector<LPCSTR> getDrives() {
    DWORD size = GetLogicalDriveStrings(0, NULL);
    char* drive_strings = new char [size];
    GetLogicalDriveStrings(size, drive_strings);
    vector <LPCSTR> result;
    result.push_back(drive_strings);
    for (int i=0; i < size - 2; ++i){
        if (drive_strings[i] == 0) {
            result.push_back(&drive_strings[i + 1]);
        }
    }
    return result;
}

DWORD printError(){
    cout << "ERROR!" << endl;
    return GetLastError();
}

void getDriveInfo(LPSTR& disk){
    string fsTypes[7] = {"unknown type",
                       "no root dir",
                       "removable drive",
                       "HDD",
                       "remote drive",
                       "CD/DVD",
                       "RAM"};
    char driveName[32], fileSystem[16];
    UINT type = GetDriveType(disk);
    DWORD sectorPerCluster, bytesPerSector, freeClusters, totalClusters;
    GetVolumeInformation(disk,
                         driveName,
                         sizeof(driveName),
                         NULL,
                         NULL,
                         NULL,
                         fileSystem,
                         sizeof(fileSystem));
    GetDiskFreeSpace(disk,
                     &sectorPerCluster,
                     &bytesPerSector,
                     &freeClusters,
                     &totalClusters);
    cout << "Main disk info" << endl
         << "Drive " << disk << endl;
    cout
    << "Drive name: " << driveName << endl
    << "Disk type: " << fsTypes[type] << endl
    << "File system: " << fileSystem << endl
    << "Total space: " << totalClusters / 1024 * sectorPerCluster * bytesPerSector / (1024 * 1024) << " GB" << endl
    << "Free space: " << freeClusters / 1024 * sectorPerCluster * bytesPerSector / (1024 * 1024) << " GB" << endl;
}


void createDir(){
    char dirPath[257];
    cout << "Enter the path to the directory you want to create" << endl;
    cin.get();
    cin.getline(dirPath, sizeof(dirPath), '\n');
    bool dir = CreateDirectory(dirPath, NULL);
    if (dir) {
        cout << "Success!" << endl;
    }
    else{
        DWORD error = GetLastError();
        cout << "ERROR" << endl;
        if (error == ERROR_ALREADY_EXISTS){
            cout << "the path " << dirPath <<" already exists" << endl;
        }
        if (error == ERROR_PATH_NOT_FOUND) {
            cout << "path " << dirPath << " not found" << endl;
        }
    }
}

void removeDir(){
    char dirPath[257];
    cout << "Enter the path to the directory you want to remove" << endl;
    cin.getline(dirPath, sizeof(dirPath), '\n');
    bool dir = RemoveDirectory(dirPath);
    if (dir) {
        cout << "Success!" << endl;
    }
    else{
        DWORD error = GetLastError();
        cout << "ERROR" << endl;
        if (error == ERROR_ALREADY_EXISTS){
            cout << "the path " << dirPath <<" already exists" << endl;
        }
        if (error == ERROR_PATH_NOT_FOUND) {
            cout << "path " << dirPath << " not found" << endl;
        }
        else{
            cout << "Error code: " << error;
        }
    }
}

void newFile(){
    char filePath[257];
    cin.get();
    cout << "Enter the path to the file you want to create" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR)filePath,
                            GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            CREATE_NEW,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        cout << "ERROR" << endl
             << "Error number: " << GetLastError() << endl;
    }
    else {
        cout << "Success!" << endl;
        CloseHandle(hFile);
    }
}


void makeCopy(){
    char exFileName[257], newFileName[257];
    cout << "Enter the path to the file/directory you want to copy:" << endl;
    cin.get();
    cin.getline(exFileName, sizeof(exFileName), '\n');
    cout << "Enter the path where you want to copy this file/directory:" << endl;
    cin.getline(newFileName, sizeof(newFileName), '\n');
    bool copySuccess = CopyFile(exFileName, newFileName, TRUE);
    if (! copySuccess){
        DWORD err = printError();
        cout << "Error code: " << err;
    }
}

void makeMove(){
    char exFileName[257], newFileName[257];
    cout << "Enter the path to the file/directory you want to move:" << endl;
    cin.get();
    cin.getline(exFileName, sizeof(exFileName), '\n');
    cout << "Enter the path where you want to move this file/directory:" << endl;
    cin.getline(newFileName, sizeof(newFileName), '\n');
    bool moveSuccess = MoveFile(exFileName, newFileName);
    if (! moveSuccess){
        DWORD err = printError();
        cout << "Error code: " << err;
    }
}

void printAttrs(DWORD attributes){
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        if (attributes & FILE_ATTRIBUTE_NORMAL) {
            cout << "Attributes are not set" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_ARCHIVE) {
            cout << "Archive file or directory" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_COMPRESSED) {
            cout << "Compressed file or directory" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_DEVICE) {
            cout << "Reserved by the system" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            cout << "Directory" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_ENCRYPTED) {
            cout << "Encrypted file or directory" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_HIDDEN) {
            cout << "The file or directory is hidden" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) {
            cout << "The file or directory is not to be indexed by the content indexing service" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_OFFLINE) {
            cout << "The data of a file is not available immediately" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_READONLY) {
            cout << "Read-only" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_REPARSE_POINT) {
            cout << "A file or directory that has an associated reparse point, or a file that is a symbolic link"
                 << endl;
        }
        if (attributes & FILE_ATTRIBUTE_SPARSE_FILE) {
            cout << "A file that is a sparse file" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_SYSTEM) {
            cout << "A file or directory that the operating system uses a part of, or uses exclusively" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_TEMPORARY) {
            cout << "Temporary directory or file" << endl;
        }
        if (attributes & FILE_ATTRIBUTE_VIRTUAL) {
            cout << "This value is reserved for system use" << endl;
        }
    }
    else{
        DWORD err = printError();
        cout << "Error code = " << err << endl;
    }
}

void analyseFileAttributes(){
    char fileName[257];
    cout << "Enter the path to the file to get attributes:" << endl;
    cin.getline(fileName, sizeof(fileName), '\n');
//    _BY_HANDLE_FILE_INFORMATION fileInfo{};
//    bool val = GetFileInformationByHandle(hFile, &fileInfo);
    DWORD attributes = GetFileAttributes(fileName);

}


vector <int> inputIntArr(){
    char a[13];
    vector<int> res;
    cout << "Enter the items separated by a space" << endl;
    cin.getline(a, sizeof(a), '\n');
    for (auto i: a) {
        if (isdigit(i)) {
            res.push_back((int) i - 48);
        }
    }
    for (auto i: res){
        cout << i << " ";
    }
    cout << endl;
    return res;
}


void refactorAttributes(){
    char fileName[257];
    DWORD attr[8] = {FILE_ATTRIBUTE_ARCHIVE,
                     FILE_ATTRIBUTE_HIDDEN,
                     FILE_ATTRIBUTE_NORMAL,
                     FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
                     FILE_ATTRIBUTE_OFFLINE,
                     FILE_ATTRIBUTE_READONLY,
                     FILE_ATTRIBUTE_SYSTEM,
                     FILE_ATTRIBUTE_TEMPORARY};
    cout << "Enter the path to the file to set new attributes:" << endl;
    cin.getline(fileName, sizeof(fileName), '\n');
    cout << "Select the attributes to adds:" << endl;
    cout << "1 - Archive file or directory" << endl
         << "2 - The file or directory is hidden" << endl
         << "3 - Attributes are not set" << endl
         << "4 - The file or directory is not to be indexed by the content indexing service" << endl
         << "5 - The data of a file is not available immediately" << endl
         << "6 - Read-only" << endl
         << "7 - A file or directory that the operating system uses a part of, or uses exclusively" << endl
         << "8 - Temporary directory or file" << endl;
    vector<int> choice = inputIntArr();
    DWORD oldAttrs = FILE_ATTRIBUTE_NORMAL;
    for (auto i: choice){
        if (i > 0 && i < 8){
            if (!(oldAttrs & attr[i - 1])){
                oldAttrs |= attr[i - 1];
//                oldAttrs = oldAttrs | attr[i];
            }
        }
        else{
            printError();
            cout << "The item " << i << " is not in the menu" << endl;
            system("pause");
        }
    }
//    SetFileAttributes(fileName, oldAttrs);
    if (SetFileAttributes(fileName, oldAttrs)){
        cout << "Success!" << endl;
        cout << GetFileAttributes(fileName) << " " << oldAttrs << endl;
    }
    else {
        DWORD err = printError();
        cout << "Error number: " << err << endl;
    }
}

void getFileInfo(){
    char filePath[257];
    cout << "Enter the path to the file you want to create" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR)filePath,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        cout << "ERROR" << endl
             << "Error number: " << GetLastError() << endl;
    }
    else {
        cout << "File opened!" << endl;
    }
    _BY_HANDLE_FILE_INFORMATION fileInfo{};
    bool val = GetFileInformationByHandle(hFile, &fileInfo);
    if (val){
        cout << "File attributes: " << endl;
        printAttrs(fileInfo.dwFileAttributes);
        cout << "Volume serial number: " << fileInfo.dwVolumeSerialNumber << endl;
        cout << "File size high: " << fileInfo.nFileIndexHigh << endl
             << "File size low: " << fileInfo.nFileSizeLow;
        cout << "Number of links" << fileInfo.nNumberOfLinks << endl;
        cout << "File index high: " << fileInfo.nFileIndexHigh << endl
             << "File index low: " << fileInfo.nFileSizeLow << endl;
    }
    CloseHandle(hFile);
}

void printTime(SYSTEMTIME time){
    cout << time.wHour << ":"
         << time.wMinute << ":"
         << time.wSecond << " "
         << time.wDay << "."
         << time.wMonth << "."
         << time.wYear <<endl;
}

void getTime(){
    char filePath[257];
    cout << "Enter the path to the file you want to get time" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR)filePath,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        cout << "ERROR" << endl
             << "Error number: " << GetLastError() << endl;
    }
    else {
        cout << "File opened!" << endl;
        FILETIME creationTime;
        FILETIME accessTime;
        FILETIME writeTime;
        bool got = GetFileTime(hFile, &creationTime, &accessTime, &writeTime);
        if (got){
            SYSTEMTIME time;
            FileTimeToSystemTime(&creationTime, &time);
            cout << "Creation time - ";
            printTime(time);
            FileTimeToSystemTime(&accessTime, &time);
            cout << "Access time - ";
            printTime(time);
            FileTimeToSystemTime(&writeTime, &time);
            cout << "Write time - ";
            printTime(time);
        }
        else {
            DWORD err = printError();
            cout << "Error number: " << err;
        }
        CloseHandle(hFile);
    }
}


FILETIME inputSystime(FILETIME fTime){
    cout << "Select the parameter you want to change: " << endl;
    int change;
    int test;
    SYSTEMTIME time;
    FileTimeToSystemTime(&fTime, &time);
    cout << "0 - exit" << endl
         << "1 - year" << endl
         << "2 - month" << endl
         << "3 - day" << endl
         << "4 - hour" << endl
         << "5 - minute" << endl
         << "6 - second" << endl;
    cin >> change;
    while ((change < 0) || (change > 6)){
        cout << "Select an item from the menu!" << endl;
        cin >> change;
    }
    while (change != 0){
        switch (change) {
            case 1:
                cout << "Input year: " << endl;
                cin >> test;
                while ((test > 30827 || test < 1601)){
                    cin >> test;
                }
                time.wYear = test;
                break;
            case 2:
                cout << "Input month: " << endl;
                cin >> test;
                while ((test > 12 || test < 1)){
                    cin >> test;
                }
                time.wMonth = test;
                break;
            case 3:
                cout << "Input day: " << endl;
                while ((test > 31 || test < 1)){
                    cin >> test;
                }
                time.wDay = test;
                break;
            case 4:
                cout << "Input hour: " << endl;
                while ((test > 24 || test < 0)){
                    cin >> test;
                }
                time.wHour = test;
                break;
            case 5:
                cout << "Input minutes: " << endl;
                while ((test > 60 || test < 0)){
                    cin >> test;
                }
                time.wMinute = test;
                break;
            case 6:
                cout << "Input seconds: " << endl;
                while ((test > 60 || test < 0)){
                    cin >> test;
                }
                time.wSecond = test;
                break;
            default:
                break;
        }
        cout << "0 - exit" << endl
             << "1 - year" << endl
             << "2 - month" << endl
             << "3 - day" << endl
             << "4 - hour" << endl
             << "5 - minute" << endl
             << "6 - second" << endl;
        cin >> change;
    }
    FILETIME resTime;
    SystemTimeToFileTime(&time, &resTime);
    return resTime;
}


void setTime() {
    char filePath[257];
    cout << "Enter the path to the file you want to get time" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR) filePath,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "ERROR" << endl
             << "Error number: " << GetLastError() << endl;
    } else {
        cout << "File opened!" << endl;
        FILETIME times[3];
        int change;
        cout << "What time of the file should be changed?" << endl
             << "0 - exit" << endl
             << "1 - Creation time" << endl
             << "2 - Access time" << endl
             << "3 - Write time" << endl;
        cin >> change;
        if (change != 0 && change <= 3) {
            bool got = GetFileTime(hFile, &times[0], &times[1], &times[2]);
            if (got) {
                FILETIME  resTime = inputSystime(times[change - 1]);
//                SystemTimeToFileTime(&sysTime, &times[change - 1]);
                times[change - 1] = resTime;
                SetFileTime(hFile, &times[0], &times[1], &times[2]);
                cout << "Success!" << endl;
            }

        }
        CloseHandle(hFile);
    }
}


int getMainMenuInfo(){
    int menu;
    cout << "Select a menu item" << endl;
    cout << "0 - Exit" << endl
         << "1 - Output a list of disks" << endl
         << "2 - Output of basic information about disks" << endl
         << "3 - Create catalogs" << endl
         << "4 - Delete catalog" << endl
         << "5 - Create file" << endl
         << "6 - Copy/move file" << endl
         << "7 - File attributes" << endl;
    cin >> menu;
    return menu;
}



int main()
{
    int menu = -1;
    vector<LPCSTR> drives;
    while (menu != 0){
        system("cls");
        menu = getMainMenuInfo();
        switch(menu){
            case 1:
                system("cls");
                drives = getDrives();
                cout << "List of disks used:" << endl;
                for (auto & drive : drives){
                    cout << drive << endl;
                }
                cout << endl;
                system("pause");
                break;
            case 2:
                system("cls");
                drives = getDrives();
                cout << "Choose disk:" << endl;
                cout << "0 - Exit" << endl;
                for (int i=0; i < drives.size(); ++i){
                    cout << i + 1 << " - " << drives[i] << endl;
                }
                int driveChoice;
                cin >> driveChoice;
                while (driveChoice < 0 || driveChoice > drives.size()) {
                    if (driveChoice == 0) break;
                    printError();
                    cout << "Select an item from the menu!" << endl;
                    cin >> driveChoice;
                }
                if (driveChoice != 0){
                    getDriveInfo(const_cast<LPSTR &>(drives[driveChoice - 1]));
                }
                system("pause");
                break;
            case 3:
                system("cls");
                cout << "Create catalogs:" << endl;
                createDir();
                system("pause");
                break;
            case 4:
                system("cls");
                cout << "Remove catalogs:" << endl;
                cin.get();
                removeDir();
                system("pause");
                break;
            case 5:
                system("cls");
                cout << "Create file:" << endl;
                newFile();
                system("pause");
                break;
            case 6:
                int copyMenu;
                system("cls");
                cout << "Copy/move file:" << endl;
                cout << "What do you want to do?" << endl
                     << "0 - exit" << endl
                     << "1 - copy" << endl
                     << "2 - move" << endl;
                cin >> copyMenu;
                while (copyMenu < 0 || copyMenu > 2){
                    printError();
                    cout << "Select an item from the menu!" << endl;
                    cin >> copyMenu;
                }
                if (copyMenu != 0) {
                    if (copyMenu == 1) {
                        makeCopy();
                    } else {
                        makeMove();
                    }
                }
                system("pause");
                break;
            case 7:
                int attrMenu;
                system("cls");
                cout << "File attributes:" << endl;
                cout << "What do you want to do?" << endl
                     << "0 - exit" << endl
                     << "1 - attributes analysis" << endl
                     << "2 - set file attributes" << endl
                     << "3 - file information" << endl
                     << "4 - get file time" << endl
                     << "5 - set file time" << endl;
                cin >> attrMenu;
                while (attrMenu < 0 || attrMenu > 5){
                    printError();
                    cout << "Select an item from the menu!" << endl;
                    cin >> attrMenu;
                }
                if (attrMenu != 0){
                    switch(attrMenu){
                        case 1:
                            system("cls");
                            cout << "Attributes analyses" << endl;
                            cin.get();
                            analyseFileAttributes();
                            system("pause");
                            break;
                        case 2:
                            system("cls");
                            cout << "Set file attributes" << endl;
                            cin.get();
                            refactorAttributes();
                            system("pause");
                            break;
                        case 3:
                            system("cls");
                            cout << "File information" << endl;
                            cin.get();
                            getFileInfo();
                            system("pause");
                            break;
                        case 4:
                            system("cls");
                            cout << "File time" << endl;
                            cin.get();
                            getTime();
                            system("pause");
                            break;
                        case 5:
                            system("cls");
                            cout << "Set file time" << endl;
                            cin.get();
                            setTime();
                            system("pause");
                            break;
                        default:
                            break;
                    }
                }
            default:
                break;
        }
    }
    system("pause");
    return 0;
}
