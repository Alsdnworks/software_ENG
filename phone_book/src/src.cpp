//**참고: 비어있는줄오른쪽보면 구현해야할 내용 있음**
//주요디버깅이슈는 메인의 상단참조

#include"book.h"

//초성버퍼
static const wchar_t wcF[] = {
    L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
    L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
    L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
    L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
    L'ㅌ', L'ㅍ', L'ㅎ' };

//알파벳버퍼
static const wchar_t wcA[] = {
    L'A', L'B', L'C', L'D',
    L'E', L'F', L'G', L'H',
    L'I', L'J', L'K', L'L',
    L'M', L'N', L'O', L'P',
    L'Q', L'R', L'S', L'T', L'U',
    L'V', L'W', L'X', L'Y', L'Z' };


bool checkHangulForUniCode(const wchar_t& ch) {// for unicode
    return !(ch < 44032 || ch > 55199); // 0xAC00(가) ~ 0xD7A3(힣)
}
bool checkAlphForUniCode(const wchar_t& ch) {// for unicode
    return ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)); // 영어 대문자 소문자
}

// 초성분리
wchar_t wchar_FML(const wchar_t& wc) {
    int F;
    // 한글인지 구분
    if (checkHangulForUniCode(wc)) {
        int wc2 = (int)wc - 0xAC00;
        wc2 /= 28;
        F = wc2 / 21;
        return wcF[F];
    }//영어인지확인
    else if (checkAlphForUniCode(wc)) {
        int wca = wc;
        if (wc >= 97 && wc <= 122)
            wca -= 32;
        wca -= 65;
        return wcA[wca];
    }
    return '*'; // 둘다아님 숫자와 특수문자->*로처리 실제 출력에서는 -로
}


wchar_t headCharParser(wstring in) {
    wstring a;
    a.assign(in.begin(), in.end()); // 분석할 문자열
    return wchar_FML(a[0]);
}
//초성분리를 위해 wsting의 유니코드영역사용. 동일하게 영문자도 분리+ 대소문자 동일취급기능추가
//한글초성원본코드https://github.com/letyletylety/Hangul

bool asname(node a, node b) {//이름순정렬
    return a.name < b.name;
}

bool asgroup(node a, node b) {//그룹명순정렬
    return a.group < b.group;
}

wstring Book::assignHyphenRule(wstring callnum) {//서울권, 일반, 특수번호 하이픈적용함수
    wstring assigned = callnum;
    if ((callnum.length() == 9 || callnum.length() == 10) && wcsncmp(callnum.c_str(), L"02", 2) == 0) { //서울권
        assigned.insert(2, L"-");
        assigned.insert((assigned.length() - 4), L"-");
    }

    else if (callnum.length() == 11 || callnum.length() == 10) { //일반번호
        assigned.insert(3, L"-");
        assigned.insert((assigned.length() - 4), L"-");
    }
    return assigned;
}

void Book::delData(int index) {//인덱스받고데이터삭제
    //인덱스모드
    if (index > modePointer->size() - 1 || context.empty()) {
        printf("잘못된 입력입니다\n");
        return;
    }
    modePointer->erase(modePointer->begin() + index);
    printf("삭제완료\n");
    return;
}

void Book::srchclear() {//서치버퍼초기화,서칭모드해제
    setMode(false);
    srchBuffer.clear();
}

void Book::newData() {//데이터입력
    node newman;
    printf("new data\n");
namecorrection:
    printf("\nname: ");
    wcin >> newman.name;

    if (newman.name.empty()) {
        printf("이름은 비워둘수없습니다");
        goto namecorrection;
    }
    newman.name_index = headCharParser(newman.name);
numbercorrection:
    printf("\nphonenumber: ");
    wcin >> newman.telnum;
    if (newman.telnum.empty()) {
        printf("번호는 비워둘수없습니다");
        goto numbercorrection;
    }
    if (_wtoi(newman.telnum.c_str()) == 0) {
        printf("문자나 '-'없이 번호만 입력해주세요(입력오류)");
        goto numbercorrection;
    }
    printf("\ngroup: ");
    wcin.ignore();
    getline(wcin, newman.group);
    if (newman.group.empty())
        newman.group = L"-";

    newman.group_index = headCharParser(newman.group);
    context.push_back(newman);
}

void Book::editData(int index) {  //데이터편집
    //인덱스모드
    int ctrl;
    if (index > modePointer->size() - 1 || context.empty()) {
        printf("잘못된 입력입니다\n");
        return;
    }
    wcout << "수정할 항목을 선택하십시오" << endl
        << "1. 이름: " << modePointer->at(index).name << endl
        << "2. 번호: " << modePointer->at(index).telnum << endl
        << "3. 그룹: " << modePointer->at(index).group << endl;
    cin >> ctrl;
    switch (ctrl) {
    case 1:
    rename:
        printf("\nname: ");
        wcin >> (modePointer->at(index)).name;
        if ((modePointer->at(index).name).empty()) {
            printf("이름은 비워둘수없습니다");
            goto rename;
        }
        modePointer->at(index).name_index = headCharParser((modePointer->at(index)).name);
        break;
    case 2:
    renumber:
        printf("\nphonenumber: ");
        wcin >> (modePointer->at(index)).telnum;
        if ((modePointer->at(index)).telnum.empty()) {
            printf("번호는 비워둘수없습니다");
            goto renumber;
        }
        break;
    case 3:
        printf("\ngroup: ");
        wcin >> (modePointer->at(index)).group;
        (modePointer->at(index)).group_index = headCharParser((modePointer->at(index)).group);
        break;
    }
    printf("변경완료\n");
    return;
}

