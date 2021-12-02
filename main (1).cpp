#include"main.h"

void FilledStructure(PacketData &stData,vector<string>line)
{
	stData.Time.assign(line[0]);
	stData.OrderType.assign(line[1]);
	stData.Token.assign(line[2]);
	stData.price.assign(line[3]);
	stData.Qty.assign(line[4]);
	stData.Action.assign(line[5]);
}

void read_record()
{

	// File pointer
	fstream fin;

	// Open an existing file
	fin.open(fileName.c_str(), ios::in);
	if (fin.is_open())
	{		
		vector<string> row;
		vector<PacketData> pktdata;
		pktdata.clear();		
		LatenyDiff.clear();
		int diffrence;
		string line, word;
		cout << "					________________________________________________________________________________________________"<<endl;

		cout <<"					"<<setw(10) << left << "| Token" << setw(16) << "| Price_Token" << setw(17) << "| Ord_Req_Time" << setw(17)
			 << "| Ord_Msg_Time" << setw(10)<< "| Price" << setw(10)<< "| Action |" << setw(5)<< " = " << setw(7) << "Difference|"
			 <<std::endl;
		cout << "					------------------------------------------------------------------------------------------------"<<endl;
		while (std::getline(fin, line)) {
			row.clear();
			diffrence = 0;
			stringstream s(line);
			while (getline(s, word, ',')) {				
				row.push_back(word);				
			}			
			if (!strcmp(row[0].c_str(), "TIME"))
			{
				continue;
			}			
			PacketData stData;
			FilledStructure(stData, row);			
			pktdata.push_back(stData);			
			if (!strcmp(stData.OrderType.c_str(), "NEW_ORDER"))
			{			
				for (int reversecount=(int)(pktdata.size()-1);reversecount>= (pktdata.size()-1)-5; reversecount--)
				{										
					if (pktdata[reversecount].Token== stData.Token && pktdata[reversecount].OrderType =="ORDER_MESSAGE"
						&& stoi(pktdata[reversecount].price)<= stoi(stData.price)&& pktdata[reversecount].Action!=stData.Action)
					{
						diffrence = stoi(stData.Time) - stoi(pktdata[reversecount].Time);
						LatenyDiff.push_back(diffrence);
						PrintData stPdata;
						stPdata.OrderReqToken = stData.Token;
						stPdata.OrderReqPrice = stData.price;
						stPdata.OrderReqTime = stData.Time;
						stPdata.OrderMsgTime = pktdata[reversecount].Time;
						stPdata.OrderMsgPrice = pktdata[reversecount].price;
						stPdata.OrderMsgAction = pktdata[reversecount].Action;
						fnprintData(stPdata, diffrence);
						break;
					}
				}
			}			
		}
		if (LatenyDiff.size() == 0)
		{
			std::cout << "No Mactching data found for tick to order" << std::endl;
		}
		else
		{
			MinMaxAvg(LatenyDiff);
		}
	}
}

void fnprintData(const PrintData stPdata, int Difference)
{
	cout <<"					| " <<setw(10) << left << stPdata.OrderReqToken << setw(16);
	cout << stPdata.OrderReqPrice<< setw(17);
	cout << stPdata.OrderReqTime << setw(17);
	cout << stPdata.OrderMsgTime << setw(10);
	cout << stPdata.OrderMsgPrice << setw(10);
	cout << stPdata.OrderMsgAction << setw(5);
	cout << "= " << setw(7) << Difference << " |"<<std::endl;
	cout <<endl;
}

void usage()
{
	std::cout <<"usage: .\LatencyAnalyzer.exe xyz.csv" << std::endl;
}

void MinMaxAvg(vector<int>LatecncyDiff)
{
	int max = getMax(LatecncyDiff, (int)LatecncyDiff.size());	
	int min = getMin(LatecncyDiff, (int)LatecncyDiff.size());
	int avg = calcAvg(LatecncyDiff, (int)LatecncyDiff.size());
	std::cout << "					_______________________________________________________________________________________________" << std::endl
		<<"					|					MAX: " << max << "							|"<<std::endl
		<< "					|					MIN: " << min << "							|"<<std::endl
		<< "					|					AVG: " << avg << "							|"<<std::endl;
	cout << "					------------------------------------------------------------------------------------------------" << endl;
	cout << "					|				" << min << " is the LOW LATENCY "<<"						|"<<endl;
	cout << "					________________________________________________________________________________________________" << endl;
}	
int getMin(vector<int>vDiff, int n)
{
	int res = vDiff[0];
	for (int i = 1; i < n; i++)
		res = min(res, vDiff[i]);
	return res;
}

int getMax(vector<int>vDiff, int n)
{
	int res = vDiff[0];
	for (int i = 1; i < n; i++)
		res = max(res, vDiff[i]);
	return res;
}

int calcAvg(vector<int>vDiff, int n)
{
	int res = 0;	
	for (int i = 0; i < n; i++)
		res += vDiff[i];
	int Avg = res / n;
	return Avg;
}

int main(int argc,char** argv)
{
	if (argc < 2)
	{
		usage();
		return 0;
	}
	fileName.assign(argv[1]);	
	read_record();		
	return 0;
}
