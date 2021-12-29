#define _CRT_SECURE_NO_WARNINGS
#include"vidRental.h"

int Videoclient::nowtime() {
    time_t lowdata;
    struct tm* reTime;
    lowdata = time(NULL);
    reTime = localtime(&lowdata);
    int year = reTime->tm_year + 1900;
    int month = reTime->tm_mon + 1;
    int day = reTime->tm_mday;
    return year * 10000 + month * 100 + day;
}
int Videoclient::aftime() {
    time_t lowdata;
    struct tm* reTime;
    lowdata = time(NULL) + (86400 * remainTime);
    reTime = localtime(&lowdata);
    int year = reTime->tm_year + 1900;
    int month = reTime->tm_mon + 1;
    int day = reTime->tm_mday;
    return year * 10000 + month * 100 + day;
}
int Videoclient::overduetime(int rt) {
    int year, month, day;
    int rentedtime = rt;
    day = rentedtime % 100;
    rentedtime /= 100;
    month = rentedtime % 100;
    rentedtime /= 100;
    year = rentedtime;

    time_t start, end;
    struct tm user_stime;
    int tm_day, tm_hour, tm_min, tm_sec;
    double diff;

    user_stime.tm_year = year - 1900;
    user_stime.tm_mon = month - 1;
    user_stime.tm_mday = day;
    user_stime.tm_hour = 0;//
    user_stime.tm_min = 0;
    user_stime.tm_sec = 0;

    start = mktime(&user_stime);
    time(&end);

    diff = difftime(end, start);
    return diff /= 86400;
}

string Videoclient::assignHyphenRule(string callnum) {
    string assigned = callnum;
    if ((callnum.length() == 9 || callnum.length() == 10) && strncmp(callnum.c_str(), "02", 2) == 0) {
        assigned.insert(2, "-");
        assigned.insert((assigned.length() - 4), "-");
    }

    else if (callnum.length() == 11 || callnum.length() == 10) {
        assigned.insert(3, "-");
        assigned.insert((assigned.length() - 4), "-");
    }
    return assigned;
}

void Videoclient::chkIn() {
    string username; string vidname; int i = 0; int fee_ = 0; int userid; int vidid;
    cout << "회원전화번호(ID)를 입력";
    cin >> username;
    if (searchUser(username) != INT_MAX) userid = searchUser(username); else { cout << "유효하지않은 이름입니다" << endl; return; }
    for (; 0 < userDB[userid].nofRent; userDB[userid].nofRent--, i++) {
    vidinput:
        cout << userDB[userid].uname << "님의 남은 대출가능수는" << userDB[userid].nofRent << "입니다" << endl << "대출하실 비디오의 바코드를 입력해주세요. 0을 눌러 대출을 종료합니다.";
        cin >> vidname;
        if (vidname == "0")break;
        if (searchVid(vidname) != INT_MAX) vidid = searchVid(vidname); else { cout << "유효하지않은 이름입니다" << endl; goto vidinput; }
        if (vidDB[vidid].dateRent > 0) {//??????
            cout << "이미 대출중인 영상입니다.";
            goto vidinput;
        }
        if (vidDB[vidid].rRate == true && userDB[userid].birth > nowtime() - 190000) {///////???γ?)
            cout << "미성년자는 관람 불가능한 비디오입니다.";
            goto vidinput;
        }
        vidDB[vidid].dateRent = aftime(); ////////////////////////////////////////??￥
        vidDB[vidid].uname = userDB[userid].telnum;
        cout << vidDB[vidid].vname << "대출완료" << endl;

    }
    cout << "총" << i << "권 대출완료" << "반납예정일은" << aftime() << "까지입니다" << endl << "대출료는" << i * fee << "입니다.";
}

