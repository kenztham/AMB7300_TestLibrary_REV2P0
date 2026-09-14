#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "Test Function/TestFunction.h"
#include "AMB7300/AMB7300.h"

// <Test Resources>
#include "Test Function/Aemulus.Hardware.SMU.h"
#include "Test Function/Aemulus.Hardware.DM.h"
#include "Test Function/Aemulus.Hardware.CM.h"

using namespace Functions;
using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Collections::Concurrent;	//for x64 programs, use ConcurrentDictionary (for threading applications)

using namespace System::Runtime::InteropServices;
using namespace Aemulus::Tech;
using namespace Aemulus::TestLib;
using namespace Aemulus::Hardware;
using namespace Aemulus::Tech::Flow;
using namespace Aemulus::Tech::Flow::Result;
using namespace Aemulus::TestLib::Utility;



namespace AMB7300_TestLibrary_REV2P0
{
	public ref class TestProgram : ITestProgram
	{
	private:

		TestFunction ^ tl;
		AMB7300TestLibrary ^ amb7300tl;
		Aemulus::TestLib::Utility::Utilities ^ Util;
		AppDomain ^ currentDomain;

#pragma region "Global Variable"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Error Message in Threading
		**	----------------------------------------------------------------------------------------------------
		*/
		static array<String^>^ G_RunTimeErrorMessage;
		static array<int>^ G_RunTimeErrorCode;
		static array<bool>^ G_RunTimeError;
		static bool G_JumpOnFail;
		int G_TotalSites;
		
		//Results array for True Parellel test
		array<int> ^ resultIndex;
		array<Object ^, 2> ^ TPtestResult;

		int lineNUM;
		String ^ timerFilename;

#pragma endregion

	public:

		

		// TestProgram Constructor
		TestProgram(void);
		~TestProgram(void);

		// Public Methods
		int Load(Site ^ site);
		int Unload(Site ^ site);
		int PreProcessing(Site^ site);
		int PostProcessing(Site^ site);
		void SaveSnpToBinAfterCommitResults(Site^ site);

#pragma region "TestMethod.cpp -> Test Method for Dc, Pattern, Vna Test Item"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Method for Dc, Pattern, Vna Test Item
		**	----------------------------------------------------------------------------------------------------
		*/
		int seq_TestMethod(Site ^ site);

#pragma endregion

#pragma region "AMB7300Utility.cpp -> Cast Conditions & Value Validate"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	TF Test Library XML & Flow Item's Conditions (Test Item, Test Parameter, Control Step)
		**	----------------------------------------------------------------------------------------------------
		*/


		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step: DcControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_ControlStep_DcControl(Site ^ site);
		int TestLib_ControlStep_DcControl_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: DcTest
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_DcTest(Site ^ site);
		int TestLib_TestParameter_DcTest_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step: PatternControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_ControlStep_PatternControl(Site ^ site);
		int TestLib_ControlStep_PatternControl_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: PatternTest
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_PatternTest(Site ^ site);
		int TestLib_TestParameter_PatternTest_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step: DmControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_ControlStep_DmControl(Site ^ site);
		int TestLib_ControlStep_DmControl_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step: VnaConfig
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_ControlStep_VnaConfig(Site ^ site);
		int TestLib_ControlStep_VnaConfig_CastCondition(Site ^ site, int tfSite, int siteIndex, int segmentSetCount);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step: VnaFetch
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_ControlStep_VnaFetch(Site ^ site);
		int TestLib_ControlStep_VnaFetch_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: VnaDataStore
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_VnaDataStore(Site ^ site);
		int TestLib_TestParameter_VnaDataStore_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: VnaDataAnalysis
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_VnaDataAnalysis(Site ^ site);
		int TestLib_TestParameter_VnaDataAnalysis_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: VnaSwTime
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_VnaSwTime(Site ^ site);
		int TestLib_TestParameter_VnaSwTime_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter: Math
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestLib_TestParameter_Math(Site ^ site);
		int TestLib_TestParameter_Math_CastCondition(Site ^ site, int tfSite, int siteIndex);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> DcControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_DcControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int totalConfigurationSets);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> DcTest
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_DcTest(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> PatternControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_PatternControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> PatternTest
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_PatternTest(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> DmControl
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_DmControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int totalConfigurationSets);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> VnaConfig
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestProgram::ValidateConditionValueInput_VnaConfig(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int vnaConfigSegmentCount);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> VnaFetch
		**	----------------------------------------------------------------------------------------------------
		*/
		int TestProgram::ValidateConditionValueInput_VnaFetch(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> VnaDataAnalysis
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_VnaDataAnalysis(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> VnaSwTime
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_VnaSwTime(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Validate condition value input ---> Math
		**	----------------------------------------------------------------------------------------------------
		*/
		int ValidateConditionValueInput_Math(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Check condition change ---> VnaConfig
		**	----------------------------------------------------------------------------------------------------
		*/
		int CheckConditionChange_VnaConfig(Site ^ site, int tfSite, int siteIndex, int segmentSetCount);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Threading Helper Function
		**	----------------------------------------------------------------------------------------------------
		*/
		void DoThread(ParameterizedThreadStart^ function, Site^ site);
		void IsRunTest(Site^ site, array<bool>^ run_test);
		void ExecuteControlStep_DC(Object^ object);
		void ExecuteTestParameter_DC(Object^ object);

#pragma endregion

	};
}
