#pragma once
#include <bits/stdc++.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

class node {
public:
    node() {
        name = telnum = s_telnum = group = L"지정되지않음";
        group_index = name_index = '*';
    }
    node(wstring na, wstring te, wstring ste, wstring gr, wchar_t nind, wchar_t gind) {
        name = na;
        telnum = te;
        s_telnum = ste;
        group = gr;
        name_index = nind;
        group_index = gind;
    }
    wstring name;
    wstring telnum;
    wstring s_telnum;
    wstring group;
    wchar_t name_index;
    wchar_t group_index;
};

class Book {
public:
    Book() {};
    ~Book() {};
    //void delDatabyString(string var);
    //void serchbyString(string var);
    void terminate();         //파일저장후종료
    void newData();           //새 데이터
    void editData(int index); //기존데이터수정 현재 작성중
    void sorting(int piv);    //정렬기준변경, 적용
    void delData(int index);  //데이터삭제
    void search(wstring kwd);   //탐색및 탐색버퍼에 데이터 저장    
    void printall();          //전체출력
    void init();              //데이터불러오기
    void srchclear();         //탐색모드에서 복귀
    bool getMode();           //모드확인
    void setMode(bool mode);  //탐색/메인모드변경
    wstring assignHyphenRule(wstring callnum);//하이픈룰적용
private:
    bool srchMod = false;
    bool sortvar = 1;                      //시간여유되면reverse써서 오름차 내림차도 구현
    vector<node> context;                 //자료구조는 구조체벡터를 사용한 딕셔너리 사용. 벡터는 랜덤액세스의 장점을 가지며 가장 자신있는 stl이기에.. 맵을 사용해보려고했으나 데이터 입력, 삭제로 자꾸 sort가 되기에..
    vector<node> srchBuffer;           //검색시 사용되는벡터로 컨텍스트의 참조포인터일예정이였으나 모드포인터가 auto가 안되서 주먹구구식으로..
    vector<node>* modePointer = &context; //기본 컨텍스트로 설정되며 작업영역을 나타낸다
};