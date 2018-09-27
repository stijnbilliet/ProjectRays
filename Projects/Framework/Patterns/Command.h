#pragma once
#include "Subject.h"
class Command : public Subject
{
public:
	Command();
	~Command();

	virtual void execute() = 0;
};