#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/DataClasses/IndexedList.hpp"
#include <assert.h>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iterator>

void PrintInternalState(ssGUI::IndexedList<int>& list)
{
    ssLOG_LINE("Stored Items")
    int index = 0;
    for(auto it = list.InternalList.begin(); it != list.InternalList.end(); it++)
    {
        ssLOG_LINE(index<<": "<<(*it));
        index++;
    }

    /*ssLOG_LINE("Queue Cache");
    for(auto it = list.QueuedItems.begin(); it != list.QueuedItems.end(); it++)
    {
        ssLOG_LINE(""<<(*it).first<<", "<<*(*it).second);
    }*/

    ssLOG_LINE("Sequential Cache");
    /*if(list.SequentialCacheValid)
    {
        ssLOG_LINE(""<<list.SequentialCache.first<<", "<<*list.SequentialCache.second);
    }
    else
    {
        ssLOG_LINE(""<<list.SequentialCache.first);
    }
    */

    ssLOG_LINE("Map Cache");
    for(auto it = list.MappedItems.begin(); it != list.MappedItems.end(); it++)
    {
        ssLOG_LINE(""<<it->first<<", "<<*it->second);
    }
}

void ConstructTestIndexedList(ssGUI::IndexedList<int>& output, int startNum, int endNum)
{
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
    //TODO
}

void AssignmentOperatorTest()
{
    //TODO
}

void AtAndSquareOperatorTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 6);

    assert(test[3] == 4);
    assert(test.At(3) == 4);

    ssLOG_LINE("\u2705");
}

void SizeTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 20);
    assert(test.Size() == 20);

    ssLOG_LINE("\u2705");
}

void EmptyTest()
{
    ssGUI::IndexedList<int> test;
    assert(test.Empty());
    ssLOG_LINE("\u2705");
}

void RemoveSingleTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Remove(4); //Remove number 5   (1, 2, 3, 4, 6, 7, 8, 9, 10)
    test.Remove(0); //Remove number 1   (2, 3, 4, 6, 7, 8, 9, 10)

    // for(int i = 0; i < test.Size(); i++)
    // {
        // int t = test[i];
        // ssLOG_LINE(i<<"th element: "<<t);
    // }
    
    assert(test[0] == 2);
    assert(test[1] == 3);
    assert(test[2] == 4);
    assert(test[3] == 6);
    assert(test[4] == 7);
    assert(test[5] == 8);
    assert(test[6] == 9);
    ssLOG_LINE("\u2705");
}

void RemoveRangeTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Remove(3, 6);      //Remove index 3 to 6 (exclusive) (1, 2, 3, 7, 8, 9, 10)

    assert(test[0] == 1);
    assert(test[1] == 2);
    assert(test[2] == 3);
    assert(test[3] == 7);
    assert(test[4] == 8);
    assert(test[5] == 9);
    assert(test[6] == 10);
    ssLOG_LINE("\u2705");
}

void ClearTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Clear();
    assert(test.Empty());
    ssLOG_LINE("\u2705");
}

void AddSingleTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Add(11);
    int t = test[10];
    assert(test[10] == 11);
    ssLOG_LINE("\u2705");
}

void AddRangeWithIndexedListTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    ssGUI::IndexedList<int> test2;
    ConstructTestIndexedList(test, 11, 15);
    test.Add(test2);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    ssLOG_LINE("\u2705");
}

void AddRangeWithVectorTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    std::vector<int> test2;
    ConstructTestVector(test2, 11, 15);
    test.Add(test2);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    ssLOG_LINE("\u2705");
}

void AddRangeWithListTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    std::list<int> test2;
    ConstructTestList(test2, 11, 15);
    test.Add(test2);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    ssLOG_LINE("\u2705");
}

void AddRangeWithArrayTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    int test2[] = {11, 12, 13, 14, 15};
    test.Add(test2, 5);

    assert(test[10] == 11);
    assert(test[11] == 12);
    assert(test[12] == 13);
    assert(test[13] == 14);
    assert(test[14] == 15);
    ssLOG_LINE("\u2705");
}

void AddSingleWithIndexTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Add(11, 5);

    assert(test[4] == 5);
    assert(test[5] == 11);
    assert(test[6] == 6);

    ssLOG_LINE("\u2705");
}

void AddRangeWithIndexedListWithIndexTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    ssGUI::IndexedList<int> test2;
    ConstructTestIndexedList(test2, 11, 15);
    test.Add(test2, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    ssLOG_LINE("\u2705");
}

void AddRangeWithVectorWithIndexTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    ssGUI::IndexedList<int> test2;
    ConstructTestIndexedList(test2, 11, 15);
    test.Add(test2, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    ssLOG_LINE("\u2705");
}

void AddRangeWithListWithIndexTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    std::list<int> test2;
    ConstructTestList(test2, 11, 15);
    test.Add(test2, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    ssLOG_LINE("\u2705");
}

void AddRangeWithArrayWithIndexTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    int test2[] = {11, 12, 13, 14, 15};
    test.Add(test2, 5, 7);                         //1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 8, 9, 10

    assert(test[6] == 7);
    assert(test[7] == 11);
    assert(test[8] == 12);
    assert(test[11] == 15);
    assert(test[12] == 8);
    ssLOG_LINE("\u2705");
}

void SwapTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);
    test.Swap(3, 7);

    assert(test[2] == 3);
    assert(test[3] == 8);
    assert(test[4] == 5);

    assert(test[6] == 7);
    assert(test[7] == 4);
    assert(test[8] == 9);

    ssLOG_LINE("\u2705");
}

void FrontTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);

    assert(test.Front() == 1);
    ssLOG_LINE("\u2705");
}

void BackTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);

    assert(test.Back() == 10);
    ssLOG_LINE("\u2705");
}

void QueueCacheTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);

    int t = test[5];            //This won't hit cache
    assert(t == 6);

    int t2 = test[5];           //This will hit cache
    assert(t2 == 6);
    
    int t3 = test[6];           //This will hit sequential cache
    assert(t3 == 7);
    
    int t4 = test[4];           //This will hit sequential cache
    assert(t4 == 5);
    ssLOG_LINE("\u2705");
}

void MapCacheTest()
{
    ssGUI::IndexedList<int> test;
    ConstructTestIndexedList(test, 1, 10);

    int t = test[5];            //This won't hit cache
    assert(t == 6);

    int t2 = test[7];           //This will hit map cache
    assert(t2 == 8);
    
    int t3 = test[3];           //This will hit map cache
    assert(t3 == 4);
    
    ssLOG_LINE("\u2705");
}

