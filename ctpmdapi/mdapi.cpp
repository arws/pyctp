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
	LOG(INFO) << "连接成功！" << endl;
	//cout << "连接成功！" << endl;
	this->_queue.enqueue(task);
}

void MdApi::OnFrontDisconnected(int nReason)
{
	Task task;
	task.name = ON_FRONT_DISCONNECTED;
	task.task_data = nReason;
	LOG(INFO) << "连接断开！" << endl;
	this->_queue.enqueue(task);
}

void MdApi::OnHeartBeatWarning(int nTimeLapse)
{
	Task task;
	task.name = ON_HEARTBEAT_WARNING;
	task.task_data = nTimeLapse;
	LOG(INFO) << "心跳警告！" << endl;
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
	LOG(INFO) << "登陆成功！" << endl;
	LOG(INFO) << "用户:" << pRspUserLogin->UserID << endl;
	LOG(INFO) << "经纪商" << pRspUserLogin->BrokerID << endl;
	LOG(INFO) << "日期:" << pRspUserLogin->TradingDay << endl;
	LOG(INFO) << "登录时间:" << pRspUserLogin->LoginTime << endl;
	LOG(INFO) << "郑商所时间:" << pRspUserLogin->CZCETime << endl;
	LOG(INFO) << "大商所时间:" << pRspUserLogin->DCETime << endl;
	LOG(INFO) << "中金所时间：" << pRspUserLogin->FFEXTime << endl;
	LOG(INFO) << "上期所时间: " << pRspUserLogin->SHFETime << endl;
	LOG(INFO) << "能源中心时间:" << pRspUserLogin->INETime << endl;
	//cout << "登陆成功！" << endl;
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
	LOG(INFO) << "登出成功！" << endl;
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
	LOG(INFO) << string("订阅合约:") << pSpecificInstrument->InstrumentID << string("回报信息:") << pRspInfo->ErrorMsg << endl;
	
	//cout << "订阅回报!" << endl;
	//cout << "订阅合约:" << pSpecificInstrument->InstrumentID << "回报信息:" << pRspInfo->ErrorMsg << endl;
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
	LOG(INFO) << "行情系统初始化" << endl;
	//this->api->Join();
}

void MdApi::close()
{
	this->_api->RegisterSpi(nullptr);
	this->_api->Release();
	this->_api = nullptr;
	LOG(INFO) << "行情系统关闭" << endl;
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
	LOG(INFO) << "订阅全部合约" << endl;
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
	LOG(INFO) << "准备登陆行情前置,登陆返回码:" << i << endl;
	//cout << "登陆返回码" << i << endl;
}

void MdApi::logout()
{
	CThostFtdcUserLogoutField logout_req;
	memset(&logout_req, 0, sizeof(logout_req));

	strcpy_s(logout_req.BrokerID,sizeof(logout_req.BrokerID),  "9999");
	strcpy_s(logout_req.UserID, sizeof(logout_req.UserID), "092433");

	int i = this->_api->ReqUserLogout(&logout_req, ++request_id);
	cout << "登出返回码:" << i << endl;
}



void MdApi::process_task()
{
//主动函数
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
	/*cout << "登陆成功" << endl;*/
	this->login();
}

void MdApi::process_on_front_disconneted(Task task)
{
	int reason = boost::any_cast<int>(task.task_data);
	//cout << "连接断开，原因:" << reason << endl;
}

void MdApi::process_on_heartbeat_warning(Task task)
{
	int time_elapsed =  boost::any_cast<int>(task.task_data);
	//cout << "心跳警告:" << time_elapsed << "未收到心跳" << endl;
}

void MdApi::process_on_rsp_userlogin(Task task)
{
	CThostFtdcRspUserLoginField login_field = boost::any_cast<CThostFtdcRspUserLoginField>(task.task_data);


	//cout << "用户:" << login_field.UserID << endl;
	//cout << "经纪商" << login_field.BrokerID << endl;
	//cout << "日期:" << login_field.TradingDay << endl;
	//cout << "登录时间:" << login_field.LoginTime << endl;
	//cout << "郑商所时间:" << login_field.CZCETime << endl;
	//cout << "大商所时间:" << login_field.DCETime << endl;
	//cout << "中金所时间：" << login_field.FFEXTime << endl;
	//cout << "上期所时间: " << login_field.SHFETime << endl;
	//cout << "能源中心时间:" << login_field.INETime << endl;
}

void MdApi::process_on_rsp_user_logout(Task task)
{
	CThostFtdcUserLogoutField logout_field = boost::any_cast<CThostFtdcUserLogoutField>(task.task_data);
	cout << "经纪商:" << logout_field.BrokerID << endl;
	cout << "用户:" << logout_field.UserID <<endl;
}

