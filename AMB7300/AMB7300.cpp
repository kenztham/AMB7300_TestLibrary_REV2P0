/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			AMB7300.cpp
Purpose:		Contain AMB7300 platform utility functions
Version:		v1.0.0.6
----------------------------------------------------------------------*/

#define _USE_MATH_DEFINES
#include "AMB7300.h"
#include <math.h>

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	AMB7300TestLibrary Contsructor and Destructor 
	**	----------------------------------------------------------------------------------------------------
	*/
	AMB7300TestLibrary::AMB7300TestLibrary(TestFunction ^ TestFunc)
	{
		tl = TestFunc;

	}
	AMB7300TestLibrary::~AMB7300TestLibrary(void)
	{
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	AMB7300 System Initialize & Uninitialize
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeTester(Site ^ site)
	{
		/*****************************************************************************************************
		** InitializeTester
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function is to initialize AMB7300 system hardware.
		******************************************************************************************************/

		// Local variable
		int ret		= 0;
		int tfSite	= tl->glob->tf.TestSite;

		//Util->InitTimer(1);
		//timerFilename = "TimingFile_7300_ORI_" + System::DateTime::Now.ToString("yyyyMMddHHmmss");

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Execute 'Load' phase. Initialize tester hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Execute 'Load' phase. Initialize tester hardware.");

#pragma region "Init AMB7300 VNA Global Object & Variable"

		InitializeVnaGlobalResultVariable(tfSite);

#pragma endregion

#pragma region "Get AMB7300 System Configuration File"
			
		ret = GetSystemConfigurationFileInfo(tfSite);
		if (ret != 0) goto EndOfTest;

		// Get system configuration name from AMB7300Config.xml
		if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A0ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A0";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A1ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A1";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A2ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A2";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A3ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A3";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A4ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A4";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300A5ConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300A5";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300S2P1DConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300S2P1D";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300S4P1DConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300S4P1D";
		}
		else if (sysConfigInfo.configurationType->Contains(sysConfigInfo.amb7300S6P1DConfigName))
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300S6P1D";
		}
		else if (sysConfigInfo.configurationType == sysConfigInfo.amb7300S6P22NConfigName)
		{
			amb7300SystemSetting->amb7300ConfigName = "AMB7300S6P22N";
		}
		else
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGFILE_CONTENT_INVALID;
			tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester] Invalid content inside the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester] Invalid content inside the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

#pragma endregion

#pragma region "Init Project State File & Mapping File"

		// Initialize project state file and mapping file in local 'ProgramData' folder
		ret = InitializeProjectStateFileMappingFile(tfSite);
		if (ret != 0) goto EndOfTest;

		// Assign project state file name based on test site
		vnaSetting[0].StateFile = projectStateInfo.localStateFileName;
		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Project state file to be initialized: " + vnaSetting[0].StateFile + projectStateInfo.stateFileType);
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Project state file to be initialized: " + vnaSetting[0].StateFile + projectStateInfo.stateFileType);

		// Get project mapping file info
		ret = GetProjectMappingInfo(tfSite);
		if (ret != 0) goto EndOfTest;

		// Checking project mapping file site to site
		if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
		{
			// [Future Enchancement] To compare the mapping info site to site. Each site should have the same info.
		}

#pragma endregion

#pragma region "Check VNA Process"

		String^ executablePath;
		System::Diagnostics::Process ^ process = gcnew System::Diagnostics::Process();
		array <System::Diagnostics::Process^>^ currentActiveProcess = gcnew array <System::Diagnostics::Process^>(1);
		
		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			executablePath = "C:\\VNA\\S2VNA\\S2VNA.exe";
			currentActiveProcess = Process::GetProcessesByName("S2VNA");
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			executablePath = "C:\\Program Files\\Keysight\\Network Analyzer\\835x.exe";
			currentActiveProcess = Process::GetProcessesByName("835x");
		}

		//Execute the VNA SFP
		if (currentActiveProcess->Length == 0)
		{
			process->StartInfo->UseShellExecute = false;
			process->StartInfo->FileName = executablePath;
			//process->StartInfo->Arguments = arguments;
			process->StartInfo->RedirectStandardOutput = true;
			process->Start();
			Util->WaitSecond(20 S);
		}

#pragma endregion		

#pragma region "Minimize VNA SFP"
		// Minimize the process using ShowWindow
		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			currentActiveProcess = Process::GetProcessesByName("S2VNA");
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			currentActiveProcess = Process::GetProcessesByName("835x");
		}
		if (currentActiveProcess->Length > 0)
		{
			Process^ vnaProcess = currentActiveProcess[0];
			IntPtr windowHandle = vnaProcess->MainWindowHandle;

			if (windowHandle != IntPtr::Zero)
			{
				// Bring the window to the foreground
				//Make sure to set Project Properties > Linker>Input > Additional Dependencies > win32.lib to run this API
				ShowWindow(static_cast<HWND>(windowHandle.ToPointer()), SW_MINIMIZE);
			}
		}
#pragma endregion

#pragma region "Init Vna Hardware"

		// VNA hardware status
		AMB7300_VnaHardwareStatus = gcnew array<bool>(tl->glob->tf.NumberOfTestSites);
		for (int i = 0; i < tl->glob->tf.NumberOfTestSites; i++)
		{
			AMB7300_VnaHardwareStatus[i] = false;
		}
		
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			//ret = InitializeVna(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;

			ret =  InitializeExtModule(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;

		}

#pragma endregion

#pragma region "Init CM400e Series Hardware"
			
		//// [Future Enchancement] Implement CM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_CM->Length != 0)
		//{
		//	cm = gcnew array<CM ^>(tl->glob->tf.NumberOfTestSites);
		//	
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = InitializeCmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Init AM400e Series Hardware"

		if (tl->glob->RsrcManager.hardwareStatus_AM->Length != 0)
		{
			smu = gcnew array<SMU ^>(tl->glob->tf.NumberOfTestSites);
			
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				//ret = InitializeAmSeriesResource(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Init DM400e Series Hardware"

		if (tl->glob->RsrcManager.hardwareStatus_DM->Length != 0)
		{
			dm = gcnew array<DM ^>(tl->glob->tf.NumberOfTestSites);
		
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				//ret = InitializeDmSeriesResource(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Init IOM400e Series Hardware"

		// [Future Enchancement] Implement IOM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_IOM->Length != 0)
		//{
		//	iom = gcnew array<DIO ^>(tl->glob->tf.NumberOfTestSites);
		//	
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = InitializeIomSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Init ACM400e Series Hardware"

		//// [Future Enchancement] Implement ACM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_ACM->Length != 0)
		//{
		//	acm = gcnew array<ACM ^>(tl->glob->tf.NumberOfTestSites);
		//	
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = InitializeAcmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Init TM400e Series Hardware"

		//// [Future Enchancement] Implement TM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_TM->Length != 0)
		//{
		//	tm = gcnew array<TM ^>(tl->glob->tf.NumberOfTestSites);
		//
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = InitializeTmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Other Operation / Settings That Execute Once"

#pragma region "Acquire Mutex"
		//Release mutex if it's locked accidentally
		bool createdNew;
		mutex = gcnew Mutex(false, "Global\\VNAAccessMutex", createdNew);


		try
		{
			//Try to wait for the mutex (non-blocking with a 0ms timeout)
			if (mutex->WaitOne(0, false))
			{
				//If acquired, immediately release to "reset" it for the new insantance
				mutex->ReleaseMutex();
			}

		}
		catch (AbandonedMutexException^)
		{
			//If the mutex was abandoned by a previous crashed instance
			mutex->ReleaseMutex();
		}
#pragma endregion

#pragma region "Set SNP file data format "

		saveRecallSetting->touchstoneFileDataFormat = tl->glob->AWV.touchstoneFileDataFormat;

#pragma endregion

#pragma region "Assign all the available channels in the mapping file to the vna_chn array"

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			for (int i = 0; i < 16; i++)
			{
				vnaSetting[siteIndex].vna_chn[i] = projectMappingInfo.mapVnaChannelArr[i];
			}
		}

#pragma endregion

#pragma region "Get vna system count"
		amb7300SystemSetting->vnaSystemCount = 0;

		if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
		{
			amb7300SystemSetting->vnaSystemCount = 1;
		}
		else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) ||
			(tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)) ||
			(tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA)))
		{
			// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
			for each (String ^ Rsrc in tl->glob->RsrcManager.RsrcMngr[0]->MapNames)
			{
				Resource ^ hardwareResource = tl->glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(AMB7300_TYPE) == true))
				{
					if (((hardwareResource->Alias->StartsWith(AMB7300_S2P1_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S2P4_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S2P8_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S4P2_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S4P1_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S2P16_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S2P1D_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S4P1D_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S6P1D_TYPE) == true) ||
						(hardwareResource->Alias->StartsWith(AMB7300_S6P22N_TYPE) == true)) &&
						((hardwareResource->Type->StartsWith(AMB7300_S2P1_TYPE)) ||
						(hardwareResource->Type->StartsWith(AMB7300_S2P4_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S2P8_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S4P2_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S4P1_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S2P16_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S2P1D_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S4P1D_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S6P1D_TYPE)) ||
							(hardwareResource->Type->StartsWith(AMB7300_S6P22N_TYPE))))
					{
						amb7300SystemSetting->vnaSystemCount++;
					}
				}
			}
		}
#pragma endregion

#pragma region "Map techFlow siteIndex to vnaSiteIndex"

		ret = Map_siteIndex_To_vnaSiteIndex(tfSite);
		if (ret != 0) goto EndOfTest;

#pragma endregion

		int siteIndex = 0;
//
//#pragma region "Get Current Channel and Trace Info"
//
//		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
//		{
//			ret = GetChannelandTraceSetting_CMT(tfSite, siteIndex);
//			if (ret != 0) goto EndOfTest;
//		}
//		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
//		{
//			
//			ret = GetChannelandTraceSetting_Keysight(tfSite, siteIndex);
//			if (ret != 0) goto EndOfTest;
//
//			//SharedMemoryTransfer
//			if(tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
//			{
//				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureChannelCoupling_Keysight] . ");
//				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureChannelCoupling_Keysight] . ");
//				
//				ret = ConfigureChannelCoupling_Keysight(tfSite, siteIndex);
//				if (ret != 0) goto EndOfTest;
//			}
//		}
//
//#pragma endregion
//
//#pragma region "Set Port Matching circuit to Port"
//		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
//		{
//			ret = SetPortMatchingCircuit_CMT(tfSite, siteIndex);
//			if (ret != 0) goto EndOfTest;
//		}
//		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
//		{
//			ret = SetPortMatchingCircuit_Keysight(tfSite, siteIndex);
//			if (ret != 0) goto EndOfTest;
//		}
//		
//#pragma endregion
//
		// Spare for future use
#pragma region "Get total num of Channel from TestRecipe > VnaFetch Conditions"
		//int TestRecipeUsedChannel_Count = 0;
		//int Casted_ChannelNum;
		//int RecipeMeasurePortPair_Count = 0;
		//String ^ measurePortPair = String::Empty;
		//PortPair_By_Channel = gcnew Dictionary<String^, int>();

		//TestItem^ ti;
		//AFlowSubItem^ subFlowItem;

		//array<String^> ^ arrStr = gcnew array<String^>(0);
		//array<String^> ^ separator = gcnew array<String^>(1);
		//separator[0] = ",";

		//for each(AFlowItem^ fi in site->FlowItems)
		//{
		//	if (fi->GetType() == TestItem::typeid)
		//	{
		//		ti = (TestItem^)fi;
		//		if (!ti->Bypass)
		//		{
		//			for each(subFlowItem  in ti->SubItems)
		//			{
		//				{
		//					if (dynamic_cast<ControlStep^>(subFlowItem->Data) != nullptr)
		//					{
		//						AFlowStep^ flowStep = (AFlowStep^)ti->FlowSteps[subFlowItem->Name];
		//						if (!flowStep->Bypass)
		//						{
		//							if (subFlowItem->Name->Contains(PHASE_CONST_VNA_FETCH))
		//							{
		//								measurePortPair = (String^)ti->FlowSteps[subFlowItem->Name]->Conditions[VnaFetchConditionName_MeasurePortPair][site]->Value;

		//								arrStr = measurePortPair->Split(separator, StringSplitOptions::None);

		//								if (!ti->FlowSteps[subFlowItem->Name]->Conditions->ContainsKey(VnaConfigConditionName_ChannelNum))
		//								{
		//									array<int>^ vnaChannel = gcnew array<int>(1);
		//									array<bool>^ isReverseRequire = gcnew array<bool>(1);
		//									int portCount = arrStr->Length;
		//									array<String^>^ pinAlias = gcnew array<String^>(portCount);
		//									for (int i = 0; i < portCount; i++)
		//									{
		//										pinAlias[i] = arrStr[i];
		//									}
		//									isReverseRequire[0] = false;

		//									amb7300_driver[0]->GetVNAChannel(pinAlias, vnaChannel, isReverseRequire);
		//									if (vnaChannel->Length > 0)
		//									{
		//										if (PortPair_By_Channel->ContainsKey(measurePortPair + "_Ch" + vnaChannel[0].ToString())) // dont count for repeated measurePortPair
		//										{
		//										}
		//										else
		//										{
		//											PortPair_By_Channel->Add(measurePortPair + "_Ch" + vnaChannel[0].ToString(), vnaChannel[0]); // ( "P1,P2_Ch1" , 1 )
		//											TestRecipeUsedChannel_Count++;
		//										}
		//									}
		//								}
		//								else
		//								{
		//									Casted_ChannelNum = (int)ti->FlowSteps[subFlowItem->Name]->Conditions[VnaConfigConditionName_ChannelNum][site]->Value;

		//									if (PortPair_By_Channel->ContainsKey(measurePortPair + "_Ch" + Casted_ChannelNum.ToString()))
		//									{
		//									}
		//									else
		//									{
		//										PortPair_By_Channel->Add(measurePortPair + "_Ch" + Casted_ChannelNum.ToString(), Casted_ChannelNum); // ( "P1,P2_Ch1" , 1 )
		//										TestRecipeUsedChannel_Count++;
		//									}
		//								}
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
#pragma endregion

#pragma region "Compare State File Active Channel number with Mapping File Channel number" 

		/*if ( (projectStateInfo.Channels->Length != projectMappingInfo.portXmlMapping->Length)
			&& (projectStateInfo.Channels->Length != TestRecipeUsedChannel_Count * amb7300SystemSetting->vnaSystemCount * tl->glob->tf.NumberOfTestSites))
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_STATEMAPPINGFILE_CHANNEL_NOT_MATCH;
			String ^ additionalMessage = "Number of Active Channel in State file does not match Mapping(.xml) file." + "\n" +
				"Number of Active Channel in State file: " + projectStateInfo.Channels->Length + "\n" +
				"Number of  Channel in Mapping(.xml) file: " + projectMappingInfo.portXmlMapping->Length;

			MessageBox::Show(additionalMessage, TITLE_CONST_CHANNEL_NUMBER_NOT_MATCH, MessageBoxButtons::OK, MessageBoxIcon::Warning);
			tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeTester] Number of active channel in State file does not match Mapping(.xml) file!");
			tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeTester] Number of active channel in State file does not match Mapping(.xml) file!");
			goto EndOfTest;
		}*/

#pragma endregion


#pragma endregion

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Execute 'Load' phase. Initialize tester hardware succeed !!!");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester] Execute 'Load' phase. Initialize tester hardware succeed !!!");

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeTester(Site ^ site)
	{
		/*****************************************************************************************************
		** UninitializeTester
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to uninitialize AMB7300 system hardware.
		******************************************************************************************************/

		// Local variable
		int ret		= 0;
		int tfSite	= tl->glob->tf.TestSite;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester] Executing 'Unload' phase. Uninitialize tester hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester] Executing 'Unload' phase. Uninitialize tester hardware.");

#pragma region "Uninit Vna Hardware"

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			ret = UninitializeVna(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}

#pragma endregion

#pragma region "Uninit CM400e Series Hardware"

		//// [Future Enchancement] Implement CM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_CM->Length != 0)
		//{
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = UninitializeCmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Uninit AM400e Series Hardware"

		if (tl->glob->RsrcManager.hardwareStatus_AM->Length != 0)
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = UninitializeAmSeriesResource(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Uninit DM400e Series Hardware"

		if (tl->glob->RsrcManager.hardwareStatus_DM->Length != 0)
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = UninitializeDmSeriesResource(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Uninit IOM400e Series Hardware"

		//// [Future Enchancement] Implement IOM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_IOM->Length != 0)
		//{
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = UninitializeIomSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Uninit ACM400e Series Hardware"

		//// [Future Enchancement] Implement ACM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_ACM->Length != 0)
		//{
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = UninitializeAcmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

#pragma region "Uninit TM400e Series Hardware"

		//// [Future Enchancement] Implement TM module function.
		//if (tl->glob->RsrcManager.hardwareStatus_TM->Length != 0)
		//{
		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		//	{
		//		ret = UninitializeTmSeriesResource(tfSite, siteIndex);
		//		if (ret != 0) goto EndOfTest;
		//	}
		//}

#pragma endregion

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester] Executing 'Unload' phase. Uninitialize tester hardware succeed !!!");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester] Executing 'Unload' phase. Uninitialize tester hardware succeed !!!");

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::PreProcessingTester(Site ^ site)
	{
		/*****************************************************************************************************
		** UninitializeTester
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to operate PostProcessing actions on AMB7300 system hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = tl->glob->tf.TestSite;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PreProcessing -> PreProcessingTester] Executing 'PreProcessing' Control Item.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PreProcessing -> PreProcessingTester] Executing 'PreProcessing' Control Item.");

#pragma region "Mutex -> VNA_Lock()"
		if (tl->glob->AWV.VNA_Mutex_EN == true)
		{
			ret = VNA_Lock(site);
			if (ret != 0) goto EndOfTest;
		}
#pragma endregion

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PreProcessing -> PreProcessingTester] 'PreProcessing' Control Item done !!!");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PreProcessing -> PreProcessingTester] 'PreProcessing' Control Item done !!!");

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::PostProcessingTester(Site ^ site)
	{
		/*****************************************************************************************************
		** UninitializeTester
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to operate PostProcessing actions on AMB7300 system hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = tl->glob->tf.TestSite;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PostProcessing -> PostProcessingTester] Executing 'PostProcessing' Control Item.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PostProcessing -> PostProcessingTester] Executing 'PostProcessing' Control Item.");

#pragma region "Mutex -> VNA_Unlock()"
		if (tl->glob->AWV.VNA_Mutex_EN == true)
		{
			ret = VNA_Unlock(site);
			if (ret != 0) goto EndOfTest;
		}
#pragma endregion

#pragma region "Source Low Vna Hardware"
		if (tl->glob->AWV.HighPwrTest_EN == true)
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = HighPwrTest_VNASourceLow(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

//#pragma region "Source Low CM400e Series Hardware"
//
//		//// [Future Enchancement] Implement CM module function.
//		//if (tl->glob->RsrcManager.hardwareStatus_CM->Length != 0)
//		//{
//		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//		//	{
//		//		ret = UninitializeCmSeriesResource(tfSite, siteIndex);
//		//		if (ret != 0) goto EndOfTest;
//		//	}
//		//}
//
//#pragma endregion
//
//#pragma region "Source Low AM400e Series Hardware"
//
//		if (tl->glob->RsrcManager.hardwareStatus_AM->Length != 0)
//		{
//			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//			{
//				ret = UninitializeAmSeriesResource(tfSite, siteIndex);
//				if (ret != 0) goto EndOfTest;
//			}
//		}
//
//#pragma endregion
//
//#pragma region "Source Low DM400e Series Hardware"
//
//		if (tl->glob->RsrcManager.hardwareStatus_DM->Length != 0)
//		{
//			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//			{
//				ret = UninitializeDmSeriesResource(tfSite, siteIndex);
//				if (ret != 0) goto EndOfTest;
//			}
//		}
//
//#pragma endregion
//
//#pragma region "Source Low IOM400e Series Hardware"
//
//		//// [Future Enchancement] Implement IOM module function.
//		//if (tl->glob->RsrcManager.hardwareStatus_IOM->Length != 0)
//		//{
//		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//		//	{
//		//		ret = UninitializeIomSeriesResource(tfSite, siteIndex);
//		//		if (ret != 0) goto EndOfTest;
//		//	}
//		//}
//
//#pragma endregion
//
//#pragma region "Source Low ACM400e Series Hardware"
//
//		//// [Future Enchancement] Implement ACM module function.
//		//if (tl->glob->RsrcManager.hardwareStatus_ACM->Length != 0)
//		//{
//		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//		//	{
//		//		ret = UninitializeAcmSeriesResource(tfSite, siteIndex);
//		//		if (ret != 0) goto EndOfTest;
//		//	}
//		//}
//
//#pragma endregion
//
//#pragma region "Source Low TM400e Series Hardware"
//
//		//// [Future Enchancement] Implement TM module function.
//		//if (tl->glob->RsrcManager.hardwareStatus_TM->Length != 0)
//		//{
//		//	for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
//		//	{
//		//		ret = UninitializeTmSeriesResource(tfSite, siteIndex);
//		//		if (ret != 0) goto EndOfTest;
//		//	}
//		//}
//
//#pragma endregion

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PostProcessing -> PostProcessingTester] 'PostProcessing' Control Item done !!!");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[PostProcessing -> PostProcessingTester] 'PostProcessing' Control Item done !!!");

	EndOfTest:
		return ret;
	}

	int AMB7300TestLibrary::VNA_Lock(Site ^ site)
	{
		/*****************************************************************************************************
		** PostProcessing
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to to lock VNA resource from being used by other testsite during index parallel testing.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//Mutex
		try
		{
			//Wait for Hardware available with 10s of timeout, will occupy the mutex once available
			mutex->WaitOne(3000);
		}
		catch (Exception^ ex)
		{
			mutex->ReleaseMutex();
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::VNA_Unlock(Site ^ site)
	{
		/*****************************************************************************************************
		** PostProcessing
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to unlock VNA resource for being used by other testsite during index parallel testing.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//Release the hardware lock
		mutex->ReleaseMutex();

	EndOfTest:
		return ret;
	}


	/*
	**	----------------------------------------------------------------------------------------------------
	**	AMB7300 VNA Global Object & Variable
	**	----------------------------------------------------------------------------------------------------
	*/
	void AMB7300TestLibrary::InitializeVnaGlobalResultVariable(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeVnaGlobalResultVariable
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to initialize all the global variables 
		**		that are related to the VNA settings and result variables.
		******************************************************************************************************/

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeVnaGlobalResultVariable] Initialize VNA global object & result variable.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeVnaGlobalResultVariable] Initialize VNA global object & result variable.");

#pragma region "Initialize object"

		amb7300_				= gcnew array<Aemulus::Hardware::AMB7300 ^>(tl->glob->tf.NumberOfTestSites);
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			amb7300_[siteIndex]	= gcnew Aemulus::Hardware::AMB7300();
		}

		amb7300_driver			= gcnew array<AMB7300_Driver ^>(tl->glob->tf.NumberOfTestSites);
		amb7300_NA				= gcnew array<NetworkAnalyzer ^>(tl->glob->tf.NumberOfTestSites);

		siteIndex_By_vnaSiteIndex = gcnew array<int>(tl->glob->tf.NumberOfTestSites);

		// -------------------- AMB7300 system info --------------------
		amb7300SystemSetting	= gcnew AMB7300SystemSetting();
	
		// -------------------- Vna settings --------------------
		vnaSetting				= gcnew array<VNASettting>(tl->glob->tf.NumberOfTestSites);
		segmentSetting			= gcnew array<SParamSegment>(tl->glob->tf.NumberOfTestSites);
		prev_segmentSetting		= gcnew array<prev_SParamSegment>(tl->glob->tf.NumberOfTestSites);
		averageSetting			= gcnew array<AverageSettting>(tl->glob->tf.NumberOfTestSites);
		msSetting				= gcnew array<MarkerSearchSetting>(tl->glob->tf.NumberOfTestSites);
		mmSetting				= gcnew array<MarkerMathSetting>(tl->glob->tf.NumberOfTestSites);
		smtSetting				= gcnew array<SharedMemoryTransferSetting>(tl->glob->tf.NumberOfTestSites);
		
		// -------------------- Results data --------------------
		traceFormatData			= gcnew TraceFormatData();
		msFormatData			= gcnew MarkerSearchFormatData();
		mmFormatData			= gcnew MarkerMathFormatData();
		SharedMemFormatData		= gcnew SharedMemoryFormatData();

		// -------------------- Vna Analysis --------------------
		limitTest				= gcnew LimitTest();
		rippleLimit				= gcnew RippleLimit();
		// [Future Enchancement] To include fixture simulation function.
		fixtureSimulation		= gcnew FixtureSimulation();

		// -------------------- Vna Calibration --------------------
		// [Future Enchancement] To include port extension function.
		portExtensionConfig		= gcnew PortExtensionConfig();

		// -------------------- Vna Save / Recall --------------------
		saveRecallSetting		= gcnew SaveRecallSettings();
		
#pragma endregion

#pragma region "Initialize vnaSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			// Vna channel & averaging channel
			vnaSetting[siteIndex].vna_chn						= gcnew array<int>(16);
			vnaSetting[siteIndex].vna_chn_ave					= gcnew array<int>(16);
			for (int i = 0; i < vnaSetting[siteIndex].vna_chn->Length; i++)
			{
				vnaSetting[siteIndex].vna_chn[i]				= Vna_Channel_Reset;			// maximize the channel to 16x, will define all channel by referring to the project's mapping file info
				vnaSetting[siteIndex].vna_chn_ave[i]			= Vna_Channel_Reset;			// will define when user choose to enable averaging function on that specific channel
			}

			// Vna trace & smoothing trace
			vnaSetting[siteIndex].vna_trace						= gcnew array<int>(16);
			vnaSetting[siteIndex].vna_trace_smo					= gcnew array<int>(16);
			for (int i = 0; i < vnaSetting[siteIndex].vna_trace->Length; i++)
			{
				vnaSetting[siteIndex].vna_trace[i]				= i + 1;						// maximize the trace to 16x
				vnaSetting[siteIndex].vna_trace_smo[i]			= Vna_Trace_Reset;				// will define when user choose to enable smoothing function on that specific trace
			}

			// Vna trace measurement parameter
			vnaSetting[siteIndex].measType						= gcnew array<String^>(16);
			for (int i = 0; i < vnaSetting[siteIndex].measType->Length; i++)
			{
				vnaSetting[siteIndex].measType[i]				= Vna_Measurement_Type_Reset;	// will define when user select the measurement parameter on the specific trace
			}

			// Vna trace measurement format
			vnaSetting[siteIndex].vna_traceFormat				= gcnew array<int>(16);
			for (int i = 0; i < vnaSetting[siteIndex].vna_traceFormat->Length; i++)
			{
				vnaSetting[siteIndex].vna_traceFormat[i]		= Vna_Format_Reset;				// will define when user select the measurement format on the specific trace
			}

			// Vna state file
			vnaSetting[siteIndex].StateFile						= String::Empty;				// will get from project's state file

			// Vna pin alias
			vnaSetting[siteIndex].pinAlias						= gcnew array<String^>(2);
			for (int i = 0; i < vnaSetting[siteIndex].pinAlias->Length; i++)
			{
				vnaSetting[siteIndex].pinAlias[i]				= String::Empty;				// will define when user select the port pair to executed [setting in 'VnaFetch']
			}

			// For port pair assignment usage
			vnaSetting[siteIndex].isReverseRequire				= gcnew array<bool>(16);		// will return from wrapper api

			// User target trace count
			vnaSetting[siteIndex].userTraceCount				= 4;							// User target trace to be displayed on the specific channel [setting in 'VnaFetch']

			// Current active channel that reserved for the user input pin alias (port pair)
			vnaSetting[siteIndex].currentVnaFetchChannel		= Vna_Channel_Reset;
			vnaSetting[siteIndex].currentVnaFetchChannelAve		= Vna_Channel_Reset;
		}

