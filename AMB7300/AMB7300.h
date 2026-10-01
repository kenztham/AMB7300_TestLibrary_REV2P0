/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			AMB7300.h
Purpose:		Declare all AMB7300 test library functions
Version:		v1.0.0.5
----------------------------------------------------------------------*/


#pragma once

// <AMB7300 DLLs>
#using "C:\Aemulus\techFlow3\bin\NF4\x64\Aemulus.Hardware.AMB7300.dll"
#using "C:\Aemulus\techFlow3\bin\NF4\Aemulus.Configuration.AMB7300.dll"
#using "C:\Aemulus\techFlow3\bin\NF4\Aemulus.Hardware.NetworkAnalyzer.Base.dll"
#using "C:\Aemulus\techFlow3\bin\NF4\Aemulus.Hardware.NetworkAnalyzer.dll"
#using "C:\Aemulus\techFlow3\bin\NF4\Aemulus.Hardware.RFSM.Base.dll"
#using "C:\Aemulus\techFlow3\bin\NF4\Aemulus.Hardware.RFSM.dll"

#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <WinUser.h>
#include "../Test Function/TestFunction.h"
#include "../Test Function/Aemulus.Hardware.CM.h"
#include "../Test Function/Aemulus.Hardware.SMU.h"
#include "../Test Function/Aemulus.Hardware.DM.h"
#include "../Test Function/Aemulus.Hardware.DIO.h"
#include "../AMB7300/AMB7300.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Xml;
using namespace Aemulus::Hardware;
using namespace Aemulus::Configuration;
using namespace Microsoft::VisualBasic;
using namespace Aemulus::Configuration::AMB7300;
using namespace Aemulus::Tech::Flow::ProductionSystem;
using namespace System::Runtime::Remoting;
using namespace System::Globalization;
using namespace System::Threading::Tasks;
using namespace JYPXIe69852;

