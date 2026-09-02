/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				KeysightVnaUtility.cpp
Purpose:			Contain Keysight VNA related utility functions
Supported Module:	Keysight VNA M9804A
Version:			v1.0.0.5
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Initialize & Uninitialize Vna Hardware
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeVna_Keysight(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, int vnaSiteIndexAlias, bool offline)
	{
		/*****************************************************************************************************
		** InitializeVna_Keysight
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		offline				- techFlow run in online or offline mode.
		**
		** Descriptions:
		**		This is a function is to initialize Keysight VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		String ^ systemAlias = String::Empty;
		// To prevent multiple same resources are loaded
		Dictionary<String^, int> ^ ResourceAlias = gcnew Dictionary<String ^, int>();

		//Util->InitTimer(1);
		//timerFilename = "TimingFile_7300_KeysightVNAUtil_" + System::DateTime::Now.ToString("yyyyMMddHHmmss");
		try
		{
#pragma region"Initialize Keysight Vna"

			/*
			**	S2P1D -}
			**	S4P1D -}-->Direct Keysight Ports
			**  S6P1D -}
			**	S6P22N M908X VNA + A5
			**	S4P1 M908X VNA + A4
			**	S4P2 P9371B VNA + A3 (WSH7600)
			*/

			for each (String ^ Rsrc in tl->glob->RsrcManager.RsrcMngr[siteIndex]->MapNames)
			{
				Resource ^ HardwareRsrc = tl->glob->RsrcManager.RsrcMngr[siteIndex]->ResolveResource(Rsrc)[0];

				if ((HardwareRsrc->Hierarchy->Length == 1) &&
					((HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S2P1D") == true) ||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S4P1D") == true) ||
						(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S6P1D") == true)) ||
						(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S6P22N") == true) ||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S4P1") == true) ||
					(HardwareRsrc->Hierarchy[0]->StartsWith("AMB7300S4P2") == true) &&
					(ResourceAlias->ContainsKey(HardwareRsrc->Alias->ToString()) == false))
				{
					// S2P1D
					if (Rsrc->StartsWith(AMB7300_S2P1D_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S2P1D_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S2P1D_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::S2P1D);
					}
					// S4P1D
					else if (Rsrc->StartsWith(AMB7300_S4P1D_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S4P1D_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S4P1D_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::S4P1D);
					}
					// S6P1D
					else if (Rsrc->StartsWith(AMB7300_S6P1D_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S6P1D_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S6P1D_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::S6P1D);
					}
					// S6P22N
					else if (Rsrc->StartsWith(AMB7300_S6P22N_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S6P22N_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S6P22N_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::S6P22N);
					}
					// A4 [S4P1]
					else if (Rsrc->StartsWith(AMB7300_S4P1_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S4P1_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S4P1_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::A4);
					}
					// A3 [S4P2]
					else if (Rsrc->StartsWith(AMB7300_S4P2_TYPE) == true)
					{
						if (tl->glob->tf.ProjectType != int(ProjectType::IndexParallel))
						{
							systemAlias = AMB7300_S4P2_TYPE + "_" + vnaSiteIndexAlias;
						}

						amb7300SystemSetting->systemAlias = AMB7300_S4P2_TYPE;
						amb7300SystemSetting->systemConfig = int(SystemConfig::A3);
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
							tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
						}
						else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
						{
							tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
						}
						else if (tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
						{
							tl->CheckError(tfSite, amb7300_[siteIndex]->Load(hardwareProfile, testHead, siteIndex, systemAlias, offline, 0, vnaSetting[0].StateFile));
						}
						else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
						{
							// [Future Enchancement] reserved for index parallel project.
						}
					}
					AMB7300_VnaHardwareStatus[siteIndex] = true;

					amb7300_driver[siteIndex] = (AMB7300_Driver ^)amb7300_[siteIndex]->amb7300;
					amb7300_NA[siteIndex] = (NetworkAnalyzer ^)amb7300_[siteIndex]->amb7300->NetworkAnalyzer[0];
				}
			}

#pragma endregion
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_KEYSIGHT_VNA_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Fail to initialize Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> KeysightVnaUtility -> InitializeVna_Keysight] Fail to initialize Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeVna_Keysight(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** UninitializeVna_Keysight
		**		tfSite			- This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to uninitialize Keysight VNA hardware.
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
			ret = ER_CONST_UNINIT_KEYSIGHT_VNA_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> KeysightVnaUtility -> UninitializeVna_Keysight] Fail to uninitialize Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> KeysightVnaUtility -> UninitializeVna_Keysight] Fail to uninitialize Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Configure Channel Coupling
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureChannelCoupling_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureChannelCoupling_Keysight
		**	testSite		- This is techFlow site index.
		**	vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure the coupled channel for parallel measurement.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int option = 0;

		try
		{
			// Channel Couple Parallel Setup - Support for AMB7300S4P1D and AMB7300S6P1D only
			if (tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
			{
				if ((amb7300SystemSetting->systemAlias == AMB7300_S6P1D_TYPE))
				{
					array< int> ^ CoupledChannel = { 0,1,2,3,4,5 };
					tl->CheckError(tfSite, amb7300_driver[tfSite]->System_Channel_CoupleParallel_Setup(CoupledChannel, option, true, true));//>> For S4P1D
																																			//>> For S6P1D
																																			//>>> option can be 2 or 3
																																			//>>> if option == 2
																																			//Ch1: Port 1 & 2, Ch2 : Port 3 & 4, Ch3 : Port 5 & 6
																																			//>>> if option == 3
																																			//Ch1: Port 1 & 2 & 3, Ch2 : Port 4 & 5 & 6
				}
				else if ((amb7300SystemSetting->systemAlias == AMB7300_S4P1D_TYPE))
				{
					array< int> ^ CoupledChannel = { 0,1,2,3 };
					tl->CheckError(tfSite, amb7300_driver[tfSite]->System_Channel_CoupleParallel_Setup(CoupledChannel, option, true, true));
				}
			}
		}
		catch (Exception^ ex)
		{
			//ret = ER_CONST_CONFIGURE_MPEX_PORT_PAIR_SWITCHING_FAIL;
			//tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Fail to configure MPEX port pair switching." + "\n" +
			//	"Please check the port pair (AMAP) if existed in the Mapping File." + "\n" +
			//	" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			//tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Fail to configure MPEX port pair switching." + "\n" +
			//	"Please check the port pair (AMAP) if existed in the Mapping File." + "\n" +
			//	" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			return ret;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : MPEX Port Pair Switching
	**	----------------------------------------------------------------------------------------------------
	*/
	// Keysight VNA +A5 => S6P22N
	int AMB7300TestLibrary::ConfigureMpexPortPairSwitching_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureMpexPortPairSwitching_Keysight
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
			// Configure the mpex swiches path by referring to the port pair | the port pair must exist in the mapping file  (Support S6P22N[VNA+A5], S4P1 [VNA+A4], S4P2 [VNA+A3, WSH7600])
			// OUT1/OUt2 -> P1A/P2B must exists in the mapping file
			if ((amb7300SystemSetting->systemConfig == int(SystemConfig::S6P22N)) ||
				(amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) ||
				(amb7300SystemSetting->systemConfig == int(SystemConfig::A3)))
			{
				tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->ConfigureSwitchPath(vnaSetting[vnaSiteIndex].pinAlias));
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_MPEX_PORT_PAIR_SWITCHING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Fail to configure MPEX port pair switching." + "\n" +
				"Please check the port pair (AMAP) if existed in the Mapping File." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureMpexPortPairSwitching_Keysight] Fail to configure MPEX port pair switching." + "\n" +
				"Please check the port pair (AMAP) if existed in the Mapping File." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
	int AMB7300TestLibrary::SetPortMatchingCircuit_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	SetPortMatchingCircuit_Keysight
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

			for (int i = 0; i < projectStateInfo.Channels->Length ; i++) 
			{
				for (int port = 0; port < VNA_port_count; port++)
				{
					if (tl->glob->AWV.PortMatching_EN[port] == true)
					{
						tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->SetEmbedding(Int32::Parse(projectStateInfo.Channels[i]), port+1, true, PortMatching_filepath[port]));
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SET_PORT_MATCHING_CIRCUIT_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SetPortMatchingCircuit_Keysight] Fail to set Port Matching circuit." + "\n" +
				"Please check the provided App-Wide Variable." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SetPortMatchingCircuit_Keysight] Fail to set Port Matching circuit." + "\n" +
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
	int AMB7300TestLibrary::ConfigureSegmentSetting_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSegmentSetting_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure the segment settings at the active channel (VNA stimulus).
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		array<int>^ vnaChannel = gcnew array<int>(1);
		vnaChannel[0] = 999;

		String^ command;
		String^ Val = String::Empty;

		try
		{

#pragma region "HighPwrTest Control"
			if (tl->glob->AWV.HighPwrTest_EN == true)
			{
				String^ key = "Ch" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() + "_P" + segmentSetting[vnaSiteIndex].power[0].ToString() + "_Site" + vnaSiteIndex.ToString();

				double source_level = tl->glob->AppsCalFile.AppsCalFactor[key];

				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_SourcePower_SetLevel(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, source_level));							// Set to Stimulus > Power > Main > Power Level
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_MeasureDelaySet(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, segmentSetting[vnaSiteIndex].delay[0]));		// Set to Stimulus > Sweep > Sweep Timing > Sweep Delay
				for (int i =0; i< segmentSetting[vnaSiteIndex].segmentCount;i++)
				{
					segmentSetting[vnaSiteIndex].power[i] = source_level;			// Cast for configure segment table 'power'
					segmentSetting[vnaSiteIndex].delay[i] = 0;						// Set all segment delay value back to zero so that will not add into segment table 'delay'
				}
			}
