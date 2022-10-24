#ifndef SSGUI_SEGMENTED_VECTOR
#define SSGUI_SEGMENTED_VECTOR

#include "ssGUI//DebugAndBuild//ssGUIBuildAndDebugConfig.hpp"
#include <vector>
#include <utility>      //Swap
#include <stdexcept>    //Exception
#include <cstdlib>      //Abs      
#include <algorithm>    //Move
#include <iterator>
#include <string>

//namespace: ssGUI
namespace ssGUI
{
    template<typename T>
    class SegmentedVector
    {
        public:
            //class: ssGUI::SegmentedVector::Iterator
            //This is a Bidirection iterator that allows sequential read for <SegmentedVector> withouot interfering the cache of it.
            //Any changes to the <SegmentedVector> will invalidate the iterator.
            class Iterator
            {
                private:
                    size_t GlobalIndex;
                    size_t VectorIndex;
                    size_t InnerVectorSize;
                    size_t InnerVectorSizeMinusOne;

                    size_t InnerVectorIndex;
                    SegmentedVector<T>* TargetSegmentedVector;
                    T* ItemPt;

                public:
                    using iterator_category = std::bidirectional_iterator_tag;      //Too much work for Random Access iterator :(
                    using difference_type   = std::ptrdiff_t;
                    using value_type        = T;
                    using pointer           = T*;
                    using reference         = T&;

                    Iterator() : TargetSegmentedVector(nullptr), ItemPt(nullptr){};
                    Iterator(size_t globalIndex, size_t vectorIndex, size_t innerVectorSize, 
                                size_t innerVectorIndex, SegmentedVector<T>* targetSegmentedVector, T* itemPt)
                    {
                        GlobalIndex = globalIndex;
                        VectorIndex = vectorIndex;
                        InnerVectorSize = innerVectorSize;
                        InnerVectorSizeMinusOne = InnerVectorSize - 1;
                        InnerVectorIndex = innerVectorIndex;
                        TargetSegmentedVector = targetSegmentedVector;
                        ItemPt = itemPt;
                    }

                    Iterator(const Iterator& other)
                    {
                        GlobalIndex = other.GlobalIndex;
                        VectorIndex = other.VectorIndex;
                        InnerVectorSize = other.InnerVectorSize;
                        InnerVectorSizeMinusOne = other.InnerVectorSizeMinusOne;
                        InnerVectorIndex = other.InnerVectorIndex;
                        TargetSegmentedVector = other.TargetSegmentedVector;
                        ItemPt = other.ItemPt;
                    };

                    //function: operator!=
                    inline bool operator!= (const Iterator& other) { return ItemPt != other.ItemPt; }; 
                    //function: operator==
                    inline bool operator== (const Iterator& other) { return ItemPt == other.ItemPt; };
                    //function: operator*
                    inline T& operator*() const { return *ItemPt; };
                    //function: operator->
                    inline T* operator->() { return ItemPt; };
                    //function: operator++
                    inline Iterator& operator++() 
                    {
                        GlobalIndex++;
                        
                        //Check out of range
                        if(GlobalIndex >= TargetSegmentedVector->Size() - 1)
                        {
                            InnerVectorIndex++;
                            ItemPt++;
                            return *this;
                        }

                        if(InnerVectorIndex == InnerVectorSizeMinusOne)
                        {
                            InnerVectorSize = TargetSegmentedVector->InternalVectors[++VectorIndex].size();
                            InnerVectorIndex = 0;
                            InnerVectorSizeMinusOne = InnerVectorSize - 1;
                            ItemPt = &TargetSegmentedVector->InternalVectors[VectorIndex].front();
                            return *this;
                        }

                        InnerVectorIndex++;
                        ItemPt++;
                        return *this;
                    };
                    //function: operator++
                    inline Iterator operator++(int)
                    {
                        Iterator old = *this;
                        operator++();
                        return old;
                    };
                    //function: operator--
                    inline Iterator& operator--()
                    {
                        GlobalIndex--;

                        //Check out of range
                        if(GlobalIndex <= 0)
                        {
                            InnerVectorIndex--;
                            ItemPt--;
                            return *this;
                        }

                        if(InnerVectorIndex == 0)
                        {
                            InnerVectorSize = TargetSegmentedVector->InternalVectors[--VectorIndex].size();
                            InnerVectorIndex = InnerVectorSize - 1;
                            InnerVectorSizeMinusOne = InnerVectorIndex;
                            ItemPt = &TargetSegmentedVector->InternalVectors[VectorIndex].back();
                            return *this;
                        }

                        InnerVectorIndex--;
                        ItemPt--;
                        return *this;
                    };
                    //function: operator--
                    inline Iterator operator--(int)
                    {
                        Iterator old = *this;
                        operator--();
                        return old;
                    };
            };
        