#pragma endregion

#pragma region "Initialize segmentSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			// Trace measurement format
			segmentSetting[siteIndex].sparaFormat				= SParam_FormatVNA::LogMagnitude;	// will define when user select the measurement format on the specific trace

			// Segment setting
			segmentSetting[siteIndex].segmentCount				= 0;								// will define when user specifies the segment settings
			segmentSetting[siteIndex].sweepType					= String::Empty;					// will define when user specifies the segment settings
			segmentSetting[siteIndex].totalPoint				= 0;								// will define when user specifies the segment settings

			// Power sweep
			segmentSetting[siteIndex].startPow					= 0.0;								// will define when user specifies the power sweep settings
			segmentSetting[siteIndex].stopPow					= 0.0;								// will define when user specifies the power sweep settings
			segmentSetting[siteIndex].powLevel					= 0.0;								// will define when user specifies the power sweep settings

			// Measurement method for previous test library, will not be used in this latest test library, reserved for future usage
			segmentSetting[siteIndex].EnableFetchMarker			= false;  
			segmentSetting[siteIndex].EnableFetchFormatedData	= false;
			segmentSetting[siteIndex].EnableMeasSpara			= false;
			segmentSetting[siteIndex].GetFromStateFile			= false;
			segmentSetting[siteIndex].fetchS3PFormatedData		= false;
			segmentSetting[siteIndex].fetchS3PMarkerData		= false;
		}

#pragma endregion

#pragma region "Initialize prev_segmentSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			// Trace measurement format
			prev_segmentSetting[siteIndex].sparaFormat = SParam_FormatVNA::LogMagnitude;	// will define when user select the measurement format on the specific trace

																					// Segment setting
			prev_segmentSetting[siteIndex].segmentCount = 0;								// will define when user specifies the segment settings
			prev_segmentSetting[siteIndex].sweepType = String::Empty;					// will define when user specifies the segment settings
			prev_segmentSetting[siteIndex].totalPoint = 0;								// will define when user specifies the segment settings

																					// Power sweep
			prev_segmentSetting[siteIndex].startPow = 0.0;								// will define when user specifies the power sweep settings
			prev_segmentSetting[siteIndex].stopPow = 0.0;								// will define when user specifies the power sweep settings
			prev_segmentSetting[siteIndex].powLevel = 0.0;								// will define when user specifies the power sweep settings

																					// Measurement method for previous test library, will not be used in this latest test library, reserved for future usage
			prev_segmentSetting[siteIndex].EnableFetchMarker = false;
			prev_segmentSetting[siteIndex].EnableFetchFormatedData = false;
			prev_segmentSetting[siteIndex].EnableMeasSpara = false;
			prev_segmentSetting[siteIndex].GetFromStateFile = false;
			prev_segmentSetting[siteIndex].fetchS3PFormatedData = false;
			prev_segmentSetting[siteIndex].fetchS3PMarkerData = false;
		}

#pragma endregion

#pragma region "Initialize aveSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			// Function enable / disable
			averageSetting[siteIndex].enableActiveTraceTransform	= false;					// will define when user specifies the function usage
			averageSetting[siteIndex].enableAveCorrection			= false;					// will define when user specifies the function usage
			averageSetting[siteIndex].enableSmoCorrection			= false;					// will define when user specifies the function usage

			averageSetting[siteIndex].vnaCHwSmo						= gcnew array<int>(16);
			averageSetting[siteIndex].vnaCHTracewSmo				= gcnew array<int>(16);
			averageSetting[siteIndex].vnaCHwAvgFactor				= gcnew array<int>(16);
			averageSetting[siteIndex].vnaCHTracewAvgFactor			= gcnew array<int>(16);
			averageSetting[siteIndex].SmoStartIndex					= gcnew array<int>(16);
			averageSetting[siteIndex].SmoStopIndex					= gcnew array<int>(16);
			averageSetting[siteIndex].SmoMeasTypes					= gcnew array<String^>(16);
			for (int i = 0; i < 15; i++)
			{
				averageSetting[siteIndex].SmoSegmentCount			= 1;
				averageSetting[siteIndex].vnaCHwSmo[i]				= int(VNAChannel::RESERVED);
				averageSetting[siteIndex].vnaCHTracewSmo[i]			= int(VNAChannel::RESERVED);
				averageSetting[siteIndex].vnaCHwAvgFactor[i]		= int(VNAChannel::RESERVED);
				averageSetting[siteIndex].vnaCHTracewAvgFactor[i]	= int(VNAChannel::RESERVED);
				averageSetting[siteIndex].SmoStartIndex[i]			= 0;
				averageSetting[siteIndex].SmoStopIndex[i]			= 0;
				averageSetting[siteIndex].SmoMeasTypes[i]			= Vna_Measurement_Type_Reset;
			}
		}

#pragma endregion

#pragma region "Initialize msSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
		}

#pragma endregion

#pragma region "Initialize mmSetting"

		// Reset all variables / settings to default value
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
		}

#pragma endregion

#pragma region "Initialize data"

		// Support the Sparameter measurement up to S4P (for ProjectType::SingleTFSiteSingleUUTOffsetWithThread)
		dataType.iSparameterData = gcnew array<array<array<Sparameter_Data^>^>^>(4);

#pragma endregion

#pragma region "Initialize traceFormatData"

		traceFormatData->traceFormatedDataIntStatus = gcnew array <Dictionary <String ^, int> ^>(tl->glob->tf.NumberOfTestSites);
		for (int i = 0; i < traceFormatData->traceFormatedDataIntStatus->Length; i++)
		{
			traceFormatData->traceFormatedDataIntStatus[i] = gcnew Dictionary<String^, int>();
		}

#pragma endregion

#pragma region "Initialize rippleLimit"

		//// Reserved for Ripple Limit Test 
		//rippleLimit->rippleValueType		= int(RippleValueType::Absolute);
		//rippleLimit->rippleValueBand		= gcnew array<double>(15) { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

#pragma endregion

#pragma region "Initialize limitTest"

		////Reserved for Limit Test 
		//limitTest->activeMarkerResponse		= int(VNAMARKER::MARKER1);
		//limitTest->enMarkerResponseOffset	= false;
		//limitTest->enLimitLine				= true;

#pragma endregion

#pragma region "Initialize saveRecallSetting"

		// Flag
		saveRecallSetting->EnableSaveSnpData				= false;
		saveRecallSetting->EnableSaveAsCsvFormat			= false;
		saveRecallSetting->EnableSaveTraceData				= false;
		saveRecallSetting->MultipleS2PGeneration			= false;

		// General VNA info
		saveRecallSetting->Model							= String::Empty;
		saveRecallSetting->SerialNumber						= String::Empty;
		saveRecallSetting->FWVersion						= String::Empty;

		// File info
		saveRecallSetting->touchstoneFileSnpType			= String::Empty;
		saveRecallSetting->touchstoneFilePath				= String::Empty;
		saveRecallSetting->touchstoneFilePortNumber			= gcnew array<int>(2);
		saveRecallSetting->touchstoneFileSeparator			= int(SnPSeparator::TAB);			// Use int(SnPSeparator::TAB) to have better *snp to *csv conversion
		saveRecallSetting->touchstoneFileDataFormat			= int(SnPFormat::SNP_DB_FORMAT);
		saveRecallSetting->touchstoneFileDomainFormat		= int(TouchstoneFileFormat::Frequency_Domain);
		saveRecallSetting->restoreRippleLimitFile			= String::Empty;
		saveRecallSetting->restoreLimitTableFile			= String::Empty;
		saveRecallSetting->activeTrace						= Vna_Trace_Reset;

		// [Special Request from HuaYing] Reserved for SaveDataToTouchstoneFile function
		saveRecallSetting->tpIndex							= 0;
		saveRecallSetting->totalTpCountForCurrentVnaFetch	= 0;
		saveRecallSetting->currentVnaFetchSubItemName		= String::Empty;

#pragma endregion

	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	AMB7300 System Configuration File
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::GetSystemConfigurationFileInfo(int tfSite)
	{
		/*****************************************************************************************************
		** GetSystemConfigurationFileInfo
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to get information from the AMB7300 configuration xml file.
		******************************************************************************************************/

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Load AMB7300 system configuration file '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml'. Target directory: C:\\ProgramData\\Aemulus\\AMB7300\\" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + "x.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Load AMB7300 system configuration file '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml'. Target directory: C:\\ProgramData\\Aemulus\\AMB7300\\" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + "x.");

		// Local variable
		int ret			= 0;
		XmlLib ^ xmlLib = gcnew XmlLib();

#pragma region "Get AMB7300 system configuration file directory at local 'ProgramData' folder"

		// Single or Parallel Site Project
		if (tl->glob->tf.StageCount == 1)
		{
			// C:\ProgramData\Aemulus\AMB7300\Config"x"
			if (tfSite == 0)
				sysConfigInfo.localConfigFolderDirectory = FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0;
			else
				sysConfigInfo.localConfigFolderDirectory = FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGX + tfSite.ToString();

			// C:\ProgramData\Aemulus\AMB7300\Config"x"\AMB7300Configuration.xml
			sysConfigInfo.localConfigFileDirectory = sysConfigInfo.localConfigFolderDirectory + "\\" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml";
		}
		else
		{	// [Future Enchancement] reserved for index parallel project.
		}

#pragma endregion

#pragma region "Check AMB7300 system configuration file folder directory existence at local 'ProgramData' folder"

		if (!Directory::Exists(sysConfigInfo.localConfigFolderDirectory))
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGFILEFOLDER_NOT_FOUND;
			if (tfSite == 0)
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			else
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			ret = 0;
			// Create 'Configx' folder
			Directory::CreateDirectory(sysConfigInfo.localConfigFolderDirectory);
		}

#pragma endregion

#pragma region "Check AMB7300 system configuration file directory existence at local 'ProgramData' folder"

		if (!File::Exists(sysConfigInfo.localConfigFileDirectory))
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGFILE_NOT_FOUND;
			if (tfSite == 0)
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file does not exist in the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file does not exist in the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIG0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			else
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file does not exist in the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file does not exist in the '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			goto EndOfTest;
		}

#pragma endregion

#pragma region "Read AMB7300Configuration.xml file"

		try
		{
			// Read AMB7300Configuration.xml file
			xmlLib->Read(sysConfigInfo.localConfigFileDirectory, sysConfigInfo.systemConfiguration);
			// Get info
			sysConfigInfo.moduleConfiguration = sysConfigInfo.systemConfiguration->ModuleConfigurations->ToArray();
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGFILE_READ_FAIL;
			tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Fail to read '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Fail to read '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

#pragma endregion

#pragma region "Get info from AMB7300Configuration.xml file"

		sysConfigInfo.moduleConfigurationAddress	= String::Empty;
		sysConfigInfo.moduleConfigurationChassis	= String::Empty;
		sysConfigInfo.moduleConfigurationName		= String::Empty;
		sysConfigInfo.moduleConfigurationSlot		= String::Empty;
		sysConfigInfo.moduleConfigurationSN			= String::Empty;
		sysConfigInfo.moduleConfigurationType		= String::Empty;
		sysConfigInfo.configurationType				= String::Empty;

		try
		{ 
			if (sysConfigInfo.systemConfiguration != nullptr)
			{ 
				// Get configuration file info
				sysConfigInfo.moduleConfigurationAddress	= sysConfigInfo.moduleConfiguration[0]->Address;	// eg: TCPIP0::localhost::5025::SOCKET
				sysConfigInfo.moduleConfigurationChassis	= sysConfigInfo.moduleConfiguration[0]->Chassis;	// eg: -1
				sysConfigInfo.moduleConfigurationName		= sysConfigInfo.moduleConfiguration[0]->Name;		// eg: S5090
				sysConfigInfo.moduleConfigurationSlot		= sysConfigInfo.moduleConfiguration[0]->Slot;		// eg: -1
				sysConfigInfo.moduleConfigurationSN			= sysConfigInfo.moduleConfiguration[0]->SN;			// eg: 21019246
				sysConfigInfo.moduleConfigurationType		= sysConfigInfo.moduleConfiguration[0]->Type;		// eg: VNA_1
				// Get configuration type
				sysConfigInfo.configurationType				= sysConfigInfo.systemConfiguration->type;			// eg: AMB7300S2P1 / AMB7300S2P4 / AMB7300S4P1 / ...
			
				//Post Process Configuration Type (To Remove unwanted characters for the test Library)
				array<String^> ^ arrStr2 = gcnew array<String^>(0);
				array<String^> ^ separator2 = gcnew array<String^>(1);
				separator2[0] = "=";
				arrStr2 = sysConfigInfo.systemConfiguration->type->Split(separator2, StringSplitOptions::None);
				sysConfigInfo.configurationType = arrStr2[0];

				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> AMB7300ConfigurationFile] AMB7300 VNA system configuration info Site" + tfSite.ToString()	+ ": "		+ "\n" +
																																"\t" + "config address: "			+ sysConfigInfo.moduleConfigurationAddress				+ "\n" +
																																"\t" + "config chassis: "			+ sysConfigInfo.moduleConfigurationChassis				+ "\n" +
																																"\t" + "config name: "				+ sysConfigInfo.moduleConfigurationName					+ "\n" +
																																"\t" + "config slot: "				+ sysConfigInfo.moduleConfigurationSlot					+ "\n" +
																																"\t" + "config serial number: "		+ sysConfigInfo.moduleConfigurationSN					+ "\n" +
																																"\t" + "config type: "				+ sysConfigInfo.moduleConfigurationType					+ "\n" +
																																"\t" + "system config: "			+ sysConfigInfo.configurationType);
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> AMB7300ConfigurationFile] AMB7300 VNA system configuration info Site" + tfSite.ToString()		+ ": "		+ "\n" +
																																"\t" + "config address: "			+ sysConfigInfo.moduleConfigurationAddress				+ "\n" +
																																"\t" + "config chassis: "			+ sysConfigInfo.moduleConfigurationChassis				+ "\n" +
																																"\t" + "config name: "				+ sysConfigInfo.moduleConfigurationName					+ "\n" +
																																"\t" + "config slot: "				+ sysConfigInfo.moduleConfigurationSlot					+ "\n" +
																																"\t" + "config serial number: "		+ sysConfigInfo.moduleConfigurationSN					+ "\n" +
																																"\t" + "config type: "				+ sysConfigInfo.moduleConfigurationType					+ "\n" +
																																"\t" + "system config: "			+ sysConfigInfo.configurationType);
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGFILE_CONTENT_INVALID;
			tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Invalid content inside '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetSystemConfigurationFileInfo] Invalid content inside '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_CONFIGXFOLDER_CONFIG_XML + ".xml' file. Target directory: " + sysConfigInfo.localConfigFileDirectory + "." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

#pragma endregion

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	AMB7300 Project State File & Mapping File
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeProjectStateFileMappingFile(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeProjectStateFileMappingFile
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function is to check the existence of the project state file and mapping file
		**		in the local ProgramData folder.
		**		If the state file or mapping file do not exist in the local folder, 
		**		copy both the state file and mapping file template from the project recipes folder 
		**		'DeviceStateFileTemplate' to the local ProgramData folder.
		******************************************************************************************************/

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Load project state file (.sta) and mapping file (.xml). Target directory: C:\\ProgramData\\Aemulus\\AMB7300\\" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + "x.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Load project state file (.sta) and mapping file (.xml). Target directory: C:\\ProgramData\\Aemulus\\AMB7300\\" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + "x.");

		// Local variable
		int ret					= 0;

#pragma region "Identify state file type [ CMT -> .sta | KEYSIGHT -> .csa ]"

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			projectStateInfo.stateFileType = ".sta";
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			projectStateInfo.stateFileType = ".csa";
		else
			projectStateInfo.stateFileType = "";

#pragma endregion

#pragma region "Get project state file and mapping file directory at local 'ProgramData' folder"

		// Single or Parallel Site Project
		if (tl->glob->tf.StageCount == 1)
		{
			// C:\ProgramData\Aemulus\AMB7300\State"x"
			if (tfSite == 0)
				projectStateInfo.localStateFileFolderDirectory = FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0;
			else
				projectStateInfo.localStateFileFolderDirectory = FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString();

			if (tl->glob->AWV.GenericStateMappingFile_EN == 0)
			{
				// AMB7300A"x"_"DeviceName" 
				projectStateInfo.localStateFileName = amb7300SystemSetting->amb7300ConfigName + "_" + tl->glob->tf.DeviceName;
				// AMB7300A"x"_"DeviceName"Mapping 
				projectMappingInfo.localMappingFileName = amb7300SystemSetting->amb7300ConfigName + "_" + tl->glob->tf.DeviceName + "Mapping";
			}
			else
			{
				// GenericStateFileName 
				projectStateInfo.localStateFileName = tl->glob->AWV.GenericStateMappingFile_Name;
				// AMB7300A"x"_"DeviceName"Mapping 
				projectMappingInfo.localMappingFileName = tl->glob->AWV.GenericStateMappingFile_Name + "Mapping";
			}

			// C:\ProgramData\Aemulus\AMB7300\State"x"\localStateFileName.sta (.csa)
			projectStateInfo.localStateFileDirectory = projectStateInfo.localStateFileFolderDirectory + "\\" + projectStateInfo.localStateFileName + projectStateInfo.stateFileType;
				
			
			// C:\ProgramData\Aemulus\AMB7300\Statex\localMappingFileNameMapping.xml
			projectMappingInfo.localMappingFileDirectory = projectStateInfo.localStateFileFolderDirectory + "\\" + projectMappingInfo.localMappingFileName + ".xml";
		}
		else
		{	// [Future Enchancement] reserved for index parallel project.
		}

#pragma endregion

#pragma region "Check project state file folder directory existence at local 'ProgramData' folder"

		if (!Directory::Exists(projectStateInfo.localStateFileFolderDirectory))
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_STATEFILEFOLDER_NOT_FOUND;
			if (tfSite == 0)
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			else
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			ret = 0;
			// Create 'Statex' folder
			Directory::CreateDirectory(projectStateInfo.localStateFileFolderDirectory);
		}

#pragma endregion

#pragma region "Check project state file and mapping file directory existence at local 'ProgramData' folder"

		projectStateInfo.isLocalProjectStateFileNotFound = false;
		if (!File::Exists(projectStateInfo.localStateFileDirectory))
		{
			projectStateInfo.isLocalProjectStateFileNotFound = true;
		}

#pragma endregion

#pragma region "Copy project state file and mapping file from project 'TestRecipes' to local 'ProgramData' if NOT FOUND in local 'ProgramData'"

			if (projectStateInfo.isLocalProjectStateFileNotFound == true)
			{
				// Use generic state file
				if ((tl->glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid == true) || (tl->glob->DeviceStateFileTemplate.isDeviceStateFileTemplateMappingFileInvalid == true))
				{
					// Replace state file with generic
					String ^ genericStateFileName					= amb7300SystemSetting->amb7300ConfigName;
					projectStateInfo.localStateFileDirectory		= projectStateInfo.localStateFileDirectory->Replace(projectStateInfo.localStateFileName, genericStateFileName);
					projectStateInfo.localStateFileName				= genericStateFileName;
					// Replace mapping file wit generic
					String ^ genericMappingFileName					= amb7300SystemSetting->amb7300ConfigName + "Mapping";
					projectMappingInfo.localMappingFileDirectory	= projectMappingInfo.localMappingFileDirectory->Replace(projectMappingInfo.localMappingFileName, genericMappingFileName);
					projectMappingInfo.localMappingFileName			= genericStateFileName + "Mapping";

					if (tfSite == 0)
					{ 
						MessageBox::Show(	projectStateInfo.localStateFileName + projectStateInfo.stateFileType + " & " +
											projectStateInfo.localStateFileName + "Mapping.xml"	+ " does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder."	+ "\n\n"	+
											"The generic AMB7300 state file and mapping file will be loaded."																																												+ "\n"		+
											"User may create the project state file and mapping file through the AMB7300 DCS, rename the files, and then continue to the calibration process."																								+ "\n\n"	+
											"Please UNLOAD the test program and conduct AMB7300 DCS calibration by using this local project state file before any debug activities or run production activities."																			+ "\n\n"	+ 
											"Please copy the calibrated project state file and mapping file from the local 'ProgramData\\State' folder to the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder.", "Local Project State File & Mapping File Not Found");
					
						tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. The generic state file and mapping file has been loaded.");
						tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. The generic state file and mapping file has been loaded.");
					}
					else
					{ 
						MessageBox::Show(	projectStateInfo.localStateFileName + projectStateInfo.stateFileType + " & " +
											projectStateInfo.localStateFileName + "Mapping.xml" + " does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder."	+ "\n\n"	+
											"The generic AMB7300 state file and mapping file will be loaded."																																																	+ "\n"		+
											"User may create the project state file and mapping file through the AMB7300 DCS, rename the files, and then continue to the calibration process."																													+ "\n\n"	+
											"Please UNLOAD the test program and conduct AMB7300 DCS calibration by using this local project state file before any debug activities or run production activities."																								+ "\n\n"	+ 
											"Please copy the calibrated project state file and mapping file from the local 'ProgramData\\State" + tfSite.ToString() + "' folder to the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder.", "Local Project State File & Mapping File Not Found");
					
						tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. The generic state file and mapping file has been loaded.");
						tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in both local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + "and project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. The generic state file and mapping file has been loaded.");
					}
				}
				// Copy from project 'TestRecipes\\DeviceStateFileTemplate'
				else if ((tl->glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid == false) && (tl->glob->DeviceStateFileTemplate.isDeviceStateFileTemplateMappingFileInvalid == false))
				{
					// Copy state file
					if (projectStateInfo.stateFileType == ".sta")
					{
						File::Copy(tl->glob->DeviceStateFileTemplate.stateFileDirectory_sta, projectStateInfo.localStateFileDirectory, true);
					}
					else
					{
						File::Copy(tl->glob->DeviceStateFileTemplate.stateFileDirectory_csa, projectStateInfo.localStateFileDirectory, true);
					}
					// Copy mapping file
					File::Copy(tl->glob->DeviceStateFileTemplate.mappingFileDirectory, projectMappingInfo.localMappingFileDirectory, true);

					if (tfSite == 0)
					{ 
						MessageBox::Show(	projectStateInfo.localStateFileName + projectStateInfo.stateFileType + " & " +
											projectStateInfo.localStateFileName + "Mapping.xml"	+ " does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0									+ "\n\n"	+ 
											"Both the project state file and mapping file has been copied from the project 'TestRecipes\\DeviceStateFileTemplate' folder to the local 'ProgramData\\State' folder."		+ "\n\n"	+
											"Please UNLOAD the test program and conduct AMB7300 DCS calibration by using this local project state file before any debug activities or run production activities.", "Local Project State File & Mapping File Not Found");
					
						tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + ". Copy project state file and mapping file from the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder to the local " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + " folder.");
						tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + ". Copy project state file and mapping file from the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder to the local " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATE0 + " folder.");
					}
					else
					{ 
						MessageBox::Show(	projectStateInfo.localStateFileName + projectStateInfo.stateFileType + " & " +
											projectStateInfo.localStateFileName + "Mapping.xml"	+ " does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString()										+ "\n\n"	+ 
											"Both the project state file and mapping file has been copied from the project 'TestRecipes\\DeviceStateFileTemplate' folder to the local 'ProgramData\\State" + tfSite.ToString() + "' folder."	+ "\n\n"	+
											"Please UNLOAD the test program and conduct AMB7300 DCS calibration by using this local project state file before any debug activities or run production activities.", "Local Project State File & Mapping File Not Found");
					
						tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + ". Copy project state file and mapping file from the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder to the local " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + " folder.");
						tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Project state file and mapping file does not exist in local directory : " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + ". Copy project state file and mapping file from the project 'TestRecipes\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder to the local " + FOLDER_CONST_PROGRAMDATA_AEM_AMB7300_STATEX + tfSite.ToString() + " folder.");
					}
				}
				// Undefined error
				else
				{
					ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_STATEMAPPINGFILE_INIT_FAIL;
					tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Fail to initialize project state file and mapping file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> InitializeProjectStateFileMappingFile] Fail to initialize project state file and mapping file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}

