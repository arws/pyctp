#include "mdapi.h"
#include <iostream>
#include <unordered_map>
#include <corecrt_io.h>
#include "FileUtil.h"

#define DBLMXTZERO(x) (x==DBL_MAX)?0:x

void MdApi::push_task(Task task)
{
	this->_queue.enqueue(task);
}



void MdApi::OnFrontConnected()
{
	Task task;
	task.name = ON_FRONT_CONNECTED;
	LOG(INFO) << "���ӳɹ���" << endl;
	//cout << "���ӳɹ���" << endl;
	this->_queue.enqueue(task);
}

void MdApi::OnFrontDisconnected(int nReason)
{
	Task task;
	task.name = ON_FRONT_DISCONNECTED;
	task.task_data = nReason;
	LOG(INFO) << "���ӶϿ���" << endl;
	this->_queue.enqueue(task);
}

void MdApi::OnHeartBeatWarning(int nTimeLapse)
{
	Task task;
	task.name = ON_HEARTBEAT_WARNING;
	task.task_data = nTimeLapse;
	LOG(INFO) << "�������棡" << endl;
	push_task(task);
}

void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_USRLOGIN;
	task.task_data = *pRspUserLogin;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	LOG(INFO) << "��½�ɹ���" << endl;
	LOG(INFO) << "�û�:" << pRspUserLogin->UserID << endl;
	LOG(INFO) << "������" << pRspUserLogin->BrokerID << endl;
	LOG(INFO) << "����:" << pRspUserLogin->TradingDay << endl;
	LOG(INFO) << "��¼ʱ��:" << pRspUserLogin->LoginTime << endl;
	LOG(INFO) << "֣����ʱ��:" << pRspUserLogin->CZCETime << endl;
	LOG(INFO) << "������ʱ��:" << pRspUserLogin->DCETime << endl;
	LOG(INFO) << "�н���ʱ�䣺" << pRspUserLogin->FFEXTime << endl;
	LOG(INFO) << "������ʱ��: " << pRspUserLogin->SHFETime << endl;
	LOG(INFO) << "��Դ����ʱ��:" << pRspUserLogin->INETime << endl;
	//cout << "��½�ɹ���" << endl;
	push_task(task);
	this->subscribe();
}

void MdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_USRLOGOUT;
	task.task_data = *pUserLogout;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	LOG(INFO) << "�ǳ��ɹ���" << endl;
	push_task(task);
}

void MdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_ERR;
	task.task_data = nullptr;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	LOG(INFO) << "error!" << endl;
	push_task(task);
};

void MdApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_SUBMARKET_DATA;
	task.task_data = *pSpecificInstrument;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	LOG(INFO) << string("���ĺ�Լ:") << pSpecificInstrument->InstrumentID << string("�ر���Ϣ:") << pRspInfo->ErrorMsg << endl;
	
	//cout << "���Ļر�!" << endl;
	//cout << "���ĺ�Լ:" << pSpecificInstrument->InstrumentID << "�ر���Ϣ:" << pRspInfo->ErrorMsg << endl;
	push_task(task);
}

void MdApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_UNSUBMARKET_DATA;
	task.task_data = *pSpecificInstrument;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	push_task(task);
}

void MdApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_SUB_FOR_QUOTE_RSP;
	task.task_data = *pSpecificInstrument;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	push_task(task);
};

void MdApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task;
	task.name = ON_RSP_UNSUB_FOR_QUOTE_RSP;
	task.task_data = *pSpecificInstrument;
	task.task_error = *pRspInfo;
	task.request_id = nRequestID;
	task.is_last = bIsLast;
	push_task(task);
};

void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	Task task;
	task.name = ON_RTN_DEPTH_MARKET_DATA;
	task.task_data = *pDepthMarketData;
	push_task(task);
};

void MdApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	Task task;
	task.name = ON_RTN_FOR_QUOTE_RSP;
	task.task_data = *pForQuoteRsp;
	push_task(task);
};

