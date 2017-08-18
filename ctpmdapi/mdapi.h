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


	//��������
	void connect();

	void close();

	void subscribe();

	void login();

	void logout();

	int join();

	void process_task();

	void push_task(Task task);

	string getTradingDay();

	//�ص�����

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected() ;

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason) ;

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) ;


	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ѯ��Ӧ��
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ȡ������ѯ��Ӧ��
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

	///ѯ��֪ͨ
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;


private:
	CThostFtdcMdApi* _api;
	moodycamel::BlockingConcurrentQueue<Task> _queue;
	shared_ptr<thread> _t;
	int request_id;

	void release();

	//�����̵߳Ĵ���
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