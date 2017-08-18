#pragma once


#include <string>
#include <direct.h>
#include "ThostFtdcMdApi.h"
#include "blockingconcurrentqueue.h"
#include "boost/any.hpp"

#include "FileUtil.h"

#include "Config.h"
using namespace std;

enum task_name
{
	ON_FRONT_CONNECTED,
	ON_FRONT_DISCONNECTED,
	ON_HEARTBEAT_WARNING,
	ON_RSP_USRLOGIN,
	ON_RSP_USRLOGOUT,
	ON_RSP_ERR,
	ON_RSP_SUBMARKET_DATA,
	ON_RSP_UNSUBMARKET_DATA,
	ON_RSP_SUB_FOR_QUOTE_RSP,
	ON_RSP_UNSUB_FOR_QUOTE_RSP,
	ON_RTN_DEPTH_MARKET_DATA,
	ON_RTN_FOR_QUOTE_RSP,
	EMPTY_TASK
};

class Task
{
public:
	Task()
	{
		name = EMPTY_TASK;
		task_data = nullptr;
		task_error = nullptr;
		request_id = 0;
		is_last = true;
	}
	Task(task_name name, boost::any task_data):name(name), task_data(task_data)
	{
		task_error = nullptr;
		request_id = 0;
		is_last = true;
	}
	task_name name;
	boost::any task_data;
	boost::any task_error;
	int request_id;
	bool is_last;
};


class MdApi : public CThostFtdcMdSpi
{


public:
	MdApi()
	{
		request_id = 0;
	}

	virtual ~MdApi()
	{
		this->_t->detach();
		//this->t->join();
	}

	void init();


	//主动函数
	void connect();

	void close();

	void subscribe();

	void login();

	void logout();

	int join();

	void process_task();

	void push_task(Task task);

	string getTradingDay();

	//回调部分

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected() ;

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason) ;

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse) ;


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅询价应答
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅询价应答
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;


private:
	CThostFtdcMdApi* _api;
	moodycamel::BlockingConcurrentQueue<Task> _queue;
	shared_ptr<thread> _t;
	int request_id;

	void release();

	//单独线程的处理
	void process_on_front_connected(Task task);
		 
	void process_on_front_disconneted(Task task);
		 
	void process_on_heartbeat_warning(Task task);
		 
	void process_on_rsp_userlogin(Task task);
		 
	void process_on_rsp_user_logout(Task task);
		 
	void process_on_rsp_error(Task task);
		 
	void process_on_rsp_submarket_data(Task task);
		 
	void process_on_rsp_unsubmarket_data(Task task);
		 
	void process_on_rsp_sub_for_quote_rsp(Task task);
		 
	void process_on_rsp_unsub_for_quote_rsp(Task task);

	void process_on_rtn_depth_market_data(Task task);
		 
	void process_on_rtn_for_quote_rsp(Task task);

	void insert_head(fstream& f);
	void insert_tick(CThostFtdcDepthMarketDataField& market_data, fstream& f);


	void insert_data(CThostFtdcDepthMarketDataField market_data, string filename);
};