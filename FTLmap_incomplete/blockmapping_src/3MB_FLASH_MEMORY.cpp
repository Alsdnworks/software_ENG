#define _CRT_SECURE_NO_WARNINGS
#include "3MB_FLASH_MEMORY.h"

//용량
int _vol = -1;
//FTL테이블
int* FTLtbl = new int[(Default * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
//플래시메모리
blc* flash = new blc[(Default * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];

void init(int volByMB) {//as mount 기존 메모리 삭제후 용량 할당
	delete[] flash;
	delete[] FTLtbl;
	//플레시메모리 생성
	flash = new blc[(volByMB * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
	for (int i = 0; i < BLOCK_CAPACITY; i++) {
		for (int j = 0; j < (volByMB * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY); j++) {
			memset(flash[j].s[i].chars, 0x20, SECTOR_CAPACITY);
		}
	}
//	//테이블생성
	FTLtbl = new int[(volByMB * MegaByte) / (SECTOR_CAPACITY)];
	for (int i = 0; i <= (volByMB * MegaByte) / (SECTOR_CAPACITY); i++) {
		FTLtbl[i] = Unsigned;
	}
	//용량갱신
	_vol = volByMB;
	//기록

}

//플래시 메모리 하드웨어 구성 read, write, erase
void Flash_read(int& PSN) {
	//read용 반복자
	int iter = 0;
	PSN = FTLtbl[PSN];
	char out[SECTOR_CAPACITY];
	memset(out, 0, SECTOR_CAPACITY);
	//출력버퍼


	if (_vol == -1) {
		std::cout << "초기화되지 않은 메모리입니다." << std::endl;
		return;
	}
	if (PSN == Unsigned) {
		std::cout << "데이터가 없습니다." << std::endl;
		return;
	}
	while (flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].chars[iter] != 0x20) {
		out[iter] = (flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].chars[iter]);
		iter++;
	}
	std::cout << out;
}

	void FTL_read(int& PSN) {
		//read용 반복자
		int iter = 0;
		char out[SECTOR_CAPACITY];
		memset(out, 0, SECTOR_CAPACITY);
		//출력버퍼
		if (_vol == -1) {
			std::cout << "초기화되지 않은 메모리입니다." << std::endl;
			return;
		}
		if (flash[PSN / BLOCK_CAPACITY].s[PSN % SECTOR_CAPACITY].chars[iter] == 0x20) {
			std::cout << "데이터가 없습니다." << std::endl;
			return;
		}
		while (flash[PSN / BLOCK_CAPACITY].s[PSN % SECTOR_CAPACITY].chars[iter] != 0x20) {
			out[iter] = (flash[PSN / BLOCK_CAPACITY].s[PSN % SECTOR_CAPACITY].chars[iter]);
			iter++;
		}

	std::cout << out;
}
void Flash_write(int PSN, const char* data) {
	if (_vol == -1) {
		std::cout << "초기화되지 않은 메모리입니다." << std::endl;
		return;
	}
	int i = 0;
	char buffer;
	//데이터의 번지를따라 섹터에 순차적할당
	while (data[i] != NULL) {
		buffer = data[i];
		flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].chars[i] = buffer;
		i++;
	}
	//섹터 write업데이트
	flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].uses++;
	int out = flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].uses;
	std::cout << data << " is writted on block " << PSN / BLOCK_CAPACITY << " psn " << PSN % BLOCK_CAPACITY << std::endl;
	std::cout << "write count is" << out << std::endl;
	return;

}

//지우기작업은 0x20덮어씌우기로 이루어짐
void Flash_erase(int PBN) {
	for (int i = 0; i < BLOCK_CAPACITY; i++) 
		memset(flash[PBN].s[i].chars, 0x20, SECTOR_CAPACITY);
	flash[PBN].erases++;
	std::cout << "block " << PBN << " is erased" << std::endl;
	std::cout << "erase count is" << flash[PBN].erases << std::endl;
	return;
}

//FTL처리
void FTL(int LSN, const char* data) {
	bool proc = false;
	int PSN = FTLtbl[LSN];
	int currentBlock = LSN / BLOCK_CAPACITY;
	int sec_counter = 0;
	int block_checker = 0;
	int checked = 0;
	int offset = 1;
	while (1) {
		//만약 섹터가 가득찼다면
		if (sec_counter == BLOCK_CAPACITY) {
			//다음 블록이 빈블록인지확인
			while (true) {
				block_checker = (PSN / BLOCK_CAPACITY) + offset;
				for (int i = 0; i < BLOCK_CAPACITY; i++) {
					if (flash[block_checker].s[i].chars[0] == 0x20)
						checked++;
				}
				//비어있다면 옮긴다
				if (checked == BLOCK_CAPACITY) {
					//덮어쓰기대상의 섹터 할당 해제
					FTLtbl[LSN] = Unsigned;
					//현재 물리적 사상된 섹터를 옮긴다
					for (int i = currentBlock*BLOCK_CAPACITY; i < currentBlock * BLOCK_CAPACITY+BLOCK_CAPACITY; i++) {
						if (FTLtbl[i] != Unsigned) {
							//새로운 사상
							strcpy(flash[block_checker].s[FTLtbl[i]%BLOCK_CAPACITY].chars, flash[currentBlock].s[FTLtbl[i] % BLOCK_CAPACITY].chars);
							flash[block_checker].s[FTLtbl[i]].uses++;
							FTLtbl[i] = block_checker*BLOCK_CAPACITY+ (FTLtbl[i] % BLOCK_CAPACITY);
						}
					}
					//이전블록 제거
					Flash_erase(currentBlock);
					PSN += offset * BLOCK_CAPACITY;
					proc = true;
					currentBlock = block_checker;
					break;
				}
				else {
					checked = 0;
					offset++;
				} 
			}
		}
		//블록 넘지않도록 제어
		if (PSN / BLOCK_CAPACITY != currentBlock) {
			PSN = currentBlock * BLOCK_CAPACITY;
		}
		sec_counter++;
		//비어있는 섹터가있다면 기록
		if (flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].chars[0] == 0x20) {
			//라이트함수로 리턴
			FTLtbl[LSN] = PSN;
			Flash_write(PSN, data);
			return;
		}
		//다음 번호로 사상
		else PSN++;
	}
}
void lookup(int start,int end) {
	int i = start;
	int j = 0;
	while (i < end) {
		 j = 0;
		 std::cout << " PSN: " << i;
		 std::cout << " Value: ";
		while (flash[i / BLOCK_CAPACITY].s[i % BLOCK_CAPACITY].chars[j]!=0x20) {
			std::cout << flash[i / BLOCK_CAPACITY].s[i %BLOCK_CAPACITY].chars[j];
			j++;
		}
		std::cout << std::endl;
		i++;
	}
}
void flookup(int start, int end) {
	int i = start;
	while (i < end) {
		std::cout << " PSN: " << FTLtbl[i];
		std::cout << std::endl;
		i++;
	}
}