#pragma endregion

			if ((segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) ||
				(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment) &&
				(segmentSetting[vnaSiteIndex].SettingHasChanged == true))
			{
				for (int iChannel = 0; iChannel < vnaSetting[vnaSiteIndex].vna_chn->Length; iChannel++)
				{
					if (vnaSetting[vnaSiteIndex].vna_chn[iChannel] != Vna_Channel_Reset)
					{
						// Local variable channelarr for wrapper used
						//vnaChannel[0] = vnaSetting[vnaSiteIndex].vna_chn[iChannel];

						if (!vnaSetting[vnaSiteIndex].ChannelNumCondition_Exist)
						{
							vnaChannel[0] = iChannel + 1;											// If ChannelNum condition not exist, loop every specified Channel from xml
						}
						else
						{
							vnaChannel[0] = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;		// If ChannelNum condition exist, use the condition value
						}

#pragma region "Configure segment settings"
						
						if (//(segmentSetting[vnaSiteIndex].GetFromStateFile == false) && 
							(segmentSetting[vnaSiteIndex].segmentCount > 1) &&
							(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_Segment))
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
								segmentSetting[vnaSiteIndex].startFreq));
						}
						else if (//(segmentSetting[vnaSiteIndex].GetFromStateFile == false) && 
							(segmentSetting[vnaSiteIndex].segmentCount == 1) &&
							(segmentSetting[vnaSiteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq))
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
							ret = GetChannelandTraceSetting_Keysight(tfSite, vnaSiteIndex);
						}

					}
				}
			}
			else if ((segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer) &&
				(segmentSetting[vnaSiteIndex].SettingHasChanged == true))
			{

				for (int iChannel = 0; iChannel < vnaSetting[vnaSiteIndex].vna_chn->Length; iChannel++)
				{
					if (vnaSetting[vnaSiteIndex].vna_chn[iChannel] != Vna_Channel_Reset)
					{
						// Local variable channelarr for wrapper used
						//vnaChannel[0] = vnaSetting[vnaSiteIndex].vna_chn[iChannel];

						if (!vnaSetting[vnaSiteIndex].ChannelNumCondition_Exist)
						{
							vnaChannel[0] = iChannel + 1;											// If ChannelNum condition not exist, loop every specified Channel from xml
						}
						else
						{
							vnaChannel[0] = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;		// If ChannelNum condition exist, use the condition value
						}

#pragma region "Configure segment settings"
						
						if ((segmentSetting[vnaSiteIndex].segmentCount > 1) &&
							(segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer))
						{
							command = "SENSe" + vnaChannel + ":SEGMent:BWIDth:CONTrol ON\n";
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							command = "SENSe" + vnaChannel + ":SEGMent:SWEep:TIME:CONTrol ON\n";
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							command = "SENSe" + vnaChannel + ":SEGMent:POWer:CONTrol ON\n";
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

							command = "SENSe1:SEGMent:LIST SSTOP," + segmentSetting[vnaSiteIndex].segmentCount + ",";
							for (int i = 0; i < segmentSetting[vnaSiteIndex].segmentCount; i++)
							{
								
								command += "1," + segmentSetting[vnaSiteIndex].point[i].ToString() + "," + //Start with 1, to On the segment
									segmentSetting[vnaSiteIndex].startFreq[i].ToString() + "," +
									segmentSetting[vnaSiteIndex].stopFreq[i].ToString() + "," +
									segmentSetting[vnaSiteIndex].ifbw[i].ToString() + "," +
									segmentSetting[vnaSiteIndex].delay[i].ToString() + "," +
									segmentSetting[vnaSiteIndex].power[i].ToString();
								if (i != segmentSetting[vnaSiteIndex].segmentCount - 1)
								{
									command += ",";
								}
							}

							command += "\n";
							amb7300_NA[tfSite]->System_Scpi_WriteString(command);

							command = "SENSe" + vnaChannel + ":SWEep:TYPE SEGment\n";
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

							do
							{

								command = String::Format("*OPC?\n");
									
								amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
								amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
								//Util->WaitSecond(1 mS);
								if (Val->Contains("1"))
									break;
							} while (true);
						}
						else if ((segmentSetting[vnaSiteIndex].segmentCount == 1) &&
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
							ret = GetChannelandTraceSetting_Keysight(tfSite, vnaSiteIndex);
						}

					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_SEGMENT_SETTING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSegmentSetting_Keysight] Fail to configure segment setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSegmentSetting_Keysight] Fail to configure segment setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetChannelandTraceSetting_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetChannelandTraceSetting_Keysight
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

		// Local variable
		int ret = 0;

		String^ command;
		String^ Val = String::Empty;
		array<String^>^ Freqs;
		array<String^>^ StateFile_Channels;
		array<String^>^ FirstTrace_Temp;
		//array<String^>^ FirstTrace;
		double totalTrace = 0;
		double temp;
		int OPC_count = 0;

		try
		{
			do
			{
				command = "*OPC? \n";
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);
				OPC_count++;
				if (Val->Contains("1"))
				{
					break;
				}
				
				if (OPC_count > 7)
				{
					break;
				}

			} while (true);
			
			
			// Get total number of channel
			command = "SYST:CHAN:CAT?\n";
			amb7300_NA[tfSite]->System_Scpi_WriteString(command);
			amb7300_NA[tfSite]->System_Scpi_ReadString(Val);		// "1,2"

			Val = Val->Replace('"', ' ')->Trim();					// 1,2
			projectStateInfo.Channels = Val->Split(',');			// {1,2}
			for (int chan_count = 0; chan_count < projectStateInfo.Channels->Length; chan_count++)
			{
				// Get sparam & trace number from each channel/windows
				command = String::Format("CALC{0}:PAR:CAT? \n", projectStateInfo.Channels[chan_count]);
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);		// "1,2"

				Val = Val->Replace('"', ' ')->Trim();					// 1,2
				projectStateInfo.Channel_Sparam_TraceNo_Info = Val->Split(',');			// {1,2}
				for (int trace_count = 0; trace_count < projectStateInfo.Channel_Sparam_TraceNo_Info->Length; trace_count++)
				{
					List<String^>^ Trace_Format_List;

				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_TOTAL_ACTIVE_CHANNEL_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetChannelandTraceSetting_Keysight] Fail to get total active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetChannelandTraceSetting_Keysight] Fail to get total active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
				int window = i + 1;
				command = String::Format("DISP:WIND{0}:TRAC1:SEL \n", window);		// Select 1st trace in a window
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				command = String::Format("CALC{0}:PAR:COUN? \n", window);			//total trace in a window
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);	// +4

				totalTrace = Double::Parse(Val) + totalTrace;		//sum up all traces in a sta file

				command = String::Format("CALC{0}:PAR:CAT?\n", window);
				amb7300_NA[tfSite]->System_Scpi_WriteString(command);
				amb7300_NA[tfSite]->System_Scpi_ReadString(Val);	// "CH1_S11_1,S22,CH1_S11_2,S21,CH1_S11_3,S12,CH1_S11_4,S22"

				FirstTrace_Temp = Val->Split(',');					// {"CH1_S11_1,S22,CH1_S11_2,S21,CH1_S11_3,S12,CH1_S11_4,S22"}
				FirstTrace_Temp = FirstTrace_Temp[0]->Split('_');	// {"CH1,S11,1}

				// Get First Trace number from each Channel
				if (FirstTrace_Temp->Length == 3)
				{
					SharedMemFormatData->FirstTrace[i] = FirstTrace_Temp[2]; //pass in trace number into string array
				}

				command = String::Format("CALC{0}:MEAS{1}:X:VAL? \n", window, totalTrace); //all freq in a window
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
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetChannelandTraceSetting_Keysight] Fail to get frequency point from channel." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetChannelandTraceSetting_Keysight] Fail to get frequency point from channel." + "\n" +
				" | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			return ret;
		}

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Power Sweep Settings [Sweep->Sweep Type->Power Sweep]
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigurePowerSweepSetting_Keysight(int tfSite, int vnaSiteIndex) //Under development, not support yet
	{
		/*****************************************************************************************************
		**	ConfigurePowerSweepSetting_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure the power sweep settings at the active channel (VNA stimulus).
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		array<int>^ vnaChannel = gcnew array<int>(1);
		vnaChannel[0] = 999;

		try
		{
			
#pragma region "Configure segment settings"
			for (int iChannel = 0; iChannel < vnaSetting[vnaSiteIndex].vna_chn->Length; iChannel++)
			{
				if (vnaSetting[vnaSiteIndex].vna_chn[iChannel] != Vna_Channel_Reset)
				{
					// Local variable channelarr for wrapper used
					vnaChannel[0] = vnaSetting[vnaSiteIndex].vna_chn[iChannel];
					//vnaChannel[0] = vnaSetting[vnaSiteIndex].currentVnaFetchChannel;


					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ConfigurePower_PowerSweep(1,
						segmentSetting[vnaSiteIndex].startFreq[0],
						segmentSetting[vnaSiteIndex].stopFreq[0],
						segmentSetting[vnaSiteIndex].point[0],
						segmentSetting[vnaSiteIndex].ifbw[0])); 
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ConfigurePower_PowerSweep(2,
							segmentSetting[vnaSiteIndex].startFreq[0],
							segmentSetting[vnaSiteIndex].stopFreq[0],
							segmentSetting[vnaSiteIndex].point[0],
							segmentSetting[vnaSiteIndex].ifbw[0]));
					/*tl->CheckError(tfSite, tfSite, amb7300_[vnaSiteIndex]->amb7300->MeasureSparamSetupPowerSweep(
						vnaChannel,
						SParam_FormatVNA(segmentSetting[vnaSiteIndex].sparaFormat),
						FreqPow_StyleVNA::StartStop,
						segmentSetting[vnaSiteIndex].power[0],
						segmentSetting[vnaSiteIndex].startFreq[0],
						segmentSetting[vnaSiteIndex].stopFreq[0],
						segmentSetting[vnaSiteIndex].ifbw[0],
						segmentSetting[vnaSiteIndex].point[0],
						segmentSetting[vnaSiteIndex].singleMarker));*/

						// Update array size of Freq Point if segment is configured
					if (segmentSetting[vnaSiteIndex].sweepType == Vna_SharedMemoryTransfer_PowerSweep)
					{
						ret = GetChannelandTraceSetting_Keysight(tfSite, vnaSiteIndex);
					}
				}
			}
#pragma endregion

		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_POWERSWEEP_SETTING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigurePowerSweepSetting_Keysight] Fail to configure power sweep setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigurePowerSweepSetting_Keysight] Fail to configure power sweep setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Trigger Settings [Stimulus->Trigger]
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureTriggerSource_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerSource_Keysight
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
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTriggerSource_Keysight] Fail to configure trigger source." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTriggerSource_Keysight] Fail to configure trigger source." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	// [Future Enchancement] API not support TriggerExternal function.
	// [Future Enchancement] API not support TriggMOerOutput function.
	int AMB7300TestLibrary::ConfigureTriggerScope_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTriggerScope_Keysight
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
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTriggerScope_Keysight] Fail to configure trigger scope." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTriggerScope_Keysight] Fail to configure trigger scope." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureSingleTriggerForGetFromStateFileType_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSingleTriggerForGetFromStateFileType_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to send a single trigger to the vna for GetFromStateFile mode only.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		String ^ OPC = String::Empty;

		try
		{
			//if (segmentSetting[vnaSiteIndex].GetFromStateFile == true)
			{
				// [Future Enchancement] API not support.
				//// Display active channel
				//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

				//// Configure stimulus trigger initial | Need to apply Channel_Stimulus_TriggerInitial() + Channel_Stimulus_TriggerSingle() for GetFromStateFile type
				//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_TriggerInitial(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

				// Send single trigger | Need to apply Channel_Stimulus_TriggerInitial() + Channel_Stimulus_TriggerSingle() for GetFromStateFile type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Channel_Stimulus_TriggerSingle());

				// Wait until sweep is finished
				//while (OPC)
				//{
				//	tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->OPCStatus_Get(OPC));
				//	if ((OPC->Contains("1")) || (ret == ER_CONST_TERMINATION_CHARACTER_VISA_ERROR)) // [Info] By referring to wrapper, it is using &&, currently change to || and need to keep monitor on it whenever there is project with online debug.
				//	{
				//		ret = 0; //reset ret to 0 to solve 0x3FFF0005 termination character VISA error
				//		break;
				//	}
				//}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GETFROMSTATEFILE_CONFIGURE_SINGLE_TRIGGER_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Fail to configure single trigger for GetFromStateFile type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSingleTriggerForGetFromStateFileType_Keysight] Fail to configure single trigger for GetFromStateFile type." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureActiveChannelAndSingleTrigger_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureActiveChannelAndSingleTrigger_Keysight
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
			// [Future Enchancement] API not support.
			//// Display active channel
			//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

			// Arm wait for data --> including: hold all channel, init target channel, send single trigger, get opc
			tl->CheckError(tfSite, amb7300_[vnaSiteIndex]->amb7300->ArmWaitForData(vnaSetting[vnaSiteIndex].pinAlias));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRIGGER_ARM_WAIT_FOR_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Fail to configure active channel, arm and wait for data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureActiveChannelAndSingleTrigger_Keysight] Fail to configure active channel, arm and wait for data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
	int AMB7300TestLibrary::ConfigureTraces_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureTraces_Keysight
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
		int ret = 0;
		int traceParameterType = 999;
		int tracePort1 = 999;
		int tracePort2 = 999;
		int Trace; //Trace used in Add Api
		String^ temp;
		String^ command;

		try
		{
			//cast the First Trace to Int
			temp = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
			Trace = Convert::ToInt32(temp);

			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
				{
					//get the trace count in VNA, VNA_TraceCount == 0 after program first load only
					if (vnaSetting[siteIndex].VNA_TraceCount == 0)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_TraceCount_Get(vnaSetting[siteIndex].currentVnaFetchChannel, vnaSetting[siteIndex].VNA_TraceCount));
						if (vnaSetting[siteIndex].VNA_TraceCount != vnaSetting[siteIndex].userTraceCount) //check if trace count of channel in tf3 same as trace count in VNA
						{

#pragma region "[Display->Num Of Traces]"

							// [Future Enchancement] API not support.
							// Setup number of traces --> fix to max 16x traces
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_NumOfTrace_Set(vnaSetting[siteIndex].currentVnaFetchChannel, vnaSetting[siteIndex].userTraceCount));
							
							//try
							//array<int>^ out_TraceNum = gcnew array<int>(6);
							//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_TraceNumber_Get(vnaSetting[siteIndex].currentVnaFetchChannel, out_TraceNum));

							//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveTrace_Set(vnaSetting[siteIndex].currentVnaFetchChannel, 6));
#pragma endregion

#pragma region "[Display->Allocate Traces]"

							// [Future Enchancement] API not support.
							//// Setup traces allocation for the selected channel --> fix to max 16x traces
							//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_AllocateTrace(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, 16));

#pragma endregion

#pragma region "[Measurement->S11/S12/S21/S22/...] & [Format->MLOG/SWR/...]"

							// For every trace that specified by the users
							for (int iTraceIndex = 0; iTraceIndex < vnaSetting[vnaSiteIndex].vna_trace->Length; iTraceIndex++)
							{
								if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] != Vna_Measurement_Type_Reset)
								{

#pragma region "Get measurement parameter based on user input measure type (without smo)"

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

#pragma region "Trace parameter port"
									if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S11 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A11 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B11)
									{
										tracePort1 = 1;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S12 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A12 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B12)
									{
										tracePort1 = 1;
										tracePort2 = 2;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S13 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A13 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B13)
									{
										tracePort1 = 1;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S14 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A14 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B14)
									{
										tracePort1 = 1;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S15 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A15 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B15)
									{
										tracePort1 = 1;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S16 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A16 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B16)
									{
										tracePort1 = 1;
										tracePort2 = 6;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S21 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A21 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B21)
									{
										tracePort1 = 2;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S22 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A22 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B22)
									{
										tracePort1 = 2;
										tracePort2 = 2;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S23 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A23 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B23)
									{
										tracePort1 = 2;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S24 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A24 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B24)
									{
										tracePort1 = 2;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S25 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A25 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B25)
									{
										tracePort1 = 2;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S26 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A26 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B26)
									{
										tracePort1 = 2;
										tracePort2 = 6;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S31 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A31 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B31)
									{
										tracePort1 = 3;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S32 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A32 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B32)
									{
										tracePort1 = 3;
										tracePort2 = 2;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S33 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A33 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B33)
									{
										tracePort1 = 3;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S34 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A34 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B34)
									{
										tracePort1 = 3;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S35 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A35 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B35)
									{
										tracePort1 = 3;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S36 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A36 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B36)
									{
										tracePort1 = 3;
										tracePort2 = 6;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S41 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A41 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B41)
									{
										tracePort1 = 4;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S42 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A42 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B42)
									{
										tracePort1 = 4;
										tracePort2 = 2;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S43 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A43 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B43)
									{
										tracePort1 = 4;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S44 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A44 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B44)
									{
										tracePort1 = 4;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S45 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A45 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B45)
									{
										tracePort1 = 4;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S46 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A46 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B46)
									{
										tracePort1 = 4;
										tracePort2 = 6;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S51 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A51 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B51)
									{
										tracePort1 = 5;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S52 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A52 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B52)
									{
										tracePort1 = 5;
										tracePort2 = 2;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S53 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A53 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B53)
									{
										tracePort1 = 5;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S54 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A54 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B54)
									{
										tracePort1 = 5;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S55 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A55 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B55)
									{
										tracePort1 = 5;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S56 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A56 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B56)
									{
										tracePort1 = 5;
										tracePort2 = 6;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S61 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A61 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B61)
									{
										tracePort1 = 6;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S62 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A62 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B62)
									{
										tracePort1 = 6;
										tracePort2 = 1;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S63 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A63 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B63)
									{
										tracePort1 = 6;
										tracePort2 = 3;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S64 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A64 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B64)
									{
										tracePort1 = 6;
										tracePort2 = 4;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S65 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A65 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B65)
									{
										tracePort1 = 6;
										tracePort2 = 5;
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_S66 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_A66 ||
										vnaSetting[vnaSiteIndex].measType[iTraceIndex] == Vna_Measurement_Type_B66)
									{
										tracePort1 = 6;
										tracePort2 = 6;
									}
#pragma endregion

#pragma endregion

									// Select the active trace at the vna sfp
									//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Display_ActiveTrace_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaSetting[vnaSiteIndex].vna_trace[iTraceIndex]));
									String^ MeasureFormat;

									if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("A"))
									{
										MeasureFormat = "A";
										command = String::Format("CALC{0}:MEAS{1}:PAR '{2}' \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel, Trace + iTraceIndex, MeasureFormat + tracePort2 + "," + tracePort1); // "A1,2"
										amb7300_NA[tfSite]->System_Scpi_WriteString(command);
									}
									else if (vnaSetting[vnaSiteIndex].measType[iTraceIndex]->Contains("B"))
									{
										MeasureFormat = "B";
										command = String::Format("CALC{0}:MEAS{1}:PAR '{2}' \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel, Trace + iTraceIndex, MeasureFormat + tracePort2 + "," + tracePort1); // "A1,2"
										amb7300_NA[tfSite]->System_Scpi_WriteString(command);
									}
									else // Generic Sxx Trace
									{
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
							}

#pragma endregion
							vnaSetting[siteIndex].VNA_TraceCount = vnaSetting[siteIndex].userTraceCount;
						}
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_TRACE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTraces_Keysight] Fail to configure trace setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureTraces_Keysight] Fail to configure trace setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
	int AMB7300TestLibrary::GetTraceFormatData_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetTraceFormatData_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to read formated data from the target trace, and store in TraceFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

		try
		{
			// Total points
			traceFormatData->totalFreqPoints = segmentSetting[vnaSiteIndex].totalPoint;

#pragma region "Initiallize traceFormatData array"
			// Raw data
			traceFormatData->XAxis = gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->realRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
			traceFormatData->imaginaryRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
			// Freq
			traceFormatData->freqPoint = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S11
			traceFormatData->S11Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S11Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S12
			traceFormatData->S12Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S12Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S13
			traceFormatData->S13Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S13Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S14
			traceFormatData->S14Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S14Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S15
			traceFormatData->S15Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S15Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S16
			traceFormatData->S16Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S16Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S21
			traceFormatData->S21Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S21Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S22
			traceFormatData->S22Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S22Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S23
			traceFormatData->S23Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S23Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S24
			traceFormatData->S24Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S24Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S25
			traceFormatData->S25Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S25Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S26
			traceFormatData->S26Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S26Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S31
			traceFormatData->S31Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S31Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S32
			traceFormatData->S32Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S32Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S33
			traceFormatData->S33Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S33Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S34
			traceFormatData->S34Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S34Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S35
			traceFormatData->S35Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S35Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S36
			traceFormatData->S36Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S36Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S41
			traceFormatData->S41Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S41Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S42
			traceFormatData->S42Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S42Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S43
			traceFormatData->S43Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S43Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S44
			traceFormatData->S44Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S44Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S45
			traceFormatData->S45Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S45Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S46
			traceFormatData->S46Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S46Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S51
			traceFormatData->S51Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S51Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S52
			traceFormatData->S52Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S52Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S53
			traceFormatData->S53Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S53Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S54
			traceFormatData->S54Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S54Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S55
			traceFormatData->S55Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S55Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S56
			traceFormatData->S56Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S56Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S61
			traceFormatData->S61Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S61Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S62
			traceFormatData->S62Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S62Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S63
			traceFormatData->S63Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S63Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S64
			traceFormatData->S64Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S64Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S65
			traceFormatData->S65Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S65Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			// S66
			traceFormatData->S66Real = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
			traceFormatData->S66Imaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
#pragma endregion

			// Get trace format data from the selective channel and trace
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
				traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));

			// [CHECK] if total freq points from user does not match with the actual trace data points
			if (traceFormatData->realRaw->Length != traceFormatData->totalFreqPoints)
			{
				ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetTraceFormatData_Keysight] Target test frequency does not exist in the trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			// Store trace data accordingly
			for (int i = 0; i < traceFormatData->totalFreqPoints; i++)
			{
				if (vnaSetting[vnaSiteIndex].vna_traceFormat[TraceNum - 1] != Vna_Format_Reset)
				{
#pragma region "Transfer realRaw & imaginaryRaw to respective Sxx array"
					if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S11)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S12)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S12Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S12Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S13)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S13Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S13Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S14)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S14Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S14Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S15)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S15Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S15Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S16)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S16Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S16Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S21)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S21Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S21Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S22)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S22Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S22Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S23)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S23Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S23Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S24)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S24Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S24Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S25)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S25Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S25Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S26)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S26Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S26Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S31)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S31Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S31Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S32)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S32Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S32Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S33)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S33Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S33Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S34)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S34Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S34Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S35)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S35Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S35Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S36)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S36Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S36Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S41)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S41Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S41Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S42)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S42Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S42Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S43)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S43Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S43Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S44)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S44Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S44Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S45)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S45Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S45Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S46)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S46Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S46Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S51)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S51Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S51Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S52)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S52Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S52Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S53)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S53Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S53Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S54)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S54Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S54Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S55)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S55Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S55Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S56)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S56Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S56Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S61)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S61Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S61Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S62)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S62Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S62Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S63)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S63Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S63Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S64)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S64Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S64Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S65)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S65Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S65Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
					else if (vnaSetting[vnaSiteIndex].measType[TraceNum - 1] == Vna_Measurement_Type_S66)
					{
						traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[i];
						traceFormatData->S66Real[vnaSiteIndex, i] = traceFormatData->realRaw[i];
						traceFormatData->S66Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[i];
					}
