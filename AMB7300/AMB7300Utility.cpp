/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			AMB7300Utility.cpp
Purpose:		Contain flow item condition value casting
				Contain flow item condition name & value validation
Version:		v1.0.0.11
----------------------------------------------------------------------*/


#include "..\\TestProgram.h"

namespace AMB7300_TestLibrary_REV2P0
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	TF Test Library XML & Flow Item's Conditions (Test Item, Test Parameter, Control Step)
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> AppsWideVariable"

	/// <summary> AppsWideVariable
	/// <list type="AppsWideVariable">
	/// <item attribute="name">SharedVNA</item>
	/// <item attribute="description">Specifies if VNA is shared for MultiUUT Offset.</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">gCreateFixedOffsetFile</item>
	/// <item attribute="description">True = create offset file as {TesterId}_{ProjectName}_CorrFactor_S{Site}.csv</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">SaveSnpData_Format</item>
	/// <item attribute="description">[snp] To enable the function of saving trace data into snp file. | Specifies the snp format.</item>
	/// <item attribute="value">SaveSnpDataOff/dB_Angle</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">S2Ppath</item>
	/// <item attribute="description">[snp] Specifies the snp filepath if SaveSnpDataOn.</item>
	/// <item attribute="value">C:\snp</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">S2Psave_ByHardBin</item>
	/// <item attribute="description">[snp] Move the S2P generated in S2Ppath to S2Ppath\Binnings [Based on the hard bin of the DUT result]</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">S2Prename_EN</item>
	/// <item attribute="description">[snp] 0 = Default | 1 = Rename as S2Prename_Name</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">S2Prename_Name</item>
	/// <item attribute="description">[snp] Specifies the snp file name if S2Prename_EN = 1, supported ${Macro} please refer to related Config Manual.</item>
	/// <item attribute="value">abc1234_$(Program Name)_$(TimeNow)</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">S2PswapData</item>
	/// <item attribute="description">[snp] Swap data in S2P file [Port1 -> Port2; Port2->Port1]</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">GenericStateMappingFile_EN</item>
	/// <item attribute="description">Use standardized state / mapping file istead of "AMB7300Config_DeviceName"</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">GenericStateMappingFile_Name</item>
	/// <item attribute="description">Specifies the file name in [ProgramData\Aemulus\AMB7300\State] for the generic state / mapping file when GenericStateMappingFile_EN is TRUE</item>
	/// <item attribute="value">AMB7300S6P1D_AMB7300_TestLibrary</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">HighPwrTest_EN</item>
	/// <item attribute="description">[HighPowerTest] 0 = Default | 1 = Enable high power testing. Expected external PA connected and separate AppsPowerCal done.</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">HighPwrTest_AppsCalFile</item>
	/// <item attribute="description">[HighPowerTest] Specifies AppsCalFile path if HighPwrTest_EN = 1.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\AppsCal\SampleProfile\VNA_Cal_list.csv</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">VNA_Mutex_EN</item>
	/// <item attribute="description">[Mutex] 0 = Off | 1 = Enable enable ping-pong testing, use this for index parallel testing when VNA resource is shared for multiple sites.</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching1</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 1.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching2</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 2.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching3</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 3.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching4</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 4.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching5</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 5.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="AppsWideVariable">
	/// <item attribute="name">PortMatching6</item>
	/// <item attribute="description">[PortMatching] To specify the snp filepath of matching circuit at Port 6.</item>
	/// <item attribute="value">C:\Aemulus\TesterInfo\Port Matching\PLPC.s2p</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </summary>

#pragma endregion
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Item: PreProcessing
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Item -> PreProcessing"

	/// <summary> PreProcessing
	/// <list type="ControlItem" name="PreProcessing" category="PreProcessing" group="Items" conditiontype="splitbysemicolon" methodname="PreProcessing">
	/// <item attribute="name">PreProcessing</item>
	/// <item attribute="displayas">PreProcessing</item>
	/// <item attribute="description">The is 'PreProcessing' item, which is to reload offsest file and spare for pre-test control.</item>
	/// <list type="ItemCondition">
	/// <item attribute="name">CorrFactorDirectory</item>
	/// <item attribute="description">Specifies the directory of CorrFactor file.</item>
	/// <item attribute="value">C:\Aemulus\techFlow3\Projects\TestRecipes\SampleProfile\AMB7300_TestLibrary_REV2P0_Beta\FixedOffsetFileFolder\AMB7300SR_AMB7300_TestLibrary_REV2P0_Beta_CorrFactor_S0.csv</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Item: PostProcessing
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Item -> PostProcessing"

	/// <summary> PostProcessing
	/// <list type="ControlItem" name="PostProcessing" category="PostProcessing" group="Items" conditiontype="splitbysemicolon" methodname="PostProcessing">
	/// <item attribute="name">PostProcessing</item>
	/// <item attribute="displayas">PostProcessing</item>
	/// <item attribute="description">The is 'PostProcessing' item, which spare for use of post-testing action.</item>
	/// <list type="ItemCondition">
	/// <item attribute="name">Condition</item>
	/// <item attribute="description">Specifies the condition description.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Step: DcControl
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Step -> DcControl"

	/// <summary> DcControl
	/// <list type="ControlStep" name="TestLib_ControlStep_DcControl" category="DcControl" group="Items" conditiontype="splitbysemicolon" methodname="seq_TestMethod">
	/// <item attribute="name">DcControl</item>
	/// <item attribute="displayas">DcControl</item>
	/// <item attribute="description">The is 'DcControl' item, which is to control and configure dc pin resource.</item>
	/// <list type="StepCondition">
	/// <item attribute="name">Module</item>
	/// <item attribute="description">Specifies the module type of the target pin resource.</item>
	/// <item attribute="value">AM</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">PinAlias</item>
	/// <item attribute="description">Specifies the pin alias of the target pin. The pin alias must exist in the resource planner.</item>
	/// <item attribute="value">VIO</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">ControlFunction</item>
	/// <item attribute="description">Specifies the control function to apply on the target pin.</item>
	/// <item attribute="value">DVCI</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">NPLC</item>
	/// <item attribute="description">Specifies the nplc to apply on the target pin.</item>
	/// <item attribute="value">0.1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">VoltageLevel</item>
	/// <item attribute="description">Specifies the voltage level for the target pin (applicable for drive or clamp).</item>
	/// <item attribute="value">1.8</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">V</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">CurrentLevel</item>
	/// <item attribute="description">Specifies the current level for the target pin (applicable for drive or clamp).</item>
	/// <item attribute="value">100</item>
	/// <item attribute="prefix">Micro</item>
	/// <item attribute="unit">A</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Delay</item>
	/// <item attribute="description">Specifies the delay time after execute the target pin.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">Milli</item>
	/// <item attribute="unit">sec</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// </list>
	/// <param name="site">This is techFlow site object.</param>
	/// </summary>

#pragma endregion
	int TestProgram::TestLib_ControlStep_DcControl(Site ^ site)
	{
		/*
		** Dummy control step function.
		** To generate Project Library's control step item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_ControlStep_DcControl_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_DcControl_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is resource object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'DcControl'.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		array<String^> ^ reservedForNA				= nullptr;
		int totalConditionCount						= 0;
		int totalConfigurationSets					= 0;
		int singleConfigurationSetItemCount			= 7; // For DcControl condition: 'Module', 'PinAlias', 'ControlFunction', 'NPLC', 'VoltageLevel', 'CurrentLevel', 'Delay'

#pragma region "Get total configuration sets count"

		//-------------------- Get Condition List Count --------------------
		totalConditionCount = tf_FlowStep_ConditionCount(tl->glob->currentSubItemName[siteIndex]);

		//-------------------- Get Configuration Sets --------------------
		totalConfigurationSets = totalConditionCount / singleConfigurationSetItemCount;

		if ((totalConditionCount % singleConfigurationSetItemCount) != 0)
		{
			ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
			String ^ additionalMessage =	"One complete set of 'DcControl' condition consists of " + singleConfigurationSetItemCount.ToString() + "x conditions."									+ "\n" +
											"Multiple sets of 'DcControl' condition are allowed by duplicate the condition with an additional '_1', '_2', '_3' at the end of the condition name."	+ "\n" +
											"Example: Module, PinAlias, ControlFunction, NPLC, VoltageLevel, CurrentLevel, Delay"																	+ "\n" +
											"Module_1, PinAlias_1, ControlFunction_1, NPLC_1, VoltageLevel_1, CurrentLevel_1, Delay_1"																+ "\n" +
											"Module_2, PinAlias_2, ControlFunction_2, NPLC_2, VoltageLevel_2, CurrentLeve_2, Delay_2";
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_Module + " & "  + DcControlConditionName_PinAlias + " & " + DcControlConditionName_ControlFunction + " & " + DcControlConditionName_NPLC + " & " + DcControlConditionName_VoltageLevel + " & " + DcControlConditionName_CurrentLevel);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition set verification] 'DcControl' condition set amount invalid. 1x condition set should contain " + singleConfigurationSetItemCount.ToString() + "x condition." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition set verification] 'DcControl' condition set amount invalid. 1x condition set should contain " + singleConfigurationSetItemCount.ToString() + "x condition." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

#pragma region "Module"

		//-------------------- Module --------------------
		amb7300tl->DcControlCSC.module = gcnew array<String^>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Module)))
			{
				 amb7300tl->DcControlCSC.module[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Module);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Module_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.module[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Module_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_Module);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Module' or 'Module_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Module' or 'Module_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'Module'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_Module, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "PinAlias"

		//-------------------- PinAlias --------------------
		amb7300tl->DcControlCSC.pinAlias = gcnew array<String^>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_PinAlias)))
			{
				amb7300tl->DcControlCSC.pinAlias[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_PinAlias);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_PinAlias_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.pinAlias[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_PinAlias_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_PinAlias);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'PinAlias' or 'PinAlias_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'PinAlias' or 'PinAlias_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'PinAlias'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_PinAlias, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "ControlFunction"

		//-------------------- ControlFunction --------------------
		amb7300tl->DcControlCSC.controlFunction = gcnew array<String^>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction)))
			{
				amb7300tl->DcControlCSC.controlFunction[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.controlFunction[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_ControlFunction);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'ControlFunction' or 'ControlFunction_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'ControlFunction' or 'ControlFunction_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'ControlFunction'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_ControlFunction, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "NPLC"

		//-------------------- NPLC --------------------
		amb7300tl->DcControlCSC.nplc = gcnew array<double>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_NPLC)))
			{
				amb7300tl->DcControlCSC.nplc[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_NPLC);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_NPLC_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.nplc[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_NPLC_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_NPLC);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'NPLC' or 'NPLC_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'NPLC' or 'NPLC_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'NPLC'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_NPLC, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "CurrentLevel"

		// Check current before voltage, because of the voltage checking in 'ValidateConditionValueInput_DcControl' needs current range info

		//-------------------- CurrentLevel --------------------
		amb7300tl->DcControlCSC.currentLevel = gcnew array<double>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_CurrentLevel)))
			{
				amb7300tl->DcControlCSC.currentLevel[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_CurrentLevel);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_CurrentLevel_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.currentLevel[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_CurrentLevel_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_CurrentLevel);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'CurrentLevel' or 'CurrentLevel_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'CurrentLevel' or 'CurrentLevel_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'CurrentLevel'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_CurrentLevel, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "VoltageLevel"

		//-------------------- VoltageLevel --------------------
		amb7300tl->DcControlCSC.voltageLevel = gcnew array<double>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_VoltageLevel)))
			{
				amb7300tl->DcControlCSC.voltageLevel[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_VoltageLevel);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_VoltageLevel_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.voltageLevel[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_VoltageLevel_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_VoltageLevel);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'VoltageLevel' or 'VoltageLevel_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'VoltageLevel' or 'VoltageLevel_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'VoltageLevel'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_VoltageLevel, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Delay"

		//-------------------- Delay --------------------
		amb7300tl->DcControlCSC.delay = gcnew array<double>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Delay)))
			{
				amb7300tl->DcControlCSC.delay[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Delay);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Delay_ + i.ToString())))
			{
				amb7300tl->DcControlCSC.delay[i] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_Delay_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_Delay);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Delay' or 'Delay_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Delay' or 'Delay_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input --> 'Delay'
		ret = ValidateConditionValueInput_DcControl(tfSite, siteIndex, DcControlConditionName_Delay, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: DcTest
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> DcTest"

	/// <summary> DcTest
	/// <list type="TestItem" name="TestLib_TestParameter_DcTest" category="DcTest" group="Items" conditiontype="splitbysemicolon" methodname="seq_TestMethod">
	/// <item attribute="name">DcTest_TestItemName</item>
	/// <item attribute="displayas">DcTest_TestItemName</item>
	/// <item attribute="description">This is 'DcTest' item with the combination of test item and test parameter, which is to execute the test function and return result to the particular test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_DcTest" category="DcTest" group="Items">
	/// <item attribute="name">DcTest_TestParameterName</item>
	/// <item attribute="displayas">DcTest_TestParameterName</item>
	/// <item attribute="description">DcTest</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">Micro</item>
	/// <item attribute="unit">A</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">0</item>
	/// <item attribute="lolimitprefix">Micro</item>
	/// <item attribute="hilimit">10</item>        
	/// <item attribute="hilimitprefix">Micro</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Module</item>
	/// <item attribute="description">Specifies the module type of the target pin resource.</item>
	/// <item attribute="value">AM</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">PinAlias</item>
	/// <item attribute="description">Specifies the pin alias of the target pin. The pin alias must exist in the resource planner.</item>
	/// <item attribute="value">VIO</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TestFunction</item>
	/// <item attribute="description">Specifies the test function to apply on the target pin.</item>
	/// <item attribute="value">MeasureCurrent</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">NPLC</item>
	/// <item attribute="description">Specifies the nplc to apply on the target pin during the measurement. For result tuning purpose.</item>
	/// <item attribute="value">0.1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">MeasureDelay</item>
	/// <item attribute="description">Specifies the delay time before the measurement take place on the target pin. For result tuning purpose.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">Milli</item>
	/// <item attribute="unit">sec</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion
	int TestProgram::TestLib_TestParameter_DcTest(Site ^ site)
	{
		/*
		** Dummy test parameter function.
		** To generate Project Library's test parameter item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_DcTest_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_TestParameter_DcTest_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is resource object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the test parameter item 'DcTest'.
		******************************************************************************************************/

		// Local variable
		int ret							= 0;
		array<String^> ^ reservedForNA	= nullptr;

#pragma region "Module"

		//-------------------- Module --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_Module))
		{
			amb7300tl->DcTestTPC.module = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_Module);
		}
		else
		{
			ret = ER_CONST_DCTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcTestConditionName_Module);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'Module' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'Module' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'Module'
		ret = ValidateConditionValueInput_DcTest(tfSite, siteIndex, DcTestConditionName_Module, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "PinAlias"

		//-------------------- PinAlias --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_PinAlias))
		{
			amb7300tl->DcTestTPC.pinAlias = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_PinAlias);
		}
		else
		{
			ret = ER_CONST_DCTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcTestConditionName_PinAlias);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'PinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'PinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'PinAlias'
		ret = ValidateConditionValueInput_DcTest(tfSite, siteIndex, DcTestConditionName_PinAlias, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "TestFunction"

		//-------------------- TestFunction --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_TestFunction))
		{
			amb7300tl->DcTestTPC.testFunction = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_TestFunction);
		}
		else
		{
			ret = ER_CONST_DCTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcTestConditionName_TestFunction);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'TestFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'TestFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		
		// Validate condition value input --> 'TestFunction'
		ret = ValidateConditionValueInput_DcTest(tfSite, siteIndex, DcTestConditionName_TestFunction, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "NPLC"

		//-------------------- NPLC --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_NPLC))
		{
			amb7300tl->DcTestTPC.nplc = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_NPLC);
		}
		else
		{
			ret = ER_CONST_DCTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcTestConditionName_NPLC);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'NPLC' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'NPLC' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'NPLC'
		ret = ValidateConditionValueInput_DcTest(tfSite, siteIndex, DcTestConditionName_NPLC, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "MeasureDelay"

		//-------------------- MeasureDelay --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_MeasureDelay))
		{
			amb7300tl->DcTestTPC.measureDelay = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcTestConditionName_MeasureDelay);
		}
		else
		{
			ret = ER_CONST_DCTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcTestConditionName_MeasureDelay);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'MeasureDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition name verification] 'MeasureDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MeasureDelay'
		ret = ValidateConditionValueInput_DcTest(tfSite, siteIndex, DcTestConditionName_MeasureDelay, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Step: PatternControl
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Step -> PatternControl"

	/// <summary> PatternControl
	/// <list type="ControlStep" name="TestLib_ControlStep_PatternControl" category="PatternControl" group="Items" conditiontype="splitbycomma" methodname="seq_TestMethod">
	/// <item attribute="name">PatternControl</item>
	/// <item attribute="displayas">PatternControl</item>
	/// <item attribute="description">The is 'PatternControl' item, which is to configure digital pattern setting.</item>
	/// <list type="StepCondition">
	/// <item attribute="name">ControlFunction</item>
	/// <item attribute="description">Specifies the control function to apply on the pattern operation.</item>
	/// <item attribute="value">MipiRead</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">isMultiVecToOneResult</item>
	/// <item attribute="description">This is opeartion setting. Select true if running multiple vector file (with multiple read back) and return a single result to a test parameter.</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">isOneVecToMultiResult</item>
	/// <item attribute="description">This is opration setting. Select true if running a single vector file (with single/multiple read back) and return single/multiple results to each test parameter.</item>
	/// <item attribute="value">True</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">sclkPinAlias</item>
	/// <item attribute="description">Specifies the pin alias for the mipi clock pin. The pin alias must exist in the resource planner.</item>
	/// <item attribute="value">SCLK</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">sdataPinAlias</item>
	/// <item attribute="description">Specifies the pin alias for the mipi data pin. The pin alias must exist in the resource planner.</item>
	/// <item attribute="value">SDATA</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">MipiFreq</item>
	/// <item attribute="description">Specifies the mipi speed or known as mipi frequency.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">DmTerminationVoltage</item>
	/// <item attribute="description">Specifies the DM pin termination voltage (Vth). Usually apply to mipi data pin. This is for mipi read back tuning purpose.</item>
	/// <item attribute="value">0.9</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">V</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">DmInputChDelay</item>
	/// <item attribute="description">Specifies the DM input channel delay in clock cycle. Usually apply to mipi data pin. This is for mipi read back tuning purpose.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">ClockCycle</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">RunVectorDelay</item>
	/// <item attribute="description">Specifies the delay time after playing the vector file or pattern.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">Milli</item>
	/// <item attribute="unit">sec</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">VectorFileName</item>
	/// <item attribute="description">Specifies the vector file name to be executed.</item>
	/// <item attribute="value">Read_Reg1F</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <param name="site">This is techFlow site object.</param>
	/// </summary>

#pragma endregion
	int TestProgram::TestLib_ControlStep_PatternControl(Site ^ site)
	{
		/*
		** Dummy control step function.
		** To generate Project Library's control step item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_ControlStep_PatternControl_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_PatternControl_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is resource object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'PatternControl'.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		array<String^> ^ reservedForNA				= nullptr;
		ConditionCollection ^ conditionCollection	= gcnew ConditionCollection;
		Condition ^ condition;

#pragma region "ControlFunction"

		//-------------------- ControlFunction --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_ControlFunction))
		{
			 amb7300tl->PatternControlCSC.controlFunction = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_ControlFunction);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_ControlFunction);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'ControlFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'ControlFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'ControlFunction'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_ControlFunction, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "isMultiVecToOneResult"

		//-------------------- isMultiVecToOneResult --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_isMultiVecToOneResult))
		{
			 amb7300tl->PatternControlCSC.isMultiVecToOneResult = (bool)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_isMultiVecToOneResult);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_isMultiVecToOneResult);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'isMultiVecToOneResult' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'isMultiVecToOneResult' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

#pragma region "isOneVecToMultiResult"

		//-------------------- isOneVecToMultiResult --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_isOneVecToMultiResult))
		{
			 amb7300tl->PatternControlCSC.isOneVecToMultiResult = (bool)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_isOneVecToMultiResult);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_isOneVecToMultiResult);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'isOneVecToMultiResult' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'isOneVecToMultiResult' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'isMultiVecToOneResult' + 'isOneVecToMultiResult'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_isOneVecToMultiResult, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "sclkPinAlias"

		//-------------------- sclkPinAlias --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_sclkPinAlias))
		{
			 amb7300tl->PatternControlCSC.sclkPinAlias = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_sclkPinAlias);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_sclkPinAlias);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'sclkPinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'sclkPinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'sclkPinAlias'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_sclkPinAlias, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "sdataPinAlias"

		//-------------------- sdataPinAlias --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_sdataPinAlias))
		{
			 amb7300tl->PatternControlCSC.sdataPinAlias = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_sdataPinAlias);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_sdataPinAlias);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'sdataPinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'sdataPinAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'sdataPinAlias'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_sdataPinAlias, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "MipiFreq"

		//-------------------- MipiFreq --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_MipiFreq))
		{
			amb7300tl->PatternControlCSC.userMipiFreq = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_MipiFreq);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_MipiFreq);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'MipiFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'MipiFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MipiFreq'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_MipiFreq, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "DmTerminationVoltage"

		//-------------------- DmTerminationVoltage --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_DmTerminationVoltage))
		{
			amb7300tl->PatternControlCSC.userVth = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_DmTerminationVoltage);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_DmTerminationVoltage);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'DmTerminationVoltage' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'DmTerminationVoltage' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'DmTerminationVoltage'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_DmTerminationVoltage, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "DmInputChDelay"

		//-------------------- DmInputChDelay --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_DmInputChDelay))
		{
			amb7300tl->PatternControlCSC.userDmInputChDelay = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_DmInputChDelay);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_DmInputChDelay);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'DmInputChDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'DmInputChDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'DmInputChDelay'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_DmInputChDelay, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "RunVectorDelay"

		//-------------------- RunVectorDelay --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_RunVectorDelay))
		{
			 amb7300tl->PatternControlCSC.runVectorDelay = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_RunVectorDelay);
		}
		else
		{
			ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_RunVectorDelay);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'RunVectorDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'RunVectorDelay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'RunVectorDelay'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_RunVectorDelay, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "VectorFileName"

		int countVectorFile = 0;
		conditionCollection = tf_FlowStep_ConditionList(tl->glob->currentSubItemName[siteIndex]);
		for each (condition in conditionCollection)
		{
			if (condition->Name->Contains(PatternControlConditionName_VectorFileName))
				countVectorFile++;
		}

		//-------------------- VectorFileName --------------------
		amb7300tl->PatternControlCSC.vectorFileName = gcnew array<String^>(countVectorFile);

		for (int i = 0; i < countVectorFile; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_VectorFileName)))
			{
				amb7300tl->PatternControlCSC.vectorFileName[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_VectorFileName);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_VectorFileName_ + i.ToString())))
			{
				amb7300tl->PatternControlCSC.vectorFileName[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternControlConditionName_VectorFileName_ + i.ToString());
			}
			else
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternControlConditionName_VectorFileName);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'VectorFileName' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition name verification] 'VectorFileName' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Record for generic use
		amb7300tl->PatternControlCSC.overallVectorFileCount = countVectorFile;

		// Validate condition value input --> 'VectorFileName'
		ret = ValidateConditionValueInput_PatternControl(tfSite, siteIndex, PatternControlConditionName_VectorFileName, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: PatternTest
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> PatternTest"

	/// <summary> PatternTest
	/// <list type="TestItem" name="TestLib_TestParameter_PatternTest" category="PatternTest" group="Items" conditiontype="splitbysemicolon" methodname="seq_TestMethod">
	/// <item attribute="name">PatternTest_TestItemName</item>
	/// <item attribute="displayas">PatternTest_TestItemName</item>
	/// <item attribute="description">This is 'PatternTest' item with the combination of test item and test parameter, which is to execute the test function and return result to the particular test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_PatternTest" category="PatternTest" group="Items">
	/// <item attribute="name">PatternTest_TestParameterName</item>
	/// <item attribute="displayas">PatternTest_TestParameterName</item>
	/// <item attribute="description">PatternTest</item>
	/// <item attribute="datatype">Int32</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">24</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">24</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">ReturnIndex</item>
	/// <item attribute="description">Represent the READ operation index inside the target vector file, and return the specific register data of the target READ operation to the test parameter.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion
	int TestProgram::TestLib_TestParameter_PatternTest(Site ^ site)
	{
		/*
		** Dummy test parameter function.
		** To generate Project Library's test parameter item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_PatternTest_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_PatternTest_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is resource object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the test parameter item 'PatternTest'.
		******************************************************************************************************/

		// Local variable
		int ret							= 0;
		array<String^> ^ reservedForNA	= nullptr;

#pragma region "ReturnIndex"

		//-------------------- ReturnIndex --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], PatternTestConditionName_ReturnIndex))
		{
			 amb7300tl->PatternTestTPC.returnIndex = (int)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], PatternTestConditionName_ReturnIndex);
		}
		else
		{
			ret = ER_CONST_PATTERNTEST_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, PatternTestConditionName_ReturnIndex);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternTest' condition name verification] 'ReturnIndex' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternTest' condition name verification] 'ReturnIndex' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'ReturnIndex'
		ret = ValidateConditionValueInput_PatternTest(tfSite, siteIndex, PatternTestConditionName_ReturnIndex, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		return ret;
	}


	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Step: DmControl
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Step -> DmControl"

	/// <summary> DmControl
	/// <list type="ControlStep" name="TestLib_ControlStep_DmControl" category="DmControl" group="Items" conditiontype="splitbysemicolon" methodname="seq_TestMethod">
	/// <item attribute="name">DmControl</item>
	/// <item attribute="displayas">DmControl</item>
	/// <item attribute="description">The is 'DmControl' item, which is to control and configure dc pin resource.</item>
	/// <list type="StepCondition">
	/// <item attribute="name">Module</item>
	/// <item attribute="description">Specifies the module type of the target pin resource.</item>
	/// <item attribute="value">AM</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">PinAlias</item>
	/// <item attribute="description">Specifies the pin alias of the target pin. The pin alias must exist in the resource planner.</item>
	/// <item attribute="value">VIO</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">ControlFunction</item>
	/// <item attribute="description">Specifies the control function to apply on the target pin.</item>
	/// <item attribute="value">DVCI</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">NPLC</item>
	/// <item attribute="description">Specifies the nplc to apply on the target pin.</item>
	/// <item attribute="value">0.1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">VoltageLevel</item>
	/// <item attribute="description">Specifies the voltage level for the target pin (applicable for drive or clamp).</item>
	/// <item attribute="value">1.8</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">V</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">CurrentLevel</item>
	/// <item attribute="description">Specifies the current level for the target pin (applicable for drive or clamp).</item>
	/// <item attribute="value">100</item>
	/// <item attribute="prefix">Micro</item>
	/// <item attribute="unit">A</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Delay</item>
	/// <item attribute="description">Specifies the delay time after execute the target pin.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">Milli</item>
	/// <item attribute="unit">sec</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// </list>
	/// <param name="site">This is techFlow site object.</param>
	/// </summary>

