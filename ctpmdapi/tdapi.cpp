#include "tdapi.h"



tdapi::tdapi()
{
	request_id = 0;
}


tdapi::~tdapi()
{
}

void tdapi::init()
{
	api = CThostFtdcTraderApi::CreateFtdcTraderApi("");

	auto& cfg = Config::getConfig(string("config.ini"));

	string trade_front = cfg.Read("TradeFront", string(""));

	api->RegisterSpi(this);
	api->RegisterFront(const_cast<char*>(trade_front.c_str()));
	api->SubscribePrivateTopic(THOST_TERT_QUICK);
	api->SubscribePublicTopic(THOST_TERT_QUICK);
}


void tdapi::join()
{
	this->api->Join();
}

void tdapi::connect()
{
	api->Init();
}

void tdapi::login()
{
	CThostFtdcReqUserLoginField login_req;
	memset(&login_req, 0, sizeof(login_req));

	auto& cfg = Config::getConfig(string("config.ini"));

	string BrokerId = cfg.Read("brokerID", string(""));
	string UserId = cfg.Read("investorID", string(""));
	string password = cfg.Read("password", string(""));

	strcpy_s(login_req.BrokerID, sizeof(login_req.BrokerID), BrokerId.c_str());
	strcpy_s(login_req.UserID, sizeof(login_req.UserID), UserId.c_str());
	strcpy_s(login_req.Password, sizeof(login_req.Password), password.c_str());

	int i = this->api->ReqUserLogin(&login_req, ++request_id);
	cout << "��½������" << i << endl;
}

void tdapi::logout()
{
	CThostFtdcUserLogoutField logout_req;
	memset(&logout_req, 0, sizeof(logout_req));

	auto& cfg = Config::getConfig(string("config.ini"));

	string BrokerId = cfg.Read("brokerID", string(""));
	string UserId = cfg.Read("investorID", string(""));

	strcpy_s(logout_req.BrokerID, sizeof(logout_req.BrokerID), BrokerId.c_str());
	strcpy_s(logout_req.UserID, sizeof(logout_req.UserID), UserId.c_str());


	int i = this->api->ReqUserLogout(&logout_req, ++request_id);
	cout << "�ǳ�������:" << i << endl;
}

void tdapi::close()
{
	this->api->RegisterSpi(nullptr);
	this->api->Release();
	this->api = nullptr;
}

void tdapi::query_all_contract()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	api->ReqQryInstrument(&req, ++request_id);
}

void tdapi::OnFrontConnected()
{
	std::cout << "����ǰ�����ӳɹ�" << endl;
	this->login();
}

void tdapi::OnFrontDisconnected(int nReason)
{
	cout << "�Ͽ�����,ԭ��:" << nReason<< endl;
}

void tdapi::OnHeartBeatWarning(int nTimeLapse)
{
	cout << "��������:" << nTimeLapse << endl;
}

void tdapi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "�ͻ�����Ӧ��֤" << endl;
}

void tdapi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "���׵�½�ɹ�"<<endl;
	this->query_all_contract();
}

void tdapi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "�ǳ��ر�" << endl;
	cout << "�û�:" << pUserLogout->UserID << endl;
	cout << "������:" << pUserLogout->BrokerID << endl;
}

void tdapi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "��Լ�ر�" << endl;
	cout << pInstrument->InstrumentID << endl;
	cout << pInstrument->InstrumentName << endl;
	cout << pInstrument->ExchangeID << endl;
	auto& cfg = Config::getConfig("config.ini");
	string symbol_file = cfg.Read("symbolDir", string(""));
	fstream f(symbol_file.c_str(), ios::out|ios::app);
	
	if(!f.fail())
	{
		f << pInstrument->InstrumentID << "," << pInstrument->InstrumentName << "," 
		  << pInstrument->ExchangeID << ","  << pInstrument->ExpireDate << "," << endl;
	}
	f.close();

}
