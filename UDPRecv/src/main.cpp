#include"../include/UDPSocket.h"
#include"../include/recv_json.h"
#include<vector>
using namespace std;
unsigned short sport=10002;// 目的Port
unsigned short rport=30002; //接收Port
unsigned short sgport = 31002;
uint8_t channel;// Channel 号= 0x01;
map<uint8_t, string>mapc_p;
/*void config()
{
	ifstream fin("json.txt", ios::in);
	if (!fin.is_open())
	{
		cout << "未成功" << endl;
		return;
	}
	vector<string>tmp;
	string s;
	int index = 0;
	while (getline(fin, s))
	{
		tmp.push_back(s);
	}
	rport = atoi(tmp[0].c_str());
	cout << rport << endl;
	sport = atoi(tmp[1].c_str());
	cout << sport << endl;
	for (int i = 2; i < tmp.size() - 1; i+=2)
	{
		destip = tmp[i];
		cout << destip << endl;
		unsigned short channel_ = atoi(tmp[i+1].c_str());
		channel = (uint8_t)(channel_);
		cout << channel << endl;
		mapc_p[channel] = destip;
	}
	/*destip = tmp[2];
	cout << destip << endl;
	unsigned short channel_ = atoi(tmp[3].c_str());
    channel=(uint8_t)(channel_);
    cout<<channel<<endl;
	mapc_t[channel] = destip;
	destip = tmp[4];
	cout << destip << endl;
	unsigned short channel_ = atoi(tmp[5].c_str());
	channel = (uint8_t)(channel_);
	mapc_p[channel] = destip;
	destip = tmp[6];
	cout << destip << endl;
	unsigned short channel_ = atoi(tmp[7].c_str());
	channel = (uint8_t)(channel_);
	mapc_p[channel] = destip;
	fin.close();
}*/
int main()
{
	UDPSocket signal;//监听信号
	if(signal.create(sgport)<0)
    {
        cout<<"sock error"<<endl;
        return 0;
    }
	char sign_buf[100];
	string localip_;
    if (signal.recvbuf(sign_buf, 100, localip_, sgport) < 0)
		cout << "signal is error" << endl;
	//signal.Close();
    //strcpy(sign_buf,"m_open_1");
	if (strcmp(sign_buf,"m_open_")==0)
	{
		read_config("../web/ch2ip.json", mapc_p);
		UDPSocket rsock;
		UDPSocket ssock;
		rsock.create(rport);
		ssock.create();
		char *buf = new char[50000];
		int recvsize;
		int count = 0;
		unsigned short port;
		string localip;
		while (1)
		{
			recvsize = rsock.recvbuf(buf, 50000, localip, port);
			cout << port << endl;
			if (port != 9212)
			{
				++count;
				printf("the buf is %d,the length is %d\n", count, recvsize);
				uint8_t tmp = (uint8_t)buf[0];
				//printf("the dest is %x",tmp);
				//if(tmp==channel)
				//{
				int send = ssock.sendbuf(buf + 1, recvsize - 1, mapc_p[tmp], sport);
				printf("the sendbuf is %d\n", send);
				memset(buf, 0, 50000);
				//}
			}
			else
			{
				sleep(1);
				read_config("../web/ch2ip.json", mapc_p);
			}
		}
		delete[] buf;
		rsock.Close();
		ssock.Close();
		return 0;
	}
	else
	{
        cout<<"close myrecv....."<<endl;
		return 0;
	}
}