#pragma endregion
	int TestProgram::TestLib_ControlStep_DmControl(Site ^ site)
	{
		/*
		** Dummy control step function.
		** To generate Project Library's control step item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_ControlStep_DmControl_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_DmControl_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is resource object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'DmControl'.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		array<String^> ^ reservedForNA				= nullptr;
		int totalConditionCount						= 0;
		int totalConfigurationSets					= 0;
		int singleConfigurationSetItemCount			= 0; 

#pragma region "Get ControlFunction"
		if (!(tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction)))
		{
			amb7300tl->DmControlCSC.controlFunction = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DcControlConditionName_ControlFunction);
		}
		else
		{
			ret = ER_CONST_DMCONTROL_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_ControlFunction);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DmControl' condition name verification] 'ControlFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DmControl' condition name verification] 'ControlFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "Get total configuration sets count"

		//-------------------- Get Condition List Count --------------------
		totalConditionCount = tf_FlowStep_ConditionCount(tl->glob->currentSubItemName[siteIndex]);

		if (amb7300tl->DmControlCSC.controlFunction == "ConfigureOutputTrigger")
		{
			singleConfigurationSetItemCount = 3;
		}

		//-------------------- Get Configuration Sets --------------------
		totalConfigurationSets = ( totalConditionCount - 1) / singleConfigurationSetItemCount; //Minus 1 to exclude the ControlFunction condition

		if ((totalConditionCount % singleConfigurationSetItemCount) != 0)
		{
			ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
			String ^ additionalMessage =	"One complete set of 'DmControl' condition consists of " + singleConfigurationSetItemCount.ToString() + "x conditions."									+ "\n" +
				"Multiple sets of 'DmControl' condition are allowed by duplicate the condition with an additional '_1', '_2', '_3' at the end of the condition name."	+ "\n" +
				"Example: Module, PinAlias, ControlFunction, NPLC, VoltageLevel, CurrentLevel, Delay"																	+ "\n" +
				"Module_1, PinAlias_1, ControlFunction_1, NPLC_1, VoltageLevel_1, CurrentLevel_1, Delay_1"																+ "\n" +
				"Module_2, PinAlias_2, ControlFunction_2, NPLC_2, VoltageLevel_2, CurrentLeve_2, Delay_2";
			//amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DmControlConditionName_Module + " & "  + DmControlConditionName_PinAlias + " & " + DmControlConditionName_ControlFunction + " & " + DmControlConditionName_NPLC + " & " + DmControlConditionName_VoltageLevel + " & " + DmControlConditionName_CurrentLevel);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DmControl' condition set verification] 'DmControl' condition set amount invalid. 1x condition set should contain " + singleConfigurationSetItemCount.ToString() + "x condition." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DmControl' condition set verification] 'DmControl' condition set amount invalid. 1x condition set should contain " + singleConfigurationSetItemCount.ToString() + "x condition." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

#pragma region "ConfigureOutputTrigger"

		//-------------------- Array initialization --------------------
		amb7300tl->DmControlCSC.moduleAlias = gcnew array<String^>(totalConfigurationSets);
		amb7300tl->DmControlCSC.outputTrigger0 = gcnew array<int>(totalConfigurationSets);
		amb7300tl->DmControlCSC.outputTrigger1 = gcnew array<int>(totalConfigurationSets);

		for (int i = 0; i < totalConfigurationSets; i++)
		{
			if ((i == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_ModuleAlias)))
			{
				amb7300tl->DmControlCSC.moduleAlias[i] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_ModuleAlias);
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_TriggerOutput0 + i.ToString())))
			{
				amb7300tl->DmControlCSC.outputTrigger0[i] = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_TriggerOutput0 + i.ToString());
			}
			else if ((i > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_TriggerOutput1 + i.ToString())))
			{
				amb7300tl->DmControlCSC.outputTrigger1[i] = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], DmControlConditionName_TriggerOutput1 + i.ToString());
			}
			else
			{
				ret = ER_CONST_DCCONTROL_CONDITION_NAME_NOT_FOUND;
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, DcControlConditionName_Module);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Module' or 'Module_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition name verification] 'Module' or 'Module_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		// Validate condition value input
		ret = ValidateConditionValueInput_DmControl(tfSite, siteIndex, DmControlConditionName_ModuleAlias, reservedForNA, totalConfigurationSets);
		ret = ValidateConditionValueInput_DmControl(tfSite, siteIndex, DmControlConditionName_TriggerOutput0, reservedForNA, totalConfigurationSets);
		ret = ValidateConditionValueInput_DmControl(tfSite, siteIndex, DmControlConditionName_TriggerOutput1, reservedForNA, totalConfigurationSets);
		if (ret != 0) goto EndOfTest;

#pragma endregion

		EndOfTest:
				 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Step: VnaConfig
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Step -> VnaConfig"

	/// <summary> VnaConfig
	/// <list type="ControlStep" name="TestLib_ControlStep_VnaConfig" category="VnaConfig" group="Items" conditiontype="splitbycomma" methodname="seq_TestMethod">
	/// <item attribute="name">VnaConfig</item>
	/// <item attribute="displayas">VnaConfig</item>
	/// <item attribute="description">The is 'VnaConfig' item, which is to configure vna segment setting.</item>
	/// <list type="StepCondition">
	/// <item attribute="name">SweepType</item>
	/// <item attribute="description">Specifies the vna sweep type.</item>
	/// <item attribute="value">SharedMemoryTransfer</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">ChannelNum</item>
	/// <item attribute="description">Specifies Channel to be configured.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">StartFreq</item>
	/// <item attribute="description">Specifies the start frequency of the target segment.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">StopFreq</item>
	/// <item attribute="description">Specifies the stop frequency of the target segment.</item>
	/// <item attribute="value">3000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Points</item>
	/// <item attribute="description">Specifies the number of points of the target segment.</item>
	/// <item attribute="value">201</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Ifbw</item>
	/// <item attribute="description">Specifies the ifbw of the target segment.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Power</item>
	/// <item attribute="description">Specifies the power level of the target segment.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dBm</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">Delay</item>
	/// <item attribute="description">Specifies the delay of the target segment.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">sec</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">SNP</item>
	/// <item attribute="description">Specifies the file type to override the config auto SNP file.</item>
	/// <item attribute="value">NA</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <param name="site">This is techFlow site object.</param>
	/// </summary>

#pragma endregion 
	int TestProgram::TestLib_ControlStep_VnaConfig(Site ^ site)
	{
		/*
		** Dummy control step function.
		** To generate Project Library's control step item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_ControlStep_VnaConfig_CastCondition(Site ^ site, int tfSite, int siteIndex, int segmentSetCount)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_VnaConfig_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**	segmentSetCount	- This is total segment set count of the VNA segment settings.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'VnaConfig'.
		******************************************************************************************************/

		// Local variable
		int ret							= 0;
		array<String^> ^ reservedForNA	= nullptr;

		amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = Vna_Channel_Reset;
		amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannelAve = Vna_Channel_Reset;

#pragma region "SweepType"

		//-------------------- SweepType --------------------
		array<String^> ^ sweepType = gcnew array<String^>(1);
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_SweepType))
		{
			sweepType[0] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_SweepType);
		}
		else
		{
			ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_SweepType);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'SweepType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'SweepType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		
		// Validate condition value input --> 'SweepType'
		ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_SweepType, sweepType, amb7300tl->segmentSetting[siteIndex].segmentCount);
		if (ret != 0) goto EndOfTest;

		amb7300tl->segmentSetting[siteIndex].GetFromStateFile = false;
		if (sweepType[0] == Cond_SweepType_LinearFreq)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_Stimulus_SweepType_LinearFreq;
		}
		else if (sweepType[0] == Cond_SweepType_LogFreq)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_Stimulus_SweepType_LogFreq;
		}
		else if (sweepType[0] == Cond_SweepType_Segment)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_Stimulus_SweepType_Segment;
		}
		else if (sweepType[0] == Cond_SweepType_PowerSweep)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_Stimulus_SweepType_PowerSweep;
		}
		else if (sweepType[0] == Cond_SweepType_GetFromStateFile)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_SweepType_GetFromStateFile;
			amb7300tl->segmentSetting[siteIndex].GetFromStateFile	= true;
		}
		else if (sweepType[0] == Cond_SweepType_SharedMemoryTransfer)
		{
			amb7300tl->segmentSetting[siteIndex].sweepType			= Vna_SharedMemoryTransfer;
		}

#pragma endregion

#pragma region "ChannelNum"
		amb7300tl->vnaSetting[siteIndex].ChannelNumCondition_Exist = false;

		//-------------------- ChannelNum --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_ChannelNum))
		{
			amb7300tl->vnaSetting[siteIndex].ChannelNumCondition_Exist = true;

			// For True Parallel MultiUUT Project
			if (tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
			{
				amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_ChannelNum) + siteIndex;
			}
			else
			{
				amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_ChannelNum);
			}

			// Validate condition value input --> 'ChannelNum'
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_ChannelNum, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
		}
		//else
		{
			/*ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_ChannelNum);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'ChannelNum' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'ChannelNum' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;*/
		}

#pragma endregion

		/*if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SweepType_GetFromStateFile)
		{
			ret = amb7300tl->GetChannelandTraceSetting_CMT(tfSite, siteIndex);
			if (ret != 0) goto EndOfTest;
		}
		else*/
		{
#pragma region "Setting <-> Not from the control step condition"

			if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq || amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_PowerSweep)
			{
				amb7300tl->segmentSetting[siteIndex].segmentCount = 1;
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_Segment)
			{
				amb7300tl->segmentSetting[siteIndex].segmentCount = segmentSetCount;
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SweepType_GetFromStateFile)
			{
				amb7300tl->segmentSetting[siteIndex].segmentCount = 1;
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SharedMemoryTransfer)
			{
				amb7300tl->segmentSetting[siteIndex].segmentCount = segmentSetCount;
			}
			// [Future Enchancement] For GetFromStateFile, for now user still need to fill-in Points, maybe there is a way to grab the points from the selected state file.

		// Setup storage
			amb7300tl->segmentSetting[siteIndex].startFreq = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].stopFreq = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].point = gcnew array<int>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].ifbw = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].power = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].delay = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);
			amb7300tl->segmentSetting[siteIndex].singleMarker = gcnew array<double>(amb7300tl->segmentSetting[siteIndex].segmentCount);

			if (amb7300tl->prev_segmentSetting[siteIndex].segmentCount != amb7300tl->segmentSetting[siteIndex].segmentCount)
			{
				amb7300tl->prev_segmentSetting[siteIndex].segmentCount = amb7300tl->segmentSetting[siteIndex].segmentCount;
				amb7300tl->prev_segmentSetting[siteIndex].startFreq = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].stopFreq = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].point = gcnew array<int>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].ifbw = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].power = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].delay = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
				amb7300tl->prev_segmentSetting[siteIndex].singleMarker = gcnew array<double>(amb7300tl->prev_segmentSetting[siteIndex].segmentCount);
			}

#pragma endregion

			// Get segment settings. Support multiple segment set
			for (int segmentSetIndex = 0; segmentSetIndex < amb7300tl->segmentSetting[siteIndex].segmentCount; segmentSetIndex++)
			{
#pragma region"Start Freq"
				//-------------------- Start Freq --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StratFreq)))
				{
					amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StratFreq);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StratFreq_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StratFreq_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_StratFreq);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'StartFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'StartFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

#pragma region "Stop Freq"
				//-------------------- Stop Freq --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StopFreq)))
				{
					amb7300tl->segmentSetting[siteIndex].stopFreq[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StopFreq);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StopFreq_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].stopFreq[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_StopFreq_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_StopFreq);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'StopFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'StopFreq' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

#pragma region "Points"
				//-------------------- Points --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Points)))
				{
					amb7300tl->segmentSetting[siteIndex].point[segmentSetIndex] = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Points);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Points_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].point[segmentSetIndex] = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Points_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_Points);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Points' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Points' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

#pragma region "Ifbw"
				//-------------------- Ifbw --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Ifbw)))
				{
					amb7300tl->segmentSetting[siteIndex].ifbw[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Ifbw);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Ifbw_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].ifbw[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Ifbw_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_Ifbw);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Ifbw' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Ifbw' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

#pragma region "Power"
				//-------------------- Power --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Power)))
				{
					amb7300tl->segmentSetting[siteIndex].power[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Power);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Power_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].power[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Power_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_Power);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Power' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Power' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

#pragma region "Delay"
				//-------------------- Delay --------------------
				if ((segmentSetIndex == 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Delay)))
				{
					amb7300tl->segmentSetting[siteIndex].delay[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Delay);
				}
				else if ((segmentSetIndex > 0) && (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Delay_ + segmentSetIndex.ToString())))
				{
					amb7300tl->segmentSetting[siteIndex].delay[segmentSetIndex] = (double)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_Delay_ + segmentSetIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_Delay);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Delay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'Delay' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
#pragma endregion

				//-------------------- Single Marker (Dummy) --------------------
				if (segmentSetIndex == 0)
					amb7300tl->segmentSetting[siteIndex].singleMarker[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex];
			}

#pragma region "Setting <-> Not from the control step condition"

			// Get total points
			if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq)
			{
				amb7300tl->segmentSetting[siteIndex].totalPoint = amb7300tl->segmentSetting[siteIndex].point[0];
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_Segment)
			{
				int totalPoints = 0;
				for (int i = 0; i < amb7300tl->segmentSetting[siteIndex].point->Length; i++)
				{
					totalPoints += amb7300tl->segmentSetting[siteIndex].point[i];
				}
				amb7300tl->segmentSetting[siteIndex].totalPoint = totalPoints;
				totalPoints = 0;
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SweepType_GetFromStateFile)
			{
				amb7300tl->segmentSetting[siteIndex].totalPoint = amb7300tl->segmentSetting[siteIndex].point[0];
				// [Future Enchancement] For GetFromStateFile, for now user still need to fill-in Points, maybe there is a way to get from the selected state file.
			}
			else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SharedMemoryTransfer)
			{
			//	amb7300tl->segmentSetting[siteIndex].totalPoint = amb7300tl->segmentSetting[siteIndex].point[0];
				int totalPoints = 0;
				for (int i = 0; i < amb7300tl->segmentSetting[siteIndex].point->Length; i++)
				{
					totalPoints += amb7300tl->segmentSetting[siteIndex].point[i];
				}
				amb7300tl->segmentSetting[siteIndex].totalPoint = totalPoints;
				totalPoints = 0;
			}

			// Default set LogMag for initial segment setting only, will be seting again in control step item 'VnaFetch'
			amb7300tl->segmentSetting[siteIndex].sparaFormat = SParam_FormatVNA::LogMagnitude;

#pragma endregion
		}

		// Validate condition value input --> 'FreqStart' + 'FreqStop', 'Points', 'Ifbw', 'Power', 'Delay'
		if ((amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_LinearFreq) || (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_Stimulus_SweepType_Segment))
		{	
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_StratFreq, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_Points, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_Ifbw, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_Power, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
			ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_Delay, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			if (ret != 0) goto EndOfTest;
		}
		else if (amb7300tl->segmentSetting[siteIndex].sweepType == Vna_SweepType_GetFromStateFile)
		{
			//ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_Points, reservedForNA, amb7300tl->segmentSetting[siteIndex].segmentCount);
			//if (ret != 0) goto EndOfTest;
			// [Future Enchancement] For GetFromStateFile, for now user still need to fill-in Points, maybe there is a way to get from the selected state file.
		}

#pragma region "SNP"

		// Obsolete
		////-------------------- SNP --------------------
		//array<String^> ^ SNP_Override = gcnew array<String^>(1);
		//if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_SNP))
		//{
		//	SNP_Override[0] = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaConfigConditionName_SNP);
		//}
		//else
		//{
		//	ret = ER_CONST_VNACONFIG_CONDITION_NAME_NOT_FOUND;
		//	amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaConfigConditionName_SNP);
		//	tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'SNP' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//	tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition name verification] 'SNP' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//	return ret;
		//}

		//// Validate condition value input --> 'SweepType'
		//ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaConfigConditionName_SNP, SNP_Override, 1);
		//if (ret != 0) goto EndOfTest;

		//if (SNP_Override[0] == "NA")
		//{
		//	amb7300tl->projectStateInfo.SNP_Override = "NA";
		//}
		//else if (SNP_Override[0] == Vna_SaveDataToTouchstoneFile_Type_S3P)
		//{
		//	amb7300tl->projectStateInfo.SNP_Override = Vna_SaveDataToTouchstoneFile_Type_S3P;
		//}

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Control Step: VnaFetch
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Control Step -> VnaFetch"

	/// <summary> VnaFetch
	/// <list type="ControlStep" name="TestLib_ControlStep_VnaFetch" category="VnaFetch" group="Items" conditiontype="nosplit" methodname="seq_TestMethod">
	/// <item attribute="name">VnaFetch</item>
	/// <item attribute="displayas">VnaFetch</item>
	/// <item attribute="description">The is 'VnaFetch' item, which is to configure vna trace setting and then fetch data.</item>
	/// <list type="StepCondition">
	/// <item attribute="name">MeasurePortPair</item>
	/// <item attribute="description">Specifies the mpex port pair to perform the measurement. The alias must exist in the resource planner, and the port alias must follow the label on the mpex hardware. The group pair must exist in the mapping file as well.</item>
	/// <item attribute="value">OUT1,OUT17</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">TraceCount</item>
	/// <item attribute="description">Specifies the total trace count to activate in the target channel. The trace count must match with the 'TraceMeasurementFormat' setting.</item>
	/// <item attribute="value">4</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">TraceMeasurementFormat</item>
	/// <item attribute="description">Specifies the measurement parameter and format for each trace.</item>
	/// <item attribute="value">S11/LogMag,S21/LogMag,S12/LogMag,S22/LogMag</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="StepCondition">
	/// <item attribute="name">AveragingSetting</item>
	/// <item attribute="description">To enable averaging on the target channel. | Specifies the averaging factor.</item>
	/// <item attribute="value">AveOff/10</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/*/// <list type="StepCondition">
	/// <item attribute="name">ChannelNum</item>
	/// <item attribute="description">Only applicable for Keysight DIRECT Config, S4P1D/S6P1D.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>*/
	/// <list type="StepCondition">
	/// <item attribute="name">SaveSnpData</item>
	/// <item attribute="description">To enable snp data generation for current TestParameter.</item>
	/// <item attribute="value">False</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Boolean</item>
	/// </list>
	/// </list>
	/// <param name="site">This is techFlow site object.</param>
	/// </summary>

#pragma endregion 
	int TestProgram::TestLib_ControlStep_VnaFetch(Site ^ site)
	{
		/*
		** Dummy control step function.
		** To generate Project Library's control step item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod()
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_ControlStep_VnaFetch_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_ControlStep_VnaFetch_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'VnaFetch'.
		******************************************************************************************************/

		// Local variable
		int ret							= 0;
		array<String^> ^ reservedForNA	= nullptr;

#pragma region "Setting <-> Not from the control step condition - Reset Before New VnaFetch"

		// Reset vna setting, trace measurement parameter, and trace format before execute a new 'VnaFetch' (new sweep)
		for (int i = 0; i < 16; i++)
		{
			amb7300tl->vnaSetting[siteIndex].measType[i]			= Vna_Measurement_Type_Reset;
			amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i]		= Vna_Format_Reset;
		}
		amb7300tl->vnaSetting[siteIndex].userTraceCount				= 0;

#pragma endregion

#pragma region "MeasurePortPair"

		//-------------------- MeasurePortPair --------------------
		String ^ measurePortPair = String::Empty;
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_MeasurePortPair))
		{
			measurePortPair = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_MeasurePortPair);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_MeasurePortPair);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'MeasurePortPair' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'MeasurePortPair' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		array<String^> ^ arrStrFull		= gcnew array<String^>(0);
		array<String^> ^ separatorFull	= gcnew array<String^>(1);
		separatorFull[0]				= "/";
		arrStrFull						= measurePortPair->Split(separatorFull, StringSplitOptions::None);

		array<String^> ^ arrStr		= gcnew array<String^>(0);
		array<String^> ^ separator	= gcnew array<String^>(1);
		separator[0]				= ",";
		//arrStr						= measurePortPair->Split(separator, StringSplitOptions::None);
		if (arrStrFull->Length > 1)
		{
			arrStr = arrStrFull[siteIndex]->Split(separator, StringSplitOptions::None);
		}
		else
		{
			arrStr = arrStrFull[0]->Split(separator, StringSplitOptions::None);
		}

		// Validate condition value input --> 'MeasurePortPair'
		ret = ValidateConditionValueInput_VnaFetch(tfSite, siteIndex, VnaFetchConditionName_MeasurePortPair, arrStr);
		if (ret != 0) goto EndOfTest;

		int portCount = arrStr->Length;
		amb7300tl->vnaSetting[siteIndex].pinAlias = gcnew array<String^>(portCount);
		for (int i = 0; i < portCount; i++)
		{
			amb7300tl->vnaSetting[siteIndex].pinAlias[i] = arrStr[i];
		}

		/* 
		** Record the type of measurement 
		** Example: 2-port measurement: OUT1,OUT17
		** array size = 2
		** it is S2P type
		**
		** Example: 4-port measurement: OUT1,OUT2,OUT3,OUT4
		** array size = 4
		** it is S4P type
		*/
		amb7300tl->vnaSetting[siteIndex].measureSnpType = portCount;

#pragma endregion

#pragma region "TraceCount"

		//-------------------- TraceCount --------------------
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_TraceCount))
		{
			amb7300tl->vnaSetting[siteIndex].userTraceCount = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_TraceCount);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_TraceCount);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'TraceCount' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'TraceCount' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'TraceCount'
		ret = ValidateConditionValueInput_VnaFetch(tfSite, siteIndex, VnaFetchConditionName_TraceCount, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "TraceMeasurementFormat"

		//-------------------- TraceMeasurementFormat --------------------
		String ^ traceMeasurementFormat = String::Empty;
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_TraceMeasurementFormat))
		{
			traceMeasurementFormat = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_TraceMeasurementFormat);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_TraceMeasurementFormat);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'TraceMeasurementFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'TraceMeasurementFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		arrStr						= gcnew array<String^>(0);
		separator					= gcnew array<String^>(1);
		separator[0]				= ",";
		arrStr						= traceMeasurementFormat->Split(separator, StringSplitOptions::None);
		/*	outcome example	
		**	arrStr[0] = "S11/SWR"
		**	arrStr[1] = "S21/LogMag"
		**	arrStr[2] = "S12/LogMag"
		**	arrStr[3] = "S22/LogMag"
		*/

		// Validate condition value input --> 'TraceMeasurementFormat'
		ret = ValidateConditionValueInput_VnaFetch(tfSite, siteIndex, VnaFetchConditionName_TraceMeasurementFormat, arrStr);
		if (ret != 0) goto EndOfTest;

		array<String^> ^ arrStr2	= gcnew array<String^>(0);
		array<String^> ^ separator2 = gcnew array<String^>(1);
		separator2[0]				= "/";
		for (int i = 0; i < arrStr->Length; i++)
		{
			arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);
			/*	outcome example
			**	arrStr2[0] = "S11"	|	arrStr2[0]	= "S21"	
			**	arrStr2[1] = "SWR"	|	arrStr2[1]	= "LogMag"
			*/

			//-------------------- Trace Measure Type --------------------
			if ((arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S11) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S12) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S13) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S14) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S15) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S16) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S21) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S22) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S23) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S24) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S25) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S26) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S31) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S32) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S33) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S34) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S35) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S36) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S41) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S42) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S43) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S44) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S45) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S46) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S51) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S52) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S53) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S54) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S55) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S56) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S61) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S62) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S63) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S64) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S65) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_S66) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A11) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A12) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A13) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A14) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A15) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A16) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A21) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A22) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A23) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A24) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A25) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A26) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A31) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A32) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A33) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A34) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A35) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A36) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A41) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A42) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A43) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A44) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A45) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A46) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A51) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A52) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A53) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A54) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A55) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A56) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A61) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A62) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A63) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A64) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A65) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_A66) || 
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B11) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B12) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B13) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B14) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B15) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B16) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B21) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B22) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B23) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B24) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B25) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B26) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B31) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B32) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B33) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B34) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B35) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B36) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B41) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B42) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B43) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B44) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B45) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B46) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B51) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B52) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B53) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B54) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B55) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B56) ||
				(arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B61) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B62) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B63) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B64) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B65) || (arrStr2[0] == Cond_TraceMeasurementFormat_Parameter_B66))
			{
				amb7300tl->vnaSetting[siteIndex].measType[i] = arrStr2[0];
			}

			//-------------------- Trace Format --------------------
			if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_LogMag)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::MLOGarithmic);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SWR)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SWR);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_Phase)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::PHASe);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_ExPhase)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::UPHase);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_Delay)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::GDELay);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_LinMag)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::MLINear);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_Real)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::REAL);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_Imag)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::IMAGinary);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SmithLog)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SLOGarithmic);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SmithLin)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SLINear);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SmithReIm)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SCOMplex);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SmithRJX)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SMITh);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_SmithGJB)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::SADMittance);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_PolarLog)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::PLOGarithmic);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_PolarLin)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::PLINear);
			}
			else if (arrStr2[1] == Cond_TraceMeasurementFormat_Format_PolarReIm)
			{
				amb7300tl->vnaSetting[siteIndex].vna_traceFormat[i] = int(VNATraceFormat::POLar);
			}

			// Reset content
			arrStr2 = gcnew array<String^>(0);
		}