void Videoclient::chkOut() {
    string vidname; int vidid; int i = 0;
    while (1) {
    chkout_:
        cout << "반납하실 비디오의 바코드(이름)를 입력해주세요. 0을 눌러 반납을 종료합니다." << endl;
        cin >> vidname;
        if (vidname == "0")break;
        if (searchVid(vidname) != INT_MAX) vidid = searchVid(vidname); else { cout << "유효하지않은 이름입니다" << endl; goto chkout_; }
        if (vidDB[vidid].dateRent == 0) { cout << "대출중인 영상이 아닙니다" << endl; goto chkout_; }
        int ovrDue = overduetime(vidDB[vidid].dateRent);
        userDB[searchUser(vidDB[vidid].uname)].nofRent++;///////////////////////////////////////////////////////////????o?????????
        cout << vidDB[vidid].uname << "님의 남은 대출권수는" << remainRent - userDB[searchUser(vidDB[vidid].uname)].nofRent << "입니다";
        if (ovrDue > 0) cout << ovrDue << "일 연체입니다" << endl << "연체료는" << fine * ovrDue << "입니다.";
        vidDB[vidid].uname = "0"; vidDB[vidid].dateRent = 0;
    }
}

void Videoclient::delVid(int index) {//?ε???????????????
    if (index == INT_MAX) { cout << "유효하지않은 이름입니다" << endl; return; }
    if (vidDB[index].dateRent > 0) { cout << "대출중인 영상은 삭제 불가합니다"; return; }
    vidDB.erase(vidDB.begin() + index);
    printf("삭제완료\n");
    return;
}

void Videoclient::delUser(int index) {//?ε???????????????
    if (index == INT_MAX) { cout << "유효하지않은 이름입니다" << endl; return; }
    if (userDB[index].nofRent != remainRent) { cout << "대출중인 회원은 삭제 불가합니다"; return; }
    userDB.erase(userDB.begin() + index);
    printf("삭제완료\n");
    return;
}

void Videoclient::newVid(string na, bool nf) {//?????????
    if (searchVid(na) != INT_MAX) { cout << "중복되는 영상은 등록 불가합니다" << endl; return; }
    if (na.empty()) {
        printf("이름은 비워둘수없습니다");
        return;
    }
    vidDB.push_back(video(na, "", nf, 0, (vidDB.back().Vid) + 1));
}

void Videoclient::newUser(string na, string te, string br, int nr) {//?????????
    if (searchUser(te) != INT_MAX) { cout << "전화번호가 중복되는 회원은 등록불가합니다" << endl; return; }
    if (na.empty()) {
        printf("이름은 비워둘 수 없습니다");
        return;
    }
    if (te.empty())
        printf("전화번호는 비워둘 수 없습니다");
    if (atoi(te.c_str()) == 0)
        printf("전화번호는 문자나 '-'없이 번호만 입력해주세요(입력오류)");
    if (atoi(br.c_str()) == 0)
        printf("생일은 문자나 '.'없이 번호만 입력해주세요(입력오류)");

    userDB.push_back(user(na, te, atoi(br.c_str()), remainRent, (userDB.back().Uid) + 1));
}

void Videoclient::editVid(int index) {  //??????????
    if (index == INT_MAX) { cout << "유효하지않은 이름입니다" << endl; return; }
    //?ε??????
    int ctrl;
    cout << "수정할 항목을 선택하십시오" << endl
        << "1. 비디오이름: " << vidDB[index].vname << endl
        << "2. 청소년시청불가여부(1은 청소년 시청불가입니다)" << vidDB[index].rRate << endl;
    cin >> ctrl;
    if ((ctrl < 1) || (ctrl > 2)) {
        printf("이름은 비워둘수없습니다\n");
        cin.clear();//????????
        cin.ignore(100, '\n');//?????? ?????????? ????? ????? 
        return;
    }
    switch (ctrl) {
    case 1:
    confirmName:
        printf("\nname: ");
        cin >> vidDB[index].vname;
        if ((vidDB[index].vname).empty()) {
            printf("이름은 비워둘수없습니다");
            goto confirmName;
        }
        break;
    case 2:
        int nf;
    confirmrRate:
    renumber:
        cout << "청소년 시청 불가 입니까?\n1.예\n2.아니요";
        cin >> nf;
        if (nf == 1)
            vidDB[index].rRate = true;
        else if (nf == 2)
            vidDB[index].rRate = false;
        else {
            cout << "잘못입력했습니다";
            goto confirmrRate;
        }
        break;
    case 3:
        cout << "잘못입력했습니다";
        return;
        break;
    default:
        cout << "잘못입력했습니다";
        return;
    }
    printf("변경완료\n");
    return;
}