void MdApi::process_on_rsp_error(Task task)
{
	CThostFtdcRspInfoField rsp_error = boost::any_cast<CThostFtdcRspInfoField>(task.task_error);
	cout << "错误代码：" << rsp_error.ErrorID << endl;
	cout << "错误信息：" << rsp_error.ErrorMsg <<endl;
}

void MdApi::process_on_rsp_submarket_data(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	//LOG(INFO) << "订阅回报,代码:" << instrument_field.InstrumentID << endl;
	//cout << "订阅回报,代码:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_unsubmarket_data(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "解订回报,代码:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_sub_for_quote_rsp(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "询价回报，代码:" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rsp_unsub_for_quote_rsp(Task task)
{
	CThostFtdcSpecificInstrumentField instrument_field = boost::any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	cout << "取消询价回报,代码" << instrument_field.InstrumentID << endl;
}

void MdApi::process_on_rtn_depth_market_data(Task task)
{
	cout << " ---------------深度行情通知-------------------" << endl;
	CThostFtdcDepthMarketDataField market_data = boost::any_cast<CThostFtdcDepthMarketDataField>(task.task_data);
	//cout<< "代码：" << market_data.InstrumentID<<endl;
	//cout<< "交易日：" << market_data.TradingDay	 <<endl;
	//cout<< "业务日：" << market_data.ActionDay	 <<endl;
	//cout << "时间:" << market_data.UpdateTime << ":" << market_data.UpdateMillisec << endl;
	//cout << "最新价  " << "成交量  " << "开盘价  " << "最高价  " << "最低价  " << endl;
	//cout << market_data.LastPrice << "  " << market_data.Volume << "  " << market_data.OpenPrice << "  " << market_data.HighestPrice << "  " << market_data.LowestPrice << endl;

	//cout << "五" << market_data.AskPrice5 << "  " << market_data.AskVolume5<<endl;
	//cout << "四" << market_data.AskPrice4 << "  " << market_data.AskVolume4<<endl;
	//cout << "三" << market_data.AskPrice3 << "  " << market_data.AskVolume3<<endl;
	//cout << "二" << market_data.AskPrice2 << "  " << market_data.AskVolume2<<endl;
	//cout << "一" << market_data.AskPrice1 << "  " << market_data.AskVolume1<<endl;
	//cout << "---------------------------------------------------------------------" << endl;
	//cout << "一" << market_data.BidPrice1 << "  " << market_data.BidVolume1 << endl;
	//cout << "二" << market_data.BidPrice2 << "  " << market_data.BidVolume2 << endl;
	//cout << "三" << market_data.BidPrice3 << "  " << market_data.BidVolume3 << endl;
	//cout << "四" << market_data.BidPrice4 << "  " << market_data.BidVolume4 << endl;
	//cout << "五" << market_data.BidPrice5 << "  " << market_data.BidVolume5 << endl;

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
	cout << " ---------------询价通知回报-------------------" << endl;
	cout << "代码：" << quote_rsp.InstrumentID<<endl;
	cout << "业务日期：" << quote_rsp.ActionDay	<<endl;
	cout << "交易所：" << quote_rsp.ExchangeID	<<endl;
	cout << "交易日：" << quote_rsp.TradingDay	<<endl;
	cout << "交易时间:" << quote_rsp.ForQuoteTime<<endl;
	cout << "交易系统代码:" << quote_rsp.ForQuoteSysID << endl;
}


void MdApi::insert_head(fstream& f)
{
	f << "交易日" << ","
		<< "合约代码" << ","
		<< "交易所代码" << ","
		<< "合约在交易所的代码" << ","
		<< "最新价" << ","
		<< "上次结算价" << ","
		<< "昨收盘" << ","
		<< "昨持仓量" << ","
		<< "今开盘" << ","
		<< "最高价" << ","
		<< "最低价" << ","
		<< "数量" << ","
		<< "成交金额" << ","
		<< "持仓量" << ","
		<< "今收盘" << ","
		<< "本次结算价" << ","
		<< "涨停板价" << ","
		<< "跌停板价" << ","
		<< "昨虚实度" << ","
		<< "今虚实度" << ","
		<< "最后修改时间" << ","
		<< "最后修改毫秒" << ","
		<< "申买价一" << ","
		<< "申买量一" << ","
		<< "申卖价一" << ","
		<< "申卖量一" << ","
		<< "申买价二" << ","
		<< "申买量二" << ","
		<< "申卖价二" << ","
		<< "申卖量二" << ","
		<< "申买价三" << ","
		<< "申买量三" << ","
		<< "申卖价三" << ","
		<< "申卖量三" << ","
		<< "申买价四" << ","
		<< "申买量四" << ","
		<< "申卖价四" << ","
		<< "申卖量四" << ","
		<< "申买价五" << ","
		<< "申买量五" << ","
		<< "申卖价五" << ","
		<< "申卖量五" << ","
		<< "当日均价" << ","
		<< "业务日期"
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