#pragma endregion

#pragma region "AveragingSetting"

		//-------------------- AveragingSetting --------------------
		String ^ averagingSetting = String::Empty;
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_AveragingSetting))
		{
			averagingSetting = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_AveragingSetting);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_AveragingSetting);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'AveragingSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'AveragingSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		arrStr			= gcnew array<String^>(0);
		separator		= gcnew array<String^>(1);
		separator[0]	= "/";
		arrStr			= averagingSetting->Split(separator, StringSplitOptions::None);

		// Validate condition value input --> 'AveragingSetting'
		ret = ValidateConditionValueInput_VnaFetch(tfSite, siteIndex, VnaFetchConditionName_AveragingSetting, arrStr);
		if (ret != 0) goto EndOfTest;

		// AveragingSetting parameter: AveragingEnable (AveOn | AveOff), AveragingFactor (default 10)
		if (arrStr->Length == 2)
		{
			amb7300tl->averagingSettings.averagingEnable = arrStr[0];
			amb7300tl->averagingSettings.averagingFactor = Double::Parse(arrStr[1]);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
			String ^ additionalMessage =	"'AveragingSetting' contain 2x parameter which is differentiate by '/'."	+ "\n" +
											"1st parameter: Enable averaging."											+ "\n" +
											"2nd parameter: Set averaging factor."										+ "\n" +
											"Example: AveOn/10 --> meaning enable averaging, apply 10 averaging factor.";
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_AveragingSetting, additionalMessage);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

#pragma region "ChannelNum"

		//// Obsolete
		////-------------------- ChannelNum --------------------
		////if ((amb7300tl->amb7300SystemSetting->systemAlias = AMB7300_S4P1D_TYPE) ||
		////	(amb7300tl->amb7300SystemSetting->systemAlias = AMB7300_S6P1D_TYPE))
		//{
		//	if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_ChannelNum))
		//	{
		//		//if (tl->glob->tf.activeUUT_count > 1)
		//		//{
		//		//	// Hardcode here for MultiUUT True Parellel channel
		//		//	if (siteIndex == 0)
		//		//		amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = 1;

		//		//	else if (siteIndex == 1)
		//		//		amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = 2;
		//		//}
		//		//else
		//		//{
		//		//	amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_ChannelNum);
		//		//}
		//		amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel = (int)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_ChannelNum) + siteIndex;
		//	}
		//	//else
		//	{
		//		/*ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
		//		amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_ChannelNum);
		//		tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'ChannelNum' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//		tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'ChannelNum' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//		return ret;*/
		//	}

		//	// Validate condition value input --> 'ChannelNum'
		//	ret = ValidateConditionValueInput_VnaConfig(tfSite, siteIndex, VnaFetchConditionName_ChannelNum, reservedForNA);
		//	if (ret != 0) goto EndOfTest;
		//}

#pragma endregion

#pragma region "SaveSnpData"

		//-------------------- SaveSnpData --------------------

		// Need to set SaveSnpDataOn in AWV to enable snp data generation for current TestParameter

		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_SaveSnpData))
		{
			amb7300tl->saveRecallSetting->EnableSaveSnpData = (bool)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_SaveSnpData);
		}
		else
		{
			// do nothing, code error message here if need to make this mandatory in future
		}

#pragma endregion

/*#pragma region "SnpDataFilePath"

		//-------------------- SnpDataFilePathway --------------------
		String ^ SnpDataFilePathway = String::Empty;
		if (tf_FlowStep_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_SnpDataFilePath))
		{
			amb7300tl->saveRecallSetting->touchstoneFilePath = (String^)tf_FlowStep_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaFetchConditionName_SnpDataFilePath);
		}
		else
		{
			ret = ER_CONST_VNAFETCH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaFetchConditionName_SnpDataFilePath);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'SnpDataFilePathway' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition name verification] 'SnpDataFilePathway' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion*/

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: VnaDataStore
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> VnaDataStore"

	/// <summary> VnaDataStore
	/// <list type="TestItem" name="TestLib_TestParameter_VnaDataStore" category="VnaDataStore" group="Items" conditiontype="splitbysemicoloncomma" methodname="seq_TestMethod">
	/// <item attribute="name">VnaDataStore_TestItemName</item>
	/// <item attribute="displayas">VnaDataStore_TestItemName</item>
	/// <item attribute="description">This is 'VnaDataStore' item with the combination of test item and test parameter, which is to execute test function and return result to test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataStore" category="VnaDataStore" group="Items">
	/// <item attribute="name">VnaDataStore_TestParameterName_S11</item>
	/// <item attribute="displayas">VnaDataStore_TestParameterName_S11</item>
	/// <item attribute="description">VnaDataStore</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataStore" category="VnaDataStore" group="Items">
	/// <item attribute="name">VnaDataStore_TestParameterName_S21</item>
	/// <item attribute="displayas">VnaDataStore_TestParameterName_S21</item>
	/// <item attribute="description">VnaDataStore</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataStore" category="VnaDataStore" group="Items">
	/// <item attribute="name">VnaDataStore_TestParameterName_S12</item>
	/// <item attribute="displayas">VnaDataStore_TestParameterName_S12</item>
	/// <item attribute="description">VnaDataStore</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">3</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataStore" category="VnaDataStore" group="Items">
	/// <item attribute="name">VnaDataStore_TestParameterName_S22</item>
	/// <item attribute="displayas">VnaDataStore_TestParameterName_S22</item>
	/// <item attribute="description">VnaDataStore</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">4</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion
	int TestProgram::TestLib_TestParameter_VnaDataStore(Site ^ site)
	{
		/* 
		** Dummy test parameter function.
		** To generate Project Library's test parameter item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod() 
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_VnaDataStore_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_TestParameter_VnaDataStore_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the control step item 'VnaDataStore'.
		******************************************************************************************************/
		int ret = 0;

		ret = TestLib_TestParameter_VnaDataAnalysis_CastCondition(site, tfSite, siteIndex);
		
	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: VnaDataAnalysis
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> VnaDataAnalysis"

	/// <summary> VnaDataAnalysis
	/// <list type="TestItem" name="TestLib_TestParameter_VnaDataAnalysis" category="VnaDataAnalysis" group="Items" conditiontype="splitbysemicoloncomma" methodname="seq_TestMethod">
	/// <item attribute="name">VnaDataAnalysis_TestItemName</item>
	/// <item attribute="displayas">VnaDataAnalysis_TestItemName</item>
	/// <item attribute="description">This is 'VnaDataAnalysis' item with the combination of test item and test parameter, which is to execute test function and return result to test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataAnalysis" category="VnaDataAnalysis" group="Items">
	/// <item attribute="name">VnaDataAnalysis_TestParameterName_S11</item>
	/// <item attribute="displayas">VnaDataAnalysis_TestParameterName_S11</item>
	/// <item attribute="description">VnaDataAnalysis</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataAnalysis" category="VnaDataAnalysis" group="Items">
	/// <item attribute="name">VnaDataAnalysis_TestParameterName_S21</item>
	/// <item attribute="displayas">VnaDataAnalysis_TestParameterName_S21</item>
	/// <item attribute="description">VnaDataAnalysis</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataAnalysis" category="VnaDataAnalysis" group="Items">
	/// <item attribute="name">VnaDataAnalysis_TestParameterName_S12</item>
	/// <item attribute="displayas">VnaDataAnalysis_TestParameterName_S12</item>
	/// <item attribute="description">VnaDataAnalysis</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">3</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaDataAnalysis" category="VnaDataAnalysis" group="Items">
	/// <item attribute="name">VnaDataAnalysis_TestParameterName_S22</item>
	/// <item attribute="displayas">VnaDataAnalysis_TestParameterName_S22</item>
	/// <item attribute="description">VnaDataAnalysis</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">4</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion
	int TestProgram::TestLib_TestParameter_VnaDataAnalysis(Site ^ site)
	{
		/* 
		** Dummy test parameter function.
		** To generate Project Library's test parameter item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod() 
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_VnaDataAnalysis_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_TestParameter_VnaDataAnalysis_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the test parameter item 'VnaDataAnalysis'.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		array<String^> ^ reservedForNA				= nullptr;
		array<String^> ^ arrStr						= gcnew array<String^>(0);
		array<String^> ^ separator					= gcnew array<String^>(1);
		array<String^> ^ arrStr2					= gcnew array<String^>(0);
		array<String^> ^ separator2					= gcnew array<String^>(1);
		ConditionCollection ^ conditionCollection	= gcnew ConditionCollection;
		Condition ^ condition;

#pragma region "TraceIndex"

		//-------------------- TraceIndex --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_TraceIndex))
		{
			amb7300tl->vnaDataAnalysisTPC.traceIndex = (int)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_TraceIndex);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_TraceIndex);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'TraceIndex' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'TraceIndex' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'TraceIndex'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_TraceIndex, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Function"

		//-------------------- Function --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Function))
		{
			amb7300tl->vnaDataAnalysisTPC.function = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Function);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_Function);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Function' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Function' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'Function'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_Function, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Freq | FreqRangeStart | FreqRangeStop"

		/*
		**	Default condition got 1x Freq, 1x FreqRangeStart, 1x FreqRangeStop.
		**	If it is a single test freq or single marker		--> user will insert in the 'Freq' condition.
		**	If it is a test freq range or marker freq range		--> user will insert in the 'FreqRangeStart' & 'FreqRangeStop' condition.
		**	If it is the multiple freq or freq range, the condition will have 'Freq_n' or 'FreqRangeStart_n', or 'FreqRangeStop_n' duplicated condition.
		**	The count and variable's array are reserved for the multiple set of freq or freq range condition in future.
		*/

		int countFreq				= 0;
		int countFreqRangeStart		= 0;
		int countFreqRangeStop		= 0;
		conditionCollection			= tf_TestParameter_ConditionList(tl->glob->currentSubItemName[siteIndex]);
		for each (condition in conditionCollection)
		{
			if (condition->Name->Contains(VnaDataAnalysisConditionName_FreqRangeStart))
				countFreqRangeStart++;
			else if (condition->Name->Contains(VnaDataAnalysisConditionName_FreqRangeStop))
				countFreqRangeStop++;
			else if (condition->Name->Contains(VnaDataAnalysisConditionName_Freq))
				countFreq++;
		}

		if ((countFreq == countFreqRangeStart) && (countFreq == countFreqRangeStop) & (countFreqRangeStart == countFreqRangeStop))
		{
			amb7300tl->vnaDataAnalysisTPC.testFreq				= gcnew array<double>(countFreq);
			amb7300tl->vnaDataAnalysisTPC.testFreqRangeStart	= gcnew array<double>(countFreqRangeStart);
			amb7300tl->vnaDataAnalysisTPC.testFreqRangeStop		= gcnew array<double>(countFreqRangeStop);

			for (int freqIndex = 0; freqIndex < countFreq; freqIndex++)
			{
				//-------------------- Single Freq --------------------
				if ((freqIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Freq)))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreq[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Freq);
				}
				else if ((freqIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Freq_ + freqIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreq[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Freq_ + freqIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_Freq);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Freq' or 'Freq_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Freq' or 'Freq_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				//-------------------- Freq Range Start --------------------
				if ((freqIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStart)))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreqRangeStart[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStart);
				}
				else if ((freqIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStart_ + freqIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreqRangeStart[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStart_ + freqIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_FreqRangeStart);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'FreqRangeStart' or 'FreqRangeStart_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'FreqRangeStart' or 'FreqRangeStart_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				//-------------------- Freq Range Stop --------------------
				if ((freqIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStop)))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreqRangeStop[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStop);
				}
				else if ((freqIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStop_ + freqIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testFreqRangeStop[freqIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_FreqRangeStop_ + freqIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_FreqRangeStop);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'FreqRangeStop' or 'FreqRangeStop_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'FreqRangeStop' or 'FreqRangeStop_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			// Record for generic use
			amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount = countFreq;
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Freq);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition set verification] 'Freq', 'FreqRangeStart', 'FreqRangeStop' condition name set amount invalid. Should have the same condition name set amount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition set verification] 'Freq', 'FreqRangeStart', 'FreqRangeStop' condition name set amount invalid. Should have the same condition name set amount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'Freq' + 'FreqRangeStart' + 'FreqRangeStop'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_Freq, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "PowerRangeStart | PowerRangeStop"

		/*
		**	Default condition got 1x PowerRangeStart, 1x PowerRangeStop.
		**	If it is a single test power or single marker		--> user will insert in the 'Power' condition.
		**	If it is a test power range or marker power range		--> user will insert in the 'PowerRangeStart' & 'PowerRangeStop' condition.
		**	If it is the multiple power or power range, the condition will have 'Power_n' or 'PowerRangeStart_n', or 'PowerRangeStop_n' duplicated condition.
		**	The count and variable's array are reserved for the multiple set of power or power range condition in future.
		*/

		int countPower = 0;
		int countPowerRangeStart = 0;
		int countPowerRangeStop = 0;
		conditionCollection = tf_TestParameter_ConditionList(tl->glob->currentSubItemName[siteIndex]);
		for each (condition in conditionCollection)
		{
			if (condition->Name->Contains(VnaDataAnalysisConditionName_PowerRangeStart))
				countPowerRangeStart++;
			else if (condition->Name->Contains(VnaDataAnalysisConditionName_PowerRangeStop))
				countPowerRangeStop++;
			else if (condition->Name->Contains(VnaDataAnalysisConditionName_Power))
				countPower++;
		}

		if ((countPower == countPowerRangeStart) && (countPower == countPowerRangeStop) & (countPowerRangeStart == countPowerRangeStop))
		{
			amb7300tl->vnaDataAnalysisTPC.testPower = gcnew array<double>(countPower);
			amb7300tl->vnaDataAnalysisTPC.testPowerRangeStart = gcnew array<double>(countPowerRangeStart);
			amb7300tl->vnaDataAnalysisTPC.testPowerRangeStop = gcnew array<double>(countPowerRangeStop);

			for (int powerIndex = 0; powerIndex < countPower; powerIndex++)
			{
				//-------------------- Single Power --------------------
				if ((powerIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Power)))
				{
					amb7300tl->vnaDataAnalysisTPC.testPower[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Power);
				}
				else if ((powerIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Power_ + powerIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testPower[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Power_ + powerIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_Power);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Power' or 'Power_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Power' or 'Power_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				//-------------------- Power Range Start --------------------
				if ((powerIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStart)))
				{
					amb7300tl->vnaDataAnalysisTPC.testPowerRangeStart[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStart);
				}
				else if ((powerIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStart_ + powerIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testPowerRangeStart[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStart_ + powerIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_PowerRangeStart);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'PowerRangeStart' or 'PowerRangeStart_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'PowerRangeStart' or 'PowerRangeStart_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				//-------------------- Power Range Stop --------------------
				if ((powerIndex == 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStop)))
				{
					amb7300tl->vnaDataAnalysisTPC.testPowerRangeStop[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStop);
				}
				else if ((powerIndex > 0) && (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStop_ + powerIndex.ToString())))
				{
					amb7300tl->vnaDataAnalysisTPC.testPowerRangeStop[powerIndex] = (double)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_PowerRangeStop_ + powerIndex.ToString());
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_PowerRangeStop);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'PowerRangeStop' or 'PowerRangeStop_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'PowerRangeStop' or 'PowerRangeStop_x' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			// Record for generic use
			amb7300tl->vnaDataAnalysisTPC.overallTestPowerCount = countPower;
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Power);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition set verification] 'Power', 'PowerRangeStart', 'PowerRangeStop' condition name set amount invalid. Should have the same condition name set amount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition set verification] 'Power', 'PowerRangeStart', 'PowerRangeStop' condition name set amount invalid. Should have the same condition name set amount." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'Power' + 'PowerRangeStart' + 'PowerRangeStop'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_Power, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "AnalysisSetting"

		/*
		**	The index usage and variable in array format are reserved for the multiple set of analysis settings in future.
		*/

		//-------------------- AnalysisSetting --------------------
		String ^ analysisSetting = String::Empty;
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_AnalysisSetting))
		{
			analysisSetting = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_AnalysisSetting);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_AnalysisSetting);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'AnalysisSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'AnalysisSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		//-------------------- Handle Analysis Settings For Each DataAnalysis's Type --------------------
		arrStr			= gcnew array<String^>(0);
		separator		= gcnew array<String^>(1);
		separator[0]	= ",";
		arrStr			= analysisSetting->Split(separator, StringSplitOptions::None);
		arrStr2			= gcnew array<String^>(0);
		separator2		= gcnew array<String^>(1);
		separator2[0]	= "/";

		int index = 0;
		for (int i = 0; i < arrStr->Length; i++)
		{
			// TraceData
			if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				// NA
			}
			// mmMinMax
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMax)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> msMinMax'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.msMinMax_SearchRangeEnable	= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.msMinMax_SearchType			= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// msMinMax parameter: SearchRangeEnable (SearchRangeOn | SearchRangeOff), SearchType (Max | Min)
				if (arrStr2->Length == 2)
				{
					amb7300tl->analysisSetting.msMinMax_SearchRangeEnable[index]	= arrStr2[0];
					amb7300tl->analysisSetting.msMinMax_SearchType[index]			= arrStr2[1];
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msMinMax' function contain 2x parameter which is differentiate by '/'." + "\n" +  
													"1st parameter: Enable marker search range."													+ "\n" + 
													"2nd parameter: Select marker search type."														+ "\n" + 
													"Example: SearchRangeOn/Max --> meaning enable marker search range, searching for maximum value.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msMinMax' mode is invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msMinMax' mode is invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// msPeak
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msPeak)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> msPeak'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.msPeak_Type			= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.msPeak_PeakExcursion = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.msPeak_PeakPolarity	= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				/*amb7300tl->analysisSetting.msPeak_PeakThreshold = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);*/
				// msPaek parameter: PeakType (SearchPeak | SearchPeakLeft | SearchPeakRight), PeakExcursion (default 1dB), PeakPolarity (Positive | Negative | Both)
				if (arrStr2->Length == 3)
				{
					amb7300tl->analysisSetting.msPeak_Type[index]			= arrStr2[0];
					amb7300tl->analysisSetting.msPeak_PeakExcursion[index]	= Double::Parse(arrStr2[1]);
					amb7300tl->analysisSetting.msPeak_PeakPolarity[index]	= arrStr2[2];
					/*amb7300tl->analysisSetting.msPeak_PeakThreshold[index] = Double::Parse(arrStr2[3]);*/
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msPeak' function contain 3x parameter which is differentiate by '/'."	+ "\n" +
													"1st parameter: Select marker search peak's type."												+ "\n" + 
													"2nd parameter: Set peak excursion."															+ "\n" + 
													"3rd parameter: Select peak polarity."															+ "\n" + 
													"Example: SearchPeak/1/Positive --> meaning select search peak type, apply 1dB peak excursion, select positive peak polarity.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msPeak' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msPeak' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// msTarget
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msTarget)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> msTarget'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.msTarget_Type				= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.msTarget_TargetValue			= gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.msTarget_TargetTransition	= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				//amb7300tl->analysisSetting.msTarget_TargetLineEnable	= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// msTarget parameter: TargetType (SearchTarget | SearchTargetLeft | SearchTargetRight), TargetValue (default 0dB), TargetTransition (Both | Positive | Negative)
				if (arrStr2->Length == 3)
				{
					amb7300tl->analysisSetting.msTarget_Type[index]				= arrStr2[0];
					amb7300tl->analysisSetting.msTarget_TargetValue[index]		= Double::Parse(arrStr2[1]);
					amb7300tl->analysisSetting.msTarget_TargetTransition[index] = arrStr2[2];
					//amb7300tl->analysisSetting.msTarget_TargetLineEnable[index] = arrStr2[3];
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msTarget' function contain 3x parameter which is differentiate by '/'." + "\n" + 
													"1st parameter: Select marker search target's type."											+ "\n" + 
													"2nd parameter: Set target value."																+ "\n" + 
													"3rd parameter: Select target transition."														+ "\n" +  
													"Example: SearchTarget/-10/Positive --> meaning select search target, apply 10dB target value, select positive target transition.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msTarget' mode is invalid. It should contain 4x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'msTarget' mode is invalid. It should contain 4x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// P1dBTarget
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_P1dBTarget)
			{
			}
			// mmStatistics
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> mmStatistics'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.mmStatistics_StatisticsRangeEnable = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// mmStatistics parameter: StatisticsRangeEnable (StatisticsRangeOn | StatisticsRangeOff)
				if (arrStr2->Length == 1)
				{
					amb7300tl->analysisSetting.mmStatistics_StatisticsRangeEnable[index] = arrStr2[0];
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'mmStatistics' function contain 1x parameter which is differentiate by '/'." + "\n" +  
													"1st parameter: Enable statistics range."															+ "\n" +  
													"Example: StatisticsRangeOn --> meaning enable statistics range.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmStatistics' mode is invalid. It should contain 1x value parameter, without a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmStatistics' mode is invalid. It should contain 1x value parameter, without a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmBwSearch
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> mmBwSearch'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.mmBwSearch_Type				= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_SearchRef			= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_BandwidthValue	= gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_Condition         = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// mmBwSearch parameter: Type (Bandpass | Notch), Search Ref (Marker | Max | Min | Peak), Bandwidth Value (default -3dB)
				
				/*if (arrStr2->Length == 4) //obsolete
				{
					amb7300tl->analysisSetting.mmBwSearch_Type[index]			= arrStr2[0];
					amb7300tl->analysisSetting.mmBwSearch_SearchRef[index]		= arrStr2[1];
					amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
					amb7300tl->analysisSetting.mmBwSearch_Condition[index]      = arrStr2[3];
				}*/
				if (arrStr2->Length == 3)
				{
					amb7300tl->analysisSetting.mmBwSearch_Type[index] = arrStr2[0];
					amb7300tl->analysisSetting.mmBwSearch_SearchRef[index] = arrStr2[1];
					amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
					amb7300tl->analysisSetting.mmBwSearch_Condition[index] = "N/A";
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'mmBwSearch' function contain 3x parameter which is differentiate by '/'."	+ "\n" +  
													"1st parameter: Select search type."																+ "\n" + 
													"2nd parameter: Select search reference."															+ "\n" +  
													"3rd parameter: Set bandwidth value."																+ "\n" +  
													"Example: Bandpass/Marker/-3 --> meaning select bandpass type, select marker as search reference, apply -3dB bandwidth value.\n";           
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmBwSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmBwSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmBwSearchAbs
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> mmBwSearch'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.mmBwSearch_Type = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_SearchRef = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_BandwidthValue = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.mmBwSearch_Condition = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// mmBwSearch parameter: Type (Bandpass | Notch), Search Ref (Marker | Max | Min | Peak), Bandwidth Value (default -3dB)
				
				/*if (arrStr2->Length == 4) //obsolete
				{
					amb7300tl->analysisSetting.mmBwSearch_Type[index] = arrStr2[0];
					amb7300tl->analysisSetting.mmBwSearch_SearchRef[index] = arrStr2[1];
					amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
					amb7300tl->analysisSetting.mmBwSearch_Condition[index] = arrStr2[3];
				}*/
				if (arrStr2->Length == 3)
				{
					amb7300tl->analysisSetting.mmBwSearch_Type[index] = arrStr2[0];
					amb7300tl->analysisSetting.mmBwSearch_SearchRef[index] = arrStr2[1];
					amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
					amb7300tl->analysisSetting.mmBwSearch_Condition[index] = "Abs";
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'mmBwSearchAbs' function contain 3x parameter which is differentiate by '/'." + "\n" +
						"1st parameter: Select search type." + "\n" +
						"2nd parameter: Select search reference." + "\n" +
						"3rd parameter: Set bandwidth value." + "\n" +
						"Example: Bandpass/Marker/-3 --> meaning select bandpass type, select marker as search reference, apply -3dB bandwidth value.\n";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmBwSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'mmBwSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmFlatness
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				// NA
			}
			// msMinMaxRipple
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple || amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				// NA
			}
			// SharedMemoryDataMinMax
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> SharedMemoryDataMinMax'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.SharedMemorySearchRangeEnable = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataMinMax_SearchType = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// msMinMax parameter: SearchRangeEnable (SearchRangeOn | SearchRangeOff), SearchType (Max | Min)
				if (arrStr2->Length == 2)
				{
					amb7300tl->analysisSetting.SharedMemorySearchRangeEnable[index] = arrStr2[0];
					amb7300tl->analysisSetting.SharedMemoryDataMinMax_SearchType[index] = arrStr2[1];
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataMinMax' function contain 2x parameter which is differentiate by '/'." + "\n" +
						"1st parameter: Enable marker search range." + "\n" +
						"2nd parameter: Select marker search type." + "\n" +
						"Example: SearchRangeOn/Max --> meaning enable marker search range, searching for maximum value.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataMinMax' mode is invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataMinMax' mode is invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// SharedMemoryDataTarget
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> SharedMemoryDataTarget'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.SharedMemorySearchRangeEnable = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataTarget_TargetValue = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataTarget_TargetTransition = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				//amb7300tl->analysisSetting.msTarget_TargetLineEnable	= gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				// msTarget parameter: TargetType (SearchTarget | SearchTargetLeft | SearchTargetRight), TargetValue (default 0dB), TargetTransition (Both | Positive | Negative)
				if (arrStr2->Length == 3)
				{
					amb7300tl->analysisSetting.SharedMemorySearchRangeEnable[index] = arrStr2[0];
					amb7300tl->analysisSetting.SharedMemoryDataTarget_TargetValue[index] = Double::Parse(arrStr2[1]);
					amb7300tl->analysisSetting.SharedMemoryDataTarget_TargetTransition[index] = arrStr2[2];
					//amb7300tl->analysisSetting.msTarget_TargetLineEnable[index] = arrStr2[3];
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataTarget' function contain 3x parameter which is differentiate by '/'." + "\n" +
						"1st parameter: Select marker search target's type." + "\n" +
						"2nd parameter: Set target value." + "\n" +
						"3rd parameter: Select target transition." + "\n" +
						"Example: SearchTarget/-10/Positive --> meaning select search target, apply 10dB target value, select positive target transition.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataTarget' mode is invalid. It should contain 4x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataTarget' mode is invalid. It should contain 4x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// SharedMemoryDataBWSearch
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> SharedMemoryDataBwSearch'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.SharedMemorySearchRangeEnable = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Type = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_SearchRef = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_BandwidthValue = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Condition = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
								
				/*if (arrStr2->Length == 4) //Obsolete
				{
					amb7300tl->analysisSetting.mmBwSearch_Type[index] = arrStr2[0];
					amb7300tl->analysisSetting.mmBwSearch_SearchRef[index] = arrStr2[1];
					amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
					amb7300tl->analysisSetting.mmBwSearch_Condition[index] = arrStr2[3];
				}*/
				if (arrStr2->Length == 4)
				{
					amb7300tl->analysisSetting.SharedMemorySearchRangeEnable[index] = arrStr2[0];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Type[index] = arrStr2[1];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_SearchRef[index] = arrStr2[2];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[index] = Double::Parse(arrStr2[3]);
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Condition[index] = "N/A";//原先为 N/A，现修改为。。
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function contain 3x parameter which is differentiate by '/'." + "\n" +
						"1st parameter: Select search range On/Off." + "\n" +
						"2nd parameter: Select search type." + "\n" +
						"3rd parameter: Select search reference." + "\n" +
						"4th parameter: Set bandwidth value." + "\n" +
						"Example: SearchRangeOff/Bandpass/Marker/-3 --> meaning select bandpass type, select marker as search reference, apply -3dB bandwidth value.\n";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataBWSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataBWSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// SharedMemoryDataBWSearch
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> SharedMemoryDataBwSearch'
				ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;

				amb7300tl->analysisSetting.SharedMemorySearchRangeEnable = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Type = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_SearchRef = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_BandwidthValue = gcnew array<double>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);
				amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Condition = gcnew array<String^>(amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount);

				/*if (arrStr2->Length == 4) //Obsolete
				{
				amb7300tl->analysisSetting.mmBwSearch_Type[index] = arrStr2[0];
				amb7300tl->analysisSetting.mmBwSearch_SearchRef[index] = arrStr2[1];
				amb7300tl->analysisSetting.mmBwSearch_BandwidthValue[index] = Double::Parse(arrStr2[2]);
				amb7300tl->analysisSetting.mmBwSearch_Condition[index] = arrStr2[3];
				}*/
				if (arrStr2->Length == 4)
				{
					amb7300tl->analysisSetting.SharedMemorySearchRangeEnable[index] = arrStr2[0];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Type[index] = arrStr2[1];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_SearchRef[index] = arrStr2[2];
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_BandwidthValue[index] = Double::Parse(arrStr2[3]);
					amb7300tl->analysisSetting.SharedMemoryDataBWSearch_Condition[index] = "Abs";
				}
				else
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function contain 3x parameter which is differentiate by '/'." + "\n" +
						"1st parameter: Select search range On/Off." + "\n" +
						"2nd parameter: Select search type." + "\n" +
						"3rd parameter: Select search reference." + "\n" +
						"4th parameter: Set bandwidth value." + "\n" +
						"Example: SearchRangeOff/Bandpass/Marker/-3 --> meaning select bandpass type, select marker as search reference, apply -3dB bandwidth value.\n";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataBWSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value for 'SharedMemoryDataBWSearch' mode is invalid. It should contain 3x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			index++;
		}
		index = 0;

#pragma endregion

#pragma region "SmoothingSetting"

		//-------------------- SmoothingSetting --------------------
		String ^ smoothingSetting = String::Empty;
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_SmoothingSetting))
		{
			smoothingSetting = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_SmoothingSetting);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_SmoothingSetting);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'SmoothingSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'SmoothingSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		arrStr			= gcnew array<String^>(0);
		separator		= gcnew array<String^>(1);
		separator[0]	= "/";
		arrStr			= smoothingSetting->Split(separator, StringSplitOptions::None);

		// Validate condition value input --> 'SmoothingSetting'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_SmoothingSetting, arrStr);
		if (ret != 0) goto EndOfTest;

		// SmoothingSetting parameter: SmoothingEnable (SmoOn | SmoOff), SmoothingAperture (default 1%)
		if (arrStr->Length == 2)
		{
			amb7300tl->smoothingSettings.smoothingEnable	= arrStr[0];
			amb7300tl->smoothingSettings.smoothingAperture	= Double::Parse(arrStr[1]);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
			String ^ additionalMessage =	"'SmoothingSetting' contain 2x parameter which is differentiate by '/'."	+ "\n" +
											"1st parameter: Enable smoothing."											+ "\n" +
											"2nd parameter: Set smoothing aperture."									+ "\n" +
											"Example: SmoOn/5 --> meaning enable smoothing, apply 5% smoothing aperture.";
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_SmoothingSetting, additionalMessage);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

