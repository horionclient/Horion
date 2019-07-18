#include "TestCommand.h"



TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	// HTTP TEST

#ifdef HTTP_TEST
	{
		WinHttpClient client(L"http://ip-api.com/line/");

		// Send HTTP request, a GET request by default.
		client.SendHttpRequest();

		// The response header.
		wstring httpResponseHeader = client.GetResponseHeader();

		logF("%S", httpResponseHeader.c_str());

		// The response content.
		wstring httpResponseContent = client.GetResponseContent();

		logF("%S", httpResponseContent.c_str());


	}
#endif


	return true;
}