void MdApi::init()
{
	auto f = std::bind(&MdApi::process_task, this);
	_t = make_shared<thread>(f);
}

void MdApi::connect()
{
	auto& cfg = Config::getConfig(string("config.ini"));

	this->_api = CThostFtdcMdApi::CreateFtdcMdApi("");
	this->_api->RegisterSpi(this);
	this->_api->RegisterFront(const_cast<char*>(cfg.Read("MarketFront", string("")).c_str()));
	this->_api->Init();
	LOG(INFO) << "����ϵͳ��ʼ��" << endl;
	//this->api->Join();
}

void MdApi::close()
{
	this->_api->RegisterSpi(nullptr);
	this->_api->Release();
	this->_api = nullptr;
	LOG(INFO) << "����ϵͳ�ر�" << endl;
}

void MdApi::subscribe()
{
	char buffer[128];
	vector<string> contract_name_list;
	auto& config = Config::getConfig("config.ini");
	auto symbol_dir = config.Read("symbolDir", string(""));
	fstream symbolfile(symbol_dir.c_str());
	while (!symbolfile.eof())
	{
		symbolfile.getline(buffer, sizeof(buffer));
		auto line = string(buffer);
		auto contract_name = line.substr(0, line.find(','));
		contract_name_list.push_back(contract_name);
	}
	char** req = new char*[contract_name_list.size()];
	int i = 0;
	for(auto& contract:contract_name_list)
	{
		req[i++] = const_cast<char*>(contract.c_str());
	}
	int j = this->_api->SubscribeMarketData(req, contract_name_list.size());
	LOG(INFO) << "����ȫ����Լ" << endl;
	delete[] req;
}



void MdApi::login()
{
	CThostFtdcReqUserLoginField login_req;
	memset(&login_req, 0, sizeof(login_req));

	auto& cfg = Config::getConfig(string("config.ini"));

	string BrokerId = cfg.Read("brokerID", string(""));
	string UserId = cfg.Read("investorId", string(""));
	string password = cfg.Read("password", string(""));

	strcpy_s(login_req.BrokerID, sizeof(login_req.BrokerID), BrokerId.c_str());
	strcpy_s(login_req.UserID, sizeof(login_req.UserID), UserId.c_str());
	strcpy_s(login_req.Password, sizeof(login_req.Password), password.c_str());

	int i = this->_api->ReqUserLogin(&login_req, ++request_id);
	LOG(INFO) << "׼����½����ǰ��,��½������:" << i << endl;
	//cout << "��½������" << i << endl;
}

void MdApi::logout()
{
	CThostFtdcUserLogoutField logout_req;
	memset(&logout_req, 0, sizeof(logout_req));

	strcpy_s(logout_req.BrokerID,sizeof(logout_req.BrokerID),  "9999");
	strcpy_s(logout_req.UserID, sizeof(logout_req.UserID), "092433");

	int i = this->_api->ReqUserLogout(&logout_req, ++request_id);
	cout << "�ǳ�������:" << i << endl;
}



