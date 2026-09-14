/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			TestFunction.h
Purpose:		Declare all test functions
Version:		v1.0.0.3
----------------------------------------------------------------------*/


#pragma once

#include <windows.h>
#include "Enum.h"
#include "Globals.h"
#include "Defines.h"
#include "TF-Macro.h"

// Support Kill Process Function 
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <fstream>

using namespace System;
using namespace System::IO;
using namespace System::IO::MemoryMappedFiles;
using namespace System::Text;
using namespace System::Net;
using namespace System::Xml;
using namespace System::Reflection;
using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Runtime::Remoting;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Aemulus::Hardware;
using namespace Aemulus::Tech::Flow;
using namespace Aemulus::Tech::Flow::Result;
using namespace Aemulus::TestLib::Utility;
using namespace Aemulus::Tech::Flow::ProductionSystem;
//using namespace Aemulus::WaferTestLibrary;

namespace Functions
{
	public ref class TestFunction
	{
	public:

		// Instanstiate Utilities 
		Aemulus::TestLib::Utility::Utilities ^ Util;

		// Instanstiate Logger
		Aemulus::TestLib::Utility::FileLogger^ FileLog;

		TestFunction();
		~TestFunction(void);

		AppDomain ^ currentDomain;

		Globals ^ glob;

		// Barrier
		BaseBarrierCollections^ barrier;

#pragma region "Globals.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Initiliaze Program
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeProgram(Site ^ site);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	techFlow Property | techFlow Project Type | techFlow File/Folder Directory
		**	----------------------------------------------------------------------------------------------------
		*/
		void GetTechFlowSiteProperty(Site ^ site);
		void GetTechFlowProjectType(Site ^ site);
		void GetTechFlowFilePathProperty(Site ^ site);
		void GetTechFlowBinningProperty(Site ^ site);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Tester ID
		**	----------------------------------------------------------------------------------------------------
		*/
		void GetTesterID(Site ^ site, int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	App-Wide-Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetTechFlowAppsWideVariable(Site^ site, int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemlus --> [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		void InitializeDebugFolder(int tfSite);
		void InitializeTesterInfoFolder(int tfSite);
		void InitializeWolferFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> BoardLossFileFolder [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeBoardLossFileFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> DeviceStateFileTemplate [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeDeviceStateFileTemplateFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> FixedOffsetFileFolder [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeFixedOffsetFileFolder(Site ^ site, int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> ModulationFileFolderSitex [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeModulationFileFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> VectorFileFolderSitex [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeVectorFileFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> VectorStateFileFolderSitex [Init Related Variables]
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeVectorStateFileFolder(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Resource Manager Property (AEM DC Module)
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeResourceManagerProperty(Site ^ site, int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Global Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		void InitializeGlobalVariables(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Tracer Logger
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeTracerLogger(Site ^ site, int tfSite);
		void WRITETOTRACERLOGGER(int tfSite, int siteIndex, String ^ messageType, String ^ message, int programLineNumber, String ^ programFileName, String ^ programFunctionName);
#define WriteToTracerLogger(tfSite, siteIndex, messageType, message) WRITETOTRACERLOGGER(tfSite, siteIndex, messageType, message, __LINE__, __FILE__, __FUNCTION__);
		void WriteToTcrLgr(String ^ tracerTabName, String ^ message);
		int UninitializeTracerLogger();

		/*
		**	----------------------------------------------------------------------------------------------------
		**	File Logger
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeFileLogger(int tfSite);
		void WRITETOFILELOGGER(int tfSite, int siteIndex, String ^ messageType, String ^ message, int programLineNumber, String ^ programFileName, String ^ programFunctionName);
#define WriteToFileLogger(tfSite, siteIndex, messageType, message) WRITETOFILELOGGER(tfSite, siteIndex, messageType, message, __LINE__, __FILE__, __FUNCTION__);
		void WriteToFileLgr(String ^ fileDirectory, String ^ message);
		int UninitializeFileLogger();

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Check Error
		**	----------------------------------------------------------------------------------------------------
		*/
		int CHECKERROR(int siteIndex, int errorCode, int ErrorLineNumber, String ^ FileName);
#define CheckError(siteIndex, errorCode) CHECKERROR(siteIndex, errorCode, __LINE__, __FILE__);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Warning Message Box
		**	----------------------------------------------------------------------------------------------------
		*/
		void WarningMessageBox(String ^ MssgContent, String ^ WarningMssgType);

#pragma endregion

#pragma region "Files.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Running Production
		**	----------------------------------------------------------------------------------------------------
		*/
		bool IsRunningProduction(Site^ site);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	BoardLoss File Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void CheckExistingBoardLossFileContent(int tfSite, String ^ fileDirectory, String ^ fileName);
		void GenerateBoardLossFile(int tfSite, String ^ fileDirectory);
		void LoadBoardLossFile(int tfSite, String ^ fileDirectory);
		double GetBoardLossFactor(int tfSite, int siteIndex, String ^ hardwarePathKey);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DeviceStateFile Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void LoadDeviceStateFileTemplate(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	FixedOffset File Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void CheckExistingFixedOffsetFileContent(Site ^ site, int tfSite, String ^ fileDirectory, String ^ fileName);
		void GenerateFixedOffsetFile(Site ^ site, int tfSite, String ^ fileDirectory);
		void LoadFixedOffsetFile(int tfSite, String ^ fileDirectory);
		double GetFixedOffsetValue(int tfSite, int siteIndex, String ^ testParameterKey);
		//double GetOffset(int tfSite, int siteIndex, String ^ testParameterKey);


		/*
		**	----------------------------------------------------------------------------------------------------
		**	Modulation File Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void LoadModulationFile(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	VectorFile Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void LoadVectorFile(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	VectorStateFile Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void LoadVectorStateFile(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	AppsCal File Related
		**	----------------------------------------------------------------------------------------------------
		*/
		void LoadAppsCalFile(int tfSite, String ^ fileDirectory);

#pragma endregion

#pragma region "techFlow.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Update Test Property
		**	----------------------------------------------------------------------------------------------------
		*/
		int UpdateTestProperty(Site ^ site, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Update Control Step Test Result To Dicitionary
		**	----------------------------------------------------------------------------------------------------
		*/
		int UpdateControlStepTestResulToDictionary(Site^ site, int siteIndex, String^ Identifier, double CSTestResult);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Update Test Result To techFlow
		**	----------------------------------------------------------------------------------------------------
		*/
		int UpdateTestResultWithOffsetToTechFlow(Site^ site, int siteIndex, array<Object ^,2> ^ testResult);

		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get Unit Prefix (in String format)
		**	----------------------------------------------------------------------------------------------------
		*/
		String^ GetStringUnitPrefix(Site^ site, String ^ TestItemName, String ^ TestParameterName);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Execute each control step item and test parameter item
		**	----------------------------------------------------------------------------------------------------
		*/
		//void ExecuteFlowItems(Object^ object);

#pragma endregion

	private:

		// Perform assembly resolve if the loading of .dll/.exe turns out to be unsucessfull
		Assembly ^ currentDomain_AssemblyResolve(Object^ Sender, ResolveEventArgs^ args)
		{
			bool Is64BitProcess = false;
			String^ assemblyPath = String::Empty;

			//Check if it is running in 64bit or 32bit
			if (IntPtr::Size == 8)
			{
				Is64BitProcess = true;
			}
			else
			{
				Is64BitProcess = false;
			}

			if (Is64BitProcess == true)
			{
				if (args->Name->Contains("Aemulus.TestLib.Utility") == true)
				{
					assemblyPath = Path::Combine(Path::Combine(Environment::GetEnvironmentVariable("techFlow"), "bin", "x64"), "Aemulus.TestLib.Utility.dll");
				}
				if (args->Name->Contains("Aemulus.Hardware.SMU") == true)
				{
					assemblyPath = Path::Combine(Path::Combine(Environment::GetEnvironmentVariable("techFlow"), "bin", "x64"), "Aemulus.Hardware.SMU.dll");
				}
			}
			else
			{
				if (args->Name->Contains("Aemulus.TestLib.Utility") == true) 
				{
					assemblyPath = Path::Combine(Path::Combine(Environment::GetEnvironmentVariable("techFlow"), "bin"), "Aemulus.TestLib.Utility.dll");
				}
				if (args->Name->Contains("Aemulus.Hardware.SMU") == true)
				{
					assemblyPath = Path::Combine(Path::Combine(Environment::GetEnvironmentVariable("techFlow"), "bin"), "Aemulus.Hardware.SMU.dll");
				}
			}

			return Assembly::LoadFile(assemblyPath);
		}
	};

}


/*----------------------------------------------------------------------
* Revision Log
* &Log: TestFunction.h.rca&

*** Version	: v1.0.0.3
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Added LoadAppsCalFile()

*** Version	: v1.0.0.2
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Added GetTechFlowBinningProperty()
* Modified UpdateTestResultWithOffsetToTechFlow()

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
----------------------------------------------------------------------*/