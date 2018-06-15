#pragma once

template<class T>
class List
{

private:
	class Node
	{

	public:
		const int mId;
		T mData;
		volatile Node* mNext;

		inline Node(int id, T data)	:mId(id), mData(data), mNext(nullptr)	{}
		inline ~Node() { delete mNext; }

	};

private:
	volatile Node* mFirst;
	volatile Node* mCurrent;
	volatile Node* mLast;
	volatile int mSize;

public:
	inline List()	:mFirst(nullptr), mCurrent(nullptr), mLast(nullptr), mSize(0)	{}
	inline ~List() { delete mFirst; }
	
	inline void add(T data) 
	{
		addById(0, data);
	}

	inline void addById(int id, T data) 
	{
		volatile Node* nodeToAdd = new Node(id, data);
		if (mSize)
		{
			mLast->mNext = nodeToAdd;
			mLast = mLast->mNext;
		}
		else
		{
			mLast = nodeToAdd;
			mFirst = mLast;
		}
		++mSize;
	}

	inline T removeById(int id)	
	{
		volatile Node* previous = mFirst;
		volatile Node* current = previous;
		T result = nullptr;

		while (current != nullptr && current->mId != id)
		{
			previous = current;
			current = current->mNext;
		}

		if (current != nullptr)
		{
			result = current->mData;
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

		return result;
	}

	inline bool remove(T data) 
	{
		volatile Node* previous = mFirst;
		volatile Node* current = previous;

		while (current != nullptr && current->mData != data)
		{
			previous = current;
			current = current->mNext;
		}

		if (current != nullptr)
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

			return true;
		}

		return false;
	}

	inline T removeFirst() 
	{
		T result = nullptr;

		if (mSize)
		{
			volatile Node* first = mFirst;
			result = first->mData;
			mFirst = mFirst->mNext;
			first->mNext = nullptr;
			delete first;
			--mSize;
		}

		return result;
	}

	inline T findById(int id) 
	{
		volatile Node* current = mFirst;

		while (current != nullptr && current->mId != id);

		return (current) ? current->mData : nullptr;
	}

	inline bool include(T data) 
	{
		volatile Node* current = mFirst;
		while (current != nullptr && current->mData != data);

		return (current);
	}

	inline int size()  const		{ return mSize; }

	inline void begin() 			{ mCurrent = mFirst; }

	inline bool end() 		{ return mCurrent == nullptr; }
	
	inline T next() 
	{
		T data = mCurrent->mData;
		mCurrent = mCurrent->mNext;
		return data;
	}
};