namespace Functions
{
	public ref class AMB7300TestLibrary
	{
	public:

#if CM_ENABLE 
		array<CM ^>^ cm;
#endif 

#if SMU_ENABLE 
		array<SMU ^>^ smu;
#endif 

#if DM_ENABLE 
		array<DM ^>^ dm;
#endif 
		// [Future Enchancement] To ready CM, IOM, ACM, TMU dll and then add reference. Check encryption issue.
#if DIO_ENABLE 
		array<DIO ^>^ iom;
#endif 

#if ACM_ENABLE 
		array<ACM ^>^ acm;
#endif 

#if TMU_ENABLE 
		array<TMU ^>^ tm;
#endif 

		AMB7300TestLibrary::AMB7300TestLibrary(TestFunction ^ TestFunc);
		~AMB7300TestLibrary(void);

		TestFunction ^ tl;
		Aemulus::TestLib::Utility::Utilities ^ Util;
		int lineNUM;
		String^ timerFilename;

		array <Aemulus::Hardware::AMB7300 ^> ^ amb7300_;
		array <Aemulus::Hardware::AMB7300_Driver ^> ^ amb7300_driver;
		array <Aemulus::Hardware::NetworkAnalyzer ^> ^ amb7300_NA;

		Mutex^ mutex;

		//Shared Memory Object
		array<MemoryMappedFile^> ^ mappedFile;
		array<MemoryMappedViewAccessor^> ^ mappedFileView;

		// Vna hardware init status
		array<bool>^ AMB7300_VnaHardwareStatus;

		// Resource Manager
		array<ResourceManager ^>^ RsrcManager;
		Dictionary <String ^, int> ^ ResourceAlias;

		//Asynchronous Task List
		array<Task<double>^> ^ S3P_TaskList;

		//siteIndex to vnaSiteIndex Mapping
		array<int>^ siteIndex_By_vnaSiteIndex;

		//PortPair to Channel Mapping
		Dictionary<String^, int> ^ PortPair_By_Channel;

#pragma region "Value struct"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Data Type
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct DataTypes
		{
			array<array<Sparameter_Data^>^> ^ SparameterData;
			array<array<BandWidth_Data^>^> ^ BWSearchData;
			array<array<Flatness_Data^>^> ^ FlatnessData;
			array<array<Statistic_Data^>^> ^ StatisticData;

			// Support for ProjectType::SingleTFSiteSingleUUTOffsetWithThread 
			array<array<array<Sparameter_Data^>^>^> ^ iSparameterData;
		};
		DataTypes dataType;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step Condition -> 'DcControl'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct ControlStepCondition_DcControl
		{
			array<String^> ^ module;
			array<String^> ^ pinAlias;
			array<String^> ^ controlFunction;
			array<double> ^ nplc;
			array<double> ^ voltageLevel;
			array<double> ^ currentLevel;
			array<double> ^ delay;
		};
		ControlStepCondition_DcControl DcControlCSC;
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'DcTest'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct TestParameterCondition_DcTest
		{
			String ^ module;
			String ^ pinAlias;
			String ^ testFunction;
			double nplc;
			double measureDelay;
		};
		TestParameterCondition_DcTest DcTestTPC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step Condition -> 'PatternControl'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct ControlStepCondition_PatternControl
		{
			String ^ controlFunction;
			bool isMultiVecToOneResult;
			bool isOneVecToMultiResult;
			String ^ sclkPinAlias;
			String ^ sdataPinAlias;
			double userMipiFreq;
			double userVth;
			double userDmInputChDelay;
			double runVectorDelay;
			array<String^> ^ vectorFileName;

			int overallVectorFileCount;
		};
		ControlStepCondition_PatternControl PatternControlCSC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step Condition -> 'PatternTest'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct TestParameterCondition_PatternTest
		{
			int returnIndex;
		};
		TestParameterCondition_PatternTest PatternTestTPC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step Condition -> 'DmControl'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct ControlStepCondition_DmControl
		{
			String^ controlFunction;
			array<String^> ^ moduleAlias;
			array<String^> ^ pinAlias;
			array<int> ^ outputTrigger0;
			array<int> ^ outputTrigger1;
		};
		ControlStepCondition_DmControl DmControlCSC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Global Variable in 'PatternControl' 
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct PatternControl_GlobalResultVariable
		{
			// Usage: For 'PatternTest' multiple test parameter condition, to avoid the overall result (1st tp test) has been removed when looping the test parameter.
			array<int> ^ historyRam;
			int vecFailCount;
			int vecFirstFail;
			int totalVecReadCount;
			int totalVecFailCount;

			// Usage: For result logger message
			array<String^> ^ resMsg_VecFileName;
			array<int> ^ resMsg_VecFailCount;
			array<int> ^ resMsg_VecFirstFail;
			array<array<int>^> ^ resMsg_HistoryRam;
		};
		PatternControl_GlobalResultVariable PatternControlGlobalResultVariable;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM PE Mode Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct DmPeModeSetting
		{
			double mipiFreq;
			double vih;
			double vil;
			double voh;
			double vol;
			double ioh;
			double iol;
			double vch;
			double vcl;
			double vth;
			double inputChannelDelay;
			int dpinGroup;
		};
		DmPeModeSetting dmPeModeSetting;
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM MIPI Library Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct DmMipiVector
		{
			//int totalVecFileExist;
			//int totalVecStateFileExist;
			//
			//String ^ vecFileFolderName;
			//String ^ vecStateFileFolderName;
			//
			//String ^ vecFileFolderDirectory;
			//String ^ vecStateFileFolderDirectory;
			//
			//array<String^> ^ vecFilePathList;
			//array<String^> ^ vecStateFilePathList;
			//
			//array<String^> ^ vecFileName;
			//array<int> ^ vecFileNumber;

			double runMipiDelay;

			array<String^> ^ displayMipiMode;
			array<int> ^ displayMipiUsid;
			array<int> ^ displayMipiRegAddr;
			array<int> ^ displayMipiRegData;
			array<int> ^ displayMipiRegData1;
			array<int> ^ displayMipiRegData2;
			array<int> ^ displayMipiRegData3;
			array<int> ^ displayMipiRegData4;
			array<int> ^ displayMipiRegData5;
			array<int> ^ displayMipiRegData6;
			array<int> ^ displayMipiRegData7;
			array<int> ^ displayMipiRegData8;
			array<int> ^ displayMipiRegData9;
			array<int> ^ displayMipiRegData10;
			array<int> ^ displayMipiRegData11;
			array<int> ^ displayMipiRegData12;
			array<int> ^ displayMipiRegData13;
			array<int> ^ displayMipiRegData14;
			array<int> ^ displayMipiRegData15;
			array<int> ^ displayMipiRegData16;
			array<String^> ^ displayMipiOperation;
			array<String^> ^ displayMipiSpeed;

			array<int> ^ displayMipiUsidTarget;
			array<int> ^ displayMipiRegAddrTarget;
			array<int> ^ displayMipiRegDataTarget;
		};
		DmMipiVector dmMipiVec;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM SPI Library Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct DmSpiVector
		{
		};
		DmMipiVector dmSpiVec;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM I2C Library Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct DmI2cVector
		{
		};
		DmMipiVector dmI2cVec;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Project State File
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct ProjectStateFileInfo
		{
			// Project state file & mapping file folder directory at local 'ProgramData' folder
			String ^ localStateFileFolderDirectory;

			// Project state file at local 'ProgramData' folder
			String ^ localStateFileName;
			String ^ localStateFileDirectory;

			// To determine whether the state file is valid inside the local 'ProgramData' folder
			bool isLocalProjectStateFileNotFound;	
			String^ SNP_Override = "NA";

			String^ stateFileType;

			array<String^>^ Channels;
			array<String^>^ Channel_Sparam_TraceNo_Info;
			List<String^>^ Trace_Format_List;
		};
		ProjectStateFileInfo projectStateInfo;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Project Mapping File
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct ProjectMappingFileInfo
		{
			// Project mapping file at local 'ProgramData' folder
			String ^ localMappingFileName;
			String ^ localMappingFileDirectory;

			// Array reserve for multiple site usage
			array<int> ^ mapPort1Arr;
			array<int> ^ mapPort2Arr;
			array<int> ^ mapVnaChannelArr;
			array<String^> ^ mapVnaChannelStringArr;
			GroupMapping ^ groupMapping;
			array<Aemulus::Configuration::Group> ^ portXmlMapping;
		};
		ProjectMappingFileInfo projectMappingInfo;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	System Configuration File
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct SystemConfigurationFileInfo
		{
			// AMB7300 system configuration file folder directory at local 'ProgramData' folder
			String ^ localConfigFolderDirectory;
			
			// AMB7300 system configuration file at local 'ProgramData' folder
			String ^ localConfigFileDirectory;

			// AMB7300 system configuration file info
			SystemConfiguration ^ systemConfiguration;
			array<Aemulus::Configuration::AMB7300::ModuleConfiguration^> ^ moduleConfiguration;
			String ^ moduleConfigurationAddress;
			String ^ moduleConfigurationChassis;
			String ^ moduleConfigurationName;
			String ^ moduleConfigurationSlot;
			String ^ moduleConfigurationSN;
			String ^ moduleConfigurationType;
			String ^ configurationType;

			// AMB7300 configuration standard name
			static String ^ amb7300A0ConfigName			= "AMB7300S2P1";
			static String ^ amb7300A1ConfigName			= "AMB7300S2P4";
			static String ^ amb7300A2ConfigName			= "AMB7300S2P8";
			static String ^ amb7300A3ConfigName			= "AMB7300S4P2";
			static String ^ amb7300A4ConfigName			= "AMB7300S4P1";
			static String ^ amb7300A5ConfigName			= "AMB7300S2P16N";
			static String ^ amb7300S2P1DConfigName		= "AMB7300S2P1D";
			static String ^ amb7300S4P1DConfigName		= "AMB7300S4P1D";
			static String ^ amb7300S6P1DConfigName		= "AMB7300S6P1D";
			static String ^ amb7300S6P22NConfigName		= "AMB7300S6P22N";
		};
		SystemConfigurationFileInfo sysConfigInfo;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct VNASettting
		{
			// To specifies the active channel when two or more channel windows are open. 
			// The control commands executed by the user are applied to the active channel respectively.
			// Max 16x channel.
			// The channel info inside the array index will follow the project's mapping file info.
			// 999 (undefined) represent channel not available. 
			// Default setting = 999 (undefined).
			array<int> ^ vna_chn;
			array<int> ^ vna_chn_ave;

			// To specifies the activated (selected) trace as the active vna trace. 
			// User can select the active trace if the active channel window contains two or more traces.
			// The control commands executed by the user are applied to the active trace respectively. 
			// Max 16x trace. 
			// User can freely comfigure the amount of trace to display and its measurement type.
			// 999 (undefined) represent trace not available.
			// Default setting 999 = (undefined).
			array<int> ^ vna_trace;
			array<int> ^ vna_trace_smo;

			// Support Spara measurement in two port and four port system setup, ensure correct measType assignment eg. TwoPortMeasS11, TwoPortMeasS12 etc in order to take effect on "Measured Data Setting". 
			// Use defined measurement defination (eg. TwoPortMeasS21) in Define.h under region AMB7300, to assign the measured parameters (S11, S21, S12 or S22) to a trace. 
			// Max 16x, it is apply to each trace.
			// "uninitiate" represent measurement parameter not configured.
			// Default setting = "uninitiate".
			array<String^> ^ measType;

			// To specifies the trace measurement format. 
			// The control commands executed by the user are applied to the active trace respectively. 
			// Max 16x, it is apply to each trace.
			// User can freely comfigure the type of measurement format on the specific trace.
			// 999 (undefined) represent measurement format not configured.
			// Default setting 999 = (undefined).
			array<int> ^ vna_traceFormat;

			// To specifies the state file required during AMB7300 load phase.  
			// Load project's state file.
			// If project's state file not available, load the generic AMB7300 config state file.
			String ^ StateFile;

			// To specifies the pin alias used for the specified port pair.
			// The pin alias must follows pin map name in the resource planner.
			// Also known as vna port pair to be executed [setting in 'VnaFetch'].
			// The pi
			array<String^> ^ pinAlias;
			array<String^> ^ pinAliasSXP;

			// For port pair assignment usage.
			// Will return from wrapper api.
			array<bool> ^ isReverseRequire;

			// User target total trace to be displayed on the specific channel [setting in 'VnaFetch'].
			// Max 16x trace.
			// Default setting = 4x trace.
			int userTraceCount;

			// Current active channel that reserved for the user input pin alias (port pair).
			int currentVnaFetchChannel;
			int currentVnaFetchChannelAve;
			bool ChannelNumCondition_Exist;
			int measureSnpType;

			int VNA_TraceCount =0;
		};
		array<VNASettting> ^ vnaSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Segment Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct SParamSegment
		{
			// Specifies the display format setting. By default the display format for all the traces is set to logarithmic magnitude(dB). 
			SParam_FormatVNA ^ sparaFormat;

			// [Sweep Span Setting] The stimulus power level should be set for linear and logarithmic frequency sweeps. 
			// For the segment sweep type, the method of power level setting described can be used only if the same power level is set for all the segments of the sweep.
			array<double> ^ power;

			// [Sweep Span Setting] For any segment it is necessary to set the mandatory parameters: frequency range and number of sweep points.
			// Set the start frequency values of the sweep range. 
			array<double> ^ startFreq;

			// [Sweep Span Setting] For any segment it is necessary to set the mandatory parameters: frequency range and number of sweep points.
			// Set the stop frequency values of the sweep range. 
			array<double> ^ stopFreq;

			// [Sweep Span Setting] For any segment user enable to edit additional parameter, IF bandwidth. 
			// Set the list of IF bandwidth. 
			array<double> ^ ifbw;

			// [Sweep Span Setting] The number of sweep points should be set for linear and logarithmic frequency sweeps, and for linear power sweep.
			// Set the number of sweep points. 
			array<int> ^ point;

			// [Sweep Span Setting] For any segment user allow to edit additional parameter, delay time. Set the list of IF bandwidth. 
			// Measurement delay function allows for adding an additional time interval at each measurement point between the moment when the source output frequency becomes stable and the start of the measurement.
			// This capability can be useful for measurements in narrowband circuits with transient periods longer than the measurement time per point.
			array<double> ^ delay;

			// Reserved for single marker setup for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API only. 
			array<double> ^ singleMarker;

			// Total number of segment count in a segment table. Used to defined the array size of array<double> ^ functions::amb7300::SParamSegment::startFreq, stopFreq, point, ifbw, power and etc. 
			int segmentCount;

			// [Sweep Span Setting] Support SWEEP_TYPE_POWER_SWEEP, SWEEP_TYPE_LIN_FREQ, SWEEP_TYPE_LOG_FREQ, SWEEP_TYPE_SEGMENT. 
			// Note: The stimulus parameter settings apply to each channel. Before you set the stimulus parameters of a channel, make the channel active. 
			String ^ sweepType;

			// [state Sweep Type] Define the type of sweeping when using GetFromState File. Suport Linear and Segment Sweek.
			String ^ stateSweepType;

			// Total number of sweep points. 
			int totalPoint;

			// Specifies start/Center Power in dBm.
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.
			double startPow;

			// Specifies stop/Span Power in dBm. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.   
			double stopPow;

			// Specifies power level in dBm. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.  
			double powLevel;

			// Enable to fetch all the defined markers; 
			// User is allow to define more than 16 markers per trace, it's capable to segment/group the markers into sequencial segment of markers, 
			// perform marker measurement and return all the defined measurement type (Sparameter_Data, BWSearchData and etc).  
			bool EnableFetchMarker;

			// Enable to fetch all the Formated Data from current selected PortPairing channel with the defined SegmentSetting; 
			// User no need to define the required SegmentSetting eg startFreq, stopFreq, point, ifbw etc.   
			bool EnableFetchFormatedData;

			// Set True to use long Aemulus::Hardware::IAMB7300_Driver::MeasureSParam API. 
			// Note: Limited to 15 markers measurement only. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSParam API.  
			bool EnableMeasSpara;

			// Set True to by-passed the segment setting and direct refer/depend the segment setting stated in state file (.sta). 
			// Set False to set up Linear S-parameter (Frequency/Power sweep) measurement setting in frequency points.
			// Note: Analyzer settings that become saved into the Analyzer state file (.sta) are the segment settings that measurement refer to.  
			bool GetFromStateFile;

			//  User can activate up to 16 markers on each trace. The markers support following tasks:
			//		+ Reading absolute values of a stimulus and a measured parameter in selected points on the trace. 
			//		+ Reading relative values of a stimulus and a measured parameter related to the reference point. 
			//		+ Search for specific points on the trace (minimum, maximum, target level, etc.). 
			//		+ Determining trace parameters (statistics, bandwidth, etc.). 
			//		+ Editing stimulus parameters using markers.
			array<double> ^ markers;

			// Specifies measurement power marker points in array.
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API. 
			// Turning ON a marker with the number from 1 to 15 will turn ON all the markers of smaller numbers.
			// Turning OFF a marker with the number from 1 to 15 will turn OFF all the markers of greater numbers(except of the reference marker with number 16).
			// Turning ON / OFF the reference marker with number 16 does not turn ON / OFF the markers with the numbers from 1 to 15, but switchs these markers between relative and absolute measurement mode.
			array<double> ^ markersPow;

			// Set True to enable fetch S3P formated data from three traces. 
			// System will ignore to fetch the formated trace data at first and second sweep and only fetch the trace formated data at third sweep. 
			// Default set as False. 
			bool fetchS3PFormatedData;

			// Set True to enable fetch S3P marker data from three traces. 
			// System will ignore to fetch the trace data at first and second sweep and only fetch the marker data at third sweep. 
			// Default set as False. 
			bool fetchS3PMarkerData;

			// Condition change flag for sweep setting change check
			// Default set as False
			bool SettingHasChanged = false;
		};
		array<SParamSegment> ^ segmentSetting;

		value struct prev_SParamSegment
		{
			// Specifies the display format setting. By default the display format for all the traces is set to logarithmic magnitude(dB). 
			SParam_FormatVNA ^ sparaFormat;

			// [Sweep Span Setting] The stimulus power level should be set for linear and logarithmic frequency sweeps. 
			// For the segment sweep type, the method of power level setting described can be used only if the same power level is set for all the segments of the sweep.
			array<double> ^ power;

			// [Sweep Span Setting] For any segment it is necessary to set the mandatory parameters: frequency range and number of sweep points.
			// Set the start frequency values of the sweep range. 
			array<double> ^ startFreq;

			// [Sweep Span Setting] For any segment it is necessary to set the mandatory parameters: frequency range and number of sweep points.
			// Set the stop frequency values of the sweep range. 
			array<double> ^ stopFreq;

			// [Sweep Span Setting] For any segment user enable to edit additional parameter, IF bandwidth. 
			// Set the list of IF bandwidth. 
			array<double> ^ ifbw;

			// [Sweep Span Setting] The number of sweep points should be set for linear and logarithmic frequency sweeps, and for linear power sweep.
			// Set the number of sweep points. 
			array<int> ^ point;

			// [Sweep Span Setting] For any segment user allow to edit additional parameter, delay time. Set the list of IF bandwidth. 
			// Measurement delay function allows for adding an additional time interval at each measurement point between the moment when the source output frequency becomes stable and the start of the measurement.
			// This capability can be useful for measurements in narrowband circuits with transient periods longer than the measurement time per point.
			array<double> ^ delay;

			// Reserved for single marker setup for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API only. 
			array<double> ^ singleMarker;

			// Total number of segment count in a segment table. Used to defined the array size of array<double> ^ functions::amb7300::SParamSegment::startFreq, stopFreq, point, ifbw, power and etc. 
			int segmentCount;

			// [Sweep Span Setting] Support SWEEP_TYPE_POWER_SWEEP, SWEEP_TYPE_LIN_FREQ, SWEEP_TYPE_LOG_FREQ, SWEEP_TYPE_SEGMENT. 
			// Note: The stimulus parameter settings apply to each channel. Before you set the stimulus parameters of a channel, make the channel active. 
			String ^ sweepType;

			// [state Sweep Type] Define the type of sweeping when using GetFromState File. Suport Linear and Segment Sweek.
			String ^ stateSweepType;

			// Total number of sweep points. 
			int totalPoint;

			// Specifies start/Center Power in dBm.
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.
			double startPow;

			// Specifies stop/Span Power in dBm. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.   
			double stopPow;

			// Specifies power level in dBm. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API.  
			double powLevel;

			// Enable to fetch all the defined markers; 
			// User is allow to define more than 16 markers per trace, it's capable to segment/group the markers into sequencial segment of markers, 
			// perform marker measurement and return all the defined measurement type (Sparameter_Data, BWSearchData and etc).  
			bool EnableFetchMarker;

			// Enable to fetch all the Formated Data from current selected PortPairing channel with the defined SegmentSetting; 
			// User no need to define the required SegmentSetting eg startFreq, stopFreq, point, ifbw etc.   
			bool EnableFetchFormatedData;

			// Set True to use long Aemulus::Hardware::IAMB7300_Driver::MeasureSParam API. 
			// Note: Limited to 15 markers measurement only. 
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSParam API.  
			bool EnableMeasSpara;

			// Set True to by-passed the segment setting and direct refer/depend the segment setting stated in state file (.sta). 
			// Set False to set up Linear S-parameter (Frequency/Power sweep) measurement setting in frequency points.
			// Note: Analyzer settings that become saved into the Analyzer state file (.sta) are the segment settings that measurement refer to.  
			bool GetFromStateFile;

			//  User can activate up to 16 markers on each trace. The markers support following tasks:
			//		+ Reading absolute values of a stimulus and a measured parameter in selected points on the trace. 
			//		+ Reading relative values of a stimulus and a measured parameter related to the reference point. 
			//		+ Search for specific points on the trace (minimum, maximum, target level, etc.). 
			//		+ Determining trace parameters (statistics, bandwidth, etc.). 
			//		+ Editing stimulus parameters using markers.
			array<double> ^ markers;

			// Specifies measurement power marker points in array.
			// This setting is reserved for long Aemulus::Hardware::IAMB7300_Driver::MeasureSparamSetupPowerSweep API. 
			// Turning ON a marker with the number from 1 to 15 will turn ON all the markers of smaller numbers.
			// Turning OFF a marker with the number from 1 to 15 will turn OFF all the markers of greater numbers(except of the reference marker with number 16).
			// Turning ON / OFF the reference marker with number 16 does not turn ON / OFF the markers with the numbers from 1 to 15, but switchs these markers between relative and absolute measurement mode.
			array<double> ^ markersPow;

			// Set True to enable fetch S3P formated data from three traces. 
			// System will ignore to fetch the formated trace data at first and second sweep and only fetch the trace formated data at third sweep. 
			// Default set as False. 
			bool fetchS3PFormatedData;

			// Set True to enable fetch S3P marker data from three traces. 
			// System will ignore to fetch the trace data at first and second sweep and only fetch the marker data at third sweep. 
			// Default set as False. 
			bool fetchS3PMarkerData;
		};
		array<prev_SParamSegment> ^ prev_segmentSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Average Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct AverageSettting
		{
			// Selective option when execute SaveToTouchstoneFile
			// True: enable active transform | False: disable
			// Default setting = False
			bool enableActiveTraceTransform;

			// True: turn on averaging (ave correction) | False: turn off
			// Default setting = False
			bool enableAveCorrection;

			// True: turn on smoothing (smo correction) | False: turn off
			// Default setting = False
			bool enableSmoCorrection;

			//Specifies the segment count number with smoothing averaging. Default set as 1.
			int SmoSegmentCount;
			int SmoSegmentLength;

			// Specifies the vna channel to perform smoothing function. Only specified vna channel will take effect. 
			// For example, vnaCHwSmo[0] = 1 represent vna channel 1.  
			// Note that smoothing of the sweep result averages adjacent points of the trace as determined by the moving aperture. Smoothing helps to reduce noise bursts.
			array<int> ^ vnaCHwSmo;

			// Specifies the vna channel with vna trace to perform smoothing function. Only specified vna channel will take effect. 
			// For example, vnaCHwSmo[0] = 1 represent vna channel 1.  
			// Note that smoothing of the sweep result averages adjacent points of the trace as determined by the moving aperture. Smoothing helps to reduce noise bursts.
			array<int> ^ vnaCHTracewSmo;

			// Specifies the vna channel to perform averaging function. Only specified vna channel will take effect. 
			// For example, vnaCHwAvgFactor[0] = 1 represent vna channel 1. 
			// Note that averaging is performed at a measurement point over several previous sweeps. It allows for reduction of self - noise and widening the dynamic measurement range of the Analyzer.
			array<int> ^ vnaCHwAvgFactor;

			// Specifies the vna channel with vna trace to perform averaging function. Only specified vna channel will take effect. 
			// For example, vnaCHwAvgFactor[0] = 1 represent vna channel 1. 
			// Note that averaging is performed at a measurement point over several previous sweeps. It allows for reduction of self - noise and widening the dynamic measurement range of the Analyzer.
			array<int> ^ vnaCHTracewAvgFactor;

			//Specifies the start index of sparameter data among the total points with smoothing averaging. Default set as 0.
			array<int> ^ SmoStartIndex;

			//Specifies the stop index of sparameter data among the total points with smoothing averaging. Default set as 0.
			array<int> ^ SmoStopIndex;

			//Specifies the measurement types with smoothing averaging. Array index represent segment index with smoothing.
			array<String^> ^ SmoMeasTypes;

			// True when turns ON the trace smoothing, else False(OFF).
			bool EnSmoCorrectionCTRL;
		};
		array<AverageSettting> ^ averageSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Marker Search
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct MarkerSearchSetting
		{
			/*
			**	----------------------------------------------------------------------------------------------------
			**	Generic Marker Search
			**	----------------------------------------------------------------------------------------------------
			*/
			int markerIndex;
			int targetMarkerCount; // For array size usage.
			array<double> ^ targetMakerFreq;
			// Softkey	: Markers --> Marker Search
			array<bool> ^ ms_TrackingEnable;
			array<bool> ^ ms_SearchRangeEnable;
			array<double> ^ ms_SearchStart;
			array<double> ^ ms_SearchStop;
			array<bool> ^ ms_CoupleEnable;
			array<bool> ^ ms_SearchRangeLinesEnable;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Search Min MAx
			**	----------------------------------------------------------------------------------------------------
			*/
			// Softkey	: Markers --> Marker Search --> Maximum / Minimum
			// Maximum	: enum 0
			// Minimum	: enum 1
			array<int> ^ msMinMax_Type;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Search Peak
			**	----------------------------------------------------------------------------------------------------
			*/
			// Softkey	: Markers --> Marker Search --> Peak --> Search Peak / Search MaxPeak (unavailable) / Search Peak Left / Search Peak Right
			// Search Peak			: enum 2
			// Search Peak Left		: enum 3
			// Search Peak Right	: enum 4
			array<int> ^ msPeak_Type;
			// Softkey	: Markers --> Marker Search --> Peak --> Peak Excursion
			array<double> ^ msPeak_PeakExcursion;
			// Softkey	: Markers --> Marker Search --> Peak --> Peak Polarity --> Positive / Negative / Both
			// Positive	: enum 0
			// Negative	: enum 1
			// Both		: enum 2
			array<int> ^ msPeak_PeakPolarity;
			/*//Peak Threshold level
			array<double> ^ msPeak_PeakThreshold;*/

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Search Target
			**	----------------------------------------------------------------------------------------------------
			*/
			// Softkey	: Markers --> Marker Search --> Target --> Search Target / Search Target Left / Search Target Right
			// Search Target		: enum 5
			// Search Target Left	: enum 6
			// Search Target Right	: enum 7
			array<int> ^ msTarget_Type;
			// Softkey	: Markers --> Marker Search --> Target --> Target Value
			array<double> ^ msTarget_TargetValue;
			// Softkey	: Markers --> Marker Search --> Target --> Target Transition --> Positive / Negative / Both
			// Positive	: enum 0
			// Negative	: enum 1
			// Both		: enum 2
			array<int> ^ msTarget_TargetTransition;
			// Softkey	: Markers --> Marker Search --> Target --> Target Line
			array<bool> ^ msTarget_TargetLineEnable;
		};
		array<MarkerSearchSetting> ^ msSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Marker Math
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct MarkerMathSetting
		{
			/*
			**	----------------------------------------------------------------------------------------------------
			**	Generic Marker Math
			**	----------------------------------------------------------------------------------------------------
			*/
			int targetMarkerCount; // For array size usage.

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Math Statistics
			**	----------------------------------------------------------------------------------------------------
			*/
			// Softkey	: Markers --> Marker Math --> Statistics --> Statistics Range
			array<bool> ^ mmStatistics_StatisticsRangeEnable;
			// Softkey	: Markers --> Marker Math --> Statistics --> Statistics Start
			array<double> ^ mmStatistics_StatisticsStart;
			// Softkey	: Markers --> Marker Math --> Statistics --> Statistics Stop
			array<double> ^ mmStatistics_StatisticsStop;

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Math Bandwidth Search
			**	----------------------------------------------------------------------------------------------------
			*/
			// Softkey	: Markers --> Marker Math --> Bandwidth Search --> Type --> Bandpass / Notch
			// Bandpass	: enum 0
			// Notch	: enum 1
			array<int> ^ mmBwSearch_Type;
			// Softkey	: Markers --> Marker Math --> Bandwidth Search --> Search Ref --> Marker, Maximum, Minimum
			// Marker	: enum 0
			// Maximum	: enum 1
			// Minimum	: enum 2
			array<int> ^ mmBwSearch_SearchRef;
			// Softkey	: Markers --> Marker Math --> Bandwidth Search --> Bandwidth Value
			array<double> ^ mmBwSearch_BandwidthValue;
			// For wrapper API usage only
			array<double> ^ mmBwSearch_Marker1; // use
			array<double> ^ mmBwSearch_Marker2; // dummy

			/*
			**	----------------------------------------------------------------------------------------------------
			**	Marker Math Flatness
			**	----------------------------------------------------------------------------------------------------
			*/
			// For wrapper API usage only
			array<double> ^ mmFlatness_Marker1; // use
			array<double> ^ mmFlatness_Marker2; // use
		};
		array<MarkerMathSetting> ^ mmSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Shared Memory Transfer
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct SharedMemoryTransferSetting
		{
			/*
			**	----------------------------------------------------------------------------------------------------
			**	Generic Marker Math
			**	----------------------------------------------------------------------------------------------------
			*/
			int targetMarkerCount; // For array size usage.

			/*
			**	----------------------------------------------------------------------------------------------------
			**	SharedMemoryTransfer Min Max
			**	----------------------------------------------------------------------------------------------------
			*/
			int searchRangeOption, searchOption;
			double userStartFreq, userStopFreq;

			//PostProc Settings
			double StartFreq;
			double StopFreq;

			double TargetValue;
			array<double>^ TargetFrequency;
			int Transition; //0-pos, 1-neg, 2-both
			int BwOption; //0-Bw Search, 1-Notch Search
			double BwLevel;
			int MarkerRefOption; //0 -marker, 1 - peak
			double M1Freq;

		};
		array<SharedMemoryTransferSetting> ^ smtSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Control Step Condition -> 'VnaFetch' -> 'AverageSetting'
		**	----------------------------------------------------------------------------------------------------
		*/ 
		value struct VnaFetchConditionAveragingSettings
		{
			String ^ averagingEnable;
			double averagingFactor;
			String ^ averagingTrigger;
		};
		VnaFetchConditionAveragingSettings averagingSettings;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'VnaDataAnalysis'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct VnaDataAnalysisTestParameterCondition
		{
			int traceIndex;
			int traceIndexSmo;

			String ^ function;

			String ^ analysisSetting;
			
			//String ^ smoothingSetting;

			array<double> ^ testFreq;
			array<double> ^ testFreqRangeStart;
			array<double> ^ testFreqRangeStop;
			int overallTestFreqCount;

			array<double> ^ testPower;
			array<double> ^ testPowerRangeStart;
			array<double> ^ testPowerRangeStop;
			int overallTestPowerCount;

			String ^ outputType;
			//double ^ Threshold;
			String ^ outputFormat;

			bool saveSnpData;
		};
		VnaDataAnalysisTestParameterCondition vnaDataAnalysisTPC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'VnaSwTime'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct VnaSwTimeTestParameterCondition
		{
			String ^ measureType;

			String ^ dmModuleAlias;
			String ^ dmVectorTrigger;

			String ^ analysisSetting;
			String ^ vnaSwTime_TriggerEdge;
			String ^ vnaSwTime_MovingAvgState;
			String ^ vnaSwTime_SearchLimitState;
			double vnaSwTime_SearchLimitLevel;

			String ^ outputType;
			String ^ outputFormat;

			bool saveSnpData;
		};
		VnaSwTimeTestParameterCondition vnaSwTimeTPC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'VnaDataAnalysis' -> 'AnalysisSetting'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct VnaDataAnalysisTestParameterConditionAnalysisSetting
		{
			//-- NA for TraceData

			// msMinMax
			array<String^> ^ msMinMax_SearchRangeEnable;
			array<String^> ^ msMinMax_SearchType;

			// msPeak
			array<String^> ^ msPeak_Type;
			array<double> ^ msPeak_PeakExcursion;
			array<String^> ^ msPeak_PeakPolarity;
			/*array<double> ^ msPeak_PeakThreshold;*/

			// msTarget
			array<String^> ^ msTarget_Type;
			array<double> ^ msTarget_TargetValue;
			array<String^> ^ msTarget_TargetTransition;
			array<String^> ^ msTarget_TargetLineEnable;

			// mmStatistics
			array<String^> ^ mmStatistics_StatisticsRangeEnable;

			// mmBwSearch
			array<String^> ^ mmBwSearch_Type;
			array<String^> ^ mmBwSearch_SearchRef;
			array<double> ^ mmBwSearch_BandwidthValue;
			array<String^> ^ mmBwSearch_Condition;

			// SharedMemoryDataMinMax
			array<String^> ^ SharedMemorySearchRangeEnable;
			array<String^> ^ SharedMemoryDataMinMax_SearchType;

			// SharedMemoryDataTarget
			array<String^> ^ SharedMemoryDataTarget_Type;
			array<double> ^ SharedMemoryDataTarget_TargetValue;
			array<String^> ^ SharedMemoryDataTarget_TargetTransition;
			array<String^> ^ SharedMemoryDataTarget_TargetLineEnable;

			// SharedMemoryDataBWSearch
			array<String^> ^ SharedMemoryDataBWSearch_Type;
			array<String^> ^ SharedMemoryDataBWSearch_SearchRef;
			array<double> ^ SharedMemoryDataBWSearch_BandwidthValue;
			array<String^> ^ SharedMemoryDataBWSearch_Condition;

			//--  NA for mmFlatness

			//  TraceData_Sds
			array<String^> ^ TraceData_Sds_SearchRangeEnable;
			array<String^> ^ TraceData_Sds_SearchType;
		};
		VnaDataAnalysisTestParameterConditionAnalysisSetting analysisSetting;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'VnaDataAnalysis' -> 'SmoothingSetting'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct VnaDataAnalysisTestParameterConditionSmoothingSettings
		{
			String ^ smoothingEnable;
			double smoothingAperture;
		};
		VnaDataAnalysisTestParameterConditionSmoothingSettings smoothingSettings;

		value struct VnaDataAnalysisTestParameterConditionChannelUse
		{
			String ^ ChannelUseFristChannel;
			String^ ChannelUseSecondChannel;
		};
		VnaDataAnalysisTestParameterConditionChannelUse ChannelUseTPC;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Test Parameter Condition -> 'Math'
		**	----------------------------------------------------------------------------------------------------
		*/
		value struct TestParameterCondition_Math
		{
			String ^ mathFunction;
			String ^ mathTP1;
			String ^ mathTP2;
		};
		TestParameterCondition_Math MathTPC;

		//External Digitizer
		value struct PXIe9852_Info
		{
			JYPXIe69852AITask ^ aitask0;
			//		JYPXIe69852AITask ^ aitask1;

			array<int> ^ slotNo;
			double sampleRate;
			double sampleNumber_double;
			int sampleNumber;
			array<double> ^ readValue0;
			array<double> ^ readValue1;
			array<double> ^ Power_dBmSite0;
			array<double> ^ Power_dBmSite1;
			array<double> ^ Power_dBmSite2;
			array<double> ^ Power_dBmSite3;
			array<double> ^ Power_dBmSite0_Avg;
			array<double> ^ Power_dBmSite1_Avg;
			array<double> ^ Power_dBmSite2_Avg;
			array<double> ^ Power_dBmSite3_Avg;
			array<double> ^ readValueSite0;
			array<double> ^ readValueSite1;
			array<double> ^ readValueSite2;
			array<double> ^ readValueSite3;
			double deltaF;
			double g_SINAD;
			array<String^, 2> ^ NF_data; //debug
			int NF_counter; //debug
		};
		PXIe9852_Info^ pXIe9852_Info;
#pragma endregion

#pragma region "Reference struct"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	AMB7300 System Info
		**	----------------------------------------------------------------------------------------------------
		*/
		ref struct AMB7300SystemSetting
		{
			int vnaSystemCount;						// Acutal vna hardware inside AMB7300 | aka = tF Site = vnaSiteIndex | Apply in data analysis array
			int systemConfig;						// A0 / A1 / A2 / A3 / A4 / A5 | Apply in CMT Init
			String ^ systemAlias;					// Supported format: AMB7300_S2P4_TYPE, AMB7300_S2P8_TYPE, AMB7300_S4P2_TYPE, AMB7300_S4P4_TYPE, AMB7300_S2P16_TYPE | Apply in CMT Init
					
			String ^ amb7300ConfigName;				//To obtain AMB7300config type(AMB7300A0 / AMB7300A1 / AMB7300A2 / ...)
		};
		AMB7300SystemSetting ^ amb7300SystemSetting;	

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Results Data -> Trace Data, Marker Search Data, Marker Math Data
		**	----------------------------------------------------------------------------------------------------
		*/
		ref struct TraceFormatData
		{
			// S1x [S11~S66]
			array<double, 2> ^ S11Real;
			array<double, 2> ^ S11Imaginary;
			array<double, 2> ^ S11RealSmo;
			array<double, 2> ^ S11ImaginarySmo;
			array<double, 2> ^ S12Real;
			array<double, 2> ^ S12Imaginary;
			array<double, 2> ^ S12RealSmo;
			array<double, 2> ^ S12ImaginarySmo;
			array<double, 2> ^ S13Real;
			array<double, 2> ^ S13Imaginary;
			array<double, 2> ^ S13RealSmo;
			array<double, 2> ^ S13ImaginarySmo;
			array<double, 2> ^ S14Real;
			array<double, 2> ^ S14Imaginary;
			array<double, 2> ^ S14RealSmo;
			array<double, 2> ^ S14ImaginarySmo;
			array<double, 2> ^ S15Real;
			array<double, 2> ^ S15Imaginary;
			array<double, 2> ^ S15RealSmo;
			array<double, 2> ^ S15ImaginarySmo;
			array<double, 2> ^ S16Real;
			array<double, 2> ^ S16Imaginary;
			array<double, 2> ^ S16RealSmo;
			array<double, 2> ^ S16ImaginarySmo;
			// S2x [S11~S66]
			array<double, 2> ^ S21Real;
			array<double, 2> ^ S21Imaginary;
			array<double, 2> ^ S21RealSmo;
			array<double, 2> ^ S21ImaginarySmo;
			array<double, 2> ^ S22Real;
			array<double, 2> ^ S22Imaginary;
			array<double, 2> ^ S22RealSmo;
			array<double, 2> ^ S22ImaginarySmo;
			array<double, 2> ^ S23Real;
			array<double, 2> ^ S23Imaginary;
			array<double, 2> ^ S23RealSmo;
			array<double, 2> ^ S23ImaginarySmo;
			array<double, 2> ^ S24Real;
			array<double, 2> ^ S24Imaginary;
			array<double, 2> ^ S24RealSmo;
			array<double, 2> ^ S24ImaginarySmo;
			array<double, 2> ^ S25Real;
			array<double, 2> ^ S25Imaginary;
			array<double, 2> ^ S25RealSmo;
			array<double, 2> ^ S25ImaginarySmo;
			array<double, 2> ^ S26Real;
			array<double, 2> ^ S26Imaginary;
			array<double, 2> ^ S26RealSmo;
			array<double, 2> ^ S26ImaginarySmo;
			// S3x [S11~S66]
			array<double, 2> ^ S31Real;
			array<double, 2> ^ S31Imaginary;
			array<double, 2> ^ S31RealSmo;
			array<double, 2> ^ S31ImaginarySmo;
			array<double, 2> ^ S32Real;
			array<double, 2> ^ S32Imaginary;
			array<double, 2> ^ S32RealSmo;
			array<double, 2> ^ S32ImaginarySmo;
			array<double, 2> ^ S33Real;
			array<double, 2> ^ S33Imaginary;
			array<double, 2> ^ S33RealSmo;
			array<double, 2> ^ S33ImaginarySmo;
			array<double, 2> ^ S34Real;
			array<double, 2> ^ S34Imaginary;
			array<double, 2> ^ S34RealSmo;
			array<double, 2> ^ S34ImaginarySmo;
			array<double, 2> ^ S35Real;
			array<double, 2> ^ S35Imaginary;
			array<double, 2> ^ S35RealSmo;
			array<double, 2> ^ S35ImaginarySmo;
			array<double, 2> ^ S36Real;
			array<double, 2> ^ S36Imaginary;
			array<double, 2> ^ S36RealSmo;
			array<double, 2> ^ S36ImaginarySmo;
			// S4x [S11~S66]
			array<double, 2> ^ S41Real;
			array<double, 2> ^ S41Imaginary;
			array<double, 2> ^ S41RealSmo;
			array<double, 2> ^ S41ImaginarySmo;
			array<double, 2> ^ S42Real;
			array<double, 2> ^ S42Imaginary;
			array<double, 2> ^ S42RealSmo;
			array<double, 2> ^ S42ImaginarySmo;
			array<double, 2> ^ S43Real;
			array<double, 2> ^ S43Imaginary;
			array<double, 2> ^ S43RealSmo;
			array<double, 2> ^ S43ImaginarySmo;
			array<double, 2> ^ S44Real;
			array<double, 2> ^ S44Imaginary;
			array<double, 2> ^ S44RealSmo;
			array<double, 2> ^ S44ImaginarySmo;
			array<double, 2> ^ S45Real;
			array<double, 2> ^ S45Imaginary;
			array<double, 2> ^ S45RealSmo;
			array<double, 2> ^ S45ImaginarySmo;
			array<double, 2> ^ S46Real;
			array<double, 2> ^ S46Imaginary;
			array<double, 2> ^ S46RealSmo;
			array<double, 2> ^ S46ImaginarySmo;
			// S5x [S11~S66]
			array<double, 2> ^ S51Real;
			array<double, 2> ^ S51Imaginary;
			array<double, 2> ^ S51RealSmo;
			array<double, 2> ^ S51ImaginarySmo;
			array<double, 2> ^ S52Real;
			array<double, 2> ^ S52Imaginary;
			array<double, 2> ^ S52RealSmo;
			array<double, 2> ^ S52ImaginarySmo;
			array<double, 2> ^ S53Real;
			array<double, 2> ^ S53Imaginary;
			array<double, 2> ^ S53RealSmo;
			array<double, 2> ^ S53ImaginarySmo;
			array<double, 2> ^ S54Real;
			array<double, 2> ^ S54Imaginary;
			array<double, 2> ^ S54RealSmo;
			array<double, 2> ^ S54ImaginarySmo;
			array<double, 2> ^ S55Real;
			array<double, 2> ^ S55Imaginary;
			array<double, 2> ^ S55RealSmo;
			array<double, 2> ^ S55ImaginarySmo;
			array<double, 2> ^ S56Real;
			array<double, 2> ^ S56Imaginary;
			array<double, 2> ^ S56RealSmo;
			array<double, 2> ^ S56ImaginarySmo;
			// S6x [S11~S66]
			array<double, 2> ^ S61Real;
			array<double, 2> ^ S61Imaginary;
			array<double, 2> ^ S61RealSmo;
			array<double, 2> ^ S61ImaginarySmo;
			array<double, 2> ^ S62Real;
			array<double, 2> ^ S62Imaginary;
			array<double, 2> ^ S62RealSmo;
			array<double, 2> ^ S62ImaginarySmo;
			array<double, 2> ^ S63Real;
			array<double, 2> ^ S63Imaginary;
			array<double, 2> ^ S63RealSmo;
			array<double, 2> ^ S63ImaginarySmo;
			array<double, 2> ^ S64Real;
			array<double, 2> ^ S64Imaginary;
			array<double, 2> ^ S64RealSmo;
			array<double, 2> ^ S64ImaginarySmo;
			array<double, 2> ^ S65Real;
			array<double, 2> ^ S65Imaginary;
			array<double, 2> ^ S65RealSmo;
			array<double, 2> ^ S65ImaginarySmo;
			array<double, 2> ^ S66Real;
			array<double, 2> ^ S66Imaginary;
			array<double, 2> ^ S66RealSmo;
			array<double, 2> ^ S66ImaginarySmo;

			// Total number of frequency points of the SegmentSetting. For array size usage. 
			int totalFreqPoints;
			// Returned
			array<double> ^ XAxis;						// Generic API returned
			array<double> ^ realRaw;					// Generic API returned
			array<double> ^ imaginaryRaw;				// Generic API returned
			array<double, 2> ^ freqPoint;				// Data storage

			// To store temporary results when doing S3P or S4P
			array<double, 2> ^ S11Real_Temp;
			array<double, 2> ^ S11Imaginary_Temp;
			array<double, 2> ^ S12Real_Temp;
			array<double, 2> ^ S12Imaginary_Temp;
			array<double, 2> ^ S21Real_Temp;
			array<double, 2> ^ S21Imaginary_Temp;
			array<double, 2> ^ S22Real_Temp;
			array<double, 2> ^ S22Imaginary_Temp;
			
			// The Second Channel <<< Total number of frequency points of the SegmentSetting. For array size usage. 
			// Returned
			array<double> ^ XAxis2;						// Generic API returned
			array<double> ^ realRaw2;					// Generic API returned
			array<double> ^ imaginaryRaw2;				// Generic API returned
			array<double, 2> ^ freqPoint2;				// Data storage

			array<double> ^ XAxis3;						// Generic API returned
			array<double> ^ realRaw3;					// Generic API returned
			array<double> ^ imaginaryRaw3;				// Generic API returned
			array<double, 2> ^ freqPoint3;				// Data storage

			array<double> ^ XAxis4;						// Generic API returned
			array<double> ^ realRaw4;					// Generic API returned
			array<double> ^ imaginaryRaw4;				// Generic API returned
			array<double, 2> ^ freqPoint4;				// Data storage

/////////// To store temporary results when doing S3P or S4P
			array<double, 2> ^ S11Real2;
			array<double, 2> ^ S11Imaginary2;
			array<double, 2> ^ S21Real2;
			array<double, 2> ^ S21Imaginary2;
			array<double, 2> ^ S12Real2;
			array<double, 2> ^ S12Imaginary2;
			array<double, 2> ^ S22Real2;
			array<double, 2> ^ S22Imaginary2;

			array<double, 2> ^ S11Rea13;
			array<double, 2> ^ S11Imaginary3;
			array<double, 2> ^ S21Rea13;
			array<double, 2> ^ S21Imaginary3;
			array<double, 2> ^ S12Rea13;
			array<double, 2> ^ S12Imaginary3;
			array<double, 2> ^ S22Rea13;
			array<double, 2> ^ S22Imaginary3;

			array<double, 2> ^ S11Rea23;
			array<double, 2> ^ S11Imaginary4;
			array<double, 2> ^ S21Rea23;
			array<double, 2> ^ S21Imaginary4;
			array<double, 2> ^ S12Rea23;
			array<double, 2> ^ S12Imaginary4;
			array<double, 2> ^ S22Rea23;
			array<double, 2> ^ S22Imaginary4;

			// True when using Single Measurement Different Traces, else No Duplicate Measurement at different trace. 
			bool isSMDT;

			// Store the TestItemName as Key and amb7300tl->segmentSetting[siteIndex].totalPoint as Value. 
			// Condition: Store the current TIName if succesfully instantiated the traceFormatedData related variables. 
			// Reserved for NativeSparameterMeasurement function.
			array<Dictionary<String ^, int>^>^ traceFormatedDataIntStatus;

			// Set True to instanstiate the traceFormatData related variables once only. Set False to disable. 
			// Reserved for ProjectType::SingleTFSiteSingleUUTOffsetWithThread. 
			bool isThreadAlive;

			// Return the raw data array for the ripple limit function. 
			// Data Format: <numeric 1>, <numeric 2>, � <numeric 3N+1> (With "," separator.)
			// Data Sequence: The array size is 1 + 3N, where N is the number of ripple limit bands.
			//					For the n�th point, where n from 1 to N :
			//						<numeric 1>         N total number of the bands
			//						<numeric 3n�1>      n number of the band
			//						<numeric 3n�0>      Ripple value in the n�th band
			//						<numeric 3n + 1>    Ripple limit test result in the n�th band :
			//											0 - Pass
			//											1 - Fail
			array<String ^> ^ rawRippleLimit;

			// Return the sequential raw data array for the ripple limit function. (Without "," separator.)
			array<String ^> ^ rippleLimitValueStr;

			// Return the data array (ripple value in the n�th band), which is the limit line (determine by Ripple Value Band) for the ripple limit function. 
			array<double> ^ rippleLimitValue;

			// [Query Only] Reads out the upper limit value in the n�th point (0 � if there is no limit), which is the limit test result.
			array<double> ^ limitUpperLimit;

			// [Query Only] Reads out the lower limit value in the n�th point (0 � if there is no limit) , which is the limit test result.
			array<double> ^ limitLowerLimit;

			// [Query Only] Return the stimulus value in the n�th point, which is the limit test result.
			array<double> ^ limitStimulusValue;

			// [Query Only] Reads out the the limit test result in the n�th point (�1: No limit, 0:  Fail, 1 : Pass), which is the limit test result.
			array<int> ^ limitResults;

			// [Query Only] Reads out the limit line count configured.
			int limitStimulusCount;

			// [Query Only] Reads out the limit test result. PASS or FAILED, which is the limit test result. Default String::Empty.
			String ^ limitTestResult;
		};
		TraceFormatData ^ traceFormatData;

		ref struct MarkerSearchFormatData
		{
			array<double> ^ XAxis;							// Generic API returned
			array<double> ^ realRaw;						// Generic API returned
			array<double> ^ imaginaryRaw;					// Generic API returned
			array<String^> ^ markerType;					// Generic API returned
			int numberOfMarker;								// Generic API returned

			array<double, 2> ^ msFreq;						// Data storage
			array<double, 2> ^ msReal;						// Data storage
			array<double, 2> ^ msImaginary;					// Data storage
			array<String^, 2> ^ msType;						// Data storage | for ms min max usage

			array<double, 2> ^ msInputPower;						// Data storage
			array<double, 2> ^ msOutputPower;						// Data storage
			array<double, 2> ^ msOutputGain;						// Data storage
		};
		MarkerSearchFormatData ^ msFormatData;

		ref struct MarkerMathFormatData
		{
			// Marker Math Statistics
			array<double> ^ mmStatistics_MeanRaw;			// Generic API returned
			array<double> ^ mmStatistics_StdDevRaw;			// Generic API returned
			array<double> ^ mmStatistics_PeakToPeakRaw;		// Generic API returned
			array<double, 2> ^ mmStatistics_Mean;			// Data storage
			array<double, 2> ^ mmStatistics_StdDev;			// Data storage
			array<double, 2> ^ mmStatistics_PeakToPeak;		// Data storage

			// Marker Math Bandwidth Search
			array<double> ^ mmBwSearch_BWRaw;				// Generic API returned
			array<double> ^ mmBwSearch_CentRaw;				// Generic API returned
			array<double> ^ mmBwSearch_LowRaw;				// Generic API returned
			array<double> ^ mmBwSearch_HighRaw;				// Generic API returned
			array<double> ^ mmBwSearch_QRaw;				// Generic API returned
			array<double> ^ mmBwSearch_LossRaw;				// Generic API returned
			array<double, 2> ^ mmBwSearch_BW;				// Data storage
			array<double, 2> ^ mmBwSearch_Cent;				// Data storage
			array<double, 2> ^ mmBwSearch_Low;				// Data storage
			array<double, 2> ^ mmBwSearch_High;				// Data storage
			array<double, 2> ^ mmBwSearch_Q;				// Data storage
			array<double, 2> ^ mmBwSearch_Loss;				// Data storage

			// Marker Math Flatness
			array<double> ^ mmFlatness_GainRaw;				// Generic API returned
			array<double> ^ mmFlatness_SlopeRaw;			// Generic API returned
			array<double> ^ mmFlatness_FlatRaw;				// Generic API returned
			array<double, 2> ^ mmFlatness_Gain;				// Data storage
			array<double, 2> ^ mmFlatness_Slope;			// Data storage
			array<double, 2> ^ mmFlatness_Flat;				// Data storage
		};
		MarkerMathFormatData ^ mmFormatData;

		ref struct SharedMemoryFormatData
		{
			//store output data
			array<array<float>^>^ formattedData; //store mutiple trace data
			array<array<float>^>^ complexData;

			//Store frequency points for each channels
			array<array<double>^>^ FreqPoints;
			
			//Store the first trace in each window
			array<String^>^ FirstTrace;

			//map the VnaTrace to traceCount according to current active UUT
			array<int>^ VnaTrace_to_TraceCount_Map;

			//use for shared memory BW Search
			array<double, 2>^ BW;
			array<double, 2>^ High;
			array<double, 2>^ Cent;
			array<double, 2>^ Low;
			array<double, 2>^ Q;
			array<double, 2>^ Loss;

			//use for shared memory Target
			array<double>^ XAxis;
			array<double>^ YAxis;
			
			//Post-Proc Data Storage
			array<double,2> ^ Result_MarkX;
			array<double,2> ^ Result_MarkY;
		};
		SharedMemoryFormatData ^ SharedMemFormatData;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Analysis -> Ripple Limit, Limit Test, Fixture Simulation
		**	----------------------------------------------------------------------------------------------------
		*/
		ref struct RippleLimit
		{
			// Ripple limit test is an automatic pass/fail check of the measured trace data. 
			// The trace is checked against the maximum ripple value(ripple limit) defined by the user.
			// The ripple value is the difference between the maximum and minimum response of the trace in the trace frequency band.

			int totalBand;							// Specifies the total number of the band.
			array <double> ^ rippleValueBand;		// Specifies the selected band.
			int rippleValueType;					// Specifies the ripple value type. Ripple value can be represented as an absolute value or as a margin to the limit. Default: RippleValueType::Absolute [0]
			array<double> ^ startStimulus;			// Specifies the start frequency for ripple limit test.  
			array<double> ^ stopStimulus;			// Specifies the stop frequency for the ripple limit test. 
			array<double> ^ rippleLimitValue;		// Specifies the ripple limit (in dB) for the ripple limit test. 

			// Specifies the data array, which is the limit line for the ripple limit function. Note: The array size is 1 + 4N, where N is the number of limit line segments. 
			// <numeric 1>		the number of limit line segments N is the integer from 0 to 12. Setting 0 clears the limit line.
			// <numeric 4n�2>	type of the n�th limit line segment
			//							0: Off.
			// 							1 : On
			// <numeric 4n�1>	the stimulus value in the beginning point of the n�th segment
			// <numeric 4n�0>	the stimulus value in the end point of the n�th segment
			// <numeric 4n + 1>	the ripple limit value of the n�th segment.
			array<String ^ > ^ rippleLimitList;
		};
		RippleLimit ^ rippleLimit;

		ref struct LimitTest
		{
			// The limit test is a function of automatic pass/fail judgment for the trace of the measurement result.
			// The judgment is based on the comparison of the trace to the limit line set by the user. 
			// The limit line can consist of one or several segments. Each segment checks the measurement value for failing whether upper or lower limit.
			// The limit line segment is defined by specifying the coordinates of the beginning (X0, Y0) and the end(X1, Y1) of the segment, and type of the limit.

			int totalLimitLine;							// Specifies the total number of the limit lines.
			bool enLimitLine;							// Set True to enable Limit Line display on the specified trace, else False. Default set as True. 
			bool enMarkerResponseOffset;				// Set True to used the active marker position as the response offset. Note: Active marker can be configurable in real time with "activeMarkerResponse" variable in LimitTest struc.
			int activeMarkerResponse;					// Specifies the active marker when enable "enMarkerResponseOff" variable. Default set as Marker1. 
			array <int> ^ limitLineType;				// Specifies the limit line type. Support type: Min, Max, Single or Off. Note: The MAX or MIN limit types check if the trace falls outside of the upper or lower limit respectively.
			array<double> ^ limitLineBeginStimulus;		// Specifies the start frequency (Begin stimulus) for limit test. Unit in Hz. Note: Stimulus value in the beginning point of the segment.
			array<double> ^ limitLineEndStimulus;		// Specifies the stop frequency (End stimulus) for limit test. Unit in Hz. Note: Stimulus value in the ending point of the segment.
			array<double> ^ limitLineBeginResponse;		// Specifies the begin response (unit in dB) for the limit test. Note: Response value in the beginning point of the segment.
			array<double> ^ limitLineEndResponse;		// Specifies the end response (unit in dB) for the limit test. Note: Response value in the ending point of the segment.
			double limitLineStimulusOffset;				// Specifies the stimulus offset to define the limit line offset along X-axis. Default set with 0 Hz | s | dBm.
			double limitLineResponseOffset;				// Specifies the response offset to define the limit line offset along Y-axis. Default set with 0 dB |� |s.
			// Return the data array, which is the limit test result.
			// The array size is 4N, where N is the number of measurement points.
			// For the n�th point, where n from 1 to N :
			//		<numeric 4n�3>        the stimulus value in the n�th point;
			//		<numeric 4n�2>        the limit test result in the n�th point;
			//								�1: No limit	
			//								0 : Fail
			//								1 : Pass
			//		<numeric 4n�1>         the upper limit value in the n�th point(0 � if there is no limit)
			//		<numeric 4n�0>         the lower limit value in the n�th point(0 � if there is no limit)
			array<String ^ > ^ LimitDataArray;
		};
		LimitTest ^ limitTest;

		ref struct FixtureSimulation
		{
		};
		FixtureSimulation ^ fixtureSimulation;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Calibration -> Port Extension
		**	----------------------------------------------------------------------------------------------------
		*/
		ref struct PortExtensionConfig
		{
		};
		PortExtensionConfig ^ portExtensionConfig;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Save / Recall -> SaveDataToTouchstoneFile
		**	----------------------------------------------------------------------------------------------------
		*/
		ref struct SaveRecallSettings
		{
			// Flag
			bool EnableSaveSnpData;					// Set TRUE to enable save trace data as snp file.
			bool EnableSaveAsCsvFormat;				// Set TRUE to enable save trace data as csv file with the same contents as .snp file.
			bool EnableSaveTraceData;				// Set TRUE to enable save the particular vna channel's trace data in csv file. 
			bool MultipleS2PGeneration;				// [Future Enchancement] Reserved for snp file merging process (CMT S3P / S4P).
				
			// General VNA info
			String ^ Model;
			String ^ SerialNumber;
			String ^ FWVersion;

			// File info
			String ^ touchstoneFileSnpType;			// Touchstone file type. Eg: xxx.s2p / xxx.s3p.
			String ^ touchstoneFilePath;			// Touchstone file target directory to be saved.
			String ^ touchstoneFolder;				// Touchstone file target directory
			array<int> ^ touchstoneFilePortNumber;	// Touchstone file header port number.
			int touchstoneFileSeparator;			// Touchstone file data separator symbol when the S�parameters are saved. 
			int touchstoneFileDataFormat;			// Touchstone file data format. Eg: RI/ MA/ DB. Preset in RI format.
			int touchstoneFileDomainFormat;			// Time Domain touchstone file format is similar to frequency domain touchstone except the x-axis unit is time (s, ns) instead of frequency (Hz, MHz).
			String ^ restoreRippleLimitFile;		// Specifies the ripple limit file directory to open the table with a *.rlm file extension. 
			String ^ restoreLimitTableFile;			// Specifies the limit file directory to open the table with a *.lim file extension. 
			int activeTrace;						// Specifies the active trace of the vna channel. 
			String ^ touchstoneFileName;			// Touchstone file naming (special format for skysemi | default naming)

			// [Special Request from HuaYing] Reserved for SaveDataToTouchstoneFile function
			int tpIndex;
			int totalTpCountForCurrentVnaFetch;
			String ^ currentVnaFetchSubItemName;
		};
		SaveRecallSettings ^ saveRecallSetting;

		// Structure to hold configuration information
		ref struct ConfigInfo
		{
			String^ name;
			int systemConfig;
			int configType;
			array<String^>^ validParams;
		};

#pragma endregion

#pragma region "AMB7300.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	AMB7300 System Initialize & Uninitialize
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeTester(Site ^ site);
		int UninitializeTester(Site ^ site);
		int PreProcessingTester(Site ^ site);
		int PostProcessingTester(Site ^ site);
		int VNA_Lock(Site^ site);
		int VNA_Unlock(Site^ site);
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	AMB7300 VNA Global Object & Variable
		**	----------------------------------------------------------------------------------------------------
		*/
		void InitializeVnaGlobalResultVariable(int tfSite);
	
		/*
		**	----------------------------------------------------------------------------------------------------
		**	AMB7300 System Configuration File
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetSystemConfigurationFileInfo(int tfSite);
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	AMB7300 Project State File & Mapping File
		**	----------------------------------------------------------------------------------------------------
		*/		
		int InitializeProjectStateFileMappingFile(int tfSite);
		int GetProjectMappingInfo(int tfSite);
		void ModifiedLoadedMappingToInteger(GroupMapping ^ % groupMapping, bool % isFormatCorrect);
		void AssignMappingVariable(array<Aemulus::Configuration::Group> ^ portXmlMapping, array<int> ^ % mapPort1Arr, array<int> ^ % mapPort2Arr, array<int> ^ % mapVnaChannelArr, array<String^> ^ % mapVnaChannelStringArr);
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	techFlow siteIndex to vnaSiteIndex mapping
		**	----------------------------------------------------------------------------------------------------
		*/
		int Map_siteIndex_To_vnaSiteIndex(int tfSite);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Operation (Copper Mountain | Keysight)
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeVna(int tfSite, int siteIndex);
		int UninitializeVna(int tfSite, int siteIndex);
		int HighPwrTest_VNASourceLow(int tfSite, int siteIndex);

		int VnaConfig(int tfSite, int vnaSiteIndex);
		int VnaFetch(int tfSite, int vnaSiteIndex);
		int VnaDataAnalysis(int tfSite, int vnaSiteIndex, double % result);
		int SaveToTouchstoneFile(Site ^ site, int tfSite, int vnaSiteIndex);
		int VnaFetch_TrueParallel(int tfSite, int vnaSiteIndex);
		int VnaDataAnalysis_TrueParallel(int tfSite, int vnaSiteIndex, double % result);

		int VnaSwTime(int tfSite, int vnaSiteIndex, double % result);
		double CalculateSwTime_WithMA(array<double>^ dbmArray, int rise_h_fall_l, double sampleRate, double digitizerTriggerDelay, double %powerInit, double %powerSettle);
		double CalculateSwTime_NoMA(array<double>^ dbmArray, int rise_h_fall_l, double sampleRate, double digitizerTriggerDelay, double %powerInit, double %powerSettle);

		// Log Detector & Digitizer (for switching time)
		int InitializeExtModule(int tfSite, int siteIndex);
		int UninitializeExtModule(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Vna Operation Helper Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetCurrentVnaFetchChannel(int tfSite, int vnaSiteIndex);
		double GetMeasurementData(int tfSite, int vnaSiteIndex, int i, String^ measType, String^ outputFormat);
		double ProcessVnaMeasurement(int tfSite,  int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Math Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int MathFunction(int tfSite, int siteIndex, double % result);
		/*
		**	----------------------------------------------------------------------------------------------------
		**	CM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeCmSeriesResource(int tfSite, int siteIndex);
		int UninitializeCmSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	AM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeAmSeriesResource(int tfSite, int siteIndex);
		int UninitializeAmSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeDmSeriesResource(int tfSite, int siteIndex);
		int UninitializeDmSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	IOM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeIomSeriesResource(int tfSite, int siteIndex);
		int UninitializeIomSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	ACM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeAcmSeriesResource(int tfSite, int siteIndex);
		int UninitializeAcmSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	TM400 Series Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeTmSeriesResource(int tfSite, int siteIndex);
		int UninitializeTmSeriesResource(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DC Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int DcControl(int tfSite, int siteIndex);
		int DcTest(int tfSite, int siteIndex, double % result);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Digital/Pattern Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int PatternControl(int tfSite, int siteIndex);
		int PatternTest(int tfSite, int siteIndex, int % result, String^ % resultMessage);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	DM Operation
		**	----------------------------------------------------------------------------------------------------
		*/
		int DmControl(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Other Utilities
		**	----------------------------------------------------------------------------------------------------
		*/
		int IdentifyTestParameterPhaseType(Site ^ site, int tfSite, int siteIndex, String ^ % currentPhase);
		void ShowMessageBox(int tfSite, int siteIndex, String ^ messageBoxTitle, String ^ conditionName);
		void ShowMessageBox(int tfSite, int siteIndex, String ^ messageBoxTitle, String ^ conditionName, String ^ additionalMessage);

#pragma endregion

#pragma region "CopperMountainVnaUtility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Initialize & Uninitialize Vna Hardware
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeVna_CMT(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, int vnaSiteIndex, bool offline);
		int UninitializeVna_CMT(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : MPEX Port Pair Switching
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureMpexPortPairSwitching_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Set Port Matching Circuit
		**	----------------------------------------------------------------------------------------------------
		*/
		int AMB7300TestLibrary::SetPortMatchingCircuit_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Segment Settings [Stimulus->Segment]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureSegmentSetting_CMT(int tfSite, int vnaSiteIndex);
		int GetChannelandTraceSetting_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Trigger Settings [Stimulus->Trigger]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureTriggerSource_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureTriggerExternal_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureTriggerOutput_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureTriggerScope_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureSingleTriggerForGetFromStateFileType_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureActiveChannelAndSingleTrigger_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Trace Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureTraces_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Vna Data Analysis Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetTraceFormatData_CMT(int tfSite, int vnaSiteIndex);
		int GetMarkerSearchFormatData_CMT(int tfSite, int vnaSiteIndex);
		int GetMarkerMathFormatData_CMT(int tfSite, int vnaSiteIndex);
		int GetMarkerSearchMinMaxRipple_CMT(int tfSite, int vnaSiteIndex);
		int GetMeanFromRange_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Averaging & Smoothing Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureAveraging_CMT(int tfSite, int vnaSiteIndex);
		int ConfigureSmoothing_CMT(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Channel Source Low Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int SourceLowVna_CMT(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Save To Touchstone File
		**	----------------------------------------------------------------------------------------------------
		*/
		int SaveTraceDataToTouchstoneFile_CMT(int tfSite, int vnaSiteIndex);

		int GetTraceFormatData_CMT_Sds(int tfSite, int vnaSiteIndex);

#pragma endregion

#pragma region "KeysightVnaUtility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Initialize & Uninitialize Vna Hardware
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeVna_Keysight(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, int vnaSiteIndexAlias, bool offline);
		int UninitializeVna_Keysight(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Configure Channel Coupling
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureChannelCoupling_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : MPEX Port Pair Switching
		**	----------------------------------------------------------------------------------------------------
		*/
		// Keysight VNA + A5
		int ConfigureMpexPortPairSwitching_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Set Port Matching Circuit
		**	----------------------------------------------------------------------------------------------------
		*/
		int AMB7300TestLibrary::SetPortMatchingCircuit_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Segment Settings [Stimulus->Segment]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureSegmentSetting_Keysight(int tfSite, int vnaSiteIndex);
		int GetChannelandTraceSetting_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Power Sweep Settings [Sweep->Sweep Type->Power Sweep]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigurePowerSweepSetting_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : CW Time Sweep Settings [Sweep->Sweep Type->CW Time Sweep]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureCWTimeSweepSetting_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Trigger Settings [Stimulus->Trigger]
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureTriggerSource_Keysight(int tfSite, int vnaSiteIndex);
		// [Future Enchancement] API not support TriggerExternal function.
		// [Future Enchancement] API not support TriggerOutput function.
		int ConfigureTriggerScope_Keysight(int tfSite, int vnaSiteIndex);
		int ConfigureSingleTriggerForGetFromStateFileType_Keysight(int tfSite, int vnaSiteIndex);
		int ConfigureActiveChannelAndSingleTrigger_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Trace Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureTraces_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Vna Data Analysis Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetTraceFormatData_Keysight(int tfSite, int vnaSiteIndex);
		int GetMarkerSearchFormatData_Keysight(int tfSite, int vnaSiteIndex);
		int GetMarkerMathFormatData_Keysight(int tfSite, int vnaSiteIndex);
		int GetMarkerSearchMinMaxRipple_Keysight(int tfSite, int vnaSiteIndex);
		int GetMarkerCompressionSearchData_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Averaging & Smoothing Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int ConfigureAveraging_Keysight(int tfSite, int vnaSiteIndex);
		int ConfigureSmoothing_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Channel Source Low Function
		**	----------------------------------------------------------------------------------------------------
		*/
		int SourceLowVna_Keysight(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : Save To Touchstone File
		**	----------------------------------------------------------------------------------------------------
		*/
		int SaveTraceDataToTouchstoneFile_Keysight(int tfSite, int vnaSiteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs : SharedMemoryTransfer
		**	----------------------------------------------------------------------------------------------------
		*/
		int SharedMemoryTransfer_Initialise(int tfSite, int vnaSiteIndex);
		int SharedMemoryTransfer_FetchData(int tfSite, int vnaSiteIndex);
		int SharedMemoryTransfer_DataAnalysis(int tfSite, int vnaSiteIndex);
		int SharedMemoryTransfer_MinMaxRipple(int tfSite, int vnaSiteIndex);
		int SharedMemoryTransfer_FetchData_TrueParallel(int tfSite, int vnaSiteIndex);
		int SharedMemoryTransfer_DataAnalysis_TrueParallel(int tfSite, int testSiteIndex);
		int SharedMemoryTransfer_MinMaxRipple_TrueParallel(int tfSite, int testSiteIndex);

#pragma endregion

#pragma region "CM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: CM Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeCm(String ^ hardwareProfile, int testHead, int tfSite, int vnaSiteIndex, bool offline);
		int UninitializeCm(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: CM Test Function
		**	----------------------------------------------------------------------------------------------------
		*/

#pragma endregion

#pragma region "AM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: SMU Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeSmu(String ^ hardwareProfile, int testHead, int tfSite, int vnaSiteIndex, bool offline);
		int UninitializeSmu(int tfSite, int siteIndex);
		int ConfigurePinSmu(int tfSite, int siteIndex, String ^ pinAlias, String ^ controlFunction, double nplc, double voltageLevel, double currentLevel, double delay);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: SMU Test Function (DcTest)
		**	----------------------------------------------------------------------------------------------------
		*/
		int SmuTestFunction_OS(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);
		int SmuTestFunction_MeasureCurrent(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);
		int SmuTestFunction_MeasureVoltage(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);

#pragma endregion

#pragma region "DM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: DM Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeDm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline);
		int UninitializeDm(int tfSite, int siteIndex);
		int ConfigurePinDm(int tfSite, int siteIndex, String ^ pinAlias, String ^ controlFunction, double nplc, double voltageLevel, double currentLevel, double delay);
		int ConfigureDmVectorEngine_Mipi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, double mipiFreq, double inputChannelDelay, bool isEnableTrigger, bool isEnableContinuous, int triggerOutput0, int triggerOutput1);
		int ConfigureDmVectorEngine_Spi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias);
		int ConfigureDmVectorEngine_I2c(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias);
		int DmConfigurePinToPeMode(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ dmPinAlias, bool isEnableInputTermination, bool isEnableHv, bool isEnableActiveLoad, bool isEnableDiffComparator, bool isEnableTrigger, bool isEnableContinuous, double vih, double vil, double voh, double vol, double ioh, double iol, double vch, double vcl, double vth);
		int DmConfigurePinToPmuMode(int tfSite, int siteIndex, String ^ dmPinAlias, String ^ forceMode, bool isEnableInputTermination, bool isEnableHv, bool isEnableActiveLoad, bool isEnableDiffComparator, double currentLevel, double voltageLevel, double nplc);
		int DmConfigurePinToDioMode(int tfSite, int siteIndex, String ^ dmPinAlias);
		int DmConfigureDmOutputTrigger(int tfSite, int siteIndex, String ^ dmModuleAlias, int triggerOutput0, int triggerOutput1);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: DM Test Function (DcTest)
		**	----------------------------------------------------------------------------------------------------
		*/
		int DmTestFunction_OS(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);
		int DmTestFunction_MeasureCurrent(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);
		int DmTestFunction_MeasureVoltage(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result);
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: DM Test Function (PatternTest)
		**	----------------------------------------------------------------------------------------------------
		*/
		int DmPatternTestFunction_MipiWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName);
		int DmPatternTestFunction_MipiRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName, bool isMultiVecToOneResult, bool isOneVecToMultiResult);
		int DmPatternTestFunction_SpiWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName);
		int DmPatternTestFunction_SpiRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName);
		int DmPatternTestFunction_I2cWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName);
		int DmPatternTestFunction_I2cRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName);
		
		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: DM Helper Function - General
		**	----------------------------------------------------------------------------------------------------
		*/
		int DmLoadVectorFile(int tfSite, int siteIndex, String ^ dmModuleAlias);
		int DmVectorFileReadBack_Mipi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, String ^ targetVecFileName, bool isReadHistoryRam, bool isReadFailCount, array<int> ^ % readHistoryRam, int % vecFailCount, int % vecFirstFail);
		int DmVectorFileReadBack_Spi(int tfSite, int siteIndex);
		int DmVectorFileReadBack_I2c(int tfSite, int siteIndex);
		int DmRunVector(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ targetVecFileName);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: DM Helper Function - Special For Mipi Test Library
		**	----------------------------------------------------------------------------------------------------
		*/
		int GetDpinGroupInfo(array<String^> ^ mipiSetInfo, int % dpinGroup, String^ % errorMessage);
		int GetSclkChannelInfo(array<String^> ^ mipiSetInfo, int dpinGroup, int % sclkChannel, String^ % errorMessage);
		int GetSdataChannelInfo(array<String^> ^ mipiSetInfo, int dpinGroup, int sclkChannel, int % sdataChannel, String^ % errorMessage);
		int GetVecTimingSetInfo(array<String^> ^ mipiSetInfo, int % vecTimingSet, String^ % errorMessage);
		int CheckMipiSetIdentifier(array<String^> ^ mipiSetInfo, String^ % errorMessage);
		int GetModeList(array<String^> ^ mipiSetInfo, String^ % mode, String^ % errorMessage);
		int GetUSIDList(array<String^> ^ mipiSetInfo, int % usid, String^ % errorMessage);
		int GetRegAddrList(array<String^> ^ mipiSetInfo, String^ mode, int % regAddr, String^ % errorMessage);
		int GetRegDataList(array<String^> ^ mipiSetInfo, String^ mode, int % regData, String^ % errorMessage);
		int GetOperationList(array<String^> ^ mipiSetInfo, String^ mode, String^ % operation, String^ % errorMessage);
		int GetSpeedList(array<String^> ^ mipiSetInfo, String^ mode, String^ % speed, String^ % errorMessage);

#pragma endregion

#pragma region "IOM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: IOM Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeIom(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline);
		int UninitializeIom(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: IOM Test Function
		**	----------------------------------------------------------------------------------------------------
		*/

#pragma endregion

#pragma region "ACM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: ACM Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeAcm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline);
		int UninitializeAcm(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: ACM Test Function
		**	----------------------------------------------------------------------------------------------------
		*/

#pragma endregion

#pragma region "TM400Utility.cpp"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: TM Settings
		**	----------------------------------------------------------------------------------------------------
		*/
		int InitializeTm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline);
		int UninitializeTm(int tfSite, int siteIndex);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	APIs: TM Test Function
		**	----------------------------------------------------------------------------------------------------
		*/

#pragma endregion

#pragma region "Get PRODUCTIONS STATUS"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get ProductionSystem Status if its running
		**	----------------------------------------------------------------------------------------------------
		*/
		bool IsRunningProduction(Site ^ site);
#pragma endregion

	};
}


/*----------------------------------------------------------------------
* Revision Log
* $Log: AMB7300.h.rca$

*** Version	: v1.0.0.5
*** Date	: 2 September 2026
*** PIC		: Xuli 
* Add Channel_Sparam_TraceNo_Info & Trace_Format_List

*** Version	: v1.0.0.4
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Add Map_siteIndex_To_vnaSiteIndex

*** Version	: v1.0.0.3
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Add include Windows.h and WinUser.h
* Added ConfigureChannelCoupling_Keysight()
* Added SharedMemoryTransfer_GetFreqPoints()

*** Version	: v1.0.0.2
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Support SharedMemoryTransfer method
* Support TrueParallel Multi UUT project using SharedMemoryTransfer method

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
* Support AMB7300 platform configuration and testing functions.
* Support project UUT:
  - Single Site Single UUT
  - Single Site Multi UUT
  - True Parallel Single UUT
* Support AEM module:
  - AM
  - DM
* Support VNA:
  - CMT SC5090
  - Keysight M9804A
* Support platform execution phase:
  - DcControl
  - DcTest
  - PatternControl
  - PatternTest
  - VnaConfig
  - VnaFetch
  - VnaDataAnalysis
----------------------------------------------------------------------*/