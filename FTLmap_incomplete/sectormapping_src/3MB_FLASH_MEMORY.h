#include <iostream>
#include <iomanip>
#include <algorithm>
#include<vector>
#include<utility>

#define BLOCK_CAPACITY 32//A BLOCK IS SET OF 32 SECTOR
#define SECTOR_CAPACITY 512 //A SECTOR IS UNIT OF 512Byte
#define MegaByte 1048576 //MB for Bytes
#define Default sizeof(char) //최초실행시 메모리 초기화용
#define Unsigned_S INT_MAX //사용되지 않은 LSN의 PSN값
#define Used sizeof(short)//라이트카운터
#define Unsigned_B USHRT_MAX//사상되지않은 블록값

//1섹터의 크기는 512바이트
struct sec {
	char* chars = new char[SECTOR_CAPACITY];
	short uses = 1;
};
//1블록은 32섹터
struct blc {
	sec* s = new sec[BLOCK_CAPACITY];
	short erases = 0;
};

void init(int volByMB);
void Flash_read(int& PSN);
void FTL_read(int& PSN);
void Flash_write(int PSN, const char* data);
void Flash_erase(int PBN);
void FTL(int LSN, const char* data);
void lookup(int start, int end);
void flookup(int start, int end);
void rflookup(int start, int end);
void menu();