// #include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/DataClasses/SegmentedVector.hpp"
#include <iostream>
#include <assert.h>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <list>
#include <functional>

#define PRINT_LINE(debugText)\
{\
    std::cout<<debugText<<"\n";\
}


void PrintInternalState(ssGUI::SegmentedVector<int>& vec)
{
    /*PRINT_LINE("Stored Items")
    int index = 0;

    for(int i = 0; i < vec.InternalVectors.size(); i++)
    {
        PRINT_LINE("segment vector["<<i<<"]: ");
        
        for(int j = 0; j < vec.InternalVectors[i].size(); j++)
        {
            PRINT_LINE("["<<index<<"]: "<<vec.InternalVectors[i][j]);
            index++;
        }
    }

    PRINT_LINE("Sequential Cache");
    if(vec.SequentialCacheValid)
    {
        // PRINT_LINE(""<<vec.SequentialCacheIndex<<", "<<*vec.SequentialCache);
        PRINT_LINE(""<<vec.SequentialCacheIndex<<", "<<vec.InternalVectors[vec.SequentialCacheVectorIndex][vec.SequentialCacheInnerVectorIndex]);
    }*/
}

void ConstructTestSegmentedVector(ssGUI::SegmentedVector<int>& output, int startNum, int endNum, int segmentSize)
{
    if(segmentSize > 0)
        output.SetMaxSegmentVectorSize(segmentSize);

    for(int i = startNum; i <= endNum; i++)
        output.Add(i);
}

void ConstructTestVector(std::vector<int>& output, int startNum, int endNum)
{
    for(int i = startNum; i <= endNum; i++)
        output.push_back(i);
}

void ConstructTestList(std::list<int>& output, int startNum, int endNum)
{
    for(int i = startNum; i <= endNum; i++)
        output.push_back(i);
}

void CopyConstructorTest()
{
    std::vector<int> testVector;
    ConstructTestVector(testVector, 1, 10);

    ssGUI::SegmentedVector<int> test(testVector);

    assert(test[0] == 1);
    assert(test[1] == 2);
    assert(test[2] == 3);
    assert(test[3] == 4);
    assert(test[4] == 5);
    assert(test[5] == 6);
    assert(test[6] == 7);
    assert(test[7] == 8);
    assert(test[8] == 9);
    assert(test[9] == 10);

    ssGUI::SegmentedVector<int> test2({1,2,3,4,5,6,7,8,9,10});

    assert(test2[0] == 1);
    assert(test2[1] == 2);
    assert(test2[2] == 3);
    assert(test2[3] == 4);
    assert(test2[4] == 5);
    assert(test2[5] == 6);
    assert(test2[6] == 7);
    assert(test2[7] == 8);
    assert(test2[8] == 9);
    assert(test2[9] == 10);

    PRINT_LINE(__func__<<": \u2705");
}

void AssignmentOperatorTest()
{
    ssGUI::SegmentedVector<int> test;
    ssGUI::SegmentedVector<int> test2;

    ConstructTestSegmentedVector(test, 1, 10, -1);
    test2 = test;

    assert(test2[0] == 1);
    assert(test2[1] == 2);
    assert(test2[2] == 3);
    assert(test2[3] == 4);
    assert(test2[4] == 5);
    assert(test2[5] == 6);
    assert(test2[6] == 7);
    assert(test2[7] == 8);
    assert(test2[8] == 9);
    assert(test2[9] == 10);

    PRINT_LINE(__func__<<": \u2705");
}

void AtAndSquareOperatorTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 6, 4);

    assert(test[0] == 1);
    assert(test[1] == 2);
    assert(test[2] == 3);
    assert(test[3] == 4);
    assert(test[4] == 5);
    assert(test[5] == 6);
    assert(test.At(5) == 6);
    assert(test.At(4) == 5);
    assert(test.At(3) == 4);
    assert(test.At(2) == 3);
    assert(test.At(1) == 2);
    assert(test.At(0) == 1);

    PRINT_LINE(__func__<<": \u2705");
}

void SizeTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 20, 4);
    assert(test.Size() == 20);

    PRINT_LINE(__func__<<": \u2705");
}

void EmptyTest()
{
    ssGUI::SegmentedVector<int> test;
    assert(test.Empty());
    PRINT_LINE(__func__<<": \u2705");
}

void RemoveSingleTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Remove(4); //Remove number 5   (1, 2, 3, 4, 6, 7, 8, 9, 10)
    test.Remove(0); //Remove number 1   (2, 3, 4, 6, 7, 8, 9, 10)

    // for(int i = 0; i < test.Size(); i++)
    // {
        // int t = test[i];
        // PRINT_LINE(i<<"th element: "<<t);
    // }
    
    assert(test[0] == 2);
    assert(test[1] == 3);
    assert(test[2] == 4);
    assert(test[3] == 6);
    assert(test[4] == 7);
    assert(test[5] == 8);
    assert(test[6] == 9);
    PRINT_LINE(__func__<<": \u2705");
}

void RemoveRangeTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Remove(3, 6);      //Remove index 3 to 6 (exclusive) (1, 2, 3, 7, 8, 9, 10)

    assert(test[0] == 1);
    assert(test[1] == 2);
    assert(test[2] == 3);
    assert(test[3] == 7);
    assert(test[4] == 8);
    assert(test[5] == 9);
    assert(test[6] == 10);

    // PrintInternalState(test);

    test.Clear();
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Remove(3, 7);      //Remove index 3 to 7 (exclusive) (1, 2, 3, 8, 9, 10)
    assert(test[0] == 1);
    assert(test[1] == 2);
    assert(test[2] == 3);
    assert(test[3] == 8);
    assert(test[4] == 9);
    assert(test[5] == 10);

    PRINT_LINE(__func__<<": \u2705");
}

void ClearTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Clear();
    assert(test.Empty());
    PRINT_LINE(__func__<<": \u2705");
}

void AddSingleTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Add(11);
    int t = test[10];
    assert(test[10] == 11);
    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithSegmentedVectorTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    ssGUI::SegmentedVector<int> test2;
    ConstructTestSegmentedVector(test, 11, 15, 4);
    test.Add(test2);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithVectorTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    std::vector<int> test2;
    ConstructTestVector(test2, 11, 15);
    test.Add(test2);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithArrayTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    int test2[] = {11, 12, 13, 14, 15};
    test.Add(test2, 5);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    PRINT_LINE(__func__<<": \u2705");
}

void AddSingleWithIndexTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Add(11, 5);

    assert(test[4] == 5);
    assert(test[5] == 11);
    assert(test[6] == 6);

    PrintInternalState(test);

    test.Add(12, 11);
    PrintInternalState(test);
    assert(test[11] == 12);

    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithSegmentedVectorWithIndexTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    ssGUI::SegmentedVector<int> test2;
    ConstructTestSegmentedVector(test2, 11, 15, 4);
    test.Add(test2, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithVectorWithIndexTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    ssGUI::SegmentedVector<int> test2;
    ConstructTestSegmentedVector(test2, 11, 15, 4);
    test.Add(test2, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    PRINT_LINE(__func__<<": \u2705");
}

void AddRangeWithArrayWithIndexTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    int test2[] = {11, 12, 13, 14, 15};
    test.Add(test2, 5, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    PRINT_LINE(__func__<<": \u2705");
}

void SwapTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    test.Swap(3, 7);

    assert(test[2] == 3);
    assert(test[3] == 8);
    assert(test[4] == 5);

    assert(test[6] == 7);
    assert(test[7] == 4);
    assert(test[8] == 9);

    PRINT_LINE(__func__<<": \u2705");
}

void FrontTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);

    assert(test.Front() == 1);
    PRINT_LINE(__func__<<": \u2705");
}

void BackTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);

    assert(test.Back() == 10);
    PRINT_LINE(__func__<<": \u2705");
}

void IteratorTest()
{
    ssGUI::SegmentedVector<int> test;
    ConstructTestSegmentedVector(test, 1, 10, 4);
    
    //Test begin
    ssGUI::SegmentedVector<int>::Iterator it = test.Begin();
    assert(*it == 1);
    //Test it++
    it++;
    assert(*it == 2);
    //Test it--
    it--;
    assert(*it == 1);
    //Test end
    it = test.End();
    it--;
    assert(*it == 10);
    //Test --it
    assert(*(--it) == 9);
    //Test ++it
    assert(*(++it) == 10);
    //Test GetIterator
    assert(*test.GetIterator(5) == 6);

    //Test Whole iteration
    int num = 1;
    for(it = test.Begin(); it != test.End(); it++)
    {
        assert(*it == num);
        num++;
    }

    num = 10;
    for(it = --test.End(); it != test.Begin(); it--)
    {
        assert(*it == num);
        num--;
    }

    PRINT_LINE(__func__<<": \u2705");
}

void ConstructTestObjects(ssGUI::SegmentedVector<int>& SegmentedVector, std::list<int>& list, std::vector<int>& vector, int numOfItems)
{
    //Intial list, vector and SegmentedVector
    ConstructTestSegmentedVector(SegmentedVector, 1, numOfItems, -1);

    for(int i = 0; i < numOfItems; i++)
    {
        list.push_back(i+1);
        vector.push_back(i+1);
    }
}

double GetMin(const std::vector<double>& times)
{
    double min = times.at(0);

    for(int i = 0; i < times.size(); i++)
        min = min < times[i] ? min : times[i];
    
    return min;
}

