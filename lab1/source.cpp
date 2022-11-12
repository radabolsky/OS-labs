#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

int main()
{    //вектор, итератор и дескриптор Heap'а
    vector<char*> vDrives;
    vector<char*>::iterator curDrive;
    HANDLE hHeap;

    //Переменные для GetVolumeInformation и GetDiskFreeSpace
    char FileSystemName[6], szDriveName[32], szOutData[128];
    DWORD dwSecPerClus, dwBytesPerSec, dwFreeClus, dwTotalClus;
    float fTotal, fFree;

    //Получаем строку и заполняем вектор
    DWORD dwSize = GetLogicalDriveStringsA(0,NULL);
    hHeap = GetProcessHeap();
    char* szDrives = (char*)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,dwSize);
    GetLogicalDriveStrings(dwSize,szDrives);
    UINT nDriveType;
    char* szNext=szDrives;
    while(szNext[0]!=0)
    {
        vDrives.push_back(szNext);
        szNext=strchr(szNext,0)+1;
    }

    //Подготовка и вывод на экран информации
    for(curDrive=vDrives.begin();curDrive<vDrives.end();curDrive++)
    {
        GetVolumeInformation(*curDrive,szDriveName,sizeof(szDriveName),NULL,NULL,NULL,FileSystemName,sizeof(FileSystemName));
        GetDiskFreeSpace(*curDrive,&dwSecPerClus,&dwBytesPerSec,&dwFreeClus,&dwTotalClus);
        fTotal = (float)dwTotalClus/1024*dwSecPerClus*dwBytesPerSec/1048576;
        fFree = (float)dwFreeClus/1024*dwSecPerClus*dwBytesPerSec/1048576;
        nDriveType = GetDriveType(*curDrive);
        switch(nDriveType)
        {
            case 0: cout << *curDrive << " - Unknown" << endl;
                break;
            case 1: cout << *curDrive << " - No root dir" << endl;
                break;
            case 2: cout << *curDrive << " - Removable(Floppy or Flash drive)" << endl;
                break;
            case 3: cout << *curDrive << " - Hard disk" << endl;
                break;
            case 4: cout << *curDrive << " - Remote" << endl;
                break;
            case 5: cout << *curDrive << " - CD/DVD-ROM" << endl;
                break;
            case 6: cout << *curDrive << " - RAM - disk" << endl;
        }
        sprintf(szOutData,"\t%s - %s\n\tTotal: %3.2f GB, Free: %3.2f GB\n\n",szDriveName,FileSystemName,fTotal,fFree);
        cout << szOutData;
    }

    //Освобождаем Heap и делаем паузу
    HeapFree(hHeap,0,szDrives);

//    TCHAR test[] = "Русский";
//    cout << test;
    system("pause");
    return 0;
}