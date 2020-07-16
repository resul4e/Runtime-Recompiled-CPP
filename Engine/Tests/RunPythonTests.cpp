#include <gtest/gtest.h>
#include "RunPython.h"
#include "FileSystem.h"
#include <iostream>
#include <fstream>

bool LoadTestFile(RCP::fs::path _filePath)
{
    std::string line;
    std::ifstream myfile(_filePath.string());
    if (myfile.is_open())
    {
        getline(myfile, line);
        EXPECT_STREQ(line.c_str(), "SOME_CONTENT_IN_THE_FILE");
        myfile.close();
        return true;
    }
    else
    {
        return false;
    }
}

TEST(RunPythonTests, RunWhereFileExists)
{
    RCP::fs::path scriptPath = RCP::fs::path(TEST_DATA_DIR) / "RunPython" / "TestPythonScript.py";
    RCP::fs::path outputPath = RCP::fs::path(TEST_DATA_DIR) / "RunPython" / "PYTHON_OUTPUT_FILE.txt";

	bool output = RunPython::Run(scriptPath.string());
	EXPECT_NE(output, false);
    EXPECT_NE(LoadTestFile(outputPath), false);
    RCP::fs::remove(outputPath);
}

TEST(RunPythonTests, RunWhereFileDoesNotExist)
{
    RCP::fs::path scriptPath = RCP::fs::path(TEST_DATA_DIR) / "RunPython" / "PYTHON_SCRIPT_THAT_DOES_NOT_EXIST.py";

    bool output = RunPython::Run(scriptPath.string());
    EXPECT_EQ(output, false);
}