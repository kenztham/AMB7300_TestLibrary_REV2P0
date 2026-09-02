/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			TestFunction.cpp
Purpose:		Contain constructor and destructor
Version:		v1.0.0.1
----------------------------------------------------------------------*/


#include "TestFunction.h"

namespace Functions
{
	TestFunction::TestFunction()
	{
		glob = gcnew Globals();
	}
	TestFunction::~TestFunction(void)
	{
	}
}


/*----------------------------------------------------------------------
* Revision Log
* $Log: TestFunction.cpp.rca$


*** Version	:
*** Date	:
*** PIC		:
*

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
----------------------------------------------------------------------*/
