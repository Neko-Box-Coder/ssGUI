#ifndef SSGUI_INDEXED_LIST
#define SSGUI_INDEXED_LIST

#include "ssGUI//DebugAndBuild//ssGUIBuildAndDebugConfig.hpp"
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <limits>


//namespace: ssGUI
namespace ssGUI
{
    template<typename T>
    class IndexedList
    {
        private:
            inline typename std::list<T>::iterator GetIterator(size_t index)
            {
                //Check out of range
                if(index < 0 || index >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));
                
                const size_t maxMapSize = MapCacheSize;
                const size_t minCacheDistance = MinMapDistance; 

                //Check sequential cache
                typename std::list<T>::iterator foundIt = InternalList.end();
                if(SequentialCacheValid)
                {
                    int32_t diff = index - SequentialCache.Index;

                    if(diff == 0)
                        return SequentialCache.TargetIt;
                    else if(diff == 1)
                    {
                        auto it = SequentialCache.TargetIt;
                        foundIt = ++it;
                        SequentialCache = {index, foundIt};
                        return foundIt;
                    }
                    else if(diff == -1)
                    {
                        auto it = SequentialCache.TargetIt;
                        foundIt = --it;
                        SequentialCache = {index, foundIt};
                        return foundIt;
                    } 
                }

                //If size is less than 5, just iterate to find it
                if(foundIt == InternalList.end() && CurrentSize < 5)
                {
                    auto it = InternalList.begin();
                    for(int i = 0; i < index; i++)
                        it++;
                    
                    return it;
                }

                bool isInMap = false;
                //If we can't find it in queue, navigate to the closest item on map or front/back of list
                if(foundIt == InternalList.end())
                {                  
                    //Find distance from front
                    int frontDiff = index;

                    //Find distance from back
                    int backDiff = index - (InternalList.size() - 1);

                    //Check is front or back first
                    if(frontDiff == 0)
                    {
                        foundIt = InternalList.begin();
                        isInMap = true;
                    }
                    else if(backDiff == 0)
                    {
                        foundIt = --InternalList.end();
                        isInMap = true;
                    }
                    else
                    {
                        //Find distance from closest cache (same item or item after)
                        auto mapIt = MappedItems.lower_bound(index);
                        int mapDiff = mapIt == MappedItems.end() ? std::numeric_limits<int>::max() : index - mapIt->first;

                        //Traverse accordingly
                        int absFrontDiff = abs(frontDiff);
                        int absBackDiff = abs(backDiff);
                        int absMapDiff = abs(mapDiff);
                        
                        if(absFrontDiff < absBackDiff && absFrontDiff < absMapDiff)
                        {
                            auto it = InternalList.begin();
                            for(int i = 0; i < index; i++)
                                it++;

                            foundIt = it;
                        }
                        else if(absBackDiff < absFrontDiff && absBackDiff < absMapDiff)
                        {
                            auto it = --InternalList.end();
                            for(int i = 0; i < absBackDiff; i++)
                                it--;

                            foundIt = it;
                        }
                        else
                        {
                            if(mapDiff == 0)
                            {
                                foundIt = mapIt->second;
                                isInMap = true;
                            }
                            else if(mapDiff > 0)
                            {
                                auto it = mapIt->second;
                                for(int i = 0; i < absMapDiff; i++)
                                    it++;

                                foundIt = it;
                            }
                            else
                            {
                                auto it = mapIt->second;
                                for(int i = 0; i < absMapDiff; i++)
                                    it--;

                                foundIt = it;
                            }
                        }
                    }
                }

                //Add to seqential 
                SequentialCache = {index, foundIt};
                SequentialCacheValid = true;
                  
                //Add to MappedItems
                if(!isInMap)
                {
                    auto closestIt = MappedItems.lower_bound(index);
                    if(closestIt != MappedItems.end() && abs(index - closestIt->first) > minCacheDistance)
                    {
                        //Check max size is reach
                        if(MappedItems.size() >= maxMapSize)
                        {
                            int midPoint = (MappedItems.rbegin()->first + MappedItems.begin()->first) / 2;

                            if(index < midPoint)
                                MappedItems.erase(MappedItems.begin());
                            else
                                MappedItems.erase(--MappedItems.end());
                        }

                        MappedItems[index] = foundIt;
                    }
                }

