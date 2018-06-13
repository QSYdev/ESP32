#pragma once

template<class T>
class List
{

private:
	class Node
	{

	public:
		T mData;
		Node* mNext;

		inline Node(T data)	:mData(data), mNext(nullptr)	{}
		inline ~Node() { delete mNext; }

	};

private:
	Node* mFirst;
	Node* mCurrent;
	Node* mLast;
	int mSize;

public:
	inline List()	:mFirst(nullptr), mCurrent(nullptr), mLast(nullptr), mSize(0)	{}
	inline ~List() { delete mFirst; }
	
	inline void add(T data)
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

	inline bool remove(T data)	
	{
		Node* previous = mFirst;
		Node* current = previous;
		bool found = false;

		while (current != nullptr && current->mData != data)
		{
			previous = current;
			current = current->mNext;
		}

		if (found)
		{
			if (current == previous)
			{
				mFirst = current->mNext;
				mLast = (mSize > 1) ? mLast : mFirst;
			}
			else
			{
				previous->mNext = current->mNext;
			}

			current->mNext = nullptr;
			delete current;
			--mSize;
		}

		return found;
	}

	inline bool include(T data)
	{
		Node* current = mCurrent;
		bool found = false;
		begin();
		while (!end() && data != next());
		mCurrent = current;
		return found;
	}

	inline int size()		{ return mSize; }

	inline void begin() 	{ mCurrent = mFirst; }

	inline bool end()		{ return mCurrent == nullptr; }
	
	inline T next()
	{
		T data = mCurrent->mData;
		mCurrent = mCurrent->mNext;
		return data;
	}
};