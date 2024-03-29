#include <gtest/gtest.h>
#include "Level.h"
#include "Logger.h"
#include "CustomMacros.h"
#include "Object.h"
//#include "TestData/Level/Game/Scripts/TestScript.h"


class LevelTests : public ::testing::Test {
protected:
	void SetUp() override
	{
		Logger::AddOrGet("core");
		Logger::AddOrGet("levelTest");

		//make sure that if we get a error or above, we throw an exception.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);

		const testing::TestInfo* info = ::testing::UnitTest::GetInstance()->current_test_info();
		configDir = std::make_shared<ConfigDirectories>();
		configDir->RootSourceDirectory = { std::string{ SOURCE_DIR } };
		configDir->RootBinaryDirectory = { std::string{ BINARY_DIR } };
		configDir->EngineSourceDirectory = { configDir->RootSourceDirectory / "Engine" };
		configDir->PluginSourceDirectory = { configDir->EngineSourceDirectory / "Plugins" };
#if defined(WIN32) || defined(__WIN32)
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" };
#else
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" / "Linux" };
#endif
		configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "Level" / "Game" };
		configDir->RootGameBinaryDirectory = { configDir->RootBinaryDirectory / "Engine" / "Tests" / "Game" };
		configDir->RootGameBinaryDirectory /= std::string(info->test_case_name()) +  info->name();
	}

	void TearDown() override
	{
		//Reset threshold for other tests.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);

		if(Logger::Find("script"))
		{
			Logger::Remove(Logger::Get("script"));
		}
	}

	const std::string testObjectName = "SOME_OBJECT_NAME";
	const std::string nonExistentObjectName = "SOME_OBJECT_NAME_THAT_DOES_NOT_EXIST";
	std::shared_ptr<ConfigDirectories> configDir;
};

TEST_F(LevelTests, Creator)
{
	EXPECT_NO_FATAL_FAILURE(CreateLevel(configDir.get()));
}

TEST_F(LevelTests, CreatorNoConfigDirectory)
{
	EXPECT_THROW_WITH_MESSAGE(CreateLevel(nullptr), LoggerException, 
		"The directories struct has not been passed in. This is necessary to find a lot of important directories.\nThis will probably crash!");
}

TEST_F(LevelTests, Start)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
}

TEST_F(LevelTests, Update)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Update(0.016f);
}

TEST_F(LevelTests, UpdateWithNegativeDeltaTime)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Update(-0.016f);
}

TEST_F(LevelTests, FixedUpdate)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->FixedUpdate();
}

TEST_F(LevelTests, Delete)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Delete();
}

TEST_F(LevelTests, CreateObjectThatExists)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->CreateObject("TestScript", "SOME_TEST_SCRIPT");
}

TEST_F(LevelTests, CreateObjectThatDoesNotExist)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	EXPECT_THROW_WITH_MESSAGE(lvl->CreateObject("SCRIPT_THAT_DOES_NOT_EXIST", "SOME_TEST_SCRIPT"), LoggerException, 
		"Could not create object with type SCRIPT_THAT_DOES_NOT_EXIST, because the type does not exist!");
}

TEST_F(LevelTests, CreateObjectThatDoesNotExistNoThrow)
{
	Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);
	
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	ObjectHandle hndl = lvl->CreateObject("SCRIPT_THAT_DOES_NOT_EXIST", "SOME_TEST_SCRIPT");
	EXPECT_EQ(hndl.index, std::numeric_limits<size_t>::max());

	Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);
}

//TEST_F(LevelTests, GetObjectPointer)
//{
//	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
//	lvl->Start();
//	ObjectHandle hndl = lvl->CreateObject("TestScript", "SOME_TEST_SCRIPT_BY_POINTER");
//	Object* obj = lvl->GetObjectPointer(hndl);
//	EXPECT_STREQ(obj->GetName(), "SOME_TEST_SCRIPT_BY_POINTER");
//	std::string proof = dynamic_cast<TestScript*>(obj)->TestFunction();
//	EXPECT_STREQ(proof.c_str(), "PROOF_OF_TEST_SCRIPT");
//}

TEST_F(LevelTests, GetObjectPointerNonExistentHandle)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->CreateObject("TestScript", "SOME_TEST_SCRIPT_BY_POINTER");
	EXPECT_THROW_WITH_MESSAGE(lvl->GetObjectPointer({11111}), LoggerException, "Object not found returning nullptr!");
}

TEST_F(LevelTests, GetObjectWithName)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->CreateObject("TestScript", testObjectName.c_str());
	ObjectHandle obj = lvl->GetObjectWithName(testObjectName.c_str());
}

TEST_F(LevelTests, GetObjectWithNameThatDoesNotExist)
{
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->CreateObject("TestScript", testObjectName.c_str());
	ObjectHandle obj;
	std::string message = "Could not find object with name \"" + nonExistentObjectName + "\"";
	EXPECT_THROW_WITH_MESSAGE(lvl->GetObjectWithName(nonExistentObjectName.c_str()), LoggerException, message.c_str());
}

TEST_F(LevelTests, GetObjectWithNameThatDoesNotExistNoThrow)
{
	Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);
	
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->CreateObject("TestScript", testObjectName.c_str());
	ObjectHandle obj = {0};
	EXPECT_NO_FATAL_FAILURE(obj = lvl->GetObjectWithName(nonExistentObjectName.c_str()));
	EXPECT_EQ(obj.index, std::numeric_limits<size_t>::max());

	Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);
}