        private:
            inline void SplitVectorIfNecessary(size_t index)
            {                
                if(InternalVectors[index].size() >= MaxSegmentVectorSize)
                {
                    size_t splitVectorIndex = InternalVectors[index].size() / 2;
                    splitVectorIndex = splitVectorIndex >= MaxSegmentVectorSize ? MaxSegmentVectorSize / 2 : splitVectorIndex;
                    InternalVectors.insert(InternalVectors.begin() + index + 1, std::vector<T>());
                    std::move(InternalVectors[index].begin() + splitVectorIndex, InternalVectors[index].end(), std::back_inserter(InternalVectors[index+1]));
                    InternalVectors[index].erase(InternalVectors[index].begin() + splitVectorIndex, InternalVectors[index].end());
                    SplitVectorIfNecessary(index+1);
                }
            }

            inline void FindVector(size_t index, size_t& foundVectorIndex, size_t& vectorStartIndex)
            {
                vectorStartIndex = 0;
                foundVectorIndex = 0;
                size_t currentEnd = 0;
                for(size_t i = 0; i < InternalVectors.size(); i++)
                {
                    currentEnd = vectorStartIndex + InternalVectors[i].size();
                    if(index < currentEnd)
                    {
                        foundVectorIndex = i;
                        break;
                    }
                    vectorStartIndex = currentEnd;
                }
            };

