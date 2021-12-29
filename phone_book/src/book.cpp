#include"book.h"
Book func;
string svar;
int pivot_var = 1;

void add() {
	func.newData();
}
void del() {
	int set;
	svar.clear();
	printf("인덱스를 입력해 삭제합니다: ");
	cin >> set;
	func.delData(set);
}

void edit() {
	int set;
	printf("인덱스를 입력해 검색합니다: ");
	cin >> set;
	func.editData(set);
}
void pivot() {
	printf("1. 이름을 기준으로 내림차순 정렬합니다\n2. 그룹을 기준으로 내림차순 정렬합니다\n");
	cin >> pivot_var;
	if ((pivot_var < 1) || (pivot_var > 2)) {
		printf("잘못 입력하였습니다.\n");
		cin.clear();//내용초기화
		cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키 
		pivot();
	}
	(pivot_var == 2) ? pivot_var = 0 : pivot_var = 1;
	func.sorting(pivot_var);
}

void srch() {
	wstring set;
	printf("완전한 그룹명이나 이름을 입력해 검색합니다: ");
	wcin >> set;
	func.search(set);
}
void shutdown() {
	func.terminate();
}
void srchclear() {
	func.srchclear();
}
//main starts here
int main() {
	setlocale(LC_ALL, "korean"); // locale setting
	wcin.imbue(locale("korean"));
	wcout.imbue(locale("korean"));
	func.init();
	cout << "전화번호부\n\n" << endl;
	//임포트
	while (true) {
		func.sorting(pivot_var);
		if (func.getMode() == false) {
			func.printall();
			cout << "----------명령을 선택하십시오----------\n"
				<< "1.새 데이터를 추가합니다.\n"
				<< "2.기존 데이터를 삭제합니다.\n"
				<< "3.기존 데이터를 편집합니다.\n"
				<< "4.데이터 정렬 기준을 변경합니다.\n"
				<< "5.데이터를 검색합니다.\n"
				<< "6.변동사항을 저장하고 프로그램을 종료합니다.\n" << endl;
			int set = 0;
			cin >> set;
			if ((set < 1) || (set > 6)) {
				printf("잘못 입력하였습니다.\n");
				cin.clear();//내용초기화
				cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키 
				continue;
			}
			else {
				switch (set)
				{
				case 1:
					add();
					break;
				case 2:
					del();
					break;
				case 3:
					edit();
					break;
				case 4:
					pivot();
					break;
				case 5:
					srch();
					break;
				case 6:
					shutdown();
					break;
				};
			}
		}
		else {
			func.printall();
			cout << "----------검색결과----------\n"

				<< "1.메인메뉴로 복귀합니다.\n" << endl;
			int set = 0;
			cin >> set;
			if ((set < 1) || (set > 1)) {
				printf("잘못 입력하였습니다.\n");
				cin.clear();//내용초기화
				cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키 
				continue;
			}
			else {
				switch (set)
				{
				case 1:
					srchclear();
					break;
				};
			}
		}
	}
	return 0;
}