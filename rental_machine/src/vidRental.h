#pragma once
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

class user {
public:
    user() {
        uname = telnum  = "지정되지않음";birth = 0; Uid=nofRent = 5;
    }
    user(string na, string te, int br, int  nr, int id) {
        uname = na;
        telnum = te;
        birth = br;
        nofRent = nr;
        Uid = id;
    }
    string uname;
    string telnum;
    int birth;
    int nofRent;
    int Uid;
};

class video {
public:
    video() {
        uname,vname = "지정되지않음";rRate = false;Vid=dateRent = 0;
    }
    video(string vn, string un, bool nf, int dr,int id) {
        vname = vn;
        rRate = nf;
        dateRent = dr;
        uname = un;
        Vid = id;
    }
    string vname;
    string uname;
    bool rRate;
    int dateRent;
    int Vid;
};

class Videoclient {
public:
    Videoclient() { srchMod = false; remainTime = 7; remainRent = 3; fine = 500; sortvar = true; fee = 1000; };
    ~Videoclient() {};
    
//system
    void init();                                                                   //데이터불러오기
    void terminate();                                                          //파일저장후 클라이언트 종료
    
//dataCtrl
    void newVid(string na, bool nf);                                   //새 비디오
    void newUser(string na, string te, string br, int  nr);          //새 유저
    void editVid(int index) ;                                                  //기존데이터수정 비디오
    void editUser(int index);                                                //기존데이터수정 유저
    void delVid(int index);                                                   //삭제 비디오
    void delUser(int index);                                                 //삭제 유저
    //검색결과있을시 인덱스를 반환 
    int searchVid(string kwd);                                         //비디오 검색    
    int searchUser(string kwd);                                       //유저 검색
    //검색결과있을시 정보 출력 *를 사용해 모든 정보 출력 
    void searchVidList(string kwd);                                         //비디오리스트 검색    
    void searchUserList(string kwd);                                       //유저리스트 검색

//rentalWork
    void chkIn();                                                    //남은 대출가능수 고려
    void chkOut();

//utility
    void setSysVar();         //반납기간 대출수 연체금 조작가능 디폴트(7, 3, 500)
    void printAll();                                                             //메인 대출현황 조회 ||| 이름(전화번호) / 대출가능수 / 반납(연체)금->반납일
    bool getMode();                                                          //모드확인
    string assignHyphenRule(string callnum);                  //전화번호 하이픈룰적용
    //void delinquentLookup();                                             //연체자조회 일괄 문자발송(가정)
    int nowtime();
    int aftime();
    int overduetime(int rentedtime);
private:
    bool srchMod;
    int remainTime; 
    int remainRent; 
    int fine;
    bool sortvar = 1;
    int fee;
    vector<user> userDB;
    vector<video> vidDB;
    vector<user> usersrchBuffer;                                      
    vector<video> videosrchBuffer;
    
};


