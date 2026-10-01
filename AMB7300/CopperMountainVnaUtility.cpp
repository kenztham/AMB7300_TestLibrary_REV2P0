/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				CopperMountainVnaUtility.cpp
Purpose:			Contain Copper Mountain VNA related utility functions
Supported Module:	Copper Mountain VNA SC5090
Version:			v1.0.0.3
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Initialize & Uninitialize Vna Hardware
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeVna_CMT(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, int vnaSiteIndexAlias, bool offline)
	{
		/*****************************************************************************************************
		** InitializeVna_CMT
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		offline				- techFlow run in online or offline mode.
		**
		** Descriptions:
		**		This is a function is to initialize Copper Mountain VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		String ^ systemAlias	= String::Empty;
		// To prevent multiple same resources are loaded
		Dictionary<String^, int> ^ ResourceAlias = gcnew Dictionary<String ^, int>();

		try
		{
#pragma region "Initialize S2VNA SFP"

			// [Future Enchancement] To execute bat file and auto run the s2vna software 

#pragma endregion

#pragma region "Initialize Copper Mountain Vna"

			/*
			**	A0 - S2P1
			**	A1 - S2P4
			**	A2 - S2P8
			**	A3 - S4P2 [4x S2P / 2x S4P]
			**	A4 - S4P1 [2x S2P / 1x S4P]
			**	A5 - S2P16N (customized 2x S2P)
			*/

			for each (String ^ Rsrc in tl->glob->RsrcManager.RsrcMngr[siteIndex]->MapNames)
			{
				Resource ^ HardwareRsrc = tl->glob->RsrcManager.RsrcMngr[siteIndex]->ResolveResource(Rsrc)[0];

				if ((HardwareRsrc->Hierarchy->Length == 1)									&&
					((HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S2P16N") == true)		||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S4P1") == true)			||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S4P2") == true)			||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S2P8") == true)			||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S2P4") == true)			||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S2P1") == true))		&&
					(ResourceAlias->ContainsKey(HardwareRsrc->Alias->ToString()) == false))
				{
					// A0 [S2P1]
					if (Rsrc->StartsWith(AMB7300_S2P1_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S2P1_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S2P1_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A0);
					}

					// A5 [S2P16N]
					if (Rsrc->StartsWith(AMB7300_S2P16_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S2P16_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S2P16_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A5);
					}
					// A4 [S4P1]
					else if (Rsrc->StartsWith(AMB7300_S4P1_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S4P1_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S4P1_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A4);
					}
					// A3 [S4P2]
					else if (Rsrc->StartsWith(AMB7300_S4P2_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S4P2_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S4P2_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A3);
					}
					// A2 [S2P8]
					else if (Rsrc->StartsWith(AMB7300_S2P8_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S2P8_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S2P8_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A2);
					}
					// A1 [S2P4]
					else if (Rsrc->StartsWith(AMB7300_S2P4_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S2P4_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias	= AMB7300_S2P4_TYPE;
						amb7300SystemSetting->systemConfig	= int(SystemConfig::A1);
					}
 
					// Initialize
					if ((AMB7300_VnaHardwareStatus[siteIndex] == false) && (ResourceAlias->ContainsKey(Rsrc) == false))
					{
						// To prevent multiple same resources are loaded
						ResourceAlias->Add(HardwareRsrc->Alias->ToString(), 1);

						/*
						** Create AMB7300 object only when detect different config (eg AMB7300S2P8_1) which is different from other/first object (eg AMB7300S2P8_0)
						** Load Option 0 = using mpex controller | 1 = not using mpex controller
						*/
						if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
						{
							if (amb7300SystemSetting->systemConfig == int(SystemConfig::A0))
							{
								tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
							}
							else
							{
								// Init option 1 = enable mux controller, for A1~A5 config --> ALL option set to 0 as 1 not longer needed in for this api
								tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
							}
						}
						else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
						{
							if (amb7300SystemSetting->systemConfig == int(SystemConfig::A0))
							{
								tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
							}
							else
							{
								// Init option 1 = enable mux controller, for A1~A5 config
								tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile)); 
							}
						}
						else if (tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
						{
							if (siteIndex == 0)
							{
								tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
							}
							else
							{
								amb7300_[siteIndex] = amb7300_[0];
							}
						}
						else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
						{
							// [Future Enchancement] reserved for index parallel project.
						}
					}
					AMB7300_VnaHardwareStatus[siteIndex] = true;

					amb7300_driver[siteIndex]	= (AMB7300_Driver ^)amb7300_[siteIndex]->amb7300;
					amb7300_NA[siteIndex]		= (NetworkAnalyzer ^)amb7300_[siteIndex]->amb7300->NetworkAnalyzer[0];	
				}
			}

#pragma endregion

		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_COPPER_MOUNTAIN_VNA_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Fail to initialize Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> CopperMountainVnaUtility -> InitializeVna_CMT] Fail to initialize Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeVna_CMT(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeVna_CMT
		**		tfSite	- This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to uninitialize Copper Mountain VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			if (tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
			{
				if (siteIndex == 0)
				{
					tl->CheckError(tfSite, amb7300_[siteIndex]->UnLoad());
				}
				else
				{

				}
			}
			else
			{
				tl->CheckError(tfSite, amb7300_[siteIndex]->UnLoad());
			}

			AMB7300_VnaHardwareStatus[siteIndex] = false;
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_COPPER_MOUNTAIN_VNA_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> CopperMountainVnaUtility -> UninitializeVna_CMT] Fail to uninitialize Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> CopperMountainVnaUtility -> UninitializeVna_CMT] Fail to uninitialize Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : MPEX Port Pair Switching
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureMpexPortPairSwitching_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureMpexPortPairSwitching_CMT
		**	testSite		- This is techFlow site index.
		**	vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure the MPEX port pair path switching.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Configure the mpex swiches path by referring to the port pair | the port pair must exist in the mapping file  (Support A1-A5) | No need to configure for A0 config
			if (amb7300SystemSetting->systemConfig != int(SystemConfig::A0))
			{
				tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->ConfigureSwitchPath(vnaSetting[vnaSiteIndex].pinAlias));
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_MPEX_PORT_PAIR_SWITCHING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Fail to configure MPEX port pair switching." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureMpexPortPairSwitching_CMT] Fail to configure MPEX port pair switching." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			return ret;
		}

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Set Port Matching Circuit
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::SetPortMatchingCircuit_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	SetPortMatchingCircuit_CMT
		**	testSite		- This is techFlow site index.
		**	vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to set the Port Matching circuit with given filepath.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int VNA_port_count = 6;

		try
		{
			//if (amb7300SystemSetting->systemConfig == int(SystemConfig::S2P1D))
			//{
			//	VNA_port_count = 2;
			//}
			//else if (amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D))
			//{
			//	VNA_port_count = 4;
			//}
			//else if ((amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) ||
			//	(amb7300SystemSetting->systemConfig == int(SystemConfig::S6P22N)))
			//{
			//	VNA_port_count = 6;
			//}
			//else
			//{
			//	VNA_port_count = 2;
			//}

			array<String^>^ PortMatching_filepath = gcnew array<String^>(VNA_port_count);

			//Assign PortMatching filepath to local array
			for (int port; port < VNA_port_count; port++)
			{
				if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching1;
				}
				else if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching2;
				}
				else if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching3;
				}
				else if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching4;
				}
				else if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching5;
				}
				else if (tl->glob->AWV.PortMatching_EN[port] == true)
				{
					PortMatching_filepath[port] = tl->glob->AWV.PortMatching6;
				}
			}

			for (int i = 0; i < projectStateInfo.Channels->Length; i++)
			{
				for (int port = 0; port < VNA_port_count; port++)
				{
					if (tl->glob->AWV.PortMatching_EN[port] == true)
					{
						tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->SetEmbedding(Int32::Parse(projectStateInfo.Channels[i]), port + 1, true, PortMatching_filepath[port]));
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SET_PORT_MATCHING_CIRCUIT_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SetPortMatchingCircuit_CMT] Fail to set Port Matching circuit." + "\n" +
				"Please check the provided App-Wide Variable." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SetPortMatchingCircuit_CMT] Fail to set Port Matching circuit." + "\n" +
				"Please check the provided App-Wide Variable." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			return ret;
		}

	EndOfTest:
		return ret;
	}
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Segment Settings [Stimulus->Segment]
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureSegmentSetting_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSegmentSetting_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		** 
		**	Descriptions:
		**		This is a function to configure the segment settings at the active channel (VNA stimulus).
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		array<int>^ vnaChannel	= gcnew array<int>(1);
		vnaChannel[0]			= 999;

		try
		{

#pragma region "HighPwrTest Control"
			if (tl->glob->AWV.HighPwrTest_EN == true)
			{
				String^ key = "Ch" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() + "_P" + segmentSetting[vnaSiteIndex].power[0].ToString() + "_Site" + vnaSiteIndex.ToString();

				double source_level = tl->glob->AppsCalFile.AppsCalFactor[key];

				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_SourcePower_SetLevel(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, source_level));
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_MeasureDelaySet(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, segmentSetting[vnaSiteIndex].delay[0]));
			}
