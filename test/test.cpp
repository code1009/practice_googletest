/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <Windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../ConsoleApplication1/communication.hpp"
#include "../ConsoleApplication1/application.hpp"

#include "../ConsoleApplication1/application.cpp"
#include "../ConsoleApplication1/communication.cpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class mock_communication : public communication
{
public:
	MOCK_METHOD(int, communicate, (), (override));
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class fake_communication : public communication
{
public:
	virtual int communicate(void)
	{
		printf("fake_communication::communicate()\n");

		return 100;
	}
};

class mock_fake_communication : public communication
{
public:
	MOCK_METHOD(int, communicate, (), (override));

public:
	void delegate_to_fake(void)
	{
		ON_CALL(*this, communicate()).WillByDefault(testing::Invoke(&_fake, &fake_communication::communicate));
	}

private:
	fake_communication _fake;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
TEST(TestCaseNameA, TestName0) 
{
	application app;
	communication comm;

	app._communication = &comm;
	app.run();
}

//===========================================================================
TEST(TestCaseNameB, TestName1) 
{
	//-----------------------------------------------------------------------
	application app;
	mock_communication comm;


	app._communication = &comm;


	//-----------------------------------------------------------------------
#if 0
	EXPECT_CALL(comm, communicate())
		.WillOnce(testing::Return(9)); // 가짜로 9을 리턴
	app.run();

	app.run(); // FAILED
#endif


	//-----------------------------------------------------------------------
	EXPECT_CALL(comm, communicate())
		.WillRepeatedly(testing::Return(10)); // 가짜로 10을 리턴

	app.run();


	//-----------------------------------------------------------------------
	EXPECT_CALL(comm, communicate())
		.WillRepeatedly(testing::Return(11)); // 가짜로 11을 리턴

	app.run();
}

//===========================================================================
TEST(TestCaseNameC, TestName2) 
{
	//-----------------------------------------------------------------------
	application app;
	mock_communication comm;


	//-----------------------------------------------------------------------
	EXPECT_CALL(comm, communicate());

	EXPECT_EQ(0, comm.communicate()); // Mock Class 내용을 실행
}

//===========================================================================
TEST(TestCaseNameD, TestName3) 
{
	//-----------------------------------------------------------------------
	application app;
	mock_communication comm;


	app._communication = &comm;


	//-----------------------------------------------------------------------
	// 함수 수행 횟수 확인
	EXPECT_CALL(comm, communicate())
		.Times(testing::AtLeast(5)); // 6이면 OK, 7이면 FAILED, 5이면 OK


	//-----------------------------------------------------------------------
	// 6 번 Mock 호출
	int i;


	for (i = 0; i < 5; i++)
	{
		app.run();
		Sleep(100);
	}

	EXPECT_EQ(0, comm.communicate()); // Mock 호출
}

//===========================================================================
TEST(TestCaseNameD, TestName4) 
{
	//-----------------------------------------------------------------------
	application app;
	mock_communication comm;


	app._communication = &comm;


	//-----------------------------------------------------------------------
	// 함수 수행 횟수 확인
	EXPECT_CALL(comm, communicate())
		.Times(testing::AtMost(6)); // 6이면 OK, 5이면 FAILED, 7이면 OK


	//-----------------------------------------------------------------------
	// 6 번 Mock 호출
	int i;


	for (i = 0; i < 5; i++)
	{
		app.run();
		Sleep(100);
	}

	EXPECT_EQ(0, comm.communicate()); // Mock 호출
}

//===========================================================================
TEST(TestCaseNameE, TestName5) 
{
	//-----------------------------------------------------------------------
	application app;
	mock_fake_communication comm;


	app._communication = &comm;


	//-----------------------------------------------------------------------
	EXPECT_CALL(comm, communicate())
		.Times(testing::AtLeast(1));

	
	//-----------------------------------------------------------------------
	comm.delegate_to_fake();


	//-----------------------------------------------------------------------
	int n;


	n = app.run();


	EXPECT_EQ(n, 100);
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	testing::InitGoogleMock(&argc, argv);

	return RUN_ALL_TESTS();
}
