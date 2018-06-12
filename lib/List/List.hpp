#pragma once

template<class T>
class List
{
private:
	class Node
	{
	public:
		T* mData;
		Node* mNext;

		inline Node(T* data)	:mData(data), mNext(nullptr)	{}
	};

private:
	Node* mFirst;
	Node* mCurrent;
	Node* mLast;
	int mSize;

public:
	inline List()	:mFirst(nullptr), mCurrent(nullptr), mLast(nullptr), mSize(0)	{}
	inline void add(T* data)
	{
		if (mSize)
		{
			mLast->mNext = new Node(data);
			mLast = mLast->mNext;
		}
		else
		{
			mLast = new Node(data);
			mFirst = mLast;
		}
		++mSize;
	}
	inline bool remove(T* data)	{ return false;}
	inline int size()			{ return mSize; }
	inline void begin() 		{ mCurrent = mFirst; }
	inline bool end()			{ return mCurrent == nullptr; }
	inline T* next()
	{
		T* data = mCurrent->mData;
		mCurrent = mCurrent->mNext;
		return data;
	}
};