#pragma endregion

			if (((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) ||
				(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment)) ||
				(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer) &&
				(segmentSetting[vnaSiteIndex].SettingHasChanged == true))
			{
				for (int iChannel = 0; iChannel < vnaSetting[vnaSiteIndex].vna_chn->Length; iChannel++)
				{
					if (vnaSetting[vnaSiteIndex].vna_chn[iChannel] != Vna_Channel_Reset)
					{
						// Local variable channelarr for wrapper used
						//vnaChannel[0] = vnaSetting[vnaSiteIndex].vna_chn[iChannel];
						//vnaChannel[0] = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;
						if (!vnaSetting[vnaSiteIndex].ChannelNumCondition_Exist)
						{
							vnaChannel[0] = iChannel + 1;
						}
						else
						{
							vnaChannel[0] = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;
						}

#pragma region "Display active channel"

						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaChannel[0]));

#pragma endregion

#pragma region "Configure segment settings"

						if ((segmentSetting[vnaSiteIndex].segmentCount > 1) &&
							(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) ||
							(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
						{
							tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->MeasureSparamSetupLinear(
								vnaChannel,
								SParam_FormatVNA(segmentSetting[vnaSiteIndex].sparaFormat),
								segmentSetting[vnaSiteIndex].power,
								segmentSetting[vnaSiteIndex].startFreq,
								segmentSetting[vnaSiteIndex].stopFreq,
								segmentSetting[vnaSiteIndex].ifbw,
								segmentSetting[vnaSiteIndex].point,
								segmentSetting[vnaSiteIndex].delay,
								segmentSetting[vnaSiteIndex].singleMarker));
						}
						else if ((segmentSetting[vnaSiteIndex].segmentCount == 1) &&
							(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment) ||
							(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
						{
							tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->MeasureSparamSetupLinear(
								vnaChannel,
								SParam_FormatVNA(segmentSetting[vnaSiteIndex].sparaFormat),
								FreqPow_StyleVNA::StartStop,
								segmentSetting[vnaSiteIndex].power[0],
								segmentSetting[vnaSiteIndex].startFreq[0],
								segmentSetting[vnaSiteIndex].stopFreq[0],
								segmentSetting[vnaSiteIndex].ifbw[0],
								segmentSetting[vnaSiteIndex].point[0],
								segmentSetting[vnaSiteIndex].singleMarker));
						}

#pragma endregion

						// Update array size of Freq Point if segment is configured
						if (segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer)
						{
							ret = GetChannelandTraceSetting_CMT(tfSite, vnaSiteIndex);
						}
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_SEGMENT_SETTING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSegmentSetting_CMT] Fail to configure segment setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSegmentSetting_CMT] Fail to configure segment setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetChannelandTraceSetting_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetChannelandTraceSetting_CMT
		**	testSite		- This is techFlow site index.
		**	vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to get:
		**			- current number of active channel
		**			- first trace of each channel
		**			- total trace count 
		**			- [Future Enhancement] trace measurement format
		**			- trace sweep type (Lin,Seg,Power)
		**			- trace FreqPoint
		******************************************************************************************************/

		int ret = 0;

		String^ command;
		String^ Val = String::Empty;
		array<String^>^ Freqs;
		array<String^>^ StateFile_Channels;
		array<String^>^ FirstTrace_Temp;
		//array<String^>^ FirstTrace;
		double totalTrace = 0;
		double temp;
		try
		{
			// Get total number of channel
			command = "DISP:SPL?\n";
			amb7300_NA[tfSite]->System_Scpi_WriteString(command);
			amb7300_NA[tfSite]->System_Scpi_ReadString(Val);		// Follow index of options in Display > Allocate Channels
			Val = Val->Trim();

			if (Val == "1")
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1" };
			}
			else if ((Val == "2") | (Val == "3"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2" };
			}
			else if ((Val == "4") | (Val == "5") | (Val == "6"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3" };
			}
			else if ((Val == "7") | (Val == "8"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4" };
			}
			else if ((Val == "9") | (Val == "10"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4","5","6" };
			}
			else if ((Val == "11") | (Val == "12"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4","5","6","7","8" };
			}
			else if (Val == "13")
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4","5","6","7","8","9" };
			}
			else if ((Val == "14") | (Val == "15"))
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4","5","6","7","8","9","10","11","12" };
			}
			else if (Val == "16")
			{
				projectStateInfo.Channels = gcnew array<String^>{ "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16" };
			}
			
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_TOTAL_ACTIVE_CHANNEL_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetChannelandTraceSetting_CMT] Fail to get total active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetChannelandTraceSetting_CMT] Fail to get total active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

		try
		{
			StateFile_Channels = gcnew array<String^>(projectStateInfo.Channels->Length);
			StateFile_Channels = projectStateInfo.Channels;
			SharedMemFormatData->FreqPoints = gcnew array<array<double>^>(StateFile_Channels->Length);
			SharedMemFormatData->FirstTrace = gcnew array<String^>(StateFile_Channels->Length);

			array<int> ^ vnaChannel = gcnew array<int>(StateFile_Channels->Length);

			// Assign total freq points to each channel arrays
			for (int i = 0; i < StateFile_Channels->Length; i++)
			{
				int channel = i + 1;
				command = String::Format("CALC{0}:PAR1:SEL \n", channel);		// Select 1st trace in a channel
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				command = String::Format("CALC{0}:PAR:COUN? \n", channel);			//total trace in a channel
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);	// 4

				totalTrace = Double::Parse(Val) + totalTrace;		//sum up all traces in a sta file

				//command = String::Format("CALC{0}:PAR:DEF?\n", channel);
				//amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				//amb7300_NA[tfSite]->System_Scpi_ReadString(Val);	// "CH1_S11_1,S22,CH1_S11_2,S21,CH1_S11_3,S12,CH1_S11_4,S22"

				//FirstTrace_Temp = Val->Split(',');					// {"CH1_S11_1,S22,CH1_S11_2,S21,CH1_S11_3,S12,CH1_S11_4,S22"}
				//FirstTrace_Temp = FirstTrace_Temp[0]->Split('_');	// {"CH1,S11,1}

																	// Get First Trace number from each Channel
				//if (FirstTrace_Temp->Length == 3)
				//{
				//	SharedMemFormatData->FirstTrace[i] = FirstTrace_Temp[2]; //pass in trace number into string array
				//}
				SharedMemFormatData->FirstTrace[i] = "1"; //pass in trace number into string array

				command = String::Format("CALC{0}:DATA:XAX? \n", channel); //all freq in a channel
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);			// +1.00000000000E+009,+1.00100000000E+009,+2.00000000000E+009,+2.00100000000E+009

				Freqs = Val->Split(',');									// {+1.00000000000E+009,+1.00100000000E+009,+2.00000000000E+009,+2.00100000000E+009}

																			// Get FreqPoint value for each sweep point
				SharedMemFormatData->FreqPoints[i] = gcnew array<double>(Freqs->Length);
				for (int index = 0; index < Freqs->Length; index++)
				{
					try
					{
						temp = Double::Parse(Freqs[index]);
					}
					catch (Exception ^ Ex)
					{
						temp = 0;
					}

					SharedMemFormatData->FreqPoints[i][index] = temp;
				}

				vnaChannel[i] = double::Parse(StateFile_Channels[i]);
			}

		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_GET_CHANNEL_FREQ_POINT_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetChannelandTraceSetting_CMT] Fail to get frequency point from channel." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetChannelandTraceSetting_CMT] Fail to get frequency point from channel." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			return ret;
		}
		//Get Type of Sweep of the channel

		//command = String::Format("SENS:SWE:TYPE? \n");
		//String ^ sweepType = nullptr;
		//array<String^>^ RawData = nullptr;
		//amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(sweepType);

		//try
		//{
		//	if (sweepType->Contains("LIN"))
		//	{
		//		segmentSetting[vnaSiteIndex].segmentCount	= 1;
		//		segmentSetting[vnaSiteIndex].startFreq		= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].stopFreq		= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].point			= gcnew array<int>(1);
		//		segmentSetting[vnaSiteIndex].ifbw			= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].power			= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].delay			= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].singleMarker	= gcnew array<double>(1);
		//		segmentSetting[vnaSiteIndex].totalPoint		= 0;

		//		//Get Point
		//		command = String::Format("SENS{0}:SWE:POIN? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		String^ readBack = nullptr;
		//		array<String^>^ RawData = nullptr;
		//		
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(readBack);
		//		RawData = readBack->Split(',');
		//		segmentSetting[vnaSiteIndex].totalPoint = static_cast<int>(double::Parse(RawData[0], CultureInfo::InvariantCulture));
		//		segmentSetting[vnaSiteIndex].point[0]	= static_cast<int>(double::Parse(RawData[0], CultureInfo::InvariantCulture));

		//		//Get Start Frequency
		//		command = String::Format("SENS{0}:FREQ:STAR? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		readBack = nullptr;
		//		RawData = nullptr;
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(readBack);
		//		RawData = readBack->Split(',');
		//		segmentSetting[vnaSiteIndex].startFreq[0] = (double::Parse(RawData[0], CultureInfo::InvariantCulture));

		//		//Get Stop Frequency
		//		command = String::Format("SENS{0}:FREQ:STOP? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		readBack = nullptr;
		//		RawData = nullptr;
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(readBack);
		//		RawData = readBack->Split(',');
		//		segmentSetting[vnaSiteIndex].stopFreq[0] = (double::Parse(RawData[0], CultureInfo::InvariantCulture));

		//		//Get IFBW
		//		command = String::Format("SENS{0}:BAND? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		readBack = nullptr;
		//		RawData = nullptr;
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(readBack);
		//		RawData = readBack->Split(',');
		//		segmentSetting[vnaSiteIndex].ifbw[0] = (double::Parse(RawData[0], CultureInfo::InvariantCulture));

		//		//Get Power
		//		command = String::Format("SOUR{0}:POW? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		readBack = nullptr;
		//		RawData = nullptr;
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(readBack);
		//		RawData = readBack->Split(',');
		//		segmentSetting[vnaSiteIndex].power[0] = (double::Parse(RawData[0], CultureInfo::InvariantCulture));



		//	}
		//	else if (sweepType->Contains("SEGM"))
		//	{

		//		int startSettings = 0;
		//		bool ifbwEnabled = false;
		//		bool powerEnabled = false;
		//		bool delayEnabled = false;
		//		int segmentCount = 0;

		//		command = String::Format("SENS{0}:SEGM:DATA? \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
		//		String ^ Val = nullptr;
		//		array<String^>^ RawData = nullptr;
		//		amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
		//		RawData = Val->Split(',');
		//		for (int i = 0; i < RawData->Length; i++)
		//		{
		//			RawData[i] = RawData[i]->Trim();
		//		}

		//		double temp = 0.0;

		//		temp = double::Parse(RawData[1], CultureInfo::InvariantCulture);
		//		startSettings = static_cast<int>(temp);
		//		temp = double::Parse(RawData[2], CultureInfo::InvariantCulture);
		//		ifbwEnabled = static_cast<int>(temp);
		//		temp = double::Parse(RawData[3], CultureInfo::InvariantCulture);
		//		powerEnabled = static_cast<int>(temp);
		//		temp = double::Parse(RawData[4], CultureInfo::InvariantCulture);
		//		delayEnabled = static_cast<int>(temp);
		//		temp = double::Parse(RawData[6], CultureInfo::InvariantCulture);
		//		segmentCount = static_cast<int>(temp);

		//		segmentSetting[vnaSiteIndex].segmentCount	= segmentCount;
		//		segmentSetting[vnaSiteIndex].startFreq		= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].stopFreq		= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].point			= gcnew array<int>(segmentCount);
		//		segmentSetting[vnaSiteIndex].ifbw			= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].power			= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].delay			= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].singleMarker	= gcnew array<double>(segmentCount);
		//		segmentSetting[vnaSiteIndex].totalPoint		= 0;

		//		
		//		int nextSegmentCount = 3 + ifbwEnabled + powerEnabled + delayEnabled;

		//		for (int i = 0; i < segmentCount; i++)
		//		{
		//			int startCount = 7;
		//			segmentSetting[vnaSiteIndex].startFreq[i] = double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture);
		//			startCount++;
		//			segmentSetting[vnaSiteIndex].stopFreq[i] = double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture);
		//			startCount++;
		//			segmentSetting[vnaSiteIndex].point[i] = static_cast<int>(double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture));
		//			startCount++;
		//			segmentSetting[vnaSiteIndex].totalPoint += segmentSetting[vnaSiteIndex].point[i];

		//			if (ifbwEnabled)
		//			{
		//				segmentSetting[vnaSiteIndex].ifbw[i] = double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture);
		//				startCount++;
		//			}
		//			if (powerEnabled)
		//			{
		//				segmentSetting[vnaSiteIndex].power[i] = double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture);
		//				startCount++;
		//			}
		//			if (delayEnabled)
		//			{
		//				segmentSetting[vnaSiteIndex].delay[i] = double::Parse(RawData[startCount + (i * nextSegmentCount)], CultureInfo::InvariantCulture);
		//				startCount++;
		//			}
		//		}

		//	}



		//	//temp = Double::Parse(RawData2[0]);

		//	//amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString()
		//	//amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString()
		//	//segmentSetting[vnaSiteIndex].stateSweepType = 


		//}
		//catch (Exception^ ex)
		//{

		//}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Trigger Settings [Stimulus->Trigger]
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureTriggerSource_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerSource_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure vna's Stimulus -> Trigger -> Trigger Source
		**		Hold all channel and set 'Trigger Source' to 'BUS'
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Stimulus hold all channels
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->TriggerHoldALL());
			
			// Set 'Trigger Source' to 'BUS', will apply for all channels
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SetTriggerInSource((int)(TRIG_SOURCE::BUS)));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_SOURCE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerSource_CMT] Fail to configure trigger source." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerSource_CMT] Fail to configure trigger source." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureTriggerExternal_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerExternal_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to ...
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for future development.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_EXTERNAL_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerExternal_CMT] Fail to configure trigger external." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerExternal_CMT] Fail to configure trigger external." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureTriggerOutput_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerOutput_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to ...
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for future development.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_OUTPUT_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerOutput_CMT] Fail to configure trigger output." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerOutput_CMT] Fail to configure trigger output." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureTriggerScope_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerScope_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to ...
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for future development.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_SCOPE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerScope_CMT] Fail to configure trigger scope." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTriggerScope_CMT] Fail to configure trigger scope." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureSingleTriggerForGetFromStateFileType_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSingleTriggerForGetFromStateFileType_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to send a single trigger to the vna for GetFromStateFile mode only.
		******************************************************************************************************/

		// Local variable
		int ret			= 0;
		String ^ OPC	= String::Empty;

		try
		{
			if (segmentSetting[vnaSiteIndex].GetFromStateFile == true || segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer)
			{
				// Display active channel
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));
				
				// Configure stimulus trigger initial | Need to apply Channel_Stimulus_TriggerInitial() + Channel_Stimulus_TriggerSingle() for GetFromStateFile type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_TriggerInitial(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

				// Send single trigger | Need to apply Channel_Stimulus_TriggerInitial() + Channel_Stimulus_TriggerSingle() for GetFromStateFile type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_TriggerSingle());

				// Wait until sweep is finished
				while (OPC)
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->OPCStatus_Get(OPC));
					if ((OPC->Contains("1")) || (ret == ER_CONST_TERMINATION_CHARACTER_VISA_ERROR)) // [Info] By referring to wrapper, it is using &&, currently change to || and need to keep monitor on it whenever there is project with online debug.
					{
						ret = 0; //reset ret to 0 to solve 0x3FFF0005 termination character VISA error
						break;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GETFROMSTATEFILE_CONFIGURE_SINGLE_TRIGGER_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Fail to configure single trigger for GetFromStateFile type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_CMT] Fail to configure single trigger for GetFromStateFile type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureActiveChannelAndSingleTrigger_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureActiveChannelAndSingleTrigger_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure active channel,
		**		hold all channel, and send single trigger to the active channel.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Display active channel
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

			// Arm wait for data --> including: hold all channel, init target channel, send single trigger, get opc
			tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->ArmWaitForData(vnaSetting[vnaSiteIndex].pinAlias));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_ARM_WAIT_FOR_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Fail to configure active channel, arm and wait for data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureActiveChannelAndSingleTrigger_CMT] Fail to configure active channel, arm and wait for data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Trace Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureTraces_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTraces_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to: 
		**		- to configure number of traces in a selected channel.
		**		- to configure allocate traces in a selected channel.
		**		- to configure the measurement parameter (S11/S21/S12/S22) on the specified trace.
		**		- to configure the format (MLOG/SWR/...) on the specified trace.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		int traceParameterType	= 999;
		int tracePort1			= 999;
		int tracePort2			= 999;

		try
		{
			// Display active channel
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

#pragma region "[Display->Num Of Traces]"

			// Setup number of traces --> fix to max 16x traces
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_NumOfTrace_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].userTraceCount));

#pragma endregion

#pragma region "[Display->Allocate Traces]"

			if (vnaSetting[vnaSiteIndex].userTraceCount <= 1)
			{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_AllocateTrace(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, 1));
			}
			else if (vnaSetting[vnaSiteIndex].userTraceCount <= 2)
			{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_AllocateTrace(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, 3));
			}
			else if (vnaSetting[vnaSiteIndex].userTraceCount <= 4)
			{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_AllocateTrace(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, 8));
			}
			else
			{
				// Setup traces allocation for the selected channel --> fix to max 16x traces
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_AllocateTrace(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, 16));
			}

#pragma endregion