            inline T& GetItem(size_t index)
            {
                //Check out of range
                if(index < 0 || index >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                if(SequentialCacheValid)
                {
                    int32_t diff = index - SequentialCacheIndex;
                    switch (diff) 
                    {
                        case -1:
                            SequentialCacheIndex--;
                            if(SequentialCacheInnerVectorIndex == 0)
                            {
                                SequentialCacheVectorSize = InternalVectors[--SequentialCacheVectorIndex].size();
                                SequentialCacheInnerVectorIndex = SequentialCacheVectorSize - 1;
                                SequentialCacheVectorSizeMinusOne = SequentialCacheInnerVectorIndex;
                                SequentialCacheValuePt = &InternalVectors[SequentialCacheVectorIndex].back();
                                return *SequentialCacheValuePt;
                            }

                            SequentialCacheInnerVectorIndex--;
                            SequentialCacheValuePt--;
                            return *SequentialCacheValuePt;
                        case 0:
                            return InternalVectors[SequentialCacheVectorIndex][SequentialCacheInnerVectorIndex];
                        case 1:
                            SequentialCacheIndex++;
                            if(SequentialCacheInnerVectorIndex == SequentialCacheVectorSizeMinusOne)
                            {
                                SequentialCacheVectorSize = InternalVectors[++SequentialCacheVectorIndex].size();
                                SequentialCacheInnerVectorIndex = 0;
                                SequentialCacheVectorSizeMinusOne = SequentialCacheVectorSize - 1;
                                SequentialCacheValuePt = &InternalVectors[SequentialCacheVectorIndex].front();
                                return *SequentialCacheValuePt;
                            }

                            SequentialCacheInnerVectorIndex++;
                            SequentialCacheValuePt++;
                            return *SequentialCacheValuePt;
                    }
                }

                //Not sequential read, find element
                size_t startIndex = 0;
                size_t foundVector = -1;
                
                FindVector(index, foundVector, startIndex);

                SequentialCacheIndex = index;
                SequentialCacheVectorIndex = foundVector;
                SequentialCacheVectorSize = InternalVectors[SequentialCacheVectorIndex].size();
                SequentialCacheVectorSizeMinusOne = SequentialCacheVectorSize - 1;
                SequentialCacheInnerVectorIndex = index - startIndex;
                SequentialCacheValuePt = &InternalVectors[SequentialCacheVectorIndex][SequentialCacheInnerVectorIndex];
                SequentialCacheValid = true;

                return *SequentialCacheValuePt;
            };
        
        protected:
        // public:
            std::vector<std::vector<T>> InternalVectors;
            size_t MaxSegmentVectorSize;

            size_t SequentialCacheIndex;                    //Global index
            size_t SequentialCacheVectorIndex;              //Index of the segmented/inner vector that contains the item
            size_t SequentialCacheVectorSize;               //The size of the segmented/inner vector
            size_t SequentialCacheVectorSizeMinusOne;       //Above but minus one

            size_t SequentialCacheInnerVectorIndex;         //The index in the segmented/inner vector for the item
            T* SequentialCacheValuePt;                      //The pointer for the item

            bool SequentialCacheValid;
            size_t CurrentSize;


        //class: ssGUI::SegmentedVector
        //This is a custom sequence container that allows random access with fast data insertion.
        //This is the most suitable to store a large amount of data that needs index access.
        //Although most of the data is stored contiguously, there is *no* guarantee that data are stored next to each other, even though most of them are.
        public:
            inline SegmentedVector() : InternalVectors(), MaxSegmentVectorSize(131072 / sizeof(T)), SequentialCacheIndex(0), 
                                        SequentialCacheVectorIndex(0), SequentialCacheVectorSize(0), SequentialCacheInnerVectorIndex(0), 
                                        SequentialCacheValuePt(nullptr), SequentialCacheValid(false), CurrentSize(0)
            {
                InternalVectors.push_back(std::vector<T>());
            };

            //function: SegmentedVector
            //Constructor that constructs by copying a vector
            inline SegmentedVector(const std::vector<T>& other) : SegmentedVector()
            {
                Add(other);
            };

            //function: SegmentedVector
            //Constructor that constructs by copying an array
            inline SegmentedVector(const T objs[], size_t len)
            {
                Add(objs, len);
            };

            //function: SegmentedVector
            //Constructor that constructs by copying another <SegmentedVector>
            inline SegmentedVector(const SegmentedVector<T>& other)
            {
                InternalVectors = other.InternalVectors;
                MaxSegmentVectorSize = other.MaxSegmentVectorSize;

                SequentialCacheIndex = other.SequentialCacheIndex;
                SequentialCacheVectorIndex = other.SequentialCacheVectorIndex;
                SequentialCacheVectorSize = other.SequentialCacheVectorSize;
                SequentialCacheVectorSizeMinusOne = other.SequentialCacheVectorSizeMinusOne;

                SequentialCacheInnerVectorIndex = other.SequentialCacheInnerVectorIndex;
                SequentialCacheValuePt = &InternalVectors[SequentialCacheVectorIndex][SequentialCacheInnerVectorIndex];

                SequentialCacheValid = other.SequentialCacheValid;
                CurrentSize = other.CurrentSize;
            };

            //function: operator=
            inline SegmentedVector<T>& operator=(const SegmentedVector<T>& other)
            {
                InternalVectors = other.InternalVectors;
                MaxSegmentVectorSize = other.MaxSegmentVectorSize;

                SequentialCacheIndex = other.SequentialCacheIndex;
                SequentialCacheVectorIndex = other.SequentialCacheVectorIndex;
                SequentialCacheVectorSize = other.SequentialCacheVectorSize;
                SequentialCacheVectorSizeMinusOne = other.SequentialCacheVectorSizeMinusOne;

                SequentialCacheInnerVectorIndex = other.SequentialCacheInnerVectorIndex;
                SequentialCacheValuePt = &InternalVectors[SequentialCacheVectorIndex][SequentialCacheInnerVectorIndex];

                SequentialCacheValid = other.SequentialCacheValid;
                CurrentSize = other.CurrentSize;

                return *this;
            };

            //function: operator[]
            //This returns the reference to the item. This operation is *not* thread safe.
            //Use <ssGUI::SegmentedVector::Iterator> instead to be thread safe.
            inline T& operator[](int index)
            {
                return GetItem(index);
            };

            //function: Begin
            //Returns an iterator that points to the first element. Equals to <End> if container is empty.
            inline Iterator Begin()
            {
                return Iterator(0, 0, InternalVectors[0].size(), 0, this, &InternalVectors[0][0]);
            };

            //function: End
            //Returns an iterator that points to the end of the container, after the last element.
            inline Iterator End()
            {
                return Iterator(CurrentSize, InternalVectors.size() - 1, InternalVectors.back().size(), 
                    InternalVectors.back().size(), this, &InternalVectors.back()[InternalVectors.back().size()]);
            }

            //function: GetIterator
            //Returns an <ssGUI::SegmentedVector::Iterator> pointing at index position
            inline Iterator GetIterator(int index)
            {
                //Check out of range
                if(index < 0 || index >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));
                
                //Not sequential read, find element
                size_t startIndex = 0;
                size_t foundVector = -1;
                
                FindVector(index, foundVector, startIndex);

                return Iterator(index, foundVector, InternalVectors[foundVector].size(), 
                    index - startIndex, this, &InternalVectors[foundVector][index - startIndex]);
            }

