#include "ssGUI/TestBase.hpp"

//[Variables Declaration]

SSGUI_TEST_INIT();

void SetUp()
{
    //[Variables Initialization]
}

void CleanUp()
{
    //[Variables Cleanup]
}

void DummyTest()
{
    //Perform some action...

    SSGUI_TEST_OUTPUT_ASSERT(   __func__+std::string("(Optional Info)"),    //Test name and info if more than 1 test
                                true);                                      //Evaluation
}

int main()
{
    SetUp();
    
    SSGUI_TEST_TITLE(SSGUI_TEST_FILE_NAME());
    
    try
    {
        DummyTest();
    }
    catch(...)
    {
        CleanUp();
        throw;
    }
    
    CleanUp();
    
    SSGUI_TEST_END();
}