void MdApi::process_task()
{
//��������
	while(true)
	{
		Task task;
		this->_queue.wait_dequeue(task);
		switch (task.name)
		{
		case ON_FRONT_CONNECTED:
			{
			this->process_on_front_connected(task);
				break;
			}
		case ON_FRONT_DISCONNECTED:
			{
			this->process_on_front_disconneted(task);
				break;
			}
		case ON_HEARTBEAT_WARNING:
			{
			this->process_on_heartbeat_warning(task);
			break;
			}
		case ON_RSP_USRLOGIN:
			{
			this->process_on_rsp_userlogin(task);
			break;
			}
		case ON_RSP_USRLOGOUT:
			{
			this->process_on_rsp_user_logout(task);
			break;
			}
		case ON_RSP_ERR:
			{
			this->process_on_rsp_error(task);
			break;
			}
		case ON_RSP_SUBMARKET_DATA:
			{
			this->process_on_rsp_submarket_data(task);
			break;
			}
		case ON_RSP_UNSUBMARKET_DATA:
			{
			this->process_on_rsp_unsubmarket_data(task);
			break;
			}
		case ON_RSP_SUB_FOR_QUOTE_RSP:
			{
			this->process_on_rsp_sub_for_quote_rsp(task);
			break;
			}
		case ON_RSP_UNSUB_FOR_QUOTE_RSP:
			{
			this->process_on_rsp_unsub_for_quote_rsp(task);
			break;
			}
		case ON_RTN_DEPTH_MARKET_DATA:
			{
			this->process_on_rtn_depth_market_data(task);
			break;
			}
		case ON_RTN_FOR_QUOTE_RSP:
			{
			this->process_on_rtn_for_quote_rsp(task);
			break;
			}
		default:
			break;
		}
	}
}


void MdApi::release()
{
	this->_api->Release();
}

int MdApi::join()
{
	int i = this->_api->Join();
	return i;
}
 
string MdApi::getTradingDay()
{
	string day = this->_api->GetTradingDay();
	return day;
}


void MdApi::process_on_front_connected(Task task)
{
	/*cout << "��½�ɹ�" << endl;*/
	this->login();
}

void MdApi::process_on_front_disconneted(Task task)
{
	int reason = boost::any_cast<int>(task.task_data);
	//cout << "���ӶϿ���ԭ��:" << reason << endl;
}

void MdApi::process_on_heartbeat_warning(Task task)
{
	int time_elapsed =  boost::any_cast<int>(task.task_data);
	//cout << "��������:" << time_elapsed << "δ�յ�����" << endl;
}

void MdApi::process_on_rsp_userlogin(Task task)
{
	CThostFtdcRspUserLoginField login_field = boost::any_cast<CThostFtdcRspUserLoginField>(task.task_data);


	//cout << "�û�:" << login_field.UserID << endl;
	//cout << "������" << login_field.BrokerID << endl;
	//cout << "����:" << login_field.TradingDay << endl;
	//cout << "��¼ʱ��:" << login_field.LoginTime << endl;
	//cout << "֣����ʱ��:" << login_field.CZCETime << endl;
	//cout << "������ʱ��:" << login_field.DCETime << endl;
	//cout << "�н���ʱ�䣺" << login_field.FFEXTime << endl;
	//cout << "������ʱ��: " << login_field.SHFETime << endl;
	//cout << "��Դ����ʱ��:" << login_field.INETime << endl;
}

void MdApi::process_on_rsp_user_logout(Task task)
{
	CThostFtdcUserLogoutField logout_field = boost::any_cast<CThostFtdcUserLogoutField>(task.task_data);
	cout << "������:" << logout_field.BrokerID << endl;
	cout << "�û�:" << logout_field.UserID <<endl;
}

void MdApi::process_on_rsp_error(Task task)
{
	CThostFtdcRspInfoField rsp_error = boost::any_cast<CThostFtdcRspInfoField>(task.task_error);
	cout << "������룺" << rsp_error.ErrorID << endl;
	cout << "������Ϣ��" << rsp_error.ErrorMsg <<endl;
}

