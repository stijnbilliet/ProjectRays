#pragma once
#include "Subject.h"
class Command : public Subject
{
public:
	Command();
	virtual ~Command();

	virtual void execute() = 0;
};