#pragma endregion

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetProjectMappingInfo(int tfSite)
	{
		/*****************************************************************************************************
		** GetProjectMappingInfo
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function is to get the project mapping file info.
		******************************************************************************************************/

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> GetProjectMappingInfo] Get mapping info from project mapping file (.xml).");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> GetProjectMappingInfo] Get mapping info from project mapping file (.xml).");

		// Local varaible
		int ret											= 0;
		bool isFormatCorrect							= true;
		XmlLib ^ xmlLib									= gcnew XmlLib();
		// max channel is 16x | one port pair = one channel
		projectMappingInfo.mapPort1Arr					= gcnew array<int>(16);
		projectMappingInfo.mapPort2Arr					= gcnew array<int>(16);
		projectMappingInfo.mapVnaChannelArr				= gcnew array<int>(16);
		projectMappingInfo.mapVnaChannelStringArr		= gcnew array<String^>(16);

		for (int vnaChIndex = 0; vnaChIndex < 16; vnaChIndex++)
		{
			projectMappingInfo.mapPort1Arr[vnaChIndex]				= MappingFile_PortPair_Reset;
			projectMappingInfo.mapPort2Arr[vnaChIndex]				= MappingFile_PortPair_Reset;
			projectMappingInfo.mapVnaChannelArr[vnaChIndex]			= MappingFile_Vna_Channel_Reset;
			projectMappingInfo.mapVnaChannelStringArr[vnaChIndex]	= MappingFile_Vna_ChannelStr_Reset;
		}

		// Get port1, port2, and channel info from each available tF test site (Config, Config1, Config2... | State, State1, State2...)
		try
		{
			// Read project's mapping xml file
			xmlLib->Read(projectMappingInfo.localMappingFileDirectory, projectMappingInfo.groupMapping);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_MAPPINGFILE_NOT_FOUND;
			if (tfSite == 0)
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Project mapping file (.xml) does not exist in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Project mapping file (.xml) does not exist in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			}
			else
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Project mapping file (.xml) does not exist in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Project mapping file (.xml) does not exist in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			}
			goto EndOfTest;
		}

		// Assign
		ModifiedLoadedMappingToInteger(projectMappingInfo.groupMapping, isFormatCorrect);
		if (isFormatCorrect == true)
		{
			// Get port1, port2, and channel info 
			projectMappingInfo.portXmlMapping = projectMappingInfo.groupMapping->Mapping->ToArray();
			// Assign
			AssignMappingVariable(	projectMappingInfo.portXmlMapping,
									projectMappingInfo.mapPort1Arr,
									projectMappingInfo.mapPort2Arr,
									projectMappingInfo.mapVnaChannelArr,
									projectMappingInfo.mapVnaChannelStringArr);
		}
		else
		{
			ret = ER_CONST_PROGRAMDATA_AEM_AMB7300_MAPPINGFILE_CONTENT_INVALID;
			if (tfSite == 0)
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Invalid content inside project mapping file (.xml) in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Invalid content inside project mapping file (.xml) in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			else
			{
				tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Invalid content inside project mapping file (.xml) in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeTester -> GetProjectMappingInfo] Invalid content inside project mapping file (.xml) in the local '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATEXFOLDER + tfSite.ToString() + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			}
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	void AMB7300TestLibrary::ModifiedLoadedMappingToInteger(GroupMapping ^ % groupMapping, bool % isFormatCorrect)
	{
		/*****************************************************************************************************
		** ModifiedLoadedMappingToInteger
		**		groupMapping	- This is the class ref GroupMapping.
		**		isFormatCorrect	- This is the conversion status.
		**
		** Descriptions:
		**		This is a function is to modify the original mapping port name into integer, and store it.
		**		eg: 1A ---> to ---> 1 | 2A ---> to ---> 2
		**		The conversion status will return as true if there is no error occur during the conversion.
		******************************************************************************************************/

		isFormatCorrect = true;

		if (groupMapping != nullptr)
		{
			if (groupMapping->type == sysConfigInfo.amb7300A0ConfigName)
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1") ||
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("2"))
							isFormatCorrect = true;
						else
							isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type->Contains(sysConfigInfo.amb7300A1ConfigName))
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "4";
						else isFormatCorrect = false;

						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "8";
						else isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type->Contains(sysConfigInfo.amb7300A2ConfigName))
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "4";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1E"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1F"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1G"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1H"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "8";
						else isFormatCorrect = false;

						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "9";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "10";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "11";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "12";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2E"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "13";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2F"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "14";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2G"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "15";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2H"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "16";
						else isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type->Contains(sysConfigInfo.amb7300A3ConfigName))
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "4";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("3A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("3B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("4A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("4B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "8";
						else isFormatCorrect = false;

						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "4";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("3A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("3B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("4A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("4B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "8";
						else isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type->Contains(sysConfigInfo.amb7300A4ConfigName))
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1") || groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("2") ||
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("3") || groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("4"))
							isFormatCorrect = true;
						else
							isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type == sysConfigInfo.amb7300A5ConfigName)
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "4";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1E"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1F"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1G"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1H"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "8";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1I"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "9";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1J"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "10";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1K"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "11";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1L"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "12";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1M"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "13";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1N"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "14";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1O"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "15";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1P"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "16";
						else isFormatCorrect = false;

						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "17";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "18";
						else isFormatCorrect = false;
					}
				}
			}
			else if (groupMapping->type == sysConfigInfo.amb7300S6P22NConfigName)
			{
				for (int i = 0; i < groupMapping->Mapping->Count; i++)
				{
					for (int j = 0; j < groupMapping->Mapping[i].VNAMapping_Pair->Length; j++)
					{
						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "1";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "2";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1C"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "3";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1D"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "4";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1E"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "5";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1F"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "6";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1G"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "7";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1H"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "8";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1I"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "9";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1J"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "10";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1K"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "11";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1L"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "12";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1M"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "13";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1N"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "14";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1O"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "15";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port1->Contains("1P"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port1 = "16";
						else isFormatCorrect = false;

						if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2A"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "17";
						else if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("2B"))
							groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "18";
						else isFormatCorrect = false;

						//if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port3->Contains("3A"))
						//	groupMapping->Mapping[i].VNAMapping_Pair[j].Port3 = "19";
						//else isFormatCorrect = false;

						//if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("4A"))
						//	groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "20";
						//else isFormatCorrect = false;

						//if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port3->Contains("5A"))
						//	groupMapping->Mapping[i].VNAMapping_Pair[j].Port3 = "21";
						//else isFormatCorrect = false;

						//if (groupMapping->Mapping[i].VNAMapping_Pair[j].Port2->Contains("6A"))
						//	groupMapping->Mapping[i].VNAMapping_Pair[j].Port2 = "22";
						//else isFormatCorrect = false;
					}
				}
			}
		}
		else
		{
			isFormatCorrect == false;
		}
	}
	void AMB7300TestLibrary::AssignMappingVariable(array<Aemulus::Configuration::Group> ^ portXmlMapping, array<int> ^ % mapPort1Arr, array<int> ^ % mapPort2Arr, array<int> ^ % mapVnaChannelArr, array<String^> ^ % mapVnaChannelStringArr)
	{
		/*****************************************************************************************************
		** AssignMappingVariable
		**		portXmlMapping			- This is class ref Group.
		**		mapPort1Arr				- This is port1 alias in int.
		**		mapPort2Arr				- This is port2 alias in int.
		**		mapVnaChannelArr		- This is channel alias in int.
		**		mapVnaChannelStringArr	- This is channel alias in string.
		**
		** Descriptions:
		**		This is a function is to get the mapping file info.
		**		eg: port1 alias, port2 alias, and the relative channel alias.
		******************************************************************************************************/

		// Local variable
		int length = 0;
		VNAMapping_Pair pair;
		Aemulus::Configuration::Group ^ group;

		if (portXmlMapping != nullptr)
		{
			for (int index = 0; index < portXmlMapping->Length; index++)
			{
				group = portXmlMapping[index];
				for (int pairIndex = 0; pairIndex < group->VNAMapping_Pair->Length; pairIndex++)
				{
					pair							= group->VNAMapping_Pair[pairIndex];
					mapPort1Arr[length]				= Int32::Parse(pair.Port1);
					mapPort2Arr[length]				= Int32::Parse(pair.Port2);
					mapVnaChannelArr[length]		= Int32::Parse(pair.VNAChannel);
					mapVnaChannelStringArr[length]	= "Channel" + Int32::Parse(pair.VNAChannel);
					length++;
				}
			}
		}
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	techFlow siteIndex to vnaSiteIndex mapping
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::Map_siteIndex_To_vnaSiteIndex(int tfSite)
	{
		/*****************************************************************************************************
		** Map_siteIndex_To_vnaSiteIndex
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		 Modify this portion for SharedVna / True Parallel project
		**		 Assign each test site to respective VNA index
		**		 e.g. siteIndex_By_vnaSiteIndex = { (int)siteIndex, (int)vnaSiteIndex }
		******************************************************************************************************/

		int ret = 0;

		try 
		{
			for (int i = 0; i < tl->glob->tf.NumberOfTestSites; i++)
			{
				if (i == 0)
				{
					siteIndex_By_vnaSiteIndex[i] = 0;
				}
				else if (i == 1)
				{
					siteIndex_By_vnaSiteIndex[i] = 0;
				}
				else if (i == 2)
				{
					;		//Spare for use of site 2
				}
				else if (i == 3)
				{
					;		//Spare for use of site 3
				}
				else if (i == 4)
				{
					;		//Spare for use of site 4
				}
				else if (i == 5)
				{
					;		//Spare for use of site 5
				}
				if (siteIndex_By_vnaSiteIndex[i] > amb7300SystemSetting->vnaSystemCount)
				{
					throw gcnew System::Exception("[Invalid Mapping] vnaSiteIndex for site " + i + " exceeds total vnaSystemCount!");
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_ASSIGNED_VNASITEINDEX_INVALID;
			tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> Map_siteIndex_To_vnaSiteIndex] Invalid mapped value for VNA site index '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, ERROR, "[Load -> Map_siteIndex_To_vnaSiteIndex] Invalid mapped value for VNA site index '" + FILENAME_CONST_PROGRAMDATA_AEM_AMB7300_STATE0FOLDER + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Vna Operation (Copper Mountain | Keysight)
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeVna(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeVna
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		int vnaSiteIndexAlias	= 0;

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Initialize Copper Mountain VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Initialize Copper Mountain VNA hardware.");

			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				vnaSiteIndexAlias = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				vnaSiteIndexAlias = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}
			
			// Initialize VNA
			ret = InitializeVna_CMT(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tfSite, siteIndex, vnaSiteIndexAlias, tl->glob->AWV.Offline);
			if (ret != 0) goto EndOfTest;

			// Get VNA info
			tl->CheckError(tfSite, amb7300_NA[siteIndex]->GetDeviceInfo(saveRecallSetting->Model, saveRecallSetting->SerialNumber, saveRecallSetting->FWVersion));

			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Copper Mountain VNA info: "  																+ " \n " +
																																								"\t" + "VNA Model: "			+ saveRecallSetting->Model			+ " \n " +
																																								"\t" + "VNA Serial Number: "	+ saveRecallSetting->SerialNumber	+ " \n " +
																																								"\t" + "VNA FW Version: "		+ saveRecallSetting->FWVersion);
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Copper Mountain VNA info: "  																+ " \n " +
																																								"\t" + "VNA Model: "			+ saveRecallSetting->Model			+ " \n " +
																																								"\t" + "VNA Serial Number: "	+ saveRecallSetting->SerialNumber	+ " \n " +
																																								"\t" + "VNA FW Version: "		+ saveRecallSetting->FWVersion);
		
			////Check Calibration Validity 
			//array<int>^ vnaChannel = nullptr;
			//array<String^>^ calDate = nullptr;
			//int returnCode = 0;

			//tl->CheckError(tfSite, amb7300_[siteIndex]->amb7300->QueryCalibrationDate(vnaChannel, calDate));
			//DateTime currentDate = DateTime::Now;	//Get current date time

			//array<String^>^ formattedDate = gcnew array<String^>(0);
			//array<String^> ^ separatorFull = gcnew array<String^>(1);

			////separate the date and time
			//separatorFull[0] = " ";
			//formattedDate = calDate[0]->Split(separatorFull, StringSplitOptions::None);

			//if (formattedDate[0]->Contains("-"))
			//{
			//	separatorFull[0] = "-";
			//}
			//else if (formattedDate[0]->Contains("/"))
			//{
			//	separatorFull[0] = "/";
			//}

			////separate date in months, days, yeaars
			//formattedDate = formattedDate[0]->Split(separatorFull, StringSplitOptions::None);

			////check format available
			//String^ format = nullptr;

			//if (formattedDate[0]->Length == 4)
			//{
			//	format = "yyyy" + separatorFull[0] + "MM" + separatorFull[0] + "dd HH:mm:ss";
			//}
			//else if (formattedDate[0]->Length == 2 && formattedDate[1]->Length == 2)
			//{
			//	if (formattedDate[2]->Length == 4)
			//		format = "dd" + separatorFull[0] + "MM" + separatorFull[0] + "yyyy HH:mm:ss";
			//	else if (formattedDate[2]->Length == 2)
			//		format = "dd" + separatorFull[0] + "MM" + separatorFull[0] + "yy HH:mm:ss";
			//}
			//else
			//{
			//	if (formattedDate[2]->Length == 4)
			//		format = "d" + separatorFull[0] + "M" + separatorFull[0] + "yyyy HH:mm:ss";
			//	else if (formattedDate[2]->Length == 2)
			//		format = "d" + separatorFull[0] + "M" + separatorFull[0] + "yy HH:mm:ss";
			//}

			///*if (formattedDate[0]->Length == 4 && separatorFull[0] == "-")
			//{
			//	format = "yyyy-MM-dd HH:mm:ss";
			//}
			//else if (formattedDate[0]->Length == 4 && separatorFull[0] == "/")
			//{
			//	format = "yyyy/MM/dd HH:mm:ss";
			//}
			//else if (formattedDate[0]->Length == 2 && formattedDate[1]->Length == 2)
			//{
			//	if (separatorFull[0] == "/")
			//	{
			//		if (formattedDate[2]->Length == 4)
			//			format = "dd/MM/yyyy HH:mm:ss";
			//		else if (formattedDate[2]->Length == 2)
			//			format = "dd/MM/yy HH:mm:ss";
			//	}
			//	else
			//	{
			//		if (formattedDate[2]->Length == 4)
			//			format = "dd-MM-yyyy HH:mm:ss";
			//		else if (formattedDate[2]->Length == 2)
			//			format = "dd-MM-yy HH:mm:ss";
			//	}
			//}
			//else
			//{
			//	if (separatorFull[0] == "/")
			//	{
			//		if (formattedDate[2]->Length == 4)
			//			format = "d/M/yyyy HH:mm:ss";
			//		else if (formattedDate[2]->Length == 2)
			//			format = "d/M/yy HH:mm:ss";
			//	}
			//	else
			//	{
			//		if (formattedDate[2]->Length == 4)
			//			format = "d-M-yyyy HH:mm:ss";
			//		else if (formattedDate[2]->Length == 2)
			//			format = "d-M-yy HH:mm:ss";
			//	}
			//}*/

			////Will conduct Calibration Validity Check if CalibrationValidityDay != 0
			//if (tl->glob->AWV.CalibrationValidityDay != 0)
			//{
			//	for each(String^ date in calDate)
			//	{
			//		DateTime formatedCalDate = DateTime::ParseExact(date, format, CultureInfo::InvariantCulture);
			//		TimeSpan timeDifference = currentDate - formatedCalDate;

			//		//Cal Validity Check
			//		if (timeDifference.TotalDays > tl->glob->AWV.CalibrationValidityDay)
			//		{
			//			MessageBox::Show("Calibration has been Expired! \nLast calibration date time: " + date + "\nPlease conduct calibration of the system!", "Calibration Expried");
			//			break;
			//		}
			//	}
			//}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Initialize Keysight VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Initialize Keysight VNA hardware.");

			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				vnaSiteIndexAlias = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				vnaSiteIndexAlias = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}

			// Initialize VNA
			ret = InitializeVna_Keysight(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tfSite, siteIndex, vnaSiteIndexAlias, tl->glob->AWV.Offline);
			if (ret != 0) goto EndOfTest;

			// Get VNA info
			tl->CheckError(tfSite, amb7300_NA[siteIndex]->GetDeviceInfo(saveRecallSetting->Model, saveRecallSetting->SerialNumber, saveRecallSetting->FWVersion));

			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Keysight VNA info: "  																		+ " \n " +
																																								"\t" + "VNA Model: "			+ saveRecallSetting->Model			+ " \n " +
																																								"\t" + "VNA Serial Number: "	+ saveRecallSetting->SerialNumber	+ " \n " +
																																								"\t" + "VNA FW Version: "		+ saveRecallSetting->FWVersion);
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Keysight VNA info: "  																		+ " \n " +
																																								"\t" + "VNA Model: "			+ saveRecallSetting->Model			+ " \n " +
																																								"\t" + "VNA Serial Number: "	+ saveRecallSetting->SerialNumber	+ " \n " +
																																								"\t" + "VNA FW Version: "		+ saveRecallSetting->FWVersion);
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeVna(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeVna
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Unload -> UninitializeTester -> CopperMountainVnaUtility -> UninitializeVna_CMT] Uninitialize Copper Mountain VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Unload -> UninitializeTester -> CopperMountainVnaUtility -> UninitializeVna_CMT] Uninitialize Copper Mountain VNA hardware.");

			// Uninitialize VNA
			ret = UninitializeVna_CMT(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Unload -> UninitializeTester -> KeysightVnaUtility -> UninitializeVna_Keysight] Uninitialize Keysight VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Unload -> UninitializeTester -> KeysightVnaUtility -> UninitializeVna_Keysight] Uninitialize Keysight VNA hardware.");

			// Uninitialize VNA
			ret = UninitializeVna_Keysight(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::HighPwrTest_VNASourceLow(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeVna
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to source low VNA hardware for each active channel during HighPwrTest.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PostProcessing -> PostProcessingTester -> CopperMountainVnaUtility -> SourceLowVna_CMT] Source Low Copper Mountain VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PostProcessing -> PostProcessingTester -> CopperMountainVnaUtility -> SourceLowVna_CMT] Source Low Copper Mountain VNA hardware.");

			// Source Low VNA
			ret = SourceLowVna_CMT(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PostProcessing -> PostProcessingTester -> KeysightVnaUtility -> SourceLowVna_Keysight] Source Low Keysight VNA hardware.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PostProcessing -> PostProcessingTester -> KeysightVnaUtility -> SourceLowVna_Keysight] Source Low Keysight VNA hardware.");

			// Source Low VNA
			ret = SourceLowVna_Keysight(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::VnaConfig(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		** VnaConfig
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'VnaConfig' phase.
		******************************************************************************************************/

		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig] Executing 'VnaConfig' phase.");
		tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig] Executing 'VnaConfig' phase.");

		// Local variable
		int ret = 0;

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) || 
				(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment) ||
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> CopperMountainVnaUtility -> ConfigureSegmentSetting_CMT] Configure segment setting at the active channel.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> CopperMountainVnaUtility -> ConfigureSegmentSetting_CMT] Configure segment setting at the active channel.");
				// Configure segment settings on every available channel in mapping file
				ret = ConfigureSegmentSetting_CMT(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}

			////Only get once NowTime for S3P asynchronous save.
			//if (projectStateInfo.SNP_Override == "S3P")
			//{
			//	tl->glob->S3P_TimeNow = DateTime::Now.ToString("yyyyMMdd'_'hmmssffff");
			//}

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> CopperMountainVnaUtility -> ConfigureTriggerSource_CMT] Configure trigger source.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> CopperMountainVnaUtility -> ConfigureTriggerSource_CMT] Configure trigger source.");
			// Configure trigger source + hold all channel
			ret = ConfigureTriggerSource_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) || 
				(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment) ||
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureSegmentSetting_Keysight] Configure segment setting at the active channel.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureSegmentSetting_Keysight] Configure segment setting at the active channel.");
				// Configure segment settings on every available channel in mapping file
				ret = ConfigureSegmentSetting_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_PowerSweep) || // Under development, not support yet
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer_PowerSweep))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigurePowerSweep_Keysight] Configure power sweep setting at the active channel.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigurePowerSweep_Keysight] Configure power sweep setting at the active channel.");
				// Configure segment settings on every available channel in mapping file
				ret = ConfigurePowerSweepSetting_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_CWTime))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureCWTime_Keysight] Configure CW Time sweep setting at the active channel.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureCWTime_Keysight] Configure CW Time sweep setting at the active channel.");
				// Configure segment settings on every available channel in mapping file
				ret = ConfigureCWTimeSweepSetting_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if (segmentSetting[vnaSiteIndex].sweepType == Vna_SweepType_GetFromStateFile)
			{
			}


			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureTriggerSource_Keysight] Configure trigger source.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaConfig -> KeysightVnaUtility -> ConfigureTriggerSource_Keysight] Configure trigger source.");
			// Configure trigger source + hold all channel
			//ret = ConfigureTriggerSource_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::VnaFetch(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		** VnaFetch
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'VnaFetch' phase.
		******************************************************************************************************/
		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch] Executing 'VnaFetch' phase.");
		tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch] Executing 'VnaFetch' phase.");

		// Local variable
		int ret = 0;

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Configure MPEX port pair switching.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Configure MPEX port pair switching.");
			// Configure MPEX port pair switching
			ret = ConfigureMpexPortPairSwitching_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
			
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			// Get current vna fetch active channel for generic usage
			ret = GetCurrentVnaFetchChannel(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureTraces_CMT] Configure trace setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureTraces_CMT] Configure trace setting at the active channel.");
			// Configure trace settings at the active channel
			ret = ConfigureTraces_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
	
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureAveraging_CMT] Configure averaging setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureAveraging_CMT] Configure averaging setting at the active channel.");
			// If required to apply averaging at the active channel
			ret = ConfigureAveraging_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
			// Configure active channel + single trigger at the active channel
			if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) || 
				(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Configure active channel, arm and wait for data.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Configure active channel, arm and wait for data.");
				ret = ConfigureActiveChannelAndSingleTrigger_CMT(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[vnaSiteIndex].sweepType == Vna_SweepType_GetFromStateFile) || 
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Configure single trigger for GetFromStateFile type.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Configure single trigger for GetFromStateFile type.");
				ret = ConfigureSingleTriggerForGetFromStateFileType_CMT(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			// [Future Enchancement] API not support. 
			// There is A5, A4 MPEX setup for Keysight VNA.
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Configure MPEX port pair switching.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Configure MPEX port pair switching.");
			// Configure MPEX port pair switching
			ret = ConfigureMpexPortPairSwitching_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			// Get current vna fetch active channel for generic usage
			ret = GetCurrentVnaFetchChannel(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
			
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureTraces_Keysight] Configure trace setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureTraces_Keysight] Configure trace setting at the active channel.");
			// Configure trace settings at the active channel
			ret = ConfigureTraces_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureAveraging_Keysight] Configure averaging setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureAveraging_Keysight] Configure averaging setting at the active channel.");
			// If required to apply averaging at the active channel
			ret = ConfigureAveraging_Keysight(tfSite, vnaSiteIndex);			
			if (ret != 0) goto EndOfTest;
			// Configure active channel + single trigger at the active channel
			if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) || (segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Configure active channel, arm and wait for data.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Configure active channel, arm and wait for data.");
				ret = ConfigureActiveChannelAndSingleTrigger_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if (segmentSetting[vnaSiteIndex].sweepType == Vna_SweepType_GetFromStateFile)
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Configure single trigger for GetFromStateFile type.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Configure single trigger for GetFromStateFile type.");
				ret = ConfigureSingleTriggerForGetFromStateFileType_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if (segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer)
			{
				//SharedMemoryTransfer
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_Initialise] . Initialisation for MemoryMap");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_Initialise] . Initialisation for Memory Map");
				ret = SharedMemoryTransfer_Initialise(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;

				//SharedMemoryTransfer
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_FetchData] . Fetch Data from Memory Map");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_FetchData] . Fetch Data from Memory Map");
				ret = SharedMemoryTransfer_FetchData(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::VnaDataAnalysis(int tfSite, int vnaSiteIndex, double % result)
	{
		/*****************************************************************************************************
		** VnaDataAnalysis
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**		result			- Return test result.
		**
		** Descriptions:
		**		This is a function to execute 'VnaDataAnalysis' phase.
		******************************************************************************************************/

		/*	Info
		**	LogMag		--> SFP 1x parameter	--> API return as real raw
		**	SWR			--> SFP 1x parameter	--> API return as real raw
		**	Phase		--> SFP 1x parameter	--> API return as real raw
		**	ExPhase		--> SFP 1x parameter	--> API return as real raw
		**	Delay		--> SFP 1x parameter	--> API return as real raw
		**	LinMag		--> SFP 1x parameter	--> API return as real raw
		**	Reak		--> SFP 1x parameter	--> API return as real raw
		**	Imag		--> SFP 1x parameter	--> API return as real raw
		**	SmithLog	--> SFP 2x parameter	--> [wrapper special handle] API return: 1st para as real raw | 2nd para as imag raw
		**	SmithLin	--> SFP 2x parameter	--> [wrapper special handle] (error)return wrong												!
		**	SmithReIm	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw
		**	SmithR+JX	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw | (error)does not have 3rd para		!
		**	SmithG+JB	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw | (error)does not have 3rd para		!
		**	PolarLog	--> SFP 2x parameter	--> [wrapper special handle] API return: 1st para as real raw | 2nd para as imag raw
		**	PolarLin	--> SFP 2x parameter	--> [wrapper special handle] (error)return wrong												!
		**	PolarReIm	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw
		*/

		// Local variable
		int ret = 0;
		result	= (double)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis] Executing 'VnaDataAnalysis' phase.");
		tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis] Executing 'VnaDataAnalysis' phase.");

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Configure smoothing setting at the active trace.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Configure smoothing setting at the active trace.");
			// If required to apply smoothing at the active trace
			ret = ConfigureSmoothing_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			// Execute data analysis function
			if (vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					// Get trace format data
					ret = GetTraceFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					result = ProcessVnaMeasurement(tfSite, vnaSiteIndex);

					// If target test freq does not exists in the segment range
					if (result == (double)CONST_INVALID_RESULT)
					{
						result = (double)CONST_INVALID_RESULT;
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMax) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msPeak) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msTarget) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget))
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Get marker search format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Get marker search format data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic))	||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math statistics format data
					ret = GetMarkerMathFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmStatistics_Mean->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_Mean)
						{
							result = mmFormatData->mmStatistics_Mean[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_StdDev)
						{
							result = mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_PeakToPeak)
						{
							result = mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				// Get target marker math flatness format data
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math flatness format data
					ret = GetMarkerMathFormatData_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmFlatness_Gain->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Gain)
						{
							result = mmFormatData->mmFlatness_Gain[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Slope)
						{
							result = mmFormatData->mmFlatness_Slope[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Flat)
						{
							result = mmFormatData->mmFlatness_Flat[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple))
			{
				// Get msMin & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Get marker search min max ripple data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Get marker search min max ripple data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchMinMaxRipple_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// Fix inside the GetMarkerSearchMinMaxRipple() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic))	||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
					}

					// Get min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_TraceData_Sds)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					// Get trace format data
					ret = GetTraceFormatData_CMT_Sds(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					double l_Real = 0.0; //for S21 & S11
					double l_Imag = 0.0; //for S21 & S11
					double delta_Real = 0.0;
					double delta_Imag = 0.0;
					double normalized = 0.0;
					double Compare_Value0 = 0.0;
					double Compare_Value1 = 0.0;
					int FristCount = 0;
					if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_SdsSearch)
					{

						if (analysisSetting.TraceData_Sds_SearchType[0] == "Min")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
									delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
									normalized = (sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / (sqrt(2));


									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = 20 * (std::log10(normalized)); //change to dB
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = 20 * (std::log10(normalized));
										if (Compare_Value0 <= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}
						else if (analysisSetting.TraceData_Sds_SearchType[0] == "Max")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
									delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
									normalized = (std::sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / (std::sqrt(2));

									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = 20 * (std::log10(normalized)); //change to dB
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = 20 * (std::log10(normalized));
										if (Compare_Value0 >= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}
						else if (analysisSetting.TraceData_Sds_SearchType[0] == "FixedFreq")
						{
							//test freq <= centerFreq,search from start, test freq > centerFreq,search from stop, 
							double centerFreq = (traceFormatData->freqPoint[vnaSiteIndex, 0] + traceFormatData->freqPoint[vnaSiteIndex, (traceFormatData->totalFreqPoints - 1)]) / 2;
							//	double centerFreq = (vnaDataAnalysisTPC.testFreqRangeStart[0] + vnaDataAnalysisTPC.testFreqRangeStop[0]) / 2;
							if (vnaDataAnalysisTPC.testFreq[0] <= centerFreq)
							{
								for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
								{
									if (traceFormatData->freqPoint[vnaSiteIndex, j] == vnaDataAnalysisTPC.testFreq[0])
									{
										delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
										delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
										normalized = (std::sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / (std::sqrt(2));
										result = 20 * (std::log10(normalized)); //change to dB

									}
								}
							}
							else
							{
								for (int j = traceFormatData->totalFreqPoints - 1; j >= 0; j--)
								{
									if (traceFormatData->freqPoint[vnaSiteIndex, j] == vnaDataAnalysisTPC.testFreq[0])
									{
										delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
										delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
										//normalized = ((delta_Real * delta_Real) + (delta_Imag * delta_Imag)) / (std::sqrt(2));
										normalized = (std::sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / (std::sqrt(2));
										result = 20 * (std::log10(normalized)); //change to dB

									}
								}
							}

						}

					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_Sdd22Search)
					{
						if (analysisSetting.TraceData_Sds_SearchType[0] == "Max")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									//delta_Real = (traceFormatData->S22Real[vnaSiteIndex, j] - traceFormatData->S21Rea23[vnaSiteIndex, j] - traceFormatData->S12Rea23[vnaSiteIndex, j] + traceFormatData->S22Rea13[vnaSiteIndex, j]) / 2;
									//delta_Imag = (traceFormatData->S22Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary4[vnaSiteIndex, j] - traceFormatData->S12Imaginary4[vnaSiteIndex, j] + traceFormatData->S22Imaginary3[vnaSiteIndex, j]) / 2;
									delta_Real = (traceFormatData->S11Real[vnaSiteIndex, j] - traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S12Real[vnaSiteIndex, j] + traceFormatData->S22Real[vnaSiteIndex, j])/2 ;
									delta_Imag = (traceFormatData->S11Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S12Imaginary[vnaSiteIndex, j] + traceFormatData->S22Imaginary[vnaSiteIndex, j])/2 ;
									//normalized = (sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / 2;
									normalized = (sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag));

									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = 20 * log10(normalized); //change to dB
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = 20 * log10(normalized);
										if (Compare_Value0 >= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}

					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_AmpImbalance)
					{
						if (analysisSetting.TraceData_Sds_SearchType[0] == "Max")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									//delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
									//delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
									//normalized = sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag);

									// 一次性计算幅度不平衡度，直接赋给 normalized,normalized = fabs( 20 * log10( sqrt( S21_real² + S21_imag² ) )-20 * log10(sqrt(S31_real² + S31_imag²)) );
									normalized = fabs(20 * log10(sqrt(traceFormatData->S21Real[vnaSiteIndex, j] * traceFormatData->S21Real[vnaSiteIndex, j]
										+ traceFormatData->S21Imaginary[vnaSiteIndex, j] * traceFormatData->S21Imaginary[vnaSiteIndex, j]))
										- 20 * log10(sqrt(traceFormatData->S21Real2[vnaSiteIndex, j] * traceFormatData->S21Real2[vnaSiteIndex, j]
											+ traceFormatData->S21Imaginary2[vnaSiteIndex, j] * traceFormatData->S21Imaginary2[vnaSiteIndex, j])));


									if ((normalized > 0) && (FristCount == 0))
									{
										//Compare_Value0 = abs(20 * log10(normalized)); //change to dB
										Compare_Value0 = normalized;
										FristCount++;
									}
									else if (normalized > 0)
									{
										//Compare_Value1 = abs(20 * log10(normalized));
										Compare_Value1 = normalized;
										if (Compare_Value0 >= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}
					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_PhaseImbalance)
					{
						double Phase_Angle_S21 = 0.0;
						double Phase_Angle_S31 = 0.0;
						double Angle_S21_Pre = 0.0;
						double Angle_S31_Pre = 0.0;
						double UnwrapAngle_S21 = 0.0;
						double UnwrapAngle_S31 = 0.0;
						double Diff_S21 = 0.0;
						double Diff_S31 = 0.0;
						if (analysisSetting.TraceData_Sds_SearchType[0] == "Max")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									//change to Phase Angle
									Phase_Angle_S21 = (atan2(traceFormatData->S21Imaginary[vnaSiteIndex, j], traceFormatData->S21Real[vnaSiteIndex, j])) * 180 / M_PI;
									Phase_Angle_S31 = (atan2(traceFormatData->S21Imaginary2[vnaSiteIndex, j], traceFormatData->S21Real2[vnaSiteIndex, j])) * 180 / M_PI;
									//unwrap angle
									if (FristCount == 0)
									{
										Angle_S21_Pre = Phase_Angle_S21;
										Angle_S31_Pre = Phase_Angle_S31;

									}
									else
									{
										Diff_S21 = Phase_Angle_S21 - Angle_S21_Pre;
										Diff_S31 = Phase_Angle_S31 - Angle_S31_Pre;
										if (Diff_S21 > 180)
										{
											Phase_Angle_S21 -= 360;
										}
										else if (Diff_S21 < -180)
										{
											Phase_Angle_S21 += 360;
										}

										if (Diff_S31 > 180)
										{
											Phase_Angle_S31 -= 360;
										}
										else if (Diff_S31 < -180)
										{
											Phase_Angle_S31 += 360;
										}
										//set Pre value
										Angle_S21_Pre = Phase_Angle_S21;
										Angle_S31_Pre = Phase_Angle_S31;
									}

									// calculat Max Value
									if (FristCount == 0)
									{
										//Compare_Value0 = 180 - abs(Phase_Angle_S21 - Phase_Angle_S31);
										Compare_Value0 = abs(Phase_Angle_S21 - Phase_Angle_S31 - 180);
										FristCount++;
									}
									else
									{
										Compare_Value1 = abs(Phase_Angle_S21 - Phase_Angle_S31 - 180);
										if (Compare_Value0 >= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}


								}
							}
						}
					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_ComRejectRatio)
					{

						double add_normalized = 0.0;
						double add_Real = 0.0;
						double add_Imag = 0.0;
						double dB_Value = 0.0;
						double add_dB_Value = 0.0;

						if (analysisSetting.TraceData_Sds_SearchType[0] == "Min")
						{
							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									delta_Real = traceFormatData->S21Real[vnaSiteIndex, j] - traceFormatData->S21Real2[vnaSiteIndex, j];
									delta_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] - traceFormatData->S21Imaginary2[vnaSiteIndex, j];
									normalized = (sqrt(delta_Real * delta_Real + delta_Imag * delta_Imag)) / (sqrt(2));
									dB_Value = 20 * log10(normalized);

									add_Real = traceFormatData->S21Real[vnaSiteIndex, j] + traceFormatData->S21Real2[vnaSiteIndex, j];
									add_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j] + traceFormatData->S21Imaginary2[vnaSiteIndex, j];
									add_normalized = (sqrt(add_Real * add_Real + add_Imag * add_Imag)) / (sqrt(2));
									add_dB_Value = 20 * log10(add_normalized);

									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = abs(dB_Value - add_dB_Value);
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = abs(dB_Value - add_dB_Value);
										if (Compare_Value0 <= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}

					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_S21Search)
					{

						if (analysisSetting.TraceData_Sds_SearchType[0] == "FixedFreq")
						{
							double	incidentPower_dBm = 0.0 dBm;
							//test freq <= centerFreq,search from start, test freq > centerFreq,search from stop, 
							double centerFreq = (traceFormatData->freqPoint[vnaSiteIndex, 0] + traceFormatData->freqPoint[vnaSiteIndex, (traceFormatData->totalFreqPoints - 1)]) / 2;
							//	double centerFreq = (vnaDataAnalysisTPC.testFreqRangeStart[0] + vnaDataAnalysisTPC.testFreqRangeStop[0]) / 2;
							if (vnaDataAnalysisTPC.testFreq[0] <= centerFreq)
							{
								for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
								{
									if (traceFormatData->freqPoint[vnaSiteIndex, j] == vnaDataAnalysisTPC.testFreq[0])
									{
										l_Real = traceFormatData->S21Real[vnaSiteIndex, j];
										l_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j];
										normalized = sqrt(l_Real * l_Real + l_Imag * l_Imag);
										result = 20 * (log10(normalized)); //change to dB

									}
								}
							}
							else
							{
								for (int j = traceFormatData->totalFreqPoints - 1; j >= 0; j--)
								{
									if (traceFormatData->freqPoint[vnaSiteIndex, j] == vnaDataAnalysisTPC.testFreq[0])
									{
										l_Real = traceFormatData->S21Real[vnaSiteIndex, j];
										l_Imag = traceFormatData->S21Imaginary[vnaSiteIndex, j];
										normalized = sqrt(l_Real * l_Real + l_Imag * l_Imag);
										result = 20 * (log10(normalized)); //change to dB

									}
								}
							}

						}

					}
					else if (analysisSetting.TraceData_Sds_SearchRangeEnable[0] == Cond_AnalysisSetting_TraceData_Sds_S11Search)
					{

						if (analysisSetting.TraceData_Sds_SearchType[0] == "Max")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									l_Real = traceFormatData->S11Real[vnaSiteIndex, j];
									l_Imag = traceFormatData->S11Imaginary[vnaSiteIndex, j];
									normalized = sqrt(l_Real * l_Real + l_Imag * l_Imag);


									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = 20 * (log10(normalized)); //change to dB
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = 20 * (log10(normalized));
										if (Compare_Value0 >= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}
						else if (analysisSetting.TraceData_Sds_SearchType[0] == "Min")
						{

							for (int j = 0; j < traceFormatData->totalFreqPoints; j++)
							{
								if ((traceFormatData->freqPoint[vnaSiteIndex, j] >= vnaDataAnalysisTPC.testFreqRangeStart[0]) &&
									(traceFormatData->freqPoint[vnaSiteIndex, j] <= vnaDataAnalysisTPC.testFreqRangeStop[0]))
								{
									l_Real = traceFormatData->S11Real[vnaSiteIndex, j];
									l_Imag = traceFormatData->S11Imaginary[vnaSiteIndex, j];
									normalized = sqrt(l_Real * l_Real + l_Imag * l_Imag);


									if ((normalized > 0) && (FristCount == 0))
									{
										Compare_Value0 = 20 * (log10(normalized)); //change to dB
										FristCount++;
									}
									else if (normalized > 0)
									{
										Compare_Value1 = 20 * (log10(normalized));
										if (Compare_Value0 <= Compare_Value1)
										{
											result = Compare_Value0;
										}
										else
										{
											Compare_Value0 = Compare_Value1;
											result = Compare_Value0;
										}
									}

								}
							}
						}

					}

					// If target test freq does not exists in the segment range
					if (result == (double)CONST_INVALID_RESULT)
					{
						result = (double)CONST_INVALID_RESULT;
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> ConfigureSmoothing_Keysight] Configure smoothing setting at the active trace.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> ConfigureSmoothing_Keysight] Configure smoothing setting at the active trace.");
			// If required to apply smoothing at the active trace
			ret = ConfigureSmoothing_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
		
			// Execute data analysis function
			if (vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Get trace format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Get trace format data.");
					// Get trace format data
					ret = GetTraceFormatData_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					result = ProcessVnaMeasurement(tfSite, vnaSiteIndex);

					// If user target test freq does not exists in the segment range / trace freq
					if (result == (double)CONST_INVALID_RESULT)
					{
						result = (double)CONST_INVALID_RESULT;
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMax) || (vnaDataAnalysisTPC.function == Cond_Function_msPeak) || (vnaDataAnalysisTPC.function == Cond_Function_msTarget))
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Get marker search format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Get marker search format data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchFormatData_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic))	||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[vnaSiteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math statistics format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmStatistics_Mean->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_Mean)
						{
							result = mmFormatData->mmStatistics_Mean[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_StdDev)
						{
							result = mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_PeakToPeak)
						{
							result = mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch) || (vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == "Cond_Function_mmFlatness_API_NotSupported_For_KeysightVna")
			{
				// Get target marker math flatness format data
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math flatness format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmFlatness_Gain->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Gain)
						{
							result = mmFormatData->mmFlatness_Gain[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Slope)
						{
							result = mmFormatData->mmFlatness_Slope[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Flat)
						{
							result = mmFormatData->mmFlatness_Flat[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple)
			{
				// Get msMin & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchMinMaxRipple_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// [Info] Fix inside the GetMarkerSearchMinMaxRipple_() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic))	||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear))		||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL))			||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[vnaSiteIndex, i];
							}
						}
					}

					// Get marker search min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataTarget] Get Target data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataTarget] Get Target data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[vnaSiteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch ||
					vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataBWSearch] Get BWSearch data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataBWSearch Get BWSearch data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i <SharedMemFormatData->BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = SharedMemFormatData->BW[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = SharedMemFormatData->Cent[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = SharedMemFormatData->Low[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = SharedMemFormatData->High[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = SharedMemFormatData->Q[vnaSiteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = SharedMemFormatData->Loss[vnaSiteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				// Get Min & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_MinMaxRipple] Shared Memory min max ripple data.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_MinMaxRipple] Shared Memory search min max ripple data.");
					// SharedMemory search format data  (min max / peak / target)
					ret = SharedMemoryTransfer_MinMaxRipple(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// [Info] Fix inside the GetMarkerSearchMinMaxRipple_() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[vnaSiteIndex, i];
							}
						}
					}

					// Get marker search min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}

			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SaveToTouchstoneFile(Site ^ site, int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		** SaveToTouchstoneFile
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to save the trace data into touchstone file / snp file
		**		for the current 'VnaFetch'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// File name
		String ^ snpFileName = nullptr;
		String ^ snpFileType = String::Empty;

		//Sub Lot Enabled, CP Test variables
		String ^ WaferID = "WaferID";
		String ^ LOTID = "LOTID";
		String ^ X = "";
		String ^ Y = "";
	//	long Current_UUT = 0;
		String ^ l_CustLotId = nullptr;
		

		String ^ strCurrent_UUT = "";

#pragma region "Process Macro Variables"

		// Get time now
		if (projectStateInfo.SNP_Override != "S3P")
		{
			tl->glob->TimeNow = DateTime::Now.ToString("yyyyMMdd'_'hmmssffff");
		}
		else
		{
			//Put TimeNow grab at VNAConfig into snp fileName
			tl->glob->TimeNow = tl->glob->S3P_TimeNow;
		}

		//LOTID
		tl->glob->tf.G_LotId = site->LotName;
		LOTID = site->LotName;

		if (tl->glob->tf.G_LotId == nullptr || tl->glob->tf.G_LotId == "" ||
			LOTID == nullptr || LOTID == "")
		{
			tl->glob->tf.G_LotId = "LOTID";
			LOTID = "LOTID";
		}

		//CurrentUUT
		int Current_UUT = (int)site->CurrentUUT - tl->glob->tf.NumberOfTestSites + 1 + vnaSiteIndex;
		if (Current_UUT == 0)
		{
			Current_UUT = 0;
		}
		strCurrent_UUT = Current_UUT.ToString();

		//TestCode
		String^ l_TestCode = nullptr;
		if (site->FlowEngine->AppWideVariables->ContainsKey("TestCode"))
		{
			l_TestCode = site->FlowEngine->AppWideVariables["TestCode"]->Value->ToString();
		}
		else
		{
			l_TestCode = "TestCode";
		}

		if (IsRunningProduction(site) == true)
		{
			l_CustLotId = site->FlowEngine->LotManager->LotTUI->PostNewLotInfo["CustLotID"];
			l_TestCode = site->FlowEngine->LotManager->LotTUI->PostNewLotInfo["TestCode"];
		}
			
		if (l_CustLotId == "" || l_CustLotId == nullptr)
		{
			l_CustLotId = "CLID";
		}

		if (l_TestCode == "")
		{
			l_TestCode = "TestCode";
		}

		// Sub-Lot Enabled (CP Test) : WaferID & X Y Coordinate
		if (site->DUTNumberMode == DUTNumberMode::Encoded)
		{
			X = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["X"]->Value->ToString();
			Y = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["Y"]->Value->ToString();

			WaferID = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["WaferID"]->Value->ToString();

			if (WaferID == "0" || WaferID == nullptr)
			{
				WaferID = "WaferID";
			}
		}

		// Assign Macro variables
		array<String^, 2>^ macroArray = gcnew array<String^, 2> {
			{"$(Device Name)", tl->glob->tf.DeviceName },
			{ "$(Device Revision)", tl->glob->tf.DeviceRevision },
			{ "$(Program Name)", tl->glob->tf.ProjectName },
			{ "$(Program Revision)", tl->glob->tf.ProgramRevision },
			{ "$(TestItem)", tl->glob->TestProperty[vnaSiteIndex].TestItemName },
			{ "$(TestParam)",  tl->glob->currentSubItemName[vnaSiteIndex] },
			{ "$(Site)", "S" + vnaSiteIndex.ToString() },
			{ "$(CustLotId)", l_CustLotId },
			{ "$(LotId)", LOTID },
			{ "$(WaferId)", WaferID },
			{ "$(XYCoordinate)", "X" + X + "_Y" + Y },
			{ "$(UUT)", strCurrent_UUT },
			{ "$(TimeNow)", tl->glob->TimeNow },
			{ "$(TestCode)", l_TestCode }
		};
#pragma endregion

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			//if (tl->glob->AWV.Debug)
			{
				/*// Prompt message box everytime when there is a vna sweep executed
				String ^ displayMessage = "Do you want to save the Touchstone file (.snp) of current sweep?";
				String ^ displayCaption = "Save Data To Touchstone File";
				DialogResult isSaveDataToTouchstoneFile = MessageBox::Show(displayMessage, displayCaption, MessageBoxButtons::YesNo);*/

				//if (isSaveDataToTouchstoneFile == DialogResult::Yes)
				{
					// Overwrite status even it is disabled in the condition setting
					saveRecallSetting->EnableSaveSnpData = true;
					// [Future Enchancement] Alternative option to save snp file in .csv format.
					saveRecallSetting->EnableSaveAsCsvFormat = false;
					// [Future Enchancement] Alternative option to save trace data in snp file in .csv format.
					saveRecallSetting->EnableSaveTraceData = false;

					if ((projectStateInfo.SNP_Override != "NA") && (projectStateInfo.SNP_Override == "S3P"))
					{
						if (projectStateInfo.SNP_Override == Vna_SaveDataToTouchstoneFile_Type_S3P)
						{
							//snpFileType = ".s3p";
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S3P;
							vnaSetting[vnaSiteIndex].measureSnpType = (int)(Measure_SNP_Type::Measure_S3P);
						}
					}
					else
					{
						// Select touchstone file snp type
						if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S1P))
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S1P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S2P))
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S2P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S3P))
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S3P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S4P))
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S4P;

						//file type
						/*if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S1P)
						snpFileType = ".s1p";
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S2P)
						snpFileType = ".s2p";
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S3P)
						snpFileType = ".s3p";
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S4P)
						snpFileType = ".s4p";
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S5P)
						snpFileType = ".s5p";
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S6P)
						snpFileType = ".s6p";*/
					}

					// Touchstone file header port number (fixed 1,2)
					saveRecallSetting->touchstoneFilePortNumber = gcnew array<int>(2) { 1, 2 };

