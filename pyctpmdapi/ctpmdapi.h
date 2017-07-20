#pragma once


#include <string>
#include <queue>



#include <boost/python/dict.hpp>
#include <boost/thread.hpp>
#include <boost/any.hpp>


#include "ThostFtdcMdApi.h"


#define ONFRONTCONNECTED 1
#define ONFRONTDISCONNECTED 2
#define ONHEARTBEATWARNING 3
#define ONRSPUSERLOGIN 4
#define ONRSPUSERLOGOUT 5
#define ONRSPERROR 6
#define ONRSPSUBMARKETDATA 7
#define ONRSPUNSUBMARKETDATA 8
#define ONRSPSUBFORQUOTERSP 9
#define ONRSPUNSUBFORQUOTERSP 10
#define ONRTNDEPTHMARKETDATA 11
#define ONRTNFORQUOTERSP 12

using namespace std;
using namespace boost;
using namespace boost::python;

class PyLock
{
private:
	PyGILState_STATE gil_state;

public:
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}

	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};


struct Task
{
	int task_name;
	boost::any task_data;
	boost::any task_error;
	int task_id;
	bool task_last;
};

template <typename Data>
class ConcurrentQueue
{
private:
	std::queue<Data> event_queue;
	mutable boost::mutex the_mutex;
	boost::condition_variable the_condition_variable;
public:
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		event_queue.push(data);
		lock.unlock();
		the_condition_variable.notify_one();
	}

	bool empty() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return event_queue.empty();
	}

	Data fetch()
	{
		boost::mutex::scoped_lock lock(the_mutex);

		while (event_queue.empty())
		{
			the_condition_variable.wait(lock);
		}
		Data event = event_queue.front();
		event_queue.pop();
		return event;
	}
};


class MdApi : public CThostFtdcMdSpi
{
private:
	CThostFtdcMdApi* api;
	boost::thread* task_thread;
	ConcurrentQueue<Task> task_queue;

public:
	MdApi()
	{
		boost::function0<void> f = boost::bind(&MdApi::processTask, this);
		boost::thread t(f);
		this->task_thread = &t;
	}

	~MdApi()
	{

	}

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();


	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	//心跳响应
	//@param nTimeLapse 距离上次接收报文时间
	virtual void OnHeartBeatWarning(int nTimeLapse);


	//登陆请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	//登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//错误应答
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	//订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅询价应答
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅询价应答
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	//task 事件处理

	void processTask();

	void processFrontConnected(Task task);

	void processFrontDisconnected(Task task);

	void processHeartBeatWarning(Task task);

	void processRspUserLogin(Task task);

	void processRspUserLogout(Task task);

	void processRspError(Task task);

	void processRspSubMarketData(Task task);

	void processRspUnSubMarketData(Task task);

	void processRspSubForQuoteRsp(Task task);

	void processRspUnSubForQuoteRsp(Task task);

	void processRtnDepthMarketData(Task task);

	void processRtnForQuoteRsp(Task task);

	//需要在python中重写的回调处理函数

	virtual void onFrontConnected() = 0;

	virtual void onFrontDisconnected(int i) = 0;

	virtual void onHeartBeatWarning(int i) = 0;

	virtual void onRspUserLogin(boost::python::dict data, boost::python::dict error, int id, bool last) = 0;

	virtual void onRspUserLogout(boost::python::dict data, boost::python::dict error, int id, bool last) = 0;

	virtual void onRspError(boost::python::dict error, int id, bool last) = 0;

	virtual void onRspSubMarketData(boost::python::dict data, boost::python::dict error, int id, bool last) = 0;

	virtual void onRspUnSubMarketData(dict data, dict error, int id, bool last) = 0;

	virtual void onRspSubForQuoteRsp(dict data, dict error, int id, bool last) = 0;

	virtual void onRspUnSubForQuoteRsp(dict data, dict error, int id, bool last) = 0;

	virtual void onRtnDepthMarketData(dict data) = 0;

	virtual void onRtnForQuoteRsp(dict data) = 0;

	//主动函数

	void createFtdcMdApi(string pszFlowPath = "");

	void release() const;

	void init() const;

	int join() const;

	int exit();

	string getTradingDay() const;

	void registerFront(string pszFrontAddress) const;

	int subscribeMarketData(string instrumentID) const;

	int unSubscribeMarketData(string instrumentID) const;

	int subscribeForQuoteRsp(string instrumentID) const;

	int unSubscribeForQuoteRsp(string instrumentID) const;

	int reqUserLogin(dict req, int nRequestID) const;

	int reqUserLogout(dict req, int nRequestID) const;
};