            //function: SetMaxSegmentVectorSize
            //This sets the size of the inner vector. By default, each inner vector holds about 512 KiB of data.
            inline void SetMaxSegmentVectorSize(size_t size)
            {
                MaxSegmentVectorSize = size;
            }

            //function: At
            //See operator[]
            inline T& At(int index)
            {
                return GetItem(index);
            };

            //function: Size
            //Returns the size of the container
            inline std::size_t Size() const
            {
                return CurrentSize;
            };

            //function: Empty
            //Returns true if the container contains no elements, false otherwise.
            inline bool Empty() const
            {
                return CurrentSize == 0;
            };

            //function: Remove
            //Removes an element at index position
            inline void Remove(size_t index)
            {
                //Check out of range
                if(index < 0 || index >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);

                InternalVectors[foundVector].erase(InternalVectors[foundVector].begin() + (index - startIndex));

                if(InternalVectors[foundVector].empty() && InternalVectors.size() > 1)
                    InternalVectors.erase(InternalVectors.begin() + foundVector);

                SequentialCacheValid = false;
                CurrentSize--;
            };

            //function: Remove
            //Removes a range of elements from startIndex position to endExclusiveIndex position where the later index is exclusive.
            inline void Remove(size_t startIndex, size_t endExclusiveIndex)
            {
                //Check out of range
                if(startIndex < 0 || startIndex >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid start index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(startIndex));

                if(endExclusiveIndex <= 0 || endExclusiveIndex > CurrentSize)
                    throw std::out_of_range(std::string("Invalid end index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(endExclusiveIndex));

                size_t vectorStartBeginIndex = 0;
                size_t foundVectorStart = 0;
                size_t vectorEndBeginIndex = 0;
                size_t foundVectorEnd = 0;

                size_t currentStart = 0;
                size_t currentEnd = 0;

                bool startFound = false;

                for(size_t i = 0; i < InternalVectors.size(); i++)
                {
                    currentEnd = currentStart + InternalVectors[i].size();
                    
                    if(!startFound && startIndex < currentEnd)
                    {
                        foundVectorStart = i;
                        vectorStartBeginIndex = currentStart;
                        startFound = true;
                    }
                    
                    if(startFound && endExclusiveIndex <= currentEnd)
                    {
                        foundVectorEnd = i;
                        vectorEndBeginIndex = currentStart;
                        break;
                    }
                    
                    currentStart = currentEnd;
                }

                //If the range is in the same vector, just remove them inside that vector
                if(foundVectorStart == foundVectorEnd)
                {
                    std::vector<T>& targetVector = InternalVectors[foundVectorStart];
                    targetVector.erase(targetVector.begin() + (startIndex - vectorStartBeginIndex), targetVector.begin() + (endExclusiveIndex - vectorStartBeginIndex));
                
                    if(InternalVectors[foundVectorStart].empty() && InternalVectors.size() > 1)
                        InternalVectors.erase(InternalVectors.begin() + foundVectorStart);
                }
                //Otherwise, remove any vectors inbetween
                else
                {
                    //Start
                    InternalVectors[foundVectorStart].erase(InternalVectors[foundVectorStart].begin() + (startIndex - vectorStartBeginIndex), InternalVectors[foundVectorStart].end());

                    //End
                    InternalVectors[foundVectorEnd].erase(InternalVectors[foundVectorEnd].begin(), InternalVectors[foundVectorEnd].begin() + (endExclusiveIndex - vectorEndBeginIndex));
                    
                    //Remove all vectors inbetween
                    if(foundVectorEnd - foundVectorStart > 1)
                        InternalVectors.erase(InternalVectors.begin() + foundVectorStart + 1, InternalVectors.begin() + foundVectorEnd);
                    
                    if(InternalVectors[foundVectorStart + 1].empty())
                        InternalVectors.erase(InternalVectors.begin() + foundVectorStart + 1);
                    
                    if(InternalVectors[foundVectorStart].empty())
                        InternalVectors.erase(InternalVectors.begin() + foundVectorStart);
                    
                    //Check empty
                    if(InternalVectors.empty())
                        InternalVectors.push_back(std::vector<T>());
                }


                SequentialCacheValid = false;
                CurrentSize -= (endExclusiveIndex - startIndex);
            };

            //function: Clear
            //Removes all elements in the container
            inline void Clear()
            {
                InternalVectors.clear();
                InternalVectors.push_back(std::vector<T>());
                CurrentSize = 0;
                SequentialCacheValid = false;
            };

            //function: Add
            inline void Add(const T& obj)
            {
                InternalVectors[InternalVectors.size() - 1].push_back(obj);
                SplitVectorIfNecessary(InternalVectors.size() - 1);
                SequentialCacheValid = false;
                CurrentSize++;
            };

            //function: Add
            inline void Add(T&& obj)
            {
                InternalVectors[InternalVectors.size() - 1].push_back(obj);
                SplitVectorIfNecessary(InternalVectors.size() - 1);
                SequentialCacheValid = false;
                CurrentSize++;
            };

            //function: Add
            inline void Add(const SegmentedVector<T>& objs)
            {
                std::vector<T>& targetVector = *--InternalVectors.end();

                for(size_t i = 0; i < objs.InternalVectors.size(); i++)
                    targetVector.insert(targetVector.end(), objs.InternalVectors[i].begin(), objs.InternalVectors[i].end());
                
                SplitVectorIfNecessary(InternalVectors.size() - 1);
                SequentialCacheValid = false;
                CurrentSize += objs.Size();
            };

            //function: Add
            inline void Add(const std::vector<T>& objs)
            {
                std::vector<T>& targetVector = *--InternalVectors.end();
                targetVector.insert(targetVector.end(), objs.begin(), objs.end());
                SplitVectorIfNecessary(InternalVectors.size() - 1);
                SequentialCacheValid = false;
                CurrentSize += objs.size();
            };

            //function: Add
            inline void Add(const T objs[], size_t len)
            {
                std::vector<T>& targetVector = *--InternalVectors.end();
                targetVector.insert(targetVector.end(), objs, objs + len);
                SplitVectorIfNecessary(InternalVectors.size() - 1);
                SequentialCacheValid = false;
                CurrentSize += len;
            };

            //function: Add
            inline void Add(const T& obj, int index)
            {
                //Check out of range
                if(index < 0 || index > CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                if(index == CurrentSize)
                {
                    Add(obj);
                    return;
                }

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);
                InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), obj);
                SplitVectorIfNecessary(foundVector);
                SequentialCacheValid = false;
                CurrentSize++;
            };