void Videoclient::editUser(int index) {  //??????????
    if (index == INT_MAX) { cout << "유효하지않은 이름입니다" << endl; return; }
    //?ε??????
    int ctrl;
    cout << "수정할 항목을 선택하십시오" << endl
        << "1. 성명: " << userDB[index].uname << endl
        << "2. 전화번호: " << userDB[index].telnum << endl
        << "3. 생년월일: " << userDB[index].birth << endl;
    cin >> ctrl;
    if ((ctrl < 1) || (ctrl > 3)) {
        printf("잘못된 입력입니다.\n");
        cin.clear();//????????
        cin.ignore(100, '\n');//?????? ?????????? ????? ????? 
        return;
    }
    switch (ctrl) {
    case 1:
    rename:
        printf("\nname: ");
        cin >> userDB[index].uname;
        if ((userDB[index].uname).empty()) {
            printf("이름은 비워둘수없습니다");
            goto rename;
        }
        break;
    case 2:
    renumber:
        printf("\ntelnum: ");
        cin >> userDB[index].telnum;
        if ((userDB[index].telnum).empty()) {
            printf("경고: 전화번호가 없습니다.");
        }
        break;
    case 3:
        printf("\nbirth: ");
        cin >> userDB[index].birth;//////////////////////////////////////???? ????? o?????
        break;
    default:
        cout << "잘못입력했습니다.";
        return;
    }
    printf("변경완료\n");
    return;
}

int Videoclient::searchVid(string kwd) {
    for (int i = 0; i < vidDB.size(); i++) {
        if (vidDB[i].vname == kwd) {
            return i;
        }
    }

    return INT_MAX;
}

int Videoclient::searchUser(string kwd) {
    for (int i = 0; i < userDB.size(); i++) {
        if (userDB[i].telnum == kwd) {
            return i;
        }
    }

    return INT_MAX;
}

void Videoclient::searchUserList(string kwd) {
    int cfind = 0;
    cout << setw(10) << "이름" << setw(20) << "전화번호" << setw(18) << "생년월일" << setw(8) << "대출가능수" << setw(18) << "고유번호" << endl;
    for (int i = 0; i < userDB.size(); i++) {
        if (userDB[i].uname == kwd || userDB[i].telnum == kwd || "*" == kwd) {
            cout << setw(10) << userDB[i].uname << setw(20) << assignHyphenRule(userDB[i].telnum) << setw(18) << userDB[i].birth << setw(8) << userDB[i].nofRent << setw(18) << userDB[i].Uid << endl;
            cfind++;
        }
    }
    if (cfind == 0)
        cout << "해당 이름이나 전화번호에 해당되는 데이터를 조회할수없습니다\n";
}

void Videoclient::searchVidList(string kwd) {
    int cfind = 0;
    cout << "영화이름                               " << "청소년 관람가       " << "대출자명       " << "대출기간  " << "고유번호       " << endl;
    for (int i = 0; i < vidDB.size(); i++) {
        if (vidDB[i].vname == kwd || "*" == kwd) {
            string adult = (vidDB[i].rRate) ? "true" : "false";
            cout << setw(40) << vidDB[i].vname << setw(18) << adult << setw(18) << vidDB[i].uname << setw(8) << vidDB[i].dateRent << setw(18) << vidDB[i].Vid << endl;
            cfind++;
        }
    }
    if (cfind == 0)
        cout << "해당 이름에 해당되는 데이터를 조회할수없습니다\n";
}

