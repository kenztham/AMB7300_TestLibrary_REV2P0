/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				ACM400Utility.cpp
Purpose:			Contain ACM400x related utility functions
Supported Module:	ACM432e, ACM433e, ACM434e
Version:			v1.0.0.1
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: ACM Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeAcm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline)
	{
		/*****************************************************************************************************
		**	InitializeAcm
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc object index, normally start from 0.
		**		offline				- techFlow run in online or offline mode.
		**
		**	Descriptions:
		**		This is a function to initialize ACM resource.
		**		Preset all the existing ACM pin inside resource planner.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement ACM module function.

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_ACM_HARDWARE_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> ACM400Utility -> InitializeAcm] Fail to initialize ACM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeAcm(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	UninitializeAcm
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to uninitialize ACM resource.
		**		Off all the existing ACM pin inside resource planner, and close session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement ACM module function.

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_ACM_HARDWARE_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> ACM400Utility -> UninitializeAcm] Fail to uninitialize ACM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}


	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: ACM Test Function
	**	----------------------------------------------------------------------------------------------------
	*/


}


/*----------------------------------------------------------------------
* Revision Log
* &Log: ACM400Utility.cpp.rca&


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