#pragma region "[Measurement->S11/S12/S21/S22/...] & [Format->MLOG/SWR/...]"

			// For every trace that specified by the users
			for (int iTraceIndex = 0; iTraceIndex < vnaSetting[vnaSiteIndex].vna_trace->Length; iTraceIndex++)
			{
				if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] != Vna_Measurement_Type_Reset)
				{

#pragma region "Get measurement parameter based on user input measure type (without smo)"

					// [Future Enchancement] Need to handle s3p & s4p selection.

					// Trace parameter type
					if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("S"))
						traceParameterType = (int)VNATraceParameterType::TraceParameterType_S;
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("T"))
						traceParameterType = (int)VNATraceParameterType::TraceParameterType_T;
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("R"))
						traceParameterType = (int)VNATraceParameterType::TraceParameterType_R;
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("A"))
						traceParameterType = (int)VNATraceParameterType::TraceParameterType_A;
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("B"))
						traceParameterType = (int)VNATraceParameterType::TraceParameterType_B;

					// Trace parameter port
					if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S11)
					{
						tracePort1	= 1;
						tracePort2	= 1;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S12)
					{
						tracePort1	= 1;
						tracePort2	= 2;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S13)
					{
						tracePort1	= 1;
						tracePort2	= 3;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S14)
					{
						tracePort1	= 1;
						tracePort2	= 4;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S21)
					{
						tracePort1	= 2;
						tracePort2	= 1;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S22)
					{
						tracePort1	= 2;
						tracePort2	= 2;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S23)
					{
						tracePort1	= 2;
						tracePort2	= 3;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S24)
					{
						tracePort1	= 2;
						tracePort2	= 4;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S31)
					{
						tracePort1	= 3;
						tracePort2	= 1;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S32)
					{
						tracePort1	= 3;
						tracePort2	= 2;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S33)
					{
						tracePort1	= 3;
						tracePort2	= 3;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S34)
					{
						tracePort1	= 3;
						tracePort2	= 4;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S41)
					{
						tracePort1	= 4;
						tracePort2	= 1;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S42)
					{
						tracePort1	= 4;
						tracePort2	= 2;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S43)
					{
						tracePort1	= 4;
						tracePort2	= 3;
					}
					else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S44)
					{
						tracePort1	= 4;
						tracePort2	= 4;
					}

#pragma endregion

					// Select the active trace at the vna sfp
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveTrace_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].vna_trace[iTraceIndex]));

					// Update the measurement parameter at the vna sfp (eg: S11/S21/S12/S22) on the active trace | measurementParameter scpi 0=S11, 1=S21, 2=S12, 3=S22
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Measurement_Para_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].vna_trace[iTraceIndex], 
						traceParameterType, tracePort1, tracePort2));
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Measurement_Parameter_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].vna_trace[iTraceIndex], 
					//	0, measurementParameter));
				
					// Update the format at the vna sfp (eg: MLOG / SWR / ...)
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Format_Calc_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].vna_trace[iTraceIndex], 
						vnaSetting[vnaSiteIndex].vna_traceFormat[iTraceIndex]));
				}
			}