void Videoclient::printAll() {//??????? ???????? ??????
    int i = 0;
    cout.precision(3);
    cout.setf(ios::left);
    cout << endl
        << endl;

    cout << setw(30) << "비디오이름" << setw(30) << "반납예정일" << setw(30) << "대출자" << endl;

    while (i < vidDB.size()) {
        if (vidDB[i].dateRent != 0)
            cout << setw(30) << vidDB[i].vname << setw(30) << vidDB[i].dateRent << setw(30) << vidDB[i].uname << endl;
        i++;
    }
    cout << setw(0);
    cout << endl;
}

void Videoclient::init() {
    vidDB.reserve(10000); //naverworks?? ????o ??? ???
    userDB.reserve(10000);
    ifstream is_data;//???????: EUC-KR?? ?????? ???????
    is_data.open("settings.txt"); //???????????? ???????
    if (!is_data.is_open() || is_data.bad()) {
        printf("설정 데이터 파일을 찾을수없습니다.\n저장경로가 다르거나 예상치못한 종료로 인해\n 파일이 손상되었을수도있습니다\n");
        return;
    }
    else {
        while (!is_data.eof()) {
            if (is_data.eof())
                break; //SOF??????? eof?????α?????(eof?????u?????? ?????μ?°? ????o??)
            is_data >> remainTime >> remainRent >> fine >> fee;

        }
        is_data.close();
    }

    ifstream iv_data;
    video importedVData;
    iv_data.open("vidDB.txt"); //???????????? ???????
    if (!iv_data.is_open() || iv_data.bad()) {
        printf("비디오 데이터 파일을 찾을수없습니다.\n저장경로가 다르거나 예상치못한 종료로 인해\n 파일이 손상되었을수도있습니다\n최초 실행의 경우 계속 진행하셔도 좋습니다.\n");
        return;
    }
    else {
        while (!iv_data.eof()) {
            if (iv_data.eof())
                break; //SOF??????? eof?????α?????(eof?????u?????? ?????μ?°? ????o??)
            iv_data >> importedVData.vname >> importedVData.uname >> importedVData.rRate >> importedVData.dateRent >> importedVData.Vid;
            if (!importedVData.vname.empty()) {
                if (vidDB.size() >= 1) {
                    if (importedVData.vname != (vidDB.back().vname))
                        vidDB.push_back(importedVData);
                }
                else
                    vidDB.push_back(importedVData);
            }
        }
        iv_data.close();
    }
    //sorting(1);

    ifstream iu_data;
    user importedUData;
    iu_data.open("userDB.txt"); //???????????? ???????
    if (!iu_data.is_open() || iu_data.bad()) {
        printf("회원 데이터 파일을 찾을수없습니다.\n저장경로가 다르거나 예상치못한 종료로 인해\n 파일이 손상되었을수도있습니다\n최초 실행의 경우 계속 진행하셔도 좋습니다.");
        return;
    }
    else {
        while (!iu_data.eof()) {
            if (iu_data.eof())
                break; //SOF??????? eof?????α?????(eof?????u?????? ?????μ?°? ????o??)
            iu_data >> importedUData.uname >> importedUData.telnum >> importedUData.nofRent >> importedUData.birth >> importedUData.Uid;
            if (!importedUData.uname.empty()) {
                if (userDB.size() >= 1) {
                    if (importedUData.uname != (userDB.back().uname))
                        userDB.push_back(importedUData);
                }
                else
                    userDB.push_back(importedUData);
            }
        }
        iu_data.close();
    }
    //sorting(1);
}

