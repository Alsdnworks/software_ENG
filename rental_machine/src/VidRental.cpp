#include"vidRental.h"
Videoclient func;
string svar;
int pivot_var = 1;


void rentAndReturn() {
	int ctrl;
	cout << "1.대출\n 2.반납";
	cin>>ctrl;
	if (ctrl == 1) {
		func.chkIn();
	}
	else if (ctrl == 2) {
		func.chkOut();
	}
}

void add() {
	string na, te, br; int nf; int ctrl; bool adult;
	cout << "1.새 비디오 등록\n2. 새 사용자 등록";
	cin>>ctrl;
	if (ctrl == 1) {
		cout << "비디오 제목 입력";
		cin >> na;
		chungsonyunshichungbulga:
		cout << "청소년 시청 불가 입니까?\n1.예\n2.아니요";
		cin >> nf;
		if (nf == 1)
			adult = true;
		else if (nf == 2)
			adult = false;
		else {
			cout<<"잘못입력했습니다.";
			goto chungsonyunshichungbulga;
		}
		func.newVid(na, adult);
	}
	else if (ctrl == 2) {
		cout << "이름 입력";
		cin >> na;
		cout << "전화번호 입력(하이픈 없이)";
		cin >> te;
		cout << "생년월일 입력('.'없이)";
		cin >> br;
		func.newUser(na,te,br,5);
	}
	else 
		printf("잘못된 명령입니다.");
}
void del() {
	string na; int ctrl;
	cout << "1.비디오 삭제\n2. 사용자 삭제";
	cin>>ctrl;
	cout << "삭제할 항목명 입력";
	cin >> na;
	if (ctrl==1)
		func.delVid(func.searchVid(na));
	else if (ctrl==2)
		func.delUser(func.searchUser(na));
	else 
		printf("잘못된 명령입니다.");	
}

void edit() {
	string na; int ctrl;
	cout << "1.비디오 편집\n2. 사용자 편집";
	cin>>ctrl;
	cout << "편집할 항목명 입력";
	cin >> na;
	if (ctrl == 1)
		func.editVid(func.searchVid(na));
	else if (ctrl == 2)
		func.editUser(func.searchUser(na));
	else
		printf("잘못된 명령입니다.");
}

void srch() {
	string na; int ctrl;
	cout << "1.비디오 찾기\n2. 사용자 찾기";
	cin>>ctrl;
	cout << "검색할 항목명 입력 *로 전체 조회 가능";
	cin >> na;
	if (ctrl==1)
		func.searchVidList(na);
	else if (ctrl==2)
		func.searchUserList(na);
	else 
		printf("잘못된 명령입니다.");
}


//main starts here
int main() {
	func.init();
	cout << "비디오 대출 관리 프로그램\n\n" << endl;
	//임포트
	while (true) {
		//func.sorting(pivot_var);
		if (func.getMode() == false) {
			func.printAll();
			cout << "----------명령을 선택하십시오----------\n"
				<< "1.대출/반납.\n"
				<< "2.새 데이터를 추가합니다.\n"
				<< "3.기존 데이터를 삭제합니다.\n"
				<< "4.기존 데이터를 편집합니다.\n"
				<< "5.데이터를 검색합니다.\n"
				<< "6.환경설정\n"
				<< "7.변동사항을 저장하고 프로그램을 종료합니다.\n"
				<< endl;
			int set = 0;
			cin >> set;
			if ((set < 1) || (set > 7)) {
				printf("잘못 입력하였습니다.\n");
				cin.clear();//내용초기화

				cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키 
				continue;
			}
			else {
				switch (set)
				{
				case 1:
					rentAndReturn();
					break;
				case 2:
					add();
					break;
				case 3:
					del();
					break;
				case 4:
					edit();
					break;
				case 5:
					srch();
					break;
				case 6:
					func.setSysVar();
					break;
				case 7:
					func.terminate();
					break;
				};
			}
		}
	}
	atexit(terminate);
	return 0;
}