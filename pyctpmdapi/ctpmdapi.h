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

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();


	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	//������Ӧ
	//@param nTimeLapse �����ϴν��ձ���ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);


	//��½������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	//�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	//��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����ѯ��Ӧ��
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ������ѯ��Ӧ��
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///ѯ��֪ͨ
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	//task �¼�����

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

	//��Ҫ��python����д�Ļص�������

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

	//��������

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