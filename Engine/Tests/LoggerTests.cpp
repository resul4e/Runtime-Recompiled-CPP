#include <gtest/gtest.h>
#include "Logger.h"

class LoggerTests : public ::testing::Test {
protected:
	void SetUp() override {
		//If a logger can't find the requested name it defaults back to "core". Maybe not the best solution
		//but for now we just add core to logger.
		Logger::AddOrGet("core");
	}
};

TEST_F(LoggerTests, Add)
{
	EXPECT_NO_FATAL_FAILURE(Logger::Add("Add"));
	EXPECT_STREQ(Logger::Get("Add").name, "Add");
}

TEST_F(LoggerTests, AddEmptyString)
{
	EXPECT_STREQ(Logger::Add("").name,  "core" );
}

TEST_F(LoggerTests, AddNullptr)
{
	EXPECT_STREQ(Logger::Add(nullptr).name, "core");
}

TEST_F(LoggerTests, Get)
{
	Logger::Add("Get");
	EXPECT_STREQ(Logger::Get("Get").name, "Get");
}

TEST_F(LoggerTests, GetEmptyString)
{
	EXPECT_STREQ(Logger::Get("").name, "core");
}

TEST_F(LoggerTests, GetNullptr)
{
	EXPECT_STREQ(Logger::Get(nullptr).name, "core");
}

TEST_F(LoggerTests, AddOrGet)
{
	EXPECT_STREQ(Logger::AddOrGet("AddOrGet").name, "AddOrGet");
}

TEST_F(LoggerTests, AddOrGetExisting)
{
	EXPECT_NO_FATAL_FAILURE(Logger::Add("AddOrGetExisting"));
	EXPECT_STREQ(Logger::AddOrGet("AddOrGetExisting").name, "AddOrGetExisting");
}

TEST_F(LoggerTests, AddOrGetEmptyString)
{
	EXPECT_STREQ(Logger::AddOrGet("").name, "core");
}

TEST_F(LoggerTests, AddOrGetNullptr)
{
	EXPECT_STREQ(Logger::AddOrGet(nullptr).name, "core");
}

TEST_F(LoggerTests, LOG_TRACE)
{
	LOG_TRACE(Logger::Get("core"), "SOME_{}_TRACE", "TEST");
}