/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:				DM400Utility.cpp
Purpose:			Contain DM400x related utility functions
Supported Module:	DM481e, DM482e, DM483e
Version:			v1.0.0.1
----------------------------------------------------------------------*/


#include "AMB7300.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: DM Settings
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::InitializeDm(String ^ hardwareProfile, int testHead, int tfSite, int siteIndex, bool offline)
	{
		/*****************************************************************************************************
		**	InitializeDm
		**		hardwareProfile		- This is project AMAP.
		**		testHead			- This is techFlow head number.
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc object index, normally start from 0.
		**		offline				- techFlow run in online or offline mode.
		**
		**	Descriptions:
		**		This is a function to initialize DM resource.
		**		Preset all the existing DM pin inside resource planner.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Able to configure dpin group and optional bit setting, for now fix to dpingroup1 and use standard init option 0xf.
			dmPeModeSetting.dpinGroup = DM_CONST_DPIN_GROUP_1;
			
			dmPeModeSetting.mipiFreq			= 1.0 MHz;	// Default initial value
			dmPeModeSetting.vih					= 1.8 V;	// Default initial value
			dmPeModeSetting.vil					= 0.0 V;	// Default initial value
			dmPeModeSetting.voh					= 0.8 V;	// Default initial value
			dmPeModeSetting.vol					= 0.8 V;	// Default initial value
			dmPeModeSetting.ioh					= 0.0 V;	// Default initial value
			dmPeModeSetting.iol					= 0.0 V;	// Default initial value
			dmPeModeSetting.vch					= 0.0 V;	// Default initial value
			dmPeModeSetting.vcl					= 0.0 V;	// Default initial value
			dmPeModeSetting.vth					= 0.9 V;	// Default initial value
			dmPeModeSetting.inputChannelDelay	= 0.0;		// Default initial value	

			// Initialize
			dm[siteIndex] = gcnew DM(hardwareProfile, dmPeModeSetting.dpinGroup, testHead, siteIndex, offline, 0xf);

			// Configure pin with default setting
			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				tl->CheckError(tfSite, dm[siteIndex]->Force(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], DM_CONST_FORCE_STATE_PMU));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePEAttribute(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_DISABLE, DM_CONST_PEATTRIBUTE_HV_DISABLE, DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE, DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUOutputFunction(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], DM_CONST_OUTPUT_FUNCTION_DVCI));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSamplingTime(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], 0.01, DM_CONST_SAMPLING_TIME_UNITS_PLC));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSense(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], DM_CONST_SENSE_LOCAL));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLimitRange(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], 25.0 mA));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLevel(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], 0.0 V));
				tl->CheckError(tfSite, dm[siteIndex]->DPINOn(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i]));
			}
			tl->Util->WaitSecond(10.0 mS);

			// [Future Enchancement] Able to configure which dm module to load vector, for now will load vector to all available DM482e/483e module.

			for (int i = 0; i < tl->glob->RsrcManager.dmModuleCount; i++)
			{
				if ((tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i]->Contains(ModuleName_DM482e)) ||
					(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i]->Contains(ModuleName_DM483e)))
				{
					// Initialize vetor state file & vector file, load vector file to engine
					if (tl->glob->VectorFile.totalVecFileExist != 0)
					{
						ret = DmLoadVectorFile(tfSite, siteIndex, tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i]);
						if (ret != 0) goto EndOfTest;
					}
			
					// [Future Enchancement] Able to configure multiple vector timing set, for now only support 1x vector timing set.
					
					// Configure vector timing set
					double dpinPeriod = 0.5 * (1.0 / dmPeModeSetting.mipiFreq);
					tl->CheckError(tfSite, dm[siteIndex]->DPINPeriod(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i], 0, dpinPeriod));
					// Configure vector engine attribute
					tl->CheckError(tfSite, dm[siteIndex]->ConfigureVectorEngineAttribute(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i], DM_CONST_VECTORENGINEATTRIBUTE_TRIGGER_DISABLE, DM_CONST_VECTORENGINEATTRIBUTE_CONTINUOUS_DISABLE));
					// Configure DM output trigger
					tl->CheckError(tfSite, dm[siteIndex]->ConfigureOutputTriggerSelect(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i], DM_CONST_TRIGGER_EXT_TRIGOUT0, DM_CONST_TRIGGER_EXT_TRIGOUT1));
				}
				else if (tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i]->Contains(ModuleName_DM481e))
				{
					// [Future Enchancement] Reserved for DM481e module.
				}
			}
			tl->Util->WaitSecond(10.0 mS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_INIT_DM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> DM400Utility -> InitializeDm] Fail to initialize DM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Load -> InitializeTester -> DM400Utility -> InitializeDm] Fail to initialize DM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::UninitializeDm(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	UninitializeDm
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to uninitialize DM resource.
		**		Off all the existing DM pin inside resource planner, and close session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Off dm pin
			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				// Configure pin to pmu mode
				ret = DmConfigurePinToPmuMode(tfSite, siteIndex,	tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i],
																	Cond_ControlFunction_DM_PMU_DVCI, 
																	DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_DISABLE,
																	DM_CONST_PEATTRIBUTE_HV_DISABLE,
																	DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE,
																	DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE,
																	0.0 V,
																	25.0 mA, 
																	0.01);
				if (ret != 0) goto EndOfTest;

				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLevel(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], 0.0 V));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLimitRange(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i], 25.0 mA));
				tl->Util->WaitSecond(10.0 mS);
				tl->CheckError(tfSite, dm[siteIndex]->DPINOff(tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i]));
				tl->Util->WaitSecond(10.0 mS);
			}

			// Off dm module
			for (int i = 0; i < tl->glob->RsrcManager.dmModuleCount; i++)
			{
				tl->CheckError(tfSite, dm[siteIndex]->Close(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndex, i]));
				tl->Util->WaitSecond(10.0 mS);
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UNINIT_DM_HARDWARE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> DM400Utility -> UninitializeDm] Fail to uninitialize DM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[Unload -> UninitializeTester -> DM400Utility -> UninitializeDm] Fail to uninitialize DM series hardware." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigurePinDm(int tfSite, int siteIndex, String ^ pinAlias, String ^ controlFunction, double nplc, double voltageLevel, double currentLevel, double delay)
	{
		/*****************************************************************************************************
		**	ConfigurePinDm
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		pinAlias			- This is target pin alias to perform the configuration.
		**		controlFunction		- This is DM output function mode.
		**		nplc				- This is nplc setting for the target pin.
		**		voltageLevel		- This is voltage level for the target pin (applicable for drive & clamp & PE mode).
		**		currentLevel		- This is current level for the target pin (applicable for drive & clamp).
		**		delay				- This is delay time after the pin configuration and pin execution.
		**
		**	Descriptions:
		**		This is a function to configure DM resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Get active module alias
			String ^ activeDmModuleAlias = String::Empty;
			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				for (int j = 0; j < tl->glob->RsrcManager.dmModuleCount; j++)
				{
					if (pinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndex, i])
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

			// Select mode
			if ((controlFunction == Cond_ControlFunction_DM_PMU_DVCI) ||
				(controlFunction == Cond_ControlFunction_DM_PMU_DICV))
			{
				// Configure pin to PMU mode
				ret = DmConfigurePinToPmuMode(tfSite, siteIndex,	pinAlias,
																	controlFunction,
																	DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_DISABLE,
																	DM_CONST_PEATTRIBUTE_HV_DISABLE,
																	DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE,
																	DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE,
																	voltageLevel,
																	currentLevel,
																	nplc);
				if (ret != 0) goto EndOfTest;
			}
			else if (controlFunction == Cond_ControlFunction_DM_PE)
			{
				// Configure pin to PE mode
				double vih	= voltageLevel;
				double vil	= 0.0 V;
				double voh	= (voltageLevel / 2.0) - 0.1 V;
				double vol	= (voltageLevel / 2.0) - 0.1 V;
				double ioh	= 0.0 A;
				double iol	= 0.0 A;
				double vch	= 0.0 V;
				double vcl	= 0.0 V;
				double vth	= voltageLevel / 2.0;
				ret = DmConfigurePinToPeMode(tfSite, siteIndex, activeDmModuleAlias, 
																pinAlias, 
																DM_CONST_PEATTRIBUTE_INPUT_TERMINATION_DISABLE, 
																DM_CONST_PEATTRIBUTE_HV_DISABLE, 
																DM_CONST_PEATTRIBUTE_ACTIVE_LOAD_DISABLE,
																DM_CONST_PEATTRIBUTE_DIFFERENTIAL_COMPARATOR_DISABLE,
																DM_CONST_VECTORENGINEATTRIBUTE_TRIGGER_DISABLE,
																DM_CONST_VECTORENGINEATTRIBUTE_CONTINUOUS_DISABLE,	
																vih, vil, voh, vol, ioh, iol, vch, vcl, vth);
				if (ret != 0) goto EndOfTest;

				//// Record latest vih
				//dmPeModeSetting.vih = DcControlCSC.voltageLevel[configurationSetsIndex]; 
			}
			else if (controlFunction == Cond_ControlFunction_DM_DIO)
			{
				// [Future Enchancement] Configure DM pin to DIO mode.
			
				// Configure pin to DIO mode
				ret = DmConfigurePinToDioMode(tfSite, siteIndex, pinAlias);
				if (ret != 0) goto EndOfTest;
			}
			
			// Delay
			tl->Util->WaitSecond(delay);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_PIN_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigurePinDm] Fail to configure DM pin." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigurePinDm] Fail to configure DM pin." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureDmVectorEngine_Mipi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, double mipiFreq, double inputChannelDelay, bool isEnableTrigger, bool isEnableContinuous, int triggerOutput0, int triggerOutput1)
	{
		/*****************************************************************************************************
		**	ConfigureDmVectorEngine_Mipi
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias		- This is DM module alias.
		**		sclkAlias			- This is MIPI sclk pin alias.
		**		sdataAlias			- This is MIPI sdata pin alias.
		**		mipiFreq			- This is MIPI frequency to run vector, aka mipi speed.
		**		inputChannelDelay	- This is input channel delay apply to DM module, only for MIPI SDATA channel.
		**		isEnableTrigger		- This is to enable/disable start to drive vector when external trigger is received (vector engine attribute).
		**		isEnableContinuous	- This is to enable/disable repeat the vector until reset is called (vector engine attribute).
		**		triggerOutput0		- This is to specifies the trigger type for DM output trigger0.
		**		triggerOutput1		- This is to specifies the trigger type for DM output trigger1.
		**
		**	Descriptions:
		**		This is a function to configure DM vector engine after DM has been configured to PE mode.
		**		For MIPI protocol.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Able to configure multiple vector timing set, for now only support 1x vector timing set.
			
			// Configure vector file timing set
			double dpinPeriod = 0.5 * (1.0 / mipiFreq);
			tl->CheckError(tfSite, dm[siteIndex]->DPINPeriod(dmModuleAlias, 0, dpinPeriod));
			
			// Configure input channel delay for mipi SDATA channel only
			double dmChannelInputDelaySec = (-1.0) * (inputChannelDelay) * (1.0 / mipiFreq);
			tl->CheckError(tfSite, dm[siteIndex]->ConfigureInputChannelDelay(sdataAlias, dmChannelInputDelaySec));

			// Configure vector engine attribute
			tl->CheckError(tfSite, dm[siteIndex]->ConfigureVectorEngineAttribute(dmModuleAlias, isEnableTrigger, isEnableContinuous));
			
			// Configure DM output trigger
			tl->CheckError(tfSite, dm[siteIndex]->ConfigureOutputTriggerSelect(dmModuleAlias, triggerOutput0, triggerOutput1));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_VECTOR_ENGINE_MIPI_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_Mipi] Fail to configure DM vector engine for MIPI test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_Mipi] Fail to configure DM vector engine for MIPI test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureDmVectorEngine_Spi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias)
	{
		/*****************************************************************************************************
		**	ConfigureDmVectorEngine_Spi
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias		- This is DM module alias.
		**		sclkAlias			- This is SPI clk pin alias.
		**		sdataAlias			- This is SPI data pin alias.
		**
		**	Descriptions:
		**		This is a function to configure DM vector engine after DM has been configured to PE mode.
		**		For SPI protocol.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for dm configuration for spi protocol.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_VECTOR_ENGINE_SPI_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_Spi] Fail to configure DM vector engine for SPI test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_Spi] Fail to configure DM vector engine for SPI test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::ConfigureDmVectorEngine_I2c(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias)
	{
		/*****************************************************************************************************
		**	ConfigureDmVectorEngine_I2c
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias		- This is DM module alias.
		**		sclkAlias			- This is I2C clk pin alias.
		**		sdataAlias			- This is I2C data pin alias.
		**
		**	Descriptions:
		**		This is a function to configure DM vector engine after DM has been configured to PE mode.
		**		For I2C protocol.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for dm configuration for i2c protocol.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_VECTOR_ENGINE_I2C_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_I2c] Fail to configure DM vector engine for I2C test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> ConfigureDmVectorEngine_I2c] Fail to configure DM vector engine for I2C test mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmConfigurePinToPeMode(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ dmPinAlias, bool isEnableInputTermination, bool isEnableHv, bool isEnableActiveLoad, bool isEnableDiffComparator, bool isEnableTrigger, bool isEnableContinuous, double vih, double vil, double voh, double vol, double ioh, double iol, double vch, double vcl, double vth)
	{
		/*****************************************************************************************************
		**	DmConfigurePinToPeMode
		**		tfSite						- This is techFlow site index.
		**		siteIndex					- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias				- This is module alias of the target pin to perform the configuration.
		**		dmPinAlias					- This is target pin alias to perform the configuration.
		**		isEnableInputTermination	- This is to enable/disable input termination (PE attribute).
		**		isEnableHv					- This is to enable/disable high voltage (PE attribute). 
		**		isEnableActiveLoad			- This is to enable/disable active load (PE attribute).
		**		isEnableDiffComparator		- This is to enable/disable differnential comparator (PE attribute).
		**		isEnableTrigger				- This is to enable/disable start to drive vector when external trigger is received (vector engine attribute).
		**		isEnableContinuous			- This is to enable/disable repeat the vector until reset is called (vector engine attribute).
		**		vih							- This is output driver high voltage level from the target pin.
		**		vil							- This is output driver low voltage level from the target pin.
		**		voh							- This is input comparator high voltage level into the target pin.
		**		vol							- This is input comparator low voltage level into the target pin.
		**		ioh							- This is sink current when active is enabled.
		**		iol							- This is source current when active is enabled.
		**		vch							- This is reflection voltage clamp level for high range.
		**		vcl							- This is reflection voltage clamp level for low range.
		**		vth							- This is termination voltage when input termination is enabled.
		**									- If input termination is disabled, driver is in high impedance.
		**
		**	Descriptions:
		**		This is a function to configure DM pin to PE mode.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			//// [Future Enchancement] Able to configure all the voltage setting.
			//dmPeModeSetting.vih		= vih;					// Get in DcControl condition 'VoltageLevel'
			////dmPeModeSetting.vil	= 0.0 V;				// Fix for now
			//dmPeModeSetting.voh		= (vih / 2.0) - 0.1 V;	// Comparator voltage, half of vih
			//dmPeModeSetting.vol		= (vih / 2.0) - 0.1 V;	// Comparator voltage, half of vih
			////dmPeModeSetting.ioh	= 0.0 V;				// Fix for now
			////dmPeModeSetting.iol	= 0.0 V;				// Fix for now
			////dmPeModeSetting.vch	= 0.0 V;				// Fix for now
			////dmPeModeSetting.vcl	= 0.0 V;				// Fix for now
			//dmPeModeSetting.vth		= vth;					// Get in PatternControl condition 'DmTerminationVoltage', and will re-configure again later in PatternControl

			// dpin level setting
			dmPeModeSetting.vih		= vih;	
			dmPeModeSetting.vil		= vil;	
			dmPeModeSetting.voh		= voh;	
			dmPeModeSetting.vol		= vol;	
			dmPeModeSetting.ioh		= ioh;	
			dmPeModeSetting.iol		= iol;	
			dmPeModeSetting.vch		= vch;	
			dmPeModeSetting.vcl		= vcl;	
			dmPeModeSetting.vth		= vth;	

			// Configure force
			tl->CheckError(tfSite, dm[siteIndex]->Force(dmPinAlias, DM_CONST_FORCE_STATE_VECTOR));
			// Configure dpin level
			tl->CheckError(tfSite, dm[siteIndex]->DPINLevel(dmPinAlias, dmPeModeSetting.vih, dmPeModeSetting.vil,
																		dmPeModeSetting.voh, dmPeModeSetting.vol,
																		dmPeModeSetting.ioh, dmPeModeSetting.iol,
																		dmPeModeSetting.vch, dmPeModeSetting.vcl,
																		dmPeModeSetting.vth));
			// Configure PE attribute
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePEAttribute(dmPinAlias, isEnableInputTermination, isEnableHv, isEnableActiveLoad, isEnableDiffComparator));
			// Configure vector engine attribute
			tl->CheckError(tfSite, dm[siteIndex]->ConfigureVectorEngineAttribute(dmModuleAlias, isEnableTrigger, isEnableContinuous));
			// On pin
			tl->CheckError(tfSite, dm[siteIndex]->DPINOn(dmPinAlias));
			tl->Util->WaitSecond(500.0 uS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_PIN_TO_PE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToPeMode] Fail to configure DM pin to PE mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToPeMode] Fail to configure DM pin to PE mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmConfigurePinToPmuMode(int tfSite, int siteIndex, String ^ dmPinAlias, String ^ forceMode, bool isEnableInputTermination, bool isEnableHv, bool isEnableActiveLoad, bool isEnableDiffComparator , double voltageLevel, double currentLevel, double nplc)
	{
		/*****************************************************************************************************
		**	DmConfigurePinToPmuMode
		**		tfSite						- This is techFlow site index.
		**		siteIndex					- This is dc resource's object index, normally start from 0.
		**		dmPinAlias					- This is target pin alias to perform the configuration.
		**		forceMode					- This is DM force mode (output function).
		**		isEnableInputTermination	- This is to enable/disable input termination (PE attribute).
		**		isEnableHv					- This is to enable/disable high voltage (PE attribute).
		**		isEnableActiveLoad			- This is to enable/disable active load (PE attribute).
		**		isEnableDiffComparator		- This is to enable/disable differnential comparator (PE attribute).
		**		voltageLevel				- This is voltage level for the target pin (applicable for drive & clamp).
		**		currentLevel				- This is current level for the target pin (applicable for drive & clamp).
		**		nplc						- This is nplc setting for the target pin.
		**
		**	Descriptions:
		**		This is a function to configure DM pin to PMU mode.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Configure force
			tl->CheckError(tfSite, dm[siteIndex]->Force(dmPinAlias, DM_CONST_FORCE_STATE_PMU));
			// Configure PE attribute
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePEAttribute(dmPinAlias, isEnableInputTermination, isEnableHv, isEnableActiveLoad, isEnableDiffComparator));

			// Configure output function, current level, voltage level
			if (forceMode == Cond_ControlFunction_DM_PMU_DVCI)
			{
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUOutputFunction(dmPinAlias, DM_CONST_OUTPUT_FUNCTION_DVCI));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLimitRange(dmPinAlias, currentLevel));
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLevel(dmPinAlias, voltageLevel));
			}
			else if (forceMode == Cond_ControlFunction_DM_PMU_DICV)
			{
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUOutputFunction(dmPinAlias, DM_CONST_OUTPUT_FUNCTION_DICV));
				double voltageLevelSet = 0.0;
				if (voltageLevel >= 0.0 V)
				{
					voltageLevelSet = (-1) * (voltageLevel);
					tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLimit(dmPinAlias, voltageLevel, voltageLevelSet));
				}
				else if (voltageLevel < 0.0 V)
				{
					voltageLevelSet = (-1) * (voltageLevel);
					tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLimit(dmPinAlias, voltageLevelSet, voltageLevel));
				}
				tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLevel(dmPinAlias, currentLevel));
			}

			// Configure nplc
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSamplingTime(dmPinAlias, nplc, DM_CONST_SAMPLING_TIME_UNITS_PLC));
			// Configure sense
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSense(dmPinAlias, DM_CONST_SENSE_LOCAL));
			// On pin
			tl->CheckError(tfSite, dm[siteIndex]->DPINOn(dmPinAlias));
			tl->Util->WaitSecond(500.0 uS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_PIN_TO_PMU_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToPmuMode] Fail to configure DM pin to PMU mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToPmuMode] Fail to configure DM pin to PMU mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmConfigurePinToDioMode(int tfSite, int siteIndex, String ^ dmPinAlias)
	{
		/*****************************************************************************************************
		**	DmConfigurePinToDioMode
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		dmPinAlias		- This is target pin alias to perform the configuration.
		**
		**	Descriptions:
		**		This is a function to configure DM pin to DIO mode.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Configure DM pin to DIO mode.
			tl->Util->WaitSecond(500.0 uS);
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_CONFIGURE_DM_PIN_TO_DIO_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToDioMode] Fail to configure DM pin to DIO mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmConfigurePinToDioMode] Fail to configure DM pin to DIO mode." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}



	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: DM Test Function (DcTest)
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::DmTestFunction_OS(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	DmTestFunction_OS
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute OS test function for DM resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			//tl->CheckError(tfSite, dm[siteIndex]->Force(pinAlias, DM_CONST_FORCE_STATE_PMU));
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSamplingTime(pinAlias, nplc, DM_CONST_SAMPLING_TIME_UNITS_PLC));
			//tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUOutputFunction(pinAlias, DM_CONST_OUTPUT_FUNCTION_DICV));
			//tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLimit(pinAlias, voltageLevelHi, voltageLevelLo));
			//tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLevel(pinAlias, currentLevel));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, dm[siteIndex]->PMUMeasure(pinAlias, DM_CONST_PMU_MEASURE_VOLTAGE, result));
			
			// Reconfigure to DVCI and drive low to cater for pin-to-pin short testing
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUOutputFunction(pinAlias, DM_CONST_OUTPUT_FUNCTION_DVCI));
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUCurrentLimitRange(pinAlias, 25.0 mA));
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUVoltageLevel(pinAlias, 0.0 V));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_TEST_FUNCTION_OS_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_OS] Fail at OS test." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_OS] Fail at OS test." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmTestFunction_MeasureCurrent(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	DmTestFunction_MeasureCurrent
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute current measurement test function for DM resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSamplingTime(pinAlias, nplc, DM_CONST_SAMPLING_TIME_UNITS_PLC));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, dm[siteIndex]->PMUMeasure(pinAlias, DM_CONST_PMU_MEASURE_CURRENT, result));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_TEST_FUNCTION_MEASURE_CURRENT_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_MeasureCurrent] Fail to measure current." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_MeasureCurrent] Fail to measure current." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmTestFunction_MeasureVoltage(int tfSite, int siteIndex, String ^ pinAlias, double nplc, double measureDelay, double % result)
	{
		/*****************************************************************************************************
		**	DmTestFunction_MeasureVoltage
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		pinAlias		- This is target pin alias to perform the testing.
		**		nplc			- This is nplc setting for the target pin.
		**		measureDelay	- This is delay time before execute the measurement at the target pin.
		**		result			- Return test result.
		**
		**	Descriptions:
		**		This is a function to execute voltage measurement test function for DM resource pin.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			tl->CheckError(tfSite, dm[siteIndex]->ConfigurePMUSamplingTime(pinAlias, nplc, DM_CONST_SAMPLING_TIME_UNITS_PLC));
			tl->Util->WaitSecond(measureDelay);
			tl->CheckError(tfSite, dm[siteIndex]->PMUMeasure(pinAlias, DM_CONST_PMU_MEASURE_VOLTAGE, result));
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_TEST_FUNCTION_MEASURE_VOLTAGE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_MeasureVoltage] Fail to measure voltage." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmTestFunction_MeasureVoltage] Fail to measure voltage." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}



	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: DM Test Function (PatternTest)
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::DmPatternTestFunction_MipiWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_MipiWrite
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias	- This is DM module alias.
		**		vectorFileName	- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for MIPI protocol.
		**		Execute MIPI wirte, which is run vector without any other function.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// Run each vector file
			for (int i = 0; i < vectorFileName->Length; i++)
			{
				ret = DmRunVector(tfSite, siteIndex, dmModuleAlias, vectorFileName[i]);
				if (ret != 0) goto EndOfTest;
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_MIPI_WRITE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_MipiWrite] Fail to perform MIPI write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_MipiWrite] Fail to perform MIPI write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmPatternTestFunction_MipiRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName, bool isMultiVecToOneResult, bool isOneVecToMultiResult)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_MipiRead
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias			- This is DM module alias.
		**		sclkAlias				- This is I2C clk pin alias.
		**		sdataAlias				- This is I2C data pin alias.
		**		vectorFileName			- This is vector file name to execute.
		**		isMultiVecToOneResult	- This is to specifies the read back mode for the MIPI test library usage.
		**		isOneVecToMultiResult	- This is to specifies the read back mode for the MIPI test library usage.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for MIPI protocol.
		**		Execute MIPI read, including ReadHistoryRam or ReadFailCount.
		******************************************************************************************************/

		// Local variable
		int ret					= 0;
		bool isReadHistoryRam	= false;
		bool isReadFailCount	= false;

		try
		{
			// Test mode select
			if (tl->glob->AWV.Debug)
			{
				//If debug mode, check both history ram and fail count
				isReadHistoryRam	= true;
				isReadFailCount		= true;
			}
			else
			{
				//If non-debug mode, only check fail count to reduce test time
				isReadHistoryRam	= false;
				isReadFailCount		= true;
			}

			//Situation: run one vector file and return multiple result
			if ((isMultiVecToOneResult == false) && (isOneVecToMultiResult == true))
			{
				// Result logger - Reset variable
				PatternControlGlobalResultVariable.resMsg_VecFileName		= gcnew array<String^>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_VecFailCount		= gcnew array<int>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_VecFirstFail		= gcnew array<int>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_HistoryRam		= gcnew array<array<int>^>(vectorFileName->Length);

				ret = DmVectorFileReadBack_Mipi(tfSite, siteIndex, dmModuleAlias, sclkAlias, sdataAlias, vectorFileName[0], isReadHistoryRam, isReadFailCount, PatternControlGlobalResultVariable.historyRam, PatternControlGlobalResultVariable.vecFailCount, PatternControlGlobalResultVariable.vecFirstFail);
				if (ret != 0) goto EndOfTest;

				// Result logger - Record result
				PatternControlGlobalResultVariable.resMsg_VecFileName[0]	= vectorFileName[0];
				PatternControlGlobalResultVariable.resMsg_VecFailCount[0]	= PatternControlGlobalResultVariable.vecFailCount;
				PatternControlGlobalResultVariable.resMsg_VecFirstFail[0]	= PatternControlGlobalResultVariable.vecFirstFail;
				PatternControlGlobalResultVariable.resMsg_HistoryRam[0]		= PatternControlGlobalResultVariable.historyRam;
			}

			//Situation: run multiple vector file and return one result
			if ((isMultiVecToOneResult == true) && (isOneVecToMultiResult == false))
			{
				// Result logger - Reset variable
				PatternControlGlobalResultVariable.resMsg_VecFileName		= gcnew array<String^>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_VecFailCount		= gcnew array<int>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_VecFirstFail		= gcnew array<int>(vectorFileName->Length);
				PatternControlGlobalResultVariable.resMsg_HistoryRam		= gcnew array<array<int>^>(vectorFileName->Length);

				//Run all the vector files if multiple vector file exist
				for (int i = 0; i < vectorFileName->Length; i++)
				{
					ret = DmVectorFileReadBack_Mipi(tfSite, siteIndex, dmModuleAlias, sclkAlias, sdataAlias, vectorFileName[i], isReadHistoryRam, isReadFailCount, PatternControlGlobalResultVariable.historyRam, PatternControlGlobalResultVariable.vecFailCount, PatternControlGlobalResultVariable.vecFirstFail);
					if (ret != 0) goto EndOfTest;

					// Result logger - Record result
					PatternControlGlobalResultVariable.resMsg_VecFileName[i]	= vectorFileName[i];
					PatternControlGlobalResultVariable.resMsg_VecFailCount[i]	= PatternControlGlobalResultVariable.vecFailCount;
					PatternControlGlobalResultVariable.resMsg_VecFirstFail[i]	= PatternControlGlobalResultVariable.vecFirstFail;
					PatternControlGlobalResultVariable.resMsg_HistoryRam[i]		= PatternControlGlobalResultVariable.historyRam;

					// Accumulate all existing fail count
					PatternControlGlobalResultVariable.totalVecFailCount += PatternControlGlobalResultVariable.vecFailCount;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_MIPI_READ_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_MipiRead] Fail to perform MIPI read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_MipiRead] Fail to perform MIPI read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmPatternTestFunction_SpiWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_SpiWrite
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias	- This is DM module alias.
		**		vectorFileName	- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for SPI protocol.
		**		Execute SPI wirte, which is run vector without any other function.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for pattern test with SPI protocol
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_SPI_WRITE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_SpiWrite] Fail to perform SPI write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_SpiWrite] Fail to perform SPI write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmPatternTestFunction_SpiRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_SpiRead
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias			- This is DM module alias.
		**		sclkAlias				- This is I2C clk pin alias.
		**		sdataAlias				- This is I2C data pin alias.
		**		vectorFileName			- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for SPI protocol.
		**		Execute SPI read, including ReadHistoryRam or ReadFailCount.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for pattern test with SPI protocol.
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_SPI_READ_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_SpiRead] Fail to perform SPI read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_SpiRead] Fail to perform SPI read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmPatternTestFunction_I2cWrite(int tfSite, int siteIndex, String ^ dmModuleAlias, array<String^> ^ vectorFileName)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_I2cWrite
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias	- This is DM module alias.
		**		vectorFileName	- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for I2C protocol.
		**		Execute I2C wirte, which is run vector without any other function.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for pattern test with I2C protocol
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_I2C_WRITE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_I2cWrite] Fail to perform I2C write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_I2cWrite] Fail to perform I2C write." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmPatternTestFunction_I2cRead(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, array<String^> ^ vectorFileName)
	{
		/*****************************************************************************************************
		**	DmPatternTestFunction_I2cRead
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias			- This is DM module alias.
		**		sclkAlias				- This is I2C clk pin alias.
		**		sdataAlias				- This is I2C data pin alias.
		**		vectorFileName			- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to execute test function (PatternTest) for I2C protocol.
		**		Execute I2C read, including ReadHistoryRam or ReadFailCount.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{
			// [Future Enchancement] Reserved for pattern test with I2C protocol.
		}
		catch (Exception ^ ex)
		{
			ret = ER_CONST_DM_PATTERN_TEST_I2C_READ_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_I2cRead] Fail to perform I2C read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmPatternTestFunction_I2cRead] Fail to perform I2C read." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}



	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: DM Helper Function - General
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::DmLoadVectorFile(int tfSite, int siteIndex, String ^ dmModuleAlias)
	{
		/*****************************************************************************************************
		**	DmLoadVectorFile
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias	- This is DM module alias.
		**
		**	Descriptions:
		**		This is a function to get all the vector file & vector state file info from the
		**		VectorFileFolder & VectorStateFileFolder.
		**		Load all the vector files from the VectorFileFolder into DM vector engine.
		******************************************************************************************************/

		// Local variable
		int ret	= 0;

		try
		{ 

//#pragma region "Get VectorFileFolder & VectorStateFileFolder directory"
//	
//			dmMipiVec.vecFileFolderName				= "VectorFileFolderSite" + siteIndex.ToString();
//			//dmMipiVec.vecFileFolderDirectory		= site->GetExternalDependency(dmMipiVec.vecFileFolderName);
//			dmMipiVec.vecFileFolderDirectory		= tl->glob->tf.RecipeFilePathDirectory + "\\" + dmMipiVec.vecFileFolderName;
//	
//			dmMipiVec.vecStateFileFolderName		= "VectorStateFileFolderSite" + siteIndex.ToString();
//			//dmMipiVec.vecStateFileFolderDirectory	= site->GetExternalDependency(dmMipiVec.vecStateFileFolderName);
//			dmMipiVec.vecStateFileFolderDirectory	= tl->glob->tf.RecipeFilePathDirectory + "\\" + dmMipiVec.vecStateFileFolderName;
//	
//#pragma endregion
//	
//#pragma region "Check VectorFileFolder & VectorStateFileFolder existence"
//	
//			if (!Directory::Exists(dmMipiVec.vecFileFolderDirectory))
//			{
//				ret = ER_CONST_PROJECT_VECTOR_FILE_FOLDER_NOT_FOUND;
//				tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] 'VectorFileFolder' does not exist in the project's 'TestRecipes' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] 'VectorFileFolder' does not exist in the project's 'TestRecipes' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				return ret;
//			}
//	
//			if (!Directory::Exists(dmMipiVec.vecStateFileFolderDirectory))
//			{
//				ret = ER_CONST_PROJECT_VECTOR_STATE_FILE_FOLDER_NOT_FOUND;
//				tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] 'VectorStateFileFolder' does not exist in the project's 'TestRecipes' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] 'VectorStateFileFolder' does not exist in the project's 'TestRecipes' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				return ret;
//			}
//	
//#pragma endregion
//	
//#pragma region "Get total vector file / vector state file count"
//	
//			dmMipiVec.totalVecFileExist			= Directory::GetFiles(dmMipiVec.vecFileFolderDirectory, "*.vec")->Length;
//			dmMipiVec.totalVecStateFileExist	= Directory::GetFiles(dmMipiVec.vecStateFileFolderDirectory, "*.csv")->Length;
//	
//			if (dmMipiVec.totalVecFileExist != dmMipiVec.totalVecStateFileExist)
//			{
//				ret = ER_CONST_PROJECT_VECTOR_FILE_STATE_FILE_CONTENT_NOT_MATCH;
//				tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] VectorFile and VectorStateFile filename/amount/content not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] VectorFile and VectorStateFile filename/amount/content not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				return ret;
//			}
//	
//			if (dmMipiVec.totalVecFileExist > 1024)
//			{
//				ret = ER_CONST_PROJECT_VECTOR_FILE_MAX_COUNT;
//				tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] VectorFile amount cannot exceed 1024x file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] VectorFile amount cannot exceed 1024x file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//				return ret;
//			}
//	
//#pragma endregion
//	
//#pragma region "Get VectorFile & VectorStateFile directory"
//	
//			dmMipiVec.vecFilePathList		= gcnew array<String^>(dmMipiVec.totalVecFileExist);
//			dmMipiVec.vecFilePathList		= Directory::GetFiles(dmMipiVec.vecFileFolderDirectory, "*.vec");
//	
//			dmMipiVec.vecStateFilePathList	= gcnew array<String^>(dmMipiVec.totalVecStateFileExist);
//			dmMipiVec.vecStateFilePathList	= Directory::GetFiles(dmMipiVec.vecStateFileFolderDirectory, "*.csv");
//	
//#pragma endregion
//	
//#pragma region "Get VectorFileName & VectorFileNumber"
//	
//			dmMipiVec.vecFileName			= gcnew array<String^>(dmMipiVec.totalVecFileExist);
//			dmMipiVec.vecFileNumber			= gcnew array<int>(dmMipiVec.totalVecFileExist);
//											
//			array<String ^> ^ arrStr		= gcnew array<String ^>(0);
//			array<String ^> ^ separator		= gcnew array<String ^>(1);
//			separator[0]					= "\\";
//	
//			for (int i = 0; i < dmMipiVec.totalVecFileExist; i++)
//			{
//				arrStr						= dmMipiVec.vecFilePathList[i]->Split(separator, StringSplitOptions::None);
//				dmMipiVec.vecFileName[i]	= arrStr[8];
//				dmMipiVec.vecFileName[i]	= dmMipiVec.vecFileName[i]->Replace(".vec", "");
//				dmMipiVec.vecFileNumber[i]	= i;
//			}
//	
//#pragma endregion
	
#pragma region "DM --> Setup resource array"
	
			array<int> ^ resourceArr = gcnew array<int>(tl->glob->VectorFile.totalVecFileExist);
			for (int i = 0; i < tl->glob->VectorFile.totalVecFileExist; i++)
			{
				resourceArr[i] = 1;
			}
			tl->CheckError(tfSite, dm[siteIndex]->DPINVectorResourceAllocation(dmModuleAlias, tl->glob->VectorFile.totalVecFileExist, resourceArr));

#pragma endregion
	
#pragma region "DM --> VecLoad"
	
			for (int i = 0; i < tl->glob->VectorFile.totalVecFileExist; i++)
			{
				tl->CheckError(tfSite, dm[siteIndex]->DPINVecLoad(dmModuleAlias, DM_CONST_DIRECTION_BIDIRECTIONAL_IO, tl->glob->VectorFile.vecFileNumber[i], tl->glob->VectorFile.vecFilePathList[i]));
			}
	
#pragma endregion

		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_LOAD_VECTOR_FILE_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] Fail to laod VectorFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmLoadVectorFile] Fail to laod VectorFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmVectorFileReadBack_Mipi(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ sclkAlias, String ^ sdataAlias, String ^ targetVecFileName, bool isReadHistoryRam, bool isReadFailCount, array<int> ^ % readHistoryRam, int % vecFailCount, int % vecFirstFail)
	{
		/*****************************************************************************************************
		**	DmVectorFileReadBack_Mipi
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is dc object index, normally start from 0.
		**		dmModuleAlias			- This is DM module alias.
		**		sclkAlias				- This is MIPI sclk pin alias.
		**		sdataAlias				- This is MIPI sdata pin alias.
		**		targetVecFileName		- This is target vector file name to execute.
		**		isReadHistoryRam		- This is to enable DM ReadHistoryRam function.
		**		isReadFailCount			- This is to enable DM ReadFailCount & GetFirstFail function.
		**		readHistoryRam			- Return an array of history ram data, regarding the amount of 'Read' opration inside the target vector file.
		**		vecFailCount			- Return the total fail count inside the target vector file.
		**		vecFirstFail			- Return the line number of the first fail occur inside the target vector file.
		**
		**	Descriptions:
		**		This is a function to get all the info in the target VectorFile/VectorStateFile, including dpingroup, sclk channel, sdata channel, timing set, mipi set.
		**		Get amount of read operation and the start read location in the target VectorFile/VectorStateFile.
		**		Run the target VectorFile.
		**		Perform read history ram & vector fail count.
		******************************************************************************************************/

		// Local variable
		int ret									= 0;
		String ^ errorMessage					= "NA";
		String ^ targetVecFileDirectory			= String::Empty;
		String ^ targetVecStateFileDirectory	= String::Empty;

#pragma region "Check target VectorFile & VectorStateFile existence"

		targetVecFileDirectory = tl->glob->VectorFile.VectorFileFolderDirectory + "\\" + targetVecFileName + ".vec";

		if (!File::Exists(targetVecFileDirectory))
		{
			ret = ER_CONST_PROJECT_VECTOR_FILE_NOT_FOUND;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Target vector file does not exist in the 'VectorFileFolder' (under project's 'TestRecipes')." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Target vector file does not exist in the 'VectorFileFolder' (under project's 'TestRecipes')." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

		targetVecStateFileDirectory = tl->glob->VectorStateFile.VectorStateFileFolderDirectory + "\\" + targetVecFileName + ".csv";

		if (!File::Exists(targetVecStateFileDirectory))
		{
			ret = ER_CONST_PROJECT_VECTOR_STATE_FILE_NOT_FOUND;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Target vector state file does not exist in the 'VectorStateFileFolder' (under project's 'TestRecipes')." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Target vector state file does not exist in the 'VectorStateFileFolder' (under project's 'TestRecipes')." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

#pragma endregion

		int dpinGroup		= 999;
		int sclkChannel		= 999;
		int sdataChannel	= 999;
		int vecTimingSet	= 999;

		array<String^> ^ mode;
		array<int> ^ usid;
		array<int> ^ regAddr;
		array<int> ^ regData;
		array<String^> ^ operation;
		array<String^> ^ speed;

		array<String^> ^ actualMode;
		array<int> ^ actualUsid;
		array<int> ^ actualRegAddr;
		array<int> ^ actualRegData1;
		array<int> ^ actualRegData2;
		array<int> ^ actualRegData3;
		array<int> ^ actualRegData4;
		array<int> ^ actualRegData5;
		array<int> ^ actualRegData6;
		array<int> ^ actualRegData7;
		array<int> ^ actualRegData8;
		array<int> ^ actualRegData9;
		array<int> ^ actualRegData10;
		array<int> ^ actualRegData11;
		array<int> ^ actualRegData12;
		array<int> ^ actualRegData13;
		array<int> ^ actualRegData14;
		array<int> ^ actualRegData15;
		array<int> ^ actualRegData16;
		array<String^> ^ actualOperation;
		array<String^> ^ actualSpeed;
		array<int> ^ actualRegDataSetCount;

		int initialMipiSetCount			= 0;
		int actualMipiSetCount			= 0;
		array<String^> ^ mipiSetInfo	= gcnew array<String^>(6);
		StreamReader ^ reader			= nullptr;
		String ^ readerLine				= String::Empty;
		int lineNumber					= 0;

#pragma region "Get initial mipi set inside a single VectorSteteFile"

		reader = gcnew StreamReader(targetVecStateFileDirectory);
		
		while ((readerLine = reader->ReadLine()) != nullptr)
		{
			lineNumber++;

			if (lineNumber > 5)
			{
				initialMipiSetCount++; 
			}
		}

		if (initialMipiSetCount == 0)
		{
			ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SET_INPUT_EMPTY;
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> DmVectorFileReadBack_Mipi] MIPI set setting cannot remain empty in the VectorStateFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> DmVectorFileReadBack_Mipi] MIPI set setting cannot remain empty in the VectorStateFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

		reader->Close();
		reader		= nullptr;
		readerLine	= String::Empty;
		lineNumber	= 0;

#pragma endregion

#pragma region "Setup storage for initial mipi set"

		mode		= gcnew array<String^>(initialMipiSetCount);
		usid		= gcnew array<int>(initialMipiSetCount);
		regAddr		= gcnew array<int>(initialMipiSetCount);
		regData		= gcnew array<int>(initialMipiSetCount);
		operation	= gcnew array<String^>(initialMipiSetCount);
		speed		= gcnew array<String^>(initialMipiSetCount);

		for (int i = 0; i < initialMipiSetCount; i++)
		{
			mode[i]			= String::Empty;
			usid[i]			= 999;
			regAddr[i]		= 999;
			regData[i]		= 999;
			operation[i]	= String::Empty;
			speed[i]		= String::Empty;
		}

#pragma endregion

#pragma region "Get general info and each mipi set info inside a single VectorStateFile"

		reader = gcnew StreamReader(targetVecStateFileDirectory);

		int index = 0;
		while ((readerLine = reader->ReadLine()) != nullptr)
		{
			lineNumber++;
			mipiSetInfo = readerLine->Split(',');

			if (lineNumber == 1)
			{
				ret = GetDpinGroupInfo(mipiSetInfo, dpinGroup, errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;
			}
			else if (lineNumber == 2)
			{
				ret = GetSclkChannelInfo(mipiSetInfo, dpinGroup, sclkChannel, errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;
			}
			else if (lineNumber == 3)
			{
				ret = GetSdataChannelInfo(mipiSetInfo, dpinGroup, sclkChannel, sdataChannel, errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;
			}
			else if (lineNumber == 4)
			{
				ret = GetVecTimingSetInfo(mipiSetInfo, vecTimingSet, errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;
			}
			else if (lineNumber == 5)
			{
				ret = CheckMipiSetIdentifier(mipiSetInfo, errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;
			}
			else
			{
				ret = GetModeList(mipiSetInfo, mode[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				ret = GetUSIDList(mipiSetInfo, usid[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				ret = GetRegAddrList(mipiSetInfo, mode[index], regAddr[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				ret = GetRegDataList(mipiSetInfo, mode[index], regData[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				ret = GetOperationList(mipiSetInfo, mode[index], operation[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				ret = GetSpeedList(mipiSetInfo, mode[index], speed[index], errorMessage);
				if ((errorMessage != "NA") || (ret != 0)) goto EndOfTest;

				index++;
			}
		}

		if (lineNumber == 5)
		{
			ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SET_INPUT_EMPTY;
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> DmVectorFileReadBack_Mipi] MIPI set setting cannot remain empty in the VectorStateFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> DmVectorFileReadBack_Mipi] MIPI set setting cannot remain empty in the VectorStateFile." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			goto EndOfTest;
		}

		reader->Close();
		reader		= nullptr;
		readerLine	= String::Empty;
		lineNumber	= 0;

#pragma endregion

#pragma region "Setup storage for actual mipi set"

		bool isFirstMipiSet			= true;
		String ^ previousMode		= String::Empty;
		int previousUsid			= 999;
		int previousRegAddr			= 999;
		int previousRegData			= 999;
		String ^ previousOperation	= String::Empty;
		String ^ previousSpeed		= String::Empty;

		for (int i = 0; i < initialMipiSetCount; i++)
		{
			if (mode[i] == "Extended")
			{
				if (isFirstMipiSet == true)
				{
					previousMode		= mode[i];
					previousUsid		= usid[i];
					previousRegAddr		= regAddr[i];
					previousRegData		= regData[i];
					previousOperation	= operation[i];
					previousSpeed		= speed[i];

					isFirstMipiSet = false;
				}
				else
				{
					if ((previousUsid == usid[i]) && (previousOperation == operation[i]) && (previousSpeed == speed[i]))
					{
						if (regAddr[i] == (previousRegAddr + 1))
						{
							previousMode		= mode[i];
							previousUsid		= usid[i];
							previousRegAddr		= regAddr[i];
							previousRegData		= regData[i];
							previousOperation	= operation[i];
							previousSpeed		= speed[i];
						}
						else
						{
							actualMipiSetCount++;

							previousMode		= mode[i];
							previousUsid		= usid[i];
							previousRegAddr		= regAddr[i];
							previousRegData		= regData[i];
							previousOperation	= operation[i];
							previousSpeed		= speed[i];
						}
					}
					else
					{
						actualMipiSetCount++;

						previousMode		= mode[i];
						previousUsid		= usid[i];
						previousRegAddr		= regAddr[i];
						previousRegData		= regData[i];
						previousOperation	= operation[i];
						previousSpeed		= speed[i];
					}
				}
			}
			else if (mode[i] == "Basic")
			{
				if (isFirstMipiSet == true)
				{
					isFirstMipiSet = false;
				}
				else
				{
					actualMipiSetCount++;
				}
			}
			else if (mode[i] == "Reg0")
			{
				if (isFirstMipiSet == true)
				{
					isFirstMipiSet = false;
				}
				else
				{
					actualMipiSetCount++;
				}
			}
		}

		actualMipiSetCount = actualMipiSetCount + 1;

		actualMode				= gcnew array<String^>(actualMipiSetCount);
		actualUsid				= gcnew array<int>(actualMipiSetCount);
		actualRegAddr			= gcnew array<int>(actualMipiSetCount);
		actualRegData1			= gcnew array<int>(actualMipiSetCount);
		actualRegData2			= gcnew array<int>(actualMipiSetCount);
		actualRegData3			= gcnew array<int>(actualMipiSetCount);
		actualRegData4			= gcnew array<int>(actualMipiSetCount);
		actualRegData5			= gcnew array<int>(actualMipiSetCount);
		actualRegData6			= gcnew array<int>(actualMipiSetCount);
		actualRegData7			= gcnew array<int>(actualMipiSetCount);
		actualRegData8			= gcnew array<int>(actualMipiSetCount);
		actualRegData9			= gcnew array<int>(actualMipiSetCount);
		actualRegData10			= gcnew array<int>(actualMipiSetCount);
		actualRegData11			= gcnew array<int>(actualMipiSetCount);
		actualRegData12			= gcnew array<int>(actualMipiSetCount);
		actualRegData13			= gcnew array<int>(actualMipiSetCount);
		actualRegData14			= gcnew array<int>(actualMipiSetCount);
		actualRegData15			= gcnew array<int>(actualMipiSetCount);
		actualRegData16			= gcnew array<int>(actualMipiSetCount);
		actualOperation			= gcnew array<String^>(actualMipiSetCount);
		actualSpeed				= gcnew array<String^>(actualMipiSetCount);
		actualRegDataSetCount	= gcnew array<int>(actualMipiSetCount);

		for (int i = 0; i < actualMipiSetCount; i++)
		{
			actualMode[i]				= String::Empty;
			actualUsid[i]				= 999;
			actualRegAddr[i]			= 999;
			actualRegData1[i]			= 999;
			actualRegData2[i]			= 999;
			actualRegData3[i]			= 999;
			actualRegData4[i]			= 999;
			actualRegData5[i]			= 999;
			actualRegData6[i]			= 999;
			actualRegData7[i]			= 999;
			actualRegData8[i]			= 999;
			actualRegData9[i]			= 999;
			actualRegData10[i]			= 999;
			actualRegData11[i]			= 999;
			actualRegData12[i]			= 999;
			actualRegData13[i]			= 999;
			actualRegData14[i]			= 999;
			actualRegData15[i]			= 999;
			actualRegData16[i]			= 999;
			actualOperation[i]			= String::Empty;
			actualSpeed[i]				= String::Empty;
			actualRegDataSetCount[i]	= 0;
		}

		actualMipiSetCount = 0;

#pragma endregion

#pragma region "Revise the actual mipi set info if involved Extended mode"

		int actualMipiSetIndex		= 0;
		int regDataSetCountIndex	= 0;
		isFirstMipiSet				= true;
		previousMode				= String::Empty;
		previousUsid				= 999;
		previousRegAddr				= 999;
		previousOperation			= String::Empty;
		previousSpeed				= String::Empty;

		for (int i = 0; i < initialMipiSetCount; i++)
		{
			if (mode[i] == "Extended")
			{
				if (isFirstMipiSet == true)
				{
					previousUsid		= usid[i];
					previousRegAddr		= regAddr[i];
					previousOperation	= operation[i];
					previousSpeed		= speed[i];

					actualMode[actualMipiSetIndex]				= mode[i];
					actualUsid[actualMipiSetIndex]				= usid[i];
					actualRegAddr[actualMipiSetIndex]			= regAddr[i];
					actualRegData1[actualMipiSetIndex]			= regData[i];
					actualOperation[actualMipiSetIndex]			= operation[i];
					actualSpeed[actualMipiSetIndex]				= speed[i];
					actualRegDataSetCount[actualMipiSetIndex]	= 1;

					regDataSetCountIndex++;

					isFirstMipiSet = false;
				}
				else
				{
					if ((previousUsid == usid[i]) && (previousOperation == operation[i]) && (previousSpeed == speed[i]))
					{
						if (regAddr[i] == (previousRegAddr + 1))
						{
							if (regDataSetCountIndex == 1)
								actualRegData2[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 2)
								actualRegData3[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 3)
								actualRegData4[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 4)
								actualRegData5[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 5)
								actualRegData6[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 6)
								actualRegData7[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 7)
								actualRegData8[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 8)
								actualRegData9[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 9)
								actualRegData10[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 10)
								actualRegData11[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 11)
								actualRegData12[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 12)
								actualRegData13[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 13)
								actualRegData14[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 14)
								actualRegData15[actualMipiSetIndex] = regData[i];
							else if (regDataSetCountIndex == 15)
								actualRegData16[actualMipiSetIndex] = regData[i];

							regDataSetCountIndex++;

							actualRegDataSetCount[actualMipiSetIndex] = actualRegDataSetCount[actualMipiSetIndex] + 1;

							previousUsid		= usid[i];
							previousRegAddr		= regAddr[i];
							previousOperation	= operation[i];
							previousSpeed		= speed[i];
						}
						else
						{
							actualMipiSetCount++;

							actualMipiSetIndex++;

							previousUsid		= usid[i];
							previousRegAddr		= regAddr[i];
							previousOperation	= operation[i];
							previousSpeed		= speed[i];

							actualMode[actualMipiSetIndex]				= mode[i];
							actualUsid[actualMipiSetIndex]				= usid[i];
							actualRegAddr[actualMipiSetIndex]			= regAddr[i];
							actualRegData1[actualMipiSetIndex]			= regData[i];
							actualOperation[actualMipiSetIndex]			= operation[i];
							actualSpeed[actualMipiSetIndex]				= speed[i];
							actualRegDataSetCount[actualMipiSetIndex]	= 1;

							regDataSetCountIndex = 0;
							regDataSetCountIndex++;
						}
					}
					else
					{
						actualMipiSetCount++;

						actualMipiSetIndex++;

						previousUsid		= usid[i];
						previousRegAddr		= regAddr[i];
						previousOperation	= operation[i];
						previousSpeed		= speed[i];

						actualMode[actualMipiSetIndex]				= mode[i];
						actualUsid[actualMipiSetIndex]				= usid[i];
						actualRegAddr[actualMipiSetIndex]			= regAddr[i];
						actualRegData1[actualMipiSetIndex]			= regData[i];
						actualOperation[actualMipiSetIndex]			= operation[i];
						actualSpeed[actualMipiSetIndex]				= speed[i];
						actualRegDataSetCount[actualMipiSetIndex]	= 1;

						regDataSetCountIndex = 0;
						regDataSetCountIndex++;
					}
				}
			}
			else if (mode[i] == "Basic")
			{
				if (isFirstMipiSet == true)
				{
					actualMode[actualMipiSetIndex]				= mode[i];
					actualUsid[actualMipiSetIndex]				= usid[i];
					actualRegAddr[actualMipiSetIndex]			= regAddr[i];
					actualRegData1[actualMipiSetIndex]			= regData[i];
					actualOperation[actualMipiSetIndex]			= operation[i];
					actualSpeed[actualMipiSetIndex]				= speed[i];
					actualRegDataSetCount[actualMipiSetIndex]	= 1;

					isFirstMipiSet = false;
				}
				else
				{
					actualMipiSetCount++;

					actualMipiSetIndex++;

					previousUsid		= 999;
					previousRegAddr		= 999;
					previousOperation	= String::Empty;
					previousSpeed		= String::Empty;

					regDataSetCountIndex = 0;

					actualMode[actualMipiSetIndex]				= mode[i];
					actualUsid[actualMipiSetIndex]				= usid[i];
					actualRegAddr[actualMipiSetIndex]			= regAddr[i];
					actualRegData1[actualMipiSetIndex]			= regData[i];
					actualOperation[actualMipiSetIndex]			= operation[i];
					actualSpeed[actualMipiSetIndex]				= speed[i];
					actualRegDataSetCount[actualMipiSetIndex]	= 1;
				}
			}
			else if (mode[i] == "Reg0")
			{
				if (isFirstMipiSet == true)
				{
					actualMode[actualMipiSetIndex]				= mode[i];
					actualUsid[actualMipiSetIndex]				= usid[i];
					actualRegAddr[actualMipiSetIndex]			= regAddr[i];
					actualRegData1[actualMipiSetIndex]			= regData[i];
					actualOperation[actualMipiSetIndex]			= operation[i];
					actualSpeed[actualMipiSetIndex]				= speed[i];
					actualRegDataSetCount[actualMipiSetIndex]	= 1;

					isFirstMipiSet = false;
				}
				else
				{
					actualMipiSetCount++;

					actualMipiSetIndex++;

					previousUsid		= 999;
					previousRegAddr		= 999;
					previousOperation	= String::Empty;
					previousSpeed		= String::Empty;

					regDataSetCountIndex = 0;

					actualMode[actualMipiSetIndex]				= mode[i];
					actualUsid[actualMipiSetIndex]				= usid[i];
					actualRegAddr[actualMipiSetIndex]			= regAddr[i];
					actualRegData1[actualMipiSetIndex]			= regData[i];
					actualOperation[actualMipiSetIndex]			= operation[i];
					actualSpeed[actualMipiSetIndex]				= speed[i];
					actualRegDataSetCount[actualMipiSetIndex]	= 1;
				}
			}
		}

		actualMipiSetCount = actualMipiSetCount + 1;

#pragma endregion

#pragma region "Get vector read start location & vector read start speed & target read data"

		int readOperationCount = 0;
		for (int i = 0; i < actualMipiSetCount; i++)
		{
			if (actualOperation[i] == "Read")
			{
				if (actualMode[i] == "Extended")
				{
					readOperationCount += actualRegDataSetCount[i];
				}
				else if (actualMode[i] == "Basic")
				{
					readOperationCount++;
				}
				else if (actualMode[i] == "Reg0")
				{
					;
				}
			}
		}

		int bitCountAddUp					= 0;
		int bufferLine						= 2;

		int basicSSC						= 4;
		int basicSA							= 8;
		int basicOperation					= 6;
		int basicRegAddr					= 10;
		int basicCommandFrameParity			= 2;
		int basicCommandFrameBusPark		= 2;
		int basicRegData					= 16;
		int basicDataFrameParity			= 2;
		int basicDataFrameBusPark			= 2;

		int extendedSSC						= 4;
		int extendedSA						= 8;
		int extendedOperation				= 8;
		int extendedBC						= 8;
		int extendedCommandFrameParity		= 2;
		int extendedRegAddr					= 16;
		int extendedRegAddrParity			= 2;
		int extendedRegAddrBusPark			= 2;
		int extendedRegData					= 16;
		int extendedRegDataParity			= 2;
		int extendedRegDataBusPark			= 2;

		int reg0SSC							= 4;
		int reg0SA							= 8;
		int reg0FixOneBit					= 2;
		int regRegData						= 14;
		int reg0PFramearity					= 2;
		int reg0BusFramePark				= 2;

		int startWithActualLH_Full			= 1;
		int startWithActualLH_Half			= 3;

		array<int> ^ readStartLocation		= gcnew array<int>(readOperationCount);
		array<String^> ^ readStartSpeed		= gcnew array<String^>(readOperationCount);
		dmMipiVec.displayMipiUsidTarget		= gcnew array<int>(readOperationCount);
		dmMipiVec.displayMipiRegAddrTarget	= gcnew array<int>(readOperationCount);
		dmMipiVec.displayMipiRegDataTarget	= gcnew array<int>(readOperationCount);
		int readIndex						= 0;

		int readTargetDataIndex = 0;
		for (int i = 0; i < initialMipiSetCount; i++)
		{
			if (operation[i] == "Read")
			{
				dmMipiVec.displayMipiUsidTarget[readTargetDataIndex]	= usid[i];
				dmMipiVec.displayMipiRegAddrTarget[readTargetDataIndex] = regAddr[i];
				dmMipiVec.displayMipiRegDataTarget[readTargetDataIndex] = regData[i];
				readTargetDataIndex++;
			}
		}

		for (int i = 0; i < actualMipiSetCount; i++)
		{
			if (actualMode[i] == "Extended")
			{
				if (actualOperation[i] == "Write")
				{
					bitCountAddUp += extendedSSC + extendedSA + extendedOperation + extendedBC + extendedCommandFrameParity + extendedRegAddr + extendedRegAddrParity + (extendedRegData * actualRegDataSetCount[i]) + (extendedRegDataParity * actualRegDataSetCount[i]) + extendedRegDataBusPark;

					if (i < (actualMipiSetCount - 1))
						bitCountAddUp += bufferLine + bufferLine;
					else
						bitCountAddUp += bufferLine;
				}
				else if (actualOperation[i] == "Read")
				{
					if (actualSpeed[i] == "Full")
					{
						bitCountAddUp += extendedSSC + extendedSA + extendedOperation + extendedBC + extendedCommandFrameParity + extendedRegAddr + extendedRegAddrParity + extendedRegAddrBusPark + (extendedRegData * actualRegDataSetCount[i]) + (extendedRegDataParity * actualRegDataSetCount[i]) + extendedRegDataBusPark;

						if (i < (actualMipiSetCount - 1))
						{
							bitCountAddUp += bufferLine + bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (extendedRegData * actualRegDataSetCount[i]) - (extendedRegDataParity * actualRegDataSetCount[i]) - extendedRegDataBusPark - bufferLine + startWithActualLH_Full + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
							readIndex++;

							for (int x = 0; x < (actualRegDataSetCount[i] - 1); x++)
							{
								readStartLocation[readIndex] = readStartLocation[readIndex - 1] + 18;
								readStartSpeed[readIndex] = actualSpeed[i];
								readIndex++;
							}
						}
						else
						{
							bitCountAddUp += bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (extendedRegData * actualRegDataSetCount[i]) - (extendedRegDataParity * actualRegDataSetCount[i]) - extendedRegDataBusPark + startWithActualLH_Full + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
							readIndex++;

							for (int x = 0; x < (actualRegDataSetCount[i] - 1); x++)
							{
								readStartLocation[readIndex] = readStartLocation[readIndex - 1] + 18;
								readStartSpeed[readIndex] = actualSpeed[i];
								readIndex++;
							}
						}
					}
					else if (actualSpeed[i] == "Half")
					{
						bitCountAddUp += extendedSSC + extendedSA + extendedOperation + extendedBC + extendedCommandFrameParity + extendedRegAddr + extendedRegAddrParity + extendedRegAddrBusPark + (extendedRegData * actualRegDataSetCount[i] * 2) + (extendedRegDataParity * actualRegDataSetCount[i] * 2) + extendedRegDataBusPark;

						if (i < (actualMipiSetCount - 1))
						{
							bitCountAddUp += bufferLine + bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (extendedRegData * actualRegDataSetCount[i] * 2) - (extendedRegDataParity * actualRegDataSetCount[i] * 2) - extendedRegDataBusPark - bufferLine + startWithActualLH_Half + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
							readIndex++;

							for (int x = 0; x < (actualRegDataSetCount[i] - 1); x++)
							{
								readStartLocation[readIndex] = readStartLocation[readIndex - 1] + 36;
								readStartSpeed[readIndex] = actualSpeed[i];
								readIndex++;
							}
						}
						else
						{
							bitCountAddUp += bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (extendedRegData * actualRegDataSetCount[i] * 2) - (extendedRegDataParity * actualRegDataSetCount[i] * 2) - extendedRegDataBusPark + startWithActualLH_Half + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
							readIndex++;

							for (int x = 0; x < (actualRegDataSetCount[i] - 1); x++)
							{
								readStartLocation[readIndex] = readStartLocation[readIndex - 1] + 36;
								readStartSpeed[readIndex] = actualSpeed[i];
								readIndex++;
							}
						}
					}
				}
			}
			else if (actualMode[i] == "Basic")
			{
				if (actualOperation[i] == "Write")
				{
					bitCountAddUp += basicSSC + basicSA + basicOperation + basicRegAddr + basicCommandFrameParity + basicRegData + basicDataFrameParity + basicDataFrameBusPark;

					if (i < (actualMipiSetCount - 1))
						bitCountAddUp += bufferLine + bufferLine;
					else
						bitCountAddUp += bufferLine;
				}
				else if (actualOperation[i] == "Read")
				{
					if (actualSpeed[i] == "Full")
					{
						bitCountAddUp += basicSSC + basicSA + basicOperation + basicRegAddr + basicCommandFrameParity + basicCommandFrameBusPark + basicRegData + basicDataFrameParity + basicDataFrameBusPark;

						if (i < (actualMipiSetCount - 1))
						{
							bitCountAddUp += bufferLine + bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - basicRegData - basicDataFrameParity - basicDataFrameBusPark - bufferLine + startWithActualLH_Full + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
						}
						else
						{
							bitCountAddUp += bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - basicRegData - basicDataFrameParity - basicDataFrameBusPark + startWithActualLH_Full + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
						}
					}
					else if (actualSpeed[i] == "Half")
					{
						bitCountAddUp += basicSSC + basicSA + basicOperation + basicRegAddr + basicCommandFrameParity + basicCommandFrameBusPark + (basicRegData * 2) + (basicDataFrameParity * 2) + basicDataFrameBusPark;

						if (i < (actualMipiSetCount - 1))
						{
							bitCountAddUp += bufferLine + bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (basicRegData * 2) - (basicDataFrameParity * 2) - basicDataFrameBusPark - bufferLine + startWithActualLH_Half + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
						}
						else
						{
							bitCountAddUp += bufferLine;
							readStartLocation[readIndex] = bitCountAddUp - (basicRegData * 2) - (basicDataFrameParity * 2) - basicDataFrameBusPark + startWithActualLH_Half + 1;
							readStartSpeed[readIndex] = actualSpeed[i];
						}
					}

					readIndex++;
				}
			}
			else if (actualMode[i] == "Reg0")
			{
				if (actualOperation[i] == "Write")
				{
					bitCountAddUp += reg0SSC + reg0SA + reg0FixOneBit + regRegData + reg0PFramearity + reg0BusFramePark;

					if (i < (actualMipiSetCount - 1))
						bitCountAddUp += bufferLine + bufferLine;
					else
						bitCountAddUp += bufferLine;
				}
			}
		}

#pragma endregion

#pragma region "Run target vector"

		int vecFileNumber = 9999;
		for (int i = 0; i < tl->glob->VectorFile.totalVecFileExist; i++)
		{
			if (tl->glob->VectorFile.vecFileName[i] == targetVecFileName)
			{
				vecFileNumber = i;
			}
		}

		int count		= 0;
		int dmStatus	= 999;
		while ((count <= 200000) && (dmStatus != 0))
		{
			dmStatus = 999;
			tl->CheckError(tfSite, dm[siteIndex]->AcquireVecEngineStatus(dmModuleAlias, dmStatus));

			if ((dmStatus == 0))
			{
				tl->CheckError(tfSite, dm[siteIndex]->RunVector(dmModuleAlias, vecFileNumber));
				tl->Util->WaitSecond(dmMipiVec.runMipiDelay);
				break;
			}
			tl->Util->WaitSecond(1.0 uS);
			count++;

			if (count >= 200000)
			{
				ret = ER_CONST_DM_VECTOR_ENGINE_STATUS_BUSY;
				tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Read History Ram Option"

		PatternControlGlobalResultVariable.totalVecReadCount = readOperationCount;

		readHistoryRam = gcnew array<int>(readOperationCount);
		for (int i = 0; i < readOperationCount; i++)
		{
			readHistoryRam[i] = 0;
		}

		try
		{ 
			if (isReadHistoryRam == true)
			{
				count					= 0;
				dmStatus				= 999;
				int readBackVecCount	= 0;
				int readRawLineIndex	= 0;

				array<int> ^ readBackArrRaw;
				array<int> ^ readBackArr;
				int readBackSumInt = 0;

				if (tl->glob->AWV.Debug)
				{
					for (int i = 0; i < readOperationCount; i++)
					{
						if (readStartSpeed[i] == "Full")
							readBackVecCount = 16;
						else if (readStartSpeed[i] == "Half")
							readBackVecCount = 32;

						readBackArrRaw = gcnew array<int>(readBackVecCount);

						if (readStartSpeed[i] == "Full")
							readBackArr = gcnew array<int>(readBackVecCount / 2);
						else if (readStartSpeed[i] == "Half")
							readBackArr = gcnew array<int>(readBackVecCount / 4);

						while ((count <= 200000) && (dmStatus != 0))
						{
							dmStatus = 999;
							tl->CheckError(tfSite, dm[siteIndex]->AcquireVecEngineStatus(dmModuleAlias, dmStatus));

							if ((dmStatus == 0))
							{
								tl->CheckError(tfSite, dm[siteIndex]->ReadHistoryRam(dmModuleAlias, readBackVecCount, readStartLocation[i], vecFileNumber, readBackArrRaw));
								break;
							}

							tl->Util->WaitSecond(1.0 uS);
							count++;
							
							if (count >= 200000)
							{
								ret = ER_CONST_DM_VECTOR_ENGINE_STATUS_BUSY;
								tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								goto EndOfTest;
							}
						}

						if (readStartSpeed[i] == "Full")
						{
							for (int j = 0; j < (readBackVecCount / 2); j++)
							{
								if ((dpinGroup == 1) || (dpinGroup == 2))
								{
									if (sdataChannel == 6)
										sdataChannel = 0;
									else if (sdataChannel == 7)
										sdataChannel = 1;
									else if (sdataChannel == 8)
										sdataChannel = 2;
									else if (sdataChannel == 9)
										sdataChannel = 3;
									else if (sdataChannel == 10)
										sdataChannel = 4;
									else if (sdataChannel == 11)
										sdataChannel = 5;

									if (sdataChannel == 0)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (0 * 2)) & 0x000003;
									if (sdataChannel == 1)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (1 * 2)) & 0x000003;
									if (sdataChannel == 2)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (2 * 2)) & 0x000003;
									if (sdataChannel == 3)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (3 * 2)) & 0x000003;
									if (sdataChannel == 4)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (4 * 2)) & 0x000003;
									if (sdataChannel == 5)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (5 * 2)) & 0x000003;
								}
								else if (dpinGroup == 3)
								{
									if (sdataChannel == 0)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (0 * 2)) & 0x000003;
									if (sdataChannel == 1)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (1 * 2)) & 0x000003;
									if (sdataChannel == 2)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (2 * 2)) & 0x000003;
									if (sdataChannel == 3)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (3 * 2)) & 0x000003;
									if (sdataChannel == 4)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (4 * 2)) & 0x000003;
									if (sdataChannel == 5)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (5 * 2)) & 0x000003;
									if (sdataChannel == 6)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (6 * 2)) & 0x000003;
									if (sdataChannel == 7)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (7 * 2)) & 0x000003;
									if (sdataChannel == 8)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (8 * 2)) & 0x000003;
									if (sdataChannel == 9)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (9 * 2)) & 0x000003;
									if (sdataChannel == 10)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (10 * 2)) & 0x000003;
									if (sdataChannel == 11)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (11 * 2)) & 0x000003;
								}

								if (readBackArr[j] == 2)
								{
									readHistoryRam[i] = 999;
								
									ret = ER_CONST_DM_PATTERN_TEST_READHISTORYRAM_UNDEFINED;
									tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Fail at ReadHistoryRam. Undefined (binary 10) has been detected. Please check the PE voltage setting, hardwares, device's spec, and vector file / vector state file contents." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Fail at ReadHistoryRam. Undefined (binary 10) has been detected. Please check the PE voltage setting, hardwares, device's spec, and vector file / vector state file contents." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									goto EndOfTest;
								}

								readBackSumInt += readBackArr[j] << ((readBackVecCount / 2) - (j + 1));
								readRawLineIndex += 2;
							}
						}
						else if (readStartSpeed[i] == "Half")
						{
							for (int j = 0; j < (readBackVecCount / 4); j++)
							{
								if ((dpinGroup == 1) || (dpinGroup == 2))
								{
									if (sdataChannel == 6)
										sdataChannel = 0;
									else if (sdataChannel == 7)
										sdataChannel = 1;
									else if (sdataChannel == 8)
										sdataChannel = 2;
									else if (sdataChannel == 9)
										sdataChannel = 3;
									else if (sdataChannel == 10)
										sdataChannel = 4;
									else if (sdataChannel == 11)
										sdataChannel = 5;

									if (sdataChannel == 0)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (0 * 2)) & 0x000003;
									if (sdataChannel == 1)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (1 * 2)) & 0x000003;
									if (sdataChannel == 2)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (2 * 2)) & 0x000003;
									if (sdataChannel == 3)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (3 * 2)) & 0x000003;
									if (sdataChannel == 4)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (4 * 2)) & 0x000003;
									if (sdataChannel == 5)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (5 * 2)) & 0x000003;
								}
								else if (dpinGroup == 3)
								{
									if (sdataChannel == 0)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (0 * 2)) & 0x000003;
									if (sdataChannel == 1)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (1 * 2)) & 0x000003;
									if (sdataChannel == 2)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (2 * 2)) & 0x000003;
									if (sdataChannel == 3)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (3 * 2)) & 0x000003;
									if (sdataChannel == 4)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (4 * 2)) & 0x000003;
									if (sdataChannel == 5)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (5 * 2)) & 0x000003;
									if (sdataChannel == 6)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (6 * 2)) & 0x000003;
									if (sdataChannel == 7)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (7 * 2)) & 0x000003;
									if (sdataChannel == 8)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (8 * 2)) & 0x000003;
									if (sdataChannel == 9)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (9 * 2)) & 0x000003;
									if (sdataChannel == 10)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (10 * 2)) & 0x000003;
									if (sdataChannel == 11)
										readBackArr[j] = (readBackArrRaw[readRawLineIndex] >> (11 * 2)) & 0x000003;
								}

								if (readBackArr[j] == 2)
								{
									readHistoryRam[i] = 999;

									ret = ER_CONST_DM_PATTERN_TEST_READHISTORYRAM_UNDEFINED;
									tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Fail at ReadHistoryRam. Undefined (binary 10) has been detected. Please check the PE voltage setting, hardwares, device's spec, and vector file / vector state file contents." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] Fail at ReadHistoryRam. Undefined (binary 10) has been detected. Please check the PE voltage setting, hardwares, device's spec, and vector file / vector state file contents." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									goto EndOfTest;
								}

								readBackSumInt += readBackArr[j] << ((readBackVecCount / 4) - (j + 1));
								readRawLineIndex += 4;
							}
						}

						readHistoryRam[i]	= readBackSumInt;
						readBackSumInt		= 0;
						readRawLineIndex	= 0;
						dmStatus			= 999;
						count				= 0;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_MIPI_VECTOR_READ_BACK_HISTORYRAM_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> DmVectorFileReadBack_Mipi] Fail at Mipi vector file read back through ReadHistoryRam." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> DmVectorFileReadBack_Mipi] Fail at Mipi vector file read back through ReadHistoryRam." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

