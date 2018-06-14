#pragma once

template<class T>
class List
{

private:
	class Node
	{

	public:
		int mId;
		T mData;
		Node* mNext;

		inline Node(int id, T data)	:mId(id), mData(data), mNext(nullptr)	{}
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
	
	inline void add(T data, int id = 0)
	{
		Node* nodeToAdd = new Node(id, data);
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
		Node* previous = mFirst;
		Node* current = previous;
		T result;

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
		Node* previous = mFirst;
		Node* current = previous;

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

	inline T findById(int id)
	{
		Node* current = mFirst;

		while (current != nullptr && current->mId != id);

		return (current) ? current->mData : 0;
	}

	inline bool include(T data)
	{
		Node* current = mFirst;
		while (current != nullptr && current->mData != data);

		return (current);
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