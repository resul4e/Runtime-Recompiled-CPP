#pragma once
#include <gtest/gtest.h>

#define EXPECT_THROW_WITH_MESSAGE(test, exception, message) \
	EXPECT_THROW({\
	try\
	{\
		test;\
	}\
	catch (const exception& e)\
	{\
		EXPECT_STREQ(message, e.what());\
		throw;\
	}\
	}, exception);