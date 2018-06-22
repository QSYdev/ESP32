#pragma once
#include <Executor.hpp>
#include <Routine.hpp>

class CustomExecutor : public Executor
{

private:
	const Routine* mRoutine;
	std::list<const Step*>::const_iterator mIterator;

public:
	CustomExecutor(std::list<uint16_t>& associationList, const Routine* routine);
	~CustomExecutor();

protected:
	bool hasNextStep() override;
	const Step* getNextStep() override;

private:
	static void toucheEvent(Executor* executor, uint16_t physicalId, uint16_t stepIndex, const Color& color, uint32_t delay);
	static void stepTimeOutEvent(Executor* executor, uint16_t stepIndex);

};