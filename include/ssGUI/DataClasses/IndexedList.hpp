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
            inline typename std::list<T>::iterator GetIterator(int index)
            {
                const int maxMapSize = MapCacheSize;
                const int maxQueueSize = QueueCacheSize;

                //Check out of range
                if(index < 0 || index >= InternalList.size())
                    throw std::out_of_range(std::string("Invalid index in IndexedList (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                //If size is less than 5, just iterate to find it
                if(InternalList.size() < 5)
                {
                    auto it = InternalList.begin();
                    for(int i = 0; i < index; i++)
                        it++;
                    
                    return it;
                }

                typename std::list<T>::iterator foundIt = InternalList.end();

                //Find in queue first, sequential read will hit this one
                for(auto it = QueuedItems.begin(); it != QueuedItems.end(); it++)
                {
                    int diff = index - (*it).first;

                    if(abs(diff) > 1)
                        continue;
                    
                    //If item is already in queue cache
                    if(diff == 0)
                        return (*it).second;
                    //If item is next to current queue cache item
                    else if(diff > 0)
                    {
                        auto tempIt = (*it).second;
                        foundIt = ++tempIt;
                        break;
                    }
                    //If item is before current queue cache item
                    else
                    {
                        auto tempIt = (*it).second; 
                        foundIt = --tempIt;
                        break;
                    }
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
                        int absMapDiff = 1000000000;//abs(mapDiff);
                        
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
                  
                //Add to MappedItems
                if(!isInMap)
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

                //Add to QueuedItems
                //Check max size is reach
                if(QueuedItems.size() >= maxQueueSize)
                    QueuedItems.erase(--QueuedItems.end());
                
                QueuedItems.push_front(std::make_pair(index, foundIt));
                
                return foundIt;
            };
        
        // protected:
        public:
            std::list<T> InternalList;
            std::map<int, typename std::list<T>::iterator> MappedItems;
            std::deque<std::pair<int, typename std::list<T>::iterator>> QueuedItems;    //New --> Old
        
            int MapCacheSize = 20;
            int QueueCacheSize = 5;

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

            inline void SetQueueCacheSize(int size)
            {
                QueueCacheSize = size;
            }

            inline int GetQueueCacheSize()
            {
                return QueueCacheSize;
            }

            inline T& At(int index)
            {
                return (*this)[index];
            };

            inline std::size_t Size() const
            {
                return InternalList.size();
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

                //Update queue cache
                auto queueItemToRemove = QueuedItems.end();
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first == index)
                        queueItemToRemove = queueIt;
                    else if((*queueIt).first > index)
                        (*queueIt).first -= 1;
                }

                if(queueItemToRemove != QueuedItems.end())
                    QueuedItems.erase(queueItemToRemove);
                
                //Update map cache
                auto mapIt = MappedItems.lower_bound(index);
                auto mapItemToRemove = MappedItems.end();
                if(mapIt != MappedItems.end())
                {
                    //Remove the target item to remove first
                    if(mapIt->first == index)
                    {
                        mapItemToRemove = mapIt;
                        mapIt++;
                    }
                    
                    //Then shift all the subsequent items behind it
                    for(; mapIt != MappedItems.end(); mapIt++)
                        MappedItems[mapIt->first - 1] = mapIt->second;
                    
                    MappedItems.erase(--MappedItems.end());
                }
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

                //Update queue cache
                int moveAmount = endExclusiveIndex - startIndex;
                std::vector<typename std::deque<std::pair<int, typename std::list<T>::iterator>>::iterator> queueItemsToRemove;
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    //Record all the items that are in the range to remove
                    if((*queueIt).first >= startIndex && (*queueIt).first < endExclusiveIndex)
                        queueItemsToRemove.push_back(queueIt);
                    //Shift all the items that are after the range
                    else if((*queueIt).first >= endExclusiveIndex)
                        (*queueIt).first -= moveAmount;
                }

                //Remove all the items needed
                if(!queueItemsToRemove.empty())
                {
                    for(int i = 0; i < queueItemsToRemove.size(); i++)
                        QueuedItems.erase(queueItemsToRemove[i]);
                }

                //Update map cache
                auto mapIt = MappedItems.lower_bound(startIndex);
                std::vector<typename std::map<int, typename std::list<T>::iterator>::iterator> mapItemsToRemove;
                if(mapIt != MappedItems.end())
                {
                    for(; mapIt != MappedItems.end(); mapIt++)
                    {
                        //Record all the items that are in the range to remove
                        if(mapIt->first >= startIndex && mapIt->first < endExclusiveIndex)
                            mapItemsToRemove.push_back(mapIt);
                        //Shift alll the items that are after the range
                        else if(mapIt->first >= endExclusiveIndex)
                            MappedItems[mapIt->first - moveAmount] = mapIt->second;
                    }

                    //Find alll the old items with incorrect index and record them 
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
            };

            inline void Clear()
            {
                InternalList.clear();
                MappedItems.clear();
                QueuedItems.clear();
            };

            inline void Add(T& obj)
            {
                InternalList.push_back(obj);
            };

            inline void Add(T&& obj)
            {
                InternalList.push_back(obj);
            };

            inline void Add(IndexedList<T>& objs)
            {
                InternalList.insert(InternalList.end(), objs.InternalList.begin(), objs.InternalList.end());
            };

            inline void Add(std::vector<T>& objs)
            {
                for(int i = 0; i < objs.size(); i++)
                    InternalList.push_back(objs[i]);
            };

            inline void Add(std::list<T>& objs)
            {
                InternalList.insert(InternalList.end(), objs.begin(), objs.end());
            };

            inline void Add(T objs[], size_t len)
            {
                for(int i = 0; i < len; i++)
                    InternalList.push_back(objs[i]);
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

                //Update queue cache
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += 1;
                }

                //Update map cache
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + 1] = mapIt->second;
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

                //Update queue cache
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += 1;
                }

                //Update map cache
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + 1] = mapIt->second;
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

                //Update queue cache
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += objs.Size();
                }

                //Update map cache
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + objs.Size()] = mapIt->second;
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

                //Update queue cache
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += objs.size();
                }

                //Update map cache
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + objs.size()] = mapIt->second;
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

                //Update queue cache
                /*for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += objs.size();
                }*/

                //Update map cache
                /*
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + objs.size()] = mapIt->second;
                        if(mapIt == foundIt)
                        {
                            MappedItems.erase(mapIt);
                            break;
                        }

                        auto itToRemove = mapIt;
                        mapIt--;
                        MappedItems.erase(itToRemove);
                    }
                }*/
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

                //Update queue cache
                for(auto queueIt = QueuedItems.begin(); queueIt != QueuedItems.end(); queueIt++)
                {
                    if((*queueIt).first >= index)
                        (*queueIt).first += len;
                }

                //Update map cache
                auto foundIt = MappedItems.lower_bound(index);
                auto mapIt = --MappedItems.end(); 
                if(foundIt != MappedItems.end())
                {                    
                    //Shift all the items that are after the item just inserted
                    while(true)
                    {
                        MappedItems[mapIt->first + len] = mapIt->second;
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