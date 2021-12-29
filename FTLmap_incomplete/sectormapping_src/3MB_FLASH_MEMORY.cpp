#define _CRT_SECURE_NO_WARNINGS
#include "3MB_FLASH_MEMORY.h"
//용량
int _vol = -1;
//최대 섹터 
int SEC_MAX;
//FTL테이블
int* FTLtbl = new int[(Default * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
std::vector<std::pair<unsigned short, unsigned short>> blcmap(2, std::make_pair(Unsigned_B, Unsigned_B));
//플래시메모리
blc* flash = new blc[(Default * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
bool* bFTLtbl = new bool[(Default * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
void init(int volByMB) { //as mount 기존 메모리 삭제후 용량 할당
    delete[] flash;
    delete[] FTLtbl;
    delete[] bFTLtbl;
    blcmap.clear();

    //플레시메모리 생성
    flash = new blc[(volByMB * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY)];
    for (int i = 0; i < BLOCK_CAPACITY; i++) {
        for (int j = 0; j < (volByMB * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY); j++) {
            memset(flash[j].s[i].chars, 0x20, SECTOR_CAPACITY);
        }
    }

    //FTL테이블생성
    FTLtbl = new int[(volByMB * MegaByte) / (SECTOR_CAPACITY)];
    for (int i = 0; i <= (volByMB * MegaByte) / (SECTOR_CAPACITY); i++)
        FTLtbl[i] = Unsigned_S;

    //읽기전용블록 표시하는 테이블
    bFTLtbl = new bool[(volByMB * MegaByte) / (SECTOR_CAPACITY * BLOCK_CAPACITY)];
    for (int i = 0; i <= (volByMB * MegaByte) / (SECTOR_CAPACITY * BLOCK_CAPACITY); i++)
        bFTLtbl[i] = true;

    //R/W 사상기록
    blcmap.resize((volByMB * MegaByte) / (BLOCK_CAPACITY * SECTOR_CAPACITY), std::make_pair(Unsigned_B, Unsigned_B));

    //용량갱신
    _vol = volByMB;
    SEC_MAX = (volByMB * MegaByte) / (SECTOR_CAPACITY);
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
    if (PSN == Unsigned_S) {
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
    unsigned short blciter;
    //출력버퍼
    if (_vol == -1) {
    READ_ERROR:
        std::cout << "초기화되지 않은 메모리 또는 읽기 오류 입니다." << std::endl;
        return;
    }
    if (blcmap[PSN / BLOCK_CAPACITY].first != Unsigned_B) {
        blciter = blcmap[PSN / BLOCK_CAPACITY].first;
    }
    else if (blcmap[PSN / BLOCK_CAPACITY].second != Unsigned_B) {
        blciter = blcmap[PSN / BLOCK_CAPACITY].second;
    }
    else goto READ_ERROR;
    while (flash[blciter].s[PSN % SECTOR_CAPACITY].chars[iter] != 0x20) {
        out[iter] = (flash[blciter].s[PSN % SECTOR_CAPACITY].chars[iter]);
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
    int offset = 0;
    if (PSN / BLOCK_CAPACITY != currentBlock)
        PSN = currentBlock * BLOCK_CAPACITY;
    //읽기용 스페어블록이라면 오프셋 추가
    if (bFTLtbl[PSN / BLOCK_CAPACITY] == false) {
        while (bFTLtbl[(PSN / BLOCK_CAPACITY) + offset] != true)
            offset++;
        PSN += BLOCK_CAPACITY * offset;
        currentBlock += offset;
    }
    while (1) {
        //만약 PSN이 섹터의 끝이라면
        if (block_checker * BLOCK_CAPACITY >= SEC_MAX) {
            block_checker = 0;
        }
        //만약 섹터가 가득찼다면
        if (sec_counter == BLOCK_CAPACITY) {
            //다음 블록이 빈블록인지확인
            while (true) {
                block_checker = (PSN / BLOCK_CAPACITY) + offset;
                for (int i = 0; i < BLOCK_CAPACITY; i++) {
                    if (flash[block_checker].s[i].chars[0] == 0x20 && bFTLtbl[block_checker] == true)
                        checked++;
                    else
                        break;
                }
                //비어있다면 옮긴다
                if (checked == BLOCK_CAPACITY) {
                    //i범위 잘못됨 1222050 i는 4부터 시작해야함
                    for (int i = 0; i < 4; i++) {
                        if (FTLtbl[PSN + i] != Unsigned_S) {
                            //새로운 사상
                            strcpy(flash[block_checker].s[FTLtbl[PSN + i] % BLOCK_CAPACITY].chars, flash[FTLtbl[PSN + i] / BLOCK_CAPACITY].s[FTLtbl[i + PSN] % BLOCK_CAPACITY].chars);
                            //flash[block_checker].s[FTLtbl[i]].uses++;
                            FTLtbl[i + PSN] = block_checker * BLOCK_CAPACITY + (FTLtbl[i + PSN] % BLOCK_CAPACITY);
                        }
                    }
                    if (blcmap[PSN / BLOCK_CAPACITY].second != Unsigned_B) {
                        Flash_erase(blcmap[PSN / BLOCK_CAPACITY].second);
                        bFTLtbl[PSN / BLOCK_CAPACITY] = true;
                    }
                    blcmap[PSN / BLOCK_CAPACITY].second = block_checker;
                    bFTLtbl[block_checker] = false;
                    Flash_erase(blcmap[PSN / BLOCK_CAPACITY].first);
                    Flash_write((currentBlock * BLOCK_CAPACITY) + (PSN % BLOCK_CAPACITY), data);
                    FTLtbl[LSN] = (currentBlock * BLOCK_CAPACITY) + (PSN % BLOCK_CAPACITY);
                    return;
                }
                else {
                    //블록 오프셋
                    checked = 0;
                    offset++;
                }
            }
        }

        //비어있는 섹터가있다면 기록
        if (flash[PSN / BLOCK_CAPACITY].s[PSN % BLOCK_CAPACITY].chars[0] == 0x20) {
            //라이트함수로 리턴
            FTLtbl[LSN] = PSN;
            if (blcmap[PSN / BLOCK_CAPACITY].first == Unsigned_B) {
                blcmap[PSN / BLOCK_CAPACITY].first = PSN / BLOCK_CAPACITY;
                PSN = (blcmap[PSN / BLOCK_CAPACITY].first * BLOCK_CAPACITY) + PSN % BLOCK_CAPACITY;
            }
            Flash_write(PSN, data);
            return;
        }
        //다음 번호로 사상
        else
            PSN++;
        sec_counter++;
        //블록 넘지않도록 제어
        if (PSN / BLOCK_CAPACITY != currentBlock) {
            PSN = currentBlock * BLOCK_CAPACITY;
        }
    }
}

void lookup(int start, int end) {
    int i = start;
    int j = 0;
    while (i < end) {
        j = 0;
        std::cout << " PSN: " << i;
        std::cout << " Value: ";
        while (flash[i / BLOCK_CAPACITY].s[i % BLOCK_CAPACITY].chars[j] != 0x20) {
            std::cout << flash[i / BLOCK_CAPACITY].s[i % BLOCK_CAPACITY].chars[j];
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

void rflookup(int start, int end) {
    int i = start;
    while (i < end) {
        i++;
    }
}