#pragma endregion
		
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRACE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTraces_CMT] Fail to configure trace setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureTraces_CMT] Fail to configure trace setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Vna Data Analysis Function
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::GetTraceFormatData_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetTraceFormatData_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to read formated data from the target trace, and store in TraceFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// [Future Enchancement] To eval and handle Marker_ReferenceMarker_Set()
		try
		{
			traceFormatData->totalFreqPoints	= segmentSetting[vnaSiteIndex].totalPoint;
			traceFormatData->XAxis				= gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->realRaw			= gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->imaginaryRaw		= gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->freqPoint			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Real			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Imaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Real			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Imaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Real			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Imaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Real			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Imaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			
			// Get trace format data from the selective channel and trace
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
				traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));

			// [CHECK] if total freq points from user does not match with the actual trace data points
			if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
			{
				ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				goto EndOfTest;
			}

			// Store trace data accordingly
			for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
				{ 
					if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i]		= traceFormatData->XAxis[i];
						traceFormatData->S11Real[vnaSiteIndex, i]		= traceFormatData->realRaw[i];
						traceFormatData->S11Imaginary[vnaSiteIndex, i]	= traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i]		= traceFormatData->XAxis[i];
						traceFormatData->S21Real[vnaSiteIndex, i]		= traceFormatData->realRaw[i];
						traceFormatData->S21Imaginary[vnaSiteIndex, i]	= traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i]		= traceFormatData->XAxis[i];
						traceFormatData->S12Real[vnaSiteIndex, i]		= traceFormatData->realRaw[i];
						traceFormatData->S12Imaginary[vnaSiteIndex, i]	= traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i]		= traceFormatData->XAxis[i];
						traceFormatData->S22Real[vnaSiteIndex, i]		= traceFormatData->realRaw[i];
						traceFormatData->S22Imaginary[vnaSiteIndex, i]	= traceFormatData->imaginaryRaw[i];
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerSearchFormatData_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchFormatData_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search and read data (MinMax, Peak, Target)
		**		from the target trace and store in MarkerSearchFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			if ((vnaDataAnalysisTPC.function == Cond_Function_msMinMax) || (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax))
			{ 
				/*
				**	Marker Search Min Max Description:
				**		Search the maximum or minimum value based on the specific frequency search range or the entire sweep range .
				**		1. Check the existing markers. Add or remove marker if does not meet the test requirement.
				**		2. Select the active marker.
				**		3. Specifies the SearchStart frequency & SearchStop frequency, and then turn-on SearchRange.
				**		   If enable the SearchRange, the search mode will only execute in the selective frequency range.
				**		   If disable the SearchRange, the search mode will execute in the entire sweep range.
				**		   [Markers --> Marker Search --> Search Range]
				**		   [Markers --> Marker Search --> Search Start]
				**		   [Markers --> Marker Search --> Search Stop]
				**		4. Select the search mode: Maximum, Minimum.
				**		   Execute the search mode.
				**		   [Markers --> Marker Search]
				**		5. Retrieve the trace data.
				**		
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				msSetting[vnaSiteIndex].targetMarkerCount		= vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].ms_SearchStart			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msMinMax_Type			= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_SearchAtt^> ^ markerSearchSetting	= gcnew array<Marker_SearchAtt^>(msSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerSearchSetting[i]						= gcnew Marker_SearchAtt();
					markerSearchSetting[i]->_search_Start		= 0.0 MHz;									// default
					markerSearchSetting[i]->_search_Stop		= 0.0 MHz;									// default
					msSetting[vnaSiteIndex].msMinMax_Type[i]	= (int)(MarkerSearchType::FUNC_MAX_TYPE);	// default

					// Marker search range enable
					if (analysisSetting.msMinMax_SearchRangeEnable[i] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						markerSearchSetting[i]->_searchRange_Enable = true;

						// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
						if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) && 
							(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0)	&& 
							(vnaDataAnalysisTPC.testFreqRangeStart[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
						{
							markerSearchSetting[i]->_search_Start	= vnaDataAnalysisTPC.testFreqRangeStart[i];
							markerSearchSetting[i]->_search_Stop	= vnaDataAnalysisTPC.testFreqRangeStop[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerSearchSetting[i]->_search_Start	= 0.0 MHz;
							markerSearchSetting[i]->_search_Stop	= 0.0 MHz;							
							ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}
					}
					else if (analysisSetting.msMinMax_SearchRangeEnable[i] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						markerSearchSetting[i]->_searchRange_Enable = false;
					}

					// Marker search min max type
					if (analysisSetting.msMinMax_SearchType[i] == Cond_AnalysisSetting_MS_Type_Maximum)
					{
						markerSearchSetting[i]->Type				= Marker_SearchType::MAX;
						msSetting[vnaSiteIndex].msMinMax_Type[i]	= (int)(MarkerSearchType::FUNC_MAX_TYPE);
					}
					else if (analysisSetting.msMinMax_SearchType[i] == Cond_AnalysisSetting_MS_Type_Minimum)
					{
						markerSearchSetting[i]->Type				= Marker_SearchType::MIN;
						msSetting[vnaSiteIndex].msMinMax_Type[i]	= (int)(MarkerSearchType::FUNC_MIN_TYPE);
					}
				}
				
				// Variables for marker search min max API
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					msSetting[vnaSiteIndex].ms_SearchStart[i]	= markerSearchSetting[i]->_search_Start;
					msSetting[vnaSiteIndex].ms_SearchStop[i]	= markerSearchSetting[i]->_search_Stop;
				}
			
				// Check existing marker count | add or remove marker by referring to the user input
				msSetting[vnaSiteIndex].markerIndex = 0;
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));

					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > markerSearchSetting->Length)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

						// Add marker
						int markerAdded = (int)markerSearchSetting->Length;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < markerSearchSetting->Length)
					{
						// Add marker
						int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;
					
					// Execute based on the marker search min max type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msMinMax_Type[i]));
				}

				// Result variables
				msFormatData->XAxis			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->realRaw		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->imaginaryRaw	= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->markerType	= gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msFreq		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msReal		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msImaginary	= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msType		= gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					msFormatData->XAxis[i]						= (double)CONST_INVALID_RESULT;
					msFormatData->realRaw[i]					= (double)CONST_INVALID_RESULT;
					msFormatData->imaginaryRaw[i]				= (double)CONST_INVALID_RESULT;
					msFormatData->markerType[i]					= String::Empty;
					msFormatData->msFreq[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
					msFormatData->msReal[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
					msFormatData->msImaginary[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					msFormatData->msType[vnaSiteIndex, i]		= String::Empty;
				}
				
				// Get marker search min max data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
					msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));

				// Store marker search min max data accordingly
				if (msFormatData->XAxis->Length != 0)
				{
					for (int i = 0; i < msFormatData->XAxis->Length; i++)
					{
						msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
						msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
						msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
						msFormatData->msType[vnaSiteIndex, i]		= msFormatData->markerType[i];
					}
				}
				else
				{ 
					ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_msPeak)
			{
				/*
				**	Marker Search Peak Description:
				**		Search the peak value at the specific frequency search range or the entire sweep range.
				**		Able to configure the peak search mode, peak excursion, and peak polarity.
				**		1. Check the existing markers. Add or remove marker if does not meet the test requirement.
				**		2. Select the active marker.
				**		3. Specifies the SearchStart frequency & SearchStop frequency, and then turn-on SearchRange.
				**		   If enable the SearchRange, the search mode will only execute in the selective frequency range.
				**		   If disable the SearchRange, the search mode will execute in the entire sweep range.
				**		   [Markers --> Marker Search --> Search Range]
				**		   [Markers --> Marker Search --> Search Start]
				**		   [Markers --> Marker Search --> Search Stop]
				**		4. Specifies the PeakExcursion.
				**		   [Markers --> Marker Search --> Peak --> Peak Excursion]
				**		5. Select the PeakPolarity type:	Positive	: if the the value in the peak is greater than the values of the adjacent points.
				**											Negative	: if the the value in the peak is smaller than the values of the adjacent points.
				**											Both		: include both condition.
				**		   [Markers --> Marker Search --> Peak --> Peak Polarity]
				**		6. Select the search mode: Search Peak / Search Peak Max / Search Peak Left / Search Peak Right.
				**		   Execute the search mode.
				**		   [Markers --> Marker Search --> Peak]
				**		7. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				msSetting[vnaSiteIndex].targetMarkerCount		= vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].targetMakerFreq			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStart			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_Type				= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_PeakExcursion	= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_PeakPolarity		= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);

				msSetting[vnaSiteIndex].markerIndex = 0;
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					// Marker search peak type
					if (analysisSetting.msPeak_Type[i] == Cond_AnalysisSetting_MSPeak_Type_SearchPeak)
					{
						msSetting[vnaSiteIndex].msPeak_Type[i] = int(PeakSearchType::FUNC_PEAK_TYPE);
					}
					else if (analysisSetting.msPeak_Type[i] == Cond_AnalysisSetting_MSPeak_Type_SearchMaxPeak)
					{
						//msSetting[vnaSiteIndex].msPeak_Type[i] = int(PeakSearchType::); // Unavailable in API
					}
					else if (analysisSetting.msPeak_Type[i] == Cond_AnalysisSetting_MSPeak_Type_SearchPeakLeft)
					{
						msSetting[vnaSiteIndex].msPeak_Type[i] = int(PeakSearchType::FUNC_LEFT_PEAK_TYPE);
					}
					else if (analysisSetting.msPeak_Type[i] == Cond_AnalysisSetting_MSPeak_Type_SearchPeakRight)
					{
						msSetting[vnaSiteIndex].msPeak_Type[i] = int(PeakSearchType::FUNC_RIGHT_PEAK_TYPE);
					}

					// Marker search peak excursion
					msSetting[vnaSiteIndex].msPeak_PeakExcursion[i] = analysisSetting.msPeak_PeakExcursion[i];

					// Marker search peak polarity
					if (analysisSetting.msPeak_PeakPolarity[i] == Cond_AnalysisSetting_MSPeak_PeakPolarity_Positive)
					{
						msSetting[vnaSiteIndex].msPeak_PeakPolarity[i] = int(PPOLType::PPOL_POSITIVE_TYPE);
					}
					else if (analysisSetting.msPeak_PeakPolarity[i] == Cond_AnalysisSetting_MSPeak_PeakPolarity_Negative)
					{
						msSetting[vnaSiteIndex].msPeak_PeakPolarity[i] = int(PPOLType::PPOL_NEGATIVE_TYPE);
					}
					else if (analysisSetting.msPeak_PeakPolarity[i] == Cond_AnalysisSetting_MSPeak_PeakPolarity_Both)
					{
						msSetting[vnaSiteIndex].msPeak_PeakPolarity[i] = int(PPOLType::PPOL_BOTH_POS_NEG_TYPE);
					}

					// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0)											&&
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0)											&&
						(vnaDataAnalysisTPC.testFreqRangeStart[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i])	&&
						(vnaDataAnalysisTPC.testFreq[i] != 0)													&&
						(vnaDataAnalysisTPC.testFreq[i] >= vnaDataAnalysisTPC.testFreqRangeStart[i])			&&
						(vnaDataAnalysisTPC.testFreq[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i]	= vnaDataAnalysisTPC.testFreq[i];
						msSetting[vnaSiteIndex].ms_SearchStart[i]	= vnaDataAnalysisTPC.testFreqRangeStart[i];
						msSetting[vnaSiteIndex].ms_SearchStop[i]	= vnaDataAnalysisTPC.testFreqRangeStop[i];
					}
					// Apply default setting if user input empty or input invalid
					else
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i]	= 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStart[i]	= 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStop[i]	= 0.0 MHz;
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSPEAK_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msPeak] For marker search peak, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msPeak] For marker search peak, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));
					
					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));
					
						// Add marker
						int markerAdded = msSetting[vnaSiteIndex].targetMarkerCount;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Add marker
						int markerAdded = msSetting[vnaSiteIndex].targetMarkerCount - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;

					// Select the active marker 
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerActive_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex));
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchCouple_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						true));  // [Future Enchancement] Previously set to false. Need to clarify what is this function.

					// Configure the generic marker search range start stop and enable the search range  
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						true));
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					//	false)); // [Future Enchancement] Need to clarify what is this function.
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRangeStartStop_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].ms_SearchStart[i], msSetting[vnaSiteIndex].ms_SearchStop[i]));

					// Configure the peak excursion for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerPExcursion_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_PeakExcursion[i]));

					// Configure the peak polarity type for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchTargetPolarity_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_PeakPolarity[i]));

					// Execute based on the marker search peak type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_Type[i]));
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Execute(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex));

					// Result variables
					msFormatData->numberOfMarker	= 0;
					msFormatData->XAxis				= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->realRaw			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->imaginaryRaw		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msFreq			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msReal			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msImaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					for (int j = 0; j < msSetting[vnaSiteIndex].targetMarkerCount; j++)
					{
						msFormatData->XAxis[j]						= (double)CONST_INVALID_RESULT;
						msFormatData->realRaw[j]					= (double)CONST_INVALID_RESULT;
						msFormatData->imaginaryRaw[j]				= (double)CONST_INVALID_RESULT;
						msFormatData->msFreq[vnaSiteIndex, j]		= (double)CONST_INVALID_RESULT;
						msFormatData->msReal[vnaSiteIndex, j]		= (double)CONST_INVALID_RESULT;
						msFormatData->msImaginary[vnaSiteIndex, j]	= (double)CONST_INVALID_RESULT;
					}

					// Get marker search target data
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msFormatData->numberOfMarker, msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw));

					// Store marker search min max data accordingly
					if (msFormatData->XAxis->Length != 0)
					{ 
						for (int i = 0; i < msFormatData->XAxis->Length; i++)
						{
							msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
							msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
							msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
						}
					}
					else
					{
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSPEAK_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msPeak] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msPeak] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_msTarget) || (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget))
			{
				/*
				**	Marker Search Target Description:
				**		Search the frequency based on the target value at the specific frequency search range or the entire sweep range.
				**		Able to configure the target search mode, target value, and target transition.
				**		1. Check the existing markers. Add or remove marker if does not meet the test requirement.
				**		2. Select the active marker.
				**		3. Specifies the SearchStart frequency & SearchStop frequency, and then turn-on SearchRange.
				**		   If enable the SearchRange, the search mode will only execute in the selective frequency range.
				**		   If disable the SearchRange, the search mode will execute in the entire sweep range.
				**		   [Markers --> Marker Search --> Search Range]
				**		   [Markers --> Marker Search --> Search Start]
				**		   [Markers --> Marker Search --> Search Stop]
				**		4. Specifies the TargetValue.
				**		   [Markers --> Marker Search --> Target --> Target Value]
				**		5. Select the TargetTransition type:	Positive	: if the function derivative (trace slope) is positive at the intersection point with the target level.
				**												Negative	: if the function derivative (trace slope) is negative at the intersection point with the target level.
				**												Both		: include both condition.
				**		   [Markers --> Marker Search --> Target --> Target Transition]
				**		6. Select whether to enable the TargetLine display. (unavailable)
				**		   [Markers --> Marker Search --> Target --> Target Line]
				**		7. Select the search mode: Search Target / Search Target Left / Search Target Right.
				**		   Execute the search mode.
				**		   [Markers --> Marker Search --> Target]
				**		8. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				msSetting[vnaSiteIndex].targetMarkerCount			= vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].targetMakerFreq				= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStart				= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop				= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_Type				= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetValue		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetTransition	= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetLineEnable	= gcnew array<bool>(msSetting[vnaSiteIndex].targetMarkerCount);

				msSetting[vnaSiteIndex].markerIndex = 0;
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					// Marker search target type
					if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTarget)
					{
						msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_TARGET_TYPE);
					}
					else if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTargetLeft)
					{
						msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_LEFT_TARGET_TYPE);
					}
					else if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTargetRight)
					{
						msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_RIGHT_TARGET_TYPE);
					}

					// Marker search target value
					msSetting[vnaSiteIndex].msTarget_TargetValue[i] = analysisSetting.msTarget_TargetValue[i];

					// Marker search target transition type
					if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Positive)
					{
						msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_POSITIVE_TYPE);
					}
					else if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Negative)
					{
						msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_NEGATIVE_TYPE);
					}
					else if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Both)
					{
						msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_BOTH_POS_NEG_TYPE);
					}
					
					//// Marker search target line enable
					//if (analysisSetting.msTarget_TargetLineEnable[i] == Cond_AnalysisSetting_MSTarget_Enable_TargetLineOn)
					//{
					//	msSetting[vnaSiteIndex].msTarget_TargetLineEnable[i] = true;
					//}
					//else if (analysisSetting.msTarget_TargetLineEnable[i] == Cond_AnalysisSetting_MSTarget_Enable_TargetLineOff)
					//{
					//	msSetting[vnaSiteIndex].msTarget_TargetLineEnable[i] = false;
					//}

					// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0)											&&
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0)											&&
						(vnaDataAnalysisTPC.testFreqRangeStart[i] < vnaDataAnalysisTPC.testFreqRangeStop[i])	&&
						(vnaDataAnalysisTPC.testFreq[i] != 0)													&&
						(vnaDataAnalysisTPC.testFreq[i] >= vnaDataAnalysisTPC.testFreqRangeStart[i])			&&
						(vnaDataAnalysisTPC.testFreq[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i]	= vnaDataAnalysisTPC.testFreq[i];
						msSetting[vnaSiteIndex].ms_SearchStart[i]	= vnaDataAnalysisTPC.testFreqRangeStart[i];
						msSetting[vnaSiteIndex].ms_SearchStop[i]	= vnaDataAnalysisTPC.testFreqRangeStop[i];
					}
					// Apply default setting if user input empty or input invalid
					else
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i]	= 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStart[i]	= 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStop[i]	= 0.0 MHz;
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));

					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;

					// Select the active marker 
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerActive_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
						msSetting[vnaSiteIndex].markerIndex));
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchCouple_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
						true)); // [Future Enchancement] Need to clarify what is this function.

					// Configure the generic marker search range start stop and enable the search range  
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
						true));
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRangeStartStop_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
						msSetting[vnaSiteIndex].ms_SearchStart[i], msSetting[vnaSiteIndex].ms_SearchStop[i]));

					// Configure the target value for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerPExcursionTargetValue_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_TargetValue[i]));

					// Configure the target transition type for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchTargetTransitionType_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].msTarget_TargetTransition[i]));

					// Execute based on the marker search target type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_Type[i]));

					// Result variables
					msFormatData->numberOfMarker	= 0;
					msFormatData->XAxis				= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->realRaw			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->imaginaryRaw		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msFreq			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msReal			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msImaginary		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					for (int j = 0; j < msSetting[vnaSiteIndex].targetMarkerCount; j++)
					{
						msFormatData->XAxis[j]						= (double)CONST_INVALID_RESULT;
						msFormatData->realRaw[j]					= (double)CONST_INVALID_RESULT;
						msFormatData->imaginaryRaw[j]				= (double)CONST_INVALID_RESULT;
						msFormatData->msFreq[vnaSiteIndex, j]		= (double)CONST_INVALID_RESULT;
						msFormatData->msReal[vnaSiteIndex, j]		= (double)CONST_INVALID_RESULT;
						msFormatData->msImaginary[vnaSiteIndex, j]	= (double)CONST_INVALID_RESULT;
					}

					// Get marker search target data
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						msFormatData->numberOfMarker, msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw));

					// Store marker search min max data accordingly
					if (msFormatData->XAxis->Length != 0)
					{
						for (int i = 0; i < msFormatData->XAxis->Length; i++)
						{
							msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
							msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
							msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
						}
					}
					else
					{
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerMathFormatData_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerMathFormatData_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker math and read data (Statistic, BwSearch, Flatness) 
		**		from the target trace and store in MarkerMathFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				/*
				**	Marker Math Statistics Description:
				**	This feature allows the user to determine and view such trace parameters as mean, standard deviation, and peak-to-peak. 
				**	The trace statistics range can be defined by two markers.
				**		1. Remove all existing markers.
				**		2. Specifies the StatisticsStart & StatisticsStop, and then turn-on StatisticsRange.
				**		   If enable the StatisticsRange, it will determine the statistics based on the selective start stop range.
				**		   If disable the StatisticsRange, it will determine the entire sweep range.
				**		   [Markers --> Marker Math --> Statistics --> Statistics Range]
				**		   [Markers --> Marker Math --> Statistics --> Statistics Start]
				**		   [Markers --> Marker Math --> Statistics --> Statistics Stop]
				**		3. Enable the marker math statistics.
				**		   [Markers --> Marker Math --> Statistics]
				**		4. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				mmSetting[vnaSiteIndex].targetMarkerCount					= vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable	= gcnew array<bool>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart		= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting					= gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i]			= gcnew Marker_MathAtt();
					markerMathSetting[i]->Type		= Marker_MathType::Statistic;
					markerMathSetting[i]->Marker1	= 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2	= 0.0 MHz;	// Default

					// mmStatistics StatisticsRange
					if (analysisSetting.mmStatistics_StatisticsRangeEnable[i] == Cond_AnalysisSetting_MMStatistics_Enable_StatisticsRangeOn)
					{
						markerMathSetting[i]->StatisticRangeEnable = true;

						// Make sure both marker1 and marker2 does not contain 0Hz | if marker1 and marker2 reverse in value accending order does not affect statistics test
						if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
							(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0)	&&
							(vnaDataAnalysisTPC.testFreqRangeStart[i] != vnaDataAnalysisTPC.testFreqRangeStop[i]))
						{
							markerMathSetting[i]->Marker1 = vnaDataAnalysisTPC.testFreqRangeStart[i];
							markerMathSetting[i]->Marker2 = vnaDataAnalysisTPC.testFreqRangeStop[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerMathSetting[i]->Marker1 = 0.0 MHz;
							markerMathSetting[i]->Marker2 = 0.0 MHz;
							ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMSTATISTICS_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmStatistics] If enable StatisticsRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmStatistics] If enable StatisticsRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}
					}
					else if (analysisSetting.mmStatistics_StatisticsRangeEnable[i] == Cond_AnalysisSetting_MMStatistics_Enable_StatisticsRangeOff)
					{
						markerMathSetting[i]->StatisticRangeEnable = false;
					}
				}

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable[i]	= markerMathSetting[i]->StatisticRangeEnable;
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart[i]			= markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop[i]			= markerMathSetting[i]->Marker2;
				}

				// Delete all existing marker because the wrapper will create and always use marker #1 & #2
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

				// Result variables
				mmFormatData->mmStatistics_MeanRaw			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_StdDevRaw		= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_PeakToPeakRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_Mean				= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_StdDev			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_PeakToPeak		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmStatistics_MeanRaw[i]					= (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_StdDevRaw[i]					= (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_PeakToPeakRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_Mean[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
				}

				// Get marker math statistics data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadStatisticData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable, mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart, mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop,
					mmFormatData->mmStatistics_MeanRaw, mmFormatData->mmStatistics_StdDevRaw, mmFormatData->mmStatistics_PeakToPeakRaw));

				// Store marker math statistics data accordingly
				if (mmFormatData->mmStatistics_MeanRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmStatistics_MeanRaw->Length; i++)
					{
						mmFormatData->mmStatistics_Mean[vnaSiteIndex, i]		= mmFormatData->mmStatistics_MeanRaw[i];
						mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i]		= mmFormatData->mmStatistics_StdDevRaw[i];
						mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i]	= mmFormatData->mmStatistics_PeakToPeakRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMSTATISTICS_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmStatistics] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmStatistics] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)
			{
				/*
				**	Marker Math Bandwidth Search Description:
				**	This feature allows the user to determine and view the following parameters of a passband or a stopband: bandwidth, center frequency, lower frequency, higher frequency, Q value, and insertion loss. 
				**	The bandwidth search is executed from the reference point. 
				**	The user can select as reference point the active marker or the maximum of the trace. 
				**	The bandwidth search function determines the lower and higher cutoff frequencies, which are apart from the reference point response by bandwidth value defined by the user (usually �3 dB).
				**		1. If the SearchRef is using Marker option, remove all existing markers.
				**		   Else the existing markers will be ignored during the measurement.
				**		2. Configure the active marker if it is using SearchRef Marker option. 
				**		3. Select the bandwidth search Type.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Type]
				**		4. Select the bandwidth search reference
				**		   [Markers --> Marker Math --> Bandwidth Search --> Search Ref]
				**		5. Specifies the bandwidth value.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Bandwidth Value]
				**		6. Enable the marker math bandwidth search.
				**		   [Markers --> Marker Math --> Bandwidth Search]
				**		7. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				mmSetting[vnaSiteIndex].targetMarkerCount			= vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmBwSearch_Type				= gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_SearchRef		= gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker1			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker2			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting			= gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);
				
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i]			= gcnew Marker_MathAtt();
					markerMathSetting[i]->Type		= Marker_MathType::BandWidth_Search;
					markerMathSetting[i]->Marker1	= 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2	= 0.0 MHz;	// Default

					// mmBwSearch Type
					if (analysisSetting.mmBwSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::BandPass;
					}
					else if (analysisSetting.mmBwSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::Notch;
					}

					// mmBwSearch SearchRef
					if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Marker;

						// Make sure marker1 does not contain 0Hz
						if (vnaDataAnalysisTPC.testFreq[i] != 0)
						{
							// Wrapper only apply marker1 for the BwSearchRef: Marker option
							markerMathSetting[i]->Marker1 = vnaDataAnalysisTPC.testFreq[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerMathSetting[i]->Marker1 = 0.0 MHz;
							ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}

						// Delete all existing marker if using Marker as ref because the wrapper will create and always use marker #1
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));
					}
					else if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Max;
					}
					else if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::MIN;
					}

					// mmBwSearch BwValue
					markerMathSetting[i]->BWValue = analysisSetting.mmBwSearch_BandwidthValue[i];
				}
				
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmBwSearch_Type[i]				= (int)(markerMathSetting[i]->Marker_BWSearchType);
					mmSetting[vnaSiteIndex].mmBwSearch_SearchRef[i]			= (int)(markerMathSetting[i]->Marker_BWSearchRef);
					mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i]	= markerMathSetting[i]->BWValue;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker1[i]			= markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker2[i]			= markerMathSetting[i]->Marker2;
				}
				
				// Result variables
				mmFormatData->mmBwSearch_BWRaw		= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_CentRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LowRaw		= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_HighRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_QRaw		= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LossRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_BW			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Cent		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Low		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_High		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Q			= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Loss		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmBwSearch_BWRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_CentRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LowRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_HighRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_QRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LossRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_BW[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Low[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_High[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Q[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
				}

				// Get bandwidth search data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
					mmSetting[vnaSiteIndex].mmBwSearch_Type, mmSetting[vnaSiteIndex].mmBwSearch_SearchRef, mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue, mmSetting[vnaSiteIndex].mmBwSearch_Marker1, mmSetting[vnaSiteIndex].mmBwSearch_Marker2,
					mmFormatData->mmBwSearch_BWRaw, mmFormatData->mmBwSearch_CentRaw,
					mmFormatData->mmBwSearch_LowRaw, mmFormatData->mmBwSearch_HighRaw,
					mmFormatData->mmBwSearch_QRaw, mmFormatData->mmBwSearch_LossRaw));

				// Store marker math bandwidth search data accordingly
				if (mmFormatData->mmBwSearch_BWRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmBwSearch_BWRaw->Length; i++)
					{
						mmFormatData->mmBwSearch_BW[vnaSiteIndex, i]	= mmFormatData->mmBwSearch_BWRaw[i];
						mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i]	= mmFormatData->mmBwSearch_CentRaw[i];
						mmFormatData->mmBwSearch_Low[vnaSiteIndex, i]	= mmFormatData->mmBwSearch_LowRaw[i];
						mmFormatData->mmBwSearch_High[vnaSiteIndex, i]	= mmFormatData->mmBwSearch_HighRaw[i];
						mmFormatData->mmBwSearch_Q[vnaSiteIndex, i]		= mmFormatData->mmBwSearch_QRaw[i];
						mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i]	= mmFormatData->mmBwSearch_LossRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs)
			{
				/*
				**	Marker Math Bandwidth Search Absolute Description:
				**	This feature allows the user to determine and view the following parameters of a passband or a stopband: bandwidth, center frequency, lower frequency, higher frequency, Q value, and insertion loss.
				**	The bandwidth search is executed from the reference point.
				**	The user can select as reference point the active marker or the maximum of the trace.
				**	The bandwidth search function determines the lower and higher cutoff frequencies, which are apart from the reference point response by bandwidth value defined by the user (usually �3 dB).
				**		1. If the SearchRef is using Marker option, remove all existing markers.
				**		   Else the existing markers will be ignored during the measurement.
				**		2. Configure the active marker if it is using SearchRef Marker option.
				**		3. Select the bandwidth search Type.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Type]
				**		4. Select the bandwidth search reference
				**		   [Markers --> Marker Math --> Bandwidth Search --> Search Ref]
				**		5. Specifies the bandwidth value.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Bandwidth Value]
				**		6. Enable the marker math bandwidth search.
				**		   [Markers --> Marker Math --> Bandwidth Search]
				**		7. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				mmSetting[vnaSiteIndex].targetMarkerCount			= vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmBwSearch_Type				= gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_SearchRef		= gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker1			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker2			= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting			= gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i] = gcnew Marker_MathAtt();
					markerMathSetting[i]->Type = Marker_MathType::BandWidth_Search;
					markerMathSetting[i]->Marker1 = 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2 = 0.0 MHz;	// Default

																// mmBwSearch Type
					if (analysisSetting.mmBwSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::BandPass;
					}
					else if (analysisSetting.mmBwSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::Notch;
					}

					// mmBwSearch SearchRef
					if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Marker;

						// Make sure marker1 does not contain 0Hz
						if (vnaDataAnalysisTPC.testFreq[i] != 0)
						{
							// Wrapper only apply marker1 for the BwSearchRef: Marker option
							markerMathSetting[i]->Marker1 = vnaDataAnalysisTPC.testFreq[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerMathSetting[i]->Marker1 = 0.0 MHz;
							ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}

						// Delete all existing marker if using Marker as ref because the wrapper will create and always use marker #1
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));
					}
					else if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Max;

						////Get Peak Value
						//msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
						//msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(1);
						//msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(1);
						//array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(1);

						//markerSearchSetting[0] = gcnew Marker_SearchAtt();
						//markerSearchSetting[0]->_search_Start = 0.0 MHz;									// default
						//markerSearchSetting[0]->_search_Stop = 0.0 MHz;									// default
						//msSetting[vnaSiteIndex].ms_SearchStart = msSetting[vnaSiteIndex].ms_SearchStart;
						//msSetting[vnaSiteIndex].ms_SearchStop = msSetting[vnaSiteIndex].ms_SearchStop;
						//markerSearchSetting[i]->Type = Marker_SearchType::MAX;
						//msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE);	// default
						//markerSearchSetting[0]->_searchRange_Enable = false;

						//// Check existing marker count | add or remove marker by referring to the user input
						//msSetting[vnaSiteIndex].markerIndex = 0;
						//for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
						//{
						//	// Check the existing markers on the target trace
						//	int existingMarkerCount = 0;
						//	tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));

						//	// If existing marker more than needed marker --> remove extra marker
						//	if (existingMarkerCount > markerSearchSetting->Length)
						//	{
						//		// Delete all marker 
						//		tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

						//		// Add marker
						//		int markerAdded = (int)markerSearchSetting->Length;
						//		for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}
						//	// If existing marker less than needed marker --> add extra marker
						//	else if (existingMarkerCount < markerSearchSetting->Length)
						//	{
						//		// Add marker
						//		int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
						//		for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}
						//	else
						//	{
						//		// Re-define the marker frequency
						//		for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}

						//	// Record current marker index
						//	msSetting[vnaSiteIndex].markerIndex = i + 1;

						//	// Execute based on the marker search min max type
						//	tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						//		msSetting[vnaSiteIndex].markerIndex, (int)(MarkerSearchType::FUNC_MAX_TYPE)));
						//}

						//// Result variables
						//msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->markerType = gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msType = gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);

						//msFormatData->XAxis[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->realRaw[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->imaginaryRaw[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->markerType[0] = String::Empty;
						//msFormatData->msFreq[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msReal[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msImaginary[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msType[vnaSiteIndex, 0] = String::Empty;

						//// Get marker search min max data
						//amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, false);
						//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
						//	msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));
						////amb7300_NA[vnaSiteIndex]->Marker_ReadData()

						//// Store marker search min max data accordingly
						//if (msFormatData->XAxis->Length != 0)
						//{
						//	for (int i = 0; i < msFormatData->XAxis->Length; i++)
						//	{
						//		msFormatData->msFreq[vnaSiteIndex, i] = msFormatData->XAxis[i];
						//		msFormatData->msReal[vnaSiteIndex, i] = msFormatData->realRaw[i];
						//		msFormatData->msImaginary[vnaSiteIndex, i] = msFormatData->imaginaryRaw[i];
						//		msFormatData->msType[vnaSiteIndex, i] = msFormatData->markerType[i];
						//	}
						//}
						//else
						//{
						//	ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
						//	tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						//	tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						//	goto EndOfTest;
						//}

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.mmBwSearch_BandwidthValue[i];

					}
					else if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::MIN;

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.mmBwSearch_BandwidthValue[i];
					}
					
				}

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmBwSearch_Type[i] = (int)(markerMathSetting[i]->Marker_BWSearchType);
					mmSetting[vnaSiteIndex].mmBwSearch_SearchRef[i] = (int)(markerMathSetting[i]->Marker_BWSearchRef);
					mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] = markerMathSetting[i]->BWValue;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker1[i] = markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker2[i] = markerMathSetting[i]->Marker2;
				}

				// Result variables
				mmFormatData->mmBwSearch_BWRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_CentRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LowRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_HighRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_QRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LossRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_BW = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Cent = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Low = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_High = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Q = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Loss = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmBwSearch_BWRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_CentRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LowRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_HighRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_QRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LossRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_BW[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Low[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_High[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Q[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				}

				// Get bandwidth search data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					mmSetting[vnaSiteIndex].mmBwSearch_Type, mmSetting[vnaSiteIndex].mmBwSearch_SearchRef, mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue, mmSetting[vnaSiteIndex].mmBwSearch_Marker1, mmSetting[vnaSiteIndex].mmBwSearch_Marker2,
					mmFormatData->mmBwSearch_BWRaw, mmFormatData->mmBwSearch_CentRaw,
					mmFormatData->mmBwSearch_LowRaw, mmFormatData->mmBwSearch_HighRaw,
					mmFormatData->mmBwSearch_QRaw, mmFormatData->mmBwSearch_LossRaw));

				//AbsBwValueCalculation
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] = mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] - mmFormatData->mmBwSearch_LossRaw[i];
				}

				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					mmSetting[vnaSiteIndex].mmBwSearch_Type, mmSetting[vnaSiteIndex].mmBwSearch_SearchRef, mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue, mmSetting[vnaSiteIndex].mmBwSearch_Marker1, mmSetting[vnaSiteIndex].mmBwSearch_Marker2,
					mmFormatData->mmBwSearch_BWRaw, mmFormatData->mmBwSearch_CentRaw,
					mmFormatData->mmBwSearch_LowRaw, mmFormatData->mmBwSearch_HighRaw,
					mmFormatData->mmBwSearch_QRaw, mmFormatData->mmBwSearch_LossRaw));

				// Store marker math bandwidth search data accordingly
				if (mmFormatData->mmBwSearch_BWRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmBwSearch_BWRaw->Length; i++)
					{
						mmFormatData->mmBwSearch_BW[vnaSiteIndex, i] = mmFormatData->mmBwSearch_BWRaw[i];
						mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i] = mmFormatData->mmBwSearch_CentRaw[i];
						mmFormatData->mmBwSearch_Low[vnaSiteIndex, i] = mmFormatData->mmBwSearch_LowRaw[i];
						mmFormatData->mmBwSearch_High[vnaSiteIndex, i] = mmFormatData->mmBwSearch_HighRaw[i];
						mmFormatData->mmBwSearch_Q[vnaSiteIndex, i] = mmFormatData->mmBwSearch_QRaw[i];
						mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i] = mmFormatData->mmBwSearch_LossRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				/*
				**	Marker Math Statistics Description:
				**	This feature allows the user to determine and view the following trace parameters: gain, slope, and flatness. 
				**	The user sets two markers to specify the flatness search range.
				**		1. Remove all existing markers.
				**		2. Specifies the FlatnessStart & FlatnessStop.
				**		   [Markers --> Marker Math --> Flatness --> Flatness Start]
				**		   [Markers --> Marker Math --> Flatness --> Flatness Stop]
				**		3. Enable the marker math flatness.
				**		   [Markers --> Marker Math --> Flatness]
				**		4. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				mmSetting[vnaSiteIndex].targetMarkerCount	= vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmFlatness_Marker1	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmFlatness_Marker2	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting	= gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);
				
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i]			= gcnew Marker_MathAtt();
					markerMathSetting[i]->Type		= Marker_MathType::Flatness;
					markerMathSetting[i]->Marker1	= 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2	= 0.0 MHz;	// Default

					// Make sure both marker1 and marker2 does not contain 0Hz | if marker1 and marker2 reverse in value accending order does not affect flatness test
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) && 
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0)	&&
						(vnaDataAnalysisTPC.testFreqRangeStart[i] != vnaDataAnalysisTPC.testFreqRangeStop[i]))
					{
						markerMathSetting[i]->Marker1 = vnaDataAnalysisTPC.testFreqRangeStart[i];
						markerMathSetting[i]->Marker2 = vnaDataAnalysisTPC.testFreqRangeStop[i];
					}
					// Apply default setting if user input empty or input invalid
					else
					{
						markerMathSetting[i]->Marker1 = 0.0 MHz;
						markerMathSetting[i]->Marker2 = 0.0 MHz;
						ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMFLATNESS_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmFlatness] For marker math flatness, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmFlatness] For marker math flatness, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}

				for (int i = 0; i < markerMathSetting->Length; i++)
				{
					mmSetting[vnaSiteIndex].mmFlatness_Marker1[i] = markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmFlatness_Marker2[i] = markerMathSetting[i]->Marker2;
				}

				// Delete all existing marker because the wrapper will create and always use marker #1 & #2
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));
				
				// Result variables
				mmFormatData->mmFlatness_GainRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_SlopeRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_FlatRaw	= gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Gain		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Slope		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Flat		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmFlatness_GainRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_SlopeRaw[i]			= (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_FlatRaw[i]				= (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Gain[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Slope[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Flat[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
				}

				// Get flatness data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
					mmSetting[vnaSiteIndex].mmFlatness_Marker1, mmSetting[vnaSiteIndex].mmFlatness_Marker2,
					mmFormatData->mmFlatness_GainRaw, mmFormatData->mmFlatness_SlopeRaw, mmFormatData->mmFlatness_FlatRaw));

				// Store marker math flatness data accordingly
				if (mmFormatData->mmFlatness_GainRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmFlatness_GainRaw->Length; i++)
					{
						mmFormatData->mmFlatness_Gain[vnaSiteIndex, i]	= mmFormatData->mmFlatness_GainRaw[i];
						mmFormatData->mmFlatness_Slope[vnaSiteIndex, i] = mmFormatData->mmFlatness_SlopeRaw[i];
						mmFormatData->mmFlatness_Flat[vnaSiteIndex, i]	= mmFormatData->mmFlatness_FlatRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMFLATNESS_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmFlatness] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmFlatness] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch)
			{
				/*
				**	Marker Math Bandwidth Search Absolute Description:
				**	This feature allows the user to determine and view the following parameters of a passband or a stopband: bandwidth, center frequency, lower frequency, higher frequency, Q value, and insertion loss.
				**	The bandwidth search is executed from the reference point.
				**	The user can select as reference point the active marker or the maximum of the trace.
				**	The bandwidth search function determines the lower and higher cutoff frequencies, which are apart from the reference point response by bandwidth value defined by the user (usually �3 dB).
				**		1. If the SearchRef is using Marker option, remove all existing markers.
				**		   Else the existing markers will be ignored during the measurement.
				**		2. Configure the active marker if it is using SearchRef Marker option.
				**		3. Select the bandwidth search Type.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Type]
				**		4. Select the bandwidth search reference
				**		   [Markers --> Marker Math --> Bandwidth Search --> Search Ref]
				**		5. Specifies the bandwidth value.
				**		   [Markers --> Marker Math --> Bandwidth Search --> Bandwidth Value]
				**		6. Enable the marker math bandwidth search.
				**		   [Markers --> Marker Math --> Bandwidth Search]
				**		7. Retrieve the trace data.
				**
				**		*** [Enhancement] The array storage is reserved for the multiple marker execution option in the future.
				*/

				// Initialize variable storage
				mmSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmBwSearch_Type = gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_SearchRef = gcnew array<int>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker1 = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmBwSearch_Marker2 = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting = gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i] = gcnew Marker_MathAtt();
					markerMathSetting[i]->Type = Marker_MathType::BandWidth_Search;
					markerMathSetting[i]->Marker1 = 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2 = 0.0 MHz;	// Default

																// mmBwSearch Type
					if (analysisSetting.SharedMemoryDataBWSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::BandPass;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_Type[i] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						markerMathSetting[i]->Marker_BWSearchType = Marker_BWSearchType::Notch;
					}

					//由于工具导入固定，需要用max，如果是marker，也进入max ---xl 260514
					
					// mmBwSearch SearchRef
					if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum)//Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Marker;

						// Make sure marker1 does not contain 0Hz
						if (vnaDataAnalysisTPC.testFreq[i] != 0)
						{
							// Wrapper only apply marker1 for the BwSearchRef: Marker option
							markerMathSetting[i]->Marker1 = vnaDataAnalysisTPC.testFreq[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerMathSetting[i]->Marker1 = 0.0 MHz;
							ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}

						// Delete all existing marker if using Marker as ref because the wrapper will create and always use marker #1
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)//Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Max;

						////Get Peak Value
						//msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
						//msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(1);
						//msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(1);
						//array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(1);

						//markerSearchSetting[0] = gcnew Marker_SearchAtt();
						//markerSearchSetting[0]->_search_Start = 0.0 MHz;									// default
						//markerSearchSetting[0]->_search_Stop = 0.0 MHz;									// default
						//msSetting[vnaSiteIndex].ms_SearchStart = msSetting[vnaSiteIndex].ms_SearchStart;
						//msSetting[vnaSiteIndex].ms_SearchStop = msSetting[vnaSiteIndex].ms_SearchStop;
						//markerSearchSetting[i]->Type = Marker_SearchType::MAX;
						//msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE);	// default
						//markerSearchSetting[0]->_searchRange_Enable = false;

						//// Check existing marker count | add or remove marker by referring to the user input
						//msSetting[vnaSiteIndex].markerIndex = 0;
						//for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
						//{
						//	// Check the existing markers on the target trace
						//	int existingMarkerCount = 0;
						//	tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));

						//	// If existing marker more than needed marker --> remove extra marker
						//	if (existingMarkerCount > markerSearchSetting->Length)
						//	{
						//		// Delete all marker 
						//		tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

						//		// Add marker
						//		int markerAdded = (int)markerSearchSetting->Length;
						//		for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}
						//	// If existing marker less than needed marker --> add extra marker
						//	else if (existingMarkerCount < markerSearchSetting->Length)
						//	{
						//		// Add marker
						//		int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
						//		for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}
						//	else
						//	{
						//		// Re-define the marker frequency
						//		for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						//		{
						//			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
						//		}
						//	}

						//	// Record current marker index
						//	msSetting[vnaSiteIndex].markerIndex = i + 1;

						//	// Execute based on the marker search min max type
						//	tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
						//		msSetting[vnaSiteIndex].markerIndex, (int)(MarkerSearchType::FUNC_MAX_TYPE)));
						//}

						//// Result variables
						//msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->markerType = gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
						//msFormatData->msType = gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);

						//msFormatData->XAxis[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->realRaw[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->imaginaryRaw[0] = (double)CONST_INVALID_RESULT;
						//msFormatData->markerType[0] = String::Empty;
						//msFormatData->msFreq[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msReal[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msImaginary[vnaSiteIndex, 0] = (double)CONST_INVALID_RESULT;
						//msFormatData->msType[vnaSiteIndex, 0] = String::Empty;

						//// Get marker search min max data
						//amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, false);
						//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
						//	msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));
						////amb7300_NA[vnaSiteIndex]->Marker_ReadData()

						//// Store marker search min max data accordingly
						//if (msFormatData->XAxis->Length != 0)
						//{
						//	for (int i = 0; i < msFormatData->XAxis->Length; i++)
						//	{
						//		msFormatData->msFreq[vnaSiteIndex, i] = msFormatData->XAxis[i];
						//		msFormatData->msReal[vnaSiteIndex, i] = msFormatData->realRaw[i];
						//		msFormatData->msImaginary[vnaSiteIndex, i] = msFormatData->imaginaryRaw[i];
						//		msFormatData->msType[vnaSiteIndex, i] = msFormatData->markerType[i];
						//	}
						//}
						//else
						//{
						//	ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
						//	tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						//	tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchFormatData_CMT -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						//	goto EndOfTest;
						//}

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[i];

					}
					else if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::MIN;

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[i];
					}

				}

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmBwSearch_Type[i] = (int)(markerMathSetting[i]->Marker_BWSearchType);
					mmSetting[vnaSiteIndex].mmBwSearch_SearchRef[i] = (int)(markerMathSetting[i]->Marker_BWSearchRef);
					mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] = markerMathSetting[i]->BWValue;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker1[i] = markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmBwSearch_Marker2[i] = markerMathSetting[i]->Marker2;
				}

				// Result variables
				mmFormatData->mmBwSearch_BWRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_CentRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LowRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_HighRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_QRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_LossRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_BW = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Cent = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Low = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_High = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Q = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmBwSearch_Loss = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmBwSearch_BWRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_CentRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LowRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_HighRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_QRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_LossRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_BW[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Low[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_High[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Q[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				}

				// Get bandwidth search data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					mmSetting[vnaSiteIndex].mmBwSearch_Type, mmSetting[vnaSiteIndex].mmBwSearch_SearchRef, mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue, mmSetting[vnaSiteIndex].mmBwSearch_Marker1, mmSetting[vnaSiteIndex].mmBwSearch_Marker2,
					mmFormatData->mmBwSearch_BWRaw, mmFormatData->mmBwSearch_CentRaw,
					mmFormatData->mmBwSearch_LowRaw, mmFormatData->mmBwSearch_HighRaw,
					mmFormatData->mmBwSearch_QRaw, mmFormatData->mmBwSearch_LossRaw));

				//AbsBwValueCalculation
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] = mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue[i] - mmFormatData->mmBwSearch_LossRaw[i];
				}

				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					mmSetting[vnaSiteIndex].mmBwSearch_Type, mmSetting[vnaSiteIndex].mmBwSearch_SearchRef, mmSetting[vnaSiteIndex].mmBwSearch_BandwidthValue, mmSetting[vnaSiteIndex].mmBwSearch_Marker1, mmSetting[vnaSiteIndex].mmBwSearch_Marker2,
					mmFormatData->mmBwSearch_BWRaw, mmFormatData->mmBwSearch_CentRaw,
					mmFormatData->mmBwSearch_LowRaw, mmFormatData->mmBwSearch_HighRaw,
					mmFormatData->mmBwSearch_QRaw, mmFormatData->mmBwSearch_LossRaw));

				// Store marker math bandwidth search data accordingly
				if (mmFormatData->mmBwSearch_BWRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmBwSearch_BWRaw->Length; i++)
					{
						mmFormatData->mmBwSearch_BW[vnaSiteIndex, i] = mmFormatData->mmBwSearch_BWRaw[i];
						mmFormatData->mmBwSearch_Cent[vnaSiteIndex, i] = mmFormatData->mmBwSearch_CentRaw[i];
						mmFormatData->mmBwSearch_Low[vnaSiteIndex, i] = mmFormatData->mmBwSearch_LowRaw[i];
						mmFormatData->mmBwSearch_High[vnaSiteIndex, i] = mmFormatData->mmBwSearch_HighRaw[i];
						mmFormatData->mmBwSearch_Q[vnaSiteIndex, i] = mmFormatData->mmBwSearch_QRaw[i];
						mmFormatData->mmBwSearch_Loss[vnaSiteIndex, i] = mmFormatData->mmBwSearch_LossRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMBWSEARCH_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Fail to get marker math format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerMathFormatData_CMT] Fail to get marker math format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerSearchMinMaxRipple_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchMinMaxRipple_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search min and max in a same freq range.
		**		Perform (max - min) to get the ripple result.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			msSetting[vnaSiteIndex].targetMarkerCount	= 2; // fix for this test function | 1x for msMin, 1x for msMax
			msSetting[vnaSiteIndex].ms_SearchStart		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msSetting[vnaSiteIndex].ms_SearchStop		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msSetting[vnaSiteIndex].msMinMax_Type		= gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);

			Marker_SearchAtt ^ ms1 = gcnew Marker_SearchAtt();
			Marker_SearchAtt ^ ms2 = gcnew Marker_SearchAtt();
			array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(msSetting[vnaSiteIndex].targetMarkerCount);
			
			ms1->Type									= Marker_SearchType::MIN;
			ms1->_searchRange_Enable					= true;
			ms1->_search_Start							= vnaDataAnalysisTPC.testFreqRangeStart[0];
			ms1->_search_Stop							= vnaDataAnalysisTPC.testFreqRangeStop[0];
			markerSearchSetting[0]						= ms1;
			msSetting[vnaSiteIndex].ms_SearchStart[0]	= markerSearchSetting[0]->_search_Start;
			msSetting[vnaSiteIndex].ms_SearchStop[0]	= markerSearchSetting[0]->_search_Stop;
			msSetting[vnaSiteIndex].msMinMax_Type[0]	= (int)(MarkerSearchType::FUNC_MIN_TYPE);

			ms2->Type									= Marker_SearchType::MAX;
			ms2->_searchRange_Enable					= true;
			ms2->_search_Start							= vnaDataAnalysisTPC.testFreqRangeStart[0];
			ms2->_search_Stop							= vnaDataAnalysisTPC.testFreqRangeStop[0];
			markerSearchSetting[1]						= ms2;
			msSetting[vnaSiteIndex].ms_SearchStart[1]	= markerSearchSetting[1]->_search_Start;
			msSetting[vnaSiteIndex].ms_SearchStop[1]	= markerSearchSetting[1]->_search_Stop;
			msSetting[vnaSiteIndex].msMinMax_Type[1]	= (int)(MarkerSearchType::FUNC_MAX_TYPE);

			for (int i = 0; i < vnaDataAnalysisTPC.testFreqRangeStart->Length; i++)
			{
				// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
				if ((vnaDataAnalysisTPC.testFreqRangeStart[i] == 0) || 
					(vnaDataAnalysisTPC.testFreqRangeStop[i] == 0)	|| 
					(vnaDataAnalysisTPC.testFreqRangeStart[i] >= vnaDataAnalysisTPC.testFreqRangeStop[i]))
				{
					markerSearchSetting[i]->_search_Start	= 0.0 MHz;
					markerSearchSetting[i]->_search_Stop	= 0.0 MHz;
					ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			
			// Check existing marker count | add or remove marker by referring to the user input
			msSetting[vnaSiteIndex].markerIndex = 0;
			for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
			{
				// Check the existing markers on the target trace
				int existingMarkerCount = 0;
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, existingMarkerCount));

				// If existing marker more than needed marker --> remove extra marker
				if (existingMarkerCount > markerSearchSetting->Length)
				{
					// Delete all marker 
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex));

					// Add marker
					int markerAdded = (int)markerSearchSetting->Length;
					for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}
				// If existing marker less than needed marker --> add extra marker
				else if (existingMarkerCount < markerSearchSetting->Length)
				{
					// Add marker
					int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
					for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}
				else
				{
					// Re-define the marker frequency
					for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}

				// Record current marker index
				msSetting[vnaSiteIndex].markerIndex = i + 1;
				
				// Execute based on the marker search min max type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, 
					msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msMinMax_Type[i]));
			}

			// Result variables
			msFormatData->XAxis			= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->realRaw		= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->imaginaryRaw	= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->markerType	= gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msFreq		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msReal		= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msImaginary	= gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msType		= gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
			{
				msFormatData->XAxis[i]						= (double)CONST_INVALID_RESULT;
				msFormatData->realRaw[i]					= (double)CONST_INVALID_RESULT;
				msFormatData->imaginaryRaw[i]				= (double)CONST_INVALID_RESULT;
				msFormatData->markerType[i]					= String::Empty;
				msFormatData->msFreq[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
				msFormatData->msReal[vnaSiteIndex, i]		= (double)CONST_INVALID_RESULT;
				msFormatData->msImaginary[vnaSiteIndex, i]	= (double)CONST_INVALID_RESULT;
				msFormatData->msType[vnaSiteIndex, i]		= String::Empty;
			}
			
			// Get marker search min max data
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
				msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));

			// Store marker search min max data accordingly
			if (msFormatData->XAxis->Length != 0)
			{
				for (int i = 0; i < msFormatData->XAxis->Length; i++)
				{
					msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
					msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
					msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
					msFormatData->msType[vnaSiteIndex, i]		= msFormatData->markerType[i];
				}
			}
			else
			{ 
				ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				goto EndOfTest;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetMarkerSearchMinMaxRipple_CMT] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMeanFromRange_CMT(int tfSite, int vnaSiteIndex)
	{
		int ret = 0;

		double searchStart = vnaDataAnalysisTPC.testFreqRangeStart[0];
		double searchStop = vnaDataAnalysisTPC.testFreqRangeStop[0];

		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Averaging & Smoothing Function
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureAveraging_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureAveraging_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure and enable averaging at the active channel.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Get vna channel with averaging
			vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;

			if (averagingSettings.averagingEnable == Cond_AveragingSetting_Enable_AveOn)
			{
				// Enable averaging function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Averaging_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve, true));
				
				// Configure averaging factor
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_AverFactor_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve, averagingSettings.averagingFactor));
			
				// To complete the averaging process based on the averaging factor | iterations
				for (int i = 0; i < (int)averagingSettings.averagingFactor; i++)
				{
					ret = ConfigureActiveChannelAndSingleTrigger_CMT(tfSite, vnaSiteIndex);
					if (ret != 0) goto EndOfTest;
				}

				//averageSetting[vnaSiteIndex].enableActiveTraceTransform	= true;
				//averageSetting[vnaSiteIndex].enableAveCorrection			= true;
			}
			else if (averagingSettings.averagingEnable == Cond_AveragingSetting_Enable_AveOff)
			{
				// Disable averaging function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Averaging_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve, false));

				// Reset
				//averageSetting[vnaSiteIndex].enableActiveTraceTransform	= false;
				//averageSetting[vnaSiteIndex].enableAveCorrection			= false;
				vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve			= Vna_Channel_Reset;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_AVERAGING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureAveraging_CMT] Fail to configure averaging setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureAveraging_CMT] Fail to configure averaging setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureSmoothing_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSmoothing_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure and enable smoothing at the active trace.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Get trace index with smoothing
			vnaDataAnalysisTPC.traceIndexSmo = vnaDataAnalysisTPC.traceIndex;

			if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOn)
			{
				// Enable smoothing function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Smoothing_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo, true));

				// configure smoothing aperture
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_SmoothingAperture_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo,
					smoothingSettings.smoothingAperture));

				averageSetting[vnaSiteIndex].enableActiveTraceTransform = true;
				averageSetting[vnaSiteIndex].enableSmoCorrection		= true;
			}
			else if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOff)
			{
				// Disable smoothing function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Smoothing_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo, false));

				// Reset
				averageSetting[vnaSiteIndex].enableActiveTraceTransform = false;
				averageSetting[vnaSiteIndex].enableSmoCorrection		= false;
				vnaDataAnalysisTPC.traceIndexSmo						= Vna_Format_Reset;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_SMOOTHING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Fail to configure smoothing setting at the active trace." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> ConfigureSmoothing_CMT] Fail to configure smoothing setting at the active trace." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Channel Source Low Function
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::SourceLowVna_CMT(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** SourceLowVna_CMT
		**		tfSite	- This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to source low each channel with Copper Mountain VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		String^ Power_Level;
		String^ command;

		try
		{
			for (int channel = 1; channel <= projectStateInfo.Channels->Length; channel++)
			{
				Power_Level = "-60";
				command = ("SOUR" + channel.ToString() + ":POW " + Power_Level + " \n");
				amb7300_NA[siteIndex]->System_Scpi_WriteString(command);
				tl->Util->WaitSecond(2 mS);
				command = ("SENS" + channel.ToString() + ":SWE:MODE CONT\n");
				amb7300_NA[siteIndex]->System_Scpi_WriteString(command);
				command = ("TRIG:SOUR MAN\n");
				amb7300_NA[siteIndex]->System_Scpi_WriteString(command);
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SOURCE_LOW_SETTING_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[PostProcessing -> PostProcessingTester -> CopperMountainVnaUtility -> SourceLowVna_CMT] Fail to source low Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[PostProcessing -> PostProcessingTester -> CopperMountainVnaUtility -> SourceLowVna_CMT] Fail to source low Copper Mountain VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Save To Touchstone File
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::SaveTraceDataToTouchstoneFile_CMT(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	SaveTraceDataToTouchstoneFile_CMT
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to save trace data to touchstone file.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		String ^ strPortNumber	= String::Empty;
		String ^ separator		= String::Empty;
		String ^ spliter		= String::Empty;
		//For Merging S3P
		int iMergeSnPFile = tl->glob->iMergeSnPFile;
		String^ touchstoneFilePath = String::Empty;
		/*String ^ loggerMessage = String::Empty;
		String ^ touchStoneFileFormat = String::Empty;
		String ^ touchStoneFormat = String::Empty;
		String ^ touchStoneDomainFormat = String::Empty;
		String ^ fileNameWExt = String::Empty;
		String ^ sparaFormat = String::Empty;*/
		//For Asynchronous S3P task list
		S3P_TaskList = gcnew array<Task< double>^>(10);

		//S3P array init
		if (iMergeSnPFile == 0)
		{
			tl->glob->mergerdTouchstoneFilePath = gcnew array<String ^>(3);

			//Manually merging S3P file //obsolete
			/*traceFormatData->totalFreqPoints = segmentSetting[vnaSiteIndex].totalPoint;
			traceFormatData->XAxis = gcnew array<double>(traceFormatData->totalFreqPoints);

			traceFormatData->S11Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

			traceFormatData->S13Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S13Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S31Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S31Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S33Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S33Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

			traceFormatData->S23Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S23Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S32Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S32Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

			// Get trace format data from the selective channel and trace
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
				traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));*/
		}

		try
		{ 
			
			for (int i = 0; i < saveRecallSetting->touchstoneFilePortNumber->Length; i++)
			{
				if (String::IsNullOrEmpty(strPortNumber) == false)
					separator = " ` ";
				else
					separator = String::Empty;

				strPortNumber = strPortNumber + separator + saveRecallSetting->touchstoneFilePortNumber[i];
			}

			// Touchstone file data spliter symbol (for s3p / s4p type)
			if (saveRecallSetting->touchstoneFileSeparator == int(SnPSeparator::TAB)) 
				spliter = "	";
			else if (saveRecallSetting->touchstoneFileSeparator == int(SnPSeparator::SPACE))
				spliter = " ";

			// [Future Enchancement] For enable active transform situation, to enable a option 1. all trace are trans. 2. only selective smo trace are trans. 
			if (averageSetting[vnaSiteIndex].enableActiveTraceTransform == true)
			{
				//if [all trace to be auto transformed and save, even only certain trace is applied with smo by the user]
				//{ 
				
				// Select touchstone file data format (Re_Imag | Mag_Angle | dB_Angle)
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFormat_Set(saveRecallSetting->touchstoneFileDataFormat));
				
				// Select touchstone file data seperator symbol
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSeparator_Set(saveRecallSetting->touchstoneFileSeparator));
			
				// Select touchstone file header port number
				if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S1P))
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S1P_Set(saveRecallSetting->touchstoneFilePortNumber[0]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S2P))
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S2P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S3P))
				{
					// [Future Enchancement] Reserved for S3P.
					// [Future Enchancement] Use Steven new API, direct output S3P/S4P file without manual merging.
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S3P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1], saveRecallSetting->touchstoneFilePortNumber[2]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S4P))
				{
					// [Future Enchancement] Reserved for S4P.
					// [Future Enchancement] Use Steven new API, direct output S3P/S4P file without manual merging.
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S4P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1], saveRecallSetting->touchstoneFilePortNumber[2], saveRecallSetting->touchstoneFilePortNumber[3]));
				}

				// Save touchstone file to target directory
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSaveSparamFile(saveRecallSetting->touchstoneFilePath));

				//}
				//else [only selective smo trace to be transformed]
				//{
				//	manual process the touchstone content
				//}
			}
			else
			{
				// Select touchstone file data format (Re_Imag | Mag_Angle | dB_Angle)
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFormat_Set(saveRecallSetting->touchstoneFileDataFormat));

				// Select touchstone file data seperator symbol
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSeparator_Set(saveRecallSetting->touchstoneFileSeparator));
			
				// Select touchstone file header port number
				if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S1P))
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S1P_Set(saveRecallSetting->touchstoneFilePortNumber[0]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S2P))
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S2P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S3P))
				{
					// [Future Enchancement] Reserved for s3p.
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S3P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1], saveRecallSetting->touchstoneFilePortNumber[2]));
					saveRecallSetting->touchstoneFilePath = saveRecallSetting->touchstoneFilePath + "_" + iMergeSnPFile;
					touchstoneFilePath = saveRecallSetting->touchstoneFilePath + ".s2p";
					tl->glob->mergerdTouchstoneFilePath[iMergeSnPFile] = touchstoneFilePath;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S2P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1]));
				}
				else if (vnaSetting[vnaSiteIndex].measureSnpType == (int)(Measure_SNP_Type::Measure_S4P))
				{
					// [Future Enchancement] Reserved for s4p.
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFile_S4P_Set(saveRecallSetting->touchstoneFilePortNumber[0], saveRecallSetting->touchstoneFilePortNumber[1], saveRecallSetting->touchstoneFilePortNumber[2], saveRecallSetting->touchstoneFilePortNumber[3]));
				}

				// Save touchstone file to target directory
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSaveSparamFile(saveRecallSetting->touchstoneFilePath));