#pragma region "OutputType"

		// [Future Enchancement] The variable's array are reserved for the multiple set of analysis settings but only return one result to TP in future.

		//-------------------- OutputType --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_OutputType))
		{
			amb7300tl->vnaDataAnalysisTPC.outputType = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_OutputType);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_OutputType);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'OutputType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'OutputType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'OutputType'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_OutputType, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "OutputFormat"

		//-------------------- OutputFormat --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_OutputFormat))
		{
			amb7300tl->vnaDataAnalysisTPC.outputFormat = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_OutputFormat);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_OutputFormat);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'OutputFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'OutputFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'OutputFormat'
		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_OutputFormat, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

//#pragma region "Threshold"
//
//		//-------------------- Threshold --------------------
//		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Threshold))
//		{
//			amb7300tl->vnaDataAnalysisTPC.Threshold = (double^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaDataAnalysisConditionName_Threshold);
//		}
//		else
//		{
//			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
//			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaDataAnalysisConditionName_Threshold);
//			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Threshold' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition name verification] 'Threshold' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
//			return ret;
//		}
//
//		// Validate condition value input --> 'Threshold'
//		ret = ValidateConditionValueInput_VnaDataAnalysis(tfSite, siteIndex, VnaDataAnalysisConditionName_OutputFormat, reservedForNA);
//		if (ret != 0) goto EndOfTest;
//
//#pragma endregion
//
	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: VnaSwTime
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> VnaSwTime"

	/// <summary> VnaSwTime
	/// <list type="TestItem" name="TestLib_TestParameter_VnaSwTime" category="VnaSwTime" group="Items" conditiontype="splitbysemicoloncomma" methodname="seq_TestMethod">
	/// <item attribute="name">VnaSwTime_TestItemName</item>
	/// <item attribute="displayas">VnaSwTime_TestItemName</item>
	/// <item attribute="description">This is 'VnaSwTime' item with the combination of test item and test parameter, which is to execute test function and return result to test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaSwTime" category="VnaSwTime" group="Items">
	/// <item attribute="name">VnaSwTime_TestParameterName_S11</item>
	/// <item attribute="displayas">VnaSwTime_TestParameterName_S11</item>
	/// <item attribute="description">VnaSwTime</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaSwTime" category="VnaSwTime" group="Items">
	/// <item attribute="name">VnaSwTime_TestParameterName_S21</item>
	/// <item attribute="displayas">VnaSwTime_TestParameterName_S21</item>
	/// <item attribute="description">VnaSwTime</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">2</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaSwTime" category="VnaSwTime" group="Items">
	/// <item attribute="name">VnaSwTime_TestParameterName_S12</item>
	/// <item attribute="displayas">VnaSwTime_TestParameterName_S12</item>
	/// <item attribute="description">VnaSwTime</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">3</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// <list type="TestParameter" name="TestLib_TestParameter_VnaSwTime" category="VnaSwTime" group="Items">
	/// <item attribute="name">VnaSwTime_TestParameterName_S22</item>
	/// <item attribute="displayas">VnaSwTime_TestParameterName_S22</item>
	/// <item attribute="description">VnaSwTime</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit">dB</item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">TraceIndex</item>
	/// <item attribute="description">Specifies the target trace for this particular measurement.</item>
	/// <item attribute="value">4</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">Int32</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">Function</item>
	/// <item attribute="description">Specifies the test function to perform the data analysis.</item>
	/// <item attribute="value">TraceData</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">AnalysisSetting</item>
	/// <item attribute="description">Specifies the settings for the specific data analysis function. Refer to the user manual for more information.| Remain empty if it is using default setting or not applicable.</item>
	/// <item attribute="value"></item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">SmoothingSetting</item>
	/// <item attribute="description">To enable the smoothing on the target trace. | Specifies the smoothing aperture.</item>
	/// <item attribute="value">SmoOff/1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">Freq</item>
	/// <item attribute="description">Specifies the single test frequency or single marker frequency. | Remain 0 if not applicable.</item>
	/// <item attribute="value">1000</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStart</item>
	/// <item attribute="description">Specifies the start frequency (freq range start). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition" excelname="">
	/// <item attribute="name">FreqRangeStop</item>
	/// <item attribute="description">Specifies the stop frequency (freq range stop). | Remain 0 if not applicable.</item>
	/// <item attribute="value">0</item>
	/// <item attribute="prefix">Mega</item>
	/// <item attribute="unit">Hz</item>
	/// <item attribute="datatype">Double</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputType</item>
	/// <item attribute="description">Specifies the result output type. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Data</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">OutputFormat</item>
	/// <item attribute="description">Specifies the result output format. Refer to user manual for more information. | Remain empty if not applicable.</item>
	/// <item attribute="value">Real</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>

#pragma endregion
	int TestProgram::TestLib_TestParameter_VnaSwTime(Site ^ site)
	{
		/* 
		** Dummy test parameter function.
		** To generate Project Library's test parameter item, and let techFlow3 user to drag into the test recipes editor.
		** Only applicable for techFlow3 user (using test recipes editor).
		** It will link to the all-in-one generic Test Method --> seq_TestMethod() 
		*/

		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_VnaSwTime_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_TestParameter_VnaSwTime_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the test parameter item 'VnaSwTime'.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		array<String^> ^ reservedForNA				= nullptr;
		array<String^> ^ arrStr						= gcnew array<String^>(0);
		array<String^> ^ separator					= gcnew array<String^>(1);
		array<String^> ^ arrStr2					= gcnew array<String^>(0);
		array<String^> ^ separator2					= gcnew array<String^>(1);
		ConditionCollection ^ conditionCollection	= gcnew ConditionCollection;
		Condition ^ condition;

#pragma region "MeasureType"

		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_MeasureType))
		{
			amb7300tl->vnaSwTimeTPC.measureType = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_MeasureType);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_MeasureType);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'MeasureType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'MeasureType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MeasureType'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_MeasureType, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "DMModuleAlias"

		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMModuleAlias))
		{
			amb7300tl->vnaSwTimeTPC.dmModuleAlias = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMModuleAlias);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_DMModuleAlias);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMModuleAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMModuleAlias' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'DMVectorTrigger'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_DMModuleAlias, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "DMVectorTrigger"

		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMVectorTrigger))
		{
			amb7300tl->vnaSwTimeTPC.dmVectorTrigger = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMVectorTrigger);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_DMVectorTrigger);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMVectorTrigger' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMVectorTrigger' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'DMVectorTrigger'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_DMVectorTrigger, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "AnalysisSetting"

		/*
		**	The index usage and variable in array format are reserved for the multiple set of analysis settings in future.
		*/

		//-------------------- AnalysisSetting --------------------
		String ^ analysisSetting = String::Empty;
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_AnalysisSetting))
		{
			analysisSetting = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_AnalysisSetting);
		}
		else
		{
			ret = ER_CONST_VNADATAANALYSIS_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_AnalysisSetting);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'AnalysisSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'AnalysisSetting' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		//-------------------- Handle Analysis Settings For Each DataAnalysis's Type --------------------
		arrStr			= gcnew array<String^>(0);
		separator		= gcnew array<String^>(1);
		separator[0]	= ",";
		arrStr			= analysisSetting->Split(separator, StringSplitOptions::None);
		arrStr2			= gcnew array<String^>(0);
		separator2		= gcnew array<String^>(1);
		separator2[0]	= "/";

		for (int i = 0; i < arrStr->Length; i++)
		{
			// TraceData
			if (amb7300tl->vnaSwTimeTPC.measureType == Cond_MeasureType_DM_PXIE9852)
			{
				arrStr2 = gcnew array<String^>(0);
				arrStr2 = arrStr[i]->Split(separator2, StringSplitOptions::None);

				// Validate condition value input --> 'AnalysisSetting -> msMinMax'
				ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_AnalysisSetting, arrStr2);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "DMVectorTrigger"

		//-------------------- SmoothingSetting --------------------
		String ^ dmVectorTrigg = String::Empty;
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMVectorTrigger))
		{
			dmVectorTrigg = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_DMVectorTrigger);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_DMVectorTrigger);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMVectorTrigger' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'DMVectorTrigger' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'SmoothingSetting'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_DMVectorTrigger, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "OutputType"

		// [Future Enchancement] The variable's array are reserved for the multiple set of analysis settings but only return one result to TP in future.

		//-------------------- OutputType --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_OutputType))
		{
			amb7300tl->vnaSwTimeTPC.outputType = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_OutputType);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_OutputType);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'OutputType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'OutputType' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'OutputType'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_OutputType, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "OutputFormat"

		//-------------------- OutputFormat --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_OutputFormat))
		{
			amb7300tl->vnaSwTimeTPC.outputFormat = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], VnaSwTimeConditionName_OutputFormat);
		}
		else
		{
			ret = ER_CONST_VNASWTIME_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, VnaSwTimeConditionName_OutputFormat);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'OutputFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition name verification] 'OutputFormat' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'OutputFormat'
		ret = ValidateConditionValueInput_VnaSwTime(tfSite, siteIndex, VnaSwTimeConditionName_OutputFormat, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		return ret;
	}
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Parameter: Math
	**	----------------------------------------------------------------------------------------------------
	*/
#pragma region ".xml file -> Test Parameter -> Math"
	/// <summary> Math
	/// <list type="TestItem" name="TestLib_TestParameter_Math" category="Math" group="Items" conditiontype="splitbysemicolon" methodname="seq_TestMethod">
	/// <item attribute="name">Math_TestItemName</item>
	/// <item attribute="displayas">Math_TestItemName</item>
	/// <item attribute="description">This is 'Math' item with the combination of test item and test parameter, which is to execute the test function and return result to the particular test parameter.</item>
	/// <list type="TestParameter" name="TestLib_TestParameter_DcTest" category="DcTest" group="Items">
	/// <item attribute="name">Math_TestParameterName</item>
	/// <item attribute="displayas">Math_TestParameterName</item>
	/// <item attribute="description">Math</item>
	/// <item attribute="datatype">Double</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="evalmode">Between_IncludeMinAndMax</item>
	/// <item attribute="lolimit">-999</item>
	/// <item attribute="lolimitprefix">None</item>
	/// <item attribute="hilimit">999</item>        
	/// <item attribute="hilimitprefix">None</item>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">MathFunction</item>
	/// <item attribute="description">Specifies the calculation type. TP1 Operates TP2. For example, using Add will yield the results of TP1 + TP2, Subtract the will yield the result of TP1 - TP2.</item>
	/// <item attribute="value">Add</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">MathTP1</item>
	/// <item attribute="description">Specifies the TP1 used for calculation.</item>
	/// <item attribute="value">TP1</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>
	/// <list type="TestParameterCondition">
	/// <item attribute="name">MathTP2</item>
	/// <item attribute="description">Specifies the TP2 used for calculation.</item>
	/// <item attribute="value">TP2</item>
	/// <item attribute="prefix">None</item>
	/// <item attribute="unit"></item>
	/// <item attribute="datatype">String</item>
	/// </list>	
	/// </list>
	/// </list>
	/// </summary>
	/// <param name="site">This is techFlow site object.</param>