                return foundIt;
            };

            inline void ShiftCache(size_t index, int32_t shiftAmount)
            {
                //Adding
                if(shiftAmount > 0)
                {
                    //Update sequential cache
                    if(SequentialCacheValid)
                        SequentialCache.Index += shiftAmount;

                    //Update map cache
                    auto foundIt = MappedItems.lower_bound(index);
                    if(foundIt != MappedItems.end())
                    {                    
                        auto mapIt = --MappedItems.end(); 
                        //Shift all the items that are after the item just inserted
                        while(true)
                        {
                            MappedItems[mapIt->first + shiftAmount] = mapIt->second;
                            if(mapIt == foundIt)
                            {
                                MappedItems.erase(mapIt);
                                break;
                            }

                            auto itToRemove = mapIt;
                            mapIt--;
                            MappedItems.erase(itToRemove);
                        }
                    }
                }
                //Removing
                else 
                {
                    size_t startIndex = index;
                    size_t endExclusiveIndex = index - shiftAmount;
                    
                    //Update sequential cache
                    if(SequentialCacheValid)
                    {
                        if(SequentialCache.Index >= startIndex && SequentialCache.Index < endExclusiveIndex)
                            SequentialCacheValid = false;
                        else
                            SequentialCache.Index += shiftAmount;
                    }

                    //Update map cache
                    auto mapIt = MappedItems.lower_bound(index);
                    if(mapIt != MappedItems.end())
                    {
                        std::vector<typename std::map<size_t, typename std::list<T>::iterator>::iterator> mapItemsToRemove;
                        for(; mapIt != MappedItems.end(); mapIt++)
                        {
                            //Record all the items that are in the range to remove
                            if(mapIt->first >= startIndex && mapIt->first < endExclusiveIndex)
                                mapItemsToRemove.push_back(mapIt);
                            //Shift alll the items that are after the range
                            else if(mapIt->first >= endExclusiveIndex)
                                MappedItems[mapIt->first + shiftAmount] = mapIt->second;
                        }

                        //Find all the old items with incorrect index and record them 
                        mapIt = MappedItems.lower_bound(InternalList.size());
                        for(; mapIt != MappedItems.end(); mapIt++)
                            mapItemsToRemove.push_back(mapIt);
                        
                        //Remove all the items needed
                        if(!mapItemsToRemove.empty())
                        {
                            for(int i = 0; i < mapItemsToRemove.size(); i++)
                                MappedItems.erase(mapItemsToRemove[i]);
                        }
                    }
                }
            }
        
        // protected:
        public:
            std::list<T> InternalList;
            std::map<size_t, typename std::list<T>::iterator> MappedItems;
            int MinMapDistance = 10;
            struct SimplePair
            {
                size_t Index;
                typename std::list<T>::iterator TargetIt;
            };
            SimplePair SequentialCache = {0, InternalList.end()};
            bool SequentialCacheValid = false;
            size_t CurrentSize = 0;
            int MapCacheSize = 100;

        public:
            inline IndexedList() = default;

            //TODO: Other constructors

            inline IndexedList(const IndexedList<T>& other)
            {
                InternalList = other.InternalList;
                
                for(auto it = other.MappedItems.begin(); it != other.MappedItems.end(); it++)
                    MappedItems[it->first] = GetIterator(it->first);
            };

            inline IndexedList<T>& operator=(const IndexedList<T>& other)
            {
                InternalList = other.InternalList;
                
                for(auto it = other.MappedItems.begin(); it != other.MappedItems.end(); it++)
                    MappedItems[it->first] = GetIterator(it->first);
                
                return *this;
            };

            inline T& operator[](int index)
            {
                return *GetIterator(index);
            };

            inline void SetMapCacheSize(int size)
            {
                MapCacheSize = size;
            }

            inline int GetMapCacheSize()
            {
                return MapCacheSize;
            }

            inline T& At(int index)
            {
                return (*this)[index];
            };

            inline std::size_t Size() const
            {
                return CurrentSize;
            };

            inline bool Empty() const
            {
                return InternalList.empty();
            };