#pragma region "Asynchronously Merging S3P"

				// S3P Asynchronous
				if (iMergeSnPFile == 2 && vnaSetting[vnaSiteIndex].measureSnpType == 3)
				{
					S3P_TaskList[0] = amb7300_[vnaSiteIndex]->amb7300->SaveDataToTouchstoneFileProcessAsync(3, saveRecallSetting->touchstoneFilePath, 10);
				}

				if (vnaSetting[vnaSiteIndex].measureSnpType == 3)
				{
					tl->glob->iMergeSnPFile++;
					if (tl->glob->iMergeSnPFile == 3 && vnaSetting[vnaSiteIndex].measureSnpType == 3)
					{
						//reset
						tl->glob->iMergeSnPFile = 0;
					}
				}

#pragma endregion

				String^ OPC;

				// Wait until sweep is finished
				while (OPC)
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->OPCStatus_Get(OPC));
					if ((OPC->Contains("1")) || (ret == ER_CONST_TERMINATION_CHARACTER_VISA_ERROR)) // [Info] By referring to wrapper, it is using &&, currently change to || and need to keep monitor on it whenever there is project with online debug.
					{
						ret = 0; //reset ret to 0 to solve 0x3FFF0005 termination character VISA error
						break;
					}
				}
#pragma region "Merge S2P into S3P //Obsolete"
				//obsolete

				/*
				//if (iMergeSnPFile == 2 && vnaSetting[vnaSiteIndex].measureSnpType == 3)//merging multiple S2P into S3p
				//{
				//	for (int i = 0; i <= 2; i++)
				//	{
				//		int iWatcher = 0;
				//		while (!File::Exists(touchstoneFilePath) && iWatcher < 30)
				//		{
				//			iWatcher++;
				//			tl->Util->WaitSecond(10 mS);
				//		}
				//		tl->Util->WaitSecond(100 mS);

				//		touchstoneFilePath = tl->glob->mergerdTouchstoneFilePath[i];

				//		StreamReader ^ snpSR = gcnew StreamReader(touchstoneFilePath);
				//		String ^ ReadLine = String::Empty;
				//		array<String ^> ^ ArrStr = gcnew array<String ^>(0);
				//		array<String ^> ^ Separator = gcnew array<String ^>(1);
				//		Separator[0] = spliter;
				//		int sparaDataCount = 0;

				//		vnaSetting[vnaSiteIndex].vna_chn->Length;

				//		while ((ReadLine = snpSR->ReadLine()) != nullptr)
				//		{
				//			ArrStr = ReadLine->Split(Separator, StringSplitOptions::None);

				//			if (ArrStr[0]->StartsWith("!") == false && ArrStr[0]->StartsWith("#") == false)	//Ignore Header Information 
				//			{
				//				/* S3P involved three *s2p files merging */
				//				if (i == 0)
				//				{
				//					/*S1X Real Img*/
				//					traceFormatData->S11Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[1]); traceFormatData->S11Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[2]);
				//					traceFormatData->S12Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[5]);	traceFormatData->S12Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[6]);
				//					/*S2X Real Img*/
				//					traceFormatData->S21Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[3]); traceFormatData->S21Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[4]);
				//					traceFormatData->S22Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[7]); traceFormatData->S22Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[8]);
				//				}
				//				else if (i == 1)
				//				{
				//					/*S1X Real Img*/
				//					traceFormatData->S13Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[5]);	traceFormatData->S13Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[6]);
				//					/*S3X Real Img*/
				//					traceFormatData->S31Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[3]); traceFormatData->S31Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[4]);
				//					traceFormatData->S33Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[7]); traceFormatData->S33Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[8]);
				//				}
				//				else if (i == 2)
				//				{
				//					/*S2X Real Img*/
				//					traceFormatData->S23Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[5]); traceFormatData->S23Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[6]);
				//					/*S3X Real Img*/
				//					traceFormatData->S32Real[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[3]); traceFormatData->S32Imaginary[vnaSiteIndex, sparaDataCount] = double::Parse(ArrStr[4]);
				//				}
				//				sparaDataCount++;
				//			}
				//		}
				//		snpSR->Close();
				//		snpSR = nullptr;
				//	}

				//	FileLogger ^ snpFileLogger = gcnew FileLogger(Path::GetDirectoryName(saveRecallSetting->touchstoneFilePath));
				//	fileNameWExt = Path::GetFileName(saveRecallSetting->touchstoneFilePath) + ".s3p";

				//	if ((saveRecallSetting->touchstoneFileDataFormat) == int(SnPFormat::SNP_RI_FORMAT))
				//	{
				//		sparaFormat = "RI";
				//		if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOn)
				//		{
				//			touchStoneFormat = "Re/Im(F2 Smo)";
				//		}
				//		else
				//		{
				//			touchStoneFormat = "Re/Im(F2)";
				//		}
				//	}
				//	else if ((saveRecallSetting->touchstoneFileDataFormat) == int(SnPFormat::SNP_MA_FORMAT))
				//	{
				//		sparaFormat = "MA";
				//		if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOn)
				//		{
				//			touchStoneFormat = "Mag/Ang(F2 Smo)";
				//		}
				//		else
				//		{
				//			touchStoneFormat = "Mag/Ang(F2)";
				//		}
				//	}
				//	else if ((saveRecallSetting->touchstoneFileDataFormat) == int(SnPFormat::SNP_DB_FORMAT))
				//	{
				//		sparaFormat = "DB";
				//		if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOn)
				//		{
				//			touchStoneFormat = "dB/Ang(F2 Smo)";
				//		}
				//		else
				//		{
				//			touchStoneFormat = "dB/Ang(F2)";
				//		}
				//	}

				//	if (saveRecallSetting->touchstoneFileDomainFormat == int(TouchstoneFileFormat::Frequency_Domain))
				//	{
				//		touchStoneFileFormat = "# Hz S ";
				//		touchStoneDomainFormat = "Frequency ";
				//	}
				//	else
				//	{
				//		touchStoneFileFormat = "# s S ";
				//		touchStoneDomainFormat = "Time ";
				//	}


				//	/*! is a commented line.*/
				//	loggerMessage += touchStoneFileFormat + sparaFormat + " R 50\n";
				//	loggerMessage += "! CMT, " + saveRecallSetting->Model + "," + saveRecallSetting->SerialNumber + "," + saveRecallSetting->FWVersion;
				//	loggerMessage += "! Date : " + DateTime::Now.Year + "/" + DateTime::Now.Month + "/" + DateTime::Now.Day + " " + DateTime::Now.ToString("h:mm:ss tt") + "\n";
				//	loggerMessage += "! State File : " + vnaSetting[vnaSiteIndex].StateFile + ".sta";
				//	snpFileLogger->WriteToFile(fileNameWExt, loggerMessage, LOGGER_CONST_OVERWRITE);
				//	loggerMessage = String::Empty;

				//	loggerMessage += "! S3P Measurements: S11, S12, S13, S21, S22, S23, S31, S32, S33: \n";
				//	loggerMessage += "! Freq	S11:" + touchStoneFormat + " S12:" + touchStoneFormat + " S13:" + touchStoneFormat +
				//		" S21:" + touchStoneFormat + " S22:" + touchStoneFormat + " S23:" + touchStoneFormat +
				//		" S31:" + touchStoneFormat + " S32:" + touchStoneFormat + " S33:" + touchStoneFormat + "\n";

				//	for (int segPoint = 0; segPoint < traceFormatData->XAxis->Length; segPoint++) //Segment Points 
				//	{
				//		loggerMessage += traceFormatData->XAxis[segPoint].ToString("0.0000000000E+00") + spliter;
				//		/*S1X*/
				//		loggerMessage += traceFormatData->S11Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S11Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S12Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S12Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S13Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S13Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		/*S2X*/
				//		loggerMessage += traceFormatData->S21Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S21Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S22Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S22Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S23Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S23Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		/*S3X*/
				//		loggerMessage += traceFormatData->S31Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S31Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S32Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S32Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;
				//		loggerMessage += traceFormatData->S33Real[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter + traceFormatData->S33Imaginary[vnaSiteIndex, segPoint].ToString("+0.##########0E+00;-0.##########0E+00") + spliter;

				//		snpFileLogger->WriteToFile(fileNameWExt, loggerMessage, LOGGER_CONST_APPEND);
				//		loggerMessage = String::Empty;
				//	}

				//}

				//if (vnaSetting[vnaSiteIndex].measureSnpType == 3)
				//{
				//	tl->glob->iMergeSnPFile++;
				//}
				//if (tl->glob->iMergeSnPFile == 3 && vnaSetting[vnaSiteIndex].measureSnpType == 3)
				//{
				//	//reset
				//	tl->glob->iMergeSnPFile = 0;
				//	if (1)
				//	{
				//		for (int i = 0; i <= 2; i++)
				//		{
				//			File::Delete(tl->glob->mergerdTouchstoneFilePath[i]);
				//		}
				//	}
				//}
