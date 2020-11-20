#include "pch.h"

#include "../ConsoleApplication1/communication.hpp"
#include "../ConsoleApplication1/application.hpp"

#include "../ConsoleApplication1/application.cpp"
#include "../ConsoleApplication1/communication.cpp"


TEST(TestCaseName, TestName) 
{
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);

	application app;
	communication comm;


	app._communication = &comm;
	EXPECT_EQ(app.run(), 1);
}