void ConstructTestObjects(ssGUI::IndexedList<int>& indexedList, std::list<int>& list, std::vector<int>& vector, int numOfItems)
{
    //Intial list, vector and IndexedList
    ConstructTestIndexedList(indexedList, 1, numOfItems);

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

void CacheBenchmarkTest()
{
    ssGUI::IndexedList<int> test;
    std::list<int> listTest;
    std::vector<int> vectorTest;

    const int containerSize = 10000000;
    const int insertContainerSize = 10;
    const int numOfTrials = 1;
    const int readTimes = 1000;
    const int insertTimes = 100;
    ConstructTestObjects(test, listTest, vectorTest, containerSize);

    //Items to be inserted
    std::list<int> listToInsert(insertContainerSize, -1);
    std::vector<int> vectorToInsert(insertContainerSize, -1);

    //Initiate random
    srand(time(NULL));


    //================================================================================================================
    //Random Access Benchmark
    //================================================================================================================
    ssLOG_LINE("Random access "<<readTimes<<" times with size "<<containerSize);
    //List
    {
        std::vector<double> listTimes;
        ssLOG_LINE("List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            auto it = listTest.begin();
            for(int j = 0; j < readTimes; j++)
            {
                it = listTest.begin();
                std::advance(it, rand() % containerSize);
                int num = *it;
            }

            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            listTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(listTimes)<<", "<<GetMin(listTimes)<<", "<<GetMax(listTimes)<<" ms");
    }
    
    //Vector
    {
        std::vector<double> vectorTimes;
        ssLOG_LINE("Vector:");
        for(int i = 0; i < numOfTrials; i++)
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < readTimes; j++)
            {
                int num = vectorTest[rand() % containerSize];
            }

            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            vectorTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(vectorTimes)<<", "<<GetMin(vectorTimes)<<", "<<GetMax(vectorTimes)<<" ms");
    }

    //IndexedList
    {
        std::vector<double> indexedListTimes;
        ssLOG_LINE("Indexed List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < readTimes; j++)
            {
                int num = test[rand() % containerSize];
            }

            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            indexedListTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(indexedListTimes)<<", "<<GetMin(indexedListTimes)<<", "<<GetMax(indexedListTimes)<<" ms");
    }

    //================================================================================================================
    //Random (linear search) Insert Benchmark
    //================================================================================================================

    ssLOG_LINE();
    ssLOG_LINE();
    ssLOG_LINE("Random (linear search) insert "<<insertContainerSize<<" elements "<<insertTimes<<" times with size "<<containerSize);
    //List
    {
        std::vector<double> listTimes;
        ssLOG_LINE("List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            listTest.clear();
            ConstructTestList(listTest, 1, containerSize);
            int targetNum = 0;
            auto it = listTest.begin();
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                targetNum = rand() % vectorTest.size() + 1;
                it = listTest.begin();

                for(int k = 0; k < vectorTest.size(); k++)
                {
                    if(*it == targetNum)
                    {
                        listTest.insert(it, listToInsert.begin(), listToInsert.end());
                        break;
                    }
                }
                
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            listTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(listTimes)<<", "<<GetMin(listTimes)<<", "<<GetMax(listTimes)<<" ms");
    }
    
    //Vector
    {
        std::vector<double> vectorTimes;
        ssLOG_LINE("Vector:");
        for(int i = 0; i < numOfTrials; i++)
        {
            vectorTest.clear();
            ConstructTestVector(vectorTest, 1, containerSize);
            int targetNum = 0;
            auto t1 = std::chrono::high_resolution_clock::now();
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
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            vectorTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(vectorTimes)<<", "<<GetMin(vectorTimes)<<", "<<GetMax(vectorTimes)<<" ms");
    }

    //IndexedList
    {
        std::vector<double> indexedListTimes;
        ssLOG_LINE("Indexed List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            test.Clear();
            ConstructTestIndexedList(test, 1, containerSize);
            int targetNum = 0;
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                targetNum = rand() % test.Size() + 1;
                //Pretending linear search
                for(int k = 0; k < test.Size(); k++)
                {
                    if(test[k] == targetNum)
                    {
                        test.Add(listToInsert, k);
                        break;
                    }
                }
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            indexedListTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(indexedListTimes)<<", "<<GetMin(indexedListTimes)<<", "<<GetMax(indexedListTimes)<<" ms");
    }

    //================================================================================================================
    //Front Insert Benchmark
    //================================================================================================================

    ssLOG_LINE();
    ssLOG_LINE();
    ssLOG_LINE("Front insert "<<insertContainerSize<<" elements "<<insertTimes<<" times with size "<<containerSize);
    //List
    {
        std::vector<double> listTimes;
        ssLOG_LINE("List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            listTest.clear();
            ConstructTestList(listTest, 1, containerSize);
            auto it = listTest.begin();
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                listTest.insert(it, listToInsert.begin(), listToInsert.end());                
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            listTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(listTimes)<<", "<<GetMin(listTimes)<<", "<<GetMax(listTimes)<<" ms");
    }
    
    //Vector
    {
        std::vector<double> vectorTimes;
        ssLOG_LINE("Vector:");
        for(int i = 0; i < numOfTrials; i++)
        {
            vectorTest.clear();
            ConstructTestVector(vectorTest, 1, containerSize);
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                vectorTest.insert(vectorTest.begin(), vectorToInsert.begin(), vectorToInsert.end());
            }
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            vectorTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(vectorTimes)<<", "<<GetMin(vectorTimes)<<", "<<GetMax(vectorTimes)<<" ms");
    }

    //IndexedList
    {
        std::vector<double> indexedListTimes;
        ssLOG_LINE("Indexed List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            test.Clear();
            ConstructTestIndexedList(test, 1, containerSize);
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                test.Add(listToInsert, 0);
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            indexedListTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(indexedListTimes)<<", "<<GetMin(indexedListTimes)<<", "<<GetMax(indexedListTimes)<<" ms");
    }

    //================================================================================================================
    //Middle Insert Benchmark
    //================================================================================================================

    ssLOG_LINE();
    ssLOG_LINE();
    ssLOG_LINE("Middle insert "<<insertContainerSize<<" elements "<<insertTimes<<" times with size "<<containerSize);
    //List
    {
        std::vector<double> listTimes;
        ssLOG_LINE("List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            listTest.clear();
            ConstructTestList(listTest, 1, containerSize);
            auto it = listTest.begin();
            std::advance(it, containerSize / 2);
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                listTest.insert(it, listToInsert.begin(), listToInsert.end());                
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            listTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(listTimes)<<", "<<GetMin(listTimes)<<", "<<GetMax(listTimes)<<" ms");
    }
    
    //Vector
    {
        std::vector<double> vectorTimes;
        ssLOG_LINE("Vector:");
        for(int i = 0; i < numOfTrials; i++)
        {
            vectorTest.clear();
            ConstructTestVector(vectorTest, 1, containerSize);
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                vectorTest.insert(vectorTest.begin()+containerSize/2, vectorToInsert.begin(), vectorToInsert.end());
            }
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            vectorTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(vectorTimes)<<", "<<GetMin(vectorTimes)<<", "<<GetMax(vectorTimes)<<" ms");
    }

    //IndexedList
    {
        std::vector<double> indexedListTimes;
        ssLOG_LINE("Indexed List:");
        for(int i = 0; i < numOfTrials; i++)
        {
            test.Clear();
            ConstructTestIndexedList(test, 1, containerSize);
            int addedAmount = 0;
            auto t1 = std::chrono::high_resolution_clock::now();
            for(int j = 0; j < insertTimes; j++)
            {
                test.Add(listToInsert, containerSize / 2 + addedAmount);
                addedAmount += listToInsert.size();
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            indexedListTimes.push_back(ms_double.count());
        }

        ssLOG_LINE("Time took with running "<<numOfTrials<<" times (average, min, max): \t\t"<<GetAverage(indexedListTimes)<<", "<<GetMin(indexedListTimes)<<", "<<GetMax(indexedListTimes)<<" ms");
    }

    //================================================================================================================
    //Sequential Read Benchmark
    //================================================================================================================

}


int main()
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
    // AddRangeWithIndexedListTest();
    // AddRangeWithVectorTest();
    // AddRangeWithListTest();
    // AddRangeWithArrayTest();
    // AddSingleWithIndexTest();
    // AddRangeWithIndexedListWithIndexTest();
    // AddRangeWithVectorWithIndexTest();
    // AddRangeWithListWithIndexTest();
    // AddRangeWithArrayWithIndexTest();
    // SwapTest();
    // FrontTest();
    // BackTest();
    // QueueCacheTest();
    // MapCacheTest();
    CacheBenchmarkTest();
}