            //function: Add
            inline void Add(const T&& obj, int index)
            {
                //Check out of range
                if(index < 0 || index > CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                if(index == CurrentSize)
                {
                    Add(obj);
                    return;
                }

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);
                InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), obj);
                SplitVectorIfNecessary(foundVector);
                SequentialCacheValid = false;
                CurrentSize++;
            };

            //function: Add
            inline void Add(const SegmentedVector<T>& objs, int index)
            {
                //Check out of range
                if(index < 0 || index > CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);
                for(size_t i = objs.InternalVectors.size() - 1; i > 0; i--)
                {
                    InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), 
                        objs.InternalVectors[i].begin(), objs.InternalVectors[i].end());
                }
                InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), 
                        objs.InternalVectors[0].begin(), objs.InternalVectors[0].end());

                SplitVectorIfNecessary(foundVector);
                SequentialCacheValid = false;
                CurrentSize += objs.Size();
            };

            //function: Add
            inline void Add(const std::vector<T>& objs, int index)
            {
                //Check out of range
                if(index < 0 || index > CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                if(index == CurrentSize)
                {
                    Add(objs);
                    return;
                }

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);
                InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), objs.begin(), objs.end());
                SplitVectorIfNecessary(foundVector);
                SequentialCacheValid = false;
                CurrentSize += objs.size();
            };

            //function: Add
            inline void Add(const T objs[], size_t len, int index)
            {
                //Check out of range
                if(index < 0 || index > CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));

                if(index == CurrentSize)
                {
                    Add(objs, len);
                    return;
                }

                size_t startIndex = 0;
                size_t foundVector = -1;

                FindVector(index, foundVector, startIndex);
                InternalVectors[foundVector].insert(InternalVectors[foundVector].begin() + (index - startIndex), objs, objs + len);
                SplitVectorIfNecessary(foundVector);
                SequentialCacheValid = false;
                CurrentSize += len;
            };

            //function: Swap
            //Swap two elements at position index and index2
            inline void Swap(int index, int index2)
            {
                //Check out of range
                if(index < 0 || index >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index));
                if(index2 < 0 || index2 >= CurrentSize)
                    throw std::out_of_range(std::string("Invalid index in SegmentedVector (Line "+std::to_string(__LINE__)+" in function "+__func__+"): ")+std::to_string(index2));
                
                std::swap(GetItem(index), GetItem(index2));
            };

            //function: Front
            //Returns the reference to first element
            inline T& Front()
            {
                return InternalVectors[0].front();
            };

            //function: Back
            //Returns the reference to last element
            inline T& Back()
            {
                return InternalVectors[InternalVectors.size()].back();
            };
    };
}

#endif