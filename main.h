
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;
std::string fileName;

vector<int>LatenyDiff;

struct PacketData
{
	string Time;
	string OrderType;
	string Token;
	string price;
	string Qty;
	string Action;
};

struct PrintData
{
	string OrderReqToken;
	string OrderReqPrice;
	string OrderReqTime;
	string OrderMsgTime;
	string OrderMsgPrice;	
	string OrderMsgAction;
	int Differnce;
};
void FilledStructure(PacketData& stData, vector<string>line);
void read_record();
void fnprintData(const PrintData stPdata,int Difference);
void usage();
void MinMaxAvg(vector<int>);
int getMin(vector<int>, int n);
int getMax(vector<int>, int n);
int calcAvg(vector<int>, int n);
