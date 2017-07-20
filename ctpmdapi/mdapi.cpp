#include "mdapi.h"
#include <iostream>


void MdApi::OnFrontConnected()
{
	cout << "���ӳɹ���" << endl;
}

void MdApi::OnFrontDisconnected(int nReason)
{
	cout << "���ӶϿ���" << endl;
}

void MdApi::OnHeartBeatWarning(int nTimeLapse)
{
	cout << "�������棡" << endl;
}

void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "��½�ɹ���" << endl;
}

void MdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "�ǳ��ɹ���" << endl;
}

void MdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "error!" << endl;
};

void MdApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "���Ļر�!" << endl;
	cout << "���ĺ�Լ:" << pSpecificInstrument->InstrumentID << "�ر���Ϣ:" << pRspInfo->ErrorMsg << endl;
}

void MdApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void MdApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

};

void MdApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

};

void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	cout << "�յ�����!" << endl;
	cout << pDepthMarketData->InstrumentID << "price:" << pDepthMarketData->ClosePrice << "volume:" << pDepthMarketData->Volume << endl;
};

void MdApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{

};

//��������

void MdApi::createFtdcMdApi(string pszFlowPath)
{
	this->api = CThostFtdcMdApi::CreateFtdcMdApi("");
	this->api->RegisterSpi(this);
}

void MdApi::init()
{
	this->api->Init();
}

void MdApi::release()
{
	this->api->Release();
}

int MdApi::join()
{
	int i = this->api->Join();
	return i;
}

int MdApi::exit()
{
	this->api->RegisterSpi(nullptr);
	this->api->Release();
	this->api = nullptr;
	return 1;
}

string MdApi::getTradingDay()
{
	string day = this->api->GetTradingDay();
	return day;
}

void MdApi::registerFront(string pszFrontAddress)
{

	this->api->RegisterFront(const_cast<char *>(pszFrontAddress.c_str()));
}

int MdApi::subscribeMarketData(string instrumentID)
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->SubscribeMarketData(req, 1);
	return i;
}

int MdApi::unSubscribeMarketData(string instrumentID)
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->UnSubscribeMarketData(req, 1);
	return i;
}


int MdApi::subscribeForQuoteRsp(string instrumentID)
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->SubscribeForQuoteRsp(req, 1);
	return i;
}

int MdApi::unSubscribeForQuoteRsp(string instrumentID)
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };;
	int i = this->api->UnSubscribeForQuoteRsp(req, 1);
	return i;
}

int MdApi::reqUserLogin(int nRequestID)
{
	CThostFtdcReqUserLoginField login_req;
	memset(&login_req, 0, sizeof(login_req));

	strcpy(login_req.BrokerID, "9999");
	strcpy(login_req.UserID, "092433");
	strcpy(login_req.Password, "zm19641");

	int i = this->api->ReqUserLogin(&login_req, 0);
	cout << i << endl;
	return i;
}