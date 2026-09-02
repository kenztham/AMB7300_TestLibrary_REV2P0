/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				TM400Utility.cpp
Purpose:			Contain TM400x related utility functions
Supported Module:	TM460e
Version:			v1.0.0.1
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: TM Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeTm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline)
	{
		/*****************************************************************************************************
		**	InitializeTm
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc object index, normally start from 0.
		**		offline				- techFlow run in online or offline mode.
		**
		**	Descriptions:
		**		This is a function to initialize TM resource.
		**		Preset all the existing TM pin inside resource planner.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement TM module function.

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_TM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> TM400Utility -> InitializeTm] Fail to initialize TM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> TM400Utility -> InitializeTm] Fail to initialize TM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeTm(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	UninitializeTm
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to uninitialize TM resource.
		**		Off all the existing TM pin inside resource planner, and close session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement TM module function.

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_TM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> TM400Utility -> UninitializeTm] Fail to uninitialize TM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> TM400Utility -> UninitializeTm] Fail to uninitialize TM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}


	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: TM Test Function
	**	----------------------------------------------------------------------------------------------------
	*/


}


/*----------------------------------------------------------------------
* Revision Log
* &Log: TM400Utility.cpp.rca&


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