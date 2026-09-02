/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				IOM400Utility.cpp
Purpose:			Contain IOM400x related utility functions
Supported Module:	IOM420e, IOM421
Version:			v1.0.0.1
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: IOM Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeIom(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline)
	{
		/*****************************************************************************************************
		**	InitializeIom
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc object index, normally start from 0.
		**		offline				- techFlow run in online or offline mode.
		**
		**	Descriptions:
		**		This is a function to initialize IOM resource.
		**		Preset all the existing IOM pin inside resource planner.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement IOM module function. Need to allocate the module alias, pin alias, port alias

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_IOM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> IOM400Utility -> InitializeIom] Fail to initialize IOM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> IOM400Utility -> InitializeIom] Fail to initialize IOM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeIom(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	UninitializeIom
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to uninitialize IOM resource.
		**		Off all the existing IOM pin inside resource planner, and close session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Implement IOM module function. Need to allocate the module alias, pin alias, port alias

			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_IOM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> IOM400Utility -> UninitializeIom] Fail to uninitialize IOM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> IOM400Utility -> UninitializeIom] Fail to uninitialize IOM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}


	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: IOM Test Function
	**	----------------------------------------------------------------------------------------------------
	*/


}


/*----------------------------------------------------------------------
* Revision Log
* &Log: IOM400Utility.cpp.rca&


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