#pragma endregion
	int TestProgram::TestLib_TestParameter_Math(Site ^ site)
	{
		// Local variable
		int ret = 0;

		ret = seq_TestMethod(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}
	int TestProgram::TestLib_TestParameter_Math_CastCondition(Site ^ site, int tfSite, int siteIndex)
	{
		/*****************************************************************************************************
		**	TestLib_TestParameter_VnaDataAnalysis_CastCondition
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**
		**	Descriptions:
		**		This function is to cast condition from the test parameter item 'Math'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		array<String^> ^ reservedForNA = nullptr;

#pragma region "MathFunction"

		//-------------------- MathFunction --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], MathConditionName_Function))
		{
			amb7300tl->MathTPC.mathFunction = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], MathConditionName_Function);
		}
		else
		{
			ret = ER_CONST_MATH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, MathConditionName_Function);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathFunction' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MathFunction'
		ret = ValidateConditionValueInput_Math(tfSite, siteIndex, MathConditionName_Function, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "MathTP1"

		//-------------------- MathFunction --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], MathConditionName_TP1))
		{
			amb7300tl->MathTPC.mathTP1 = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], MathConditionName_TP1);
		}
		else
		{
			ret = ER_CONST_MATH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, MathConditionName_TP1);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathConditionName_TP1' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathConditionName_TP1' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MathFunction'
		ret = ValidateConditionValueInput_Math(tfSite, siteIndex, MathConditionName_TP1, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "MathTP2"

		//-------------------- MathFunction --------------------
		if (tf_TestParameter_ConditionExist(tl->glob->currentSubItemName[siteIndex], MathConditionName_TP2))
		{
			amb7300tl->MathTPC.mathTP2 = (String^)tf_TestParameter_ConditionCast(tl->glob->currentSubItemName[siteIndex], MathConditionName_TP2);
		}
		else
		{
			ret = ER_CONST_MATH_CONDITION_NAME_NOT_FOUND;
			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_CONDITION_NAME_NOT_FOUND, MathConditionName_TP2);
			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathConditionName_TP2' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition name verification] 'MathConditionName_TP2' condition name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Validate condition value input --> 'MathFunction'
		ret = ValidateConditionValueInput_Math(tfSite, siteIndex, MathConditionName_TP2, reservedForNA);
		if (ret != 0) goto EndOfTest;

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> DcControl
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_DcControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int totalConfigurationSets)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_DcControl
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**	totalConfigurationSets	- This is the total sets of configuration.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'DcControl'.
		**		Include: 'Module', 'PinAlias', 'ControlFunction', 'NPLC', 'VoltageLevel', 'CurrentLevel'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "Module"

		if (conditionName == DcControlConditionName_Module)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				if ((amb7300tl->DcControlCSC.module[i] != Cond_Module_CM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_AM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_DM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_IOM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_ACM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_TM))
				{
					ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'Module' available settings: " + "\n" +
													"-> CM"							+ "\n" +
													"-> AM"							+ "\n" +
													"-> DM"							+ "\n" +
													"-> IOM"						+ "\n" +
													"-> ACM"						+ "\n" + 
													"-> TMU";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_Module, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "PinAlias"
	
		if (conditionName == DcControlConditionName_PinAlias)
		{

			int siteIndexTemp = 0;
			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				siteIndexTemp = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				siteIndexTemp = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}

			// Checker
			array<bool> ^ isPinAliasMatch = gcnew array<bool>(totalConfigurationSets);
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				isPinAliasMatch[i] = false;
			}

			// Compare with all the pin map name inside Resource Planner
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_CM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.cmPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_CM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}

				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_AM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.amPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}

				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_DM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.dmPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}

				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_IOM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.iomPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_IOM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}

				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_ACM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.acmPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_ACM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}

				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_TM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.tmPinCount; j++)
					{
						if (amb7300tl->DcControlCSC.pinAlias[i] == tl->glob->RsrcManager.pinAlias_TM_PinMapName[siteIndexTemp, j])
						{
							isPinAliasMatch[i] = true;
							break;
						}
					}
				}
			}

			// Identify the checker
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				if (isPinAliasMatch[i] == false)
				{
					ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'PinAlias' must exist inside the resource planner." + "\n" + 
													"The 'PinAlias' must be the same as the 'Pin Map Name' inside the Reource Planner.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_PinAlias, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'PinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'PinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "ControlFunction"

		if (conditionName == DcControlConditionName_ControlFunction)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_CM)
				{
					// [Future Enchancement] To list down control function available for CM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_AM)
				{
					if ((amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_AM_DVCI) &&
						(amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_AM_DICV))
					{
						ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'ControlFunction' for AM resource, the available settings: "	+ "\n" +
														"-> DVCI"														+ "\n" +
														"-> DICV";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_ControlFunction, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_DM)
				{
					if ((amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_DM_PMU_DVCI)	&&
						(amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_DM_PMU_DICV)	&&
						(amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_DM_PE)			&&
						(amb7300tl->DcControlCSC.controlFunction[i] != Cond_ControlFunction_DM_DIO))
					{
						ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'ControlFunction' for DM resource, the available settings: "	+ "\n" +
														"-> DVCI"														+ "\n" +
														"-> DICV"														+ "\n" +
														"-> PE"															+ "\n" +
														"-> DIO";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_ControlFunction, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_IOM)
				{
					// [Future Enchancement] To list down control function available for IOM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_ACM)
				{
					// [Future Enchancement] To list down control function available for ACM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_TM)
				{
					// [Future Enchancement] To list down control function available for TM.
				}
			}
		}

#pragma endregion

#pragma region "NPLC"

		if (conditionName == DcControlConditionName_NPLC)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{	
				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_CM)
				{
					// [Future Enchancement] To list down nplc available for CM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_AM)
				{
					// Get the setting boundary from the DcPwr API list
					if ((amb7300tl->DcControlCSC.nplc[i] <= 0.001) || 
						(amb7300tl->DcControlCSC.nplc[i] >= 10))
					{
						ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'NPLC' for AM resource, the available settings: "	+ "\n" +
														"-> 0.001 < NPLC < 10";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_NPLC, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_DM)
				{
					// Get the setting boundary from the DcPwr API list
					if ((amb7300tl->DcControlCSC.nplc[i] <= 0.001) || 
						(amb7300tl->DcControlCSC.nplc[i] >= 10))
					{
						ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'NPLC' for DM resource, the available settings: "	+ "\n" +
														"-> 0.001 < NPLC < 10";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_NPLC, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_IOM)
				{
					// [Future Enchancement] To list down nplc available for IOM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_ACM)
				{
					// [Future Enchancement] To list down nplc available for ACM.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_TM)
				{
					// [Future Enchancement] To list down nplc available for TM.
				}
			}
		}

#pragma endregion

#pragma region "CurrentLevel"

		if (conditionName == DcControlConditionName_CurrentLevel)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_CM)
				{
					// [Future Enchancement] To list down current range available for CM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_AM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.amPinCount; j++)
					{
						if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM430e))
						{
							/*
							**	1,2,3,4 quadrant (equal)
							**	[-100.0mA ~ +100.0mA | -10.0V ~ +10.0V]
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < AM_CONST_AM430e_CURRENT_RANGE_NEG_100MA) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > AM_CONST_AM430e_CURRENT_RANGE_POS_100MA))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for AM430e resource, the available settings: "	+ "\n" +
																"-> 1,2,3,4 quadrant (equal): "									+ "\n" + 
																"-> [-100.0mA ~ +100.0mA | -10.0V ~ +10.0V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM450e))
						{
							// [Future Enchancement] 1A pulse current will not been included in this library for now.
							// [Future Enchancement] Quadrant 2: 1.4Vo @ 1A sink envelope behaviour will not been included in this library for now.

							/*
							**	1,2 quadrant
							**	[-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]
							**	[-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]
							**	[-1.0A ~ +1.0A | 0.0V ~ +10.0V]
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < AM_CONST_AM450e_CURRENT_RANGE_NEG_1A) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > AM_CONST_AM450e_CURRENT_RANGE_POS_1A))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for AM450e resource, the available settings: "	+ "\n" +
																"-> 1,2 quadrant: "												+ "\n" + 
																"-> [-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]"						+ "\n" +
																"-> [-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]"						+ "\n" +
																"-> [-1.0A ~ +1.0A | 0.0V ~ +10.0V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM451e))
						{
							// [Future Enchancement] 10A pulse current will not been included in this library for now.

							/*
							**	1,2,3,4 quadrant
							**	[-0.228A ~ +0.228A | -48.0V ~ +48.0V]
							**	[-0.428A ~ +0.428A | -30.0V ~ +30.0V]
							**	[-0.625A ~ +0.625A | -24.0V ~ +24.0V]
							**	[-1.0A ~ +1.0A | -10.0V ~ +10.0V]
							**	[-2.0A ~ +2.0A | -6.0V ~ +6.0V]
							**	[0.0A ~ +3.0A | -1.0V ~ +1.0V]
							**	[-10.0A ~ +10.0A | -10.0V ~ +10.0V] (pulse)
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < AM_CONST_AM451e_CURRENT_RANGE_NEG_2A_SPEC_DESC) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > AM_CONST_AM451e_CURRENT_RANGE_POS_3A))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for AM451e resource, the available settings: "	+ "\n" +
																"-> 1,2,3,4 quadrant: "											+ "\n" + 
																"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM471e))
						{
							// [Future Enchancement] Quadrant 2: 1.4Vo @ 1A sink envelope behaviour will not been included in this library for now.

							/*
							**	1,2 quadrant
							**	[-100.0mA ~ +100.0mA | 0.0V ~ +10.0V]
							**	[-3.0A ~ +3.0A | 0.0V ~ +6.0V]
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < AM_CONST_AM471e_CURRENT_RANGE_NEG_3A) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > AM_CONST_AM471e_CURRENT_RANGE_POS_3A))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for AM471e resource, the available settings: "	+ "\n" +
																"-> 1,2 quadrant: "												+ "\n" + 
																"-> [-100.0mA ~ +100.0mA | 0.0V ~ +10.0V]"						+ "\n" +
																"-> [-3.0A ~ +3.0A | 0.0V ~ +6.0V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_DM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.dmPinCount; j++)
					{
						if (tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM481e))
						{
							// [Future Enchancement] Sink envelope behaviour will not been included in this library for now.

							/*
							**	-
							**	[-32.0mA ~ +32.0mA | -2.5V ~ +10.5V]
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < DM_CONST_DM481e_CURRENT_RANGE_NEG_32MA) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > DM_CONST_DM481e_CURRENT_RANGE_POS_32MA))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for DM481e resource, the available settings: "	+ "\n" +
																//"-> 1,2 quadrant: "											+ "\n" + 
																"-> [-32.0mA ~ +32.0mA | -2.5V ~ +10.5V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if ((tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM482e)) ||
								(tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM483e)))
						{
							// [Future Enchancement] Sink envelope behaviour will not been included in this library for now.

							/*
							**	-
							**	[-25.0mA ~ +25.0mA | -2.0V ~ +6.0V]
							*/

							//if (amb7300tl->DcControlCSC.currentLevel[i] < 0.0)
							//{
							//	amb7300tl->DcControlCSC.currentLevel[i] = amb7300tl->DcControlCSC.currentLevel[i] * (-1.0);
							//}

							if ((amb7300tl->DcControlCSC.currentLevel[i] < DM_CONST_DM482e_DM483e_CURRENT_RANGE_NEG_25MA) ||
								(amb7300tl->DcControlCSC.currentLevel[i] > DM_CONST_DM482e_DM483e_CURRENT_RANGE_POS_25MA))
							{
								ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"'CurrentLevel' for DM482e/DM483e resource, the available settings: "	+ "\n" +
																//"-> 1,2 quadrant: "													+ "\n" + 
																"-> [-25.0mA ~ +25.0mA | -2.0V ~ +6.0V]";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_CurrentLevel, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'CurrentLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_IOM)
				{
					// [Future Enchancement] To list down current range available for IOM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_ACM)
				{
					// [Future Enchancement] To list down current range available for ACM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_TM)
				{
					// [Future Enchancement] To list down current range available for TM series.
				}
			}
		}

#pragma endregion

#pragma region "VoltageLevel"

		if (conditionName == DcControlConditionName_VoltageLevel)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{	
				if (amb7300tl->DcControlCSC.module[i] == Cond_Module_CM)
				{
					// [Future Enchancement] To list down voltage range available for CM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_AM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.amPinCount; j++)
					{
						if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM430e))
						{
							/*
							**	1,2,3,4 quadrant (equal)
							**	[-100.0mA ~ +100.0mA | -10.0V ~ +10.0V]
							*/
							
							if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM430e_CURRENT_RANGE_NEG_100MA) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM430e_CURRENT_RANGE_POS_100MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM430e_VOLTAGE_RANGE_NEG_10V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM430e_VOLTAGE_RANGE_POS_10V))
								{ 
										ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
										String ^ additionalMessage =	"'VoltageLevel' for AM430e resource, the available settings: "	+ "\n" +
																		"-> 1,2,3,4 quadrant (equal): "									+ "\n" + 
																		"-> [-100.0mA ~ +100.0mA | -10.0V ~ +10.0V]";
										amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
										tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
										tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
										return ret;
								}
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM450e))
						{
							// [Future Enchancement] 1A pulse current will not been included in this library for now.
							// [Future Enchancement] Quadrant 2: 1.4Vo @ 1A sink envelope behaviour will not been included in this library for now.

							/*
							**	1,2 quadrant
							**	[-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]
							**	[-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]
							**	[-1.0A ~ +1.0A | 0.0V ~ +10.0V]
							*/

							if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM450e_CURRENT_RANGE_NEG_10MA) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM450e_CURRENT_RANGE_POS_10MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM450e_VOLTAGE_RANGE_POS_0V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM450e_VOLTAGE_RANGE_POS_50V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM450e resource, the available settings: "	+ "\n" +
																	"-> 1,2 quadrant: "												+ "\n" + 
																	"-> [-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]"						+ "\n" +
																	"-> [-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | 0.0V ~ +10.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM450e_CURRENT_RANGE_NEG_100MA) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM450e_CURRENT_RANGE_POS_100MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM450e_VOLTAGE_RANGE_POS_0V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM450e_VOLTAGE_RANGE_POS_48V_SPEC_DESC))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM450e resource, the available settings: "	+ "\n" +
																	"-> 1,2 quadrant: "												+ "\n" + 
																	"-> [-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]"						+ "\n" +
																	"-> [-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | 0.0V ~ +10.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM450e_CURRENT_RANGE_NEG_1A) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM450e_CURRENT_RANGE_POS_1A))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM450e_VOLTAGE_RANGE_POS_0V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM450e_VOLTAGE_RANGE_POS_10V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM450e resource, the available settings: "	+ "\n" +
																	"-> 1,2 quadrant: "												+ "\n" + 
																	"-> [-10.0mA ~ +10.0mA | 0.0V ~ +50.0V]"						+ "\n" +
																	"-> [-100.0mA ~ +100.0mA | 0.0V ~ +48.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | 0.0V ~ +10.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM451e))
						{
							// [Future Enchancement] 10A pulse current will not been included in this library for now.
						
							/*
							**	1,2,3,4 quadrant
							**	[-0.228A ~ +0.228A | -48.0V ~ +48.0V]
							**	[-0.428A ~ +0.428A | -30.0V ~ +30.0V]
							**	[-0.625A ~ +0.625A | -24.0V ~ +24.0V]
							**	[-1.0A ~ +1.0A | -10.0V ~ +10.0V]
							**	[-2.0A ~ +2.0A | -6.0V ~ +6.0V]
							**	[0.0A ~ +3.0A | -1.0V ~ +1.0V]
							**	[-10.0A ~ +10.0A | -10.0V ~ +10.0V] (pulse)
							*/

							if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_NEG_0p228A_SPEC_DESC) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_0p228A_SPEC_DESC))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_48V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_48V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_NEG_0p428A_SPEC_DESC) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_0p428A_SPEC_DESC))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_30V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_30V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_NEG_0p625A_SPEC_DESC) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_0p625A_SPEC_DESC))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_24V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_24V_SPEC_DESC))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_NEG_1A) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_1A))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_10V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_10V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_NEG_2A_SPEC_DESC) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_2A_SPEC_DESC))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_6V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_6V_SPEC_DESC))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}					
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM451e_CURRENT_RANGE_POS_0A_SPEC_DESC) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM451e_CURRENT_RANGE_POS_3A))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM451e_VOLTAGE_RANGE_NEG_1V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM451e_VOLTAGE_RANGE_POS_1V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM451e resource, the available settings: "	+ "\n" +
																	"-> 1,2,3,4 quadrant: "											+ "\n" +
																	"-> [-0.228A ~ +0.228A | -48.0V ~ +48.0V]"						+ "\n" +
																	"-> [-0.428A ~ +0.428A | -30.0V ~ +30.0V]"						+ "\n" +
																	"-> [-0.625A ~ +0.625A | -24.0V ~ +24.0V]"						+ "\n" +
																	"-> [-1.0A ~ +1.0A | -10.0V ~ +10.0V]"							+ "\n" +
																	"-> [-2.0A ~ +2.0A | -6.0V ~ +6.0V]"							+ "\n" +
																	"-> [0.0A ~ +3.0A | -1.0V ~ +1.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
						}
						else if (tl->glob->RsrcManager.pinAlias_AM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_AM471e))
						{
							// [Future Enchancement] Quadrant 2: 1.4Vo @ 1A sink envelope behaviour will not been included in this library for now.

							/*
							**	1,2 quadrant
							**	[-100.0mA ~ +100.0mA | 0.0V ~ +10.0V]
							**	[-3.0A ~ +3.0A | 0.0V ~ +6.0V]
							*/

							if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM471e_CURRENT_RANGE_NEG_100MA) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM471e_CURRENT_RANGE_POS_100MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM471e_VOLTAGE_RANGE_POS_0V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM471e_VOLTAGE_RANGE_POS_10V))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM471e resource, the available settings: "	+ "\n" +
																	"-> 1,2 quadrant: "												+ "\n" +
																	"-> [-100.0mA ~ +100.0mA | 0.0V ~ +10.0V]"						+ "\n" +
																	"-> [-3.0A ~ +3.0A | 0.0V ~ +6.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else if ((amb7300tl->DcControlCSC.currentLevel[i] >= AM_CONST_AM471e_CURRENT_RANGE_NEG_3A) &&
									(amb7300tl->DcControlCSC.currentLevel[i] <= AM_CONST_AM471e_CURRENT_RANGE_POS_3A))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < AM_CONST_AM471e_VOLTAGE_RANGE_POS_0V_SPEC_DESC) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > AM_CONST_AM471e_VOLTAGE_RANGE_POS_6V_SPEC_DESC))
								{
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for AM471e resource, the available settings: "	+ "\n" +
																	"-> 1,2 quadrant: "												+ "\n" +
																	"-> [-100.0mA ~ +100.0mA | 0.0V ~ +10.0V]"						+ "\n" +
																	"-> [-3.0A ~ +3.0A | 0.0V ~ +6.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
						}
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_DM)
				{
					for (int j = 0; j < tl->glob->RsrcManager.dmPinCount; j++)
					{
						if (tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM481e))
						{
							// [Future Enchancement] Sink envelope behaviour will not been included in this library for now.

							/*
							**	-
							**	[-32.0mA ~ +32.0mA | -2.5V ~ +10.5V]
							*/
							
							if ((amb7300tl->DcControlCSC.currentLevel[i] >= DM_CONST_DM481e_CURRENT_RANGE_NEG_32MA) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= DM_CONST_DM481e_CURRENT_RANGE_POS_32MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < DM_CONST_DM482e_DM483e_VOLTAGE_RANGE_NEG_2p5V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > DM_CONST_DM482e_DM483e_VOLTAGE_RANGE_POS_10p5V))
								{ 
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for DM481e resource, the available settings: "	+ "\n" +
																	//"-> 1,2 quadrant: "											+ "\n" + 
																	"-> [-32.0mA ~ +32.0mA | -2.5V ~ +10.5V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
						}
						else if ((tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM482e)) ||
								(tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndex, j]->Contains(ModuleName_DM483e)))
						{
							// [Future Enchancement] Sink envelope behaviour will not been included in this library for now.

							/*
							**	-
							**	[-25.0mA ~ +25.0mA | -2.0V ~ +6.0V]
							*/
							
							if ((amb7300tl->DcControlCSC.currentLevel[i] >= DM_CONST_DM482e_DM483e_CURRENT_RANGE_NEG_25MA) &&
								(amb7300tl->DcControlCSC.currentLevel[i] <= DM_CONST_DM482e_DM483e_CURRENT_RANGE_POS_25MA))
							{
								if ((amb7300tl->DcControlCSC.voltageLevel[i] < DM_CONST_DM482e_DM483e_VOLTAGE_RANGE_NEG_2V) ||
									(amb7300tl->DcControlCSC.voltageLevel[i] > DM_CONST_DM482e_DM483e_VOLTAGE_RANGE_POS_6V))
								{ 
									ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'VoltageLevel' for DM482e/DM483e resource, the available settings: "	+ "\n" +
																	//"-> 1,2 quadrant: "													+ "\n" + 
																	"-> [-25.0mA ~ +25.0mA | -2.0V ~ +6.0V]";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_VoltageLevel, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'VoltageLevel' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
						}
					}
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_IOM)
				{
					// [Future Enchancement] To list down voltage range available for IOM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_ACM)
				{
					// [Future Enchancement] To list down voltage range available for ACM series.
				}
				else if (amb7300tl->DcControlCSC.module[i] == Cond_Module_TM)
				{
					// [Future Enchancement] To list down voltage range available for TM series.
				}
			}
		}

#pragma endregion

#pragma region "Delay"

		// No need to check delay