            inline void Remove(int index)
            {
                //Check out of range
                if(index < 0 || index >= InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Call GetIterator
                auto it = GetIterator(index);

                //Remove it
                InternalList.erase(it);

                //Update cache
                ShiftCache(index, -1);

                CurrentSize--;
            };

            inline void Remove(int startIndex, int endExclusiveIndex)
            {
                //Check out of range
                if(startIndex < 0 || startIndex >= InternalList.size())
                    throw std::out_of_range(std::string("Invalid start index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(startIndex));

                if(endExclusiveIndex <= 0 || endExclusiveIndex > InternalList.size())
                    throw std::out_of_range(std::string("Invalid end index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(endExclusiveIndex));

                //Call GetIterator
                std::vector<typename std::list<T>::iterator> itemsToRemove;
                for(int i = startIndex; i < endExclusiveIndex; i++)
                    itemsToRemove.push_back(GetIterator(i));

                //Remove it 
                for(int i = 0; i < itemsToRemove.size(); i++)
                    InternalList.erase(itemsToRemove[i]);

                //Update cache
                int moveAmount = startIndex - endExclusiveIndex;
                ShiftCache(startIndex, moveAmount);

                CurrentSize += moveAmount;
            };

            inline void Clear()
            {
                InternalList.clear();
                MappedItems.clear();

                CurrentSize = 0;
            };

            inline void Add(T& obj)
            {
                InternalList.push_back(obj);
                CurrentSize++;
            };

            inline void Add(T&& obj)
            {
                InternalList.push_back(obj);
                CurrentSize++;
            };

            inline void Add(IndexedList<T>& objs)
            {
                InternalList.insert(InternalList.end(), objs.InternalList.begin(), objs.InternalList.end());
                CurrentSize += objs.Size();
            };

            inline void Add(std::vector<T>& objs)
            {
                for(int i = 0; i < objs.size(); i++)
                    InternalList.push_back(objs[i]);
                
                CurrentSize += objs.size();
            };

            inline void Add(std::list<T>& objs)
            {
                InternalList.insert(InternalList.end(), objs.begin(), objs.end());

                CurrentSize += objs.size();
            };

            inline void Add(T objs[], size_t len)
            {
                for(int i = 0; i < len; i++)
                    InternalList.push_back(objs[i]);
                
                CurrentSize += len;
            };

            inline void Add(T& obj, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(obj);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                InternalList.insert(it, obj);

                //Update cache
                ShiftCache(index, 1);

                CurrentSize++;
            };

            inline void Add(T&& obj, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(obj);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                InternalList.insert(it, obj);

                //Update cache
                ShiftCache(index, 1);

                CurrentSize++;
            };

            inline void Add(IndexedList<T>& objs, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(objs);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                InternalList.insert(it, objs.InternalList.begin(), objs.InternalList.end());

                //Update cache
                ShiftCache(index, objs.Size());

                CurrentSize += objs.Size();
            };

            inline void Add(std::vector<T>& objs, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(objs);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                for(int i = 0; i < objs.size(); i++)
                    InternalList.insert(it, objs[i]);

                //Update cache
                ShiftCache(index, objs.size());

                CurrentSize += objs.size();
            };

            inline void Add(std::list<T>& objs, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(objs);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                InternalList.insert(it, objs.begin(), objs.end());

                //Update cache
                ShiftCache(index, objs.size());

                CurrentSize += objs.size();
            };

            inline void Add(T objs[], size_t len, int index)
            {
                //Check out of range
                if(index < 0 || index > InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //Check if it is adding to the end
                if(index == InternalList.size())
                {
                    Add(objs, len);
                    return;
                }

                //Call GetIterator
                auto it = GetIterator(index);

                //Add it
                for(int i = 0; i < len; i++)
                    InternalList.insert(it, objs[i]);

                //Update cache
                ShiftCache(index, len);

                CurrentSize += len;
            };

            inline void Swap(int index, int index2)
            {
                //Check out of range
                if(index < 0 || index >= InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));
                if(index2 < 0 || index2 >= InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index2));
                
                std::swap(*GetIterator(index), *GetIterator(index2));
            };

            inline T& Front()
            {
                return InternalList.front();
            };

            inline T& Back()
            {
                return InternalList.back();
            };
    };
}

#endif