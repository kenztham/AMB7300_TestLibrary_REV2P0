/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			Globals.h
Purpose:		Define common global variables
Version:		v1.0.0.5
----------------------------------------------------------------------*/


#pragma once

#include <windows.h>
#include <math.h>
#include <iostream>

#include "Aemulus.Hardware.CM.h"
#include "Aemulus.Hardware.SMU.h"
#include "Aemulus.Hardware.DM.h"
#include "Aemulus.Hardware.DIO.h"
#include "Aemulus.Hardware.ACM.h"
#include "Aemulus.Hardware.TMU.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;
using namespace Aemulus::Tech;
using namespace Aemulus::TestLib;
using namespace Aemulus::Hardware;
using namespace Aemulus::Tech::Flow;
using namespace Aemulus::TestLib::Utility;
using namespace Aemulus::Tech::Flow::ProductionSystem;

namespace Functions
{
	public ref class Globals
	{
	public:

// [Future Enchancement] These are used in Globals.cpp check error function
// but the program is using the object from the AMB7300.h
// plan to combine or just init the object at here for check error purpose.
// Need further investigate whether to remove or not.
//#if CM_ENABLE 
//		array<CM ^>^ cm;
//#endif 
//
//#if SMU_ENABLE
//		array<SMU ^>^ smu;
//#endif 
//
//#if DM_ENABLE 
//		array<DM ^>^ dm;
//#endif 
//
//#if DIO_ENABLE 
//		array<DIO ^>^ iom;
//#endif 
//
//	// [Future Enchancement] To ready ACM & TMU dll and then add reference.
//
//#if ACM_ENABLE 
//		array<ACM ^>^ acm;
//#endif 
//
//#if TMU_ENABLE 
//		array<TMU ^>^ tm;
//#endif 

#pragma region "General"

		String ^ TesterId;
		String ^ HardwareProfile;
		String ^ currentFlowName;
		array<String^> ^ currentSubItemName;
		//String ^ currentSubItemName;

		//for Merged SNP 
		int iMergeSnPFile;
		int GeneratedS3PFile = 0;
		array<String^> ^ mergerdTouchstoneFilePath; //path to grab s2p file and merged.
		
		MessageBoxIconFormat messageBoxIcon;
		//bool EnableLogInDebug;
		//bool EnableTracerInDebug;

#pragma endregion

#pragma region "techFlow Property"

		value struct techFlowProperty 
		{
			/*
			**	----------------------------------------------------------------------------------------------------
			**	GetTechFlowSiteProperty
			**	----------------------------------------------------------------------------------------------------
			*/
			int StageCount;										// Identify single site / true parallel / index parallel project
			int TestHead;										// TF test head
			int TestSite;										// TF test site
			int TotalUUTOffsets;								// Total UUTOffset site count
			int TotalTestSite;									// Total tF test site in a project
			int NumberOfTestSites;								// For the ForLoop termination parameter usage, to handle between single site, multiple site, uutoffset
			bool SharedVNA;										//
			int activeUUT_count;
			array<bool>^ arr_activeUUT;
			/*
			**	----------------------------------------------------------------------------------------------------
			**	GetTechFlowProjectType
			**	----------------------------------------------------------------------------------------------------
			*/
			int ProjectType;									// TF project type, eg: single site single uut / single site multi uut / true parallel single uut / true parallel multi uut		

			/*
			**	----------------------------------------------------------------------------------------------------
			**	GetTechFlowFilePathProperty
			**	----------------------------------------------------------------------------------------------------
			*/
			String ^ RecipeFileName;
			String ^ ProfileName;								// eg: SampleProfile / ALP / ...
			String ^ ProjectName;								// TF Project Name
			String ^ DeviceName;								// TF Device Name
			String ^ DeviceRevision;							// TF Device Revision
			String ^ ProgramName;								// TF Program Name
			String ^ ProgramRevision;							// TF Program Revision
			String ^ HandlerPathDirectory;						// C:\Aemulus\techFlow3\Projects\Handlers
			String ^ PSRecipePathDirectory;						// C:\Aemulus\techFlow3\Projects\PSRecipes\SampleProfile\ProjectName
			String ^ TestProgramPathDirectory;					// C:\Aemulus\techFlow3\Projects\TestProgramsSampleProfile\ProjectName
			String ^ RecipeFilePathDirectory;					// C:\Aemulus\techFlow3\Projects\TestRecipesSampleProfile\ProjectName
			String ^ VSProjectPathDirectory;					// C:\Aemulus\techFlow3\Projects\VSProjectsSampleProfile\ProjectName\ProjectName

			/*
			**	----------------------------------------------------------------------------------------------------
			**	GetTechFlowBinningProperty
			**	----------------------------------------------------------------------------------------------------
			*/
			int SoftBinCount;									// To count total number of SoftBin from techFlow
			int HardBinCount;									// To count total number of HardBin from techFlow
			array<int>^ arr_HBin;								// To store value of HardBin for each SoftBin
			array<String^>^ str_arrHBin;						// To store HardBin value as String (for folder path use)
			String ^ G_LotId;
			String ^ G_TestCode;
			bool create_S2PpathByBin_flag = false;

			//放在load中，获取所有的testparameter对应的Hbin信息
			Dictionary <String^, int> ^ TiTpRule_by_HBin;

			//放在SaveToTouchstoneFile方法中，获取每一个料保存时，保存的S2P文件 对应的testitem信息
			Dictionary <String^, String^> ^ Ti_by_S2PFilename;

			//放在commitresult方法里，保存分bin文件夹的路径，包含bin1.1文件夹
			Dictionary <String^, String^> ^ BinString_by_BinPath;
			/*
			**	----------------------------------------------------------------------------------------------------
			**	Other General
			**	----------------------------------------------------------------------------------------------------
			*/
			String ^ CurrentPhase;			
		};
		techFlowProperty tf;
		
#pragma endregion

#pragma region "Tracer Logger Property"

		value struct TracerLoggerProperty
		{
			static TracerLogger ^ TracerLog;
			static double PrefixValue;
			static String ^ TracerTabNamePhysicalSite;
			static String ^ TracerMessage;

			static String ^ InfoMssg					= "[INFO]";
			static String ^ ErrorMssg					= "[ERROR]";
			static String ^ WarningMssg					= "[WARNING]";
			static String ^ TracerWarningTabName		= "WARNING";
			static String ^ TracerExceptionTabName		= "EXCEPTION";
			static String ^ TracerLoadPhaseTabName		= "LOAD PHASE";
			static String ^ TracerTestPhaseTabName		= "TEST PHASE";
			static String ^ TracerUnloadPhaseTabName	= "UNLOAD PHASE";

			static int tracerMainTab = 999;
		};
		TracerLoggerProperty TcrLgr;

#pragma endregion

#pragma region "File Logger Property"

		value struct FileLoggerProperty
		{
			FileLogger ^ FileLgr;
			FileInfo ^ fi;
			bool IsWarning;
			String ^ FileNameDebugLog;
			String ^ LogMessage;
		};
		FileLoggerProperty FileLog;

#pragma endregion

#pragma region "Debug Folder Property"

		value struct DebugFolderProperty
		{

		};
		DebugFolderProperty Debug;

#pragma endregion

#pragma region "TesterInfo Folder Property"

		value struct TesterInfoFolderProperty
		{

		};
		TesterInfoFolderProperty TesterInfo;

#pragma endregion

#pragma region "Wolfer Folder Property"

		value struct WolferFolderProperty
		{

		};
		WolferFolderProperty Wolfer;

#pragma endregion

#pragma region "BoardLossFileFolder Folder Property"

		value struct BoardLossFileFolderProperty
		{
			String ^ BoardLossFileFolderDirectory;						// Reserved for 'BoardLossFileFolder' folder path directory. Example: BoardLossFileFolder
			String ^ BoardLossFileDirectory;							// Reserved for 'BoardLoss_TesterID_Sx.csv' file patt directory. Example: BoardLoss_TesterID_S0.csv, BoardLoss_TesterID_S1.csv
		
			array<int> ^ index;											// BoardLoss file content 'Index'
			array<String^> ^ hardwarePath;								// BoardLoss file content 'HardwarePath'
			array<array<double>^> ^ factorValue;						// BoardLoss file content 'S0/S1/S2/...' boardloss factor
			String ^ BoardLossFileVersion;								// BoardLoss file header 'BoardLoss File Version'
			
			bool isCreateNewBoardLossFile;								// To determine whether to generate a new BoardLoss_TesterID_Sx.csv file
			
			Dictionary<String^, array<double>^> ^ BoardLossFactor;		// Global BoardLoss factor, to be called when necessary
		};
		BoardLossFileFolderProperty BoardLoss;

#pragma endregion

#pragma region "DeviceStateFileTemplate Folder Property"

		value struct DeviceStateFileTemplateProperty
		{
			String ^ DeviceStateFileTemplateDirectory;					// Reserved for 'DeviceStateFileTemplate' folder path directory. Example: DeviceStateFileTemplate

			int totalStateFileExist;									// Total number of state file exist inside the folder (.sta type)
			int totalMappingFileExist;									// Total number of mapping file exist inside the folder (.xml type)

			String ^ stateFileDirectory_sta;							// State file path directory (.sta type)
			String ^ stateFileDirectory_csa;							// State file path directory (.sta type)
			String ^ mappingFileDirectory;								// Mapping file path directory (.xml type)

			bool isDeviceStateFileTemplateStateFIleInvalid;				// To determine whether the state file is valid inside the 'DeviceStateFileTemplate' folder
			bool isDeviceStateFileTemplateMappingFileInvalid;			// To determine whether the mapping file is valid inside the 'DeviceStateFileTemplate' folder
		};
		DeviceStateFileTemplateProperty DeviceStateFileTemplate;

#pragma endregion

#pragma region "FixedOffsetFileFolder Folder Property"

		value struct FixedOffsetFileFolderPorperty
		{
			String ^ FixedOffsetFileFolderDirectory;					// Reserved for 'FixedOffsetFileFolder' folder path directory. Example: FixedOffsetFileFolder
			String ^ FixedOffsetFileDirectory;							// Reserved for 'FixedOffset_TesterID_Sx.csv' file patt directory. Example: FixedOffset_AMB73001_S0.csv, FixedOffset_AMB73001_S1.csv
		
			array<int> ^ index;											// FixedOffset file content 'Index'
			array<String^> ^ testItem;									// FixedOffset file content 'Test Item'
			array<String^> ^ testParameter;								// FixedOffset file content 'Test Parameter'
			array<array<double>^> ^ offsetValue;						// FixedOffset file content 'S0/S1/S2/...' offset value
			String ^ OffsetFileVersion;									// FixedOffset file header 'Offset File Version'
			
			bool isCreateNewFixedOffsetFile;							// To determine whether to generate a new FixedOffset_TesterID_Sx.csv file
			
			//Dictionary<String^, array<double>^> ^ ResultOffset;			// Global fixed offset factor, to be called when update result to techFlow

			Dictionary<String^, double> ^ ResultOffset;			// Global fixed offset factor, to be called when update result to techFlow
		};
		FixedOffsetFileFolderPorperty FixedOffset;

#pragma endregion

#pragma region "ModulationFileFolderSiteX Folder Property"

		value struct ModulationFileFolderPorperty
		{
			String ^ ModulationFileFolderDirectory;						// Reserved for 'ModulationFileFolderSite"X"' folder path directory. Example: ModulationFileFolderSite0, ModulationFileFolderSite1

			int totalAwfTypeFileExist;									// Total number of modulation file exist inside the folder (.awf type)
			int totalWfmTypeFileExist;									// Total number of modulation file exist inside the folder (.wfm type)

			array<String^> ^ awfTypeFilePathList;						// Modulation file path directory (.awf type)
			array<String^> ^ wfmTypeFilePathList;						// Modulation file path directory (.wfm type)

			array<String^> ^ awfTypeFileName;							// Modulation file name (.awf type)
			array<String^> ^ wfmTypeFileName;							// Modulation file name (.wfm type)
		};
		ModulationFileFolderPorperty ModulationFile;

#pragma endregion

#pragma region "VectorFileFolderSiteX Folder Property"

		value struct VectorFileFolderPorperty
		{
			String ^ VectorFileFolderDirectory;						// Reserved for 'VectorFileFolderSite"X"' folder path directory. Example: VectorFileFolderSite0, VectorFileFolderSite1

			int totalVecFileExist;										// Total number of vector file exist inside the folder (.vec type)
			array<String^> ^ vecFilePathList;							// Vector file path directory
			array<String^> ^ vecFileName;								// Vector file name
			array<int> ^ vecFileNumber;									// Vector file number. Example: 0,1,2,3,4,5,...
		};
		VectorFileFolderPorperty VectorFile;

#pragma endregion

#pragma region "VectorStateFileFolderSiteX Folder Property"

		value struct VectorStateFileFolderPorperty
		{
			String ^ VectorStateFileFolderDirectory;				// Reserved for 'VectorStateFileFolderSite"X"' folder path directory. Example: VectorStateFileFolderSite0, VectorStateFileFolderSite1

			int totalVecStateFileExist;									// Total number of vector state file exist inside the folder (.csv type)
			array<String^> ^ vecStateFilePathList;						// Vector state file path directory
			array<String^> ^ vecStateFileName;							// Vector state file name
			array<int> ^ vecStateFileNumber;							// Vector state file number. Example: 0,1,2,3,4,5,...
		};
		VectorStateFileFolderPorperty VectorStateFile;

#pragma endregion

#pragma region "AppsCalFileFolder Folder Property"

		value struct AppsCalFileFolderPorperty
		{
			String ^ AppsCalFileFolderDirectory;					// Reserved for 'AppsCalFileFolder' folder path directory. Example: AppsCalFileFolder
			String ^ AppsCalFileDirectory;							// Reserved for 'AppsCal_TesterID_Sx.csv' file patt directory. Example: AppsCal_AMB73001_S0.csv, AppsCal_AMB73001_S1.csv

			array<int> ^ ChannelIndex;											// AppsCal file content 'Index'
			array<double> ^ TargetPout;									// AppsCal file content 'Test Item'
			array<array<double>^> ^ offsetValue;						// AppsCal file content 'S0/S1/S2/...' offset value
			String ^ OffsetFileVersion;									// AppsCal file header 'Offset File Version'

			bool isCreateNewAppsCalFile;							// To determine whether to generate a new AppsCal_TesterID_Sx.csv file

																		//Dictionary<String^, array<double>^> ^ ResultOffset;			// Global fixed offset factor, to be called when update result to techFlow

			Dictionary<String^, double> ^ AppsCalFactor;			// Global fixed offset factor, to be called when update result to techFlow
		};
		AppsCalFileFolderPorperty AppsCalFile;

#pragma endregion

#pragma region "App-Wide-Variable Property"

		value struct AppWideVariablesProperty
		{
			int UUT;
			bool Offline;										// True = activate Offline mode
			int Debug;											// 0 = debug mode OFF | 1 = debug mode ON
			double PowerLineFreq;								// 0 = 50Hz | 1 = 60Hz
			bool CreateFixedOffsetFile;							// To enable and create a new FixedOffset.csv file
			String ^ SaveSnpData;								// SaveSnpDataOff | SaveSnpDataOn / Re_Imag | Mag_Angle | dB_Angle
			bool EnableSaveSnpData;
			int touchstoneFileDataFormat;
			String ^ S2Ppath;									//S2Ppath
			bool EnableRenameSnpData;							//S2P_rename
			String ^ snpFileName;								//snpFileName
			bool SharedVNA;
			int CalibrationValidityDay;
			bool isSwapS2PData = false;									//是否交换S2P文件内的数据位置
			bool isSaveBinFolder = false;								//是否保存到对应的分bin文件夹内
			bool GenericStateMappingFile_EN;				// 0 = AMB7300Config_DeviceName | 1 = Use specified generic state / mapping file
			String^ GenericStateMappingFile_Name;			// Generic state / ampping file name if GenericStateMappingFile_EN is TRUE
			bool HighPwrTest_EN;							// 0 = Default | 1 = Enable HighPowerTest
			String^ HighPwrTest_AppsCalFile;				// AppsCalFile path if HighPwrTest_EN is TRUE
			bool VNA_Mutex_EN;								// To enable/disable VNA_Lock() in Pre-Processing & VNA_Unlock() in Post-Processing

			//Port Matching circuit filepath
			array<bool>^ PortMatching_EN;
			String^ PortMatching1;
			String^ PortMatching2;
			String^ PortMatching3;
			String^ PortMatching4;
			String^ PortMatching5;
			String^ PortMatching6;
		};
		AppWideVariablesProperty AWV;

#pragma endregion

#pragma region "Resource Management Property"

		value struct ResourceManagementProperty
		{
			array<ResourceManager ^>^ RsrcMngr;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	CM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int cmModuleCount;
			array<String^, 2> ^ moduleAlias_CM_PinMapName;
			array<String^, 2> ^ moduleAlias_CM_Address;
			array<bool, 2> ^ hardwareStatus_CM;
			int cmPinCount;
			array<String^, 2> ^ pinAlias_CM_PinMapName;
			array<String^, 2> ^ pinAlias_CM_Address;
			array<String^, 2> ^ pinAlias_CM_HwResourceAlias;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	AM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int amModuleCount;
			array<String^, 2> ^ moduleAlias_AM_PinMapName;
			array<String^, 2> ^ moduleAlias_AM_Address;
			array<bool, 2> ^ hardwareStatus_AM;
			int amPinCount;
			array<String^, 2> ^ pinAlias_AM_PinMapName;
			array<String^, 2> ^ pinAlias_AM_Address;
			array<String^, 2> ^ pinAlias_AM_HwResourceAlias;
		
			/*
			**	----------------------------------------------------------------------------------------------------
			**	DM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int dmModuleCount;
			array<String^, 2> ^ moduleAlias_DM_PinMapName;
			array<String^, 2> ^ moduleAlias_DM_Address;
			array<bool, 2> ^ hardwareStatus_DM;
			int dmPinCount;
			array<String^, 2> ^ pinAlias_DM_PinMapName;
			array<String^, 2> ^ pinAlias_DM_Address;
			array<String^, 2> ^ pinAlias_DM_HwResourceAlias;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	IOM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int iomModuleCount;
			array<String^, 2> ^ moduleAlias_IOM_PinMapName;
			array<String^, 2> ^ moduleAlias_IOM_Address;
			array<bool, 2> ^ hardwareStatus_IOM;
			int iomPinCount;
			array<String^, 2> ^ pinAlias_IOM_PinMapName;
			array<String^, 2> ^ pinAlias_IOM_Address;
			array<String^, 2> ^ pinAlias_IOM_HwResourceAlias;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	ACM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int acmModuleCount;
			array<String^, 2> ^ moduleAlias_ACM_PinMapName;
			array<String^, 2> ^ moduleAlias_ACM_Address;
			array<bool, 2> ^ hardwareStatus_ACM;
			int acmPinCount;
			array<String^, 2> ^ pinAlias_ACM_PinMapName;
			array<String^, 2> ^ pinAlias_ACM_Address;
			array<String^, 2> ^ pinAlias_ACM_HwResourceAlias;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	TM400 Series Module Alias & Pin Alias
			**	----------------------------------------------------------------------------------------------------
			*/
			int tmModuleCount;
			array<String^, 2> ^ moduleAlias_TM_PinMapName;
			array<String^, 2> ^ moduleAlias_TM_Address;
			array<bool, 2> ^ hardwareStatus_TM;
			int tmPinCount;
			array<String^, 2> ^ pinAlias_TM_PinMapName;
			array<String^, 2> ^ pinAlias_TM_Address;
			array<String^, 2> ^ pinAlias_TM_HwResourceAlias;
		};
		ResourceManagementProperty RsrcManager;

#pragma endregion

#pragma region "Test Property Struct"

		value struct TestPropertyStruct 
		{
			// Test Property
			Dictionary<String^, String^> ^ TestParaNameWithSiteIndex;
			Dictionary<String^, String^> ^ TestParaDisplayNameWithSiteIndex;
			Dictionary<String^, Object^> ^ TestResults;
			Dictionary<String^, Object^> ^ PreviousTestResults;

			// Sub Item
			String ^ ControlItemName;
			String ^ FlowItemName;

			// Test Item
			String ^ TestItemName;
			String ^ TestItemDisplayName;

			// Sub Item (Test Step, Control Step, Test Parameter) 
			int totalSubItem;
			array<Type^> ^ SubFlowItemTypeId;								// Typeid for each flow sub-items in a test item | SubFlowItemTypeId = ControlStep::typeid or TestStep::typeid or TestParameter::typeid 

			// Test Parameter
			int totalTestParameter;
			array<String^> ^ TestParameterName;
			array<String^> ^ TestParameterDisplayName;
			array<Type^> ^ TestParameterTypeId;								// Typeid for test parameter item
			Dictionary<String^, bool> ^ TestParameterExecuted;				// Set true when the item has been executed, else false
			array<int> ^ TestParameterTestStatus;							// 0 = Fail | 1 = Pass 
			Dictionary<String^, bool> ^ TestParameterUpdateResStatus;		// False = Pending to update result to techFlow | True = Update test result to techFlow
			array<bool> ^ IsCurrentTPBypassed;
			array<bool> ^ IsHardwareInvolved;

			// Flow Step (Control Step, Test Step)
			int totalFlowStep;
			array<String^> ^ FlowStepItemName;
			array<String^> ^ FlowStepItemDisplayName;
			array<bool> ^ FlowStepItemExecuted;	

		};
		array<TestPropertyStruct> ^ TestProperty;

#pragma endregion

#pragma region "Test Parameter DataType Property"

		value struct TestParameterDataTypeProperty
		{			
			String ^ StringTyperesult;
			Int16 Int16TypeResult;
			Int32 Int32TypeResult;
			Int64 Int64TypeResult;
			float FloatTyperesult;
			double DoubleTypeResult;
			bool BoolTypeResult;
			UInt16 UInt16TypeResult;
			UInt32 UInt32TypeResult;
			UInt64 UInt64TypeResult;
			int	IntTypeResult;
			unsigned int UIntTypeResult;
		};
		array<TestParameterDataTypeProperty> ^ ResultWithDataType;

#pragma endregion

#pragma region "Flow Variable Property"

		value struct FlowVariablesProperty
		{
			bool AssemblyResolver;								// Set True to perform assembly resolve if the loading of.dll / .exe turns out to be unsucessfull due to different DLL Encryptor used for encryption, else False to by-pass the Assembly Resolve via current domain event handler.
		};
		FlowVariablesProperty FLOWVAR;

#pragma endregion

#pragma region "Data & Time"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Date & Time Now
		**	----------------------------------------------------------------------------------------------------
		*/
		static String ^ FileNameTime = DateTime::Now.ToString("yyyy.MM.dd'_'h.mm.ss.tt");
		String ^ TimeNow;
		String ^ S3P_TimeNow;

#pragma endregion

	};
}


/*----------------------------------------------------------------------
* Revision Log
* $Log: Globals.h.rca$

*** Version	: v1.0.0.5
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Add arr_activeUUT
* Add AWV for HighPwrTest, Mutex & PortMatchingX variables

*** Version	: v1.0.0.4
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Added UseGenericStateFileMappingFile in AWV
* Added S2P_rename & snpFileName
* Modified FixedOffset.ResultOffset datatype from array<double>^ to double

*** Version	: v1.0.0.3
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Added AppWideVariablesProperty variables

*** Version	: v1.0.0.2
*** Date	: 9 April 2024
*** PIC		: Ng Chen Yang
* Added S2Ppath AppWideVariable as global variable.

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* Support SkySemi special BW by detecting "SkySemi" at Analysis_Setting.
* Support for the CSA/STA state file template depends on the VNA model.
* Support specific SaveSnpFilePath entered by users. Pass the file path to AppWideVariable "S2Ppath"

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
----------------------------------------------------------------------*/