#pragma endregion

#pragma region "Read Fail Count Option"

		vecFailCount					= 999;
		vecFirstFail					= 0;
		int fixVecLineFileHeader		= 2;
		int fixVecLineTotalSpacing		= 3;
		int fixVecLineDmSettingHeader	= 1;
		int fixVecLineDmSetting			= 3;
		int fixVecLineMipiSetHeader		= 1;
		int fixVecLineTotalMipiSet		= actualMipiSetCount;

		try
		{ 
			if (isReadFailCount == true)
			{
				count		= 0;
				dmStatus	= 999;

				while ((count <= 200000) && (dmStatus != 0))
				{
					dmStatus = 999;
					tl->CheckError(tfSite, dm[siteIndex]->AcquireVecEngineStatus(dmModuleAlias, dmStatus));

					if ((dmStatus == 0))
					{
						tl->CheckError(tfSite, dm[siteIndex]->AcquireChannelFirstFailVectorCount(sdataAlias, vecFirstFail));

						if (vecFirstFail != -1)
							vecFirstFail = vecFirstFail + fixVecLineFileHeader + fixVecLineTotalSpacing + fixVecLineDmSettingHeader + fixVecLineDmSetting + fixVecLineMipiSetHeader + fixVecLineTotalMipiSet;

						tl->CheckError(tfSite, dm[siteIndex]->AcquireChannelVectorFailCount(sdataAlias, vecFailCount));
						break;
					}

					tl->Util->WaitSecond(1.0 uS);
					count++;
					
					if (count >= 200000)
					{
						ret = ER_CONST_DM_VECTOR_ENGINE_STATUS_BUSY;
						tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmVectorFileReadBack_Mipi] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						goto EndOfTest;
					}
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_MIPI_VECTOR_READ_BACK_FAILCOUNT_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> DmVectorFileReadBack_Mipi] Fail at Mipi vector file read back through ReadFailCount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[AM400Utility -> DmVectorFileReadBack_Mipi] Fail at Mipi vector file read back through ReadFailCount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

#pragma endregion

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmVectorFileReadBack_Spi(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	DmVectorFileReadBack_Spi
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc resource's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute pattern test function for SPI protocol.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// [Future Enchancement] Reserved for vector write/read back with SPI protocol.

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmVectorFileReadBack_I2c(int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	DmVectorFileReadBack_I2c
		**		tfSite		- This is techFlow site index.
		**		siteIndex	- This is dc resource's object index, normally start from 0.
		**
		**	Descriptions:
		**		This is a function to execute pattern test function for I2C protocol.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// [Future Enchancement] Reserved for vector write/read back with I2c protocol.

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::DmRunVector(int tfSite, int siteIndex, String ^ dmModuleAlias, String ^ targetVecFileName)
	{
		/*****************************************************************************************************
		**	DmRunVector
		**		tfSite				- This is techFlow site index.
		**		siteIndex			- This is dc resource's object index, normally start from 0.
		**		dmModuleAlias		- This is DM module alias.
		**		targetVecFileName	- This is vector file name to execute.
		**
		**	Descriptions:
		**		This is a function to play vector file, without any extra function involved.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		try
		{ 
			int vecFileNumber = 9999;
			for (int i = 0; i < tl->glob->VectorFile.totalVecFileExist; i++)
			{
				if (tl->glob->VectorFile.vecFileName[i] == targetVecFileName)
				{
					vecFileNumber = i;
				}
			}

			int count		= 0;
			int dmStatus	= 999;
			while ((count <= 200000) && (dmStatus != 0))
			{
				dmStatus = 999;
				tl->CheckError(tfSite, dm[siteIndex]->AcquireVecEngineStatus(dmModuleAlias, dmStatus));

				if ((dmStatus == 0))
				{
					tl->CheckError(tfSite, dm[siteIndex]->RunVector(dmModuleAlias, vecFileNumber));
					tl->Util->WaitSecond(dmMipiVec.runMipiDelay);
					break;
				}
				tl->Util->WaitSecond(1.0 uS);
				count++;

				if (count >= 200000)
				{
					ret = ER_CONST_DM_VECTOR_ENGINE_STATUS_BUSY;
					tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmRunVector] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmRunVector] DM vector engine status in BUSY." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					goto EndOfTest;
				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_DM_RUN_VECTOR_FAIL;
			tl->WriteToTracerLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmRunVector] Fail to run vector file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			tl->WriteToFileLogger(tfSite, siteIndex, ERROR, "[DM400Utility -> DmRunVector] Fail to run vector file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}



	/*
	**	----------------------------------------------------------------------------------------------------
	**	APIs: DM Helper Function - Special For Mipi Test Library
	**	----------------------------------------------------------------------------------------------------
	*/
	int AMB7300TestLibrary::GetDpinGroupInfo(array<String^> ^ mipiSetInfo, int % dpinGroup, String^ % errorMessage)
	{
		int ret			= 0;
		dpinGroup		= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (mipiSetInfo[0] != "DM_DPinGroup")
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_DPINGROUP_NOT_FOUND;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPinGroup' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPinGroup' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		if (String::IsNullOrEmpty(mipiSetInfo[1]))
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_DPINGROUP_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPinGroup' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPinGroup' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}
		else
		{
			dpinGroup = Convert::ToInt32(mipiSetInfo[1], 10);

			if ((dpinGroup < 1) || (dpinGroup > 3))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_DPINGROUP_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPINGroup' setting only available in: 1 (ch0~5) or 2 (ch6~11) or 3 (ch0~11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'DM_DPINGroup' setting only available in: 1 (ch0~5) or 2 (ch6~11) or 3 (ch0~11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetSclkChannelInfo(array<String^> ^ mipiSetInfo, int dpinGroup, int % sclkChannel, String^ % errorMessage)
	{
		int ret			= 0;
		sclkChannel		= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (mipiSetInfo[0] != "SCLK_Channel")
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_SCLK_CH_NOT_FOUND;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		if (String::IsNullOrEmpty(mipiSetInfo[1]))
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_SCLK_CH_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}
		else
		{
			sclkChannel = Convert::ToInt32(mipiSetInfo[1], 10);

			if ((sclkChannel < 0) || (sclkChannel > 11))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SCLK_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' setting only available in: 0~11 (CH0~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' setting only available in: 0~11 (CH0~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((dpinGroup == 1) && ((sclkChannel < 0) || (sclkChannel > 5)))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SCLK_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, 'SCLK_Channel' only support: 0~5 (CH0~CH5)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, 'SCLK_Channel' only support: 0~5 (CH0~CH5)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((dpinGroup == 2) && ((sclkChannel < 6) || (sclkChannel > 11)))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SCLK_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, With 'DM_DPINGroup' = 2, 'SCLK_Channel' only support: 6~11 (CH6~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, With 'DM_DPINGroup' = 2, 'SCLK_Channel' only support: 6~11 (CH6~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetSdataChannelInfo(array<String^> ^ mipiSetInfo, int dpinGroup, int sclkChannel, int % sdataChannel, String^ % errorMessage)
	{
		int ret			= 0;
		sdataChannel	= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (mipiSetInfo[0] != "SDATA_Channel")
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_NOT_FOUND;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		if (String::IsNullOrEmpty(mipiSetInfo[1]))
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}
		else
		{
			sdataChannel = Convert::ToInt32(mipiSetInfo[1], 10);

			if ((sdataChannel < 0) || (sdataChannel > 11))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' setting only available in: 0~11 (CH0~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SDATA_Channel' setting only available in: 0~11 (CH0~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((dpinGroup == 1) && ((sdataChannel < 0) || (sdataChannel > 5)))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, 'SDATA_Channel' only support: 0~5 (CH0~CH5)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 1, 'SDATA_Channel' only support: 0~5 (CH0~CH5)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((dpinGroup == 2) && ((sdataChannel < 6) || (sdataChannel > 11)))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 2, 'SDATA_Channel' only support: 6~11 (CH6~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] When 'DM_DPINGroup' = 2, 'SDATA_Channel' only support: 6~11 (CH6~CH11)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if (sdataChannel == sclkChannel)
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_SDATA_CH_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' & 'SDATA_Channel' setting cannot be the same channel at the same time." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'SCLK_Channel' & 'SDATA_Channel' setting cannot be the same channel at the same time." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetVecTimingSetInfo(array<String^> ^ mipiSetInfo, int % vecTimingSet, String^ % errorMessage)
	{
		int ret			= 0;
		vecTimingSet	= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (mipiSetInfo[0] != "VectorTimingSet")
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_TIMING_SET_NOT_FOUND;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		if (String::IsNullOrEmpty(mipiSetInfo[1]))
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_TIMING_SET_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}
		else
		{
			vecTimingSet = Convert::ToInt32(mipiSetInfo[1], 10);

			if ((vecTimingSet < 0) || (vecTimingSet > 1023))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_TIMING_SET_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' setting only available in: 0~1023." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'VectorTimingSet' setting only available in: 0~1023." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::CheckMipiSetIdentifier(array<String^> ^ mipiSetInfo, String^ % errorMessage)
	{
		int ret			= 0;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if ((mipiSetInfo[0] != "Mode")		||
			(mipiSetInfo[1] != "USID")		||
			(mipiSetInfo[2] != "RegAddr")	||
			(mipiSetInfo[3] != "RegData")	||
			(mipiSetInfo[4] != "Operation") ||
			(mipiSetInfo[5] != "Speed"))
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SET_HEADER_NOT_FOUND;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'Mode', 'USID', 'RegAddr', 'RegData', 'Operation', 'Speed' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] 'Mode', 'USID', 'RegAddr', 'RegData', 'Operation', 'Speed' identifier not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetModeList(array<String^> ^ mipiSetInfo, String^ % mode, String^ % errorMessage)
	{
		int ret			= 0;
		mode			= String::Empty;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (!String::IsNullOrEmpty(mipiSetInfo[0]))
		{
			mode = mipiSetInfo[0];

			bool isContainBasic		= false;
			bool isContainExtended	= false;
			bool isContainReg0		= false;
			isContainBasic			= mode->IndexOf("basic", StringComparison::OrdinalIgnoreCase) >= 0;
			isContainExtended		= mode->IndexOf("extended", StringComparison::OrdinalIgnoreCase) >= 0;
			isContainReg0			= mode->IndexOf("reg0", StringComparison::OrdinalIgnoreCase) >= 0;
			if ((isContainBasic == false) && (isContainExtended == false) && (isContainReg0 == false))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_MODE_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if (isContainBasic == true)
			{
				int basicModeAlphabetCount = 0;
				for (int i = 0; i < mode->Length; i++)
				{
					basicModeAlphabetCount++;
				}
				
				if (basicModeAlphabetCount != 5)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_MODE_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			if (isContainExtended == true)
			{
				int extendedModeAlphabetCount = 0;
				for (int i = 0; i < mode->Length; i++)
				{
					extendedModeAlphabetCount++;
				}
				
				if (extendedModeAlphabetCount != 8)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_MODE_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			if (isContainReg0 == true)
			{
				int reg0ModeAlphabetCount = 0;
				for (int i = 0; i < mode->Length; i++)
				{
					reg0ModeAlphabetCount++;
				}
				
				if (reg0ModeAlphabetCount != 4)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_MODE_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting only available in: 'Basic' or 'Extended' or 'Reg0'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}

			if (isContainBasic == true)
				mode = "Basic";
			else if (isContainExtended == true)
				mode = "Extended";
			else if (isContainReg0 == true)
				mode = "Reg0";
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_MODE_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Mode' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetUSIDList(array<String^> ^ mipiSetInfo, int % usid, String^ % errorMessage)
	{
		int ret			= 0;
		usid			= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (!String::IsNullOrEmpty(mipiSetInfo[1]))
		{
			usid = Convert::ToInt32(mipiSetInfo[1], 16);

			if (usid > 15)
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_USID_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'USID' setting only support 4x bits (0000~1111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'USID' setting only support 4x bits (0000~1111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_USID_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'USID' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'USID' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetRegAddrList(array<String^> ^ mipiSetInfo, String^ mode, int % regAddr, String^ % errorMessage)
	{
		int ret			= 0;
		regAddr			= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if ((!String::IsNullOrEmpty(mipiSetInfo[2])) || (mode == "Reg0"))
		{
			if ((String::IsNullOrEmpty(mipiSetInfo[2])) && (mode == "Reg0"))
				regAddr = 0;
			else
				regAddr = Convert::ToInt32(mipiSetInfo[2], 16);

			if ((mode == "Basic") && (regAddr > 31))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_ADDR_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Basic' Mode RegAddr only support 5x bits (00000~11111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Basic' Mode RegAddr only support 5x bits (00000~11111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if ((mode == "Extended") && (regAddr > 255))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_ADDR_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Extended' Mode RegAddr only support 8x bits (00000000~11111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Extended' Mode RegAddr only support 8x bits (00000000~11111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if ((mode == "Reg0") && (regAddr > 0))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_ADDR_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Reg0' Mode does not contains RegAddr, please insert value 0 or let it remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Reg0' Mode does not contains RegAddr, please insert value 0 or let it remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_ADDR_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'RegAddr' setting in 'Basic' & 'Extended' Mode cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'RegAddr' setting in 'Basic' & 'Extended' Mode cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetRegDataList(array<String^> ^ mipiSetInfo, String^ mode, int % regData, String^ % errorMessage)
	{
		int ret			= 0;
		regData			= 999;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (!String::IsNullOrEmpty(mipiSetInfo[3]))
		{
			regData = Convert::ToInt32(mipiSetInfo[3], 16);

			if (((mode == "Basic") || (mode == "Extended")) && (regData > 255))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_DATA_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Basic' & 'Extended' Mode RegData only support 8x bits (00000000~11111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Basic' & 'Extended' Mode RegData only support 8x bits (00000000~11111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if ((mode == "Reg0") && (regData > 127))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_DATA_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Reg0' Mode RegData only support 7x bits (0000000~1111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Reg0' Mode RegData only support 7x bits (0000000~1111111)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_DATA_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'RegData' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'RegData' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetOperationList(array<String^> ^ mipiSetInfo, String^ mode, String^ % operation, String^ % errorMessage)
	{
		int ret			= 0;
		operation		= String::Empty;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (!String::IsNullOrEmpty(mipiSetInfo[4]))
		{
			operation = mipiSetInfo[4];

			bool isContainWrite		= false;
			bool isContainRead		= false;
			isContainWrite			= operation->IndexOf("write", StringComparison::OrdinalIgnoreCase) >= 0;
			isContainRead			= operation->IndexOf("read", StringComparison::OrdinalIgnoreCase) >= 0;
			if ((isContainWrite == false) && (isContainRead == false))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_OPERATION_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if (isContainWrite == true)
			{
				int writeOperationAlphabetCount = 0;
				for (int i = 0; i < operation->Length; i++)
				{
					writeOperationAlphabetCount++;
				}
				
				if (writeOperationAlphabetCount != 5)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_OPERATION_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			if (isContainRead == true)
			{
				int readOperationAlphabetCount = 0;
				for (int i = 0; i < operation->Length; i++)
				{
					readOperationAlphabetCount++;
				}
				
				if (readOperationAlphabetCount != 4)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_OPERATION_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting only available in: 'Write' or 'Read'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}

			if ((mode == "Reg0") && (isContainRead == true))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_OPERATION_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting for 'Reg0' mode only available in: 'Write'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting for 'Reg0' mode only available in: 'Write'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if (isContainWrite == true)
				operation = "Write";
			else if (isContainRead == true)
				operation = "Read";
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_OPERATION_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Operation' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}
	int AMB7300TestLibrary::GetSpeedList(array<String^> ^ mipiSetInfo, String^ mode, String^ % speed, String^ % errorMessage)
	{
		int ret			= 0;
		speed			= String::Empty;
		errorMessage	= "NA";
		int tfSite		= tl->glob->tf.TestSite;

		if (!String::IsNullOrEmpty(mipiSetInfo[4]))
		{
			speed = mipiSetInfo[5];

			bool isContainFull	= false;
			bool isContainHalf	= false;
			isContainFull		= speed->IndexOf("full", StringComparison::OrdinalIgnoreCase) >= 0;
			isContainHalf		= speed->IndexOf("half", StringComparison::OrdinalIgnoreCase) >= 0;
			if ((isContainFull == false) && (isContainHalf == false))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SPEED_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if (isContainFull == true)
			{
				int fullSpeedAlphabetCount = 0;
				for (int i = 0; i < speed->Length; i++)
				{
					fullSpeedAlphabetCount++;
				}
				
				if (fullSpeedAlphabetCount != 4)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SPEED_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			if (isContainHalf == true)
			{
				int halfSpeedAlphabetCount = 0;
				for (int i = 0; i < speed->Length; i++)
				{
					halfSpeedAlphabetCount++;
				}
				
				if (halfSpeedAlphabetCount != 4)
				{
					for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
					{
						ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SPEED_INPUT_INVALID;
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting only available in: 'Full' or 'Half'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}

			if (isContainFull == true)
				speed = "Full";
			else if (isContainHalf == true)
				speed = "Half";

			if ((mode == "Write") && (speed == "Half"))
			{
				for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
				{
					ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SPEED_INPUT_INVALID;
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting for 'Write' operation only available in: 'Full'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting for 'Write' operation only available in: 'Full'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}
		else
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = ER_CONST_VECTOR_STATE_FILE_MIPI_SPEED_INPUT_EMPTY;
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[DM400Utility -> VectorStateFile content verification] MIPI 'Speed' setting cannot remain empty." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

	EndOfTest:
		return ret;
	}

}


/*----------------------------------------------------------------------
* Revision Log
* &Log: DM400Utility.cpp.rca&


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
* Support DM resource configuration.
* Support DM resource test function.
* Support MIPI protocol (write/read) test function.
----------------------------------------------------------------------*/