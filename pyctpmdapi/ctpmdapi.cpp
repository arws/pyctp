#include "ctpmdapi.h"

#include <boost/python/object.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/locale.hpp>

#include <boost/python.hpp>

void getStr(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<std::string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			strcpy_s(value, strlen(buffer) + 1, buffer);
		}
	}
}

void MdApi::OnFrontConnected()
{
	Task task = Task();
	task.task_name = ONFRONTCONNECTED;
	this->task_queue.push(task);
}

void MdApi::OnFrontDisconnected(int nReason)
{
	Task task = Task();
	task.task_name = ONFRONTDISCONNECTED;
	task.task_id = nReason;
	this->task_queue.push(task);
}

void MdApi::OnHeartBeatWarning(int nTimeLapse)
{
	Task task = Task();
	task.task_name = ONHEARTBEATWARNING;
	task.task_id = nTimeLapse;
	this->task_queue.push(task);
}

void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGIN;

	if (pRspUserLogin)
	{
		task.task_data = *pRspUserLogin;
	}
	else
	{
		CThostFtdcRspUserLoginField empty_data = CThostFtdcRspUserLoginField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
}

void MdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGOUT;

	if (pUserLogout)
	{
		task.task_data = *pUserLogout;
	}
	else
	{
		CThostFtdcUserLogoutField empty_data = CThostFtdcUserLogoutField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}

	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
}

void MdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPERROR;

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void MdApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPSUBMARKETDATA;

	if (pSpecificInstrument)
	{
		task.task_data = *pSpecificInstrument;
	}
	else
	{
		CThostFtdcSpecificInstrumentField empty_data = CThostFtdcSpecificInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void MdApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPUNSUBMARKETDATA;

	if (pSpecificInstrument)
	{
		task.task_data = *pSpecificInstrument;
	}
	else
	{
		CThostFtdcSpecificInstrumentField empty_data = CThostFtdcSpecificInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void MdApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPSUBFORQUOTERSP;

	if (pSpecificInstrument)
	{
		task.task_data = *pSpecificInstrument;
	}
	else
	{
		CThostFtdcSpecificInstrumentField empty_data = CThostFtdcSpecificInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void MdApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Task task = Task();
	task.task_name = ONRSPUNSUBFORQUOTERSP;

	if (pSpecificInstrument)
	{
		task.task_data = *pSpecificInstrument;
	}
	else
	{
		CThostFtdcSpecificInstrumentField empty_data = CThostFtdcSpecificInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	Task task = Task();
	task.task_name = ONRTNDEPTHMARKETDATA;

	if (pDepthMarketData)
	{
		task.task_data = *pDepthMarketData;
	}
	else
	{
		CThostFtdcDepthMarketDataField empty_data = CThostFtdcDepthMarketDataField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void MdApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	Task task = Task();
	task.task_name = ONRTNFORQUOTERSP;

	if (pForQuoteRsp)
	{
		task.task_data = *pForQuoteRsp;
	}
	else
	{
		CThostFtdcForQuoteRspField empty_data = CThostFtdcForQuoteRspField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

//回调处理
void MdApi::processTask()
{
	while (true)
	{
		Task task = this->task_queue.fetch();

		switch (task.task_name)
		{

		case ONFRONTCONNECTED:
		{
			this->processFrontConnected(task);
			break;
		}

		case ONFRONTDISCONNECTED:
		{
			this->processFrontDisconnected(task);
			break;
		}

		case ONHEARTBEATWARNING:
		{
			this->processHeartBeatWarning(task);
			break;
		}
		case ONRSPUSERLOGIN:
		{
			this->processRspUserLogin(task);
			break;
		}

		case ONRSPUSERLOGOUT:
		{
			this->processRspUserLogout(task);
			break;
		}

		case ONRSPERROR:
		{
			this->processRspError(task);
			break;
		}

		case ONRSPSUBMARKETDATA:
		{
			this->processRspSubMarketData(task);
			break;
		}

		case ONRSPUNSUBMARKETDATA:
		{
			this->processRspUnSubMarketData(task);
			break;
		}

		case ONRSPSUBFORQUOTERSP:
		{
			this->processRspSubForQuoteRsp(task);
			break;
		}

		case ONRSPUNSUBFORQUOTERSP:
		{
			this->processRspUnSubForQuoteRsp(task);
			break;
		}

		case ONRTNDEPTHMARKETDATA:
		{
			this->processRtnDepthMarketData(task);
			break;
		}

		case ONRTNFORQUOTERSP:
		{
			this->processRtnForQuoteRsp(task);
			break;
		}

		default:
			break;
		}
	}
}

void MdApi::processFrontConnected(Task task)
{
	PyLock lock;
	this->onFrontConnected();
}

void MdApi::processFrontDisconnected(Task task)
{
	PyLock lock;
	this->onFrontDisconnected(task.task_id);
}

void MdApi::processHeartBeatWarning(Task task)
{
	PyLock lock;
	this->onHeartBeatWarning(task.task_id);
}

void MdApi::processRspUserLogin(Task task)
{
	PyLock lock;
	CThostFtdcRspUserLoginField task_data = any_cast<CThostFtdcRspUserLoginField>(task.task_data);
	dict data;

	data["SessionID"] = task_data.SessionID;
	data["MaxOrderRef"] = boost::locale::conv::to_utf<char>(task_data.MaxOrderRef, std::string("GB2312"));
	data["LoginTime"] = boost::locale::conv::to_utf<char>(task_data.LoginTime, std::string("GB2312"));
	data["FFEXTime"] = boost::locale::conv::to_utf<char>(task_data.FFEXTime, std::string("GB2312"));
	data["SystemName"] = boost::locale::conv::to_utf<char>(task_data.SystemName, std::string("GB2312"));
	data["CZCETime"] = boost::locale::conv::to_utf<char>(task_data.CZCETime, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["DCETime"] = boost::locale::conv::to_utf<char>(task_data.DCETime, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["INETime"] = boost::locale::conv::to_utf<char>(task_data.INETime, std::string("GB2312"));
	data["SHFETime"] = boost::locale::conv::to_utf<char>(task_data.SHFETime, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);

	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspUserLogin(data, error, task.task_id, task.task_last);
}

void MdApi::processRspUserLogout(Task task)
{
	PyLock lock;
	CThostFtdcUserLogoutField task_data = any_cast<CThostFtdcUserLogoutField>(task.task_data);
	dict data;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspUserLogout(data, error, task.task_id, task.task_last);
}

void MdApi::processRspError(Task task)
{
	PyLock lock;
	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspError(error, task.task_id, task.task_last);
}

void MdApi::processRspSubMarketData(Task task)
{
	PyLock lock;
	CThostFtdcSpecificInstrumentField task_data = any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspSubMarketData(data, error, task.task_id, task.task_last);
}

void MdApi::processRspUnSubMarketData(Task task)
{
	PyLock lock;
	CThostFtdcSpecificInstrumentField task_data = any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspUnSubMarketData(data, error, task.task_id, task.task_last);
}

void MdApi::processRspSubForQuoteRsp(Task task)
{
	PyLock lock;
	CThostFtdcSpecificInstrumentField task_data = any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspSubForQuoteRsp(data, error, task.task_id, task.task_last);

}

void MdApi::processRspUnSubForQuoteRsp(Task task)
{
	PyLock lock;
	CThostFtdcSpecificInstrumentField task_data = any_cast<CThostFtdcSpecificInstrumentField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorID"] = task_error.ErrorID;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));

	this->onRspUnSubForQuoteRsp(data, error, task.task_id, task.task_last);
}

void MdApi::processRtnDepthMarketData(Task task)
{
	PyLock lock;
	CThostFtdcDepthMarketDataField task_data = any_cast<CThostFtdcDepthMarketDataField>(task.task_data);
	dict data;
	data["AskPrice5"] = task_data.AskPrice5;
	data["CurrDelta"] = task_data.CurrDelta;
	data["AskPrice2"] = task_data.AskPrice2;
	data["BidPrice4"] = task_data.BidPrice4;
	data["AveragePrice"] = task_data.AveragePrice;
	data["AskVolume4"] = task_data.AskVolume4;
	data["BidPrice1"] = task_data.BidPrice1;
	data["UpdateTime"] = boost::locale::conv::to_utf<char>(task_data.UpdateTime, std::string("GB2312"));
	data["AskPrice4"] = task_data.AskPrice4;
	data["PreOpenInterest"] = task_data.PreOpenInterest;
	data["LastPrice"] = task_data.LastPrice;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["BidPrice2"] = task_data.BidPrice2;
	data["BidPrice3"] = task_data.BidPrice3;
	data["HighestPrice"] = task_data.HighestPrice;
	data["AskPrice3"] = task_data.AskPrice3;
	data["BidVolume5"] = task_data.BidVolume5;
	data["ActionDay"] = boost::locale::conv::to_utf<char>(task_data.ActionDay, std::string("GB2312"));
	data["PreSettlementPrice"] = task_data.PreSettlementPrice;
	data["BidVolume4"] = task_data.BidVolume4;
	data["AskVolume2"] = task_data.AskVolume2;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["AskVolume3"] = task_data.AskVolume3;
	data["Volume"] = task_data.Volume;
	data["Turnover"] = task_data.Turnover;
	data["BidPrice5"] = task_data.BidPrice5;
	data["AskVolume5"] = task_data.AskVolume5;
	data["OpenPrice"] = task_data.OpenPrice;
	data["PreClosePrice"] = task_data.PreClosePrice;
	data["OpenInterest"] = task_data.OpenInterest;
	data["ClosePrice"] = task_data.ClosePrice;
	data["LowerLimitPrice"] = task_data.LowerLimitPrice;
	data["BidVolume3"] = task_data.BidVolume3;
	data["BidVolume2"] = task_data.BidVolume2;
	data["UpperLimitPrice"] = task_data.UpperLimitPrice;
	data["BidVolume1"] = task_data.BidVolume1;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["AskVolume1"] = task_data.AskVolume1;
	data["AskPrice1"] = task_data.AskPrice1;
	data["SettlementPrice"] = task_data.SettlementPrice;
	data["LowestPrice"] = task_data.LowestPrice;
	data["UpdateMillisec"] = task_data.UpdateMillisec;
	data["PreDelta"] = task_data.PreDelta;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	this->onRtnDepthMarketData(data);
}


void MdApi::processRtnForQuoteRsp(Task task)
{
	PyLock lock;
	CThostFtdcForQuoteRspField task_data = any_cast<CThostFtdcForQuoteRspField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["ForQuoteTime"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteTime, std::string("GB2312"));
	data["ActionDay"] = boost::locale::conv::to_utf<char>(task_data.ActionDay, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));

	this->onRtnForQuoteRsp(data);
}

//主动函数

void MdApi::createFtdcMdApi(string pszFlowPath)
{
	this->api = CThostFtdcMdApi::CreateFtdcMdApi("");
	this->api->RegisterSpi(static_cast<CThostFtdcMdSpi*>(this));
}

void MdApi::init() const
{
	this->api->Init();
}

void MdApi::release() const
{
	this->api->Release();
}

int MdApi::join() const
{
	int i = this->api->Join();
	return i;
}


string MdApi::getTradingDay() const
{
	string day = this->api->GetTradingDay();
	return day;
}

void MdApi::registerFront(string pszFrontAddress) const
{
	this->api->RegisterFront(const_cast<char *>(pszFrontAddress.c_str()));
}

int MdApi::subscribeMarketData(string instrumentID) const
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->SubscribeMarketData(req, 1);
	return i;
}

int MdApi::unSubscribeMarketData(string instrumentID) const
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->UnSubscribeMarketData(req, 1);
	return i;
}


int MdApi::subscribeForQuoteRsp(string instrumentID) const
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };
	int i = this->api->SubscribeForQuoteRsp(req, 1);
	return i;
}

int MdApi::unSubscribeForQuoteRsp(string instrumentID) const
{
	char* buffer = const_cast<char *>(instrumentID.c_str());
	char* req[1] = { buffer };;
	int i = this->api->UnSubscribeForQuoteRsp(req, 1);
	return i;
}

int MdApi::reqUserLogin(dict req, int nRequestID) const
{
	CThostFtdcReqUserLoginField login_req = CThostFtdcReqUserLoginField();
	memset(&login_req, 0, sizeof(login_req));
	getStr(req, "MacAddress", login_req.MacAddress);
	getStr(req, "UserProductInfo", login_req.UserProductInfo);
	getStr(req, "UserID", login_req.UserID);
	getStr(req, "TradingDay", login_req.TradingDay);
	getStr(req, "InterfaceProductInfo", login_req.InterfaceProductInfo);
	getStr(req, "BrokerID", login_req.BrokerID);
	getStr(req, "ClientIPAddress", login_req.ClientIPAddress);
	getStr(req, "OneTimePassword", login_req.OneTimePassword);
	getStr(req, "ProtocolInfo", login_req.ProtocolInfo);
	getStr(req, "Password", login_req.Password);
	int i = this->api->ReqUserLogin(&login_req, nRequestID);
	return i;
}

int MdApi::reqUserLogout(dict req, int nRequestID) const
{
	CThostFtdcUserLogoutField logout_req = CThostFtdcUserLogoutField();
	memset(&logout_req, 0, sizeof(logout_req));
	getStr(req, "UserID", logout_req.UserID);
	getStr(req, "BrokerID", logout_req.BrokerID);
	int i = this->api->ReqUserLogout(&logout_req, nRequestID);
	return i;
};


struct MdApiWrap : MdApi, wrapper<MdApi>
{
	virtual void onFrontConnected()
	{
		try
		{
			this->get_override("onFrontConnected");
		}
		catch (error_already_set const&)
		{
			PyErr_Print();
		}
	}

	virtual void onFrontDisconnected(int i)
	{
		try
		{
			this->get_override("onFrontDisconnected");
		}
		catch (error_already_set const&)
		{
			PyErr_Print();
		}
	}

	virtual void onHeartBeatWarning(int i)
	{
		try
		{
			this->get_override("onHeartBeatWarning")(i);
		}
		catch (error_already_set const&)
		{
			PyErr_Print();
		}
	}

	virtual void onRspError(dict data, int id, bool last)
	{
		try
		{
			this->get_override("onRspError")(data, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUserLogin(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUserLogin")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUserLogout(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUserLogout")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspSubMarketData(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspSubMarketData")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUnSubMarketData(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUnSubMarketData")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspSubForQuoteRsp(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspSubForQuoteRsp")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUnSubForQuoteRsp(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUnSubForQuoteRsp")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtnDepthMarketData(dict data)
	{
		try
		{
			this->get_override("onRtnDepthMarketData")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtnForQuoteRsp(dict data)
	{
		try
		{
			this->get_override("onRtnForQuoteRsp")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
};

BOOST_PYTHON_MODULE(pyctpmdapi)
{
	PyEval_InitThreads();	//导入时运行，保证先创建GIL

	class_<MdApiWrap, noncopyable>("MdApi")
		.def("createFtdcMdApi", &MdApiWrap::createFtdcMdApi)
		.def("release", &MdApiWrap::release)
		.def("init", &MdApiWrap::init)
		.def("join", &MdApiWrap::join)
		.def("exit", &MdApiWrap::exit)
		.def("getTradingDay", &MdApiWrap::getTradingDay)
		.def("registerFront", &MdApiWrap::registerFront)
		.def("subscribeMarketData", &MdApiWrap::subscribeMarketData)
		.def("unSubscribeMarketData", &MdApiWrap::unSubscribeMarketData)
		.def("subscribeForQuoteRsp", &MdApiWrap::subscribeForQuoteRsp)
		.def("unSubscribeForQuoteRsp", &MdApiWrap::unSubscribeForQuoteRsp)
		.def("reqUserLogin", &MdApiWrap::reqUserLogin)
		.def("reqUserLogout", &MdApiWrap::reqUserLogout)

		.def("onFrontConnected", pure_virtual(&MdApiWrap::onFrontConnected))
		.def("onFrontDisconnected", pure_virtual(&MdApiWrap::onFrontDisconnected))
		.def("onHeartBeatWarning", pure_virtual(&MdApiWrap::onHeartBeatWarning))
		.def("onRspError", pure_virtual(&MdApiWrap::onRspError))
		.def("onRspUserLogin", pure_virtual(&MdApiWrap::onRspUserLogin))
		.def("onRspUserLogout", pure_virtual(&MdApiWrap::onRspUserLogout))
		.def("onRspSubMarketData", pure_virtual(&MdApiWrap::onRspSubMarketData))
		.def("onRspUnSubMarketData", pure_virtual(&MdApiWrap::onRspUnSubMarketData))
		.def("onRtnDepthMarketData", pure_virtual(&MdApiWrap::onRtnDepthMarketData))
		.def("onRspSubForQuoteRsp", pure_virtual(&MdApiWrap::onRspSubForQuoteRsp))
		.def("onRspUnSubForQuoteRsp", pure_virtual(&MdApiWrap::onRspUnSubForQuoteRsp))
		.def("onRtnForQuoteRsp", pure_virtual(&MdApiWrap::onRtnForQuoteRsp))
		;
};