#pragma endregion

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> DcTest
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_DcTest(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_DcTest
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'DcTest'.
		**		Include: 'Module', 'PinAlias', 'TestFunction', 'NPLC', 'MeasureDelay'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "Module"

		if (conditionName == DcTestConditionName_Module)
		{
			if ((amb7300tl->DcTestTPC.module != Cond_Module_CM)		&&
				(amb7300tl->DcTestTPC.module != Cond_Module_AM)		&&
				(amb7300tl->DcTestTPC.module != Cond_Module_DM)		&&
				(amb7300tl->DcTestTPC.module != Cond_Module_IOM)	&&
				(amb7300tl->DcTestTPC.module != Cond_Module_ACM)	&&
				(amb7300tl->DcTestTPC.module != Cond_Module_TM))
			{
				ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'Module' available settings: " + "\n" +
												"-> CM"							+ "\n" +
												"-> AM"							+ "\n" +
												"-> DM"							+ "\n" +
												"-> IOM"						+ "\n" +
												"-> ACM"						+ "\n" + 
												"-> TMU";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_Module, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "PinAlias"

		if (conditionName == DcTestConditionName_PinAlias)
		{
			int siteIndexTemp = 0;
			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				siteIndexTemp = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				siteIndexTemp = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}

			// Checker
			bool isPinAliasMatch = false;

			// Compare with all the pin map name inside Resource Planner
			if (amb7300tl->DcTestTPC.module == Cond_Module_CM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.cmPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_CM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			if (amb7300tl->DcTestTPC.module == Cond_Module_AM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.amPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_AM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			if (amb7300tl->DcTestTPC.module == Cond_Module_DM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.dmPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			if (amb7300tl->DcTestTPC.module == Cond_Module_IOM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.iomPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_IOM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			if (amb7300tl->DcTestTPC.module == Cond_Module_ACM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.acmPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_ACM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			if (amb7300tl->DcTestTPC.module == Cond_Module_TM)
			{
				for (int j = 0; j < tl->glob->RsrcManager.tmPinCount; j++)
				{
					if (amb7300tl->DcTestTPC.pinAlias == tl->glob->RsrcManager.pinAlias_TM_PinMapName[siteIndexTemp, j])
					{
						isPinAliasMatch = true;
						break;
					}
				}
			}

			// Identify the checker
			if (isPinAliasMatch == false)
			{
				ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'PinAlias' must exist inside the resource planner." + "\n" + 
												"The 'PinAlias' must be the same as the 'Pin Map Name' inside the Reource Planner.";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_PinAlias, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'PinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'PinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "TestFunction"

		if (conditionName == DcTestConditionName_TestFunction)
		{
			if (amb7300tl->DcTestTPC.module == Cond_Module_CM)
			{
				// [Future Enchancement] To list down test function available for CM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_AM)
			{
				if ((amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_OS)				&&
					(amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_MeasureCurrent) &&
					(amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_MeasureVoltage))
				{
					ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'TestFunction' for AM resource, the available settings: "	+ "\n" +
													"-> OS"														+ "\n" +
													"-> MeasureCurrent"											+ "\n" +
													"-> MeasureVoltage";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_TestFunction, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'TestFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'TestFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_DM)
			{
				if ((amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_OS)				&&
					(amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_MeasureCurrent) &&
					(amb7300tl->DcTestTPC.testFunction != Cond_TestFunction_MeasureVoltage))
				{
					ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'TestFunction' for DM resource, the available settings: "	+ "\n" +
													"-> OS"														+ "\n" +
													"-> MeasureCurrent"											+ "\n" +
													"-> MeasureVoltage";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_TestFunction, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'TestFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'TestFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_IOM)
			{
				// [Future Enchancement] To list down test function available for IOM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_ACM)
			{
				// [Future Enchancement] To list down test function available for ACM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_TM)
			{
				// [Future Enchancement] To list down test function available for TM.
			}
		}

#pragma endregion

#pragma region "NPLC"

		if (conditionName == DcTestConditionName_NPLC)
		{
			if (amb7300tl->DcTestTPC.module == Cond_Module_CM)
			{
				// [Future Enchancement] To list down nplc available for CM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_AM)
			{
				// Get the setting boundary from the DcPwr API list
				if ((amb7300tl->DcTestTPC.nplc <= 0.001) ||
					(amb7300tl->DcTestTPC.nplc >= 10))
				{
					ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'NPLC' for AM resource, the available settings: "	+ "\n" +
													"-> 0.001 < NPLC < 10";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_NPLC, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_DM)
			{
				// Get the setting boundary from the DcPwr API list
				if ((amb7300tl->DcTestTPC.nplc <= 0.001) ||
					(amb7300tl->DcTestTPC.nplc >= 10))
				{
					ret = ER_CONST_DCTEST_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'NPLC' for DM resource, the available settings: "	+ "\n" +
													"-> 0.001 < NPLC < 10";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcTestConditionName_NPLC, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcTest' condition value verification] 'NPLC' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_IOM)
			{
				// [Future Enchancement] To list down nplc available for IOM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_ACM)
			{
				// [Future Enchancement] To list down nplc available for ACM.
			}
			else if (amb7300tl->DcTestTPC.module == Cond_Module_TM)
			{
				// [Future Enchancement] To list down nplc available for TM.
			}
		}

#pragma endregion

#pragma region "MeasureDelay"

		// No need to check measure delay

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> PatternControl
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_PatternControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_PatternControl
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'PatternControl'.
		**		Include: 'ControlFunction', 'isMultiVecToOneResult', 'isOneVecToMultiResult', 
		**		'MipiFreq', 'DmTerminationVoltage', 'DmInputChDelay', 'RunVectorDelay', 'VectorFileName'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "ControlFunction"

		if (conditionName == PatternControlConditionName_ControlFunction)
		{
			if ((amb7300tl->PatternControlCSC.controlFunction != Cond_ControlFunction_MipiWrite)	&&
				(amb7300tl->PatternControlCSC.controlFunction != Cond_ControlFunction_MipiRead)		&&
				(amb7300tl->PatternControlCSC.controlFunction != Cond_ControlFunction_PlayPattern))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'ControlFunction' available settings: "	+ "\n" +
												"-> MipiWrite"								+ "\n" +
												"-> MipiRead"								+ "\n" +
												"-> PlayPattern";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_ControlFunction, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'ControlFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "isMultiVecToOneResult & isOneVecToMultiResult"

		if ((conditionName == PatternControlConditionName_isMultiVecToOneResult) ||
			(conditionName == PatternControlConditionName_isOneVecToMultiResult))
		{
			if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == true) &&
				(amb7300tl->PatternControlCSC.isOneVecToMultiResult == true))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'isMultiVecToOneResult' & 'isOneVecToMultiResult' function cannot be enabled at the same time."			+ "\n" +
												"Please only enable one of the function by setting 'True', and then disable the rest by setting 'False'."	+ "\n"
												"Set to 'True' = enable | Set to 'False' = disable";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_isMultiVecToOneResult + " & " + PatternControlConditionName_isOneVecToMultiResult, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == false) &&
				(amb7300tl->PatternControlCSC.isOneVecToMultiResult == false))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'isMultiVecToOneResult' & 'isOneVecToMultiResult' function cannot be disabled at the same time."			+ "\n" +
												"Please only enable one of the function by setting 'True', and then disable the rest by setting 'False'."	+ "\n"
												"Set to 'True' = enable | Set to 'False' = disable";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_isMultiVecToOneResult + " & " + PatternControlConditionName_isOneVecToMultiResult, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "MipiFreq"

		if (conditionName == PatternControlConditionName_MipiFreq)
		{
			if ((amb7300tl->PatternControlCSC.userMipiFreq <= 0.0 MHz) ||
				(amb7300tl->PatternControlCSC.userMipiFreq > 52.0 MHz))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'MipiFreq' available settings: " + "\n" +
												"-> >0.0 MHz ~ 52.0 MHz";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_MipiFreq, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'MipiFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'MipiFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "sclkPinAlias"

		if (conditionName == PatternControlConditionName_sclkPinAlias)
		{
			int siteIndexTemp = 0;
			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				siteIndexTemp = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				siteIndexTemp = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}

			bool isPinAliasMatch = false;
			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				if (amb7300tl->PatternControlCSC.sclkPinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, i])
				{
					isPinAliasMatch = true;
					break;
				}
			}

			if (isPinAliasMatch == false)
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'sclkPinAlias' must exist inside the resource planner."	+ "\n" +
												"'sclkPinAlias' must be the same as the 'Pin Map Name' inside the ResourcePlanner.";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_sclkPinAlias, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sclkPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sclkPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "sdataPinAlias"

		if (conditionName == PatternControlConditionName_sdataPinAlias)
		{
			int siteIndexTemp = 0;
			if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				siteIndexTemp = tfSite;
			}
			else if ((tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (tl->glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)))
			{
				siteIndexTemp = siteIndex;
			}
			else if (tl->glob->tf.ProjectType == int(ProjectType::IndexParallel))
			{
				// [Future Enchancement] reserved for index parallel project.
			}

			bool isPinAliasMatch = false;
			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				if (amb7300tl->PatternControlCSC.sdataPinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, i])
				{
					isPinAliasMatch = true;
					break;
				}
			}

			if (isPinAliasMatch == false)
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'sdataPinAlias' must exist inside the resource planner."	+ "\n" +
												"'sdataPinAlias' must be the same as the 'Pin Map Name' inside the ResourcePlanner.";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_sdataPinAlias, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sdataPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sdataPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

#pragma region "sclk & sdata pin alias must under the same module"

			String ^ moduleAliasForSclk		= String::Empty;
			String ^ moduleAliasForSdata	= String::Empty;

			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				for (int j = 0; j < tl->glob->RsrcManager.dmModuleCount; j++)
				{
					if (amb7300tl->PatternControlCSC.sclkPinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, i])
					{
						if (tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndexTemp, i]->Contains(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndexTemp, j]))
						{
							moduleAliasForSclk = tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndexTemp, j];
						}
					}
				}
			}

			for (int i = 0; i < tl->glob->RsrcManager.dmPinCount; i++)
			{
				for (int j = 0; j < tl->glob->RsrcManager.dmModuleCount; j++)
				{
					if (amb7300tl->PatternControlCSC.sdataPinAlias == tl->glob->RsrcManager.pinAlias_DM_PinMapName[siteIndexTemp, i])
					{
						if (tl->glob->RsrcManager.pinAlias_DM_HwResourceAlias[siteIndexTemp, i]->Contains(tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndexTemp, j]))
						{
							moduleAliasForSdata = tl->glob->RsrcManager.moduleAlias_DM_PinMapName[siteIndexTemp, j];
						}
					}
				}
			}

			if (moduleAliasForSclk != moduleAliasForSdata)
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "'sclkPinAlias' & 'sdataPinAlias' resource must be under the same DM module.";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_sclkPinAlias + " & " +  PatternControlConditionName_sdataPinAlias, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sclkPinAlias' & 'sdataPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'sclkPinAlias' & 'sdataPinAlias' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

#pragma endregion

		}

#pragma endregion

#pragma region "DmTerminationVoltage"

		if (conditionName == PatternControlConditionName_DmTerminationVoltage)
		{
			if ((amb7300tl->PatternControlCSC.userVth < 0.0 V) ||
				(amb7300tl->PatternControlCSC.userVth > 15.0 V))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'DmTerminationVoltage' available settings: " + "\n" +
												"-> 0.0 V ~ 15.0 V";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_DmTerminationVoltage, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'DmTerminationVoltage' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'DmTerminationVoltage' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "DmInputChDelay"

		if (conditionName == PatternControlConditionName_DmInputChDelay)
		{
			if ((amb7300tl->PatternControlCSC.userDmInputChDelay < -10.0) ||
				(amb7300tl->PatternControlCSC.userDmInputChDelay > 100.0))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'DmInputChDelay' available settings: " + "\n" +
												"-> -10.0 clock cycle ~ 100.0 clock cycle";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_DmInputChDelay, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'DmInputChDelay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'DmInputChDelay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "RunVectorDelay"

		// No need to check run vector delay

#pragma endregion

#pragma region "VectorFileName"

		if (conditionName == PatternControlConditionName_VectorFileName)
		{
			array<bool> ^ isVectorFileNameMatch = gcnew array<bool>(amb7300tl->PatternControlCSC.vectorFileName->Length);
			for (int i = 0; i < amb7300tl->PatternControlCSC.vectorFileName->Length; i++)
			{
				isVectorFileNameMatch[i] = false;
			}

			for (int i = 0; i < amb7300tl->PatternControlCSC.vectorFileName->Length; i++)
			{
				for (int j = 0; j < tl->glob->VectorFile.vecFileName->Length; j++)
				{
					if (amb7300tl->PatternControlCSC.vectorFileName[i] == tl->glob->VectorFile.vecFileName[j])
					{
						isVectorFileNameMatch[i] = true;
					}
				}
			}

			for (int i = 0; i < isVectorFileNameMatch->Length; i++)
			{
				if (isVectorFileNameMatch[i] == false)
				{
					ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'VectorFileName' must be the same with the .vec file's name inside the 'VectorFileFolder', inside the project's 'TestRecipes' folder.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_VectorFileName, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == true) &&
				(amb7300tl->PatternControlCSC.vectorFileName->Length == 1))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"There is only 1x vector file in the 'PatternControl' condition, please enable 'isOneVecToMultiResult' function by setting 'True', and disable 'isMultiVecToOneResult' by setting 'False'."	+ "\n"
												"Set to 'True' = enable | Set to 'False' = disable";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_VectorFileName, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' & 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			if ((amb7300tl->PatternControlCSC.isOneVecToMultiResult == true) &&
				(amb7300tl->PatternControlCSC.vectorFileName->Length > 1))
			{
				ret = ER_CONST_PATTERNCONTROL_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"There is more than 1x vector file in the 'PatternControl' condition, please enable 'isMultiVecToOneResult' function by setting 'True', and disable 'isOneVecToMultiResult' by setting 'False'." + "\n"
												"Set to 'True' = enable | Set to 'False' = disable";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, PatternControlConditionName_VectorFileName, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['PatternControl' condition value verification] 'VectorFileName' & 'isMultiVecToOneResult' & 'isOneVecToMultiResult' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

	EndOfTest:
		 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> PatternTest
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_PatternTest(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_PatternTest
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'PatternTest'.
		**		Include: 'ReturnIndex'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "ReturnIndex"

		// No need to check return index

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> DmControl
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_DmControl(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int totalConfigurationSets)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_DmControl
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**	totalConfigurationSets	- This is the total sets of configuration.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'DmControl'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "ModuleAlias"

		if (conditionName == DmControlConditionName_ModuleAlias)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				//Check if value is DM resource from amap
				/*if ((amb7300tl->DcControlCSC.module[i] != Cond_Module_CM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_AM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_DM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_IOM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_ACM)	&&
					(amb7300tl->DcControlCSC.module[i] != Cond_Module_TM))
				{
					ret = ER_CONST_DCCONTROL_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'Module' available settings: " + "\n" +
						"-> CM"							+ "\n" +
						"-> AM"							+ "\n" +
						"-> DM"							+ "\n" +
						"-> IOM"						+ "\n" +
						"-> ACM"						+ "\n" + 
						"-> TMU";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, DcControlConditionName_Module, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['DcControl' condition value verification] 'Module' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}*/
			}
		}

#pragma endregion


#pragma region "TriggerOutput0"

		if (conditionName == DmControlConditionName_TriggerOutput0)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				//Validate the trigger value
			}
		}

#pragma endregion

#pragma region "TriggerOutput1"

		if (conditionName == DmControlConditionName_TriggerOutput1)
		{
			for (int i = 0; i < totalConfigurationSets; i++)
			{
				//Validate the trigger value
			}
		}

#pragma endregion
		EndOfTest:
				 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> VnaConfig
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_VnaConfig(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw, int vnaConfigSegmentCount)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_VnaConfig
		**	tfSite					- This is techFlow site object.
		**	siteIndex				- This is techFlow site index.
		**	conditionName			- This is the condition name.
		**	conditionValueRaw		- This is the condition value input (raw) in the array form.
		**	vnaConfigSegmentCount	- This is the total VNA segment count.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in 'VnaConfig'.
		**		Include: 'SweepType', 'StartFreq', 'StopFreq', 'Points', 'Ifbw', 'Power', 'Delay'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "SweepType"

		if (conditionName == VnaConfigConditionName_SweepType)
		{
			if ((conditionValueRaw[0] != Cond_SweepType_LinearFreq) &&
				(conditionValueRaw[0] != Cond_SweepType_LogFreq) &&
				(conditionValueRaw[0] != Cond_SweepType_Segment) &&
				(conditionValueRaw[0] != Cond_SweepType_PowerSweep) &&
				(conditionValueRaw[0] != Cond_SweepType_GetFromStateFile) &&
				(conditionValueRaw[0] != Cond_SweepType_SharedMemoryTransfer))
			{
				ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "'SweepType' available settings: " + "\n" +
					"-> LinearFreq" + "\n" +
					"-> LogFreq" + "\n" +
					"-> Segment" + "\n" +
					"-> PowerSweep" + "\n" +
					"-> GetFromStateFile" + "\n" +
					"-> SharedMemoryTransfer";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_SweepType, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'SweepType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'SweepType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "ChannelNum"

		if (conditionName == VnaConfigConditionName_ChannelNum)
		{
			bool match_flag = false;
			for (int i = 0; i < amb7300tl->vnaSetting[siteIndex].vna_chn->Length; i++)
			{
				if (amb7300tl->vnaSetting[siteIndex].vna_chn[i] == amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel)
				{
					match_flag = true;
				}
			}
			if ((amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel < 1) ||
				(amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel > 8))
			{
				ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "'ChannelNum' available settings: " + "\n" +
					"-> 1 ~ 16";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_ChannelNum, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
			else if (match_flag == false)
			{
				String^ str_vna_chn = "";
				ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
				for (int i = 0; i < amb7300tl->vnaSetting[siteIndex].vna_chn->Length; i++)
				{
					str_vna_chn += amb7300tl->vnaSetting[siteIndex].vna_chn[i].ToString() + ", " ;
				}
				String ^ additionalMessage = "'ChannelNum' does not exist in mapping file! Please calibrate the Channel. " + "\n" +
											"Channel available from Mapping file-> " + str_vna_chn;
					
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_ChannelNum, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "StartFreq & StopFreq"

		if ((conditionName == VnaConfigConditionName_StratFreq) ||
			(conditionName == VnaConfigConditionName_StopFreq))
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{ 
					if ((amb7300tl->segmentSetting[siteIndex].startFreq[i] < 300.0 kHz) ||
						(amb7300tl->segmentSetting[siteIndex].startFreq[i] > 9000.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'StartFreq' available settings: " + "\n" +
														"-> 300.0 kHz ~ 9000.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StratFreq, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}

					if ((amb7300tl->segmentSetting[siteIndex].stopFreq[i] < 300.0 kHz) ||
						(amb7300tl->segmentSetting[siteIndex].stopFreq[i] > 9000.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'StopFreq' available settings: " + "\n" +
														"-> 300.0 kHz ~ 9000.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StopFreq, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((amb7300tl->segmentSetting[siteIndex].startFreq[i] < 9.0 kHz) ||
						(amb7300tl->segmentSetting[siteIndex].startFreq[i] > 20000.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'StartFreq' available settings: " + "\n" +
														"-> 9.0 kHz ~ 20000.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StratFreq, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}

					if ((amb7300tl->segmentSetting[siteIndex].stopFreq[i] < 9.0 kHz) ||
						(amb7300tl->segmentSetting[siteIndex].stopFreq[i] > 20000.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'StopFreq' available settings: " + "\n" +
														"-> 9.0 kHz ~ 20000.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StopFreq, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
		}

#pragma endregion

#pragma region "If StartFreq same with StopFreq"

		if ((conditionName == VnaConfigConditionName_StratFreq) ||
			(conditionName == VnaConfigConditionName_StopFreq))
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->segmentSetting[siteIndex].startFreq[i] == amb7300tl->segmentSetting[siteIndex].stopFreq[i])
				{
					ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'StartFreq' & 'StopFreq' cannot have the same frequency condition value.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StratFreq + " & " + VnaConfigConditionName_StopFreq, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' & 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''StartFreq' & StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "If segment frequency overlap with other segment frequency"

		if ((conditionName == VnaConfigConditionName_StratFreq) ||
			(conditionName == VnaConfigConditionName_StopFreq))
		{
			for (int i = 1; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->segmentSetting[siteIndex].startFreq[i] <= amb7300tl->segmentSetting[siteIndex].stopFreq[i - 1])
				{
					ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "Segment frequency cannot overlap with other segment frequency.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StratFreq + " & " + VnaConfigConditionName_StopFreq, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' & 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''StartFreq' & StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "If FreqStart & FreqStop invalid in range order"

		if ((conditionName == VnaConfigConditionName_StratFreq) ||
			(conditionName == VnaConfigConditionName_StopFreq))
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->segmentSetting[siteIndex].startFreq[i] > amb7300tl->segmentSetting[siteIndex].stopFreq[i])
				{
					ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'StartFreq' & 'StopFreq' must exist in a valid range (StartFreq must < StopFreq).";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_StratFreq + " & " + VnaConfigConditionName_StopFreq, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'StartFreq' & 'StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''StartFreq' & StopFreq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "Points"

		if (conditionName == VnaConfigConditionName_Points)
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					// CMT spec max point is 500001, but in test library we limit the point up to 100001 to prevent S2VNA hang
					if ((amb7300tl->segmentSetting[siteIndex].point[i] < 2 ) ||
						(amb7300tl->segmentSetting[siteIndex].point[i] > 100001))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Points' available settings: " + "\n" +
														"-> 2 ~ 100001";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Ifbw, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Points' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Points' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((amb7300tl->segmentSetting[siteIndex].point[i] < 1 ) ||
						(amb7300tl->segmentSetting[siteIndex].point[i] > 100003))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Points' available settings: " + "\n" +
														"-> 1 ~ 100003";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Ifbw, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Points' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Points' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
		}

#pragma endregion

#pragma region "Ifbw"

		if (conditionName == VnaConfigConditionName_Ifbw)
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((amb7300tl->segmentSetting[siteIndex].ifbw[i] < 1.0 Hz) ||
						(amb7300tl->segmentSetting[siteIndex].ifbw[i] > 1.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Ifbw' available settings: " + "\n" +
														"-> 1.0 Hz ~ 1.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Ifbw, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Ifbw' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Ifbw' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((amb7300tl->segmentSetting[siteIndex].ifbw[i] < 1.0 Hz) ||
						(amb7300tl->segmentSetting[siteIndex].ifbw[i] > 15.0 MHz))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Ifbw' available settings: " + "\n" +
														"-> 1.0 Hz ~ 15.0 MHz";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Ifbw, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Ifbw' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Ifbw' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
		}

#pragma endregion

#pragma region "Power"

		if (conditionName == VnaConfigConditionName_Power)
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((amb7300tl->segmentSetting[siteIndex].power[i] < -50.0 dBm) ||
						(amb7300tl->segmentSetting[siteIndex].power[i] > 15.0 dBm))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Power' available settings: " + "\n" +
														"-> -50.0 dBm ~ +15.0 dBm";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Power, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((amb7300tl->segmentSetting[siteIndex].power[i] < -100.0 dBm) ||
						(amb7300tl->segmentSetting[siteIndex].power[i] > 20.0 dBm))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Power' available settings: " + "\n" +
														"-> -100.0 dBm ~ +20.0 dBm";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Power, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
		}

#pragma endregion

#pragma region "Delay"

		if (conditionName == VnaConfigConditionName_Delay)
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((amb7300tl->segmentSetting[siteIndex].delay[i] < 0.0 S) ||
						(amb7300tl->segmentSetting[siteIndex].delay[i] > 100.0 S))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Delay' available settings: " + "\n" +
														"-> 0.0 sec ~ 100.0 sec";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Delay, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Delay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Delay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((amb7300tl->segmentSetting[siteIndex].delay[i] < -86400.0 S) ||
						(amb7300tl->segmentSetting[siteIndex].delay[i] > 86400.0 S))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'Delay' available settings: " + "\n" +
														"-> -86400 sec ~ 86400.0 sec";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_Delay, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'Delay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''Delay' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
		}

#pragma endregion

#pragma region"SNP"

		if (conditionName == VnaConfigConditionName_SNP)
		{
			for (int i = 0; i < vnaConfigSegmentCount; i++)
			{
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((conditionValueRaw[0] != "NA") &&
						(conditionValueRaw[0] != Vna_SaveDataToTouchstoneFile_Type_S3P))
					{
						ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'SNP' available settings: " + "\n" +
							"-> NA\n" +
							"-> S3P\n";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_SNP, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'SNP' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''SNP' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((conditionValueRaw[0] != "NA") &&
						(conditionValueRaw[0] != Vna_SaveDataToTouchstoneFile_Type_S3P))
					{
					ret = ER_CONST_VNACONFIG_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'SNP' available settings: " + "\n" +
						"-> NA\n" +
						"-> S3P\n";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaConfigConditionName_SNP, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] 'SNP' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaConfig' condition value verification] ''SNP' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
					}
				}
			}
		}

#pragma endregion

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> VnaFetch
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_VnaFetch(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_VnaFetch
		**	tfSite				- This is techFlow site object.
		**	siteIndex			- This is techFlow site index.
		**	conditionName		- This is the condition name.
		**	conditionValueRaw	- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in the 'VnaFetch'.
		**		Include: 'MeasurePortPair', 'TraceCount', 
		**		'TraceMeasurementFormat', 'AveragingSetting', 'SaveSnpData'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "MeasurePortPair --> validate port count and port alias pair"

		if (conditionName == VnaFetchConditionName_MeasurePortPair)
		{ 
			// Determine measurement type: 1-port / 2-port / 3-port / 4-port / 5-port / 6-port
			int measurePortCount = conditionValueRaw->Length;

			if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			{
				if (measurePortCount == 2)
				{
					// A0 S2P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A0)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_A0_S2P_P1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_A0_S2P_P2))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A0 (S2P)"	+ "\n" +
															"Port1 name only available in: P1"				+ "\n" +
															"Port2 name only available in: P2";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A1 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A1)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P4_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A1_S2P_OUT1) && 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A1_S2P_OUT2)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A1_S2P_OUT3)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A1_S2P_OUT4)) ||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A1_S2P_OUT5) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A1_S2P_OUT6)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A1_S2P_OUT7)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A1_S2P_OUT8)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A1 (S2P)"				+ "\n" +
															"Port1 name only available in: OUT1, OUT2, OUT3, OUT4"		+ "\n" +
															"Port2 name only available in: OUT5, OUT6, OUT7, OUT8";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A2 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A2)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P8_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT2)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT3)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT4)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT5)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT6)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT7)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A2_S2P_OUT8)) ||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT9) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT10) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT11) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT12) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT13) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT14) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT15) && 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A2_S2P_OUT16)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A2 (S2P)"											+ "\n" +
															"Port1 name only available in: OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8"			+ "\n" +
															"Port2 name only available in: OUT9, OUT10, OUT11, OUT12, OUT13, OUT14, OUT15, OUT16";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A3 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT6)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT7)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT8))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT2)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT3)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT4)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT5)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT6)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT7)	&& 
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT8)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A3 (S2P)"											+ "\n" +
															"Port1 name only available in: OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8"			+ "\n" +
															"Port2 name only available in: OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A4 (S2P)"					+ "\n" +
															"Port1 name only available in: OUT1, OUT2, OUT3, OUT4"			+ "\n" +
															"Port2 name only available in: OUT1, OUT2, OUT3, OUT4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A5 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A5)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P16_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT1)		&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT2)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT3)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT4)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT5)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT6)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT7)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT8)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT9)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT10)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT11)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT12)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT13)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT14)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT15)		&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_A5_S2P_OUT16))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A5_S2P_OUT17)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A5_S2P_OUT18)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A5 (S2P)"																							+ "\n" +
															"Port1 name only available in: OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8, OUT9, OUT10, OUT11, OUT12, OUT13, OUT14, OUT15, OUT16"	+ "\n" +
															"Port2 name only available in: OUT17, OUT18";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration A5 only able to measure S2P ('MeasurePortPair' = OUT1,OUT17), unable to measure S3P ('MeasurePortPair' = OUT1,OUT2,OUT3).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 3)
				{
					// A3 S3P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT2))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT3) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT4)) ||
							((conditionValueRaw[2] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT5) &&
							(conditionValueRaw[2] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A3 (S3P)"		+ "\n" +
															"Port1 name only available in: OUT1, OUT2"			+ "\n" +
															"Port2 name only available in: OUT3, OUT4"			+ "\n" +
															"Port3 name only available in: OUT5, OUT6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S3P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) ||
							(conditionValueRaw[2] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A4 (S3P)"	+ "\n" +
															"Port1 name only available in: OUT1"			+ "\n" +
															"Port2 name only available in: OUT2"			+ "\n" +
															"Port3 name only available in: OUT3";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration A5 only able to measure S2P ('MeasurePortPair' = OUT1,OUT17), unable to measure S3P ('MeasurePortPair' = OUT1,OUT2,OUT3).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 4)
				{
					// A3 S4P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT2))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT3) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT4)) ||
							((conditionValueRaw[2] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT5) &&
							(conditionValueRaw[2] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT6))	||
							((conditionValueRaw[3] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT7) &&
							(conditionValueRaw[3] != Cond_MeasurePortPair_A3_S2P_S3P_S4P_OUT8)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A3 (S4P)"	+ "\n" +
															"Port1 name only available in: OUT1, OUT2"		+ "\n" +
															"Port2 name only available in: OUT3, OUT4"		+ "\n" +
															"Port3 name only available in: OUT5, OUT6"		+ "\n" +
															"Port4 name only available in: OUT7, OUT8";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S4P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) ||
							(conditionValueRaw[2] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3) ||
							(conditionValueRaw[3] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config A4 (S4P)"	+ "\n" +
															"Port1 name only available in: OUT1"			+ "\n" +
															"Port2 name only available in: OUT2"			+ "\n" +
															"Port3 name only available in: OUT3"			+ "\n" +
															"Port4 name only available in: OUT4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration A5 only able to measure S2P ('MeasurePortPair' = OUT1,OUT17), unable to measure S3P ('MeasurePortPair' = OUT1,OUT2,OUT3).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else
				{
					ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'MeasurePortPair' contain 1x parameter which is the actual port name showing in the tester front panel."	+ "\n" + 
													"The port name can be combined into a measurement port pair by using ','."									+ "\n" + 
													"Only available in 2-port, 3-port, 4-port measurement."														+ "\n" + 
													"Example: P1,P2 --> S2P 2-port measurement."																+ "\n" +  
													"Example: OUT1,OUT2,OUT3 --> S3P 3-port measurement."														+ "\n" +  
													"Example: OUT1,OUT2,OUT3,OUT4 --> S4P 4-port measurement.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			{
				if (measurePortCount == 1)
				{
					// S2P1D S1P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S2P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1D_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S2P1D_P1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S2P1D_P2))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S2P1D (S1P)"	+ "\n" +
															"Port1 name only available in: P1, P2";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S4P1D S1P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P4))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S4P1D (S1P)"	+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S6P1D S1P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S1P)"	+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S22P1D
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P22N)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P22N_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT2) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT3) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT4) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT5) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT6) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT7) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT8) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT9) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT10) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT11) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT12) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT13) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT14) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT15) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT16) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT17) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT18) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT19) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT20) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT21) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT22))

						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'MeasurePortPair' for system config S6P22N (S1P)" + "\n" +
								"Port1 name only available in: P1A, P2A, P3A up to P22A";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 2)
				{
					// S2P1D S2P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S2P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1D_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S2P1D_P1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S2P1D_P2))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S2P1D (S2P)"	+ "\n" +
															"Port1 name only available in: P1"					+ "\n" +
															"Port2 name only available in: P2";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S4P1D S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P4)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S4P1D (S2P)"	+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port2 name only available in: P2, P2, P3, P4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S6P1D S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S2P)"			+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port2 name only available in: P2, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S6P22N S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P22N)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P22N_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT2) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT3) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT4) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT5) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT6) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT7) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT8) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT9) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT10) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT11) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT12) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT13) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT14) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT15) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT16) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT17) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT18) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT19) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT20) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT21) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P22N_OUT22)) ||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT1) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT2) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT3) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT4) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT5) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT6) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT7) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT8) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT9) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT10) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT11) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT12) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT13) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT14) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT15) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT16) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT17) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT18) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT19) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT20) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT21) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P22N_OUT22)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'MeasurePortPair' for system config S6P22N (S2P)" + "\n" +
								"Port1 name only available in: OUT1~OUT18, P3, P4, P5, P6" + "\n" +
								"Port2 name only available in: OUT1~OUT18, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S2P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3) &&
							(conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4)) ||
							((conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) &&
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3) &&
								(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'MeasurePortPair' for system config A4 (S2P)" + "\n" +
								"Port1 name only available in: OUT1, OUT2, OUT3, OUT4" + "\n" +
								"Port2 name only available in: OUT1, OUT2, OUT3, OUT4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 3)
				{
					// S4P1D S3P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P4)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S4P1D (S3P)"	+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port3 name only available in: P2, P2, P3, P4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S6P1D S3P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S3P)"			+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port3 name only available in: P2, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S3P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) ||
							(conditionValueRaw[2] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'MeasurePortPair' for system config A4 (S3P)" + "\n" +
								"Port1 name only available in: OUT1" + "\n" +
								"Port2 name only available in: OUT2" + "\n" +
								"Port3 name only available in: OUT3";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 4)
				{
					// S4P1D S4P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S4P1D_P4))	||
							((conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S4P1D_P1)	&& 
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S4P1D_P2)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S4P1D_P3)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S4P1D_P4)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S4P1D (S4P)"	+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port3 name only available in: P1, P2, P3, P4"		+ "\n" +
															"Port4 name only available in: P2, P2, P3, P4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// S6P1D S4P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S4P)"			+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port3 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port4 name only available in: P2, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					// A4 S4P
					else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
					{
						if ((conditionValueRaw[0] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT1) ||
							(conditionValueRaw[1] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT2) ||
							(conditionValueRaw[2] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT3) ||
							(conditionValueRaw[3] != Cond_MeasurePortPair_A4_S2P_S3P_S4P_OUT4))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'MeasurePortPair' for system config A4 (S4P)" + "\n" +
								"Port1 name only available in: OUT1" + "\n" +
								"Port2 name only available in: OUT2" + "\n" +
								"Port3 name only available in: OUT3" + "\n" +
								"Port4 name only available in: OUT4";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 5)
				{
					// S6P1D S5P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S5P)"			+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port3 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port4 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port5 name only available in: P2, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (measurePortCount == 6)
				{
					// S6P1D S6P
					if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
					{
						if (((conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[0] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[1] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[2] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[3] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[4] != Cond_MeasurePortPair_Keysight_S6P1D_P6))	||
							((conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P1)	&& 
							(conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P2)	&&
							(conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P3)	&&
							(conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P4)	&&
							(conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P5)	&&
							(conditionValueRaw[5] != Cond_MeasurePortPair_Keysight_S6P1D_P6)))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'MeasurePortPair' for system config S6P1D (S6P)"			+ "\n" +
															"Port1 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port2 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port3 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port4 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port5 name only available in: P1, P2, P3, P4, P5, P6"		+ "\n" +
															"Port6 name only available in: P2, P2, P3, P4, P5, P6";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else
					{
						ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"Unidentified 'MeasurePortPair'."										+ "\n"
														"'MeasurePortPair' type does not match with the system configuration."	+ "\n"
														"For example: system configuration S4P1D only able to measure S1P/S2P/S3P/S4P ('MeasurePortPair' = P1,P2,P3,P4), unable to measure S6P ('MeasurePortPair' = P1,P2,P3,P4,P5,P6).";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else
				{
					ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'MeasurePortPair' contain 1x parameter which is the actual port name showing in the tester front panel."	+ "\n" + 
													"The port name can be combined into a measurement port pair by using ','."									+ "\n" + 
													"Only available in 2-port, 3-port, 4-port measurement."														+ "\n" + 
													"Example: P1,P2 --> S2P 2-port measurement."																+ "\n" +  
													"Example: OUT1,OUT2,OUT3 --> S3P 3-port measurement."														+ "\n" +  
													"Example: OUT1,OUT2,OUT3,OUT4 --> S4P 4-port measurement.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_MeasurePortPair, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'MeasurePortPair' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "TraceCount"

		if (conditionName == VnaFetchConditionName_TraceCount)
		{ 
			if ((amb7300tl->vnaSetting[siteIndex].userTraceCount < 1) ||
				(amb7300tl->vnaSetting[siteIndex].userTraceCount > 16))
			{
				ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'TraceCount' available settings: " + "\n" +
												"-> 1 ~ 16";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceCount, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceCount' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceCount' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "TraceMeasurementFormat"

		/*	raw content
		**	conditionValueRaw[0] = "S11/LogMag"
		**	conditionValueRaw[1] = "S21/LogMag"
		**	conditionValueRaw[2] = "S12/LogMag"
		**	conditionValueRaw[3] = "S22/LogMag"
		*/

		if (conditionName == VnaFetchConditionName_TraceMeasurementFormat)
		{
			array<String^> ^ arrStr2	= gcnew array<String^>(0);
			array<String^> ^ separator2 = gcnew array<String^>(1);
			separator2[0]				= "/";
			
			for (int i = 0; i < conditionValueRaw->Length; i++)
			{
				arrStr2 = conditionValueRaw[i]->Split(separator2, StringSplitOptions::None);
				/*	outcome example
				**	arrStr2[0] = "S11"	|	arrStr2[0]	= "S21"
				**	arrStr2[1] = "SWR"	|	arrStr2[1]	= "LogMag"
				*/

				if (arrStr2->Length == 2)
				{
					if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
					{
						if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 2)
						{
							// A0, A1, A2, A3, A4, A5 S2P
							if (((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A0)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A1)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P4_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A2)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P8_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A5)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P16_TYPE)))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage = "'TraceMeasurementFormat' for system config A0/A1/A2/A3/A4/A5 (S2P), 1st parameter available settings: " + "\n" +
										"-> S11 / A11 / B11" + "\n" +
										"-> S12 / A12 / B12" + "\n" +
										"-> S21 / A21 / B21" + "\n" +
										"-> S22 / A22 / B22";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 3)
						{
							// A3, A4 S3P
							if (((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE)))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config  A3/A4 (S3P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 4)
						{
							// A3, A4 S4P
							if (((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A3)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P2_TYPE)) ||
								((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE)))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config  A3/A4 (S4P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
															"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
															"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}

						// Measurement format
						if ((arrStr2[1] != Cond_TraceMeasurementFormat_Format_LogMag)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SWR)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Phase)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_ExPhase)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Delay)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_LinMag)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Real)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Imag)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithLog)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithLin)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithReIm)	&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithRJX)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithGJB)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarLog)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarLin)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarReIm))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'TraceMeasurementFormat' 2nd parameter available settings: "	+ "\n" +
															"-> LogMag"														+ "\n" +
															"-> SWR"														+ "\n" +
															"-> Phase"														+ "\n" +
															"-> ExPhase"													+ "\n" +
															"-> Delay"														+ "\n" +
															"-> LinMag"														+ "\n" +
															"-> Real"														+ "\n" +
															"-> Imag"														+ "\n" +
															"-> SmithLog"													+ "\n" +
															"-> SmithLin"													+ "\n" +
															"-> SmithReIm"													+ "\n" +
															"-> SmithR+JX"													+ "\n" +
															"-> SmithG+JB"													+ "\n" +
															"-> PolarLog"													+ "\n" +
															"-> PolarLin"													+ "\n" +
															"-> PolarReIm";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
					{
						if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 1)
						{
							// S2P1D S1P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S2P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S2P1D (S1P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S22 / A22 / B22";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S4P1D S1P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S4P1D (S1P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S6P1D S1P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S1P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 2)
						{
							// S2P1D S2P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S2P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S2P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S2P1D (S2P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S4P1D S2P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S4P1D (S2P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S6P1D S2P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S2P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S6P22N S2P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P22N)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P22N_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage = "'TraceMeasurementFormat' for system config S6P22N (S2P), 1st parameter available settings: " + "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// A4 S2P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage = "'TraceMeasurementFormat' for system config A4(S2P), 1st parameter available settings: " + "\n" +
										"-> S11 / A11 / B11 " + "\n" +
										"-> S12 / A12 / B12 " + "\n" +
										"-> S21 / A21 / B21 " + "\n" +
										"-> S22 / A22 / B22 ";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 3)
						{
							// S4P1D S3P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S4P1D (S3P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S6P1D S3P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S3P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// A4 S3P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage = "'TraceMeasurementFormat' for system config A4 (S3P), 1st parameter available settings: " + "\n" +
										"-> S11 / A11 / B11" + "\n" +
										"-> S12 / A12 / B12" + "\n" +
										"-> S13 / A13 / B13" + "\n" +
										"-> S21 / A21 / B21" + "\n" +
										"-> S22 / A22 / B22" + "\n" +
										"-> S23 / A23 / B23" + "\n" +
										"-> S31 / A31 / B31" + "\n" +
										"-> S32 / A32 / B32" + "\n" +
										"-> S33 / A33 / B33";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 4)
						{
							// S4P1D S4P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S4P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S4P1D (S4P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// S6P1D S4P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S4P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							// A3, A4 S4P
							else if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::A4)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S4P1_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage = "'TraceMeasurementFormat' for system config A4 (S4P), 1st parameter available settings: " + "\n" +
										"-> S11 / A11 / B11" + "\n" +
										"-> S12 / A12 / B12" + "\n" +
										"-> S13 / A13 / B13" + "\n" +
										"-> S14 / A14 / B14" + "\n" +
										"-> S21 / A21 / B21" + "\n" +
										"-> S22 / A22 / B22" + "\n" +
										"-> S23 / A23 / B23" + "\n" +
										"-> S24 / A24 / B24" + "\n" +
										"-> S31 / A31 / B31" + "\n" +
										"-> S32 / A32 / B32" + "\n" +
										"-> S33 / A33 / B33" + "\n" +
										"-> S34 / A34 / B34" + "\n" +
										"-> S41 / A41 / B41" + "\n" +
										"-> S42 / A42 / B42" + "\n" +
										"-> S43 / A43 / B43" + "\n" +
										"-> S44 / A44 / B44";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 5)
						{
							// S6P1D S5P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S5P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else if (amb7300tl->vnaSetting[siteIndex].measureSnpType == 6)
						{
							// S6P1D S6P
							if ((amb7300tl->amb7300SystemSetting->systemConfig == int(SystemConfig::S6P1D)) && (amb7300tl->sysConfigInfo.configurationType == AMB7300_S6P1D_TYPE))
							{
								// Measurement parameter
								if ((arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A11) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B11) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A12) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B12) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A13) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B13) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A14) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B14) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A15) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B15) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A16) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B16) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A21) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B21) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A22) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B22) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A23) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B23) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A24) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B24) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A25) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B25) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A26) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B26) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A31) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B31) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A32) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B32) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A33) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B33) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A34) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B34) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A35) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B35) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A36) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B36) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A41) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B41) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A42) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B42) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A43) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B43) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A44) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B44) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A45) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B45) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A46) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B46) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A51) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B51) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A52) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B52) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A53) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B53) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A54) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B54) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A55) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B55) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A56) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B56) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A61) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B61) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A62) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B62) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A63) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B63) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A64) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B64) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A65) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B65) &&
									(arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_S66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_A66) && (arrStr2[0] != Cond_TraceMeasurementFormat_Parameter_B66))
								{
									ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
									String ^ additionalMessage =	"'TraceMeasurementFormat' for system config S6P1D (S6P), 1st parameter available settings: "	+ "\n" +
																	"-> S11 / A11 / B11"																						+ "\n" +
																	"-> S12 / A12 / B12"																						+ "\n" +
																	"-> S13 / A13 / B13"																						+ "\n" +
																	"-> S14 / A14 / B14"																						+ "\n" +
																	"-> S15 / A15 / B15"																						+ "\n" +
																	"-> S16 / A16 / B16"																						+ "\n" +
																	"-> S21 / A21 / B21"																						+ "\n" +
																	"-> S22 / A22 / B22"																						+ "\n" +
																	"-> S23 / A23 / B23"																						+ "\n" +
																	"-> S24 / A24 / B24"																						+ "\n" +
																	"-> S25 / A25 / B25"																						+ "\n" +
																	"-> S26 / A26 / B26"																						+ "\n" +
																	"-> S31 / A31 / B31"																						+ "\n" +
																	"-> S32 / A32 / B32"																						+ "\n" +
																	"-> S33 / A33 / B33"																						+ "\n" +
																	"-> S34 / A34 / B34"																						+ "\n" +
																	"-> S35 / A35 / B35"																						+ "\n" +
																	"-> S36 / A36 / B36"																						+ "\n" +
																	"-> S41 / A41 / B41"																						+ "\n" +
																	"-> S42 / A42 / B42"																						+ "\n" +
																	"-> S43 / A43 / B43"																						+ "\n" +
																	"-> S44 / A44 / B44"																						+ "\n" +
																	"-> S45 / A45 / B45"																						+ "\n" +
																	"-> S46 / A46 / B46"																						+ "\n" +
																	"-> S51 / A51 / B51"																						+ "\n" +
																	"-> S52 / A52 / B52"																						+ "\n" +
																	"-> S53 / A53 / B53"																						+ "\n" +
																	"-> S54 / A54 / B54"																						+ "\n" +
																	"-> S55 / A55 / B55"																						+ "\n" +
																	"-> S56 / A56 / B56"																						+ "\n" +
																	"-> S61 / A61 / B61"																						+ "\n" +
																	"-> S62 / A62 / B62"																						+ "\n" +
																	"-> S63 / A63 / B63"																						+ "\n" +
																	"-> S64 / A64 / B64"																						+ "\n" +
																	"-> S65 / A65 / B65"																						+ "\n" +
																	"-> S66 / A66 / B66";
									amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
									tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
									return ret;
								}
							}
							else
							{
								ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
								String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
																"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
																"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
								amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
								tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								return ret;
							}
						}
						else
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"Unidentified 'TraceMeasurementFormat'."														+ "\n"
															"'TraceMeasurementFormat' measurement parameter does not match with the system configuration."	+ "\n"
															"For example: system configuration A5 only able to set measurement parameter S11/S12/S21/S22, unable to set S13,S23,S31,S33.";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}

						// Measurement format
						if ((arrStr2[1] != Cond_TraceMeasurementFormat_Format_LogMag)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SWR)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Phase)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_ExPhase)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Delay)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_LinMag)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Real)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_Imag)			&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithLog)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithLin)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithReIm)	&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithRJX)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_SmithGJB)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarLog)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarLin)		&&
							(arrStr2[1] != Cond_TraceMeasurementFormat_Format_PolarReIm))
						{
							ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage =	"'TraceMeasurementFormat' 2nd parameter available settings: "	+ "\n" +
															"-> LogMag"														+ "\n" +
															"-> SWR"														+ "\n" +
															"-> Phase"														+ "\n" +
															"-> ExPhase"													+ "\n" +
															"-> Delay"														+ "\n" +
															"-> LinMag"														+ "\n" +
															"-> Real"														+ "\n" +
															"-> Imag"														+ "\n" +
															"-> SmithLog"													+ "\n" +
															"-> SmithLin"													+ "\n" +
															"-> SmithReIm"													+ "\n" +
															"-> SmithR+JX"													+ "\n" +
															"-> SmithG+JB"													+ "\n" +
															"-> PolarLog"													+ "\n" +
															"-> PolarLin"													+ "\n" +
															"-> PolarReIm";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
				}
				else
				{
					ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'TraceMeasurementFormat' contain 2x parameter which is differentiate by '/'."						+ "\n" + 
													"It is able to contain multiple sets of setting for each trace# which is differentiate by ','."		+ "\n" +
													"1st parameter: Trace measurement parameter."														+ "\n" + 
													"2nd parameter: Trace measurement format."															+ "\n" + 
													"Example: S21/LogMag --> trace#1 measure 'S21' parameter with 'LogMag' format."						+ "\n" +
													"Example: S11/SWR,S21/LogMag --> trace#1 measure 'S11' parameter with 'SWR' format, trace#2 measure 'S21' parameter with 'LogMag' format.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceMeasurementFormat, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'TraceMeasurementFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "AveragingSetting"

		if (conditionName == VnaFetchConditionName_AveragingSetting)
		{
			if ((conditionValueRaw[0] != Cond_AveragingSetting_Enable_AveOn) &&
				(conditionValueRaw[0] != Cond_AveragingSetting_Enable_AveOff))
			{
				ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'AveragingSetting' 1st parameter available settings: " + "\n" +
												"-> AveOn"												+ "\n" +
												"-> AveOff";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_AveragingSetting, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			{ 
				/*
				**	Note that the average factor cannot contain decimal, even insert into the S2VNA, it will eliminate the decimal value.
				**	Example: 1.1 ~ 1.9, it will display as 1.
				*/
				if ((Double::Parse(conditionValueRaw[1]) < 1.0) ||
					(Double::Parse(conditionValueRaw[1]) > 999.0))
				{
					ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AveragingSetting' 2nd parameter available settings: " + "\n" +
													"-> 1 ~ 999";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_AveragingSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			{
				if ((Double::Parse(conditionValueRaw[1]) < 1.0) ||
					(Double::Parse(conditionValueRaw[1]) > 65535.0))
				{
					ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AveragingSetting' 2nd parameter available settings: " + "\n" +
													"-> 1 ~ 65535";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_AveragingSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'AveragingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "SaveSnpData"

		//Migrated to handled by AppsWideVariable during Load phase

#pragma endregion

#pragma region "ChannelNum"

		//// Obsolete
		//if (conditionName == VnaFetchConditionName_ChannelNum)
		//{
		//	if ((amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel < 1) ||
		//		(amb7300tl->vnaSetting[siteIndex].currentVnaFetchChannel > 8))
		//	{
		//		ret = ER_CONST_VNAFETCH_CONDITION_VALUE_INPUT_INVALID;
		//		String ^ additionalMessage = "'ChannelNum' available settings: " + "\n" +
		//			"-> 1 ~ 16";
		//		amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaFetchConditionName_TraceCount, additionalMessage);
		//		tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//		tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaFetch' condition value verification] 'ChannelNum' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
		//		return ret;
		//	}
		//}

#pragma endregion

	EndOfTest:
		return ret;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> VnaDataAnalysis
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_VnaDataAnalysis(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_VnaDataAnalysis
		**	tfSite				- This is techFlow site object.
		**	siteIndex			- This is techFlow site index.
		**	conditionName		- This is the condition name.
		**	conditionValueRaw	- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in the 'VnaDataAnalysis'.
		**		Include: 'TraceIndex', 'Function', 'AnalysisSetting', 'SmoothingSetting', 
		**		'Freq', 'FreqRangeStart', 'FreqRangeStop', 'OutputType', 'OutputFormat'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "TraceIndex"

		if (conditionName == VnaDataAnalysisConditionName_TraceIndex)
		{
			if ((amb7300tl->vnaDataAnalysisTPC.traceIndex < 1) || 
				(amb7300tl->vnaDataAnalysisTPC.traceIndex > 16))
			{
				ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'TraceIndex' available settings: " + "\n" +
												"-> 1 ~ 16";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_TraceIndex, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'TraceIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'TraceIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			if (amb7300tl->vnaDataAnalysisTPC.traceIndex > amb7300tl->vnaSetting[siteIndex].userTraceCount)
			{
				ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "Selected trace index must exists in 'TraceCount' at 'VnaFetch'.";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_TraceIndex, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'TraceIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'TraceIndex' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "Function"

		if (conditionName == VnaDataAnalysisConditionName_Function)
		{
			if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			{
				if ((amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_TraceData)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msMinMax)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msPeak)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msTarget)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmStatistics)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmBwSearch)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmBwSearchAbs)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmFlatness)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msMinMaxRipple) &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataBWSearch)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataBWSearchAbs)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataMinMax)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataTarget)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryMinMaxRipple)		&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_P1dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'Function' available settings: "	+ "\n" +
													"-> TraceData"						+ "\n" +
													"-> msMinMax"						+ "\n" +
													"-> msPeak"							+ "\n" +
													"-> msTarget"						+ "\n" +
													"-> mmStatistics"					+ "\n" + 
													"-> mmBwSearch"						+ "\n" +
													"-> mmBwSearchAbs"					+ "\n" +
													"-> mmFlatness"						+ "\n" +
													"-> msMinMaxRipple"					+ "\n" + 
													"-> Mean"							+ "\n" +
													"-> P1dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Function, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Function' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Function' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
				if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch) 
				{
					amb7300tl->vnaDataAnalysisTPC.function = Cond_Function_SharedMemoryDataBWSearch;
				}
				else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
				{
					amb7300tl->vnaDataAnalysisTPC.function = Cond_Function_mmBwSearchAbs;
				}
				else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
				{
					amb7300tl->vnaDataAnalysisTPC.function = Cond_Function_msMinMax;
				}
				else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget) 
				{
					amb7300tl->vnaDataAnalysisTPC.function = Cond_Function_msTarget;
				}
				else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple) 
				{
					amb7300tl->vnaDataAnalysisTPC.function = Cond_Function_msMinMaxRipple;
				}
			}
			else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			{
				if ((amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_TraceData)					&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msMinMax)					&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msPeak)					&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msTarget)					&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmStatistics)				&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmBwSearch)				&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_mmBwSearchAbs)				&&
					// mmFlatness not supported
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msMinMaxRipple)			  &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataBWSearch)	  &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataBWSearchAbs) &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataMinMax)	  &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryDataTarget)	  &&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_SharedMemoryMinMaxRipple)	&&
					(amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_P1dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'Function' available settings: "	+ "\n" +
													"-> TraceData"						+ "\n" +
													"-> msMinMax"						+ "\n" +
													"-> msPeak"							+ "\n" +
													"-> msTarget"						+ "\n" +
													"-> mmStatistics"					+ "\n" + 
													"-> mmBwSearch"						+ "\n" +
													"-> mmBwSearAbs"					+ "\n" +
													"-> msMinMaxRipple"					+ "\n" +
													"-> SharedMemoryDataMinMax"			+ "\n" +
													"-> SharedMemoryDataTarget"			+ "\n" +
													"-> SharedMemoryDataBWSearch"		+ "\n" +
													"-> SharedMemoryMinMaxRipple"		+ "\n" +
													"-> P1dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Function, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Function' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Function' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "AnalysisSetting"

		if (conditionName == VnaDataAnalysisConditionName_AnalysisSetting)
		{
			// TraceData
			if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_TraceData)
			{
				// NA
			}
			// msMinMax
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMax)
			{
				// Search Range On/Off
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msMinMax' function, 1st parameter available settings: "	+ "\n" +
													"-> SearchRangeOn"																+ "\n" +
													"-> SearchRangeOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Type
				if ((conditionValueRaw[1] != Cond_AnalysisSetting_MS_Type_Maximum) && 
					(conditionValueRaw[1] != Cond_AnalysisSetting_MS_Type_Minimum))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msMinMax' function, 2nd parameter available settings: "	+ "\n" +
													"-> Max"																		+ "\n" +
													"-> Min";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// msPeak
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msPeak)
			{
				// Search Type
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_MSPeak_Type_SearchPeak)		&&
					//(conditionValueRaw[0] != Cond_AnalysisSetting_MSPeak_Type_SearchMaxPeak)	&&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MSPeak_Type_SearchPeakLeft)	&&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MSPeak_Type_SearchPeakRight))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msPeak' function, 1st parameter available settings: "	+ "\n" +
													"-> SearchPeak"																	+ "\n" +
													"-> SearchPeakLeft"																+ "\n" +
													"-> SearchPeakRight";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Peak Excursion
				if ((Double::Parse(conditionValueRaw[1]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[1]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msPeak' function, 2nd parameter available settings: " + "\n" +
													"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Peak Polarity
				if ((conditionValueRaw[2] != Cond_AnalysisSetting_MSPeak_PeakPolarity_Positive)	&&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSPeak_PeakPolarity_Negative)	&&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSPeak_PeakPolarity_Both))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	 "'AnalysisSetting' for 'msPeak' function, 3rd parameter available settings: "	+ "\n" +
													"-> Positive"																	+ "\n" +
													"-> Negative"																	+ "\n" +
													"-> Both";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				/*// Peak Threshold - Not Applicable to use
				if ((Double::Parse(conditionValueRaw[3]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[3]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'msPeak' function, 4th parameter available settings: " + "\n" +
						"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}*/

			}
			// msTarget
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msTarget)
			{
				// Search Type
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_MSTarget_Type_SearchTarget)		&&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MSTarget_Type_SearchTargetLeft)	&&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MSTarget_Type_SearchTargetRight))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msTarget' function, 1st parameter available settings: " + "\n" +
													"-> SearchTarget"																+ "\n" +
													"-> SearchTargetLeft"															+ "\n" +
													"-> SearchTargetRight";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Target Value
				if ((Double::Parse(conditionValueRaw[1]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[1]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msTarget' function, 2nd parameter available settings: " + "\n" +
													"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Target Transition
				if ((conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Positive) &&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Negative) &&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Both))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'msTarget' function, 3rd parameter available settings: " + "\n" +
													"-> Positive"																	+ "\n" +
													"-> Negative"																	+ "\n" +
													"-> Both";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				//// Target Line
				//if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				//{ 
				//	if ((conditionValueRaw[3] != Cond_AnalysisSetting_MSTarget_Enable_TargetLineOn) &&
				//		(conditionValueRaw[3] != Cond_AnalysisSetting_MSTarget_Enable_TargetLineOff))
				//	{
				//			ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
				//			String ^ additionalMessage =	"'AnalysisSetting' for 'msTarget' function, 4th parameter available settings: " + "\n" +
				//											"-> TargetLineOn"																+ "\n" +
				//											"-> TargetLineOff";
				//			amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
				//			tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//			tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//			return ret;
				//		}
				//	}
				//}
			}
			// mmStatistics
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				// Statistics Range On/Off
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_MMStatistics_Enable_StatisticsRangeOn) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MMStatistics_Enable_StatisticsRangeOff))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'mmStatistics' function, 1st parameter available settings: "	+ "\n" +
													"-> StatisticsRangeOn"																+ "\n" +
													"-> StatisticsRangeOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmBwSearch
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)
			{
				// Search Type
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_MMBwSearch_Type_Bandpass) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MMBwSearch_Type_Notch))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'mmBwSearch' function, 1st parameter available settings: "	+ "\n" +
													"-> Bandpass"																		+ "\n" +
													"-> Notch";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Ref
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{ 
					if ((conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)	&&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum) &&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'AnalysisSetting' for 'mmBwSearch' function, 2nd parameter available settings: "	+ "\n" +
														"-> Marker"																			+ "\n" +
														"-> Max"																			+ "\n" +
														"-> Min";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker)	&&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage =	"'AnalysisSetting' for 'mmBwSearch' function, 2nd parameter available settings: "	+ "\n" +
														"-> Marker"																			+ "\n" +
														"-> Peak";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}

				// Bandwidth Value / Notch Value
				if ((Double::Parse(conditionValueRaw[2]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[2]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'mmBwSearch' function, 3rd parameter available settings: " + "\n" +
													"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmBwSearchAbs
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs)
			{
				// Search Type
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_MMBwSearch_Type_Bandpass) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_MMBwSearch_Type_Notch))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'mmBwSearchAbs' function, 1st parameter available settings: " + "\n" +
						"-> Bandpass" + "\n" +
						"-> Notch";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Ref
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker) &&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum) &&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'AnalysisSetting' for 'mmBwSearchAbs' function, 2nd parameter available settings: " + "\n" +
							"-> Marker" + "\n" +
							"-> Max" + "\n" +
							"-> Min";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker) &&
						(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'AnalysisSetting' for 'mmBwSearchAbs' function, 2nd parameter available settings: " + "\n" +
							"-> Marker" + "\n" +
							"-> Peak";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}

				// Bandwidth Value / Notch Value
				if ((Double::Parse(conditionValueRaw[2]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[2]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'mmBwSearchAbs' function, 3rd parameter available settings: " + "\n" +
						"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// mmFlatness
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				// NA
			}
			// msMinMaxRipple
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple || amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				// NA
			}
			// P1dB
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_P1dB)
			{
				// NA
			}
			// SharedMemoryDataMinMax
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				// Search Range On/Off
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataMinMax' function, 1st parameter available settings: " + "\n" +
						"-> SearchRangeOn" + "\n" +
						"-> SearchRangeOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Type
				if ((conditionValueRaw[1] != Cond_AnalysisSetting_MS_Type_Maximum) &&
					(conditionValueRaw[1] != Cond_AnalysisSetting_MS_Type_Minimum))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataMinMax' function, 2nd parameter available settings: " + "\n" +
						"-> Max" + "\n" +
						"-> Min";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// SharedMemoryDataTarget
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				// Search Range On/Off
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataTarget' function, 1st parameter available settings: " + "\n" +
						"-> SearchRangeOn" + "\n" +
						"-> SearchRangeOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Target Value
				if ((Double::Parse(conditionValueRaw[1]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[1]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataTarget' function, 2nd parameter available settings: " + "\n" +
						"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Target Transition
				if ((conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Positive) &&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Negative) &&
					(conditionValueRaw[2] != Cond_AnalysisSetting_MSTarget_TargetTransition_Both))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataTarget' function, 3rd parameter available settings: " + "\n" +
						"-> Positive" + "\n" +
						"-> Negative" + "\n" +
						"-> Both";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			// SharedMemoryDataBWSearch
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch ||
					amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearchAbs)
			{
				// Search Range On/Off
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOn) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_GenericMS_Enable_SearchRangeOff))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataTarget' function, 1st parameter available settings: " + "\n" +
						"-> SearchRangeOn" + "\n" +
						"-> SearchRangeOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Type
				if ((conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_Type_Bandpass) &&
					(conditionValueRaw[1] != Cond_AnalysisSetting_MMBwSearch_Type_Notch))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function, 2nd parameter available settings: " + "\n" +
						"-> Bandpass" + "\n" +
						"-> Notch";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// Search Ref
				if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
				{
					if ((conditionValueRaw[2] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker) &&
						(conditionValueRaw[2] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Maximum) &&
						(conditionValueRaw[2] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Minimum))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function, 3rd parameter available settings: " + "\n" +
							"-> Marker" + "\n" +
							"-> Max" + "\n" +
							"-> Min";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
				else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
				{
					if ((conditionValueRaw[2] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Marker) &&
						(conditionValueRaw[2] != Cond_AnalysisSetting_MMBwSearch_SearchRef_Peak))
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function, 3rd parameter available settings: " + "\n" +
							"-> Marker" + "\n" +
							"-> Peak";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}

				// Bandwidth Value / Notch Value
				if ((Double::Parse(conditionValueRaw[3]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[3]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage = "'AnalysisSetting' for 'SharedMemoryDataBWSearch' function, 4th parameter available settings: " + "\n" +
						"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

			}
		}

#pragma endregion

#pragma region "SmoothingSetting"

		if (conditionName == VnaDataAnalysisConditionName_SmoothingSetting)
		{
			if ((conditionValueRaw[0] != Cond_SmoothingSetting_Enable_SmoOn) &&
				(conditionValueRaw[0] != Cond_SmoothingSetting_Enable_SmoOff))
			{
				ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'SmoothingSetting' 1st parameter available settings: " + "\n" +
												"-> SmoOn"												+ "\n" +
												"-> SmoOff";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_SmoothingSetting, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}

			if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			{
				if ((Double::Parse(conditionValueRaw[1]) < 0.01) ||
					(Double::Parse(conditionValueRaw[1]) > 20.0))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'SmoothingSetting' 2nd parameter available settings: " + "\n" +
													"-> 0.01 ~ 20.0";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_SmoothingSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			{
				if ((Double::Parse(conditionValueRaw[1]) < 0.0) ||
					(Double::Parse(conditionValueRaw[1]) > 25.0))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'SmoothingSetting' 2nd parameter available settings: " + "\n" +
													"-> 0.0 ~ 25.0";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_SmoothingSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'SmoothingSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "Freq, FreqRangeStart, FreqRangeStop"

		// [Future Enchancement] Need to check each of the test function in different mode, need to check with config, start stop freq

		if ((conditionName == VnaDataAnalysisConditionName_Freq) || (conditionName == VnaDataAnalysisConditionName_FreqRangeStart) || (conditionName == VnaDataAnalysisConditionName_FreqRangeStop))
		{
			if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_TraceData || amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_P1dBTarget)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.

				for (int i = 0; i < amb7300tl->vnaDataAnalysisTPC.overallTestFreqCount; i++)
				{
					if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
					{
						if ((amb7300tl->vnaDataAnalysisTPC.testFreq[i] < 300.0 kHz) ||
							(amb7300tl->vnaDataAnalysisTPC.testFreq[i] > 9000.0 MHz))
						{
							ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'Freq' for '" + amb7300tl->vnaDataAnalysisTPC.function + "' function, the available settings: " + "\n" +
								"-> 300.0 kHz ~ 9000.0 MHz";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Freq, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Freq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Freq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
					{
						if ((amb7300tl->vnaDataAnalysisTPC.testFreq[i] < 9.0 kHz) ||
							(amb7300tl->vnaDataAnalysisTPC.testFreq[i] > 20000.0 MHz))
						{
							ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'Freq' for '" + amb7300tl->vnaDataAnalysisTPC.function + "' function, the available settings: " + "\n" +
								"-> 9.0 kHz ~ 20000.0 MHz";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Freq, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Freq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Freq' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}

					if (amb7300tl->vnaDataAnalysisTPC.testFreqRangeStart[i] != 0.0 MHz)
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'FreqRangeStart' is unavailable for '" + amb7300tl->vnaDataAnalysisTPC.function + "' function, please remain the condition value as 0.0 MHz.";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_FreqRangeStart, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'FreqRangeStart' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'FreqRangeStart' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}

					if (amb7300tl->vnaDataAnalysisTPC.testFreqRangeStop[i] != 0.0 MHz)
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'FreqRangeStop' is unavailable for '" + amb7300tl->vnaDataAnalysisTPC.function + "' function, please remain the condition value as 0.0 MHz.";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_FreqRangeStop, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'FreqRangeStop' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'FreqRangeStop' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMax)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msPeak)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msTarget)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple || amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_P1dB)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch)
			{
				// [Future Enchancement] To check whether the user input freq is exist in the segment settings.
			}
		}

#pragma endregion

#pragma region "Power, PowerRangeStart, PowerRangeStop"

		// [Future Enchancement] Need to check each of the test function in different mode, need to check with config, start stop power

		if ((conditionName == VnaDataAnalysisConditionName_Power) || (conditionName == VnaDataAnalysisConditionName_PowerRangeStart) || (conditionName == VnaDataAnalysisConditionName_PowerRangeStop))
		{
			if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_P1dBTarget)
			{
				for (int i = 0; i < amb7300tl->vnaDataAnalysisTPC.overallTestPowerCount; i++)
				{
					if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
					{
						if ((amb7300tl->vnaDataAnalysisTPC.testPower[i] < -50 dBm) ||
							(amb7300tl->vnaDataAnalysisTPC.testPower[i] > 15 dBm))
						{
							ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'Power' for 'P1dBTarget' function, the available settings: " + "\n" +
								"-> -50 dBm ~ 15 dBm";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Power, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}
					else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
					{
						if ((amb7300tl->vnaDataAnalysisTPC.testPower[i] < -100 dBm) ||
							(amb7300tl->vnaDataAnalysisTPC.testPower[i] > 20 dBm))
						{
							ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
							String ^ additionalMessage = "'Power' for 'P1dBTarget' function, the available settings: " + "\n" +
								"-> -100 dBm ~ 20 dBm";
							amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_Power, additionalMessage);
							tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'Power' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							return ret;
						}
					}

					if (amb7300tl->vnaDataAnalysisTPC.testPowerRangeStart[i] != 0.0 MHz)
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'PowerRangeStart' is unavailable for 'TraceData' function, please remain the condition value as 0.0 MHz.";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_PowerRangeStart, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'PowerRangeStart' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'PowerRangeStart' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}

					if (amb7300tl->vnaDataAnalysisTPC.testPowerRangeStop[i] != 0.0 MHz)
					{
						ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
						String ^ additionalMessage = "'PowerRangeStop' is unavailable for 'TraceData' function, please remain the condition value as 0.0 MHz.";
						amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_PowerRangeStop, additionalMessage);
						tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'PowerRangeStop' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'PowerRangeStop' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						return ret;
					}
				}
			}
			
		}

#pragma endregion

#pragma region "OutputType"

		if (conditionName == VnaDataAnalysisConditionName_OutputType)
		{
			if ((amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_TraceData)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMax)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msPeak)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msTarget)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)|| 
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget))
			{
				if ((amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_Generic_Freq) &&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_Generic_Data))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'TraceData', 'msMinMax', 'msPeak, 'msTarget', 'SharedMemoryDataMinMax, 'SharedMemoryDataTarget' function,"	+ "\n" +
													"the available settings: "																										+ "\n" +
													"-> Freq"																														+ "\n" +
													"-> Data";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)
			{
				if ((amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmStatistics_Mean)		&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmStatistics_StdDev)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmStatistics_PeakToPeak))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'mmStatistics' function, the available settings: "	+ "\n" +
													"-> Mean"																+ "\n" +
													"-> StdDev"																+ "\n" +
													"-> PeakToPeak";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if ((amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)			||
					(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs)			||
					(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch))
			{
				if ((amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_BW)		&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_Cent)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_Low)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_High)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_Q)		&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmBwSearch_Loss))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'mmBwSearch', 'mmBwSearchAbs', 'SharedMemoryDataBWSearch' function, the available settings: "	+ "\n" +
													"-> BW"																											+ "\n" +
													"-> Cent"																										+ "\n" +
													"-> Low"																										+ "\n" +
													"-> High"																										+ "\n" +
													"-> Q"																											+ "\n" +
													"-> Loss";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)
			{
				if ((amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmFlatness_Gain)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmFlatness_Slope)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_mmFlatness_Flat))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'mmFlatness' function, the available settings: "	+ "\n" +
													"-> Gain"															+ "\n" +
													"-> Slope"															+ "\n" +
													"-> Flat";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
			else if (amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple || amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple)
			{
				if (amb7300tl->vnaDataAnalysisTPC.outputType != Cond_OutputType_Generic_Data)
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'msMinMaxRipple', 'SharedMemoryMinMaxRipple' function, the available settings: " + "\n" +
													"-> Data";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "OutputFormat"

		if (conditionName == VnaDataAnalysisConditionName_OutputFormat)
		{
			if ((amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_TraceData)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMax)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msPeak)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msTarget)				||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_msMinMaxRipple)		||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataMinMax)||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataTarget)||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryMinMaxRipple))
			{
				if ((amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_Generic_Real)			&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_Generic_Imag)			&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithRJX_Resistance)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithRJX_Reactance)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithRJX_CapInduc)		&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithGJB_Conductance)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithGJB_Susceptance)	&&
					(amb7300tl->vnaDataAnalysisTPC.outputFormat != Cond_OutputFormat_SmithGJB_CapInduc))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'TraceData', 'msMinMax', 'msPeak, 'msTarget', 'SharedMemoryDataMinMax, 'SharedMemoryDataTarget' function," + "\n" +
													"the available settings: "																								+ "\n" +
													"-> Real"																												+ "\n" +
													"-> Imag"																												+ "\n" +
													"-> Resistance"																											+ "\n" +
													"-> Reactance"																											+ "\n" +
													"-> CapInduc"																											+ "\n" +
													"-> Conductance"																										+ "\n" +
													"-> Susceptance";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputFormat, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}

			if ((amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmStatistics)	||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearch)	||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmBwSearchAbs) ||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_mmFlatness)	||
				(amb7300tl->vnaDataAnalysisTPC.function == Cond_Function_SharedMemoryDataBWSearch))
			{
				if (amb7300tl->vnaDataAnalysisTPC.outputFormat != "")
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputFormat' is unavailable for 'mmStatistics', 'mmBwSearch', 'mmFlatness', 'SharedMemoryDataBWSearch' function, please remain the condition value empty.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_OutputFormat, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

//#pragma region "Threshold"
//
//		if (conditionName == VnaDataAnalysisConditionName_Threshold)
//		{
//			if (amb7300tl->vnaDataAnalysisTPC.function != Cond_Function_msPeak)
//			{
//				//set threshold of other function to 0 except msPeak
//				amb7300tl->vnaDataAnalysisTPC.Threshold = 0.0;
//			}
//		}
//
//#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> VnaDataAnalysis
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_VnaSwTime(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_VnaSwTime
		**	tfSite				- This is techFlow site object.
		**	siteIndex			- This is techFlow site index.
		**	conditionName		- This is the condition name.
		**	conditionValueRaw	- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in the 'VnaSwTime'.
		**		Include: 'TraceIndex', 'Function', 'AnalysisSetting', 'SmoothingSetting', 
		**		'Freq', 'FreqRangeStart', 'FreqRangeStop', 'OutputType', 'OutputFormat'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "MeasureType"

		if (conditionName == VnaSwTimeConditionName_MeasureType)
		{
			if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
			{
				//[Future Improvement]
			}
			else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
			{
				if ((amb7300tl->vnaSwTimeTPC.measureType != Cond_MeasureType_DM_PXIE9852)					)
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'Function' available settings: "	+ "\n" +
						"-> DM+PXIE9852"						;
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_MeasureType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'MeasureType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'MeasureType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "DMModuleAlias"

		if (conditionName == VnaSwTimeConditionName_DMModuleAlias)
		{
			//if ((amb7300tl->vnaSwTimeTPC.dmModuleAlias))
			{
				////[Future Improvement] Compare if module alias is an DM module
			}

		}

#pragma endregion

#pragma region "DMVectorTrigger"

		if (conditionName == VnaSwTimeConditionName_DMVectorTrigger)
		{
			if ((amb7300tl->vnaSwTimeTPC.measureType == Cond_MeasureType_DM_PXIE9852))
			{
				////[Future Improvement] Compare if vector exist from global vector list variable
				//ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
				//String ^ additionalMessage =	"'SmoothingSetting' 1st parameter available settings: " + "\n" +
				//	"-> VectorName1";
				//amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_DMVectorTrigger, additionalMessage);
				//tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'DMVectorTrigger' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'DMVectorTrigger' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				//return ret;
			}

		}

#pragma endregion

#pragma region "AnalysisSetting"

		if (conditionName == VnaSwTimeConditionName_AnalysisSetting)
		{
			// DM+PXIE9852
			if (amb7300tl->vnaSwTimeTPC.analysisSetting == Cond_MeasureType_DM_PXIE9852)
			{
				// AnalysisSetting parameter: TriggerEdge | MovingAverage | SearchLevel | SearchLimitLevel
				if (conditionValueRaw->Length != 4)
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'VnaSwTime' function contain 2x parameter which is differentiate by '/'." + "\n" +  
						"1st parameter: Trigger edge."													+ "\n" + 
						"2nd parameter: Enable Moving Average calculation."								+ "\n" + 
						"3rd parameter: Enable search limit."											+ "\n" + 
						"4th parameter: Search limit level."											+ "\n" + 
						"Example: SearchRangeOn/Max --> meaning enable marker search range, searching for maximum value.";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value for 'VnaSwTime' mode is invalid. It should contain 4 value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value for 'VnaSwTime' mode is invalid. It should contain 4 value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
				// Rising/Falling Edges
				if ((conditionValueRaw[0] != Cond_AnalysisSetting_VnaSwTime_Rising) &&
					(conditionValueRaw[0] != Cond_AnalysisSetting_VnaSwTime_Falling))
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'VnaSwTime' function, 1st parameter available settings: "	+ "\n" +
						"-> Rising"																+ "\n" +
						"-> Falling";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// MovingAvg On/Off
				if ((conditionValueRaw[1] != Cond_AnalysisSetting_VnaSwTime_MovingAvgOn) && 
					(conditionValueRaw[1] != Cond_AnalysisSetting_VnaSwTime_MovingAvgOff))
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'VnaSwTime' function, 2nd parameter available settings: "	+ "\n" +
						"-> MovingAvgOn"																		+ "\n" +
						"-> MovingAvgOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// SearchLimit On/Off
				if ((conditionValueRaw[2] != Cond_AnalysisSetting_VnaSwTime_SearchLimitOn) && 
					(conditionValueRaw[2] != Cond_AnalysisSetting_VnaSwTime_SearchLimitOff))
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'VnaSwTime' function, 3rd parameter available settings: "	+ "\n" +
						"-> SearchLimitOn"																		+ "\n" +
						"-> SearchLimitOff";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}

				// SwTime Power Limit Value
				if ((Double::Parse(conditionValueRaw[3]) < -500.0 dB) ||
					(Double::Parse(conditionValueRaw[3]) > 500.0 dB))
				{
					ret = ER_CONST_VNADATAANALYSIS_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'AnalysisSetting' for 'VnaSwTime' function, 4th parameter available settings: " + "\n" +
						"-> -500.0 dB ~ +500.0 dB";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaDataAnalysisConditionName_AnalysisSetting, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaDataAnalysis' condition value verification] 'AnalysisSetting' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "OutputType"

		if (conditionName == VnaSwTimeConditionName_OutputType)
		{
			if ((amb7300tl->vnaSwTimeTPC.measureType == Cond_MeasureType_DM_PXIE9852))
			{
				if ((amb7300tl->vnaSwTimeTPC.outputType != Cond_OutputType_Generic_Data))
				{
					ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
					String ^ additionalMessage =	"'OutputType' for 'mmBwSearch', 'mmBwSearchAbs', 'SharedMemoryDataBWSearch' function, the available settings: "	+ "\n" +
						"-> Data";
					amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_OutputType, additionalMessage);
					tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'OutputType' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					return ret;
				}
			}
		}

#pragma endregion

#pragma region "OutputFormat"

		if (conditionName == VnaSwTimeConditionName_OutputFormat)
		{
			if ((amb7300tl->vnaSwTimeTPC.outputFormat != Cond_OutputFormat_VnaSwTime_Time)				||
				(amb7300tl->vnaSwTimeTPC.outputFormat != Cond_OutputFormat_VnaSwTime_MaxPower)				||
				(amb7300tl->vnaSwTimeTPC.outputFormat != Cond_OutputFormat_VnaSwTime_MinPower)				||
				(amb7300tl->vnaSwTimeTPC.outputFormat != Cond_OutputFormat_VnaSwTime_AvgPower))
			{
				ret = ER_CONST_VNASWTIME_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'OutputType' for 'VnaSwTime' function," + "\n" +
					"the available settings: "																								+ "\n" +
					"-> Time"																												+ "\n" +
					"-> MaxPower"																												+ "\n" +
					"-> MinPower"																											+ "\n" +
					"-> AvgPower";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, VnaSwTimeConditionName_OutputFormat, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['VnaSwTime' condition value verification] 'OutputFormat' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Validate condition value input ---> Math
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::ValidateConditionValueInput_Math(int tfSite, int siteIndex, String ^ conditionName, array<String^> ^ conditionValueRaw)
	{
		/*****************************************************************************************************
		**	ValidateConditionValueInput_VnaDataAnalysis
		**	tfSite				- This is techFlow site object.
		**	siteIndex			- This is techFlow site index.
		**	conditionName		- This is the condition name.
		**	conditionValueRaw	- This is the condition value input (raw) in the array form.
		**
		**	Descriptions:
		**		This function is to validate all the condition value input in the 'Math'.
		**		Include: 'MathFunction', 'MathTP1', 'MathTP2'.
		******************************************************************************************************/

		int ret = 0;

#pragma region "MathFunction"

		if (conditionName == MathTestConditionName_MathFunction)
		{
			if ((amb7300tl->MathTPC.mathFunction != Cond_TestFunction_Add)			&&
				(amb7300tl->MathTPC.mathFunction != Cond_TestFunction_Subtract)	&&
				(amb7300tl->MathTPC.mathFunction != Cond_TestFunction_AbsAdd)		&&
				(amb7300tl->MathTPC.mathFunction != Cond_TestFunction_AbsSubtract))
			{
				ret = ER_CONST_MATH_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage =	"'MathFunction' available settings: " + "\n" +
												"-> Add"							+ "\n" +
												"-> Subtract"							+ "\n" +
												"-> AbsAdd"							+ "\n" +
												"-> AbsSubtract";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, MathTestConditionName_MathFunction, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathFunction' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "TP1"

		if(conditionName == MathConditionName_TP1)
		{

			if (!tl->glob->TestProperty[siteIndex].TestResults->ContainsKey(amb7300tl->MathTPC.mathTP1))
			{
				ret = ER_CONST_MATH_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "Test Parameter: " + amb7300tl->MathTPC.mathTP1 + " is not available!\n";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, MathTestConditionName_MathFunction, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathTP1' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathTP1' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion

#pragma region "TP2"

		if (conditionName == MathConditionName_TP2)
		{

			if (!tl->glob->TestProperty[siteIndex].TestResults->ContainsKey(amb7300tl->MathTPC.mathTP2))
			{
				ret = ER_CONST_MATH_CONDITION_VALUE_INPUT_INVALID;
				String ^ additionalMessage = "Test Parameter: " + amb7300tl->MathTPC.mathTP2 + " is not available!\n";
				amb7300tl->ShowMessageBox(tfSite, siteIndex, TITLE_CONST_INVALID_CONDITION_INPUT_VALUE, MathTestConditionName_MathFunction, additionalMessage);
				tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathTP2' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "['Math' condition value verification] 'MathTP2' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

#pragma endregion


		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Check condition change ---> VnaConfig
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::CheckConditionChange_VnaConfig(Site ^ site, int tfSite, int siteIndex, int segmentSetCount)
	{
		/*****************************************************************************************************
		**	CheckConditionChange_VnaConfig
		**	site			- This is techFlow site object.
		**	tfSite			- This is techFlow site index.
		**	siteIndex		- This is vna's object index, normally start from 0.
		**	segmentSetCount	- This is total segment set count of the VNA segment settings.
		**
		**	Descriptions:
		**		This function is to check condition change from the control step item 'VnaConfig'.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		
		// Compare each segment settings. Support multiple segment set
		for (int segmentSetIndex = 0; segmentSetIndex < amb7300tl->segmentSetting[siteIndex].segmentCount; segmentSetIndex++)
		{
			//-------------------- Start Freq --------------------
			if ((amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex]	!= amb7300tl->prev_segmentSetting[siteIndex].startFreq[segmentSetIndex])	||
				(amb7300tl->segmentSetting[siteIndex].stopFreq[segmentSetIndex]		!= amb7300tl->prev_segmentSetting[siteIndex].stopFreq[segmentSetIndex])		||
				(amb7300tl->segmentSetting[siteIndex].point[segmentSetIndex]		!= amb7300tl->prev_segmentSetting[siteIndex].point[segmentSetIndex])		||
				(amb7300tl->segmentSetting[siteIndex].ifbw[segmentSetIndex]			!= amb7300tl->prev_segmentSetting[siteIndex].ifbw[segmentSetIndex])			||
				(amb7300tl->segmentSetting[siteIndex].power[segmentSetIndex]		!= amb7300tl->prev_segmentSetting[siteIndex].power[segmentSetIndex])		||
				(amb7300tl->segmentSetting[siteIndex].delay[segmentSetIndex]		!= amb7300tl->prev_segmentSetting[siteIndex].delay[segmentSetIndex]))
			{
				if ((amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex] == 0.0) &&
					(amb7300tl->segmentSetting[siteIndex].stopFreq[segmentSetIndex] == 0.0) &&
					(amb7300tl->segmentSetting[siteIndex].point[segmentSetIndex] == 0) &&
					(amb7300tl->segmentSetting[siteIndex].ifbw[segmentSetIndex] == 0.0) &&
					(amb7300tl->segmentSetting[siteIndex].power[segmentSetIndex] == 0.0) &&
					(amb7300tl->segmentSetting[siteIndex].delay[segmentSetIndex] == 0.0))
				{
					amb7300tl->segmentSetting[siteIndex].SettingHasChanged = false;
				}
				else
				{
					amb7300tl->segmentSetting[siteIndex].SettingHasChanged = true;
					amb7300tl->prev_segmentSetting[siteIndex].startFreq[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].startFreq[segmentSetIndex];
					amb7300tl->prev_segmentSetting[siteIndex].stopFreq[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].stopFreq[segmentSetIndex];
					amb7300tl->prev_segmentSetting[siteIndex].point[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].point[segmentSetIndex];
					amb7300tl->prev_segmentSetting[siteIndex].ifbw[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].ifbw[segmentSetIndex];
					amb7300tl->prev_segmentSetting[siteIndex].power[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].power[segmentSetIndex];
					amb7300tl->prev_segmentSetting[siteIndex].delay[segmentSetIndex] = amb7300tl->segmentSetting[siteIndex].delay[segmentSetIndex];
				}
			}
			else
			{
				amb7300tl->segmentSetting[siteIndex].SettingHasChanged = false;
			}
		}

		EndOfTest:
				 return ret;
	}
	
}


/*----------------------------------------------------------------------
* Revision Log
* &Log: AMB7300Utility.cpp.rca&

*** Version	: v1.0.0.11
*** Date	: 2 September 2026
*** PIC		: Xuli & Zhi Kean
* Updated TestLib_ControlStep_VnaConfig_CastCondition for segmentSetting[siteIndex].totalPoint
* Added support for DmControl -> ConfigureOutputTrigger for SW time test

*** Version : v1.0.0.10
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Added xml code for gCreateFixedOffsetFile
* Solved bug on CheckConditionChange_VnaConfig logic
* Add PreProcessing xml condition CorrFactorDirectory
* Add AWV for HighPwrTest_EN & HighPwrTest_AppsCalFile
* Add AWV for PortMatchingX
* Add VNA_Lock & VNA_Unlock xml code
* Solve bug on SharedMemoryTransfer: amb7300tl->segmentSetting[siteIndex].segmentCount = segmentSetCount
* Support Trace Measure Type of A & B
* Move ChannelNum from VnaFetch to VnaConfig

*** Version : v1.0.0.9
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Added VnaConfig CheckConditionChange
* Updated xml configurations & condition inputs for AppsWideVariable, VnaConfig, VnaFetch & VnaDataAnalysis

*** Version : v1.0.0.8
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Updated XML file for VnaConfig and AppsWideVariable
* Added ChannelNum to VnaFetch XML file rules
* Migrate SaveToTouchstoneFile from VnaFetch_CastCondition & ValidateConditionInput to Globals.cpp

*** Version : v1.0.0.7
*** Date	: xx April 2024
*** PIC		: Jack Ho
* Target Search for BW Search
* Combine 3 S2P into S3p by input the "S3P" into the SNP condition.
* Snp file able to store into different folder/pathway based on the waferID/LotID
* Cal Expiration Check

*** Version	:  v1.0.0.6
*** Date	:  xx December 2023
*** PIC		: Jack Ho
* Fixed bug on Initialization for true paralel techflow site configurations for multi-system. (Config file at Config1 or ConfigX folder will have additional information on Config to identify between different config)
* AMB7300

*** Version	:  v1.0.0.5
*** Date	:  7 November 2023
*** PIC		: Jack Ho
* Added support for new BW Test method (mmBwSearchAbs) where it will capture the max value and calculate the new absolute BW reference value

*** Version	:  v1.0.0.4
*** Date	:  25 August 2023
*** PIC		: Jack Ho
* Added support for new Math Function. (Add, Subtract, AbsAdd, AbsSubtract)
* Added support for auto points obtain from the state file (thru S2VNA) where the users no longer required to key in the number of points in test conditions.

*** Version	:  v1.0.0.3
*** Date	:  13 August 2023
*** PIC		: Jack Ho
* Support control step and test parameters bypass
* Added support for multiUUT for Shared VNA, new project type: SingleTFSiteMultiUUTOffsetSharedVNA
* Updated XML file for SharedVNA AppWideVariable
* Changed SNP file name up to ms to avoid overwritten when the name is up to seconds.
* Support FT and CP, S2P generation. (Able to identify if sublot is enabled)
* Temporary supports only AMB7300 but not AMB7300SR!!

*** Version	: v1.0.0.2
*** Date	: 9 April 2023
*** PIC		: Ng Chen Yang
* Support the input validation of S2Ppath and SaveSnpData which moved to AppWideVariable
* Updated XML file for VNA FETCH and AppWideVariable

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* SaveSnpFilePath to store the snai file to a specific file path 
* Detect the keyword "SkySemi" to enable special bw seacrh (reference to peak)

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version. 
* Support AM, DM series configuration.
* Support CMT VNA SC5090, Keysight VNA M9804A configuration.
* Support user input validation.
* Support tF3.12.1 project library XML file.
----------------------------------------------------------------------*/