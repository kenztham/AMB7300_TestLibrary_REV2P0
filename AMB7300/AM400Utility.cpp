/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				AM400Utility.cpp
Purpose:			Contain AM400x related utility functions
Supported Module:	AM430e, AM450e, AM451e, AM471e
Version:			v1.0.0.1
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: SMU Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeSmu(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline)
	{
		/*****************************************************************************************************
		**	InitializeSmu
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc object index, normally start from 0.
		**		offline				- techFlow run in online or offline mode.
		**
		**	Descriptions:
		**		This is a function to initialize SMU resource.
		**		Preset all the existing AM pin inside resource planner.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Initialize
			smu[siteIndex] = gcnew SMU(hardwareProfile, testHead, siteIndex, offline);

			// Configure pin with default setting
			for (int i = 0; i < tl->glob->RsrcManager.amPinCount; i++)
			{
				tl->CheckError(tfSite, smu[siteIndex]->SetBandwidth(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], AM_CONST_OUTPUT_TRANSIENT_NORMAL));
				tl->CheckError(tfSite, smu[siteIndex]->ConfigureSense(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], AM_CONST_SENSE_REMOTE));
				tl->CheckError(tfSite, smu[siteIndex]->SetNPLC(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], 0.01));
				tl->CheckError(tfSite, smu[siteIndex]->ConfigureOutputFunction(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], AM_CONST_OUTPUT_FUNCTION_DVCI));
				tl->CheckError(tfSite, smu[siteIndex]->ClampCurrent(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], 100.0 mA));
				tl->CheckError(tfSite, smu[siteIndex]->DriveVoltage(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], 0.0 V));
				tl->CheckError(tfSite, smu[siteIndex]->OnSmuPin(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], AM_CONST_ONSMU_REMOTE_SENSE_ENABLE, AM_CONST_ONSMU_SERIES_RES_DISABLE));
			}
			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_AM_HARDWARE_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> AM400Utility -> InitializeSmu] Fail to initialize AM series hardware (SMU)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeSmu(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	UninitializeSmu
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to uninitialize SMU resource.
		**		Off all the existing AM pin inside resource planner, and close session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Off smu pin
			for (int i = 0; i < tl->glob->RsrcManager.amPinCount; i++)
			{
				tl->CheckError(tfSite, smu[siteIndex]->DriveVoltage(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], 0.0 V));
				tl->CheckError(tfSite, smu[siteIndex]->ClampCurrent(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i], 100.0 mA));
				tl->Util->WaitSecond(10.0 mS);
				tl->CheckError(tfSite, smu[siteIndex]->OffSmuPin(tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndex, i]));
				tl->Util->WaitSecond(10.0 mS);
			}

			// Off smu module
			for (int i = 0; i < tl->glob->RsrcManager.amModuleCount; i++)
			{
				tl->CheckError(tfSite, smu[siteIndex]->Close(tl->glob->RsrcManager.moduleAlias_AM_PinMapName[siteIndex, i]));
				tl->Util->WaitSecond(10.0 mS);
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_AM_HARDWARE_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> AM400Utility -> UninitializeSmu] Fail to initialize AM series hardware (SMU)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigurePinSmu(int tfSite, int siteIndex, String ^ pinAlias, String ^ controlFunction, double nplc, double voltageLevel, double currentLevel, double delay)
	{
		/*****************************************************************************************************
		**	ConfigurePinSmu
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		pinAlias			- This is target pin alias to perform the configuration.
		**		controlFunction		- This is SMU output function mode.
		**		nplc				- This is nplc setting for the target pin.
		**		voltageLevel		- This is voltage level for the target pin (applicable for drive & clamp).
		**		currentLevel		- This is current level for the target pin (applicable for drive & clamp).
		**		delay				- This is delay time after the pin configuration and pin execution.
		**
		**	Descriptions:
		**		This is a function to configure SMU (AM) resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Configure nplc
			tl->CheckError(tfSite, smu[siteIndex]->SetNPLC(pinAlias, nplc));

			// Configure output function, current level, voltage level
			if (controlFunction == Cond_ControlFunction_AM_DVCI)
			{
				tl->CheckError(tfSite, smu[siteIndex]->ConfigureOutputFunction(pinAlias, AM_CONST_OUTPUT_FUNCTION_DVCI));
				tl->CheckError(tfSite, smu[siteIndex]->ClampCurrent(pinAlias, currentLevel));
				tl->CheckError(tfSite, smu[siteIndex]->DriveVoltage(pinAlias, voltageLevel));
			}
			else if (controlFunction == Cond_ControlFunction_AM_DICV)
			{
				tl->CheckError(tfSite, smu[siteIndex]->ConfigureOutputFunction(pinAlias, AM_CONST_OUTPUT_FUNCTION_DICV));
				tl->CheckError(tfSite, smu[siteIndex]->ClampVoltage(pinAlias, voltageLevel));
				tl->CheckError(tfSite, smu[siteIndex]->DriveCurrent(pinAlias, currentLevel));
			}

			// Delay
			tl->Util->WaitSecond(delay);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_AM_PIN_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> ConfigurePinSmu] Fail to configure AM pin (SMU)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}



	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: SMU Test Function (DcTest)
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::SmuTestFunction_OS(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	SmuTestFunction_OS
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute OS test function for SMU resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			//tl->CheckError(tfSite, smu[siteIndex]->ConfigureOutputFunction(pinAlias, AM_CONST_OUTPUT_FUNCTION_DICV));
			//tl->CheckError(tfSite, smu[siteIndex]->ClampVoltage(pinAlias, voltageLevel));
			//tl->CheckError(tfSite, smu[siteIndex]->DriveCurrent(pinAlias, currentLevel));
			tl->CheckError(tfSite, smu[siteIndex]->SetNPLC(pinAlias, nplc));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, smu[siteIndex]->ReadVoltage(pinAlias, result));
			
			// Reconfigure to DVCI and drive low to cater for pin-to-pin short testing
			tl->CheckError(tfSite, smu[siteIndex]->ConfigureOutputFunction(pinAlias, AM_CONST_OUTPUT_FUNCTION_DVCI));
			tl->CheckError(tfSite, smu[siteIndex]->ClampCurrent(pinAlias, 100.0 mA));
			tl->CheckError(tfSite, smu[siteIndex]->DriveVoltage(pinAlias, 0.0 V));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_AM_TEST_FUNCTION_OS_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> SmuTestFunction_OS] Fail at OS test." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SmuTestFunction_MeasureCurrent(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	SmuTestFunction_MeasureCurrent
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute current measurement test function for SMU resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			tl->CheckError(tfSite, smu[siteIndex]->SetNPLC(pinAlias, nplc));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, smu[siteIndex]->ReadCurrent(pinAlias, result));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_AM_TEST_FUNCTION_MEASURE_CURRENT_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> SmuTestFunction_MeasureCurrent] Fail to measure current." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SmuTestFunction_MeasureVoltage(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	SmuTestFunction_MeasureVoltage
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute voltage measurement test function for SMU resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			tl->CheckError(tfSite, smu[siteIndex]->SetNPLC(pinAlias, nplc));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, smu[siteIndex]->ReadVoltage(pinAlias, result));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_AM_TEST_FUNCTION_MEASURE_VOLTAGE_FAIL;
			tl->WriteToTracerAndFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> SmuTestFunction_MeasureVoltage] Fail to measure voltage." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

}


/*----------------------------------------------------------------------
* Revision Log
* &Log: AM400Utility.cpp.rca&


*** Version	:
*** Date	:
*** PIC		:
*

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.1
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support AM resource configuration.
* Support AM resource test function.
----------------------------------------------------------------------*/