double GetMax(const std::vector<double>& times)
{
    double max = times.at(0);

    for(int i = 0; i < times.size(); i++)
        max = max > times[i] ? max : times[i];
    
    return max;
}

double GetAverage(const std::vector<double>& times)
{
    double total = 0;

    for(int i = 0; i < times.size(); i++)
        total += times[i];
    
    return total/times.size();
}

void BenchmarkAction(std::string benchmarkTitle, int numOfTrials, std::function<void()> setup, std::function<void()> action, std::function<void()> cleanup)
{
    std::vector<double> runTimes;
    PRINT_LINE(benchmarkTitle);
    for(int i = 0; i < numOfTrials; i++)
    {
        setup();

        auto t1 = std::chrono::high_resolution_clock::now();

        action();

        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        runTimes.push_back(ms_double.count());

        cleanup();
    }

    PRINT_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(runTimes)<<", "<<GetMin(runTimes)<<", "<<GetMax(runTimes)<<" ms");
}

void CacheBenchmarkTest()
{
    ssGUI::SegmentedVector<int> test;
    std::list<int> listTest;
    std::vector<int> vectorTest;

    const int containerSize = 2621440;
    const int insertContainerSize = 1024;
    const int numOfTrials = 5;
    const int readTimes = 1000;
    const int insertTimes = 1000;

    //Items to be inserted
    std::list<int> listToInsert(insertContainerSize, -1);
    std::vector<int> vectorToInsert(insertContainerSize, -1);

    //Initiate random
    srand(time(NULL));

    //================================================================================================================
    //Random Access Benchmark
    //================================================================================================================
    PRINT_LINE("Random access "<<readTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");
    //List
    BenchmarkAction
    (
        "List: ", 
        numOfTrials, 
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
        },
        [&]()
        {
            auto it = listTest.begin();
            for(int j = 0; j < readTimes; j++)
            {
                it = listTest.begin();
                std::advance(it, rand() % containerSize);
                int num = *it;
            }
        },
        [&](){listTest.clear();}
    );
    
    //Vector
    BenchmarkAction
    (
        "Vector:",
        numOfTrials,
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < readTimes; j++)
                int num = vectorTest[rand() % containerSize];
        },
        [&](){vectorTest.clear();}
    );

    //SegmentedVector
    BenchmarkAction
    (
        "Segmented Vector:",
        numOfTrials,
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
        },
        [&]()
        {
            for(int j = 0; j < readTimes; j++)
                int num = test[rand() % containerSize];
        },
        [&]()
        {
            test.Clear();
        }
    );

    //================================================================================================================
    //Seqnetial Read Benchmark
    //================================================================================================================

    PRINT_LINE("");
    PRINT_LINE("");
    PRINT_LINE("Seqnetial Read whole container "<<readTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");
    int doSomething = 0;

    //List
    auto it = listTest.begin();
    BenchmarkAction
    (
        "List:",
        numOfTrials,
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < readTimes; j++)
            {
                for(it = listTest.begin(); it != listTest.end(); it++)
                    doSomething = *it;
            }
        },
        [&](){listTest.clear();}
    );

    //Vector
    BenchmarkAction
    (
        "Vector:",
        numOfTrials,
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < readTimes; j++)
            {
                for(int k = 0; k < vectorTest.size(); k++)
                    doSomething = vectorTest[k];
            }
        },
        [&](){vectorTest.clear();}
    );
    

    //SegmentedVector
    BenchmarkAction
    (
        "Segmented Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
        }, 
        [&]()
        {
            for(int j = 0; j < readTimes; j++)
            {
                for(int k = 0; k < test.Size(); k++)
                    doSomething = test[k];
            }
        }, 
        [&](){test.Clear();}
    );

    //================================================================================================================
    //Seqnetial Read + Random Insert (Linear Search) Benchmark
    //================================================================================================================

    PRINT_LINE("");
    PRINT_LINE("");
    PRINT_LINE("Seqnetial Read + Random Insert (Linear Search) "<<insertContainerSize<<" elements "<<insertTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");
    int targetNum = 0;
    
    //List
    listTest.clear();
    BenchmarkAction
    (
        "List:",
        numOfTrials,
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
            targetNum = 0;
            it = listTest.begin();
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
            {
                targetNum = rand() % listTest.size() + 1;

                for(it = listTest.begin(); it != listTest.end(); it++)
                {
                    if(*it == targetNum)
                    {
                        listTest.insert(it, listToInsert.begin(), listToInsert.end());
                        break;
                    }
                }
            }
        },
        [&](){listTest.clear();}
    );

    //Vector
    vectorTest.clear();
    BenchmarkAction
    (
        "Vector:",
        numOfTrials,
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
            targetNum = 0;
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
            {
                targetNum = rand() % vectorTest.size() + 1;
                for(int k = 0; k < vectorTest.size(); k++)
                {
                    if(vectorTest[k] == targetNum)
                    {
                        vectorTest.insert(vectorTest.begin() + k, vectorToInsert.begin(), vectorToInsert.end());
                        break;
                    }
                }

                //vectorTest.insert(vectorTest.begin() + rand() % vectorTest.size(), vectorToInsert.begin(), vectorToInsert.end());
            }
        },
        [&](){vectorTest.clear();}
    );
    

    //SegmentedVector
    test.Clear();
    BenchmarkAction
    (
        "Segmented Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
            targetNum = 0;
        }, 
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
            {
                targetNum = rand() % test.Size() + 1;
                for(int k = 0; k < test.Size(); k++)
                {
                    if(test[k] == targetNum)
                    {
                        test.Add(vectorToInsert, k);
                        break;
                    }
                }
            }
        }, 
        [&](){test.Clear();}
    );

    //================================================================================================================
    //Front Insert Benchmark
    //================================================================================================================

    PRINT_LINE("");
    PRINT_LINE("");
    PRINT_LINE("Front insert "<<insertContainerSize<<" elements "<<insertTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");
    //List
    it = listTest.begin();
    BenchmarkAction
    (
        "List:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
        }, 
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                listTest.insert(it, listToInsert.begin(), listToInsert.end());                
        }, 
        [&](){listTest.clear();}
    );
    
    //Vector
    BenchmarkAction
    (
        "Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
        }, 
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                vectorTest.insert(vectorTest.begin(), vectorToInsert.begin(), vectorToInsert.end());
        }, 
        [&](){vectorTest.clear();}
    );

    //SegmentedVector
    BenchmarkAction
    (
        "Segmented Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
        }, 
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                test.Add(vectorToInsert, 0);
        },
        [&](){test.Clear();}
    );

    //================================================================================================================
    //Middle Insert Benchmark
    //================================================================================================================

    PRINT_LINE("");
    PRINT_LINE("");
    PRINT_LINE("Middle insert "<<insertContainerSize<<" elements "<<insertTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");
    //List
    BenchmarkAction
    (
        "List:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
            it = listTest.begin();
            std::advance(it, containerSize / 2);
        }, 
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                listTest.insert(it, listToInsert.begin(), listToInsert.end());
        }, 
        [&](){listTest.clear();}
    );
    
    //Vector
    BenchmarkAction
    (
        "Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                vectorTest.insert(vectorTest.begin()+containerSize/2, vectorToInsert.begin(), vectorToInsert.end());
        },
        [&](){vectorTest.clear();}
    );

    //SegmentedVector
    BenchmarkAction
    (
        "Segmented Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                test.Add(vectorToInsert, containerSize / 2);
        },
        [&](){test.Clear();}
    );

    //================================================================================================================
    //Insert Back Benchmark
    //================================================================================================================
    PRINT_LINE("");
    PRINT_LINE("");
    PRINT_LINE("Insert back "<<insertContainerSize<<" elements "<<insertTimes<<" times with container size "<<containerSize<<" ("<<containerSize * sizeof(int) / (float)1024 / (float)1024<<" MB)");

    //List
    BenchmarkAction
    (
        "List:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestList(listTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                listTest.insert(listTest.end(), listToInsert.begin(), listToInsert.end());                
        },
        [&](){listTest.clear();}
    );

    //Vector
    BenchmarkAction
    (
        "Vector:",
        numOfTrials,
        [&]()
        {
            ConstructTestVector(vectorTest, 1, containerSize);
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                vectorTest.insert(vectorTest.end(), vectorToInsert.begin(), vectorToInsert.end());
        },
        [&](){vectorTest.clear();}
    );

    //SegmentedVector
    BenchmarkAction
    (
        "Segmented Vector:", 
        numOfTrials, 
        [&]()
        {
            ConstructTestSegmentedVector(test, 1, containerSize, -1);
        },
        [&]()
        {
            for(int j = 0; j < insertTimes; j++)
                test.Add(vectorToInsert);
        },
        [&](){test.Clear();}
    );
}

void RunTests()
{
    // CopyConstructorTest();
    // AssignmentOperatorTest();
    // AtAndSquareOperatorTest();
    // SizeTest();
    // EmptyTest();
    // RemoveSingleTest();
    // RemoveRangeTest();
    // ClearTest();
    // AddSingleTest();
    // AddRangeWithSegmentedVectorTest();
    // AddRangeWithVectorTest();
    // AddRangeWithArrayTest();
    AddSingleWithIndexTest();
    // AddRangeWithSegmentedVectorWithIndexTest();
    // AddRangeWithVectorWithIndexTest();
    // AddRangeWithArrayWithIndexTest();
    // SwapTest();
    // FrontTest();
    // BackTest();
    // IteratorTest();
}

void RunBenchmark()
{
    CacheBenchmarkTest();
}


int main()
{
    RunTests();
    // RunBenchmark();
}