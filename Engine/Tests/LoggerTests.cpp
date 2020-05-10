#include <gtest/gtest.h>
#include "Logger.h"

class LoggerTests : public ::testing::Test {
protected:
	void SetUp() override {
		//If a logger can't find the requested name it defaults back to "core". Maybe not the best solution
		//but for now we just add core to logger.
		Logger::AddOrGet("core");
		Logger::AddOrGet("loggerTest");
	}

	void TearDown() override
	{
		Logger::SetExceptionThreshold(Logger::Get("loggerTest"), Logger::ExceptionThreshold::NEVER);
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);
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
	EXPECT_NO_FATAL_FAILURE(LOG_TRACE(Logger::Get("loggerTest"), "SOME_{}_TRACE", "TEST"));
}

TEST_F(LoggerTests, LOG_INFO)
{
	EXPECT_NO_FATAL_FAILURE(LOG_INFO(Logger::Get("loggerTest"), "SOME_{}_INFO", "TEST"));
}

TEST_F(LoggerTests, LOG_DEBUG)
{
	EXPECT_NO_FATAL_FAILURE(LOG_DEBUG(Logger::Get("loggerTest"), "SOME_{}_DEBUG", "TEST"));
}

TEST_F(LoggerTests, LOG_WARN)
{
	EXPECT_NO_FATAL_FAILURE(LOG_WARN(Logger::Get("loggerTest"), "SOME_{}_WARN", "TEST"));
}

TEST_F(LoggerTests, WarnWithThreshold)
{
	Logger::SetExceptionThreshold(Logger::Get("loggerTest"), Logger::ExceptionThreshold::WARN_AND_ABOVE);
	EXPECT_THROW(LOG_WARN(Logger::Get("loggerTest"), "SOME_{}_WARN", "TEST"), LoggerException);
}

TEST_F(LoggerTests, LOG_ERROR)
{
	EXPECT_NO_FATAL_FAILURE(LOG_ERROR(Logger::Get("loggerTest"), "SOME_{}_ERROR", "TEST"));
}

TEST_F(LoggerTests, ErrorWithThreshold)
{
	Logger::SetExceptionThreshold(Logger::Get("loggerTest"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);
	EXPECT_THROW(LOG_ERROR(Logger::Get("loggerTest"), "SOME_{}_ERROR", "TEST"), LoggerException);
}

TEST_F(LoggerTests, ErrorWithWarnThreshold)
{
	Logger::SetExceptionThreshold(Logger::Get("loggerTest"), Logger::ExceptionThreshold::WARN_AND_ABOVE);
	EXPECT_THROW(LOG_ERROR(Logger::Get("loggerTest"), "SOME_{}_ERROR", "TEST"), LoggerException);
}

TEST_F(LoggerTests, LOG_CRITICAL)
{
	EXPECT_NO_FATAL_FAILURE(LOG_CRITICAL(Logger::Get("loggerTest"), "SOME_{}_CRITICAL", "TEST"));
}

TEST_F(LoggerTests, CriticalWithThreshold)
{
	Logger::SetExceptionThreshold(Logger::Get("loggerTest"), Logger::ExceptionThreshold::CRITICAL_ONLY);
	EXPECT_THROW(LOG_CRITICAL(Logger::Get("loggerTest"), "SOME_{}_CRITICAL", "TEST"), LoggerException);
}