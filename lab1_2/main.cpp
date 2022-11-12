#include <iostream>
#include <windows.h>

using namespace std;

//typedef struct
//{
//    OVERLAPPED oOverlap{};
//    HANDLE hPipeInst{};
//    TCHAR chRequest{}[BUFSIZE];
//    DWORD cbRead{};
//    TCHAR chReply{}[BUFSIZE];
//    DWORD cbToWrite{};
//} PIPEINST, *LPPIPEINST;

typedef unsigned long long ull;
typedef  struct{
    OVERLAPPED  oOverlap;
    HANDLE hFrom;
    HANDLE hOut;
    DWORD highOffset;
    ull inputSize;
    DWORD bufSize;
    char* iBuffer;
    char* oBuffer;
} FILEINST, *LPFILEINST;


HANDLE chooseFile(){
    char filePath[257];
    cout << "Enter the path to the file you want to copy" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR)filePath,
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,
                              OPEN_EXISTING,
                              FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
                              NULL);
    return hFile;
}

HANDLE newFile(){
    char filePath[257];
    cin.get();
    cout << "Enter the path to the file you want to create" << endl;
    cin.getline(filePath, sizeof(filePath), '\n');
    HANDLE hFile = CreateFile((LPCSTR)filePath,
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,
                              CREATE_NEW,
                              FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
                              NULL);
}

struct writeEvent{
    LPOVERLAPPED overWrite;
    HANDLE hFile;
    char* buff;
    DWORD blockSize;


};

typedef struct writeEvent writeEvent;

int test = 0;

void CALLBACK writeCompleted(DWORD dwErrorCode, DWORD dwCount, LPOVERLAPPED lpOverlapped){
    test += 1;
}


VOID CALLBACK readCompleted(DWORD dwErrorCode, DWORD dwCount, LPOVERLAPPED lpOverlapped){
    auto pWriteEvent = (writeEvent*) lpOverlapped->hEvent;
    cout << pWriteEvent->blockSize << endl;
    SleepEx(1, true);
//    WriteFileEx(pWriteEvent->hFile,
//                pWriteEvent->buff,
//                pWriteEvent->blockSize,
//                pWriteEvent->overWrite,
//                writeCompleted);
//return;
//    test++;

}
VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap);
VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap);


VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap){
    LPFILEINST lpFileInst;
    BOOL fRead = FALSE;
    lpFileInst = (LPFILEINST) lpOverLap;
    if (dwErr != 0){
        ReadFileEx(
                lpFileInst->hFrom,
                lpFileInst->iBuffer,
                lpFileInst->bufSize,
                (LPOVERLAPPED) lpFileInst,
                (LPOVERLAPPED_COMPLETION_ROUTINE) CompletedReadRoutine
                );
    }
}

VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) {
}

void makeOverlappedCopy(HANDLE from, HANDLE out, int blockSize, int nOperations){
    LPFILEINST lpFileInst;
    OVERLAPPED  oFileInfo;

    int buffSize = blockSize / nOperations;
    char* iBuffer = new char[buffSize];
    char* oBuffer = new char[buffSize];

    lpFileInst = new FILEINST;
    lpFileInst->hFrom = from;
    lpFileInst->hOut = out;
    lpFileInst->iBuffer = iBuffer;
    lpFileInst->oBuffer = oBuffer;
    lpFileInst->inputSize = GetFileSize(from, &lpFileInst->highOffset);

    CompletedWriteRoutine(0, 0, (LPOVERLAPPED) lpFileInst);



}


void makeCopy(HANDLE from, HANDLE out, int blockSize, int nOperations){
    char buff[nOperations][blockSize];
    OVERLAPPED overRead[nOperations], overWrite[nOperations];
    writeEvent event;
    DWORD maxOffset;
    long long fileSize = GetFileSize(from, &maxOffset);
    long long cur = 0;
    event.blockSize = blockSize;
    event.hFile = out;
    for (int i=0; i < nOperations; ++i){
        overRead[i].Offset = i * blockSize;
        overWrite[i].Offset = i * blockSize;
        overRead[i].OffsetHigh = overWrite[i].OffsetHigh = maxOffset;
    }

    while (cur < fileSize){
        for (int i=0; i < nOperations; ++i){
            event.overWrite = &overWrite[i];
            event.buff = buff[i];
            overRead[i].hEvent = &event;
            ReadFileEx(from, buff[i], blockSize, &overRead[i], readCompleted);
            WriteFileEx(out, buff[i], blockSize, &overWrite[i], writeCompleted);
//            overWrite[i].Offset += blockSize*nOperations;
        }
        cur += blockSize * nOperations;

    }
    SetFilePointer(out, fileSize, NULL, FILE_BEGIN);
    SetEndOfFile(out);

}



int main() {
    HANDLE from = chooseFile();
    if (from == INVALID_HANDLE_VALUE){
        cout << "Error: " << GetLastError() << endl;
        system("pause");
        return 1;
    }
    HANDLE out = newFile();
    if (out == INVALID_HANDLE_VALUE){
        cout << "Error: " << GetLastError() << endl;
        system("pause");
        return 1;
    }

    int blockSize;
    cout << "Enter the value of the multiplicity of the block (4096): " << endl;
    cin >> blockSize;
    while (blockSize < 0){
        cout << "Invalid cluster size. The value must be greater than 0 and not exceed the file size" << endl;
        cin >> blockSize;
    }
    blockSize *= 4096;

    int nOperations;
    cout << "Enter the number of overlapping I/O operations: " << endl;
    cin >> nOperations;
    while (nOperations < 1){
        cout << "The number of operations must be greater than 0 and a multiple of 2" << endl;
        cin >> nOperations;
    }
    nOperations -= nOperations != 1 ? nOperations % 2 : 0;
    makeCopy(from, out, blockSize, nOperations);
    cout << test << endl;
    system("pause");
    return 0;
}