void MdApi::process_on_rsp_submarket_data(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	//LOG(INFO) << "���Ļر�,����:" << instrument_field.InstrumentID << endl;
	//cout << "���Ļر�,����:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_unsubmarket_data(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "�ⶩ�ر�,����:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_sub_for_quote_rsp(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "ѯ�ۻر�������:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_unsub_for_quote_rsp(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "ȡ��ѯ�ۻر�,����" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rtn_depth_market_data(Task task)
{
	cout << " ---------------�������֪ͨ-------------------" << endl;
	CThostFtdcDepthMarketDataField market_data = boost::any_cast<CThostFtdcDepthMarketDataField>(task.task_data);
	//cout<< "���룺" << market_data.InstrumentID<<endl;
	//cout<< "�����գ�" << market_data.TradingDay	 <<endl;
	//cout<< "ҵ���գ�" << market_data.ActionDay	 <<endl;
	//cout << "ʱ��:" << market_data.UpdateTime << ":" << market_data.UpdateMillisec << endl;
	//cout << "���¼�  " << "�ɽ���  " << "���̼�  " << "��߼�  " << "��ͼ�  " << endl;
	//cout << market_data.LastPrice << "  " << market_data.Volume << "  " << market_data.OpenPrice << "  " << market_data.HighestPrice << "  " << market_data.LowestPrice << endl;

	//cout << "��" << market_data.AskPrice5 << "  " << market_data.AskVolume5<<endl;
	//cout << "��" << market_data.AskPrice4 << "  " << market_data.AskVolume4<<endl;
	//cout << "��" << market_data.AskPrice3 << "  " << market_data.AskVolume3<<endl;
	//cout << "��" << market_data.AskPrice2 << "  " << market_data.AskVolume2<<endl;
	//cout << "һ" << market_data.AskPrice1 << "  " << market_data.AskVolume1<<endl;
	//cout << "---------------------------------------------------------------------" << endl;
	//cout << "һ" << market_data.BidPrice1 << "  " << market_data.BidVolume1 << endl;
	//cout << "��" << market_data.BidPrice2 << "  " << market_data.BidVolume2 << endl;
	//cout << "��" << market_data.BidPrice3 << "  " << market_data.BidVolume3 << endl;
	//cout << "��" << market_data.BidPrice4 << "  " << market_data.BidVolume4 << endl;
	//cout << "��" << market_data.BidPrice5 << "  " << market_data.BidVolume5 << endl;

	auto& config = Config::getConfig("config.ini");
	string tick_path = config.Read("TickDir", string(""));

	if(!FileUtil::is_file_exist(tick_path))
	{
		FileUtil::create_direction(tick_path);
	}

	auto instrument_id =  string(market_data.InstrumentID);
	auto instrument_type = instrument_id.substr(0, find_if(instrument_id.begin(), instrument_id.end(), [](char&c)->bool {return isdigit(c); })-instrument_id.begin());


	auto instrument_folder = tick_path + instrument_type;

	if(!FileUtil::is_file_exist(instrument_folder))
	{
		FileUtil::create_direction(instrument_folder);
	}


	auto tick_file = instrument_folder+string("/")+market_data.InstrumentID + string(".csv");

	insert_data(market_data, tick_file);
}

void MdApi::process_on_rtn_for_quote_rsp(Task task)
{
	CThostFtdcForQuoteRspField quote_rsp = boost::any_cast<CThostFtdcForQuoteRspField>(task.task_data);
	cout << " ---------------ѯ��֪ͨ�ر�-------------------" << endl;
	cout << "���룺" << quote_rsp.InstrumentID<<endl;
	cout << "ҵ�����ڣ�" << quote_rsp.ActionDay	<<endl;
	cout << "��������" << quote_rsp.ExchangeID	<<endl;
	cout << "�����գ�" << quote_rsp.TradingDay	<<endl;
	cout << "����ʱ��:" << quote_rsp.ForQuoteTime<<endl;
	cout << "����ϵͳ����:" << quote_rsp.ForQuoteSysID << endl;
}


void MdApi::insert_head(fstream& f)
{
	f << "������" << ","
		<< "��Լ����" << ","
		<< "����������" << ","
		<< "��Լ�ڽ������Ĵ���" << ","
		<< "���¼�" << ","
		<< "�ϴν����" << ","
		<< "������" << ","
		<< "��ֲ���" << ","
		<< "����" << ","
		<< "��߼�" << ","
		<< "��ͼ�" << ","
		<< "����" << ","
		<< "�ɽ����" << ","
		<< "�ֲ���" << ","
		<< "������" << ","
		<< "���ν����" << ","
		<< "��ͣ���" << ","
		<< "��ͣ���" << ","
		<< "����ʵ��" << ","
		<< "����ʵ��" << ","
		<< "����޸�ʱ��" << ","
		<< "����޸ĺ���" << ","
		<< "�����һ" << ","
		<< "������һ" << ","
		<< "������һ" << ","
		<< "������һ" << ","
		<< "����۶�" << ","
		<< "��������" << ","
		<< "�����۶�" << ","
		<< "��������" << ","
		<< "�������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "�������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "�������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "��������" << ","
		<< "���վ���" << ","
		<< "ҵ������"
		<< endl;
}

void MdApi::insert_tick(CThostFtdcDepthMarketDataField& market_data, fstream& f)
{
	f << market_data.TradingDay << ","
		<< market_data.InstrumentID << ","
		<< market_data.ExchangeID << ","
		<< market_data.ExchangeInstID << ","
		<< (DBLMXTZERO(market_data.LastPrice)) << ","
		<< (DBLMXTZERO(market_data.PreSettlementPrice)) << ","
		<< (DBLMXTZERO(market_data.PreClosePrice)) << ","
		<< market_data.PreOpenInterest << ","
		<< (DBLMXTZERO(market_data.OpenPrice)) << ","
		<< (DBLMXTZERO(market_data.HighestPrice)) << ","
		<< (DBLMXTZERO(market_data.LowestPrice)) << ","
		<< market_data.Volume << ","
		<< market_data.Turnover << ","
		<< market_data.OpenInterest << ","
		<< (DBLMXTZERO(market_data.ClosePrice)) << ","
		<< (DBLMXTZERO(market_data.SettlementPrice)) << ","
		<< (DBLMXTZERO(market_data.UpperLimitPrice)) << ","
		<< (DBLMXTZERO(market_data.LowerLimitPrice)) << ","
		<< (DBLMXTZERO(market_data.PreDelta))  << ","
		<< (DBLMXTZERO(market_data.CurrDelta)) << ","
		<< market_data.UpdateTime << ","
		<< market_data.UpdateMillisec << ","
		<< (DBLMXTZERO(market_data.BidPrice1)) << ","
		<< market_data.BidVolume1 << ","
		<< (DBLMXTZERO(market_data.AskPrice1)) << ","
		<< market_data.AskVolume1 << ","
		<< (DBLMXTZERO(market_data.BidPrice2)) << ","
		<< market_data.BidVolume2 << ","
		<< (DBLMXTZERO(market_data.AskPrice2)) << ","
		<< market_data.AskVolume2 << ","
		<< (DBLMXTZERO(market_data.BidPrice3)) << ","
		<< market_data.BidVolume3 << ","
		<< (DBLMXTZERO(market_data.AskPrice3)) << ","
		<< market_data.AskVolume3 << ","
		<< (DBLMXTZERO(market_data.BidPrice4)) << ","
		<< market_data.BidVolume4 << ","
		<< (DBLMXTZERO(market_data.AskPrice4)) << ","
		<< market_data.AskVolume4 << ","
		<< (DBLMXTZERO(market_data.BidPrice5)) << ","
		<< market_data.BidVolume5 << ","
		<< (DBLMXTZERO(market_data.AskPrice5)) << ","
		<< market_data.AskVolume5 << "," 
	    << (DBLMXTZERO(market_data.AveragePrice)) << ","
	    << market_data.ActionDay << ","
		<< endl;
}

void MdApi::insert_data(CThostFtdcDepthMarketDataField market_data, string tick_file)
{
	fstream f((tick_file).c_str(), ios::app | ios::out | ios::ate);

	if (static_cast<int>(f.tellp()) == ios::beg)
	{
		insert_head(f);
		insert_tick(market_data, f);

	}
	else
	{
		insert_tick(market_data, f);
	}
}