void Book::sorting(int piv) {//정렬기준 변경

    sortvar = piv;
    if (context.empty())
        return;
    if (sortvar == true)
        sort((modePointer->begin()), (modePointer->end()), asname);
    else
        sort((modePointer->begin()), (modePointer->end()), asgroup);
}

void Book::search(wstring kwd) {//포인터형으로미구현...mode셀렉터에 auto를 담을수이었다면...//데이터검색
//    searchRdy:
//    wchar_t kwd_c = headCharParser(kwd);
    for (int i = 0; i < context.size(); i++) {
        if (context[i].name == kwd || context[i].group == kwd) {
            srchBuffer.push_back(context[i]);
        }
    }
    if (srchBuffer.empty()) {
        cout << "해당 키워드에 해당되는 데이터를 조회할수없습니다.\n 완전한 이름, 그룹을 입력해주세요";
        return;
    }
    else {
        setMode(true);
        printf("탐색완료\n");
    }
    return;
}

void Book::printall() {//전체출력
    wcout.imbue(locale("korean"));
    wcout.precision(3);
    wcout.setf(ios::right);
    wcout << endl
        << endl;
    wcout << "Index" << setw(8) << L"no." << setw(10) << L"name" << setw(20) << L"num" << setw(20) << L"group" << endl;
    wcout << setw(0);
    for (int i = 0; i < modePointer->size(); i++) {
        if (i != 0) {
            if (sortvar == 1) {
                if ((modePointer->at(i)).name_index != (modePointer->at(i - 1)).name_index) //네임라벨링
                    wcout << endl << endl << modePointer->at(i).name_index << endl
                    << endl;
            }
            else if (sortvar == 0) {
                if ((modePointer->at(i)).group_index != (modePointer->at(i - 1)).group_index) //그룹라벨링
                    wcout << endl << endl << modePointer->at(i).group_index << endl
                    << endl;
            }
        }
        else {
            if (sortvar == 1)
                wcout << modePointer->at(i).name_index << endl;
            else
                wcout << modePointer->at(i).group_index << endl;
        }
        wcout << setw(10) << i << setw(10) << (modePointer->at(i)).name << setw(20) << assignHyphenRule((modePointer->at(i)).telnum) << setw(20) << (modePointer->at(i)).group << endl;
    }
    wcout << endl;
}

void Book::init() {
    context.reserve(10000); //naverworks의 연략처 한도 사용
    //9/11 label을 위한 더미데이터, 입력값과 곂치지않도록 제어문자 사용// 9/14예외처리 사용으로 이제 안써도됨
    locale::global(std::locale("Korean"));
    wifstream i_data;
    node importedData;
    i_data.open("data.txt"); //있다면데이터베이스 불러오기
    if (!i_data.is_open() || i_data.bad()) {
        printf("전화번호부 데이터 파일을 찾을수없습니다.\n저장경로가 다르거나 예상치못한 종료로 인해\n 파일이 손상되었을수도있습니다\n최초 실행의 경우 계속 진행하셔도 좋습니다.\n");
        return;
    }
    else {
        while (!i_data.eof()) {
            if (i_data.eof())
                break; //SOF참고하여 eof중복확인기능추가(eof함수자체오류로 중복입력되는것 예외처리)
            i_data >> importedData.name >> importedData.telnum >> importedData.group >> importedData.name_index >> importedData.group_index;
            if (!importedData.name.empty()) {
                if (context.size() >= 1) {
                    if (importedData.name != (context.back().name))
                        context.push_back(importedData);
                }
                else
                    context.push_back(importedData);
            }
        }
        i_data.close();
    }
    sorting(1);
}

void Book::terminate() {//저장하고종료
    locale::global(std::locale("Korean"));
    wofstream o_data;
    wstring nullchk;
    o_data.open("data.txt");
    for (int i = 0; i < context.size(); i++) {
        wstring name = (context.at(i)).name;
        o_data.write(name.c_str(), name.size());
        o_data << endl;
        wstring num = (context.at(i)).telnum;
        o_data.write(num.c_str(), num.size());
        o_data << endl;
        wstring group = (context.at(i).group);
        o_data.write(group.c_str(), group.size());
        o_data << endl;
        wchar_t nIndx = (context.at(i).name_index);
        o_data << nIndx;
        o_data << endl;
        wchar_t gIndx = (context.at(i).group_index);
        o_data << gIndx;
        o_data << endl;
        nullchk = gIndx;
        if (nullchk.empty())
            gIndx = L'*'; //wcahr_t->wstring char에서 ' '나 엠프티못써서 타입캐스팅
    }
    o_data.close();
    exit(0);
}

bool Book::getMode() {//메인과 검색메뉴셋
    return srchMod;
}

void Book::setMode(bool mode) {//
    srchMod = mode;
    modePointer = &context;
    (srchMod == false) ? modePointer = &context : modePointer = &srchBuffer;
}