#pragma endregion
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_TRACE_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetTraceFormatData_Keysight] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetTraceFormatData_Keysight] Fail to get trace format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerSearchFormatData_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchFormatData_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search and read data (MinMax, Peak, Target)
		**		from the target trace and store in MarkerSearchFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_msMinMax)
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
				msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msMinMax_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(msSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerSearchSetting[i] = gcnew Marker_SearchAtt();
					markerSearchSetting[i]->_search_Start = 0.0 MHz; // default
					markerSearchSetting[i]->_search_Stop = 0.0 MHz; // default
					msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE); // default

																									   // Marker search range enable
					if (analysisSetting.msMinMax_SearchRangeEnable[i] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						markerSearchSetting[i]->_searchRange_Enable = true;

						// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
						if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
							(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
							(vnaDataAnalysisTPC.testFreqRangeStart[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
						{
							markerSearchSetting[i]->_search_Start = vnaDataAnalysisTPC.testFreqRangeStart[i];
							markerSearchSetting[i]->_search_Stop = vnaDataAnalysisTPC.testFreqRangeStop[i];
						}
						// Apply default setting if user input empty or input invalid
						else
						{
							markerSearchSetting[i]->_search_Start = 0.0 MHz;
							markerSearchSetting[i]->_search_Stop = 0.0 MHz;
							ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
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
						markerSearchSetting[i]->Type = Marker_SearchType::MAX;
						msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE);
					}
					else if (analysisSetting.msMinMax_SearchType[i] == Cond_AnalysisSetting_MS_Type_Minimum)
					{
						markerSearchSetting[i]->Type = Marker_SearchType::MIN;
						msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MIN_TYPE);
					}
				}

				// Variables for marker search min max API
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					msSetting[vnaSiteIndex].ms_SearchStart[i] = markerSearchSetting[i]->_search_Start;
					msSetting[vnaSiteIndex].ms_SearchStop[i] = markerSearchSetting[i]->_search_Stop;
				}

				// Check existing marker count | add or remove marker by referring to the user input
				msSetting[vnaSiteIndex].markerIndex = 0;
				/*for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
				// Check the existing markers on the target trace
				int existingMarkerCount = 0;
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, existingMarkerCount));

				// If existing marker more than needed marker --> remove extra marker
				if (existingMarkerCount > markerSearchSetting->Length)
				{
				// Delete all marker
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

				// Add marker
				int markerAdded = (int)markerSearchSetting->Length;
				for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
				{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
				}
				}
				// If existing marker less than needed marker --> add extra marker
				else if (existingMarkerCount < markerSearchSetting->Length)
				{
				// Add marker
				int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
				for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
				{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
				}
				}
				else
				{
				// Re-define the marker frequency
				for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
				{
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
				}
				}

				// Record current marker index
				msSetting[vnaSiteIndex].markerIndex = i + 1;

				// Execute based on the marker search min max type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
				msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msMinMax_Type[i]));
				}*/

				//testing new api ms range set, must for SearchRange
				String^ command = nullptr;
				//Select trace in each window, for window 2 having 5~8 trace -> Channel 2 Trace 4;
				command = String::Format("DISP:WIND{0}:TRAC{1}:SEL \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum);
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1 ON \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

				ret += amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, msSetting[vnaSiteIndex].targetMarkerCount, 1, msSetting[vnaSiteIndex].ms_SearchStart[0], msSetting[vnaSiteIndex].ms_SearchStop[0]);

				// Result variables
				msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->markerType = gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msType = gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					msFormatData->XAxis[i] = (double)CONST_INVALID_RESULT;
					msFormatData->realRaw[i] = (double)CONST_INVALID_RESULT;
					msFormatData->imaginaryRaw[i] = (double)CONST_INVALID_RESULT;
					msFormatData->markerType[i] = String::Empty;
					msFormatData->msFreq[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					msFormatData->msReal[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					msFormatData->msImaginary[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					msFormatData->msType[vnaSiteIndex, i] = String::Empty;
				}

				// Get marker search min max data, older api
				/*tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
				msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));*/

				//testing new api of min max data
				array<int>^ UserID = gcnew array<int>(1) { 1 };
				array<int>^ SearchType = gcnew array<int>(1) { msSetting[vnaSiteIndex].msMinMax_Type[0] };
				array<double>^ Frequency = gcnew array<double>(1) { msFormatData->msFreq[vnaSiteIndex, 0] };
				array<double>^ Real = gcnew array<double>(1) { msFormatData->msReal[vnaSiteIndex, 0] };
				array<double>^ Imag = gcnew array<double>(1) { msFormatData->msImaginary[vnaSiteIndex, 0] };
				
				ret += amb7300_NA[vnaSiteIndex]->Marker_ReadMarkerSearch(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, UserID, SearchType, Frequency, Real, Imag);

				// Store marker search min max data accordingly
				if (msFormatData->XAxis->Length != 0)
				{
					for (int i = 0; i < msFormatData->XAxis->Length; i++)
					{
						//msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
						//msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
						//msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
						msFormatData->msType[vnaSiteIndex, i] = msFormatData->markerType[i];
						msFormatData->msFreq[vnaSiteIndex, i] = Frequency[0];
						msFormatData->msReal[vnaSiteIndex, i] = Real[i];
						msFormatData->msImaginary[vnaSiteIndex, i] = Imag[0];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
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
				msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].targetMakerFreq = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_PeakExcursion = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msPeak_PeakPolarity = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				/*msSetting[vnaSiteIndex].msPeak_PeakThreshold	= gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);*/

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
						//msSetting[vnaSiteIndex].msPeak_Type[i] = int(PeakSearchType::); // Unavailable
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

					/*//Marker search peak threshold
					msSetting[vnaSiteIndex].msPeak_PeakThreshold[i] = analysisSetting.msPeak_PeakThreshold[i];*/

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
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
						(vnaDataAnalysisTPC.testFreqRangeStart[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]) &&
						(vnaDataAnalysisTPC.testFreq[i] != 0) &&
						(vnaDataAnalysisTPC.testFreq[i] >= vnaDataAnalysisTPC.testFreqRangeStart[i]) &&
						(vnaDataAnalysisTPC.testFreq[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i] = vnaDataAnalysisTPC.testFreq[i];
						msSetting[vnaSiteIndex].ms_SearchStart[i] = vnaDataAnalysisTPC.testFreqRangeStart[i];
						msSetting[vnaSiteIndex].ms_SearchStop[i] = vnaDataAnalysisTPC.testFreqRangeStop[i];
					}
					// Apply default setting if user input empty or input invalid
					else
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i] = 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStart[i] = 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStop[i] = 0.0 MHz;
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSPEAK_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msPeak] For marker search peak, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msPeak] For marker search peak, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, existingMarkerCount));

					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

						// Add marker
						int markerAdded = msSetting[vnaSiteIndex].targetMarkerCount;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Add marker
						int markerAdded = msSetting[vnaSiteIndex].targetMarkerCount - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;


					// [Future Enchancement] API not support.
					//// Select the active marker 
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerActive_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					//	msSetting[vnaSiteIndex].markerIndex));
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchCouple_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					//	true));  // [Future Enchancement] Previously set to false. Need to clarify what is this function.
					//// SCPI: CALC{}:MEAS{}:MARK{}:COUP {} //ON/OFF

					//testing new api for threshold ms peak
					//ret += amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchPeakThreshold_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					//	msSetting[vnaSiteIndex].markerIndex, -100.0);

					// Configure the generic marker search range start stop and enable the search range  
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, true));

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRangeStartStop_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].ms_SearchStart[i], msSetting[vnaSiteIndex].ms_SearchStop[i]));

					// Configure the peak excursion for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerPExcursion_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_PeakExcursion[i]));

					// Configure the peak polarity type for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchTargetPolarity_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_PeakPolarity[i]));

					// Execute based on the marker search peak type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msPeak_Type[i]));

					// Result variables
					msFormatData->numberOfMarker = 0;
					msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					for (int j = 0; j < msSetting[vnaSiteIndex].targetMarkerCount; j++)
					{
						msFormatData->XAxis[j] = (double)CONST_INVALID_RESULT;
						msFormatData->realRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->imaginaryRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->msFreq[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msReal[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msImaginary[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
					}

					// Get marker search target data
					/*tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					msFormatData->numberOfMarker, msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw));*/

					//testing new api for marker search
					array<int>^ UserID = gcnew array<int>(1) { 1 };
					array<int>^ SearchType = gcnew array<int>(1) { msSetting[vnaSiteIndex].msPeak_Type[0] };
					array<double>^ Frequency = gcnew array<double>(1) { msFormatData->msFreq[vnaSiteIndex, 0] };
					array<double>^ Real = gcnew array<double>(1) { msFormatData->msReal[vnaSiteIndex, 0] };
					array<double>^ Imag = gcnew array<double>(1) { msFormatData->msImaginary[vnaSiteIndex, 0] };
					ret += amb7300_NA[vnaSiteIndex]->Marker_ReadMarkerSearch(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, UserID, SearchType, Frequency, Real, Imag);

					// Store marker search min max data accordingly
					if (msFormatData->XAxis->Length != 0)
					{
						for (int i = 0; i < msFormatData->XAxis->Length; i++)
						{
							msFormatData->msFreq[vnaSiteIndex, i] = Frequency[i];
							msFormatData->msReal[vnaSiteIndex, i] = Real[i];
							msFormatData->msImaginary[vnaSiteIndex, i] = Imag[i];
						}
					}
					else
					{
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSPEAK_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msPeak] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msPeak] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_msTarget)
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
				msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].targetMakerFreq = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetValue = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetTransition = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetLineEnable = gcnew array<bool>(msSetting[vnaSiteIndex].targetMarkerCount);

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

					//// [Future Enchancement] API not support.
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
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
						(vnaDataAnalysisTPC.testFreqRangeStart[i] < vnaDataAnalysisTPC.testFreqRangeStop[i]) &&
						(vnaDataAnalysisTPC.testFreq[i] != 0) &&
						(vnaDataAnalysisTPC.testFreq[i] >= vnaDataAnalysisTPC.testFreqRangeStart[i]) &&
						(vnaDataAnalysisTPC.testFreq[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i] = vnaDataAnalysisTPC.testFreq[i];
						msSetting[vnaSiteIndex].ms_SearchStart[i] = vnaDataAnalysisTPC.testFreqRangeStart[i];
						msSetting[vnaSiteIndex].ms_SearchStop[i] = vnaDataAnalysisTPC.testFreqRangeStop[i];
					}
					// Apply default setting if user input empty or input invalid
					else
					{
						msSetting[vnaSiteIndex].targetMakerFreq[i] = 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStart[i] = 0.0 MHz;
						msSetting[vnaSiteIndex].ms_SearchStop[i] = 0.0 MHz;
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}

					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, existingMarkerCount));

					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;

					// [Future Enchancement] API not support.
					//// Select the active marker 
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerActive_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, 
					//	msSetting[vnaSiteIndex].markerIndex));
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchCouple_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, 
					//	true));
					//// SCPI: CALC{}:MEAS{}:MARK{}:COUP {} //ON/OFF

					// Configure the generic marker search range start stop and enable the search range  
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, true));

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRangeStartStop_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].ms_SearchStart[i], msSetting[vnaSiteIndex].ms_SearchStop[i]));

					// Configure the target value for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerPExcursionTargetValue_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_TargetValue[i]));

					// Configure the target transition type for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchTargetTransitionType_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_TargetTransition[i]));

					// Execute based on the marker search target type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_Type[i]));

					// Result variables
					msFormatData->numberOfMarker = 0;
					msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					for (int j = 0; j < msSetting[vnaSiteIndex].targetMarkerCount; j++)
					{
						msFormatData->XAxis[j] = (double)CONST_INVALID_RESULT;
						msFormatData->realRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->imaginaryRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->msFreq[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msReal[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msImaginary[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
					}

					// Get marker search target data
					/*tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					msFormatData->numberOfMarker, msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw));*/

					//testing new api for marker search
					array<int>^ UserID = gcnew array<int>(1) { 1 };
					array<int>^ SearchType = gcnew array<int>(1) { msSetting[vnaSiteIndex].msTarget_Type[0] };
					array<double>^ Frequency = gcnew array<double>(1) { msFormatData->msFreq[vnaSiteIndex, 0] };
					array<double>^ Real = gcnew array<double>(1) { msFormatData->msReal[vnaSiteIndex, 0] };
					array<double>^ Imag = gcnew array<double>(1) { msFormatData->msImaginary[vnaSiteIndex, 0] };
					ret += amb7300_NA[vnaSiteIndex]->Marker_ReadMarkerSearch(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, UserID, SearchType, Frequency, Real, Imag);

					// Store marker search min max data accordingly
					if (msFormatData->XAxis->Length != 0)
					{
						for (int i = 0; i < msFormatData->XAxis->Length; i++)
						{
							msFormatData->msFreq[vnaSiteIndex, i] = Frequency[i];
							msFormatData->msReal[vnaSiteIndex, i] = Real[i];
							msFormatData->msImaginary[vnaSiteIndex, i] = Imag[i];
						}
					}
					else
					{
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_P1dBTarget)
			{
				/*
				**	Marker Search P1dB Target Description:
				**		Compute the P1dB level based on the gain value at the first input power point.
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
				msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].targetMakerFreq = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetValue = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetTransition = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msTarget_TargetLineEnable = gcnew array<bool>(msSetting[vnaSiteIndex].targetMarkerCount);

				msSetting[vnaSiteIndex].markerIndex = 0;

#pragma region "Initiallize traceFormatData array"
				// Raw data
				traceFormatData->XAxis = gcnew array<double>(traceFormatData->totalFreqPoints);
				traceFormatData->realRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
				traceFormatData->imaginaryRaw = gcnew array<double>(traceFormatData->totalFreqPoints);
				// Freq
				traceFormatData->freqPoint = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, traceFormatData->totalFreqPoints);
#pragma endregion


				// Get trace format data from the selective channel and trace
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));

				//traceFormatData->freqPoint[vnaSiteIndex, i] = traceFormatData->XAxis[0];
				//traceFormatData->S11Real[vnaSiteIndex, i] = traceFormatData->realRaw[0];
				//traceFormatData->S11Imaginary[vnaSiteIndex, i] = traceFormatData->imaginaryRaw[0];

				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					// Marker search target type
					//if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTarget)
					{
						msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_TARGET_TYPE);
					}
					//else if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTargetLeft)
					//{
					//	msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_LEFT_TARGET_TYPE);
					//}
					//else if (analysisSetting.msTarget_Type[i] == Cond_AnalysisSetting_MSTarget_Type_SearchTargetRight)
					//{
					//	msSetting[vnaSiteIndex].msTarget_Type[i] = int(TargetMarkerSearchType::FUNC_RIGHT_TARGET_TYPE);
					//}

					// Marker search target value

					analysisSetting.msTarget_TargetValue[0] = traceFormatData->realRaw[0] - 1 dBm;
					msSetting[vnaSiteIndex].msTarget_TargetValue[i] = analysisSetting.msTarget_TargetValue[i];

					//// Marker search target transition type
					//if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Positive)
					//{
					//	msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_POSITIVE_TYPE);
					//}
					//else if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Negative)
					//{
					//	msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_NEGATIVE_TYPE);
					//}
					//else if (analysisSetting.msTarget_TargetTransition[i] == Cond_AnalysisSetting_MSTarget_TargetTransition_Both)
					//{
					//	msSetting[vnaSiteIndex].msTarget_TargetTransition[i] = int(TTRType::TTR_BOTH_POS_NEG_TYPE);
					//}

					////// [Future Enchancement] API not support.
					////// Marker search target line enable
					////if (analysisSetting.msTarget_TargetLineEnable[i] == Cond_AnalysisSetting_MSTarget_Enable_TargetLineOn)
					////{
					////	msSetting[vnaSiteIndex].msTarget_TargetLineEnable[i] = true;
					////}
					////else if (analysisSetting.msTarget_TargetLineEnable[i] == Cond_AnalysisSetting_MSTarget_Enable_TargetLineOff)
					////{
					////	msSetting[vnaSiteIndex].msTarget_TargetLineEnable[i] = false;
					////}

					//// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
					//if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
					//	(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
					//	(vnaDataAnalysisTPC.testFreqRangeStart[i] < vnaDataAnalysisTPC.testFreqRangeStop[i]) &&
					//	(vnaDataAnalysisTPC.testFreq[i] != 0) &&
					//	(vnaDataAnalysisTPC.testFreq[i] >= vnaDataAnalysisTPC.testFreqRangeStart[i]) &&
					//	(vnaDataAnalysisTPC.testFreq[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					//{
					//	msSetting[vnaSiteIndex].targetMakerFreq[i] = vnaDataAnalysisTPC.testFreq[i];
					//	msSetting[vnaSiteIndex].ms_SearchStart[i] = vnaDataAnalysisTPC.testFreqRangeStart[i];
					//	msSetting[vnaSiteIndex].ms_SearchStop[i] = vnaDataAnalysisTPC.testFreqRangeStop[i];
					//}
					//// Apply default setting if user input empty or input invalid
					//else
					//{
					//	msSetting[vnaSiteIndex].targetMakerFreq[i] = 0.0 MHz;
					//	msSetting[vnaSiteIndex].ms_SearchStart[i] = 0.0 MHz;
					//	msSetting[vnaSiteIndex].ms_SearchStop[i] = 0.0 MHz;
					//	ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
					//	tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] For marker search target, Freq & FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart. Freq must within FreqRangeStart and FreqRangeStop." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	goto EndOfTest;
					//}

					// Check the existing markers on the target trace
					int existingMarkerCount = 0;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, existingMarkerCount));

					// If existing marker more than needed marker --> remove extra marker
					if (existingMarkerCount > msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Delete all marker 
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount;
						for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					// If existing marker less than needed marker --> add extra marker
					else if (existingMarkerCount < msSetting[vnaSiteIndex].targetMarkerCount)
					{
						// Add marker
						int markerAdded = (int)msSetting[vnaSiteIndex].targetMarkerCount - existingMarkerCount;
						for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}
					else
					{
						// Re-define the marker frequency
						for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].targetMakerFreq[i]));
						}
					}

					// Record current marker index
					msSetting[vnaSiteIndex].markerIndex = i + 1;

					// [Future Enchancement] API not support.
					//// Select the active marker 
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerActive_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, 
					//	msSetting[vnaSiteIndex].markerIndex));
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchCouple_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, 
					//	true));
					//// SCPI: CALC{}:MEAS{}:MARK{}:COUP {} //ON/OFF

					//// Configure the generic marker search range start stop and enable the search range  
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRange_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, true));

					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchRangeStartStop_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					//	msSetting[vnaSiteIndex].ms_SearchStart[i], msSetting[vnaSiteIndex].ms_SearchStop[i]));

					// Configure the target value for the active marker
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerPExcursionTargetValue_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_TargetValue[i]));

					//// Configure the target transition type for the active marker
					//tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearchTargetTransitionType_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					//	msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_TargetTransition[i]));

					// Execute based on the marker search target type
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
						msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msTarget_Type[i]));

					// Result variables
					msFormatData->numberOfMarker = 0;
					msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
					for (int j = 0; j < msSetting[vnaSiteIndex].targetMarkerCount; j++)
					{
						msFormatData->XAxis[j] = (double)CONST_INVALID_RESULT;
						msFormatData->realRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->imaginaryRaw[j] = (double)CONST_INVALID_RESULT;
						msFormatData->msFreq[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msReal[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
						msFormatData->msImaginary[vnaSiteIndex, j] = (double)CONST_INVALID_RESULT;
					}

					// Get marker search target data
					/*tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					msFormatData->numberOfMarker, msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw));*/

					//testing new api for marker search
					array<int>^ UserID = gcnew array<int>(1) { 1 };
					array<int>^ SearchType = gcnew array<int>(1) { msSetting[vnaSiteIndex].msTarget_Type[0] };
					array<double>^ Frequency = gcnew array<double>(1) { msFormatData->msFreq[vnaSiteIndex, 0] };
					array<double>^ Real = gcnew array<double>(1) { msFormatData->msReal[vnaSiteIndex, 0] };
					array<double>^ Imag = gcnew array<double>(1) { msFormatData->msImaginary[vnaSiteIndex, 0] };
					ret += amb7300_NA[vnaSiteIndex]->Marker_ReadMarkerSearch(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, UserID, SearchType, Frequency, Real, Imag);

					// Store marker search min max data accordingly
					if (msFormatData->XAxis->Length != 0)
					{
						for (int i = 0; i < msFormatData->XAxis->Length; i++)
						{
							msFormatData->msFreq[vnaSiteIndex, i] = Frequency[i];
							msFormatData->msReal[vnaSiteIndex, i] = Real[i];
							msFormatData->msImaginary[vnaSiteIndex, i] = Imag[i];
						}
					}
					else
					{
						ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSTARGET_FAIL;
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msTarget] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerMathFormatData_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerMathFormatData_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker math and read data (Statistic, BwSearch, Flatness)
		**		from the target trace and store in MarkerMathFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		double temp;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

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
				mmSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable = gcnew array<bool>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting = gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i] = gcnew Marker_MathAtt();
					markerMathSetting[i]->Type = Marker_MathType::Statistic;
					markerMathSetting[i]->Marker1 = 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2 = 0.0 MHz;	// Default

																// mmStatistics StatisticsRange
					if (analysisSetting.mmStatistics_StatisticsRangeEnable[i] == Cond_AnalysisSetting_MMStatistics_Enable_StatisticsRangeOn)
					{
						markerMathSetting[i]->StatisticRangeEnable = true;

						// Make sure both marker1 and marker2 does not contain 0Hz | if marker1 and marker2 reverse in value accending order does not affect statistics test
						if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
							(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
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
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmStatistics] If enable StatisticsRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmStatistics] If enable StatisticsRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
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
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable[i] = markerMathSetting[i]->StatisticRangeEnable;
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart[i] = markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop[i] = markerMathSetting[i]->Marker2;
				}

				// Delete all existing marker because the wrapper will create and always use marker #1 & #2
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

				// Result variables
				mmFormatData->mmStatistics_MeanRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_StdDevRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_PeakToPeakRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_Mean = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_StdDev = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmStatistics_PeakToPeak = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmStatistics_MeanRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_StdDevRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_PeakToPeakRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_Mean[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				}

				// Get marker math statistics data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadStatisticData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					mmSetting[vnaSiteIndex].mmStatistics_StatisticsRangeEnable, mmSetting[vnaSiteIndex].mmStatistics_StatisticsStart, mmSetting[vnaSiteIndex].mmStatistics_StatisticsStop,
					mmFormatData->mmStatistics_MeanRaw, mmFormatData->mmStatistics_StdDevRaw, mmFormatData->mmStatistics_PeakToPeakRaw));

				// Store marker math statistics data accordingly
				if (mmFormatData->mmStatistics_MeanRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmStatistics_MeanRaw->Length; i++)
					{
						mmFormatData->mmStatistics_Mean[vnaSiteIndex, i] = mmFormatData->mmStatistics_MeanRaw[i];
						mmFormatData->mmStatistics_StdDev[vnaSiteIndex, i] = mmFormatData->mmStatistics_StdDevRaw[i];
						mmFormatData->mmStatistics_PeakToPeak[vnaSiteIndex, i] = mmFormatData->mmStatistics_PeakToPeakRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMSTATISTICS_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmStatistics] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmStatistics] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if ((vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch) ||
				(vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs))
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
							tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmBwSearch] If enable SearchRefMarker, Freq cannot remain 0Hz." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							goto EndOfTest;
						}

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.mmBwSearch_BandwidthValue[i];

						// Delete all existing marker if using Marker as ref because the wrapper will create and always use marker #1
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));
					}
					else if (analysisSetting.mmBwSearch_SearchRef[i] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak)
					{
						markerMathSetting[i]->Marker_BWSearchRef = Marker_BWSearchRef::Max; 
						// [Future Enchancement] Marker_BWSearchRef:: does not contain PEAK. Wrapper using enum = 1 refer to PEAK option. For now use ::Max (enum = 1)

						// mmBwSearch BwValue
						markerMathSetting[i]->BWValue = analysisSetting.mmBwSearch_BandwidthValue[i];

						if (analysisSetting.mmBwSearch_Condition[i] == Cond_AnalysisSetting_MMBwSearch_Abs_SearchRef_Condition)
						{
							//Find peak then recalculate the -10dB BW level for skysemi (AbsBw Value)
							String^ command = nullptr;
							command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK:AOFF \n");
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							//Select trace in each window, for window 2 having 5~8 trace -> Channel 2 Trace 4;
							command = String::Format("DISP:WIND{0}:TRAC{1}:SEL \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum);
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1 ON \n");
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1:FUNC:EXEC MAX \n");
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1:Y? \n");
							amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
							String ^ Val = nullptr;
							array<String^>^ RawData2 = nullptr;
							amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
							RawData2 = Val->Split(',');
							temp = Double::Parse(RawData2[0]);
							temp = (markerMathSetting[i]->BWValue) - temp;
							markerMathSetting[i]->BWValue = temp;
						}

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
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
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
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmBwSearch] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
			else if (vnaDataAnalysisTPC.function == "Cond_Function_mmFlatness_API_NotSupported_For_KeysightVna")
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
				mmSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				mmSetting[vnaSiteIndex].mmFlatness_Marker1 = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmSetting[vnaSiteIndex].mmFlatness_Marker2 = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_MathAtt^> ^ markerMathSetting = gcnew array<Marker_MathAtt^>(mmSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					markerMathSetting[i] = gcnew Marker_MathAtt();
					markerMathSetting[i]->Type = Marker_MathType::Flatness;
					markerMathSetting[i]->Marker1 = 0.0 MHz;	// Default
					markerMathSetting[i]->Marker2 = 0.0 MHz;	// Default

																// Make sure both marker1 and marker2 does not contain 0Hz | if marker1 and marker2 reverse in value accending order does not affect flatness test
					if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
						(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
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
						tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmFlatness] For marker math flatness, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmFlatness] For marker math flatness, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}

				for (int i = 0; i < markerMathSetting->Length; i++)
				{
					mmSetting[vnaSiteIndex].mmFlatness_Marker1[i] = markerMathSetting[i]->Marker1;
					mmSetting[vnaSiteIndex].mmFlatness_Marker2[i] = markerMathSetting[i]->Marker2;
				}

				// Delete all existing marker because the wrapper will create and always use marker #1 & #2
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

				// Result variables
				mmFormatData->mmFlatness_GainRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_SlopeRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_FlatRaw = gcnew array<double>(mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Gain = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Slope = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				mmFormatData->mmFlatness_Flat = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, mmSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < mmSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					mmFormatData->mmFlatness_GainRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_SlopeRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_FlatRaw[i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Gain[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Slope[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					mmFormatData->mmFlatness_Flat[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				}

				// Get flatness data
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					mmSetting[vnaSiteIndex].mmFlatness_Marker1, mmSetting[vnaSiteIndex].mmFlatness_Marker2,
					mmFormatData->mmFlatness_GainRaw, mmFormatData->mmFlatness_SlopeRaw, mmFormatData->mmFlatness_FlatRaw));

				// Store marker math flatness data accordingly
				if (mmFormatData->mmFlatness_GainRaw->Length != 0)
				{
					for (int i = 0; i < mmFormatData->mmFlatness_GainRaw->Length; i++)
					{
						mmFormatData->mmFlatness_Gain[vnaSiteIndex, i] = mmFormatData->mmFlatness_GainRaw[i];
						mmFormatData->mmFlatness_Slope[vnaSiteIndex, i] = mmFormatData->mmFlatness_SlopeRaw[i];
						mmFormatData->mmFlatness_Flat[vnaSiteIndex, i] = mmFormatData->mmFlatness_FlatRaw[i];
					}
				}
				else
				{
					ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_MMFLATNESS_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmFlatness] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight -> mmFlatness] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_MATH_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Fail to get marker math format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerMathFormatData_Keysight] Fail to get marker math format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerSearchMinMaxRipple_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchMinMaxRipple_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search min and max in a same freq range.
		**		Perform (max - min) to get the ripple result.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

		try
		{
			msSetting[vnaSiteIndex].targetMarkerCount = 2; // fix for this test function | 1x for msMin, 1x for msMax
			msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msSetting[vnaSiteIndex].msMinMax_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);

			Marker_SearchAtt ^ ms1 = gcnew Marker_SearchAtt();
			Marker_SearchAtt ^ ms2 = gcnew Marker_SearchAtt();
			array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(msSetting[vnaSiteIndex].targetMarkerCount);

			ms1->Type = Marker_SearchType::MIN;
			ms1->_searchRange_Enable = true;
			ms1->_search_Start = vnaDataAnalysisTPC.testFreqRangeStart[0];
			ms1->_search_Stop = vnaDataAnalysisTPC.testFreqRangeStop[0];
			markerSearchSetting[0] = ms1;
			msSetting[vnaSiteIndex].ms_SearchStart[0] = markerSearchSetting[0]->_search_Start;
			msSetting[vnaSiteIndex].ms_SearchStop[0] = markerSearchSetting[0]->_search_Stop;
			msSetting[vnaSiteIndex].msMinMax_Type[0] = (int)(MarkerSearchType::FUNC_MIN_TYPE);

			ms2->Type = Marker_SearchType::MAX;
			ms2->_searchRange_Enable = true;
			ms2->_search_Start = vnaDataAnalysisTPC.testFreqRangeStart[0];
			ms2->_search_Stop = vnaDataAnalysisTPC.testFreqRangeStop[0];
			markerSearchSetting[1] = ms2;
			msSetting[vnaSiteIndex].ms_SearchStart[1] = markerSearchSetting[1]->_search_Start;
			msSetting[vnaSiteIndex].ms_SearchStop[1] = markerSearchSetting[1]->_search_Stop;
			msSetting[vnaSiteIndex].msMinMax_Type[1] = (int)(MarkerSearchType::FUNC_MAX_TYPE);

			for (int i = 0; i < vnaDataAnalysisTPC.testFreqRangeStart->Length; i++)
			{
				// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
				if ((vnaDataAnalysisTPC.testFreqRangeStart[i] == 0) ||
					(vnaDataAnalysisTPC.testFreqRangeStop[i] == 0) ||
					(vnaDataAnalysisTPC.testFreqRangeStart[i] >= vnaDataAnalysisTPC.testFreqRangeStop[i]))
				{
					markerSearchSetting[i]->_search_Start = 0.0 MHz;
					markerSearchSetting[i]->_search_Stop = 0.0 MHz;
					ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
					tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}

			// Check existing marker count | add or remove marker by referring to the user input
			msSetting[vnaSiteIndex].markerIndex = 0;
			for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
			{
				// Check the existing markers on the target trace
				int existingMarkerCount = 0;
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerCount_Get(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, existingMarkerCount));

				// If existing marker more than needed marker --> remove extra marker
				if (existingMarkerCount > markerSearchSetting->Length)
				{
					// Delete all marker 
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_RemoveALLMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum));

					// Add marker
					int markerAdded = (int)markerSearchSetting->Length;
					for (int iMarkerIndex = 1; iMarkerIndex <= markerAdded; iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}
				// If existing marker less than needed marker --> add extra marker
				else if (existingMarkerCount < markerSearchSetting->Length)
				{
					// Add marker
					int markerAdded = (int)markerSearchSetting->Length - existingMarkerCount;
					for (int iMarkerIndex = (existingMarkerCount + 1); iMarkerIndex <= (existingMarkerCount + markerAdded); iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}
				else
				{
					// Re-define the marker frequency
					for (int iMarkerIndex = 1; iMarkerIndex <= msSetting[vnaSiteIndex].targetMarkerCount; iMarkerIndex++)
					{
						tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_AddMarker_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, iMarkerIndex, true, msSetting[vnaSiteIndex].ms_SearchStart[i]));
					}
				}

				// Record current marker index
				msSetting[vnaSiteIndex].markerIndex = i + 1;

				// Execute based on the marker search min max type
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_MarkerSearch_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					msSetting[vnaSiteIndex].markerIndex, msSetting[vnaSiteIndex].msMinMax_Type[i]));
			}

			// Result variables
			msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->markerType = gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			msFormatData->msType = gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
			for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
			{
				msFormatData->XAxis[i] = (double)CONST_INVALID_RESULT;
				msFormatData->realRaw[i] = (double)CONST_INVALID_RESULT;
				msFormatData->imaginaryRaw[i] = (double)CONST_INVALID_RESULT;
				msFormatData->markerType[i] = String::Empty;
				msFormatData->msFreq[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				msFormatData->msReal[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				msFormatData->msImaginary[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				msFormatData->msType[vnaSiteIndex, i] = String::Empty;
			}

			// Get marker search min max data
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
				msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));

			// Store marker search min max data accordingly
			if (msFormatData->XAxis->Length != 0)
			{
				for (int i = 0; i < msFormatData->XAxis->Length; i++)
				{
					msFormatData->msFreq[vnaSiteIndex, i] = msFormatData->XAxis[i];
					msFormatData->msReal[vnaSiteIndex, i] = msFormatData->realRaw[i];
					msFormatData->msImaginary[vnaSiteIndex, i] = msFormatData->imaginaryRaw[i];
					msFormatData->msType[vnaSiteIndex, i] = msFormatData->markerType[i];
				}
			}
			else
			{
				ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
				tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				goto EndOfTest;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetMarkerCompressionSearchData_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchFormatData_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search and read data (MinMax, Peak, Target)
		**		from the target trace and store in MarkerSearchFormatData.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		String^ command = nullptr;
		String^ Val = String::Empty;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_P1dB)
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
				msSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
				msSetting[vnaSiteIndex].ms_SearchStart = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].ms_SearchStop = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msSetting[vnaSiteIndex].msMinMax_Type = gcnew array<int>(msSetting[vnaSiteIndex].targetMarkerCount);
				array<Marker_SearchAtt^> ^ markerSearchSetting = gcnew array<Marker_SearchAtt^>(msSetting[vnaSiteIndex].targetMarkerCount);

				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//markerSearchSetting[i] = gcnew Marker_SearchAtt();
					//markerSearchSetting[i]->_search_Start = 0.0 MHz; // default
					//markerSearchSetting[i]->_search_Stop = 0.0 MHz; // default
					//msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE); // default

					//																				   // Marker search range enable
					//if (analysisSetting.msMinMax_SearchRangeEnable[i] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					//{
					//	markerSearchSetting[i]->_searchRange_Enable = true;

					//	// Make sure the search range start stop frequency does not contain 0Hz | search stop must greater than search start
					//	if ((vnaDataAnalysisTPC.testFreqRangeStart[i] != 0) &&
					//		(vnaDataAnalysisTPC.testFreqRangeStop[i] != 0) &&
					//		(vnaDataAnalysisTPC.testFreqRangeStart[i] <= vnaDataAnalysisTPC.testFreqRangeStop[i]))
					//	{
					//		markerSearchSetting[i]->_search_Start = vnaDataAnalysisTPC.testFreqRangeStart[i];
					//		markerSearchSetting[i]->_search_Stop = vnaDataAnalysisTPC.testFreqRangeStop[i];
					//	}
					//	// Apply default setting if user input empty or input invalid
					//	else
					//	{
					//		markerSearchSetting[i]->_search_Start = 0.0 MHz;
					//		markerSearchSetting[i]->_search_Stop = 0.0 MHz;
					//		ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
					//		tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//		tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] If enable SearchRangeOn, FreqRangeStart & FreqRangeStop cannot remain 0Hz. FreqRangeStop cannot less than FreqRangeStart." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//		goto EndOfTest;
					//	}
					//}
					//else if (analysisSetting.msMinMax_SearchRangeEnable[i] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					//{
					//	markerSearchSetting[i]->_searchRange_Enable = false;
					//}

					//// Marker search min max type
					//if (analysisSetting.msMinMax_SearchType[i] == Cond_AnalysisSetting_MS_Type_Maximum)
					//{
					//	markerSearchSetting[i]->Type = Marker_SearchType::MAX;
					//	msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MAX_TYPE);
					//}
					//else if (analysisSetting.msMinMax_SearchType[i] == Cond_AnalysisSetting_MS_Type_Minimum)
					//{
					//	markerSearchSetting[i]->Type = Marker_SearchType::MIN;
					//	msSetting[vnaSiteIndex].msMinMax_Type[i] = (int)(MarkerSearchType::FUNC_MIN_TYPE);
					//}
				}

				//// Variables for marker search min max API
				//for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				//{
				//	msSetting[vnaSiteIndex].ms_SearchStart[i] = markerSearchSetting[i]->_search_Start;
				//	msSetting[vnaSiteIndex].ms_SearchStop[i] = markerSearchSetting[i]->_search_Stop;
				//}

				// Check existing marker count | add or remove marker by referring to the user input
				msSetting[vnaSiteIndex].markerIndex = 0;
				
				////Select trace in each window, for window 2 having 5~8 trace -> Channel 2 Trace 4;
				//command = String::Format("DISP:WIND{0}:TRAC{1}:SEL \n", vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum);
				//amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

				command = String::Format("CALC" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel + ":MEAS" + TraceNum.ToString() + ":MARK1 ON \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				command = String::Format("CALC" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel + ":MEAS" + TraceNum.ToString() +":MARK1:FUNC:COMP:STAT OFF \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				command = String::Format("CALC" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel + ":MEAS" + TraceNum.ToString() + ":MARK1:FUNC:COMP:LEV 1 \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				command = String::Format("CALC" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel + ":MEAS" + TraceNum.ToString() + ":MARK1:FUNC:COMP:STAT ON \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

				// Result variables
				/*msFormatData->XAxis = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->realRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->imaginaryRaw = gcnew array<double>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->markerType = gcnew array<String^>(msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msFreq = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msReal = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msImaginary = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msType = gcnew array<String^, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);*/
				msFormatData->msInputPower = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msOutputPower = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				msFormatData->msOutputGain = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, msSetting[vnaSiteIndex].targetMarkerCount);
				for (int i = 0; i < msSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//msFormatData->XAxis[i] = (double)CONST_INVALID_RESULT;
					//msFormatData->realRaw[i] = (double)CONST_INVALID_RESULT;
					//msFormatData->imaginaryRaw[i] = (double)CONST_INVALID_RESULT;
					//msFormatData->markerType[i] = String::Empty;
					//msFormatData->msFreq[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					//msFormatData->msReal[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					//msFormatData->msImaginary[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					//msFormatData->msType[vnaSiteIndex, i] = String::Empty;
					msFormatData->msInputPower[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					msFormatData->msOutputPower[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
					msFormatData->msOutputGain[vnaSiteIndex, i] = (double)CONST_INVALID_RESULT;
				}

				command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1:FUNC:COMP:PIN? \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
				msFormatData->msInputPower[vnaSiteIndex, 0] = Double::Parse(Val);// Get marker Comp Pin value

				command = String::Format("CALC:MEAS" + TraceNum.ToString() + ":MARK1:FUNC:COMP:POUT? \n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
				msFormatData->msOutputPower[vnaSiteIndex, 0] = Double::Parse(Val);// Get marker Comp Pout value

																				  // Get trace format data from the selective channel and trace
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->ReadFormatedData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum,
					traceFormatData->XAxis, traceFormatData->realRaw, traceFormatData->imaginaryRaw));

				//// Get marker search min max data, older api
				///*tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Marker_ReadData(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, msSetting[vnaSiteIndex].ms_SearchStart, msSetting[vnaSiteIndex].ms_SearchStop,
				//msFormatData->XAxis, msFormatData->realRaw, msFormatData->imaginaryRaw, msFormatData->markerType));*/

				////testing new api of min max data
				//array<int>^ UserID = gcnew array<int>(1) { 1 };
				//array<int>^ SearchType = gcnew array<int>(1) { msSetting[vnaSiteIndex].msMinMax_Type[0] };
				//array<double>^ Frequency = gcnew array<double>(1) { msFormatData->msFreq[vnaSiteIndex, 0] };
				//array<double>^ Real = gcnew array<double>(1) { msFormatData->msReal[vnaSiteIndex, 0] };
				//array<double>^ Imag = gcnew array<double>(1) { msFormatData->msImaginary[vnaSiteIndex, 0] };

				//ret += amb7300_NA[vnaSiteIndex]->Marker_ReadMarkerSearch(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, UserID, SearchType, Frequency, Real, Imag);

				//// Store marker search min max data accordingly
				//if (msFormatData->XAxis->Length != 0)
				//{
				//	for (int i = 0; i < msFormatData->XAxis->Length; i++)
				//	{
				//		//msFormatData->msFreq[vnaSiteIndex, i]		= msFormatData->XAxis[i];
				//		//msFormatData->msReal[vnaSiteIndex, i]		= msFormatData->realRaw[i];
				//		//msFormatData->msImaginary[vnaSiteIndex, i]	= msFormatData->imaginaryRaw[i];
				//		msFormatData->msType[vnaSiteIndex, i] = msFormatData->markerType[i];
				//		msFormatData->msFreq[vnaSiteIndex, i] = Frequency[0];
				//		msFormatData->msReal[vnaSiteIndex, i] = Real[i];
				//		msFormatData->msImaginary[vnaSiteIndex, i] = Imag[0];
				//	}
				//}
				//else
				//{
				//	ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_MSMINMAX_FAIL;
				//	tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//	tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight -> msMinMax] Fail to read marker data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//	goto EndOfTest;
				//}
			}
			
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_FORMAT_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchFormatData_Keysight] Fail to get marker search format data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : Averaging & Smoothing Function
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::ConfigureAveraging_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureAveraging_Keysight
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
					ret = ConfigureActiveChannelAndSingleTrigger_Keysight(tfSite, vnaSiteIndex);
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
				vnaSetting[vnaSiteIndex].currentVnaFetchChannelAve = Vna_Channel_Reset;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_AVERAGING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureAveraging_Keysight] Fail to configure averaging setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureAveraging_Keysight] Fail to configure averaging setting at the active channel." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureSmoothing_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	ConfigureSmoothing_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to configure and enable smoothing at the active trace.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		TraceNum = vnaDataAnalysisTPC.traceIndex + TraceNum - 1; //Eg: First Trace = 5, if traceIndex = 4. After calculation, get 8 -> Trace8;

		try
		{
			// Get trace index with smoothing
			vnaDataAnalysisTPC.traceIndexSmo = TraceNum;

			if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOn)
			{
				// Enable smoothing function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Smoothing_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo, true));

				// configure smoothing aperture
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_SmoothingAperture_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo,
					smoothingSettings.smoothingAperture));

				averageSetting[vnaSiteIndex].enableActiveTraceTransform = true;
				averageSetting[vnaSiteIndex].enableSmoCorrection = true;
			}
			else if (smoothingSettings.smoothingEnable == Cond_SmoothingSetting_Enable_SmoOff)
			{
				// Disable smoothing function
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->Average_Smoothing_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, vnaDataAnalysisTPC.traceIndexSmo, false));

				// Reset
				averageSetting[vnaSiteIndex].enableActiveTraceTransform = false;
				averageSetting[vnaSiteIndex].enableSmoCorrection = false;
				vnaDataAnalysisTPC.traceIndexSmo = Vna_Format_Reset;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_SMOOTHING_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSmoothing_Keysight] Fail to configure smoothing setting at the active trace." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> ConfigureSmoothing_Keysight] Fail to configure smoothing setting at the active trace." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
	int AMB7300TestLibrary::SourceLowVna_Keysight(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		** SourceLowVna_Keysight
		**		tfSite	- This is techFlow site index.
		**
		** Descriptions:
		**		This is a function is to source low each channel with Copper Mountain VNA hardware.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		String^ Power_Level;
		String^ command;
		String^ output;
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
				//tl->CheckError(tfSite, amb7300_NA[siteIndex]->Channel_Stimulus_SourcePower_PortCouple(vnaSetting[siteIndex].currentVnaFetchChannel, false));
				tl->CheckError(tfSite, amb7300_NA[siteIndex]->Channel_Stimulus_SweepType_Get(channel, output));
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SOURCE_LOW_SETTING_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[PostProcessing -> PostProcessingTester -> KeysightVnaUtility -> SourceLowVna_Keysight] Fail to source low Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[PostProcessing -> PostProcessingTester -> KeysightVnaUtility -> SourceLowVna_Keysight] Fail to source low Keysight VNA hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
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
	int AMB7300TestLibrary::SaveTraceDataToTouchstoneFile_Keysight(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	SaveTraceDataToTouchstoneFile_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to save trace data to touchstone file.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] For enable active transform situation, to enable a option 1. all trace are trans. 2. only selective smo trace are trans. 
			if (averageSetting[vnaSiteIndex].enableActiveTraceTransform == true)
			{
				//if [all trace to be auto transformed and save, even only certain trace is applied with smo by the user]
				//{ 

				// Select touchstone format
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFormat_Set(saveRecallSetting->touchstoneFileDataFormat));

				// Save to touchstone file
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSaveSparamFile(saveRecallSetting->touchstoneFilePath));

				//}
				//else [only selective smo trace to be transformed]
				//{
				//	manual process the touchstone content
				//}
			}
			else
			{
				// Select touchstone format
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneFormat_Set(saveRecallSetting->touchstoneFileDataFormat));

				// Save to touchstone file
				tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->SaveRecall_TouchToneSaveSparamFile(saveRecallSetting->touchstoneFilePath));
			}
			do
			{
				String^ command = String::Format("*OPC?\n");
				String^ Val;
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
				//Util->WaitSecond(1 mS);
				if (Val->Contains("1"))
					break;
			} while (true);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SAVE_TRACE_DATA_TO_TOUCHSTONE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SaveTraceDataToTouchstoneFile_Keysight] Fail to save trace data to touchstone file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SaveTraceDataToTouchstoneFile_Keysight] Fail to save trace data to touchstone file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs : SharedMemoryTransfer
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::SharedMemoryTransfer_Initialise(int tfSite, int vnaSiteIndex)
	{
		int ret = 0;

		try
		{
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Initial());
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_INIT_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to initialize Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to initialize Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;

	}
	int AMB7300TestLibrary::SharedMemoryTransfer_FetchData(int tfSite, int vnaSiteIndex)
	{
		int ret = 0;
		int Trace; //Trace used in Add Api
		int Trace_Count = vnaSetting[vnaSiteIndex].VNA_TraceCount;
		int DataFormat;
	    int Offset = 0; //int16?
	//	System::Int32 Offset = 0;
		int size;
		int l_point = SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1]->Length;
		String^ command;
		String^ Val;
		String^ tempNum;
		array<float> ^offsets_for_formatted_data = gcnew array<float>(Trace_Count) {};
		array<float> ^offsets_for_complex_data = gcnew array<float>(Trace_Count) {};
		SharedMemFormatData->formattedData = gcnew array<array<float>^>(vnaSetting[vnaSiteIndex].VNA_TraceCount);
		SharedMemFormatData->complexData = gcnew array<array<float>^>(vnaSetting[vnaSiteIndex].VNA_TraceCount * 2);

		try
		{
			//cast the First Trace to Int
			tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
			Trace = Convert::ToInt32(tempNum);
			for (int i = 0; i < Trace_Count; i++)
			{
				int TraceNum = Trace + i;

				//Configure a new section of memory map to monitor the complex data of the parameters.
				if ((vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLOGarithmic)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::SWR)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::PHASe)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::UPHase)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::GDELay)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLINear)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::REAL)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::IMAGinary)))
				{
					DataFormat = 1; //FDATA, if 0 = SDATA;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Add(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, DataFormat, l_point));// Channel, Trace, data format, numPoints

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Offset_Get(Offset));
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Size_Get(size));
					sizeof(Offset);
					offsets_for_formatted_data[i] = Offset;
				}
				else
				{
					DataFormat = 0; //FDATA, if 0 = SDATA;
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Add(vnaSetting[vnaSiteIndex].currentVnaFetchChannel, TraceNum, DataFormat, l_point));// Channel, Trace, data format, numPoints

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Offset_Get(Offset));
					
					offsets_for_complex_data[i] = Offset;
				}
			}
			//Allocate memory map
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Commit_Set("VNA_MemoryMap" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

			/*// Query the size of the memory map
			String^ command = String::Format("SYST:DATA:MEM:SIZE?\n");
			String^ Val;
			amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
			amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
			size = int::Parse(Val);*/

			//Query for size of the memory map
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Size_Get(size));

			//Create the memory map in C#, This requires .NET 4.5 framework
			MemoryMappedFile ^ mappedFile = MemoryMappedFile::CreateOrOpen("VNA_MemoryMap" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel, size);
			MemoryMappedViewAccessor ^ mappedFileView = mappedFile->CreateViewAccessor();

			command = ("TRIGger:SEQuence:SOURce MAN\n");
			amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

			amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel);

			command = ("SENS" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() + ":SWE:MODE SING\n");
			amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

			command = String::Format("INIT" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() +"\n");  //INIT + <channel>
			
			amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
			
			do
			{
				command = String::Format("*OPC?\n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
				//Util->WaitSecond(1 mS);
				if (Val->Contains("1"))
					break;
			} while (true);

			//Allocate buffers to hold the output data
			for (int i = 0; i < SharedMemFormatData->formattedData->Length; i++)
			{
				SharedMemFormatData->formattedData[i] = gcnew array<float>(l_point);
			}

			for (int i = 0; i < SharedMemFormatData->complexData->Length; i++)
			{
				SharedMemFormatData->complexData[i] = gcnew array<float>(l_point * 2);
			}

			for (int i = 0; i < Trace_Count; i++)
			{
				//Query the formatted data
				if ((vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLOGarithmic)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::SWR)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::PHASe)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::UPHase)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::GDELay)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLINear)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::REAL)) ||
					(vnaSetting[vnaSiteIndex].vna_traceFormat[i] == int(VNATraceFormat::IMAGinary)))
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_ReadBytes(mappedFileView, offsets_for_formatted_data[i], l_point, SharedMemFormatData->formattedData[i]));
				}
				else
				{
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_ReadBytes(mappedFileView, offsets_for_complex_data[i], l_point * 2, SharedMemFormatData->complexData[i]));
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_FETCH_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_FetchData] Fail to fetch Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_FetchData] Fail to fetch Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SharedMemoryTransfer_DataAnalysis(int tfSite, int vnaSiteIndex)
	{
		int ret = 0;
		double MarkX, MarkY;
		double Bw, High, Cent, Low, Q, Loss;
		array<double^>^ Freq;

		// Initialize variable storage and control loop
		smtSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;
		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				for (int i = 0; i < smtSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);

					smtSetting[vnaSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[vnaSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 1;
					}

					//0-> Min, 1-> Max
					if (analysisSetting.SharedMemoryDataMinMax_SearchType[0] == Cond_AnalysisSetting_MS_Type_Minimum)
					{
						smtSetting[vnaSiteIndex].searchOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataMinMax_SearchType[0] == Cond_AnalysisSetting_MS_Type_Maximum)
					{
						smtSetting[vnaSiteIndex].searchOption = 1;
					}

					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, smtSetting[vnaSiteIndex].userStartFreq, smtSetting[vnaSiteIndex].userStopFreq, smtSetting[vnaSiteIndex].searchOption,
						SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1], SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], MarkX, MarkY)); //formattedData[0] -> trace 1
				
					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i] = MarkX;
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i] = MarkY;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				for (int i = 0; i < smtSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);

					smtSetting[vnaSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[vnaSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];
					smtSetting[vnaSiteIndex].TargetValue = analysisSetting.SharedMemoryDataTarget_TargetValue[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 1;
					}

					//Transition: 0 -> Postive, 1 -> Negative, 2 -> Both
					if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Positive)
					{
						smtSetting[vnaSiteIndex].Transition = 0;
					}
					else if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Negative)
					{
						smtSetting[vnaSiteIndex].Transition = 1;
					}
					else if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Both)
					{
						smtSetting[vnaSiteIndex].Transition = 2;
					}

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerTarget(smtSetting[vnaSiteIndex].searchRangeOption, 
						smtSetting[vnaSiteIndex].userStartFreq, 
						smtSetting[vnaSiteIndex].userStopFreq, 
						smtSetting[vnaSiteIndex].TargetValue,
						smtSetting[vnaSiteIndex].Transition, 
						SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1], 
						SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], 
						SharedMemFormatData->XAxis, SharedMemFormatData->YAxis));

					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i] = SharedMemFormatData->XAxis[i];
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i] = SharedMemFormatData->YAxis[i];
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch ||
				vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				for (int i = 0; i < smtSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->BW = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->High = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Cent = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Low = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Q = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);
					SharedMemFormatData->Loss = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount);

					//M1Freq
					smtSetting[vnaSiteIndex].M1Freq = vnaDataAnalysisTPC.testFreq[0]; //if marker, then take freq as input else peak then put 0;
					smtSetting[vnaSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[vnaSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];
					smtSetting[vnaSiteIndex].BwLevel = analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[vnaSiteIndex].searchRangeOption = 1;
					}

					//BWOption: 0-> BW Search, 1-> Notch Search
					if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						smtSetting[vnaSiteIndex].BwOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						smtSetting[vnaSiteIndex].BwOption = 1;
					}

					//SearchRef: 0 -> Marker, 1 ->Peak
				//	if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)

						//~XL 20260708 UPDATE: exchange marker & peak judge
					if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak)
					{
						smtSetting[vnaSiteIndex].MarkerRefOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] ==  Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)
					{
						smtSetting[vnaSiteIndex].MarkerRefOption = 1;

						//Identify if SharedMemoryBwSearchAbs function needed or not.
					//	if (analysisSetting.SharedMemoryDataBWSearch_Condition[0] == Cond_AnalysisSetting_MMBwSearch_Abs_SearchRef_Condition) //bypass judge 
						{	smtSetting[vnaSiteIndex].searchRangeOption = 0;
							//Grab the max value using the PostProcMarkerSearchMinMax
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, smtSetting[vnaSiteIndex].userStartFreq, smtSetting[vnaSiteIndex].userStopFreq, 1,
								SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1], SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], MarkX, MarkY));

							//Get Marker Y as Max value.
							double temp;
							temp = MarkY;
							temp = smtSetting[vnaSiteIndex].BwLevel - temp;
							smtSetting[vnaSiteIndex].BwLevel = temp;
						}

					}


					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerBWSearch(smtSetting[vnaSiteIndex].searchRangeOption, smtSetting[vnaSiteIndex].userStartFreq, smtSetting[vnaSiteIndex].userStopFreq, smtSetting[vnaSiteIndex].BwOption,
						smtSetting[vnaSiteIndex].MarkerRefOption, smtSetting[vnaSiteIndex].M1Freq, smtSetting[vnaSiteIndex].BwLevel, SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1],
						SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], Bw, High, Cent, Low, Q, Loss));

					SharedMemFormatData->BW[vnaSiteIndex, i] = Bw;
					SharedMemFormatData->High[vnaSiteIndex, i] = High;
					SharedMemFormatData->Cent[vnaSiteIndex, i] = Cent;
					SharedMemFormatData->Low[vnaSiteIndex, i] = Low;
					SharedMemFormatData->Q[vnaSiteIndex, i] = Q;
					SharedMemFormatData->Loss[vnaSiteIndex, i] = Loss;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_POST_PROC_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to get the correct analysis of the Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to get the correct analysis of the Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SharedMemoryTransfer_MinMaxRipple(int tfSite, int vnaSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchMinMaxRipple_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search min and max in a same freq range.
		**		Perform (max - min) to get the ripple result.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		double MarkX, MarkY, MaxValue, MinValue;
		array<double^>^ Freq;

		// Initialize variable storage and control loop
		smtSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				for (int i = 0; i < smtSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount + 1);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount + 1);

					smtSetting[vnaSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[vnaSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					smtSetting[vnaSiteIndex].searchRangeOption = 1;

					//0-> Min, 1-> Max
					smtSetting[vnaSiteIndex].searchOption = 0;
					
					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, smtSetting[vnaSiteIndex].userStartFreq, smtSetting[vnaSiteIndex].userStopFreq, smtSetting[vnaSiteIndex].searchOption,
						SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1], SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], MarkX, MarkY)); //formattedData[0] -> trace 1

					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i] = MarkX;
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i] = MarkY;

					//0-> Min, 1-> Max
					smtSetting[vnaSiteIndex].searchOption = 1;

					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, smtSetting[vnaSiteIndex].userStartFreq, smtSetting[vnaSiteIndex].userStopFreq, smtSetting[vnaSiteIndex].searchOption,
						SharedMemFormatData->formattedData[vnaDataAnalysisTPC.traceIndex - 1], SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], MarkX, MarkY)); //formattedData[0] -> trace 1

					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i + 1] = MarkX;
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i + 1] = MarkY;
				}
			}


		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SharedMemoryTransfer_FetchData_TrueParallel(int tfSite, int vnaSiteIndex)
	{
		int ret = 0;

		int Trace; //Trace used in Add Api
		int Trace_Count;
		int l_point;
		int DataFormat;
	//	int Offset = 0;
		System::Int32 Offset = 0;
		int size;
		int counter = 0;
		String^ command;
		String^ Val;
		String^ tempNum;
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
			{
				Trace_Count = vnaSetting[siteIndex].VNA_TraceCount * tl->glob->tf.activeUUT_count;
				l_point = SharedMemFormatData->FreqPoints[vnaSetting[siteIndex].currentVnaFetchChannel - 1]->Length;
				break;
			}
		}

		SharedMemFormatData->VnaTrace_to_TraceCount_Map = gcnew array<int>(Trace_Count);
		array<float> ^offsets_for_formatted_data = gcnew array<float>(Trace_Count) {};
		array<float> ^offsets_for_complex_data = gcnew array<float>(Trace_Count) {};
		SharedMemFormatData->formattedData = gcnew array<array<float>^>(Trace_Count);
		SharedMemFormatData->complexData = gcnew array<array<float>^>(Trace_Count * 2);

		//Create the memory map in C#, This requires .NET 4.5 framework
		mappedFile = gcnew array<MemoryMappedFile^>(tl->glob->tf.activeUUT_count);
		mappedFileView = gcnew array<MemoryMappedViewAccessor^>(tl->glob->tf.activeUUT_count);

		try
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
				{
					//cast the First Trace to Int
					tempNum = SharedMemFormatData->FirstTrace[vnaSetting[siteIndex].currentVnaFetchChannel - 1];
					Trace = Convert::ToInt32(tempNum);

					for (int i = 0; i < vnaSetting[siteIndex].VNA_TraceCount; i++)
					{
						SharedMemFormatData->VnaTrace_to_TraceCount_Map[counter] = Trace + i;
						counter++;
					}
				}
			}

			counter = 0; // Reset counter

			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
				{
					for (int i = 0; i < vnaSetting[vnaSiteIndex].VNA_TraceCount; i++)
					{
						int TraceNum = SharedMemFormatData->VnaTrace_to_TraceCount_Map[counter];

						//Configure a new section of memory map to monitor the complex data of the parameters.

						if ((vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::IMAGinary)))
						{
							DataFormat = 1; //FDATA, if 0 = SDATA;
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Add(vnaSetting[siteIndex].currentVnaFetchChannel, TraceNum, DataFormat, l_point));// Channel, Trace, data format, numPoints
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Offset_Get(Offset));
							offsets_for_formatted_data[counter] = Offset;
						}
						else
						{
							DataFormat = 0; //FDATA, if 0 = SDATA;
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Add(vnaSetting[siteIndex].currentVnaFetchChannel, TraceNum, DataFormat, l_point));// Channel, Trace, data format, numPoints
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Offset_Get(Offset));
							offsets_for_complex_data[counter] = Offset;
						}
						counter++;
					}
				}
			}

			//Allocate memory map
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Commit_Set("VNA_MemoryMap" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel));

			//Query for size of the memory map
			tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_Size_Get(size));

			mappedFile[vnaSiteIndex] = MemoryMappedFile::CreateOrOpen("VNA_MemoryMap" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel, size);
			mappedFileView[vnaSiteIndex] = mappedFile[vnaSiteIndex]->CreateViewAccessor();
			
			command = ("TRIGger:SEQuence:SOURce MAN\n");
			amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

			amb7300_NA[vnaSiteIndex]->Display_ActiveChannel_Set(vnaSetting[vnaSiteIndex].currentVnaFetchChannel);
            command = ("SENS" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() + ":SWE:MODE SING\n");
            amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
            command = String::Format("INIT" + vnaSetting[vnaSiteIndex].currentVnaFetchChannel.ToString() + "\n");  //INIT + <channel>
            amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);

            //command = ("SENS1" + ":SWE:MODE SING\n");
            //amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
            ////command = String::Format("INIT1" + "\n");  //INIT + <channel>
            ////amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
            ////command = ("SENS2" + ":SWE:MODE SING\n");
            ////amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
            //command = String::Format("INIT1" + "\n");  //INIT + <channel>
            //amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
			do
			{
				command = String::Format("*OPC?\n");
				amb7300_NA[vnaSiteIndex]->System_Scpi_WriteString(command);
				amb7300_NA[vnaSiteIndex]->System_Scpi_ReadString(Val);
				//Util->WaitSecond(1 mS);
				if (Val->Contains("1"))
					break;
			} while (true);
			
			//Allocate buffers to hold the output data
			for (int i = 0; i < SharedMemFormatData->formattedData->Length; i++)
			{
				SharedMemFormatData->formattedData[i] = gcnew array<float>(l_point);
			}

			for (int i = 0; i < SharedMemFormatData->complexData->Length; i++)
			{
				SharedMemFormatData->complexData[i] = gcnew array<float>(l_point * 2);
			}

			counter = 0; // Reset counter
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
				{
					for (int i = 0; i < vnaSetting[vnaSiteIndex].VNA_TraceCount; i++)
					{
						int TraceNum = SharedMemFormatData->VnaTrace_to_TraceCount_Map[counter];

						//Query the formatted data
						if ((vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLOGarithmic)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::SWR)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::PHASe)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::UPHase)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::GDELay)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::MLINear)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::REAL)) ||
							(vnaSetting[siteIndex].vna_traceFormat[i] == int(VNATraceFormat::IMAGinary)))
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_ReadBytes(mappedFileView[vnaSiteIndex], offsets_for_formatted_data[counter], l_point, SharedMemFormatData->formattedData[counter]));
						}
						else
						{
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->System_DataSharedMemory_ReadBytes(mappedFileView[vnaSiteIndex], offsets_for_complex_data[counter], l_point * 2, SharedMemFormatData->complexData[counter]));
						}
						counter++;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_FETCH_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_FetchData] Fail to fetch Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_FetchData] Fail to fetch Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SharedMemoryTransfer_DataAnalysis_TrueParallel(int tfSite, int testSiteIndex)
	{
		int ret = 0;
		double MarkX, MarkY;
		double Bw, High, Cent, Low, Q, Loss;
		array<double^>^ Freq;

		// Map current siteIndex to respective vnaSiteIndex
		int vnaSiteIndex = 0;

		for (int i = 0; i < tl->glob->tf.NumberOfTestSites; i++)
		{
			if (i == testSiteIndex)
			{
				vnaSiteIndex = siteIndex_By_vnaSiteIndex[i];
				break;
			}
		}
		int first_active_site = 0;

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
			{
				first_active_site++;

				if (siteIndex == testSiteIndex) 
				{
					break;
				}
			}
		}

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		//		Eg:		Site0 ON Site1 ON, if traceIndex = 2, First Trace = 1 (Site0) / 5 (Site1). After calculation, get TraceNum = 2 (Site0) / 6 (Site1)
		//				Site0 ON Site1 OFF, if traceIndex = 2, First Trace = 1 (Site0). After calculation, get TraceNum = 2 (Site0)
		//				Site0 OFF Site1 ON, if traceIndex = 2, First Trace = 5 (Site1). After calculation, get TraceNum = 2 (Site0)
		TraceNum = vnaDataAnalysisTPC.traceIndex + (vnaSetting[vnaSiteIndex].VNA_TraceCount * (first_active_site - 1));

		// Initialize variable storage and control loop
		smtSetting[testSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				for (int i = 0; i < smtSetting[testSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);

					smtSetting[testSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[testSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[testSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[testSiteIndex].searchRangeOption = 1;
					}

					//0-> Min, 1-> Max
					if (analysisSetting.SharedMemoryDataMinMax_SearchType[0] == Cond_AnalysisSetting_MS_Type_Minimum)
					{
						smtSetting[testSiteIndex].searchOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataMinMax_SearchType[0] == Cond_AnalysisSetting_MS_Type_Maximum)
					{
						smtSetting[testSiteIndex].searchOption = 1;
					}

					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[testSiteIndex].searchRangeOption, 
						smtSetting[testSiteIndex].userStartFreq, 
						smtSetting[testSiteIndex].userStopFreq, 
						smtSetting[testSiteIndex].searchOption,
						SharedMemFormatData->formattedData[TraceNum - 1], 
						SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1], 
						MarkX, MarkY)); //formattedData[0] -> trace 1

					SharedMemFormatData->Result_MarkX[testSiteIndex, i] = MarkX;
					SharedMemFormatData->Result_MarkY[testSiteIndex, i] = MarkY;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				for (int i = 0; i < smtSetting[testSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);

					smtSetting[testSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[testSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];
					smtSetting[testSiteIndex].TargetValue = analysisSetting.SharedMemoryDataTarget_TargetValue[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[testSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[testSiteIndex].searchRangeOption = 1;
					}

					//Transition: 0 -> Postive, 1 -> Negative, 2 -> Both
					if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Positive)
					{
						smtSetting[testSiteIndex].Transition = 0;
					}
					else if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Negative)
					{
						smtSetting[testSiteIndex].Transition = 1;
					}
					else if (analysisSetting.SharedMemoryDataTarget_TargetTransition[0] == Cond_AnalysisSetting_MSTarget_TargetTransition_Both)
					{
						smtSetting[testSiteIndex].Transition = 2;
					}

					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerTarget(smtSetting[testSiteIndex].searchRangeOption, 
						smtSetting[testSiteIndex].userStartFreq, 
						smtSetting[testSiteIndex].userStopFreq, 
						smtSetting[testSiteIndex].TargetValue,
						smtSetting[testSiteIndex].Transition, 
						SharedMemFormatData->formattedData[TraceNum - 1], 
						SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1], 
						SharedMemFormatData->XAxis, 
						SharedMemFormatData->YAxis));

					SharedMemFormatData->Result_MarkX[testSiteIndex, i] = SharedMemFormatData->XAxis[i];
					SharedMemFormatData->Result_MarkY[testSiteIndex, i] = SharedMemFormatData->YAxis[i];
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch ||
				vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				for (int i = 0; i < smtSetting[testSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->BW = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->High = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Cent = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Low = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Q = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Loss = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);

					//M1Freq
					smtSetting[testSiteIndex].M1Freq = vnaDataAnalysisTPC.testFreq[0]; //if marker, then take freq as input else peak then put 0;
					smtSetting[testSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[testSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];
					smtSetting[testSiteIndex].BwLevel = analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[testSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[testSiteIndex].searchRangeOption = 1;
					}

					//BWOption: 0-> BW Search, 1-> Notch Search
					if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						smtSetting[testSiteIndex].BwOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						smtSetting[testSiteIndex].BwOption = 1;
					}

					//SearchRef: 0 -> Marker, 1 ->Peak
					if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)
					{
						smtSetting[testSiteIndex].MarkerRefOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak)
					{
						smtSetting[testSiteIndex].MarkerRefOption = 1;

						//Identify if SharedMemoryBwSearchAbs function needed or not.
						if (analysisSetting.SharedMemoryDataBWSearch_Condition[0] == Cond_AnalysisSetting_MMBwSearch_Abs_SearchRef_Condition)
						{
							//Grab the max value using the PostProcMarkerSearchMinMax
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[testSiteIndex].searchRangeOption, 
								smtSetting[testSiteIndex].userStartFreq, 
								smtSetting[testSiteIndex].userStopFreq, 1,
								SharedMemFormatData->formattedData[TraceNum - 1], 
								SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1], 
								MarkX, MarkY));

							//Get Marker Y as Max value.
							double temp;
							temp = MarkY;
							temp = smtSetting[testSiteIndex].BwLevel - temp;
							smtSetting[testSiteIndex].BwLevel = temp;
						}

					}


					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerBWSearch(smtSetting[testSiteIndex].searchRangeOption, 
						smtSetting[testSiteIndex].userStartFreq, 
						smtSetting[testSiteIndex].userStopFreq, 
						smtSetting[testSiteIndex].BwOption,
						smtSetting[testSiteIndex].MarkerRefOption, 
						smtSetting[testSiteIndex].M1Freq, 
						smtSetting[testSiteIndex].BwLevel, 
						SharedMemFormatData->formattedData[TraceNum - 1],
						SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1], 
						Bw, High, Cent, Low, Q, Loss));

					SharedMemFormatData->BW[testSiteIndex, i] = Bw;
					SharedMemFormatData->High[testSiteIndex, i] = High;
					SharedMemFormatData->Cent[testSiteIndex, i] = Cent;
					SharedMemFormatData->Low[testSiteIndex, i] = Low;
					SharedMemFormatData->Q[testSiteIndex, i] = Q;
					SharedMemFormatData->Loss[testSiteIndex, i] = Loss;
				}
			}
			else if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataP1dB) // Under develpoment, not support yet
			{
				for (int i = 0; i < smtSetting[testSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->BW = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->High = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Cent = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Low = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Q = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);
					SharedMemFormatData->Loss = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[testSiteIndex].targetMarkerCount);

					//M1Freq
					smtSetting[testSiteIndex].M1Freq = vnaDataAnalysisTPC.testFreq[0]; //if marker, then take freq as input else peak then put 0;
					smtSetting[testSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[testSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];
					smtSetting[testSiteIndex].BwLevel = analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff)
					{
						smtSetting[testSiteIndex].searchRangeOption = 0;
					}
					else if (analysisSetting.SharedMemorySearchRangeEnable[0] == Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn)
					{
						smtSetting[testSiteIndex].searchRangeOption = 1;
					}

					//BWOption: 0-> BW Search, 1-> Notch Search
					if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Bandpass)
					{
						smtSetting[testSiteIndex].BwOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_Type[0] == Cond_AnalysisSetting_MMBwSearch_Type_Notch)
					{
						smtSetting[testSiteIndex].BwOption = 1;
					}

					//SearchRef: 0 -> Marker, 1 ->Peak
					if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)
					{
						smtSetting[testSiteIndex].MarkerRefOption = 0;
					}
					else if (analysisSetting.SharedMemoryDataBWSearch_SearchRef[0] == Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak)
					{
						smtSetting[testSiteIndex].MarkerRefOption = 1;

						//Identify if SharedMemoryBwSearchAbs function needed or not.
						if (analysisSetting.SharedMemoryDataBWSearch_Condition[0] == Cond_AnalysisSetting_MMBwSearch_Abs_SearchRef_Condition)
						{
							//Grab the max value using the PostProcMarkerSearchMinMax
							tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[testSiteIndex].searchRangeOption,
								smtSetting[testSiteIndex].userStartFreq,
								smtSetting[testSiteIndex].userStopFreq, 1,
								SharedMemFormatData->formattedData[TraceNum - 1],
								SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1],
								MarkX, MarkY));

							//Get Marker Y as Max value.
							double temp;
							temp = MarkY;
							temp = smtSetting[testSiteIndex].BwLevel - temp;
							smtSetting[testSiteIndex].BwLevel = temp;
						}

					}


					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerBWSearch(smtSetting[testSiteIndex].searchRangeOption,
						smtSetting[testSiteIndex].userStartFreq,
						smtSetting[testSiteIndex].userStopFreq,
						smtSetting[testSiteIndex].BwOption,
						smtSetting[testSiteIndex].MarkerRefOption,
						smtSetting[testSiteIndex].M1Freq,
						smtSetting[testSiteIndex].BwLevel,
						SharedMemFormatData->formattedData[TraceNum - 1],
						SharedMemFormatData->FreqPoints[vnaSetting[testSiteIndex].currentVnaFetchChannel - 1],
						Bw, High, Cent, Low, Q, Loss));

					SharedMemFormatData->BW[testSiteIndex, i] = Bw;
					SharedMemFormatData->High[testSiteIndex, i] = High;
					SharedMemFormatData->Cent[testSiteIndex, i] = Cent;
					SharedMemFormatData->Low[testSiteIndex, i] = Low;
					SharedMemFormatData->Q[testSiteIndex, i] = Q;
					SharedMemFormatData->Loss[testSiteIndex, i] = Loss;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_SHARE_MEMORY_POST_PROC_DATA_FAIL;
			tl->WriteToTracerLogger(tfSite, testSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to get the correct analysis of the Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, testSiteIndex, ERROR, "[KeysightVnaUtility -> SharedMemoryTransfer_Initilaize] Fail to get the correct analysis of the Shared Memory Data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::SharedMemoryTransfer_MinMaxRipple_TrueParallel(int tfSite, int testSiteIndex)
	{
		/*****************************************************************************************************
		**	GetMarkerSearchMinMaxRipple_Keysight
		**		tfSite			- This is techFlow site index.
		**		vnaSiteIndex	- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute marker search min and max in a same freq range.
		**		Perform (max - min) to get the ripple result.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		double MarkX, MarkY, MaxValue, MinValue;
		array<double^>^ Freq;

		// Map current siteIndex to respective vnaSiteIndex
		int vnaSiteIndex = 0;

		for (int i = 0; i < tl->glob->tf.NumberOfTestSites; i++)
		{
			if (i == testSiteIndex)
			{
				vnaSiteIndex = siteIndex_By_vnaSiteIndex[i];
				break;
			}
		}
		int first_active_site = 0;

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (tl->glob->tf.arr_activeUUT[siteIndex] == true)
			{
				first_active_site++;

				if (siteIndex == testSiteIndex)
				{
					break;
				}
			}
		}

		//cast the first trace to int
		String^ tempNum = SharedMemFormatData->FirstTrace[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1];
		int TraceNum = Convert::ToInt32(tempNum);

		//Add traceIndex to first trace //Use TraceNum to search data in APIs.
		//		Eg:		Site0 ON Site1 ON, if traceIndex = 2, First Trace = 1 (Site0) / 5 (Site1). After calculation, get TraceNum = 2 (Site0) / 6 (Site1)
		//				Site0 ON Site1 OFF, if traceIndex = 2, First Trace = 1 (Site0). After calculation, get TraceNum = 2 (Site0)
		//				Site0 OFF Site1 ON, if traceIndex = 2, First Trace = 5 (Site1). After calculation, get TraceNum = 2 (Site0)
		TraceNum = vnaDataAnalysisTPC.traceIndex + (vnaSetting[vnaSiteIndex].VNA_TraceCount * (first_active_site - 1));

		// Initialize variable storage and control loop
		smtSetting[vnaSiteIndex].targetMarkerCount = vnaDataAnalysisTPC.overallTestFreqCount;

		try
		{
			if (vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				for (int i = 0; i < smtSetting[vnaSiteIndex].targetMarkerCount; i++)
				{
					//Result Variables
					SharedMemFormatData->Result_MarkX = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount + 1);
					SharedMemFormatData->Result_MarkY = gcnew array<double, 2>(tl->glob->tf.NumberOfTestSites, smtSetting[vnaSiteIndex].targetMarkerCount + 1);

					smtSetting[vnaSiteIndex].userStartFreq = vnaDataAnalysisTPC.testFreqRangeStart[0];
					smtSetting[vnaSiteIndex].userStopFreq = vnaDataAnalysisTPC.testFreqRangeStop[0];

					//SearchRangeOn: 0 -> Full Span, 1 -> User Define Span
					smtSetting[vnaSiteIndex].searchRangeOption = 1;

					//0-> Min, 1-> Max
					smtSetting[vnaSiteIndex].searchOption = 0;

					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, 
						smtSetting[vnaSiteIndex].userStartFreq, 
						smtSetting[vnaSiteIndex].userStopFreq, 
						smtSetting[vnaSiteIndex].searchOption,
						SharedMemFormatData->formattedData[TraceNum - 1], 
						SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], 
						MarkX, MarkY)); //formattedData[0] -> trace 1

					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i] = MarkX;
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i] = MarkY;

					//0-> Min, 1-> Max
					smtSetting[vnaSiteIndex].searchOption = 1;

					//Used the formattedData to post proc, if need imag,need to make sure trace format in imag
					tl->CheckError(tfSite, amb7300_NA[vnaSiteIndex]->PostProcMarkerSearchMinMax(smtSetting[vnaSiteIndex].searchRangeOption, 
						smtSetting[vnaSiteIndex].userStartFreq, 
						smtSetting[vnaSiteIndex].userStopFreq, 
						smtSetting[vnaSiteIndex].searchOption,
						SharedMemFormatData->formattedData[TraceNum - 1], 
						SharedMemFormatData->FreqPoints[vnaSetting[vnaSiteIndex].currentVnaFetchChannel - 1], 
						MarkX, MarkY)); //formattedData[0] -> trace 1

					SharedMemFormatData->Result_MarkX[vnaSiteIndex, i + 1] = MarkX;
					SharedMemFormatData->Result_MarkY[vnaSiteIndex, i + 1] = MarkY;
				}
			}


		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_GET_MARKER_SEARCH_MIN_MAX_RIPPLE_FAIL;
			tl->WriteToTracerLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, vnaSiteIndex, ERROR, "[KeysightVnaUtility -> GetMarkerSearchMinMaxRipple_Keysight] Fail to get marker search min max ripple data." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
}


/*----------------------------------------------------------------------
* Revision Log
* &Log: KeysightVnaUtility.cpp.rca&

*** Version	: v1.0.0.5
*** Date	: 2 September 2026
*** PIC		: Xuli
* Updated ConfigureSegmentSetting_Keysight, GetChannelandTraceSetting_Keysight & ConfigureTraces_Keysight
* Updated Cond_Function_SharedMemoryDataBWSearchAbs for exchange marker & peak judge

*** Version	: v1.0.0.4
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Separated from SourceLowVna_Keysight
* Add support for HighPwrTest in ConfigureSegmentSetting_Keysight()
* Modified function name from SharedMemoryTransfer_GetFreqPoint() to GetChannelandTraceSetting_Keysight()
* Support configure trace for A & B Meas Format
* Improve on SharedMemoryTransfer_FetchData_TrueParallel()
* Add SetPortMatchingCircuit_Keysight()

*** Version	: v1.0.0.3
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Added ConfigureChannelCoupling_Keysight()
* Added SharedMemoryTransfer_GetFreqPoints()
* Updated ConfigureSegmentSetting_Keysight() logic, do when setting has changed

*** Version	: v1.0.0.2
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Added SMT_TrueParallel APIs to support True Parallel Multi UUT project (KeysightVNA)
* ConfigureTraces_Keysight() - Modified for shorter overhead using SMT test
* GetTraceFormatData_Keysight() - Replace amb7300SystemSetting->vnaSystemCount with tl->glob->tf.NumberOfTestSites
* GetMarkerSearchFormatData_Keysight() - Replace amb7300SystemSetting->vnaSystemCount with tl->glob->tf.NumberOfTestSites
* GetMarkerMathFormatData_Keysight() - Replace amb7300SystemSetting->vnaSystemCount with tl->glob->tf.NumberOfTestSites
* SharedMemoryTransfer_FetchData() - Added if condition during SDATA/FDATA offset get for shorter overhead

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* Able to add trace if tp detects the trace in VNA is not equal to trace count in tf3.

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support all Keysight VNA configuration and testing functions.
* Support project UUT:
- Single Site Single UUT
- Single Site Multi UUT
- True Parallel Single UUT
* Support VNA:
- Keysight M9804A
* Support platform execution phase:
- VnaConfig
- VnaFetch
- VnaDataAnalysis
----------------------------------------------------------------------*/