void Videoclient::terminate() {//???????????
    ofstream so_data;
    so_data.open("settings.txt");
    so_data << remainTime;
    so_data << endl;
    so_data << remainRent;
    so_data << endl;
    so_data << fine;
    so_data << endl;
    so_data << fee;
    so_data << endl;
    so_data.close();
    ofstream vo_data;
    vo_data.open("vidDB.txt");
    for (int i = 0; i < vidDB.size(); i++) {
        string vname = (vidDB.at(i)).vname;
        vo_data.write(vname.c_str(), vname.size());
        vo_data << endl;
        string uname = (vidDB.at(i)).uname;
        vo_data.write(uname.c_str(), uname.size());
        vo_data << endl;
        bool rRate = (vidDB.at(i).rRate);
        vo_data << rRate;
        vo_data << endl;
        int dateRent = (vidDB.at(i).dateRent);
        vo_data << dateRent;
        vo_data << endl;
        int vid = (vidDB.at(i).Vid);
        vo_data << vid;
        vo_data << endl;
    }
    vo_data.close();
    ofstream uo_data;
    uo_data.open("userDB.txt");
    for (int i = 0; i < userDB.size(); i++) {
        string uname = (userDB.at(i)).uname;
        uo_data.write(uname.c_str(), uname.size());
        uo_data << endl;
        string telnum = (userDB.at(i)).telnum;
        uo_data.write(telnum.c_str(), telnum.size());
        uo_data << endl;
        int nofRent = (userDB.at(i).nofRent);
        uo_data << nofRent;
        uo_data << endl;
        int birth = (userDB.at(i).birth);
        uo_data << birth;
        uo_data << endl;
        int Uid = (userDB.at(i).Uid);
        uo_data << Uid;
        uo_data << endl;
    }
    uo_data.close();
    exit(0);
}
bool Videoclient::getMode() {//????? ????????
    return false;
}

void Videoclient::setSysVar() {
    int ctrl, i = 0; int p_remainRent, p_remainTime, p_fee, p_fine, gap, pre_remainRent;
    cout << "현재 사용중인 대출가능기간은" << remainTime << endl
        << "대출 가능 비디오 수는" << remainRent << endl
        << "연체료는" << fine << "원 입니다" << endl
        << "대출료는" << fee << "원 입니다" << endl
        << "수정할 항목을 선택해 주십시오" << endl
        << "1:대출기간" << "2:대출가능수" << "3:연체료" << "4:대출료" << endl;
    int set = 0;
    cin >> set;
    if ((set < 1) || (set > 4)) {
        printf("잘못 입력하였습니다.\n");
        cin.clear();//????????
        cin.ignore(100, '\n');//?????? ?????????? ????? ????? 
        return;
    }
    switch (set)
    {
    case 1:
        cout << "새 대출기간은 변경 이후 대출되는 영상부터 적용됩니다" << endl << "새 대출기간:";
        cin >> p_remainTime;
        if (p_remainTime < 0) { cout << "잘못 입력하였습니다."; return; }
        else remainTime = p_remainTime;
        break;
    case 2:
        cout << "새 대출가능 수";
        cin >> p_remainRent;
        if (p_remainRent < 0) { cout << "잘못 입력하였습니다."; return; }
        else pre_remainRent = remainRent; remainRent = p_remainRent;
        gap = remainRent - pre_remainRent;// 5 3 ++2  // 2 3 -1??????
        while (i < userDB.size()) {
            if ((userDB[i].nofRent + gap) < 0)
                userDB[i].nofRent = 0;
            else { userDB[i].nofRent += gap; }
            i++;
        }

        break;
    case 3:
        cout << "새 연체료";
        cin >> p_fine;
        if (p_fine < 0) { cout << "잘못 입력하였습니다."; return; }
        else fine = p_fine;
        break;
    case 4:
        cout << "새 대출료";
        cin >> p_fee;
        if (p_fee < 0) { cout << "잘못 입력하였습니다."; return; }
        else fee = p_fee;
        break;
    }
}