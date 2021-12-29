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
    cout << "ȸ����ȭ��ȣ(ID)�� �Է�";
    cin >> username;
    if (searchUser(username) != INT_MAX) userid = searchUser(username); else { cout << "��ȿ�������� �̸��Դϴ�" << endl; return; }
    for (; 0 < userDB[userid].nofRent; userDB[userid].nofRent--, i++) {
    vidinput:
        cout << userDB[userid].uname << "���� ���� ���Ⱑ�ɼ���" << userDB[userid].nofRent << "�Դϴ�" << endl << "�����Ͻ� ������ ���ڵ带 �Է����ּ���. 0�� ���� ������ �����մϴ�.";
        cin >> vidname;
        if (vidname == "0")break;
        if (searchVid(vidname) != INT_MAX) vidid = searchVid(vidname); else { cout << "��ȿ�������� �̸��Դϴ�" << endl; goto vidinput; }
        if (vidDB[vidid].dateRent > 0) {//??????
            cout << "�̹� �������� �����Դϴ�.";
            goto vidinput;
        }
        if (vidDB[vidid].rRate == true && userDB[userid].birth > nowtime() - 190000) {///////???��?)
            cout << "�̼����ڴ� ���� �Ұ����� �����Դϴ�.";
            goto vidinput;
        }
        vidDB[vidid].dateRent = aftime(); ////////////////////////////////////////??��
        vidDB[vidid].uname = userDB[userid].telnum;
        cout << vidDB[vidid].vname << "����Ϸ�" << endl;

    }
    cout << "��" << i << "�� ����Ϸ�" << "�ݳ���������" << aftime() << "�����Դϴ�" << endl << "������" << i * fee << "�Դϴ�.";
}

void Videoclient::chkOut() {
    string vidname; int vidid; int i = 0;
    while (1) {
    chkout_:
        cout << "�ݳ��Ͻ� ������ ���ڵ�(�̸�)�� �Է����ּ���. 0�� ���� �ݳ��� �����մϴ�." << endl;
        cin >> vidname;
        if (vidname == "0")break;
        if (searchVid(vidname) != INT_MAX) vidid = searchVid(vidname); else { cout << "��ȿ�������� �̸��Դϴ�" << endl; goto chkout_; }
        if (vidDB[vidid].dateRent == 0) { cout << "�������� ������ �ƴմϴ�" << endl; goto chkout_; }
        int ovrDue = overduetime(vidDB[vidid].dateRent);
        userDB[searchUser(vidDB[vidid].uname)].nofRent++;///////////////////////////////////////////////////////////????o?????????
        cout << vidDB[vidid].uname << "���� ���� ����Ǽ���" << remainRent - userDB[searchUser(vidDB[vidid].uname)].nofRent << "�Դϴ�";
        if (ovrDue > 0) cout << ovrDue << "�� ��ü�Դϴ�" << endl << "��ü���" << fine * ovrDue << "�Դϴ�.";
        vidDB[vidid].uname = "0"; vidDB[vidid].dateRent = 0;
    }
}

void Videoclient::delVid(int index) {//?��???????????????
    if (index == INT_MAX) { cout << "��ȿ�������� �̸��Դϴ�" << endl; return; }
    if (vidDB[index].dateRent > 0) { cout << "�������� ������ ���� �Ұ��մϴ�"; return; }
    vidDB.erase(vidDB.begin() + index);
    printf("�����Ϸ�\n");
    return;
}

void Videoclient::delUser(int index) {//?��???????????????
    if (index == INT_MAX) { cout << "��ȿ�������� �̸��Դϴ�" << endl; return; }
    if (userDB[index].nofRent != remainRent) { cout << "�������� ȸ���� ���� �Ұ��մϴ�"; return; }
    userDB.erase(userDB.begin() + index);
    printf("�����Ϸ�\n");
    return;
}

void Videoclient::newVid(string na, bool nf) {//?????????
    if (searchVid(na) != INT_MAX) { cout << "�ߺ��Ǵ� ������ ��� �Ұ��մϴ�" << endl; return; }
    if (na.empty()) {
        printf("�̸��� ����Ѽ������ϴ�");
        return;
    }
    vidDB.push_back(video(na, "", nf, 0, (vidDB.back().Vid) + 1));
}

