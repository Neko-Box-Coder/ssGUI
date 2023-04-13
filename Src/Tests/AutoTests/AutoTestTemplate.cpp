#include "ssTest.hpp"

//[Variables Declaration]

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        //[Variables Initialization]
    }

    ssTEST_CLEAN_UP
    {
        //[Variables Cleanup]
    }

    ssTEST("DummyTest")
    {
        //[Perform some action...]

        ssTEST_OUTPUT_ASSERT("Optional Info", true);
    }

    ssTEST_END();
}
