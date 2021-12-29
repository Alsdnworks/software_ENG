#pragma once
#include <bits/stdc++.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

class node {
public:
    node() {
        name = telnum = s_telnum = group = L"������������";
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
    void terminate();         //��������������
    void newData();           //�� ������
    void editData(int index); //���������ͼ��� ���� �ۼ���
    void sorting(int piv);    //���ı��غ���, ����
    void delData(int index);  //�����ͻ���
    void search(wstring kwd);   //Ž���� Ž�����ۿ� ������ ����    
    void printall();          //��ü���
    void init();              //�����ͺҷ�����
    void srchclear();         //Ž����忡�� ����
    bool getMode();           //���Ȯ��
    void setMode(bool mode);  //Ž��/���θ�庯��
    wstring assignHyphenRule(wstring callnum);//�����·�����
private:
    bool srchMod = false;
    bool sortvar = 1;                      //�ð������Ǹ�reverse�Ἥ ������ �������� ����
    vector<node> context;                 //�ڷᱸ���� ����ü���͸� ����� ��ųʸ� ���. ���ʹ� �����׼����� ������ ������ ���� �ڽ��ִ� stl�̱⿡.. ���� ����غ����������� ������ �Է�, ������ �ڲ� sort�� �Ǳ⿡..
    vector<node> srchBuffer;           //�˻��� ���Ǵº��ͷ� ���ؽ�Ʈ�� �����������Ͽ����̿����� ��������Ͱ� auto�� �ȵǼ� �ָԱ���������..
    vector<node>* modePointer = &context; //�⺻ ���ؽ�Ʈ�� �����Ǹ� �۾������� ��Ÿ����
};