void Videoclient::newUser(string na, string te, string br, int nr) {//?????????
    if (searchUser(te) != INT_MAX) { cout << "��ȭ��ȣ�� �ߺ��Ǵ� ȸ���� ��ϺҰ��մϴ�" << endl; return; }
    if (na.empty()) {
        printf("�̸��� ����� �� �����ϴ�");
        return;
    }
    if (te.empty())
        printf("��ȭ��ȣ�� ����� �� �����ϴ�");
    if (atoi(te.c_str()) == 0)
        printf("��ȭ��ȣ�� ���ڳ� '-'���� ��ȣ�� �Է����ּ���(�Է¿���)");
    if (atoi(br.c_str()) == 0)
        printf("������ ���ڳ� '.'���� ��ȣ�� �Է����ּ���(�Է¿���)");

    userDB.push_back(user(na, te, atoi(br.c_str()), remainRent, (userDB.back().Uid) + 1));
}

void Videoclient::editVid(int index) {  //??????????
    if (index == INT_MAX) { cout << "��ȿ�������� �̸��Դϴ�" << endl; return; }
    //?��??????
    int ctrl;
    cout << "������ �׸��� �����Ͻʽÿ�" << endl
        << "1. �����̸�: " << vidDB[index].vname << endl
        << "2. û�ҳ��û�Ұ�����(1�� û�ҳ� ��û�Ұ��Դϴ�)" << vidDB[index].rRate << endl;
    cin >> ctrl;
    if ((ctrl < 1) || (ctrl > 2)) {
        printf("�̸��� ����Ѽ������ϴ�\n");
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
            printf("�̸��� ����Ѽ������ϴ�");
            goto confirmName;
        }
        break;
    case 2:
        int nf;
    confirmrRate:
    renumber:
        cout << "û�ҳ� ��û �Ұ� �Դϱ�?\n1.��\n2.�ƴϿ�";
        cin >> nf;
        if (nf == 1)
            vidDB[index].rRate = true;
        else if (nf == 2)
            vidDB[index].rRate = false;
        else {
            cout << "�߸��Է��߽��ϴ�";
            goto confirmrRate;
        }
        break;
    case 3:
        cout << "�߸��Է��߽��ϴ�";
        return;
        break;
    default:
        cout << "�߸��Է��߽��ϴ�";
        return;
    }
    printf("����Ϸ�\n");
    return;
}

void Videoclient::editUser(int index) {  //??????????
    if (index == INT_MAX) { cout << "��ȿ�������� �̸��Դϴ�" << endl; return; }
    //?��??????
    int ctrl;
    cout << "������ �׸��� �����Ͻʽÿ�" << endl
        << "1. ����: " << userDB[index].uname << endl
        << "2. ��ȭ��ȣ: " << userDB[index].telnum << endl
        << "3. �������: " << userDB[index].birth << endl;
    cin >> ctrl;
    if ((ctrl < 1) || (ctrl > 3)) {
        printf("�߸��� �Է��Դϴ�.\n");
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
            printf("�̸��� ����Ѽ������ϴ�");
            goto rename;
        }
        break;
    case 2:
    renumber:
        printf("\ntelnum: ");
        cin >> userDB[index].telnum;
        if ((userDB[index].telnum).empty()) {
            printf("���: ��ȭ��ȣ�� �����ϴ�.");
        }
        break;
    case 3:
        printf("\nbirth: ");
        cin >> userDB[index].birth;//////////////////////////////////////???? ????? o?????
        break;
    default:
        cout << "�߸��Է��߽��ϴ�.";
        return;
    }
    printf("����Ϸ�\n");
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
    cout << setw(10) << "�̸�" << setw(20) << "��ȭ��ȣ" << setw(18) << "�������" << setw(8) << "���Ⱑ�ɼ�" << setw(18) << "������ȣ" << endl;
    for (int i = 0; i < userDB.size(); i++) {
        if (userDB[i].uname == kwd || userDB[i].telnum == kwd || "*" == kwd) {
            cout << setw(10) << userDB[i].uname << setw(20) << assignHyphenRule(userDB[i].telnum) << setw(18) << userDB[i].birth << setw(8) << userDB[i].nofRent << setw(18) << userDB[i].Uid << endl;
            cfind++;
        }
    }
    if (cfind == 0)
        cout << "�ش� �̸��̳� ��ȭ��ȣ�� �ش�Ǵ� �����͸� ��ȸ�Ҽ������ϴ�\n";
}

