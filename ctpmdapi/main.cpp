#include "mdapi.h"
#include "tdapi.h"
#include <Windows.h>

//investerID:092433
//brokerID:9999
//password:zm19641

//TradeFront               MarketFront
//180.168.146.187:10000   180.168.146.187:10010
//180.168.146.187:10001   180.168.146.187:10011
//218.202.237.33 :10002   218.202.237.33 :10012
//
//180.168.146.187:10030   180.168.146.187:10031 (7*24)


INITIALIZE_EASYLOGGINGPP

int main()
{
	el::Configurations log_config("log-config.ini");
	el::Loggers::reconfigureAllLoggers(log_config);
	MdApi api;
	api.init();
	api.connect();
	//tdapi api;
	//api.init();
	//api.connect();
	system("pause");
	api.close();
	system("pause");
	return 0;
}