#pragma endregion

			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SAVE_TRACE_DATA_TO_TOUCHSTONE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> SaveTraceDataToTouchstoneFile_CMT] Fail to save trace data to touchstone file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> SaveTraceDataToTouchstoneFile_CMT] Fail to save trace data to touchstone file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	
	int AMB7300TestLibrary::GetTraceFormatData_CMT_Sds(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetTraceFormatData_CMT_Sds
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to read formated data from the target trace, and store in TraceFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// [Future Enchancement] To eval and handle Marker_ReferenceMarker_Set()
		try
		{
			traceFormatData->totalFreqPoints = segmentSetting[vnaSiteIndex].totalPoint;
			traceFormatData->XAxis = gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->realRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->imaginaryRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->freqPoint = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

			if (vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				// Get trace format data from the selective channel and trace   
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndex,
					traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //

																										// [CHECK] if total freq points from user does not match with the actual trace data points
				if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
				{
					ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}

				// Store trace data accordingly
				for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
				{
					if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
					{
						if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
						{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
						}
						else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
						{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
						}
						else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
						{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
						}
						else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
						{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
						}
					}
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_TraceData_Sds)
			{
				if (ChannelUseTPC.ChannelUseFristChannel == ChannelUseTPC.ChannelUseSecondChannel)
				{
					int l_useVnaFetchChannel = 1;
					if (ChannelUseTPC.ChannelUseFristChannel == "Ch1" || ChannelUseTPC.ChannelUseFristChannel == "ch1")
					{
						l_useVnaFetchChannel = 1;
					}
					else if (ChannelUseTPC.ChannelUseFristChannel == "Ch2" || ChannelUseTPC.ChannelUseFristChannel == "ch2")
					{
						l_useVnaFetchChannel = 2;
					}
					else if (ChannelUseTPC.ChannelUseFristChannel == "Ch3" || ChannelUseTPC.ChannelUseFristChannel == "ch3")
					{
						l_useVnaFetchChannel = 3;
					}


					// <<<<<<<<<index 1>>>>>>>>Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel, 1,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel,vnaDataAnalysisTPC.traceIndex

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							//	if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							/*else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}*/
						}
					}

					// <<<<<<<<<index 2>>>>>>>>Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel, 2,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel,vnaDataAnalysisTPC.traceIndex

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							/*if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)*/
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							/*else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}*/
						}
					}


					// <<<<<<<<<index 3>>>>>>>>Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel, 3,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel,vnaDataAnalysisTPC.traceIndex
																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							/*if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)*/
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							/*else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}*/
						}
					}


					// <<<<<<<<<index 4>>>>>>>>Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel, 4,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel,vnaDataAnalysisTPC.traceIndex

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							/*if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
							traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
							traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
							traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)*/
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}



				}
				else if ((ChannelUseTPC.ChannelUseFristChannel != ChannelUseTPC.ChannelUseSecondChannel) && (ChannelUseTPC.ChannelUseSecondChannel != "Ch4"))
				{
					traceFormatData->XAxis2 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->realRaw2 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->imaginaryRaw2 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->freqPoint2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Real2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Imaginary2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Real2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Imaginary2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Real2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Imaginary2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Real2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Imaginary2 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

					int l_useVnaFetchChannel_1 = 1;
					int l_useVnaFetchChannel_2 = 2;
					//the frist ch
					if (ChannelUseTPC.ChannelUseFristChannel == "Ch1" || ChannelUseTPC.ChannelUseFristChannel == "ch1")
					{
						l_useVnaFetchChannel_1 = 1;
					}
					else if (ChannelUseTPC.ChannelUseFristChannel == "Ch2" || ChannelUseTPC.ChannelUseFristChannel == "ch2")
					{
						l_useVnaFetchChannel_1 = 2;
					}
					else if (ChannelUseTPC.ChannelUseFristChannel == "Ch3" || ChannelUseTPC.ChannelUseFristChannel == "ch3")
					{
						l_useVnaFetchChannel_1 = 3;
					}
					//the second ch
					if (ChannelUseTPC.ChannelUseSecondChannel == "Ch1" || ChannelUseTPC.ChannelUseSecondChannel == "ch1")
					{
						l_useVnaFetchChannel_2 = 1;
					}
					else if (ChannelUseTPC.ChannelUseSecondChannel == "Ch2" || ChannelUseTPC.ChannelUseSecondChannel == "ch2")
					{
						l_useVnaFetchChannel_2 = 2;
					}
					else if (ChannelUseTPC.ChannelUseSecondChannel == "Ch3" || ChannelUseTPC.ChannelUseSecondChannel == "ch3")
					{
						l_useVnaFetchChannel_2 = 3;
					}

					//  The Frist CH  >>>>>>> Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel_1, vnaDataAnalysisTPC.traceIndex,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					//Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}

					//  The Second CH  >>>>>>> Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel_2, vnaDataAnalysisTPC.traceIndex,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					//Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint2[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Real2[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary2[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
								traceFormatData->freqPoint2[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Real2[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary2[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
								traceFormatData->freqPoint2[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Real2[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary2[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
								traceFormatData->freqPoint2[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Real2[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary2[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}
				}
				//which ch1/ch4
				else if ((ChannelUseTPC.ChannelUseFristChannel != ChannelUseTPC.ChannelUseSecondChannel) && (ChannelUseTPC.ChannelUseSecondChannel == "Ch4"))

				{
					traceFormatData->XAxis3 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->realRaw3 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->imaginaryRaw3 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->freqPoint3 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Rea13 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Imaginary3 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Rea13 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Imaginary3 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Rea13 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Imaginary3 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Rea13 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Imaginary3 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

					traceFormatData->XAxis4 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->realRaw4 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->imaginaryRaw4 = gcnew array<double>(traceFormatData->totalFreqPoints);
					traceFormatData->freqPoint4 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Rea23 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S11Imaginary4 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Rea23 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S21Imaginary4 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Rea23 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S12Imaginary4 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Rea23 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
					traceFormatData->S22Imaginary4 = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);

					int l_useVnaFetchChannel_1 = 1;
					int l_useVnaFetchChannel_2 = 2;
					int l_useVnaFetchChannel_3 = 3;

					//  The Frist CH  >>>>>>> Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel_1, vnaDataAnalysisTPC.traceIndex,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					//Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
								traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}

					//  The Second CH  >>>>>>> Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel_2, vnaDataAnalysisTPC.traceIndex,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));  //vnaSetting[vnaSiteIndex].currentVnaFetchChannel

																											// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					//Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint3[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Rea13[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary3[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
								traceFormatData->freqPoint3[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Rea13[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary3[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
								traceFormatData->freqPoint3[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Rea13[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary3[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
								traceFormatData->freqPoint3[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Rea13[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary3[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}
					//  The Third CH  >>>>>>> Get trace format data from the selective channel and trace   
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(l_useVnaFetchChannel_3, vnaDataAnalysisTPC.traceIndex,
						traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));

					// [CHECK] if total freq points from user does not match with the actual trace data points
					if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
					{
						ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					//Store trace data accordingly
					for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
					{
						if (vnaSetting[vnaSiteIndex].vna_traceFormat[vnaDataAnalysisTPC.traceIndex - 1] != Vna_Format_Reset)
						{
							if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S11)
							{
								traceFormatData->freqPoint4[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S11Rea23[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S11Imaginary4[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S21)
							{
								traceFormatData->freqPoint4[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S21Rea23[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S21Imaginary4[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S12)
							{
								traceFormatData->freqPoint4[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S12Rea23[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S12Imaginary4[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
							else if (vnaSetting[vnaSiteIndex].measType[vnaDataAnalysisTPC.traceIndex - 1] == Vna_Measurement_Type_S22)
							{
								traceFormatData->freqPoint4[vnaSiteIndex, i] = traceFormatData->XAxis[i];
								traceFormatData->S22Rea23[vnaSiteIndex, i] = traceFormatData->realRaw[i];
								traceFormatData->S22Imaginary4[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
							}
						}
					}
				}
			}


		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[CopperMountainVnaUtility -> GetTraceFormatData_CMT] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

}


/*----------------------------------------------------------------------
* Revision Log
* &Log: CopperMountainVnaUtility.cpp.rca&

*** Version	: v1.0.0.3
*** Date	: 2 September 2026
*** PIC		: Xuli
* Added support for SharedMemoryDataBWSearch

*** Version	: v1.0.0.2
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Separated from SourceLowVna_CMT

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support all CMT VNA configuration and testing functions.
* Support project UUT:
  - Single Site Single UUT
  - Single Site Multi UUT
  - True Parallel Single UUT
* Support VNA:
  - CMT SC5090
* Support platform execution phase:
  - VnaConfig
  - VnaFetch
  - VnaDataAnalysis
----------------------------------------------------------------------*/
