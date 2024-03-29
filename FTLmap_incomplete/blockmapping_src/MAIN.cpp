#include "3MB_FLASH_MEMORY.h"
using namespace std;

int vol;

void menu() {
	cout << endl <<
		"---------------명령을 선택하십시오---------------\n" <<
		left << setw(20) << "init [volumeByMB]" << right << setw(30) << "지정된 용량을 할당합니다.\n"
		<< left << setw(20) << "R 또는 read [lsn]" << right << setw(30) << "섹터에 저장된 자료를 읽습니다.\n"
		<< left << setw(20) << "W 또는 write [lsn] [data]" << right << setw(30) << "섹터에 자료를 작성합니다.\n"
		<< left << setw(20) << "E 또는 erase [block]" << right << setw(30) << "블록을 제거합니다.\n"
		<< left << setw(20) << "L 또는 lookup [start] [end]" << right << setw(30) << "지정된 psn까지의 실제 기록된 테이블을 조회합니다..\n"
		<< left << setw(20) << "exit" << right << setw(30) << "프로그램을 종료합니다.\n"
		<< left << setw(20) << "menu" << right << setw(30) << "메뉴를 호출합니다.\n"
		<< endl;
}

int main() {

	menu();
	int psn;
	int out;
	int start;
	int end;
	char* data = new char[SECTOR_CAPACITY];
	char* command = new char[SECTOR_CAPACITY];

	while (1) {
		cout << ">";
		cin >> command;

		if (0 == strcmp(command, "R") || 0 == strcmp(command, "read")) {
			cin >> psn;
			Flash_read(psn);
			cout << endl;
		}
		else if (0 == strcmp(command, "W") || 0 == strcmp(command, "write")) {
			cin >> psn;
			cin >> data;
			out = 1;
			FTL(psn, data);
		}
		else if (0 == strcmp(command, "E") || 0 == strcmp(command, "erase")) {
			cin >> psn;
			Flash_erase(psn);
		}
		else if (0 == strcmp(command, "L") || 0 == strcmp(command, "lookup")) {
			cin >> start;
			cin >> end;
			lookup(start,end);
		}
		else if (0 == strcmp(command, "F") || 0 == strcmp(command, "flookup")) {
			cin >> start;
			cin >> end;
			flookup(start, end);
		}

		else if (0 == strcmp(command, "exit"))
			return 0;
		else if (0 == strcmp(command, "menu"))
			menu();
		else if (0 == strcmp(command, "init")) {
			cin >> vol;
			init(vol);
			cout << vol << "megabytes flash memory\n";
		}
		else {
			printf("Invalid command\n");
			cin.clear();//내용초기화
			cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키
			continue;
		}
	}
	return 0;
}