#pragma region "File Name"
					if (tl->glob->AWV.EnableRenameSnpData == true)
					{
						// Replace snpFileName with provided Macros
						snpFileName = tl->glob->AWV.snpFileName;
						for (int i = 0; i < macroArray->GetLength(0); i++) {
							snpFileName = snpFileName->Replace(macroArray[i, 0], macroArray[i, 1]);
						}
						snpFileName = snpFileName;
					}
					// Generic Filename Format
					else
					{
						// Sub-Lot Enabled (CP Test)
						if (site->DUTNumberMode == DUTNumberMode::Encoded)
						{
							snpFileName = tl->glob->tf.ProjectName + "_" + tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_S" + vnaSiteIndex.ToString() + "_" + LOTID + "_" + WaferID + "_X" + X + "_Y" + Y + "_" + tl->glob->TimeNow;// +snpFileType;
						}
						// Sub-Lot Disabled (FT Test)
						else
						{
							snpFileName = tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_" + tl->glob->currentSubItemName[vnaSiteIndex] + "_S" + vnaSiteIndex.ToString() + "_" + tl->glob->TimeNow;
						}
					}

					// Store every snpFileName into Ti_by_S2PFilename array
					if (tl->glob->tf.Ti_by_S2PFilename->ContainsKey(tl->glob->TestProperty[vnaSiteIndex].TestItemName) + "_S" + vnaSiteIndex)
					{
						tl->glob->tf.Ti_by_S2PFilename[(tl->glob->TestProperty[vnaSiteIndex].TestItemName) + "_S" + vnaSiteIndex] = snpFileName;
					}
					else
					{
						tl->glob->tf.Ti_by_S2PFilename->Add(tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_S" + vnaSiteIndex, snpFileName);
					}
#pragma endregion

#pragma region "Folder Path"
					String ^ snpFolderPath = nullptr;

					if (tl->glob->AWV.S2Ppath == String::Empty)
					{
						snpFolderPath = FOLDER_CONST_AEM_TF3_RESULTS + "\\" + tl->glob->tf.ProfileName + "\\" + tl->glob->tf.ProjectName;
					}
					else
					{
						// Replace snpFolderPath with provided Macros
						snpFolderPath = tl->glob->AWV.S2Ppath;
						for (int i = 0; i < macroArray->GetLength(0); i++) {
							snpFolderPath = snpFolderPath->Replace(macroArray[i, 0], macroArray[i, 1]);
						}
						snpFolderPath = snpFolderPath;
					}

					//// Sub Lot Enabled
					//if (site->DUTNumberMode == DUTNumberMode::Encoded)
					//{
					//	// Modify snpFolderPath with LOTID and WaferID
					//	snpFolderPath += "\\" + LOTID + "\\" + WaferID;
					//}
					saveRecallSetting->touchstoneFolder = snpFolderPath;
#pragma endregion

					// Create snp Folder Path if not exist
					if (!(Directory::Exists(saveRecallSetting->touchstoneFolder)) && !(saveRecallSetting->touchstoneFolder == String::Empty))
					{
						Directory::CreateDirectory(saveRecallSetting->touchstoneFolder);
					}

					// Set snp full File Path
					saveRecallSetting->touchstoneFilePath = saveRecallSetting->touchstoneFolder + "\\" + snpFileName;

					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[CopperMountainVnaUtility -> SaveTraceDataToTouchstoneFile_CMT] Save trace data to touchstone file.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[CopperMountainVnaUtility -> SaveTraceDataToTouchstoneFile_CMT] Save trace data to touchstone file.");
					ret = SaveTraceDataToTouchstoneFile_CMT(tfSite, vnaSiteIndex);
				}
			}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			//if (tl->glob->AWV.Debug)
			{
				/*// Prompt message box everytime when there is a vna sweep executed
				String ^ displayMessage = "Do you want to save the Touchstone file (.snp) of current sweep?";
				String ^ displayCaption = "Save Data To Touchstone File";
				DialogResult isSaveDataToTouchstoneFile = MessageBox::Show(displayMessage, displayCaption, MessageBoxButtons::YesNo);*/

				//if (isSaveDataToTouchstoneFile == DialogResult::Yes)
				{
					//lineNumber = (gcnew System::Diagnostics::StackFrame(0, true))->GetFileLineNumber();
					// Overwrite status even it is disabled in the condition setting
					saveRecallSetting->EnableSaveSnpData = true;
					// [Future Enchancement] Alternative option to save snp file in .csv format.
					saveRecallSetting->EnableSaveAsCsvFormat = false;
					// [Future Enchancement] Alternative option to save trace data in snp file in .csv format.
					saveRecallSetting->EnableSaveTraceData = false;

					// Select touchstone file snp type
					if (amb7300SystemSetting->systemAlias == AMB7300_S2P1D_TYPE)
					{
						if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S1P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S1P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S2P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S2P;
					}
					else if (amb7300SystemSetting->systemAlias == AMB7300_S4P1D_TYPE)
					{
						if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S1P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S1P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S2P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S2P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S3P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S3P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S4P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S4P;
					}
					else if (amb7300SystemSetting->systemAlias == AMB7300_S6P1D_TYPE)
					{
						if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S1P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S1P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S2P)
							saveRecallSetting->touchstoneFileSnpType = Vna_Save_SaveDataToTouchstoneFile_Type_S2P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S3P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S3P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S4P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S4P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S5P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S5P;
						else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S6P)
							saveRecallSetting->touchstoneFileSnpType = Vna_SaveDataToTouchstoneFile_Type_S6P;
					}
					//util->Teck(tfSite, timerFileName, lineNumber.ToString() + " SaveSnpData_SystemTypeSelect", 1);
					// Touchstone file header port number (fixed 1,2)
					saveRecallSetting->touchstoneFilePortNumber = gcnew array<int>(2) { 1, 2 };
					// [Future Enchancement] Able to create dialog and let user to insert snp file name to saved.
					String ^ snpFileType = String::Empty;
					if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S1P)
						snpFileType = ".s1p";
					else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S2P)
						snpFileType = ".s2p";
					else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S3P)
						snpFileType = ".s3p";
					else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S4P)
						snpFileType = ".s4p";
					else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S5P)
						snpFileType = ".s5p";
					else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)Measure_SNP_Type::Measure_S6P)
						snpFileType = ".s6p";

#pragma region "File Name"
					//~XL add -260708
					//Current_UUT = site->CurrentUUT;
					//LotId = site->LotName;
					//l_TestCode = "TC";
				//	l_CustLotId = "CLID";
					//if (LotId != nullptr)
					//{
					//	LotId = site->LotName;
					//}
					//else
					//{
					//	LotId = "LotId";
					//}

					//if (Current_UUT != 0)
					//{
					//	Current_UUT = site->CurrentUUT;
					//}
					//else
					//{
					//	Current_UUT = 0;
					//}
					//if (IsRunningProduction(site) == true)
					//{
					//	//String ^ X = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["X"]->Value->ToString();
					//	//String ^ Y = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["Y"]->Value->ToString();
					//	//String ^ WaferID = site->ResultsByOffset[site->UUTOffsetResolver->UUTOffsets[vnaSiteIndex]]->CurrentResult->CustomFieldResults["WaferID"]->Value->ToString();
					//	l_testcode = site->FlowEngine->LotManager->LotTUI->PostNewLotInfo["TestCode"];
					//	l_CustLotId = site->FlowEngine->LotManager->LotTUI->PostNewLotInfo["CustLotID"];
					//}


					if (tl->glob->AWV.EnableRenameSnpData == true)
					{
						// Replace snpFolderPath with provided Macros
						snpFileName = tl->glob->AWV.snpFileName;
						for (int i = 0; i < macroArray->GetLength(0); i++) {
							snpFileName = snpFileName->Replace(macroArray[i, 0], macroArray[i, 1]);
						}
						snpFileName = snpFileName + snpFileType;
					//	snpFileName = strCurrent_UUT + "_" + snpFileName + "_" + LOTID + "_" + l_CustLotId + "_" + l_TestCode + snpFileType;
					}
					// Generic Filename Format
					else
					{
						// Sub-Lot Enabled (CP Test)
						if (site->DUTNumberMode == DUTNumberMode::Encoded)
						{
							snpFileName = tl->glob->tf.ProjectName + "_" + tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_S" + vnaSiteIndex.ToString() + "_" + LOTID + "_" + WaferID + "_X" + X + "_Y" + Y + "_" + tl->glob->TimeNow + snpFileType;
						}
						// Sub-Lot Disabled (FT Test)
						else
						{
							snpFileName = tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_" + tl->glob->currentSubItemName[vnaSiteIndex] + "_S" + vnaSiteIndex.ToString() + "_" + tl->glob->TimeNow + snpFileType;
						}
					}

					// Store every snpFileName into Ti_by_S2PFilename array
					if (tl->glob->tf.Ti_by_S2PFilename->ContainsKey(tl->glob->TestProperty[vnaSiteIndex].TestItemName) + "_S" + vnaSiteIndex)
					{
						tl->glob->tf.Ti_by_S2PFilename[(tl->glob->TestProperty[vnaSiteIndex].TestItemName) + "_S" + vnaSiteIndex] = snpFileName;
					}
					else
					{
						tl->glob->tf.Ti_by_S2PFilename->Add(tl->glob->TestProperty[vnaSiteIndex].TestItemName + "_S" + vnaSiteIndex, snpFileName);
					}
