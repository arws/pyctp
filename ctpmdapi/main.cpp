#include "mdapi.h"
#include <Windows.h>


int main()
{
	auto api = MdApi();
	api.createFtdcMdApi("");
	api.registerFront("tcp://180.168.146.187:10031");
	api.init();
	Sleep(2000);
	api.reqUserLogin(1);
	api.subscribeMarketData("IF1708");
	api.join();
	system("pause");
	return 0;
}