void Videoclient::searchVidList(string kwd) {
    int cfind = 0;
    cout << "��ȭ�̸�                               " << "û�ҳ� ������       " << "�����ڸ�       " << "����Ⱓ  " << "������ȣ       " << endl;
    for (int i = 0; i < vidDB.size(); i++) {
        if (vidDB[i].vname == kwd || "*" == kwd) {
            string adult = (vidDB[i].rRate) ? "true" : "false";
            cout << setw(40) << vidDB[i].vname << setw(18) << adult << setw(18) << vidDB[i].uname << setw(8) << vidDB[i].dateRent << setw(18) << vidDB[i].Vid << endl;
            cfind++;
        }
    }
    if (cfind == 0)
        cout << "�ش� �̸��� �ش�Ǵ� �����͸� ��ȸ�Ҽ������ϴ�\n";
}

void Videoclient::printAll() {//??????? ???????? ??????
    int i = 0;
    cout.precision(3);
    cout.setf(ios::left);
    cout << endl
        << endl;

    cout << setw(30) << "�����̸�" << setw(30) << "�ݳ�������" << setw(30) << "������" << endl;

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
        printf("���� ������ ������ ã���������ϴ�.\n�����ΰ� �ٸ��ų� ����ġ���� ����� ����\n ������ �ջ�Ǿ��������ֽ��ϴ�\n");
        return;
    }
    else {
        while (!is_data.eof()) {
            if (is_data.eof())
                break; //SOF??????? eof?????��?????(eof?????u?????? ?????��?��? ????o??)
            is_data >> remainTime >> remainRent >> fine >> fee;

        }
        is_data.close();
    }

    ifstream iv_data;
    video importedVData;
    iv_data.open("vidDB.txt"); //???????????? ???????
    if (!iv_data.is_open() || iv_data.bad()) {
        printf("���� ������ ������ ã���������ϴ�.\n�����ΰ� �ٸ��ų� ����ġ���� ����� ����\n ������ �ջ�Ǿ��������ֽ��ϴ�\n���� ������ ��� ��� �����ϼŵ� �����ϴ�.\n");
        return;
    }
    else {
        while (!iv_data.eof()) {
            if (iv_data.eof())
                break; //SOF??????? eof?????��?????(eof?????u?????? ?????��?��? ????o??)
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
        printf("ȸ�� ������ ������ ã���������ϴ�.\n�����ΰ� �ٸ��ų� ����ġ���� ����� ����\n ������ �ջ�Ǿ��������ֽ��ϴ�\n���� ������ ��� ��� �����ϼŵ� �����ϴ�.");
        return;
    }
    else {
        while (!iu_data.eof()) {
            if (iu_data.eof())
                break; //SOF??????? eof?????��?????(eof?????u?????? ?????��?��? ????o??)
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
    cout << "���� ������� ���Ⱑ�ɱⰣ��" << remainTime << endl
        << "���� ���� ���� ����" << remainRent << endl
        << "��ü���" << fine << "�� �Դϴ�" << endl
        << "������" << fee << "�� �Դϴ�" << endl
        << "������ �׸��� ������ �ֽʽÿ�" << endl
        << "1:����Ⱓ" << "2:���Ⱑ�ɼ�" << "3:��ü��" << "4:�����" << endl;
    int set = 0;
    cin >> set;
    if ((set < 1) || (set > 4)) {
        printf("�߸� �Է��Ͽ����ϴ�.\n");
        cin.clear();//????????
        cin.ignore(100, '\n');//?????? ?????????? ????? ????? 
        return;
    }
    switch (set)
    {
    case 1:
        cout << "�� ����Ⱓ�� ���� ���� ����Ǵ� ������� ����˴ϴ�" << endl << "�� ����Ⱓ:";
        cin >> p_remainTime;
        if (p_remainTime < 0) { cout << "�߸� �Է��Ͽ����ϴ�."; return; }
        else remainTime = p_remainTime;
        break;
    case 2:
        cout << "�� ���Ⱑ�� ��";
        cin >> p_remainRent;
        if (p_remainRent < 0) { cout << "�߸� �Է��Ͽ����ϴ�."; return; }
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
        cout << "�� ��ü��";
        cin >> p_fine;
        if (p_fine < 0) { cout << "�߸� �Է��Ͽ����ϴ�."; return; }
        else fine = p_fine;
        break;
    case 4:
        cout << "�� �����";
        cin >> p_fee;
        if (p_fee < 0) { cout << "�߸� �Է��Ͽ����ϴ�."; return; }
        else fee = p_fee;
        break;
    }
}