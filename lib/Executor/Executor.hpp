#pragma once
#include <Observer.hpp>
#include <map>
#include <ExpressionTree.hpp>
#include <Routine.hpp>
#include <Arduino.h>

class Executor : public Observable
{

private:
	class BiMap
	{
	private:
		uint16_t* mPhysicalNodes;
		std::map<uint16_t, uint16_t> mLogicalNodes;

	public:
		BiMap(std::list<uint16_t>& associationList);
		~BiMap();

		inline bool contains(uint16_t physicalId)				{ return mLogicalNodes[physicalId]; }
		inline uint16_t getLogicalId(uint16_t physicalId)		{ return mLogicalNodes[physicalId]; }
		inline uint16_t getPhysicalId(uint16_t logicalId)		{ return mPhysicalNodes[logicalId]; }
		inline uint16_t size() 									{ return mLogicalNodes.size(); }
	};

	class PreInitTask
	{
	private:
		Executor* mExecutor;
		const unsigned long mDelays[9] = {0, 500, 500, 500, 500, 150, 150, 150, 150};
		unsigned long mElapsedTime;
		uint8_t mDelayIndex;
		bool mStart;

	public:
		inline PreInitTask(Executor* executor) 	:mExecutor(executor), mStart(false)	{}

		inline void start()		{ mStart = true; mDelayIndex = 0; mElapsedTime = millis(); }
		inline void stop()		{ mStart = false; }
		void tick();
	};

	class RoutineTimeOutTask
	{

	private:
		Executor* mExecutor;
		const unsigned long mRoutineTimeOut;
		unsigned long mElapsedTime;
		bool mStart;

	public:
		inline RoutineTimeOutTask(Executor* executor, unsigned long routineTimeOut)	:mExecutor(executor), mRoutineTimeOut(routineTimeOut), mStart(false)	{}

		inline void start()	{ mStart = true; mElapsedTime = millis(); }
		inline void stop()	{ mStart = false; }
		void tick();
	};

	class StepTimeOutTask
	{
	private:
		Executor* mExecutor;
		unsigned long mStepTimeOut;
		unsigned long mElapsedTime;
		uint16_t mStepIndex;
		bool mStart;

	public:
		inline StepTimeOutTask(Executor* executor)	:mExecutor(executor), mStart(false)	{}

		inline void start(unsigned long stepTimeOut, uint16_t stepIndex)	{ mStart = true; mStepTimeOut = stepTimeOut; mStepIndex = stepIndex; mElapsedTime = millis(); }
		inline void stop()	{ mStart = false; }
		void tick();
	};

private:
	static uint16_t STEP_INDEX;

	BiMap mBiMap;
	bool* mTouchedNodes;

	const Step* mCurrentStep;
	ExpressionTree* mExpressionTree;
	uint16_t mStepIndex;

	PreInitTask mPreInitTask;
	RoutineTimeOutTask mRoutineTimeOutTask;
	StepTimeOutTask mStepTimeOutTask;
	
	void(*mToucheEvent)(Executor*, uint16_t, uint16_t, Color&, uint32_t);
	void(*mStepTimeOutEvent)(Executor*, uint16_t);
	
public:
	Executor(std::list<uint16_t>& associationList, unsigned long routineTimeOut, void (*toucheEvent)(Executor*, uint16_t, uint16_t, Color&, uint32_t), void (*stepTimeOutEvent)(Executor*, uint16_t));
	~Executor();
	
	void init();
	void tick();

	void touche(uint16_t physicalId, uint16_t stepIndex, Color& color, uint32_t delay);

protected:
	virtual bool hasNextStep() = 0;
	virtual const Step* getNextStep() = 0;
	
private:
	inline bool contains(uint16_t physicalId)	{ return mBiMap.contains(physicalId); }
	void turnAllNodes(const Color& color);
	void prepareStep();
	void finalizeStep();
	void finalizeRoutine(bool notify);

};