#pragma endregion

#pragma region "Folder Path"
					String ^ snpFolderPath = nullptr;

					if (tl->glob->AWV.S2Ppath == String::Empty)
					{
						snpFolderPath = FOLDER_CONST_AEM_TF3_RESULTS + "\\" + tl->glob->tf.ProfileName + "\\" + tl->glob->tf.ProjectName;
					}
					else
					{
						// Replace snpFolderPath with provided Macros
						snpFolderPath = tl->glob->AWV.S2Ppath;
						for (int i = 0; i < macroArray->GetLength(0); i++) {
							snpFolderPath = snpFolderPath->Replace(macroArray[i, 0], macroArray[i, 1]);
						}
						snpFolderPath = snpFolderPath;
					}

					//// Sub Lot Enabled
					//if (site->DUTNumberMode == DUTNumberMode::Encoded)
					//{
					//	// Modify snpFolderPath with LOTID and WaferID
					//	snpFolderPath += "\\" + LOTID + "\\" + WaferID;
					//}
					saveRecallSetting->touchstoneFolder = snpFolderPath;
#pragma endregion

					// Create snp Folder Path if not exist
					if (!(Directory::Exists(saveRecallSetting->touchstoneFolder)) && !(saveRecallSetting->touchstoneFolder == String::Empty))
					{
						Directory::CreateDirectory(saveRecallSetting->touchstoneFolder);
					}

					// Set snp full File Path
					saveRecallSetting->touchstoneFilePath = saveRecallSetting->touchstoneFolder + "\\" + snpFileName;

					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[KeysightVnaUtility -> SaveTraceDataToTouchstoneFile_Keysight] Save trace data to touchstone file.");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[KeysightVnaUtility -> SaveTraceDataToTouchstoneFile_Keysight] Save trace data to touchstone file.");
					ret = SaveTraceDataToTouchstoneFile_Keysight(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;
				}
			}
		}

	EndOfTest:
		return ret;
	}

	int AMB7300TestLibrary::VnaSwTime(int tfSite, int vnaSiteIndex, double % result)
	{
		int ret = 0;
		result	= (double)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaSwTime] Executing 'VnaSwTime' phase.");

		if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			if (vnaSwTimeTPC.measureType == Cond_MeasureType_DM_PXIE9852)
			{
				bool IsDone0 = false;
				int c_as = 0;

				try
				{
					if (!tl->glob->AWV.Offline)
					{
						pXIe9852_Info->aitask0->Start();

						ret = DmRunVector(tfSite, tfSite, vnaSwTimeTPC.dmModuleAlias, vnaSwTimeTPC.dmVectorTrigger);
						if (ret != 0)
						{
							pXIe9852_Info->aitask0->Stop();
							goto EndOfTest;
						}

						while (!IsDone0)
						{
							if (pXIe9852_Info->aitask0->AvailableSamples >= pXIe9852_Info->sampleNumber) IsDone0 = true;
							c_as++;
							if (c_as >= 10) IsDone0 = true;
						}

						if (c_as < 10)
						{
							pXIe9852_Info->aitask0->ReadData(pXIe9852_Info->readValue0, 5000 mS);

							// Separate Port 1 (readValueSite0)
							int sampleCount = 0;
							for (int i = 0; i < pXIe9852_Info->readValue0->Length; i += 2)
							{
								pXIe9852_Info->readValueSite0[sampleCount] = pXIe9852_Info->readValue0[i];
								sampleCount++;
							}

							if (sampleCount > 0)
							{
								// 1. Lookup Table (mV to dBm)
								cli::array<double, 2>^ lookupTable = gcnew cli::array<double, 2> {
									{ 354.00, 5.0 }, { 391.23,   0.0 }, { 406.25,  -1.0 },
									{ 420.41,  -2.0 }, { 436.27,  -3.0 }, { 454.83,  -4.0 },
									{ 464.60,  -4.5 }, { 474.12,  -5.0 }, { 581.18, -10.0 },
									{ 805.30, -20.0 }, { 1028.68, -30.0 }, { 1255.00, -40.0 },
									{ 1358.76, -45.0 }, { 1457.76, -50.0 }
								};
								int tableRows = lookupTable->GetLength(0);

								// 2. Convert Voltage to dBm Array
								cli::array<double>^ dbmArray = gcnew cli::array<double>(sampleCount);
								double max_dBm = -9999.0;
								double min_dBm = 9999.0;
								double sumVoltage = 0.0;

								for (int count = 0; count < sampleCount; count++)
								{
									double vol_mV = pXIe9852_Info->readValueSite0[count] * 1000.0;
									sumVoltage += pXIe9852_Info->readValueSite0[count];
									double curr_dBm = 0.0;

									if (vol_mV <= lookupTable[0, 0]) curr_dBm = lookupTable[0, 1];
									else if (vol_mV >= lookupTable[tableRows - 1, 0]) curr_dBm = lookupTable[tableRows - 1, 1];
									else
									{
										for (int r = 0; r < tableRows - 1; r++)
										{
											if (vol_mV >= lookupTable[r, 0] && vol_mV <= lookupTable[r + 1, 0])
											{
												double p0 = lookupTable[r, 1];
												double p1 = lookupTable[r + 1, 1];
												curr_dBm = p0 + (vol_mV - lookupTable[r, 0]) * (p1 - p0) / (lookupTable[r + 1, 0] - lookupTable[r, 0]);
												break;
											}
										}
									}
									dbmArray[count] = curr_dBm;
									if (curr_dBm > max_dBm) max_dBm = curr_dBm;
									if (curr_dBm < min_dBm) min_dBm = curr_dBm;
								}

								// 3. Process Requested Output Format
								if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_Time)
								{
									int edge = (vnaSwTimeTPC.vnaSwTime_TriggerEdge == Cond_AnalysisSetting_VnaSwTime_Rising) ? 1 : 0;
									double dummyInit = 0.0, dummySettle = 0.0;
									double triggerDelay = 0.0; // Update with real digitizer delay if available
									
									if (vnaSwTimeTPC.vnaSwTime_MovingAvgState == Cond_AnalysisSetting_VnaSwTime_MovingAvgOn)
									{
										result = CalculateSwTime_WithMA(dbmArray, edge, pXIe9852_Info->sampleRate, triggerDelay, dummyInit, dummySettle);
									}
									else if (vnaSwTimeTPC.vnaSwTime_MovingAvgState == Cond_AnalysisSetting_VnaSwTime_MovingAvgOff)
									{
										result = CalculateSwTime_NoMA(dbmArray, edge, pXIe9852_Info->sampleRate, triggerDelay, dummyInit, dummySettle);
									}
								}
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_MaxPower) result = max_dBm;
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_MinPower) result = min_dBm;
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_AvgPower)
								{
									// Average using the linear voltage
									double avgVoltagemV = (sumVoltage / sampleCount) * 1000.0;
									if (avgVoltagemV <= lookupTable[0, 0]) result = lookupTable[0, 1];
									else if (avgVoltagemV >= lookupTable[tableRows - 1, 0]) result = lookupTable[tableRows - 1, 1];
									else
									{
										for (int r = 0; r < tableRows - 1; r++)
										{
											if (avgVoltagemV >= lookupTable[r, 0] && avgVoltagemV <= lookupTable[r + 1, 0])
											{
												result = lookupTable[r, 1] + (avgVoltagemV - lookupTable[r, 0]) * (lookupTable[r + 1, 1] - lookupTable[r, 1]) / (lookupTable[r + 1, 0] - lookupTable[r, 0]);
												break;
											}
										}
									}
								}
							}
						}
						pXIe9852_Info->aitask0->Stop();
					}
					else
					{

						// Read the dummy debug file and extract CH1 samples.
						// File layout: line0=CH1, line1=CH2, line2=CH1, line3=CH2, ...
						String^ csvPath = Environment::GetFolderPath(Environment::SpecialFolder::UserProfile) + "\\Downloads\\DebugFile.csv";
						array<String^>^ allLines = File::ReadAllLines(csvPath);

						int sampleCount = allLines->Length / 2; // interleaved CH1/CH2 -> half are CH1
						pXIe9852_Info->readValueSite0 = gcnew array<double>(sampleCount);

						for (int i = 0; i < sampleCount; i++)
						{
							// index i*2 = CH1 sample, i*2+1 = CH2 (skipped, mirrors the online i+=2 extraction)
							pXIe9852_Info->readValueSite0[i] = Double::Parse(
								allLines[i * 2],
								System::Globalization::CultureInfo::InvariantCulture);
						}
							if (sampleCount > 0)
							{
								// 1. Lookup Table (mV to dBm)
								cli::array<double, 2>^ lookupTable = gcnew cli::array<double, 2> {
									{ 354.00, 5.0 }, { 391.23,   0.0 }, { 406.25,  -1.0 },
									{ 420.41,  -2.0 }, { 436.27,  -3.0 }, { 454.83,  -4.0 },
									{ 464.60,  -4.5 }, { 474.12,  -5.0 }, { 581.18, -10.0 },
									{ 805.30, -20.0 }, { 1028.68, -30.0 }, { 1255.00, -40.0 },
									{ 1358.76, -45.0 }, { 1457.76, -50.0 }
								};
								int tableRows = lookupTable->GetLength(0);

								// 2. Convert Voltage to dBm Array
								cli::array<double>^ dbmArray = gcnew cli::array<double>(sampleCount);
								double max_dBm = -9999.0;
								double min_dBm = 9999.0;
								double sumVoltage = 0.0;

								for (int count = 0; count < sampleCount; count++)
								{
									double vol_mV = pXIe9852_Info->readValueSite0[count] * 1000.0;
									sumVoltage += pXIe9852_Info->readValueSite0[count];
									double curr_dBm = 0.0;

									if (vol_mV <= lookupTable[0, 0]) curr_dBm = lookupTable[0, 1];
									else if (vol_mV >= lookupTable[tableRows - 1, 0]) curr_dBm = lookupTable[tableRows - 1, 1];
									else
									{
										for (int r = 0; r < tableRows - 1; r++)
										{
											if (vol_mV >= lookupTable[r, 0] && vol_mV <= lookupTable[r + 1, 0])
											{
												double p0 = lookupTable[r, 1];
												double p1 = lookupTable[r + 1, 1];
												curr_dBm = p0 + (vol_mV - lookupTable[r, 0]) * (p1 - p0) / (lookupTable[r + 1, 0] - lookupTable[r, 0]);
												break;
											}
										}
									}
									dbmArray[count] = curr_dBm;
									if (curr_dBm > max_dBm) max_dBm = curr_dBm;
									if (curr_dBm < min_dBm) min_dBm = curr_dBm;
								}

								// 3. Process Requested Output Format
								if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_Time)
								{
									int edge = (vnaSwTimeTPC.vnaSwTime_TriggerEdge == Cond_AnalysisSetting_VnaSwTime_Rising) ? 1 : 0;
									double dummyInit = 0.0, dummySettle = 0.0;
									double triggerDelay = 0.0; // Update with real digitizer delay if available
									double dummy_sampleRate = 1 MHz;
									if (vnaSwTimeTPC.vnaSwTime_MovingAvgState == Cond_AnalysisSetting_VnaSwTime_MovingAvgOn)
									{
										result = CalculateSwTime_WithMA(dbmArray, edge, dummy_sampleRate, triggerDelay, dummyInit, dummySettle);
									}
									else if (vnaSwTimeTPC.vnaSwTime_MovingAvgState == Cond_AnalysisSetting_VnaSwTime_MovingAvgOff)
									{
										result = CalculateSwTime_NoMA(dbmArray, edge, dummy_sampleRate, triggerDelay, dummyInit, dummySettle);
									}
								}
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_MaxPower) result = max_dBm;
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_MinPower) result = min_dBm;
								else if (vnaSwTimeTPC.outputFormat == Cond_OutputFormat_VnaSwTime_AvgPower)
								{
									// Average using the linear voltage
									double avgVoltagemV = (sumVoltage / sampleCount) * 1000.0;
									if (avgVoltagemV <= lookupTable[0, 0]) result = lookupTable[0, 1];
									else if (avgVoltagemV >= lookupTable[tableRows - 1, 0]) result = lookupTable[tableRows - 1, 1];
									else
									{
										for (int r = 0; r < tableRows - 1; r++)
										{
											if (avgVoltagemV >= lookupTable[r, 0] && avgVoltagemV <= lookupTable[r + 1, 0])
											{
												result = lookupTable[r, 1] + (avgVoltagemV - lookupTable[r, 0]) * (lookupTable[r + 1, 1] - lookupTable[r, 1]) / (lookupTable[r + 1, 0] - lookupTable[r, 0]);
												break;
											}
										}
									}
								}
							}
					}
				}
				catch (Exception^ ex)
				{
					if (pXIe9852_Info->aitask0 != nullptr) pXIe9852_Info->aitask0->Stop();
					throw gcnew Aemulus::Hardware::AlarmException ("VnaSwTime test error", ex);
				}
			}
		}

	EndOfTest:
		return ret;
	}
	double AMB7300TestLibrary::CalculateSwTime_WithMA(cli::array<double>^ dbmArray, int rise_h_fall_l, double sampleRate, double digitizerTriggerDelay, double %powerInit, double %powerSettle)
	{
		double timing = CONST_INVALID_RESULT;
		int sampleNum = dbmArray->Length;

		cli::array<double>^ pData_Watt = gcnew cli::array<double>(sampleNum);
		cli::array<double>^ pData_MA = gcnew cli::array<double>(sampleNum);

		// Convert dBm directly to Watts for smoothing
		for (int i = 0; i < sampleNum; i++)
		{
			pData_Watt[i] = 1e-3 * Math::Pow(10, dbmArray[i] / 10.0);
		}

		// Moving Average logic
		int factor = 20; 
		int delta = 0;

		for (int i = 0; i < sampleNum; i++)
		{
			if (i < factor / 2)
			{
				delta = factor / 2 - i;

				if (i != 0) 
				{
					for (int idx = i - 1; idx >= i - (factor / 2) + delta; idx--) 
					{
						pData_MA[i] += pData_Watt[idx];
					}
				}

				for (int idx = i + 1; idx <= i + factor / 2 + delta; idx++) 
				{
					pData_MA[i] += pData_Watt[idx];
				}

				pData_MA[i] = (pData_MA[i] + pData_Watt[i]) / (factor + 1);
			}
			else if (i + factor / 2 > sampleNum - 1)
			{
				delta = factor / 2 + i - (sampleNum - 1);

				for (int idx = i - 1; idx >= i - (factor / 2) - delta; idx--) 
				{
					pData_MA[i] += pData_Watt[idx];
				}

				if (i != sampleNum - 1) 
				{
					for (int idx = i + 1; idx <= i + factor / 2 - delta; idx++) 
					{
						pData_MA[i] += pData_Watt[idx];
					}
				}

				pData_MA[i] = (pData_MA[i] + pData_Watt[i]) / (factor + 1);
			}
			else
			{
				for (int idx = i - 1; idx >= i - factor / 2; idx--) 
				{
					pData_MA[i] += pData_Watt[idx];
				}

				for (int idx = i + 1; idx <= i + factor / 2; idx++) 
				{
					pData_MA[i] += pData_Watt[idx];
				}

				pData_MA[i] = (pData_MA[i] + pData_Watt[i]) / (factor + 1);
			}
		}

		// Get initiate power and settled power
		int pCount = 0;
		double pInit = 0.0;
		double pSettle = 0.0;
		double pDynamicRange = 0.0;
		double power_at_threshold = 0.0;
		double pStart = 0.0;

		if (rise_h_fall_l == 0) // Falling edge
		{
			for (int i = 0; i < 100 && i < sampleNum; i++) 
			{ 
				pInit += pData_MA[i]; 
				pCount++; 
			}

			pInit /= pCount; 
			pCount = 0;

			for (int i = sampleNum - 1; i > sampleNum - 101 && i >= 0; i--) 
			{ 
				pSettle += pData_MA[i]; 
				pCount++; 
			}

			pSettle /= pCount;

			pDynamicRange = pInit - pSettle;
			pStart = pInit - (0.1 * pDynamicRange);
			power_at_threshold = pSettle + (0.1 * pDynamicRange);
		}
		else // Rising edge
		{
			for (int i = 0; i < 30 && i < sampleNum; i++) 
			{ 
				pInit += pData_MA[i]; 
				pCount++; 
			}

			pInit /= pCount; 
			pCount = 0;

			for (int i = sampleNum - 1; i > sampleNum - 601 && i >= 0; i--) 
			{ 
				pSettle += pData_MA[i]; 
				pCount++; 
			}

			pSettle /= pCount;

			pDynamicRange = pSettle - pInit;
			pStart = pInit; 
			power_at_threshold = pSettle - (0.1 * pDynamicRange);
		}

		powerInit = pInit;
		powerSettle = pSettle;

		// Threshold evaluation
		int iStart = 0;
		int count = 0;
		int resultCounter = 0;

		if (rise_h_fall_l == 1) // Rising
		{
			int pStartConsecutiveFound = 3;
			int pStartConsecutiveCount = 0;
			int consecutiveFound = 1;

			for (int i = 0; i < sampleNum; i++)
			{
				if (pData_MA[i] > pStart) 
				{
					pStartConsecutiveCount++;

					if (pStartConsecutiveCount == pStartConsecutiveFound) 
					{ 
						iStart = i - pStartConsecutiveCount; 
						break; 
					}
				} 
				else 
				{
					pStartConsecutiveCount = 0;
				}
			}

			for (int i = 0; i < sampleNum; i++)
			{
				if (pData_MA[i] > power_at_threshold) 
				{
					count++;
					resultCounter = Math::Max(resultCounter, count);

					if (resultCounter >= consecutiveFound) 
					{
						timing = ((i - iStart) - consecutiveFound) * (1.0 / sampleRate) + digitizerTriggerDelay;
						break;
					}
				} 
				else 
				{
					count = 0;
				}
			}
		}
		else // Falling
		{
			int pStartConsecutiveFound = 3;
			int pStartConsecutiveCount = 0;
			int consecutiveFound = 3;

			for (int i = sampleNum - 1; i > 1; i--)
			{
				if (pData_MA[i] > pStart) 
				{
					pStartConsecutiveCount++;

					if (pStartConsecutiveCount == pStartConsecutiveFound) 
					{ 
						iStart = i + pStartConsecutiveCount; 
						break; 
					}
				} 
				else 
				{
					pStartConsecutiveCount = 0;
				}
			}

			for (int i = sampleNum - 1; i > 1; i--)
			{
				if (pData_MA[i] > power_at_threshold) 
				{
					count++;
					resultCounter = Math::Max(resultCounter, count);

					if (resultCounter >= consecutiveFound) 
					{
						timing = ((i + consecutiveFound) - iStart) * (1.0 / sampleRate) + digitizerTriggerDelay;
						break;
					}
				} 
				else 
				{
					count = 0;
				}
			}
		}

		return timing;
	}
	double AMB7300TestLibrary::CalculateSwTime_NoMA(cli::array<double>^ dbmArray, int rise_h_fall_l, double sampleRate, double digitizerTriggerDelay, double %powerInit, double %powerSettle)
	{
		double timing = CONST_INVALID_RESULT;
		int sampleNum = dbmArray->Length;
		cli::array<double>^ pData_Watt = gcnew cli::array<double>(sampleNum);

		// Convert dBm directly to Watts (NO moving average applied)
		for (int i = 0; i < sampleNum; i++)
		{
			pData_Watt[i] = 1e-3 * Math::Pow(10, dbmArray[i] / 10.0);
		}

		// Get initiate power and settled power straight from raw Watts
		int pCount = 0;
		double pInit = 0.0;
		double pSettle = 0.0;
		double pDynamicRange = 0.0;
		double power_at_threshold = 0.0;
		double pStart = 0.0;

		if (rise_h_fall_l == 0) // Falling edge
		{
			for (int i = 0; i < 100 && i < sampleNum; i++) 
			{ 
				pInit += pData_Watt[i]; 
				pCount++; 
			}

			pInit /= pCount; 
			pCount = 0;

			for (int i = sampleNum - 1; i > sampleNum - 101 && i >= 0; i--) 
			{ 
				pSettle += pData_Watt[i]; 
				pCount++; 
			}

			pSettle /= pCount;

			pDynamicRange = pInit - pSettle;
			pStart = pInit - (0.1 * pDynamicRange);
			power_at_threshold = pSettle + (0.1 * pDynamicRange);
		}
		else // Rising edge
		{
			for (int i = 0; i < 30 && i < sampleNum; i++) 
			{ 
				pInit += pData_Watt[i]; 
				pCount++; 
			}

			pInit /= pCount; 
			pCount = 0;

			for (int i = sampleNum - 1; i > sampleNum - 601 && i >= 0; i--) 
			{ 
				pSettle += pData_Watt[i]; 
				pCount++; 
			}

			pSettle /= pCount;

			pDynamicRange = pSettle - pInit;
			pStart = pInit; 
			power_at_threshold = pSettle - (0.1 * pDynamicRange);
		}

		powerInit = pInit;
		powerSettle = pSettle;

		// Threshold evaluation directly on pData_Watt
		int iStart = 0;
		int count = 0;
		int resultCounter = 0;

		if (rise_h_fall_l == 1) // Rising
		{
			int pStartConsecutiveFound = 3;
			int pStartConsecutiveCount = 0;
			int consecutiveFound = 1;

			for (int i = 0; i < sampleNum; i++)
			{
				if (pData_Watt[i] > pStart) 
				{
					pStartConsecutiveCount++;

					if (pStartConsecutiveCount == pStartConsecutiveFound) 
					{ 
						iStart = i - pStartConsecutiveCount; 
						break; 
					}
				} 
				else 
				{
					pStartConsecutiveCount = 0;
				}
			}

			for (int i = 0; i < sampleNum; i++)
			{
				if (pData_Watt[i] > power_at_threshold) 
				{
					count++;
					resultCounter = Math::Max(resultCounter, count);

					if (resultCounter >= consecutiveFound) 
					{
						timing = ((i - iStart) - consecutiveFound) * (1.0 / sampleRate) + digitizerTriggerDelay;
						break;
					}
				} 
				else 
				{
					count = 0;
				}
			}
		}
		else // Falling
		{
			int pStartConsecutiveFound = 3;
			int pStartConsecutiveCount = 0;
			int consecutiveFound = 3;

			for (int i = sampleNum - 1; i > 1; i--)
			{
				if (pData_Watt[i] > pStart) 
				{
					pStartConsecutiveCount++;

					if (pStartConsecutiveCount == pStartConsecutiveFound) 
					{ 
						iStart = i + pStartConsecutiveCount; 
						break; 
					}
				} 
				else 
				{
					pStartConsecutiveCount = 0;
				}
			}

			for (int i = sampleNum - 1; i > 1; i--)
			{
				if (pData_Watt[i] > power_at_threshold) 
				{
					count++;
					resultCounter = Math::Max(resultCounter, count);

					if (resultCounter >= consecutiveFound) 
					{
						timing = ((i + consecutiveFound) - iStart) * (1.0 / sampleRate) + digitizerTriggerDelay;
						break;
					}
				} 
				else 
				{
					count = 0;
				}
			}
		}

		return timing;
	}

	int AMB7300TestLibrary::VnaFetch_TrueParallel(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		** VnaFetch
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'VnaFetch' phase.
		******************************************************************************************************/
		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch] Executing 'VnaFetch' phase.");
		tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch] Executing 'VnaFetch' phase.");

		// Local variable
		int ret = 0;

		int first_active_site = 0;

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
			{
				break;
			}
			else {
				first_active_site++;
			}
		}

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Configure MPEX port pair switching.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Configure MPEX port pair switching.");
			// Configure MPEX port pair switching
			ret = ConfigureMpexPortPairSwitching_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			// Get current vna fetch active channel for generic usage
			ret = GetCurrentVnaFetchChannel(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureTraces_CMT] Configure trace setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureTraces_CMT] Configure trace setting at the active channel.");
			// Configure trace settings at the active channel
			ret = ConfigureTraces_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureAveraging_CMT] Configure averaging setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureAveraging_CMT] Configure averaging setting at the active channel.");
			// If required to apply averaging at the active channel
			ret = ConfigureAveraging_CMT(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			// Configure active channel + single trigger at the active channel
			if ((segmentSetting[first_active_site].sweepType == Vna_Stimulus_SweepType_LinearFreq) || 
				(segmentSetting[first_active_site].sweepType == Vna_Stimulus_SweepType_Segment))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Configure active channel, arm and wait for data.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Configure active channel, arm and wait for data.");
				ret = ConfigureActiveChannelAndSingleTrigger_CMT(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[first_active_site].sweepType == Vna_SweepType_GetFromStateFile) || 
				(segmentSetting[first_active_site].sweepType == Vna_SharedMemoryTransfer))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Configure single trigger for GetFromStateFile type.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Configure single trigger for GetFromStateFile type.");
				ret = ConfigureSingleTriggerForGetFromStateFileType_CMT(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			// [Future Enchancement] API not support. 
			// There is A5, A4 MPEX setup for Keysight VNA.
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Configure MPEX port pair switching.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Configure MPEX port pair switching.");
			// Configure MPEX port pair switching
			ret = ConfigureMpexPortPairSwitching_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> GetCurrentVnaFetchChannel] Get VNA active channel (Current VnaFetch).");
			// Get current vna fetch active channel for generic usage
			ret = GetCurrentVnaFetchChannel(tfSite, vnaSiteIndex); //Do Nothing here for SMT
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureTraces_Keysight] Configure trace setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureTraces_Keysight] Configure trace setting at the active channel.");
			// Configure trace settings at the active channel
			ret = ConfigureTraces_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;

			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureAveraging_Keysight] Configure averaging setting at the active channel.");
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureAveraging_Keysight] Configure averaging setting at the active channel.");
			// If required to apply averaging at the active channel
			ret = ConfigureAveraging_Keysight(tfSite, vnaSiteIndex);
			if (ret != 0) goto EndOfTest;
			
			// Configure active channel + single trigger at the active channel
			if ((segmentSetting[first_active_site].sweepType == Vna_Stimulus_SweepType_LinearFreq)	|| 
				(segmentSetting[first_active_site].sweepType == Vna_Stimulus_SweepType_Segment))
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Configure active channel, arm and wait for data.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Configure active channel, arm and wait for data.");
				ret = ConfigureActiveChannelAndSingleTrigger_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[first_active_site].sweepType == Vna_SweepType_GetFromStateFile) || 
				(segmentSetting[first_active_site].sweepType == Vna_Stimulus_SweepType_PowerSweep)) // Under development, not support yet
			{
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Configure single trigger for GetFromStateFile type.");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Configure single trigger for GetFromStateFile type.");
				ret = ConfigureSingleTriggerForGetFromStateFileType_Keysight(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if ((segmentSetting[first_active_site].sweepType == Vna_SharedMemoryTransfer) || 
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer_PowerSweep)) // Under development, not support yet
			{
				//SharedMemoryTransfer
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_Initialise] . Initialisation for MemoryMap");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_Initialise] . Initialisation for Memory Map");
				ret = SharedMemoryTransfer_Initialise(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;

				//SharedMemoryTransfer
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_FetchData] . Fetch Data from Memory Map");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, INFO, "[VnaFetch -> KeysightVnaUtility -> SharedMemoryTransfer_FetchData] . Fetch Data from Memory Map");
				ret = SharedMemoryTransfer_FetchData_TrueParallel(tfSite, vnaSiteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::VnaDataAnalysis_TrueParallel(int tfSite, int siteIndex, double % result)
	{
		/*****************************************************************************************************
		** VnaDataAnalysis
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**		result			- Return test result.
		**
		** Descriptions:
		**		This is a function to execute 'VnaDataAnalysis' phase.
		******************************************************************************************************/

		/*	Info
		**	LogMag		--> SFP 1x parameter	--> API return as real raw
		**	SWR			--> SFP 1x parameter	--> API return as real raw
		**	Phase		--> SFP 1x parameter	--> API return as real raw
		**	ExPhase		--> SFP 1x parameter	--> API return as real raw
		**	Delay		--> SFP 1x parameter	--> API return as real raw
		**	LinMag		--> SFP 1x parameter	--> API return as real raw
		**	Reak		--> SFP 1x parameter	--> API return as real raw
		**	Imag		--> SFP 1x parameter	--> API return as real raw
		**	SmithLog	--> SFP 2x parameter	--> [wrapper special handle] API return: 1st para as real raw | 2nd para as imag raw
		**	SmithLin	--> SFP 2x parameter	--> [wrapper special handle] (error)return wrong												!
		**	SmithReIm	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw
		**	SmithR+JX	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw | (error)does not have 3rd para		!
		**	SmithG+JB	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw | (error)does not have 3rd para		!
		**	PolarLog	--> SFP 2x parameter	--> [wrapper special handle] API return: 1st para as real raw | 2nd para as imag raw
		**	PolarLin	--> SFP 2x parameter	--> [wrapper special handle] (error)return wrong												!
		**	PolarReIm	--> SFP 2x parameter	--> API return: 1st para as real raw | 2nd para as imag raw
		*/

		// Local variable
		int ret = 0;
		result = (double)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis] Executing 'VnaDataAnalysis' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis] Executing 'VnaDataAnalysis' phase.");

		if (sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Configure smoothing setting at the active trace.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Configure smoothing setting at the active trace.");
			// If required to apply smoothing at the active trace
			ret = ConfigureSmoothing_CMT(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;

			// Execute data analysis function
			if (vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData_CMT] Get trace format data.");
					// Get trace format data
					ret = GetTraceFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					result = ProcessVnaMeasurement(tfSite, siteIndex);

					// If target test freq does not exists in the segment range
					if (result == (double)CONST_INVALID_RESULT)
					{
						result = (double)CONST_INVALID_RESULT;
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetTraceFormatData] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMax) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msPeak) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msTarget) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget))
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Get marker search format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Get marker search format data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math statistics format data
					ret = GetMarkerMathFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmStatistics_Mean->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_Mean)
						{
							result = mmFormatData->mmStatistics_Mean[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_StdDev)
						{
							result = mmFormatData->mmStatistics_StdDev[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_PeakToPeak)
						{
							result = mmFormatData->mmStatistics_PeakToPeak[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				// Get target marker math flatness format data
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Get marker math format data.");
					// Get marker math flatness format data
					ret = GetMarkerMathFormatData_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmFlatness_Gain->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Gain)
						{
							result = mmFormatData->mmFlatness_Gain[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Slope)
						{
							result = mmFormatData->mmFlatness_Slope[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Flat)
						{
							result = mmFormatData->mmFlatness_Flat[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple) ||
				(vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple))
			{
				// Get msMin & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Get marker search min max ripple data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Get marker search min max ripple data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchMinMaxRipple_CMT(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// Fix inside the GetMarkerSearchMinMaxRipple() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
					}

					// Get min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
		}
		else if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> ConfigureSmoothing_Keysight] Configure smoothing setting at the active trace.");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> ConfigureSmoothing_Keysight] Configure smoothing setting at the active trace.");
			// If required to apply smoothing at the active trace
			ret = ConfigureSmoothing_Keysight(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;

			// Execute data analysis function
			if (vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Get trace format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Get trace format data.");
					// Get trace format data
					ret = GetTraceFormatData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					result = ProcessVnaMeasurement(tfSite, siteIndex);

					// If user target test freq does not exists in the segment range / trace freq
					if (result == (double)CONST_INVALID_RESULT)
					{
						result = (double)CONST_INVALID_RESULT;
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[VnaDataAnalysis -> KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMax) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msPeak) || 
				(vnaDataAnalysisTPC.function == Cond_Function_msTarget) || 
				(vnaDataAnalysisTPC.function == Cond_Function_P1dBTarget))
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Get marker search format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Get marker search format data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchFormatData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = msFormatData->msReal[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = msFormatData->msImaginary[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = msFormatData->msFreq[siteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math statistics format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmStatistics_Mean->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_Mean)
						{
							result = mmFormatData->mmStatistics_Mean[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_StdDev)
						{
							result = mmFormatData->mmStatistics_StdDev[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmStatistics_PeakToPeak)
						{
							result = mmFormatData->mmStatistics_PeakToPeak[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch) || (vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs))
			{
				// Get target marker math bandwidth search format data
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math bandwidth search format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmBwSearch_BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = mmFormatData->mmBwSearch_BW[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = mmFormatData->mmBwSearch_Cent[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = mmFormatData->mmBwSearch_Low[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = mmFormatData->mmBwSearch_High[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = mmFormatData->mmBwSearch_Q[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = mmFormatData->mmBwSearch_Loss[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == "Cond_Function_mmFlatness_API_NotSupported_For_KeysightVna")
			{
				// Get target marker math flatness format data
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Get marker math format data.");
					// Get marker math flatness format data
					ret = GetMarkerMathFormatData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < mmFormatData->mmFlatness_Gain->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Gain)
						{
							result = mmFormatData->mmFlatness_Gain[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Slope)
						{
							result = mmFormatData->mmFlatness_Slope[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmFlatness_Flat)
						{
							result = mmFormatData->mmFlatness_Flat[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple)
			{
				// Get msMin & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerSearchMinMaxRipple_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// [Info] Fix inside the GetMarkerSearchMinMaxRipple_() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < msFormatData->msFreq->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = msFormatData->msReal[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msReal[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = msFormatData->msImaginary[siteIndex, i];
								else if (i == 1)
									msMaxRealData = msFormatData->msImaginary[siteIndex, i];
							}
						}
					}

					// Get marker search min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_P1dB)
			{

				//vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] = Vna_Format_Reset;
				// Get msMin & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Get marker search min max ripple data.");
					// Get marker search format data  (min max / peak / target)
					ret = GetMarkerCompressionSearchData_Keysight(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					
					if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
						(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_InputPower) 
						{
							result = msFormatData->msInputPower[siteIndex, 0];
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_OutputPower)
						{
							//result = msFormatData->msOutputGain[siteIndex, i] + msFormatData->msInputPower[siteIndex, i];
							result = msFormatData->msOutputPower[siteIndex, 0];
						}
					}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
					//{ // [Future Enchancement] wrapper return incorrect parameter.
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
					//	{
					//		;// [Future Enchancement] wrapper do not return the last parameter.
					//	}
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
					//	{
					//		;// [Future Enchancement] wrapper do not return the last parameter.
					//	}
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
					//{ // [Future Enchancement] wrapper return incorrect parameter.
					//}
					//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
					//{
					//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msReal[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msReal[siteIndex, i];
					//	}
					//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
					//	{
					//		if (i == 0)
					//			msMinRealData = msFormatData->msImaginary[siteIndex, i];
					//		else if (i == 1)
					//			msMaxRealData = msFormatData->msImaginary[siteIndex, i];
					//	}
					//}
					

				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis_TrueParallel(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataTarget] Get Target data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataTarget] Get Target data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis_TrueParallel(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch ||
				vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataBWSearch] Get BWSearch data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryDataBWSearch Get BWSearch data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis_TrueParallel(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i <SharedMemFormatData->BW->Length; i++)
					{
						if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_BW)
						{
							result = SharedMemFormatData->BW[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Cent)
						{
							result = SharedMemFormatData->Cent[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Low)
						{
							result = SharedMemFormatData->Low[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_High)
						{
							result = SharedMemFormatData->High[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Q)
						{
							result = SharedMemFormatData->Q[siteIndex, i];
							break;
						}
						else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_mmBwSearch_Loss)
						{
							result = SharedMemFormatData->Loss[siteIndex, i];
							break;
						}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}

			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				// Get Min & msMax within a freq range, then calculate ripple (max-min)
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_MinMaxRipple] Shared Memory min max ripple data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_MinMaxRipple] Shared Memory search min max ripple data.");
					// SharedMemory search format data  (min max / peak / target)
					ret = SharedMemoryTransfer_MinMaxRipple_TrueParallel(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Local result for the calculation
					double msMinRealData = 0.0;
					double msMaxRealData = 0.0;
					// [Info] Fix inside the GetMarkerSearchMinMaxRipple_() function, the 1st marker is Min, the 2nd marker is Max

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
							{
								;// [Future Enchancement] wrapper do not return the last parameter.
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						{ // [Future Enchancement] wrapper return incorrect parameter.
						}
						else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						{
							if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
							else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
							{
								if (i == 0)
									msMinRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
								else if (i == 1)
									msMaxRealData = SharedMemFormatData->Result_MarkY[siteIndex, i];
							}
						}
					}

					// Get marker search min max ripple result
					result = msMaxRealData - msMinRealData;
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}

			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataP1dB)
			{
				if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis -> KeysightVnaUtility -> SharedMemoryTransfer_GetMinMax] Get MinMax data.");
					//SharedMemoryTransfer
					ret = SharedMemoryTransfer_DataAnalysis_TrueParallel(tfSite, siteIndex);
					if (ret != 0) goto EndOfTest;

					// Assign data
					for (int i = 0; i < SharedMemFormatData->Result_MarkY->Length; i++)
					{
						if ((vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::IMAGinary)))
						{
							if (vnaDataAnalysisTPC.outputType == Cond_OutputType_OutputPower)
							{
								result = SharedMemFormatData->Result_MarkY[siteIndex, i];
								break;
							}
							else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_InputPower)
							{
								result = SharedMemFormatData->Result_MarkX[siteIndex, i];
								break;
							}
						}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLOGarithmic))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SLINear))
						//{ // [Future Enchancement] wrapper return incorrect parameter.
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SCOMplex))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SMITh))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Resistance))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_CapInduc))
						//	{
						//		;// [Future Enchancement] wrapper do not return the last parameter.
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::SADMittance))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Conductance))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_CapInduc))
						//	{
						//		;// [Future Enchancement] wrapper do not return the last parameter.
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLOGarithmic))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::PLINear))
						//{ // [Future Enchancement] wrapper return incorrect parameter.
						//}
						//else if (vnaSetting[siteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] == int(VNATraceFormat::POLar))
						//{
						//	if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Real))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if ((vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Data) && (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag))
						//	{
						//		result = SharedMemFormatData->Result_MarkY[siteIndex, i];
						//		break;
						//	}
						//	else if (vnaDataAnalysisTPC.outputType == Cond_OutputType_Generic_Freq)
						//	{
						//		result = SharedMemFormatData->Result_MarkX[siteIndex, i];
						//		break;
						//	}
						//}
					}
				}
				else
				{
					result = (double)CONST_INVALID_RESULT;
				}

			}
		}

	EndOfTest:
		return ret;
	}
	

	// Log Detector & Digitizer (For switching time test)
	int AMB7300TestLibrary::InitializeExtModule(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeExtModule
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize external hardware.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;

		double g_SINAD;
		g_SINAD = 0.0;

		//tracerLogger->WriteToTracer(MainTracer, "JY9852 Init...");
		double sampleNumber_double;
		pXIe9852_Info = gcnew PXIe9852_Info();

		//pXIe9852_Info->sampleRate = (double)(tf_FlowStep_ConditionCast("sampleRate"));
		//pXIe9852_Info->sampleNumber_double = (double)(tf_FlowStep_ConditionCast("sampleNumber"));
		pXIe9852_Info->sampleNumber = (int)sampleNumber_double;

		pXIe9852_Info->readValue0 = gcnew array<double>(pXIe9852_Info->sampleNumber * 2);
		pXIe9852_Info->readValue1 = gcnew array<double>(pXIe9852_Info->sampleNumber * 2);

		pXIe9852_Info->readValueSite0 = gcnew array<double>(pXIe9852_Info->sampleNumber);
		pXIe9852_Info->readValueSite1 = gcnew array<double>(pXIe9852_Info->sampleNumber);
	
		pXIe9852_Info->Power_dBmSite0 = gcnew array<double>(pXIe9852_Info->sampleNumber / 2);
		pXIe9852_Info->Power_dBmSite1 = gcnew array<double>(pXIe9852_Info->sampleNumber / 2);
	
		pXIe9852_Info->Power_dBmSite0_Avg = gcnew array<double>(pXIe9852_Info->sampleNumber / 2);
		pXIe9852_Info->Power_dBmSite1_Avg = gcnew array<double>(pXIe9852_Info->sampleNumber / 2);
	
		pXIe9852_Info->slotNo = gcnew array<int>(1);//2

		//pXIe9852_Info->slotNo[0] = (int)(tf_FlowStep_ConditionCast("JY69852"));

		double Vrange = 2;

		//	for (int testSite = testSiteStart; testSite < totalSite; testSite++)
		{
			if (!tl->glob->AWV.Offline)
			{
				//if (testSite == 0) //0
			{

				pXIe9852_Info->aitask0 = gcnew JYPXIe69852AITask(pXIe9852_Info->slotNo[0]);
				pXIe9852_Info->aitask0->AddChannel(0, -Vrange, Vrange, AICoupling::DC, AIImpedance::ImpedanceHigh);//Impedance50Ohm
				pXIe9852_Info->aitask0->AddChannel(1, -Vrange, Vrange, AICoupling::DC, AIImpedance::ImpedanceHigh);//JYPXIe69852::AIImpedance::ImpedanceHigh
				pXIe9852_Info->aitask0->Mode = AIMode::Finite;//SFP: Sample Mode
				pXIe9852_Info->aitask0->SampleRate = pXIe9852_Info->sampleRate;
				pXIe9852_Info->aitask0->SamplesToAcquire = pXIe9852_Info->sampleNumber;
				pXIe9852_Info->aitask0->Trigger->Mode = AITriggerMode::Start;//SFP:Trigger Mode???
				pXIe9852_Info->aitask0->Trigger->Type = AITriggerType::Software;//SFP:Trigger Source
				pXIe9852_Info->aitask0->Trigger->ReTriggerCount = -1;//SFP:Re-Trigger/Re-Trigger Count
				pXIe9852_Info->aitask0->Trigger->ReTriggerFrequency = 1;
				pXIe9852_Info->aitask0->Start();
				pXIe9852_Info->aitask0->SendSoftwareTrigger();

				bool IsDone0 = false;
				while (!IsDone0)
				{
					//util->WaitSecond(1 mS);

					if (pXIe9852_Info->aitask0->AvailableSamples >= pXIe9852_Info->sampleNumber)
					{
						IsDone0 = true;
					}
				}

				pXIe9852_Info->aitask0->ReadData(pXIe9852_Info->readValue0, 5000 mS);//-1

				int j = 0;

				for (int i = 0; i < pXIe9852_Info->readValue0->Length; i++)
				{
					if (i % 2 == 0)
					{
						pXIe9852_Info->readValueSite0[j] = pXIe9852_Info->readValue0[i]; //readValueSite0
					}
					else
					{
						pXIe9852_Info->readValueSite1[j] = pXIe9852_Info->readValue0[i]; //readValueSite1
						j++;
					}
				}

				//if (debug)
				{
					//PrintWaveformToFile(testSite, "digitizer_V_load", readValueSite0);
				}

				pXIe9852_Info->aitask0->Stop();

				pXIe9852_Info->aitask0->Trigger->Mode = AITriggerMode::Start;//SFP:Trigger Mode???
				pXIe9852_Info->aitask0->Trigger->Type = AITriggerType::Digital;// use external trigger 
				pXIe9852_Info->aitask0->Trigger->Digital->Edge = AIDigitalTriggerEdge::Rising;
				pXIe9852_Info->aitask0->Trigger->Digital->Source = AIDigitalTriggerSource::TRGIN;

				pXIe9852_Info->aitask0->Trigger->ReTriggerCount = 0;//SFP:Re-Trigger/Re-Trigger Count  -1
				pXIe9852_Info->aitask0->Trigger->ReTriggerFrequency = 0;
				pXIe9852_Info->aitask0->Trigger->PreTriggerSamples = 100;
				//	aitask0->Start();
			}
			}
		}
	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeExtModule(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeExtModule
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize external hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		if (pXIe9852_Info->aitask0 != nullptr)
		{
			pXIe9852_Info->aitask0->Stop();
			//pXIe9852_Info->aitask0->RemoveChannel(-1);

		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	VnaDataAnalysis -> Trace Data Helper functions
	**	----------------------------------------------------------------------------------------------------
	*/
	double AMB7300TestLibrary::GetMeasurementData(int tfSite, int vnaSiteIndex, int i, String^ measType, String^ outputFormat) {
		if (outputFormat == Cond_OutputFormat_Generic_Real ||
			outputFormat == Cond_OutputFormat_SmithRJX_Resistance ||
			outputFormat == Cond_OutputFormat_SmithGJB_Conductance) {
			return measType == Vna_Measurement_Type_S11 ? traceFormatData->S11Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S12 ? traceFormatData->S12Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S13 ? traceFormatData->S13Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S14 ? traceFormatData->S13Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S15 ? traceFormatData->S13Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S16 ? traceFormatData->S13Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S21 ? traceFormatData->S21Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S22 ? traceFormatData->S22Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S23 ? traceFormatData->S23Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S24 ? traceFormatData->S24Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S25 ? traceFormatData->S25Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S26 ? traceFormatData->S26Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S31 ? traceFormatData->S31Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S32 ? traceFormatData->S32Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S33 ? traceFormatData->S33Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S34 ? traceFormatData->S34Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S35 ? traceFormatData->S35Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S36 ? traceFormatData->S36Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S41 ? traceFormatData->S41Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S42 ? traceFormatData->S42Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S43 ? traceFormatData->S43Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S44 ? traceFormatData->S44Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S45 ? traceFormatData->S45Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S46 ? traceFormatData->S46Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S51 ? traceFormatData->S51Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S52 ? traceFormatData->S52Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S53 ? traceFormatData->S53Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S54 ? traceFormatData->S54Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S55 ? traceFormatData->S55Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S56 ? traceFormatData->S56Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S61 ? traceFormatData->S61Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S62 ? traceFormatData->S62Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S63 ? traceFormatData->S63Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S64 ? traceFormatData->S64Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S65 ? traceFormatData->S65Real[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S66 ? traceFormatData->S66Real[vnaSiteIndex, i] : (double)CONST_INVALID_RESULT;
		}
		else if (outputFormat == Cond_OutputFormat_Generic_Imag ||
			outputFormat == Cond_OutputFormat_SmithRJX_Reactance ||
			outputFormat == Cond_OutputFormat_SmithGJB_Susceptance)
		{
			return measType == Vna_Measurement_Type_S11 ? traceFormatData->S11Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S12 ? traceFormatData->S12Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S13 ? traceFormatData->S13Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S14 ? traceFormatData->S14Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S15 ? traceFormatData->S15Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S16 ? traceFormatData->S16Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S21 ? traceFormatData->S21Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S22 ? traceFormatData->S22Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S23 ? traceFormatData->S23Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S24 ? traceFormatData->S24Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S25 ? traceFormatData->S25Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S26 ? traceFormatData->S26Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S31 ? traceFormatData->S31Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S32 ? traceFormatData->S32Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S33 ? traceFormatData->S33Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S34 ? traceFormatData->S34Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S35 ? traceFormatData->S35Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S36 ? traceFormatData->S36Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S41 ? traceFormatData->S41Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S42 ? traceFormatData->S42Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S43 ? traceFormatData->S43Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S44 ? traceFormatData->S44Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S45 ? traceFormatData->S45Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S46 ? traceFormatData->S46Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S51 ? traceFormatData->S51Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S52 ? traceFormatData->S52Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S53 ? traceFormatData->S53Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S54 ? traceFormatData->S54Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S55 ? traceFormatData->S55Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S56 ? traceFormatData->S56Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S61 ? traceFormatData->S61Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S62 ? traceFormatData->S62Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S63 ? traceFormatData->S63Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S64 ? traceFormatData->S64Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S65 ? traceFormatData->S65Imaginary[vnaSiteIndex, i] :
				measType == Vna_Measurement_Type_S66 ? traceFormatData->S66Imaginary[vnaSiteIndex, i] : (double)CONST_INVALID_RESULT;
		}
		else if (outputFormat == Cond_OutputType_Generic_Freq) {
			return traceFormatData->freqPoint[vnaSiteIndex, i];
		}
		else if (outputFormat == Cond_OutputFormat_SmithGJB_CapInduc) {
			return (double)CONST_INVALID_RESULT;
			// [Future Enchancement] wrapper do not return the last parameter.
		}
	}
	double AMB7300TestLibrary::ProcessVnaMeasurement(int tfSite, int vnaSiteIndex) {
		int traceIndex = vnaDataAnalysisTPC.traceIndex - 1;
		String^ measType = vnaSetting[vnaSiteIndex].measType[traceIndex];
		int traceFormat = vnaSetting[vnaSiteIndex].vna_traceFormat[traceIndex];
		double result;

		for (int i = 0; i < traceFormatData->totalFreqPoints; i++) {
			if (vnaDataAnalysisTPC.testFreq[0] == traceFormatData->freqPoint[vnaSiteIndex, i])
			{
				if (vnaDataAnalysisTPC.testFreq[0] == traceFormatData->freqPoint[vnaSiteIndex, i])
					switch (traceFormat) {
					case VNATraceFormat::MLOGarithmic:
					case VNATraceFormat::SWR:
					case VNATraceFormat::PHASe:
					case VNATraceFormat::UPHase:
					case VNATraceFormat::GDELay:
					case VNATraceFormat::MLINear:
					case VNATraceFormat::REAL:
					case VNATraceFormat::IMAGinary:
						if (vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_Generic_Imag ||
							vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithRJX_Reactance ||
							vnaDataAnalysisTPC.outputFormat == Cond_OutputFormat_SmithGJB_Susceptance)
						{
							// Invalid OutputFormat
							break;
						}
						else
						{
							result = GetMeasurementData(tfSite, vnaSiteIndex, i, measType, vnaDataAnalysisTPC.outputFormat);
						}
						break;
					case VNATraceFormat::SLOGarithmic:
					case VNATraceFormat::SCOMplex:
					case VNATraceFormat::SMITh:
					case VNATraceFormat::SADMittance:
					case VNATraceFormat::PLOGarithmic:
					case VNATraceFormat::POLar:
						result = GetMeasurementData(tfSite, vnaSiteIndex, i, measType, vnaDataAnalysisTPC.outputFormat);
						break;

					case VNATraceFormat::SLINear:
						// Future Enhancement placeholder
						break;
					case VNATraceFormat::PLINear:
						// Future Enhancement placeholder
						break;

					default:
						break;
					}
				break; // Exit loop once data is found
			}
		}
		return result;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Vna Operation Helper Function
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::GetCurrentVnaFetchChannel(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		** GetCurrentVnaFetchChannel
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to get the VNA channel / active channel that will perform the measurement.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		array<int> ^ vnaChannel = gcnew array<int>(1);
		vnaChannel[0] = 999;

		try
		{

			//if (sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A &&
			//	((amb7300SystemSetting->systemAlias = AMB7300_S4P1D_TYPE) || (amb7300SystemSetting->systemAlias = AMB7300_S6P1D_TYPE)))
			//{
			//	//Already got from condition at TestLib_ControlStep_VnaFetch_CastCondition.
			//	//Only supports for KeysightVNA
			//	//Because need to support case for MPEX config using single port pair + Multi Channel
			//}
			//else
			if (!vnaSetting[vnaSiteIndex].ChannelNumCondition_Exist)
			{
				// Get vna channel based on the selected port pair
				tl->CheckError(tfSite, amb7300_driver[vnaSiteIndex]->GetVNAChannel(vnaSetting[vnaSiteIndex].pinAlias, vnaChannel, vnaSetting[vnaSiteIndex].isReverseRequire));
				// Store for generic usage
				vnaSetting[vnaSiteIndex].currentVnaFetchChannel = vnaChannel[0];
			}

		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_VNA_ACTIVE_CHANNEL_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[VnaFetch -> GetCurrentVnaFetchChannel] Fail to get VNA active channel (Current VnaFetch)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[VnaFetch -> GetCurrentVnaFetchChannel] Fail to get VNA active channel (Current VnaFetch)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Math Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::MathFunction(int tfSite, int siteIndex, double % result)
	{
		// Local variable
		int ret = 0;
		result = (double)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[Math] Executing 'Math' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[Math] Executing 'Math' phase.");

		double resTP1 = (double)tl->glob->TestProperty[siteIndex].TestResults[MathTPC.mathTP1];
		double resTP2 = (double)tl->glob->TestProperty[siteIndex].TestResults[MathTPC.mathTP2];
		
		if (MathTPC.mathFunction == Cond_TestFunction_Add)
		{
			result = resTP1 + resTP2;
		}
		else if (MathTPC.mathFunction == Cond_TestFunction_Subtract)
		{
			result = resTP1 - resTP2;
		}
		else if (MathTPC.mathFunction == Cond_TestFunction_AbsAdd)
		{
			result = System::Math::Abs(resTP1 + resTP2);
		}
		else if (MathTPC.mathFunction == Cond_TestFunction_AbsSubtract)
		{
			result = System::Math::Abs(resTP1 - resTP2);
		}


	EndOfTest:
		if (ret != 0) result = (double)CONST_INVALID_RESULT;
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	CM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeCmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeCmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize CM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> CM400Utility -> InitializeCmSeriesResource] Initialize CM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> CM400Utility -> InitializeCmSeriesResource] Initialize CM series hardware.");

		// Initialize CM
		ret = InitializeCm(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tl->glob->tf.TestSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeCmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeCmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize CM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
	
		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> CM400Utility -> UninitializeCmSeriesResource] Uninitialize CM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> CM400Utility -> UninitializeCmSeriesResource] Uninitialize CM series hardware.");

		// Uninitialize CM
		ret = UninitializeCm(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	AM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeAmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeAmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize AM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> AM400Utility -> InitializeAmSeriesResource] Initialize AM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> AM400Utility -> InitializeAmSeriesResource] Initialize AM series hardware.");

		// Initialize AM
		ret = InitializeSmu(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tfSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;


	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeAmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeAmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize AM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> AM400Utility -> UninitializeAmSeriesResource] Uninitialize AM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> AM400Utility -> UninitializeAmSeriesResource] Uninitialize AM series hardware.");

		// Uninitialize AM
		ret = UninitializeSmu(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	DM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeDmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeDmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize DM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> DM400Utility -> InitializeDmSeriesResource] Initialize DM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> DM400Utility -> InitializeDmSeriesResource] Initialize DM series hardware.");

		// Initialize DM
		ret = InitializeDm(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tl->glob->tf.TestSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeDmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeDmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize DM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> DM400Utility -> UninitializeDmSeriesResource] Uninitialize DM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> DM400Utility -> UninitializeDmSeriesResource] Uninitialize DM series hardware.");

		// Uninitialize DM
		ret = UninitializeDm(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	IOM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeIomSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeIomSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize IOM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> IOM400Utility -> InitializeIomSeriesResource] Initialize IOM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> IOM400Utility -> InitializeIomSeriesResource] Initialize IOM series hardware.");

		// Initialize IOM
		ret = InitializeIom(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tl->glob->tf.TestSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeIomSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeIomSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize IOM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> IOM400Utility -> UninitializeIomSeriesResource] Uninitialize IOM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> IOM400Utility -> UninitializeIomSeriesResource] Uninitialize IOM series hardware.");

		// Uninitialize IOM
		ret = UninitializeIom(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	ACM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeAcmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeAcmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize ACM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> ACM400Utility -> InitializeAcmSeriesResource] Initialize ACM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> ACM400Utility -> InitializeAcmSeriesResource] Initialize ACM series hardware.");

		// Initialize ACM
		ret = InitializeAcm(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tl->glob->tf.TestSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeAcmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeAcmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize ACM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> ACM400Utility -> UninitializeAcmSeriesResource] Uninitialize ACM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> ACM400Utility -> UninitializeAcmSeriesResource] Uninitialize ACM series hardware.");

		// Uninitialize ACM
		ret = UninitializeAcm(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	TM400 Series Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeTmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** InitializeTmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to initialize TM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> TM400Utility -> InitializeTmSeriesResource] Initialize TM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeTester -> TM400Utility -> InitializeTmSeriesResource] Initialize TM series hardware.");

		// Initialize TM
		ret = InitializeTm(tl->glob->HardwareProfile, tl->glob->tf.TestHead, tl->glob->tf.TestSite, siteIndex, tl->glob->AWV.Offline);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeTmSeriesResource(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeTmSeriesResource
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to uninitialize TM series hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> TM400Utility -> UninitializeTmSeriesResource] Uninitialize TM series hardware.");
		tl->WriteToFileLogger(tfSite, tl->glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeTester -> TM400Utility -> UninitializeTmSeriesResource] Uninitialize TM series hardware.");

		// Uninitialize TM
		ret = UninitializeTm(tfSite, siteIndex);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	DC Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::DcControl(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** DcControl
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'DcControl' phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcControl] Executing 'DcControl' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcControl] Executing 'DcControl' phase.");

		for (int i = 0; i < DcControlCSC.module->Length; i++)
		{
			if (DcControlCSC.module[i] == Cond_Module_CM)
			{
				// [Future Enchancement] Reserved for CM resource.
			}
			else if (DcControlCSC.module[i] == Cond_Module_AM)
			{
				// Configure AM pin
				ret = ConfigurePinSmu(tfSite, siteIndex,	DcControlCSC.pinAlias[i],
															DcControlCSC.controlFunction[i], 
															DcControlCSC.nplc[i], 
															DcControlCSC.voltageLevel[i], 
															DcControlCSC.currentLevel[i], 
															DcControlCSC.delay[i]);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcControl -> AM400Utility -> ConfigurePinSmu] Module: "			+ "AM"												+ " | " + 
																												"PinAlias: "		+ DcControlCSC.pinAlias[i]							+ " | " +
																												"ControlFunction: "	+ DcControlCSC.controlFunction[i]					+ " | " +
																												"NPLC: "			+ DcControlCSC.nplc[i].ToString()					+ " | " +
																												"VoltageLevel: "	+ DcControlCSC.voltageLevel[i].ToString()	+ "V"	+ " | "	+
																												"CurrentLevel: "	+ DcControlCSC.currentLevel[i].ToString()	+ "A"	+ " | " +
																												"Delay: "			+ DcControlCSC.delay[i].ToString()			+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcControl -> AM400Utility -> ConfigurePinSmu] Module: "			+ "AM"												+ " | " + 
																												"PinAlias: "		+ DcControlCSC.pinAlias[i]							+ " | " +
																												"ControlFunction: "	+ DcControlCSC.controlFunction[i]					+ " | " +
																												"NPLC: "			+ DcControlCSC.nplc[i].ToString()					+ " | " +
																												"VoltageLevel: "	+ DcControlCSC.voltageLevel[i].ToString()	+ "V"	+ " | "	+
																												"CurrentLevel: "	+ DcControlCSC.currentLevel[i].ToString()	+ "A"	+ " | " +
																												"Delay: "			+ DcControlCSC.delay[i].ToString()			+ "sec");
			}
			else if (DcControlCSC.module[i] == Cond_Module_DM)
			{
				if ((DcControlCSC.controlFunction[i] == Cond_ControlFunction_DM_PMU_DVCI) || 
					(DcControlCSC.controlFunction[i] == Cond_ControlFunction_DM_PMU_DICV))
				{ 
					// Configure DM pin
					ret = ConfigurePinDm(tfSite, siteIndex,		DcControlCSC.pinAlias[i],
																DcControlCSC.controlFunction[i],
																DcControlCSC.nplc[i],
																DcControlCSC.voltageLevel[i],
																DcControlCSC.currentLevel[i],
																DcControlCSC.delay[i]);
					if (ret != 0) goto EndOfTest;
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcControl -> DM400Utility -> ConfigurePinDm] Module: "			+ "DM"												+ " | " + 
																													"PinAlias: "		+ DcControlCSC.pinAlias[i]							+ " | " +
																													"ControlFunction: "	+ DcControlCSC.controlFunction[i]					+ " | " +
																													"NPLC: "			+ DcControlCSC.nplc[i].ToString()					+ " | " +
																													"VoltageLevel: "	+ DcControlCSC.voltageLevel[i].ToString()	+ "V"	+ " | "	+
																													"CurrentLevel: "	+ DcControlCSC.currentLevel[i].ToString()	+ "A"	+ " | " +
																													"Delay: "			+ DcControlCSC.delay[i].ToString()			+ "sec");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcControl -> DM400Utility -> ConfigurePinDm] Module: "				+ "DM"												+ " | " +
																													"PinAlias: "		+ DcControlCSC.pinAlias[i]							+ " | " +
																													"ControlFunction: "	+ DcControlCSC.controlFunction[i]					+ " | " +
																													"NPLC: "			+ DcControlCSC.nplc[i].ToString()					+ " | " +
																													"VoltageLevel: "	+ DcControlCSC.voltageLevel[i].ToString()	+ "V"	+ " | "	+
																													"CurrentLevel: "	+ DcControlCSC.currentLevel[i].ToString()	+ "A"	+ " | " +
																													"Delay: "			+ DcControlCSC.delay[i].ToString()			+ "sec");
				}
				else if (DcControlCSC.controlFunction[i] == Cond_ControlFunction_DM_PE)
				{
					// Configure DM pin
					ret = ConfigurePinDm(tfSite, siteIndex,		DcControlCSC.pinAlias[i],
																DcControlCSC.controlFunction[i],
																DcControlCSC.nplc[i],
																DcControlCSC.voltageLevel[i],
																DcControlCSC.currentLevel[i],
																DcControlCSC.delay[i]);
					if (ret != 0) goto EndOfTest;
					tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcControl -> DM400Utility -> ConfigurePinDm] Module: "			+ "DM"															+ " | " + 
																													"PinAlias: "		+ DcControlCSC.pinAlias[i]										+ " | " +
																													"ControlFunction: "	+ DcControlCSC.controlFunction[i]								+ " | " +
																													"NPLC: "			+ "NA"															+ " | " +
																													"VoltageLevel: "	+ "NA"															+ " | "	+
																													"CurrentLevel: "	+ "NA"															+ " | " +
																													"Delay: "			+ DcControlCSC.delay[i].ToString()						+ "sec" + " | " +
																													"VIH: "				+ DcControlCSC.voltageLevel[i].ToString()				+ "V"	+ " | " +
																													"VIL: "				+ "0.0V"														+ " | " +
																													"VOH: "				+ ((DcControlCSC.voltageLevel[i]/2.0)-0.1).ToString()	+ "V"	+ " | " +
																													"VOL: "				+ ((DcControlCSC.voltageLevel[i]/2.0)-0.1).ToString()	+ "V"	+ " | " +
																													"IOH: "				+ "0.0A"														+ " | " +
																													"IOL: "				+ "0.0A"														+ " | " +
																													"VCH: "				+ "0.0V"														+ " | " +
																													"VCL: "				+ "0.0V"														+ " | " +
																													"VIH: "				+ (DcControlCSC.voltageLevel[i]/2.0).ToString()			+ "V");
					tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcControl -> DM400Utility -> ConfigurePinDm] Module: "				+ "DM"															+ " | " +
																													"PinAlias: "		+ DcControlCSC.pinAlias[i]										+ " | " +
																													"ControlFunction: "	+ DcControlCSC.controlFunction[i]								+ " | " +
																													"NPLC: "			+ "NA"															+ " | " +
																													"VoltageLevel: "	+ "NA"															+ " | "	+
																													"CurrentLevel: "	+ "NA"															+ " | " +
																													"Delay: "			+ DcControlCSC.delay[i].ToString()						+ "sec" + " | " +
																													"VIH: "				+ DcControlCSC.voltageLevel[i].ToString()				+ "V"	+ " | " +
																													"VIL: "				+ "0.0V"														+ " | " +
																													"VOH: "				+ ((DcControlCSC.voltageLevel[i]/2.0)-0.1).ToString()	+ "V"	+ " | " +
																													"VOL: "				+ ((DcControlCSC.voltageLevel[i]/2.0)-0.1).ToString()	+ "V"	+ " | " +
																													"IOH: "				+ "0.0A"														+ " | " +
																													"IOL: "				+ "0.0A"														+ " | " +
																													"VCH: "				+ "0.0V"														+ " | " +
																													"VCL: "				+ "0.0V"														+ " | " +
																													"VIH: "				+ (DcControlCSC.voltageLevel[i]/2.0).ToString()			+ "V");
				}
				else if (DcControlCSC.controlFunction[i] == Cond_ControlFunction_DM_DIO)
				{
					// [Future Enchancement] Configure DM pin to DIO mode.
				}
			}
			else if (DcControlCSC.module[i] == Cond_Module_IOM)
			{
				// [Future Enchancement] Reserved for IOM resource.
			}
			else if (DcControlCSC.module[i] == Cond_Module_ACM)
			{
				// [Future Enchancement] Reserved for ACM resource.
			}
			else if (DcControlCSC.module[i] == Cond_Module_TM)
			{
				// [Future Enchancement] Reserved for TM resource.
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DcTest(int tfSite, int siteIndex, double % result)
	{
		/*****************************************************************************************************
		** DcTest
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**		result		- Return test result.
		**
		** Descriptions:
		**		This is a function to execute 'DcTest' phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		result	= (double)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest] Executing 'DcTest' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest] Executing 'DcTest' phase.");

		if (DcTestTPC.module == Cond_Module_CM)
		{
			// [Future Enchancement] Reserved for CM resource.
		}
		else if (DcTestTPC.module == Cond_Module_AM)
		{
			if (DcTestTPC.testFunction == Cond_TestFunction_OS)
			{
				ret = SmuTestFunction_OS(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_OS] Module: "			+ "AM"								+ " | " + 
																												"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																												"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																												"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																												"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_OS] Module: "			+ "AM"								+ " | " + 
																												"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																												"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																												"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																												"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
			else if (DcTestTPC.testFunction == Cond_TestFunction_MeasureCurrent)
			{
				ret = SmuTestFunction_MeasureCurrent(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_MeasureCurrent] Module: "			+ "AM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_MeasureCurrent] Module: "			+ "AM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
			else if (DcTestTPC.testFunction == Cond_TestFunction_MeasureVoltage)
			{
				ret = SmuTestFunction_MeasureVoltage(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_MeasureVoltage] Module: "			+ "AM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> AM400Utility -> SmuTestFunction_MeasureVoltage] Module: "			+ "AM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
		}
		else if (DcTestTPC.module == Cond_Module_DM)
		{
			if (DcTestTPC.testFunction == Cond_TestFunction_OS)
			{
				ret = DmTestFunction_OS(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_OS] Module: "			+ "DM"								+ " | " + 
																												"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																												"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																												"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																												"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_OS] Module: "				+ "DM"								+ " | " + 
																												"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																												"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																												"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																												"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
			else if (DcTestTPC.testFunction == Cond_TestFunction_MeasureCurrent)
			{
				ret = DmTestFunction_MeasureCurrent(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_MeasureCurrent] Module: "			+ "DM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_MeasureCurrent] Module: "				+ "DM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
			else if (DcTestTPC.testFunction == Cond_TestFunction_MeasureVoltage)
			{
				ret = DmTestFunction_MeasureVoltage(tfSite, siteIndex, DcTestTPC.pinAlias, DcTestTPC.nplc, DcTestTPC.measureDelay, result);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_MeasureVoltage] Module: "			+ "DM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest -> DM400Utility -> DmTestFunction_MeasureVoltage] Module: "				+ "DM"								+ " | " + 
																															"PinAlias: "		+ DcTestTPC.pinAlias				+ " | " +
																															"TestFunction: "	+ DcTestTPC.testFunction			+ " | " +
																															"NPLC: "			+ DcTestTPC.nplc.ToString()			+ " | " +
																															"MeasureDelay: "	+ DcTestTPC.measureDelay.ToString()	+ "sec");
			}
		}
		else if (DcTestTPC.module == Cond_Module_IOM)
		{
			// [Future Enchancement] Reserved for IOM resource.
		}
		else if (DcTestTPC.module == Cond_Module_ACM)
		{
			// [Future Enchancement] Reserved for ACM resource.
		}
		else if (DcTestTPC.module == Cond_Module_TM)
		{
			// [Future Enchancement] Reserved for TM resource.
		}

	EndOfTest:

		if (ret != 0) result = (double)CONST_INVALID_RESULT;			
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Digital/Pattern Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::PatternControl(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** PatternControl
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'PatternControl' phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternControl] Executing 'PatternControl' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternControl] Executing 'PatternControl' phase.");

		// Get active module alias
		String ^ activeDmModuleAlias = String::Empty;
		for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
		{
			for (int j = 0; j < tl->glob->RsrcManager.dmModuleCount; j++)
			{
				if (PatternControlCSC.sdataPinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i]) // use sdataPinAlias to search, since sdata ad sclk must use the same module
				{
					if (tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, i]->Contains(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, j]))
					{
						activeDmModuleAlias = tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, j];
						goto DmModuleSelectionDone;
					}
				}
			}
		}

	DmModuleSelectionDone:

		if ((PatternControlCSC.controlFunction == Cond_ControlFunction_MipiWrite) || (PatternControlCSC.controlFunction == Cond_ControlFunction_MipiRead))
		{
			tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternControl -> DM400Utility -> ConfigureDmVectorEngine_Mipi] ControlFunction: "		+ PatternControlCSC.controlFunction									+ " | " +
																															"isMultiVecToOneResult: "	+ PatternControlCSC.isMultiVecToOneResult.ToString()				+ " | " +
																															"isOneVecToMultiResult: "	+ PatternControlCSC.isOneVecToMultiResult.ToString()				+ " | "	+
																															"sclkPinAlias: "			+ PatternControlCSC.sclkPinAlias									+ " | " +
																															"sdataPinAlias: "			+ PatternControlCSC.sdataPinAlias									+ " | " +
																															"MipiFreq: "				+ PatternControlCSC.userMipiFreq.ToString()			+ "Hz"			+ " | " +
																															"DmTerminationVoltage: "	+ PatternControlCSC.userVth.ToString()				+ "V"			+ " | " +
																															"DmInputChDelay: "			+ PatternControlCSC.userDmInputChDelay.ToString()	+ "ClockCycle"	+ " | " +
																															"RunVectorDelay: "			+ PatternControlCSC.runVectorDelay.ToString()		+ "sec");
			tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternControl -> DM400Utility -> ConfigureDmVectorEngine_Mipi] ControlFunction: "			+ PatternControlCSC.controlFunction									+ " | " +
																															"isMultiVecToOneResult: "	+ PatternControlCSC.isMultiVecToOneResult.ToString()				+ " | " +
																															"isOneVecToMultiResult: "	+ PatternControlCSC.isOneVecToMultiResult.ToString()				+ " | "	+
																															"sclkPinAlias: "			+ PatternControlCSC.sclkPinAlias									+ " | " +
																															"sdataPinAlias: "			+ PatternControlCSC.sdataPinAlias									+ " | " +
																															"MipiFreq: "				+ PatternControlCSC.userMipiFreq.ToString()			+ "Hz"			+ " | " +
																															"DmTerminationVoltage: "	+ PatternControlCSC.userVth.ToString()				+ "V"			+ " | " +
																															"DmInputChDelay: "			+ PatternControlCSC.userDmInputChDelay.ToString()	+ "ClockCycle"	+ " | " +
																															"RunVectorDelay: "			+ PatternControlCSC.runVectorDelay.ToString()		+ "sec");

			// Get latest user's input from 'PatternControl' condition
			dmPeModeSetting.mipiFreq			= PatternControlCSC.userMipiFreq;
			dmPeModeSetting.vth					= PatternControlCSC.userVth;
			dmPeModeSetting.inputChannelDelay	= PatternControlCSC.userDmInputChDelay;
			dmMipiVec.runMipiDelay				= PatternControlCSC.runVectorDelay;

			// Configure vector engine
			ret = ConfigureDmVectorEngine_Mipi(tfSite, siteIndex,	activeDmModuleAlias, 
																	PatternControlCSC.sclkPinAlias, 
																	PatternControlCSC.sdataPinAlias,
																	dmPeModeSetting.mipiFreq, 
																	dmPeModeSetting.inputChannelDelay,
																	DM_CONST_VECTORENGINEATTRIBUTE_TRIGGER_DISABLE,
																	DM_CONST_VECTORENGINEATTRIBUTE_CONTINUOUS_DISABLE,
																	DM_CONST_TRIGGER_EXT_TRIGOUT0,
																	DM_CONST_TRIGGER_EXT_TRIGOUT1);
			if (ret != 0) goto EndOfTest;
			
			// Configure pin to PE mode
			double vih	= dmPeModeSetting.vih; // Already configure in DcControl before PatternTest
			double vil	= 0.0 V;
			double voh	= (dmPeModeSetting.vih / 2.0) - 0.1 V;
			double vol	= (dmPeModeSetting.vih / 2.0) - 0.1 V;
			double ioh	= 0.0 A;
			double iol	= 0.0 A;
			double vch	= 0.0 V;
			double vcl	= 0.0 V;
			double vth	= dmPeModeSetting.vth;
			ret = DmConfigurePinToPeMode(tfSite, siteIndex, activeDmModuleAlias, 
															PatternControlCSC.sclkPinAlias, 
															DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_ENABLE, 
															DM_CONST_PEATTRIBUTE_HV_DISABLE, 
															DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE,
															DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE,
															DM_CONST_VECTORENGINEATTRIBUTE_TRIGGER_DISABLE,
															DM_CONST_VECTORENGINEATTRIBUTE_CONTINUOUS_DISABLE,	
															vih, vil, voh, vol, ioh, iol, vch, vcl, vth);
			if (ret != 0) goto EndOfTest;
			ret = DmConfigurePinToPeMode(tfSite, siteIndex, activeDmModuleAlias, 
															PatternControlCSC.sdataPinAlias, 
															DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_ENABLE, 
															DM_CONST_PEATTRIBUTE_HV_DISABLE, 
															DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE,
															DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE,
															DM_CONST_VECTORENGINEATTRIBUTE_TRIGGER_DISABLE,
															DM_CONST_VECTORENGINEATTRIBUTE_CONTINUOUS_DISABLE,	
															vih, vil, voh, vol, ioh, iol, vch, vcl, vth);
			if (ret != 0) goto EndOfTest;

			// Initial global result variable (PatternControl/PatternTest)
			PatternControlGlobalResultVariable.totalVecReadCount	= 0;
			PatternControlGlobalResultVariable.totalVecFailCount	= 0;
			PatternControlGlobalResultVariable.vecFailCount			= (int)CONST_INVALID_RESULT;
			PatternControlGlobalResultVariable.vecFirstFail			= (int)CONST_INVALID_RESULT;
			PatternControlGlobalResultVariable.historyRam			= gcnew array<int>(1);
			for (int i = 0; i < PatternControlGlobalResultVariable.historyRam->Length; i++)
			{
				PatternControlGlobalResultVariable.historyRam[i]	= (int)CONST_INVALID_RESULT;
			}

			// Execute vector
			if (PatternControlCSC.controlFunction == Cond_ControlFunction_MipiWrite)
			{
				// Run vector only
				ret = DmPatternTestFunction_MipiWrite(tfSite, siteIndex, activeDmModuleAlias, PatternControlCSC.vectorFileName);
				if (ret != 0) goto EndOfTest;
			}
			else if (PatternControlCSC.controlFunction == Cond_ControlFunction_MipiRead)
			{
				// Run vector, get result, and store result in global variable. 'PatternTest' will call the variable to display result only
				ret = DmPatternTestFunction_MipiRead(tfSite, siteIndex, activeDmModuleAlias,
																		PatternControlCSC.sclkPinAlias,
																		PatternControlCSC.sdataPinAlias,
																		PatternControlCSC.vectorFileName,
																		PatternControlCSC.isMultiVecToOneResult,
																		PatternControlCSC.isOneVecToMultiResult);
				if (ret != 0) goto EndOfTest;
			}
		}
		else if ((PatternControlCSC.controlFunction == Cond_ControlFunction_SpiWrite) || (PatternControlCSC.controlFunction == Cond_ControlFunction_SpiRead))
		{
			// [Future Enchancement] Reserved for SPI protocol.

			// Get latest user's input from 'PatternControl' condition

			// Configure vector engine
			ret = ConfigureDmVectorEngine_Spi(tfSite, siteIndex,	activeDmModuleAlias, 
																	PatternControlCSC.sclkPinAlias, 
																	PatternControlCSC.sdataPinAlias);
			if (ret != 0) goto EndOfTest;

			// Configure pin to PE mode

			// Initial global result variable (PatternControl/PatternTest)

			// Execute vector
			if (PatternControlCSC.controlFunction == Cond_ControlFunction_SpiWrite)
			{
				// Run vector only
				ret = DmPatternTestFunction_SpiWrite(tfSite, siteIndex, activeDmModuleAlias, PatternControlCSC.vectorFileName);
				if (ret != 0) goto EndOfTest;
			}
			else if (PatternControlCSC.controlFunction == Cond_ControlFunction_SpiRead)
			{
				// Run vector, get result, and store result in global variable. 'PatternTest' will call the variable to display result only
				ret = DmPatternTestFunction_SpiRead(tfSite, siteIndex,	activeDmModuleAlias,
																		PatternControlCSC.sclkPinAlias,
																		PatternControlCSC.sdataPinAlias,
																		PatternControlCSC.vectorFileName);
				if (ret != 0) goto EndOfTest;
			}
		}
		else if ((PatternControlCSC.controlFunction == Cond_ControlFunction_I2cWrite) || (PatternControlCSC.controlFunction == Cond_ControlFunction_I2cRead))
		{
			// [Future Enchancement] Reserved for I2C protocol.

			// Get latest user's input from 'PatternControl' condition

			// Configure vector engine
			ret = ConfigureDmVectorEngine_I2c(tfSite, siteIndex,	activeDmModuleAlias, 
																	PatternControlCSC.sclkPinAlias, 
																	PatternControlCSC.sdataPinAlias);
			if (ret != 0) goto EndOfTest;

			// Configure pin to PE mode

			// Initial global result variable (PatternControl/PatternTest)

			// Execute vector
			if (PatternControlCSC.controlFunction == Cond_ControlFunction_I2cWrite)
			{
				// Run vector only
				ret = DmPatternTestFunction_I2cWrite(tfSite, siteIndex, activeDmModuleAlias, PatternControlCSC.vectorFileName);
				if (ret != 0) goto EndOfTest;
			}
			else if (PatternControlCSC.controlFunction == Cond_ControlFunction_I2cRead)
			{
				// Run vector, get result, and store result in global variable. 'PatternTest' will call the variable to display result only
				ret = DmPatternTestFunction_I2cRead(tfSite, siteIndex,	activeDmModuleAlias,
																		PatternControlCSC.sclkPinAlias,
																		PatternControlCSC.sdataPinAlias,
																		PatternControlCSC.vectorFileName);
				if (ret != 0) goto EndOfTest;
			}
		}
		else if (PatternControlCSC.controlFunction == Cond_ControlFunction_PlayPattern)
		{
			// [Future Enchancement] Reserved for function playing pattern.
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::PatternTest(int tfSite, int siteIndex, int % result, String^ % resultMessage)
	{
		/*****************************************************************************************************
		** PatternTest
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DC resource object index, normally start from 0.
		**		result		- Return test result.
		**
		** Descriptions:
		**		This is a function to execute 'PatternTest' phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		result	= (int)CONST_INVALID_RESULT;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest] Executing 'PatternTest' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest] Executing 'PatternTest' phase.");

		// Check whether 'ReturnIndex' fulfill total vector's read back count inside the current vector file
		if (PatternTestTPC.returnIndex >= PatternControlGlobalResultVariable.totalVecReadCount)
		{
			ret = ER_CONST_PATTERNTEST_CONDITION_VALUE_INPUT_INVALID;
			String ^ additionalMessage = "'ReturnIndex' must not exceed the total vector's read back count inside the target vector file that has been executed.";
			ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternTestConditionName_ReturnIndex, additionalMessage);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[PatternTest condition value verification] 'ReturnIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString());
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[PatternTest condition value verification] 'ReturnIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString());
			return ret;
		}

		// Get from global result and return with respect to the user input 'ReturnIndex'
		if (PatternControlCSC.controlFunction == Cond_ControlFunction_MipiRead)
		{
			//Situation: run one vector file and return multiple result
			if ((PatternControlCSC.isMultiVecToOneResult == false) && (PatternControlCSC.isOneVecToMultiResult == true))
			{
				if (PatternControlGlobalResultVariable.resMsg_VecFailCount[0] == 0)
				{
					// Result
					result = dmMipiVec.displayMipiRegDataTarget[PatternTestTPC.returnIndex];

					// Result logger
					String ^ resultMessage1 = String::Empty;
					String ^ resultMessage2 = String::Empty;
					resultMessage1 +=	"\t Pattern Test Mode: "				+ "One Vector File To Multiple Result"									+ "\n" +
										"\t Current Vector File In Execute: "	+ PatternControlGlobalResultVariable.resMsg_VecFileName[0]				+ "\n" +
										"\t Read Back Status: "					+ "Pass"																+ "\n" +
										"\t Vector Fail Count: "				+ "0"																	+ "\n" + 
										"\t Vector First Fail: "				+ "-"																	+ "\n";
					resultMessage2 +=	"\t HistoryRam for Mipi Set #" + (PatternTestTPC.returnIndex + 1).ToString() + " -->> " + "Target Usid: " + dmMipiVec.displayMipiUsidTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Target RegAddr: " + dmMipiVec.displayMipiRegAddrTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Target RegData: " + dmMipiVec.displayMipiRegDataTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Actual RegData: " + PatternControlGlobalResultVariable.resMsg_HistoryRam[0][PatternTestTPC.returnIndex].ToString();					
					resultMessage = resultMessage1 + resultMessage2;
				}
				else
				{
					// Result
					if (PatternControlGlobalResultVariable.resMsg_HistoryRam[0][PatternTestTPC.returnIndex] == dmMipiVec.displayMipiRegDataTarget[PatternTestTPC.returnIndex])
						result = dmMipiVec.displayMipiRegDataTarget[PatternTestTPC.returnIndex];
					else	
						result = (int)CONST_INVALID_RESULT;

					// Result logger
					String ^ resultMessage1 = String::Empty;
					String ^ resultMessage2 = String::Empty;
					resultMessage1 +=	"\t Pattern Test Mode: "				+ "One Vector File To Multiple Result"									+ "\n" +
										"\t Current Vector File In Execute: "	+ PatternControlGlobalResultVariable.resMsg_VecFileName[0]				+ "\n" +
										"\t Read Back Status: "					+ "Fail"																+ "\n" +
										"\t Vector Fail Count: "				+ PatternControlGlobalResultVariable.resMsg_VecFailCount[0].ToString()	+ "\n" +
										"\t Vector First Fail: "				+ PatternControlGlobalResultVariable.resMsg_VecFirstFail[0].ToString()	+ "\n";
					resultMessage2 +=	"\t HistoryRam for Mipi Set #" + (PatternTestTPC.returnIndex + 1).ToString() + " -->> " + "Target Usid: " + dmMipiVec.displayMipiUsidTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Target RegAddr: " + dmMipiVec.displayMipiRegAddrTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Target RegData: " + dmMipiVec.displayMipiRegDataTarget[PatternTestTPC.returnIndex].ToString() + " | " + "Actual RegData: " + PatternControlGlobalResultVariable.resMsg_HistoryRam[0][PatternTestTPC.returnIndex].ToString();
					resultMessage = resultMessage1 + resultMessage2;
				}
			}
			
			//Situation: run multiple vector file and return one result
			if ((PatternControlCSC.isMultiVecToOneResult == true) && (PatternControlCSC.isOneVecToMultiResult == false))
			{
				if (PatternControlGlobalResultVariable.totalVecFailCount == 0)
				{
					// Result
					result = 1;

					// Result logger
					String ^ totalVecFileNameToExecute = String::Empty;
					for (int i = 0; i < PatternControlCSC.overallVectorFileCount; i++)
					{
						totalVecFileNameToExecute += PatternControlGlobalResultVariable.resMsg_VecFileName[i];
						
						if (i < (PatternControlCSC.overallVectorFileCount - 1))
						{
							totalVecFileNameToExecute += ", ";
						}
					}

					String ^ resultMessage1 = String::Empty;
					String ^ resultMessage2 = String::Empty;
					resultMessage1 +=			"\t Pattern Test Mode: "				+ "Multiple Vector File To One Result"									+ "\n"	+
												"\t Overall Vector File To Execute: "	+ totalVecFileNameToExecute												+ "\n";

					for (int i = 0; i < PatternControlGlobalResultVariable.resMsg_VecFileName->Length; i++)
					{
						resultMessage1 +=		"\t Current Vector File In Execute: "	+ PatternControlGlobalResultVariable.resMsg_VecFileName[i]				+ "\n"	+
												"\t Read Back Status: "					+ "Pass"																+ "\n"	+
												"\t Vector Fail Count: "				+ "0"																	+ "\n"	+
												"\t Vector First Fail: "				+ "-"																	+ "\n";	
						
						for (int j = 0; j < PatternControlGlobalResultVariable.resMsg_HistoryRam[i]->Length; j++)
						{
							resultMessage2 +=	"\t HistoryRam for Mipi Set #" + (j + 1).ToString() + " -->> " + "Target Usid: " + dmMipiVec.displayMipiUsidTarget[j].ToString() + " | " + "Target RegAddr: " + dmMipiVec.displayMipiRegAddrTarget[j].ToString() + " | " + "Target RegData: " + dmMipiVec.displayMipiRegDataTarget[j].ToString() + " | " + "Actual RegData: " + PatternControlGlobalResultVariable.resMsg_HistoryRam[i][j].ToString();
							if (j < PatternControlGlobalResultVariable.resMsg_HistoryRam[i]->Length - 1)
							{
								resultMessage2 += "\n";
							}
						}
					}						
					resultMessage = resultMessage1 + resultMessage2;
				}
				else
				{
					// Result
					result = (int)CONST_INVALID_RESULT;

					// Result logger
					String ^ totalVecFileNameToExecute = String::Empty;
					for (int i = 0; i < PatternControlCSC.overallVectorFileCount; i++)
					{
						totalVecFileNameToExecute += PatternControlGlobalResultVariable.resMsg_VecFileName[i];
						
						if (i < (PatternControlCSC.overallVectorFileCount - 1))
						{
							totalVecFileNameToExecute += ", ";
						}
					}

					String ^ resultMessage1 = String::Empty;
					String ^ resultMessage2 = String::Empty;
					resultMessage1 +=			"\t Pattern Test Mode: "				+ "Multiple Vector File To One Result"									+ "\n"	+
												"\t Overall Vector File To Execute: "	+ totalVecFileNameToExecute												+ "\n";

					for (int i = 0; i < PatternControlGlobalResultVariable.resMsg_VecFileName->Length; i++)
					{
						resultMessage1 +=		"\t Current Vector File In Execute: "	+ PatternControlGlobalResultVariable.resMsg_VecFileName[i]				+ "\n";

						if (PatternControlGlobalResultVariable.resMsg_VecFailCount[i] != 0)
						{ 
							resultMessage1 +=	"\t Read Back Status: "					+ "Fail"																+ "\n"	+
												"\t Vector Fail Count: "				+ PatternControlGlobalResultVariable.resMsg_VecFailCount[i].ToString()	+ "\n"	+
												"\t Vector First Fail: "				+ PatternControlGlobalResultVariable.resMsg_VecFirstFail[i].ToString()	+ "\n";	
						}
						else
						{ 
							resultMessage1 +=	"\t Read Back Status: "					+ "Pass"																+ "\n"	+
												"\t Vector Fail Count: "				+ "0"																	+ "\n"	+
												"\t Vector First Fail: "				+ "-"																	+ "\n";	
						}

						for (int j = 0; j < PatternControlGlobalResultVariable.resMsg_HistoryRam[i]->Length; j++)
						{
							resultMessage2 +=	"\t HistoryRam for Mipi Set #" + (j + 1).ToString() + " -->> " + "Target Usid: " + dmMipiVec.displayMipiUsidTarget[j].ToString() + " | " + "Target RegAddr: " + dmMipiVec.displayMipiRegAddrTarget[j].ToString() + " | " + "Target RegData: " + dmMipiVec.displayMipiRegDataTarget[j].ToString() + " | " + "Actual RegData: " + PatternControlGlobalResultVariable.resMsg_HistoryRam[i][j].ToString();
							if (j < PatternControlGlobalResultVariable.resMsg_HistoryRam[i]->Length - 1)
							{
								resultMessage2 += "\n";
							}
						}
					}						
					resultMessage = resultMessage1 + resultMessage2;
				}
			}
		}
		else if (PatternControlCSC.controlFunction == Cond_ControlFunction_SpiRead)
		{
			// [Future Enchancement] Reserved for SPI protocol.
		}
		else if (PatternControlCSC.controlFunction == Cond_ControlFunction_I2cRead)
		{
			// [Future Enchancement] Reserved for I2C protocol.
		}
		else if (PatternControlCSC.controlFunction == Cond_ControlFunction_PlayPattern)
		{
			// No action for 'PlayPattern' operation.
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	DM Operation
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::DmControl(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** DmControl
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is DM resource object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to execute 'DmControl' phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DmControl] Executing 'DmControl' phase.");
		tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DmControl] Executing 'DmControl' phase.");

		for (int i = 0; i < DmControlCSC.moduleAlias->Length; i++) //Iterate count based on size of first condition
		{
			if ((DmControlCSC.controlFunction == Cond_ControlFunction_DM_ConfigureOutputTrigger))
			{ 
				// Configure DM pin
				ret = DmConfigureDmOutputTrigger(tfSite, siteIndex,DmControlCSC.moduleAlias[i],
					DmControlCSC.outputTrigger0[i],
					DmControlCSC.outputTrigger1[i]);
				if (ret != 0) goto EndOfTest;
				tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DmControl -> DM400Utility -> DmConfigureDmOutputTrigger]");
				tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DmControl -> DM400Utility -> DmConfigureDmOutputTrigger]");
			}
			
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Other Utilities
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::IdentifyTestParameterPhaseType(Site ^ site, int tfSite, int siteIndex, String ^ % currentPhase)
	{
		/*****************************************************************************************************
		** IdentifyTestParameterPhaseType
		**		site			- This is techFlow site object.
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is DC resource object index, normally start from 0.
		**		currentPhase	- This is the test phase to be identify and then return test phase type.
		**
		** Descriptions:
		**		This is a function to identify the phase type among the test parameter item.
		**		To identify: 'DcTest', 'PatternTest', 'VnaDataAnalysis'.
		******************************************************************************************************/

		// Local variable
		int ret						= 0;
		int countDcTest				= 0;
		int countPatternTest		= 0;
		int countVnaDataAnalysis	= 0;
		int countVnaSwTime	= 0;
		int countMathTest			= 0;

		array<ConditionCollection^> ^ conditionCollection = gcnew array <ConditionCollection^>(tl->glob->tf.NumberOfTestSites);
		Condition ^ condition;
		//tl->glob->currentSubItemName = gcnew array<String^>(tl->glob->tf.NumberOfTestSites);

		// Get test paramter item's condition list
		conditionCollection[siteIndex] = tf_TestParameter_ConditionList(tl->glob->currentSubItemName[siteIndex]);
		for each (condition in conditionCollection[siteIndex])
		{
			if ((condition->Name->Contains(DcTestConditionName_Module))			||
				(condition->Name->Contains(DcTestConditionName_PinAlias))		||
				(condition->Name->Contains(DcTestConditionName_TestFunction))	||
				(condition->Name->Contains(DcTestConditionName_NPLC))			||
				(condition->Name->Contains(DcTestConditionName_MeasureDelay)))
			{
				countDcTest++;
			}
			else if (condition->Name->Contains(PatternTestConditionName_ReturnIndex))
			{
				countPatternTest++;
			}
			else if ((condition->Name->Contains(VnaDataAnalysisConditionName_TraceIndex))		||
					 (condition->Name == (VnaDataAnalysisConditionName_Function))			||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_AnalysisSetting))	||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_SmoothingSetting))	||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_Freq))				||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_FreqRangeStart))	||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_FreqRangeStop))	||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_OutputType))		||
					 (condition->Name->Contains(VnaDataAnalysisConditionName_OutputFormat)))
			{
				countVnaDataAnalysis++;
			}
			if ((condition->Name->Contains(VnaSwTimeConditionName_MeasureType))		||
				(condition->Name == (VnaSwTimeConditionName_DMModuleAlias))			||
				(condition->Name->Contains(VnaSwTimeConditionName_DMVectorTrigger))	||
				(condition->Name->Contains(VnaSwTimeConditionName_AnalysisSetting))	||
				(condition->Name->Contains(VnaSwTimeConditionName_OutputType))				||
				(condition->Name->Contains(VnaSwTimeConditionName_OutputFormat)))
			{
				countVnaSwTime++;
			}
			else if ((condition->Name == (MathConditionName_Function))			||
					 (condition->Name->Contains(MathConditionName_TP1))			||
					 (condition->Name->Contains(MathConditionName_TP2)))
			{
				countMathTest++;
			}

		}

		// DcTest
		if ((countDcTest == ConditionCount_DcTest)	&&
			(countPatternTest == 0)					&& 
			(countVnaDataAnalysis == 0)				&&
			(countMathTest == 0))
		{
			currentPhase = PHASE_CONST_DC_TEST;
		}
		// PatternTest
		else if ((countDcTest == 0)									&&
				(countPatternTest == ConditionCount_PatternTest)	&&
				(countVnaDataAnalysis == 0)							&&
				(countMathTest == 0))
		{
			currentPhase = PHASE_CONST_PATTERN_TEST;
		}
		// VnaDataAnalysis
		else if ((countDcTest == 0)				&&
				(countPatternTest == 0)			&&
				(countVnaDataAnalysis % 3 == 0)	&&
				(countMathTest == 0))
		{
			currentPhase = PHASE_CONST_VNA_DATA_ANALYSIS;
		}
		// VnaSwTime
		else if (((countDcTest == 0)				&&
			(countPatternTest == 0)			&&
			(countVnaDataAnalysis == 0)	&&
			(countVnaSwTime == ConditionCount_VnaSwTime)	&&
			(countMathTest == 0)) ||
			(countVnaSwTime == ConditionCount_VnaSwTime + 1)) //debugging
		{
			currentPhase = PHASE_CONST_VNA_SWTIME;
		}
		//Math
		else if ((countDcTest == 0)				&&
				(countPatternTest == 0)			&&
				(countVnaDataAnalysis == 0)		&&
				(countMathTest == ConditionCount_MathTest))
		{
			currentPhase = PHASE_CONST_MATH;
		}
		else
		{
			currentPhase = "Invalid_Phase";
			ret = ER_CONST_GET_TEST_PARAMETER_PHASE_TYPE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[AMB7300 -> IdentifyTestParameterPhaseType] Fail to identify test parameter phase type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[AMB7300 -> IdentifyTestParameterPhaseType] Fail to identify test parameter phase type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	void AMB7300TestLibrary::ShowMessageBox(int tfSite, int siteIndex, String ^ messageBoxTitle, String ^ conditionName)
	{
		/*****************************************************************************************************
		** ShowMessageBox
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is resource object index, normally start from 0.
		**		messageBoxTitle		- This is the box's title caption.
		**		conditionName		- This is the current condition name.
		**
		** Descriptions:
		**		This is a function to show message box, to display the detail/info/warning/error.
		******************************************************************************************************/

		/*	Format:
		**
		**	Test Item: xxx
		**	Sub Flow Item: xxx
		**	Condition Name: xxx
		**	Warning: xxx
		**	Additional Message: xxx (if available)
		**	Action Item: xxx
		**
		*/

		// Local variable
		String ^ messageBoxTitleCaption		= String::Empty;
		String ^ currentTestItemName		= String::Empty;
		String ^ currentSubFlowItemName		= String::Empty;
		String ^ currentConditionName		= String::Empty;
		String ^ warningMessage				= String::Empty;
		String ^ actionItemMessage			= String::Empty;
		String ^ overallDisplayMessage		= String::Empty;

		if (messageBoxTitle == TITLE_CONST_CONDITION_NAME_NOT_FOUND)
		{
			messageBoxTitleCaption		= messageBoxTitle;
			currentTestItemName			= "Test Item: "						+ tl->glob->currentFlowName;
			currentSubFlowItemName		= "Sub Flow Item: "					+ tl->glob->currentSubItemName[siteIndex];
			currentConditionName		= "Condition Name: "				+ conditionName;
			warningMessage				= MSG_CONST_HEADER_WARNING			+ "'" + conditionName + "'" + MSG_CONST_CONDITION_NAME_NOT_FOUND;
			actionItemMessage			= MSG_CONST_HEADER_ACTION_ITEM		+ "Please ensure the condition name is correct and exists in the sub flow item Conditions property.";
			
			overallDisplayMessage		=	currentTestItemName				+ "\n"		+ 
											currentSubFlowItemName			+ "\n"		+ 
											currentConditionName			+ "\n\n"	+ 
											warningMessage					+ "\n\n"	+
											actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		else if (messageBoxTitle == TITLE_CONST_INVALID_CONDITION_INPUT_VALUE)
		{
			messageBoxTitleCaption		= messageBoxTitle;
			currentTestItemName			= "Test Item: "						+ tl->glob->currentFlowName;
			currentSubFlowItemName		= "Sub Flow Item: "					+ tl->glob->currentSubItemName[siteIndex];
			currentConditionName		= "Condition Name: "				+ conditionName;
			warningMessage				= MSG_CONST_HEADER_WARNING			+ "'" + conditionName + "'" + MSG_CONST_CONDITION_VALUE_INVALID_INPUT;
			actionItemMessage			= MSG_CONST_HEADER_ACTION_ITEM		+ "Please refer to the user manual for the available Conditions input option, and need to ensure that the Conditions input value is valid.";

			overallDisplayMessage		=	currentTestItemName				+ "\n"		+ 
											currentSubFlowItemName			+ "\n"		+ 
											currentConditionName			+ "\n\n"	+ 
											warningMessage					+ "\n\n"	+
											actionItemMessage;
			
			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		else if (messageBoxTitle == TITLE_CONST_INVALID_APPWIDE_VARIABLE_NOT_FOUND)
		{
			messageBoxTitleCaption = messageBoxTitle;
			currentConditionName = "AppWide Variable Name: " + conditionName;
			warningMessage = MSG_CONST_HEADER_WARNING + "'" + conditionName + "'" + MSG_CONST_APPWIDE_VARIABLE_NOT_FOUND;
			actionItemMessage = MSG_CONST_HEADER_ACTION_ITEM + "Please check the AppWide Variables, and need to ensure that the variable exists in the AppWide Variable.";

			overallDisplayMessage = currentConditionName + "\n\n" +
				warningMessage + "\n\n" +
				actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		/*else if (messageBoxTitle == TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE)
		{
		messageBoxTitleCaption = messageBoxTitle;
		currentConditionName = "AppWide Variable Name: " + conditionName;
		warningMessage = MSG_CONST_HEADER_WARNING + "'" + conditionName + "'" + MSG_CONST_APPWIDE_VARIABLE_INVALID_INPUT;
		actionItemMessage = MSG_CONST_HEADER_ACTION_ITEM + "Please refer to the user manual for the available AppWideVariable input option, and need to ensure that the AppWideVariable input value is valid.";

		overallDisplayMessage = currentConditionName + "\n\n" +
		warningMessage + "\n\n" +
		actionItemMessage;

		// Show warning message box
		MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}*/
	}
	void AMB7300TestLibrary::ShowMessageBox(int tfSite, int siteIndex, String ^ messageBoxTitle, String ^ conditionName, String ^ additionalMessage)
	{
		/*****************************************************************************************************
		** ShowMessageBox
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is resource object index, normally start from 0.
		**		messageBoxTitle		- This is the box's title caption.
		**		conditionName		- This is the current condition name.
		**		additionalMessage	- This is the additional message.
		**
		** Descriptions:
		**		This is a function to show message box, to display the detail/info/warning/error.
		******************************************************************************************************/

		/*	Format:
		**	
		**	Test Item: xxx
		**	Sub Flow Item: xxx
		**	Condition Name: xxx
		**	Warning: xxx
		**	Additional Message: xxx (if available)
		**	Action Item: xxx
		**
		*/

		// Local variable
		String ^ messageBoxTitleCaption		= String::Empty;
		String ^ currentTestItemName		= String::Empty;
		String ^ currentSubFlowItemName		= String::Empty;
		String ^ currentConditionName		= String::Empty;
		String ^ warningMessage				= String::Empty;
		String ^ additionalMessages			= String::Empty;
		String ^ actionItemMessage			= String::Empty;
		String ^ overallDisplayMessage		= String::Empty;

		if (messageBoxTitle == TITLE_CONST_CONDITION_NAME_NOT_FOUND)
		{
			messageBoxTitleCaption		= messageBoxTitle;
			currentTestItemName			= "Test Item: "						+ tl->glob->currentFlowName;
			currentSubFlowItemName		= "Sub Flow Item: "					+ tl->glob->currentSubItemName[siteIndex];
			currentConditionName		= "Condition Name: "				+ conditionName;
			warningMessage				= MSG_CONST_HEADER_WARNING			+ "'" + conditionName + "'" + MSG_CONST_CONDITION_NAME_NOT_FOUND;
			additionalMessages			= MSG_CONST_HEADER_ADDITIONAL_MSG	+ additionalMessage;
			actionItemMessage			= MSG_CONST_HEADER_ACTION_ITEM		+ "Please ensure the condition name is correct and exists in the sub flow item Conditions property.";

			overallDisplayMessage		=	currentTestItemName				+ "\n"		+ 
											currentSubFlowItemName			+ "\n"		+ 
											currentConditionName			+ "\n\n"	+ 
											warningMessage					+ "\n\n"	+
											additionalMessages				+ "\n\n"	+
											actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		else if (messageBoxTitle == TITLE_CONST_INVALID_CONDITION_INPUT_VALUE)
		{
			messageBoxTitleCaption		= messageBoxTitle;
			currentTestItemName			= "Test Item: "						+ tl->glob->currentFlowName;
			currentSubFlowItemName		= "Sub Flow Item: "					+ tl->glob->currentSubItemName[siteIndex];
			currentConditionName		= "Condition Name: "				+ conditionName;
			warningMessage				= MSG_CONST_HEADER_WARNING			+ "'" + conditionName + "'" + MSG_CONST_CONDITION_VALUE_INVALID_INPUT;
			additionalMessages			= MSG_CONST_HEADER_ADDITIONAL_MSG	+ additionalMessage;
			actionItemMessage			= MSG_CONST_HEADER_ACTION_ITEM		+ "Please refer to the user manual for the available Conditions input option, and need to ensure that the Conditions input value is valid.";

			overallDisplayMessage		=	currentTestItemName				+ "\n"		+ 
											currentSubFlowItemName			+ "\n"		+ 
											currentConditionName			+ "\n\n"	+ 
											warningMessage					+ "\n\n"	+
											additionalMessages				+ "\n\n"	+
											actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		else if (messageBoxTitle == TITLE_CONST_INVALID_APPWIDE_VARIABLE_NOT_FOUND)
		{
			messageBoxTitleCaption = messageBoxTitle;
			currentConditionName = "AppWide Variable Name: " + conditionName;
			warningMessage = MSG_CONST_HEADER_WARNING + "'" + conditionName + "'" + MSG_CONST_APPWIDE_VARIABLE_NOT_FOUND;
			additionalMessages = MSG_CONST_HEADER_ADDITIONAL_MSG + additionalMessage;
			actionItemMessage = MSG_CONST_HEADER_ACTION_ITEM + "Please check the AppWide Variables, and need to ensure that the variable exists in the AppWide Variable.";

			overallDisplayMessage = currentConditionName + "\n\n" +
				warningMessage + "\n\n" +
				additionalMessages + "\n\n" +
				actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		else if (messageBoxTitle == TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE)
		{
			messageBoxTitleCaption = messageBoxTitle;
			currentConditionName = "AppWide Variable Name: " + conditionName;
			warningMessage = MSG_CONST_HEADER_WARNING + "'" + conditionName + "'" + MSG_CONST_APPWIDE_VARIABLE_INVALID_INPUT;
			additionalMessages = MSG_CONST_HEADER_ADDITIONAL_MSG + additionalMessage;
			actionItemMessage = MSG_CONST_HEADER_ACTION_ITEM + "Please refer to the user manual for the available AppWideVariable input option, and need to ensure that the AppWideVariable input value is valid.";

			overallDisplayMessage = currentConditionName + "\n\n" +
				warningMessage + "\n\n" +
				additionalMessages + "\n\n" +
				actionItemMessage;

			// Show warning message box
			MessageBox::Show(overallDisplayMessage, messageBoxTitleCaption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
		// Reset
		additionalMessage = String::Empty;
	}
	bool AMB7300TestLibrary::IsRunningProduction(Site ^ site)
	{
		IProductionSystem^ ps = (IProductionSystem^)site->FlowEngine->EngineManager->GetService(IProductionSystem::typeid);

		if (RemotingServices::IsTransparentProxy(ps))
			return false;
		else
			return true;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Threaded APIs
	**	----------------------------------------------------------------------------------------------------
	*/
}


/*----------------------------------------------------------------------
* Revision Log
* &Log: AMB7300.cpp.rca&

*** Version	: v1.0.0.6
*** Date	: 2 September 2026
*** PIC		: Xuli & Zhi Kean
* Commented calibration date check in InitializeVna
* Updated SmaveToTouchstoneFile CustLotId value
* Added supprot for PXI9852 module switching time test

*** Version	: v1.0.0.5
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Add support for Mutex
* Remove reload state file SCPI after SharedMemoryTransfer_GetFreqPoints()
* Shorten vnaDataAnalysis
* Add Map_siteIndex_To_vnaSiteIndex
* Add PowerSweep & P1dB related functions (under develpoment)

*** Version	: v1.0.0.4
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Support auto launch and minimize VNA SFP window feature
* Support GenericStateMappingFile
* Support total Channel check between state, mapping & test recipe
* Add SharedMemoryTransfer_GetFreqPoints() portion

*** Version	: v1.0.0.3
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Support True Parallel Multi UUT project (KeysightVNA)
* IdentifyTestParameterPhaseType() - Change conditionCollection to be array
* InitiallizeTester() - Added vnaSystemCount portion 
* Added VnaFetch_TrueParallel & VnaDataAnalysis_TrueParalel

*** Version	: v1.0.0.2
*** Date	: 9 April 2024
*** PIC		: Ng Chen Yang
* Support the S2Ppath and SaveSnpData which already moved to AppWideVariable
* Support the message box for S2Ppath and SaveSnpData in AppWideVariable

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* Either special naming or default naming depends whether if the SaveSnpFilePath is blank
* Special BW search for SkySemi (bw level = peak level - bw level)
* Support for the CSA/STA state file template depends on the VNA model.

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
