/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			TestMethod.cpp
Purpose:		To execute test function for
				DcControl, DcTest, 
				PatternControl, PatternTest,
				VnaConfig, VnaFetch, VnaDataAnalysis
Version:		v1.0.0.5
----------------------------------------------------------------------*/


#include "..\\TestProgram.h"

namespace AMB7300_TestLibrary_REV2P0
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Test Method for Dc, Pattern, Vna test items
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestProgram::seq_TestMethod(Site ^ site)
	{
		/*****************************************************************************************************
		**	seq_TestMethod
		**		site - This is techFlow site object.
		**
		**	Descriptions:
		**		This is a function to perform all the configuration and testing for all kind of flow items, 
		**		such as DcControl, DcTest, PatternControl, PatternTest, VnaConfig, VnaFetch, VnaDataAnalysis.
		**		Get test property from tF3, and then return test result to display at tF3.
		******************************************************************************************************/

#pragma region "Local variables"

		int ret = 0;

		// Test item object
		TestItem ^ testItem;

		// Every sub flow item inside a test item. Eg: test parameter, control step, test step
		AFlowSubItem ^ subFlowItem;

		// Test step object
		TestStep ^ testStep;

		// Test condition object
		ConditionCollection ^ conditionCollection = gcnew ConditionCollection;
		Condition ^ condition;

		// Local result variable
		array<double> ^ result = gcnew array<double>(tl->glob->tf.NumberOfTestSites);
		resultIndex = gcnew array<int>(tl->glob->tf.NumberOfTestSites);
		tl->glob->currentSubItemName = gcnew array<String^>(tl->glob->tf.NumberOfTestSites);


		//Save only one snp in multi-VnaFetch
		bool FirstSaveSnpForMultiFetch;

#pragma endregion

		// Get current phase
		tl->glob->tf.CurrentPhase = site->CurrentPhase->Name;

		// Get current test site
		int tfSite = tl->glob->tf.TestSite;

#pragma region "Update Test Property"

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				tl->UpdateTestProperty(site, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}

#pragma endregion

#pragma region "Get current activeUUT count & map"
		
		//Reset variable to deal with activeUUT change in Engineering Mode
		tl->glob->tf.activeUUT_count = 0;

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{

			tl->glob->tf.arr_activeUUT[siteIndex] = false;
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				tl->glob->tf.activeUUT_count++;
				tl->glob->tf.arr_activeUUT[siteIndex] = true;
			}
		}

#pragma endregion

#pragma region "Reset result index & Get current test item name"
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				// Reset index for next UUT
				resultIndex[siteIndex] = 0;

				// Get current test item object
				testItem = (TestItem^)site->FlowItems[tl->glob->TestProperty[siteIndex].TestItemName];

				//Initialize test parameter results storage
				TPtestResult = gcnew array<Object ^, 2>(tl->glob->tf.NumberOfTestSites, tl->glob->TestProperty[siteIndex].totalTestParameter);
	
				// Get current test item's name
				tl->glob->currentFlowName = testItem->Name;
			}
		}
#pragma endregion

#pragma region "Execute each control step item & test parameter item"

		/*
		**	Actual tF site 0 / site 1 / ... comes in --> use siteIndex start from 0.
		**	The siteIndex < XXX is reserve for the multi uut offset project, where XXX is the total uut offset number.
		*/

		// This part only support for MultiUUT SharedVNA Projects (hard coded only one set of vna hardware
		if (((amb7300tl->amb7300SystemSetting->systemAlias == AMB7300_S6P1D_TYPE) || (amb7300tl->amb7300SystemSetting->systemAlias == AMB7300_S4P1D_TYPE)) &&
			tl->glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA) &&
			amb7300tl->amb7300SystemSetting->vnaSystemCount == 1)
		{

			int vnaSiteIndex = 0;	// Indexing for actual VNA hardware
			int siteIndex = 0;		// Indexing for techFlow test site

			for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
			{
				if (site->UUTOffsetResolver->UUTOffsets[l_siteIndex]->Active)
				{
					siteIndex = l_siteIndex; // Reassign siteIndex with first active UUT siteIndex value
				}
			}
				
			//Control Step & Test Parameter Count
			int csCount = 0;
			int tpCount = 0;

			// Loop every flow items in sequence (control step item, test parameter item)
			for each(subFlowItem in testItem->SubItems)
			{
				for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
				{
					// Get current flow item's name
					tl->glob->currentSubItemName[l_siteIndex] = subFlowItem->Name;
				}

				if (dynamic_cast<ControlStep^>(subFlowItem->Data) != nullptr)
				{
					AFlowStep^ flowStep = (AFlowStep^)testItem->FlowSteps[tl->glob->TestProperty[siteIndex].FlowStepItemName[csCount]];

					if (!flowStep->Bypass)
					{
						if ((tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_DC_CONTROL)) ||
							(tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_PATTERN_CONTROL)))
						{
							DoThread(gcnew ParameterizedThreadStart(this, &TestProgram::ExecuteControlStep_DC), site);
						}
						else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_VNA_CONFIG))
						{
							int startFreqCount = 0;
							int stopFreqCount = 0;
							int pointsCount = 0;
							int ifbwCount = 0;
							int powerCount = 0;
							int delayCount = 0;
							int segmentSetCount = 0;

							conditionCollection = tf_FlowStep_ConditionList(tl->glob->currentSubItemName[siteIndex]);
							for each (condition in conditionCollection)
							{
								if (condition->Name->Contains(VnaConfigConditionName_StratFreq))
									startFreqCount++;

								if (condition->Name->Contains(VnaConfigConditionName_StopFreq))
									stopFreqCount++;

								if (condition->Name->Contains(VnaConfigConditionName_Points))
									pointsCount++;

								if (condition->Name->Contains(VnaConfigConditionName_Ifbw))
									ifbwCount++;

								if (condition->Name->Contains(VnaConfigConditionName_Power))
									powerCount++;

								if (condition->Name->Contains(VnaConfigConditionName_Delay))
									delayCount++;
							}

							// [CHECKING] Segment setting's conditions
							if ((startFreqCount != stopFreqCount) ||
								(startFreqCount != pointsCount) ||
								(startFreqCount != ifbwCount) ||
								(startFreqCount != powerCount) ||
								(startFreqCount != delayCount))
							{
								ret = ER_CONST_VNACONFIG_CONDITION_SEGMENT_SET_INVALID;
								amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[VnaConfig] VNA segment set invalid. 1x of complete segment set contains 'StartFreq' & 'StopFreq' & 'Points' & 'Ifbw' & 'Power' & 'Delay'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[VnaConfig] VNA segment set invalid. 1x of complete segment set contains 'StartFreq' & 'StopFreq' & 'Points' & 'Ifbw' & 'Power' & 'Delay'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								goto EndOfTest;
							}
							else
							{
								segmentSetCount = startFreqCount;
							}

							for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
							{
								if (site->UUTOffsetResolver->UUTOffsets[l_siteIndex]->Active)
								{
									// Cast condition from 'VnaConfig' & perform the rest of the necessary settings @ AMB7300Utility
									ret = TestLib_ControlStep_VnaConfig_CastCondition(site, tfSite, l_siteIndex, segmentSetCount);
									if (ret != 0) goto EndOfTest;
									
									ret = CheckConditionChange_VnaConfig(site, tfSite, l_siteIndex, segmentSetCount);
									if (ret != 0) goto EndOfTest;
								}
							}

							// Execute 'VnaConfig' phase @ AMB7300
							ret = amb7300tl->VnaConfig(tfSite, vnaSiteIndex);
							if (ret != 0) goto EndOfTest;

						}
						else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_VNA_FETCH))
						{
							for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
							{
								if (site->UUTOffsetResolver->UUTOffsets[l_siteIndex]->Active)
								{
									// Cast condition from 'VnaFetch' & perform the rest of the necessary settings @ AMB7300Utility
									ret = TestLib_ControlStep_VnaFetch_CastCondition(site, tfSite, l_siteIndex);
									if (ret != 0) goto EndOfTest;
								}
							}
							// Execute 'VnaFetch' phase @ AMB7300
							ret = amb7300tl->VnaFetch_TrueParallel(tfSite, vnaSiteIndex);
							if (ret != 0) goto EndOfTest;

							for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
							{
								if (site->UUTOffsetResolver->UUTOffsets[l_siteIndex]->Active)
								{
									if (tl->glob->AWV.EnableSaveSnpData == true && amb7300tl->saveRecallSetting->EnableSaveSnpData == true)
									{
										ret = amb7300tl->SaveToTouchstoneFile(site, tfSite, l_siteIndex);
										if (ret != 0) goto EndOfTest;
									}
								}
							}
						}
					}
					else
					{
					}
					csCount++;
				}
				else if (dynamic_cast<TestParameter^>(subFlowItem->Data) != nullptr)
				{
					TestParameter^ tp = (TestParameter^)testItem->TestParameters[tl->glob->TestProperty[siteIndex].TestParameterName[tpCount]];

					if (!tp->Bypass)
					{
						// Identify the phase type in the test parameter item
						String ^ currentPhase = String::Empty;
						ret = amb7300tl->IdentifyTestParameterPhaseType(site, tfSite, siteIndex, currentPhase);
						if (ret != 0) goto EndOfTest;

						if ((currentPhase == PHASE_CONST_DC_TEST) || (currentPhase == PHASE_CONST_PATTERN_TEST) || (currentPhase == PHASE_CONST_MATH))
						{
							DoThread(gcnew ParameterizedThreadStart(this, &TestProgram::ExecuteTestParameter_DC), site);
						}
						else if (currentPhase == PHASE_CONST_VNA_DATA_ANALYSIS)
						{
							for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
							{
								if (site->UUTOffsetResolver->UUTOffsets[l_siteIndex]->Active)
								{
									// Cast condition from 'VnaDataAnalysis' @ AMB7300Utility
									ret = TestLib_TestParameter_VnaDataAnalysis_CastCondition(site, tfSite, l_siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'VnaDataAnalysis' phase @ AMB7300
									result[l_siteIndex] = (double)CONST_INVALID_RESULT;
									amb7300tl->VnaDataAnalysis_TrueParallel(tfSite, l_siteIndex, result[l_siteIndex]);

									// Save to result object
									tl->glob->TestProperty[siteIndex].IsHardwareInvolved[resultIndex[siteIndex]] = true; //Set true for hardware result duplicate checking
									TPtestResult[l_siteIndex, resultIndex[l_siteIndex]] = result[l_siteIndex];
									resultIndex[l_siteIndex]++;
								}
							}
						}

					}
					else
					{
						resultIndex[siteIndex]++;
					}
					tpCount++;

				}
			}

			for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
			{
				// Cast test result to techFlow3
				ret = tl->UpdateTestResultWithOffsetToTechFlow(site, l_siteIndex, TPtestResult);
			}
		}
		else			//Sequentiel Testing.
		{
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
				{
					//Control Step & Test Parameter Count
					int csCount = 0;
					int tpCount = 0;

					// Loop every flow items in sequence (control step item, test parameter item)
					for each(subFlowItem in testItem->SubItems)
					{
						// Get current flow item's name
						tl->glob->currentSubItemName[siteIndex] = subFlowItem->Name;

						if (dynamic_cast<ControlStep^>(subFlowItem->Data) != nullptr)
						{
							AFlowStep^ flowStep = (AFlowStep^)testItem->FlowSteps[tl->glob->TestProperty[siteIndex].FlowStepItemName[csCount]];

							if (!flowStep->Bypass)
							{
								if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_DC_CONTROL))
								{
									// Cast condition from 'DcControl' @ AMB7300Utility
									ret = TestLib_ControlStep_DcControl_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'DcControl' phase @ AMB7300
									ret = amb7300tl->DcControl(tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;
								}
								else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_PATTERN_CONTROL))
								{
									// Cast condition from 'PatternControl' @ AMB7300Utility
									ret = TestLib_ControlStep_PatternControl_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'PatternControl' phase @ AMB7300
									ret = amb7300tl->PatternControl(tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;
								}
								else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_VNA_CONFIG))
								{
									int startFreqCount = 0;
									int stopFreqCount = 0;
									int pointsCount = 0;
									int ifbwCount = 0;
									int powerCount = 0;
									int delayCount = 0;
									int segmentSetCount = 0;

									lineNUM = (gcnew System::Diagnostics::StackFrame(0, true))->GetFileLineNumber();
									conditionCollection = tf_FlowStep_ConditionList(tl->glob->currentSubItemName[siteIndex]);
									for each (condition in conditionCollection)
									{
										if (condition->Name->Contains(VnaConfigConditionName_StratFreq))
											startFreqCount++;

										if (condition->Name->Contains(VnaConfigConditionName_StopFreq))
											stopFreqCount++;

										if (condition->Name->Contains(VnaConfigConditionName_Points))
											pointsCount++;

										if (condition->Name->Contains(VnaConfigConditionName_Ifbw))
											ifbwCount++;

										if (condition->Name->Contains(VnaConfigConditionName_Power))
											powerCount++;

										if (condition->Name->Contains(VnaConfigConditionName_Delay))
											delayCount++;
									}

									// [CHECKING] Segment setting's conditions
									if ((startFreqCount != stopFreqCount) ||
										(startFreqCount != pointsCount) ||
										(startFreqCount != ifbwCount) ||
										(startFreqCount != powerCount) ||
										(startFreqCount != delayCount))
									{
										ret = ER_CONST_VNACONFIG_CONDITION_SEGMENT_SET_INVALID;
										amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, WARNING, "[VnaConfig] VNA segment set invalid. 1x of complete segment set contains 'StartFreq' & 'StopFreq' & 'Points' & 'Ifbw' & 'Power' & 'Delay'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
										amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, WARNING, "[VnaConfig] VNA segment set invalid. 1x of complete segment set contains 'StartFreq' & 'StopFreq' & 'Points' & 'Ifbw' & 'Power' & 'Delay'." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
										goto EndOfTest;
									}
									else
									{
										segmentSetCount = startFreqCount;
									}

									// Cast condition from 'VnaConfig' & perform the rest of the necessary settings @ AMB7300SRUtility
									ret = TestLib_ControlStep_VnaConfig_CastCondition(site, tfSite, siteIndex, segmentSetCount);
									if (ret != 0) goto EndOfTest;

									ret = CheckConditionChange_VnaConfig(site, tfSite, siteIndex, segmentSetCount);
									if (ret != 0) goto EndOfTest;

									// Execute 'VnaConfig' phase @ AMB7300SR
									ret = amb7300tl->VnaConfig(tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;
								}
								else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_VNA_FETCH))
								{
									// Cast condition from 'VnaFetch' & perform the rest of the necessary settings @ AMB7300Utility
									lineNUM = (gcnew System::Diagnostics::StackFrame(0, true))->GetFileLineNumber();
									ret = TestLib_ControlStep_VnaFetch_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'VnaFetch' phase @ AMB7300
									ret = amb7300tl->VnaFetch(tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									if (tl->glob->AWV.EnableSaveSnpData == true && amb7300tl->saveRecallSetting->EnableSaveSnpData == true)
									{
										ret = amb7300tl->SaveToTouchstoneFile(site, tfSite, siteIndex);
										if (ret != 0) goto EndOfTest;
									}
								}
							}
							else
							{
							}
							csCount++;
						}
						else if (dynamic_cast<TestParameter^>(subFlowItem->Data) != nullptr)
						{
							TestParameter^ tp = (TestParameter^)testItem->TestParameters[tl->glob->TestProperty[siteIndex].TestParameterName[tpCount]];

							if (!tp->Bypass)
							{
								// Identify the phase type in the test parameter item
								String ^ currentPhase = String::Empty;
								ret = amb7300tl->IdentifyTestParameterPhaseType(site, tfSite, siteIndex, currentPhase);
								if (ret != 0) goto EndOfTest;

								if (currentPhase == PHASE_CONST_DC_TEST)
								{
									// Cast condition from 'DcTest' @ AMB7300Utility
									ret = TestLib_TestParameter_DcTest_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'DcTest' phase @ AMB7300
									result[siteIndex] = (double)CONST_INVALID_RESULT;
									ret = amb7300tl->DcTest(tfSite, siteIndex, result[siteIndex]);
									if (ret != 0) goto EndOfTest;

									// Result logger
									String ^ tpUnit = tf_TestParameter_Unit(tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]]);
									amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest Result] " + "\n" +
										"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
										"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
										"\t Test Result: " + result[siteIndex].ToString() + tpUnit + "\n");
									amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest Result] " + "\n" +
										"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
										"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
										"\t Test Result: " + result.ToString() + tpUnit + "\n");

									// Save to result object
									TPtestResult[siteIndex, resultIndex[siteIndex]] = result[siteIndex];// +GetOffset(siteIndex, tIName, TPName);
									result[siteIndex];
									resultIndex[siteIndex]++;
								}
								else if (currentPhase == PHASE_CONST_PATTERN_TEST)
								{
									// Cast condition from 'PatternTest' @ AMB7300Utility
									ret = TestLib_TestParameter_PatternTest_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'PatternTest' phase @ AMB7300
									int res = (int)CONST_INVALID_RESULT;
									String ^ resultMessage = String::Empty;
									ret = amb7300tl->PatternTest(tfSite, siteIndex, res, resultMessage);
									if (ret != 0) goto EndOfTest;

									// Get final posting result based on testing mode 
									if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == true) && (amb7300tl->PatternControlCSC.isOneVecToMultiResult == false))
									{
										DataType dataType = tf_TestParameter_DataType(tl->glob->currentSubItemName[siteIndex]);
										EvalMode evalMode = tf_TestParameter_EvalMode(tl->glob->currentSubItemName[siteIndex]);
										int lowLimitInt32 = 999;
										int highLimitInt32 = 999;
										if (dataType == DataType::Int32)
										{
											lowLimitInt32 = (int)tf_TestParameter_MinLimit(tl->glob->currentSubItemName[siteIndex]);
											highLimitInt32 = (int)tf_TestParameter_MaxLimit(tl->glob->currentSubItemName[siteIndex]);
										}

										double lowLimitDouble = 999;
										double highLimitDouble = 999;
										if (dataType == DataType::Double)
										{
											lowLimitDouble = (double)tf_TestParameter_MinLimit(tl->glob->currentSubItemName[siteIndex]);
											highLimitDouble = (double)tf_TestParameter_MaxLimit(tl->glob->currentSubItemName[siteIndex]);
										}

										if (dataType == DataType::Int32)
										{
											int finalResult = (int)CONST_INVALID_RESULT;
											if (evalMode == EvalMode::Equal)
											{
												if (res != 1)
													finalResult = (int)CONST_INVALID_RESULT;
												else
													finalResult = highLimitInt32;
											}
											else if (evalMode == EvalMode::Between_IncludeMinAndMax)
											{
												if (res != 1)
													finalResult = (int)CONST_INVALID_RESULT;
												else
													finalResult = highLimitInt32;
											}

											// Result logger
											amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");
											amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");

											// Save to result object
											TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
											resultIndex[siteIndex]++;
										}
										else if (dataType == DataType::Double)
										{
											double finalResult = (double)CONST_INVALID_RESULT;
											if (evalMode == EvalMode::Equal)
											{
												if (res != 1)
													finalResult = (double)CONST_INVALID_RESULT;
												else
													finalResult = highLimitDouble;
											}
											else if (evalMode == EvalMode::Between_IncludeMinAndMax)
											{
												if (res != 1)
													finalResult = (double)CONST_INVALID_RESULT;
												else
													finalResult = highLimitDouble;
											}

											// Result logger
											amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");
											amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");

											// Save to result object
											TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
											resultIndex[siteIndex]++;
										}
										else if (dataType == DataType::Boolean)
										{
											bool finalResult = false;
											if (evalMode == EvalMode::Equal)
											{
												if (res != 1)
													finalResult = false;
												else
													finalResult = true;
											}

											// Result logger
											amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");
											amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
												"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
												"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
												resultMessage + "\n");

											// Save to result object
											TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
											resultIndex[siteIndex]++;
										}
									}
									else if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == false) && (amb7300tl->PatternControlCSC.isOneVecToMultiResult == true))
									{
										// Result logger
										amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
											"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
											"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
											resultMessage + "\n");
										amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
											"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
											"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
											resultMessage + "\n");

										// Save to result object
										TPtestResult[siteIndex, resultIndex[siteIndex]] = res;
										resultIndex[siteIndex]++;
									}
								}
								else if (currentPhase == PHASE_CONST_VNA_DATA_ANALYSIS)
								{
									// Cast condition from 'VnaDataAnalysis' @ AMB7300Utility
									lineNUM = (gcnew System::Diagnostics::StackFrame(0, true))->GetFileLineNumber();
									ret = TestLib_TestParameter_VnaDataAnalysis_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									// Execute 'VnaDataAnalysis' phase @ AMB7300
									result[siteIndex] = (double)CONST_INVALID_RESULT;
									amb7300tl->VnaDataAnalysis(tfSite, siteIndex, result[siteIndex]);
									//Util->/(tfSite, timerFilename, lineNUM.ToString() + " VNA_DataAnalysis", 1); //ticktecktock

									// Result logger
									String ^ tpUnit = tf_TestParameter_Unit(tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]]);
									amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis Result] " + "\n" +
										"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
										"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
										"\t Test Result: " + result[siteIndex].ToString() + tpUnit + "\n");
									amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[VnaDataAnalysis Result] " + "\n" +
										"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
										"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
										"\t Test Result: " + result[siteIndex].ToString() + tpUnit + "\n");

									// Save to result object
									TPtestResult[siteIndex, resultIndex[siteIndex]] = result[siteIndex];// +GetOffset(siteIndex, tIName, TPName);
									resultIndex[siteIndex]++;
								}
								else if (currentPhase == PHASE_CONST_MATH)
								{
									lineNUM = (gcnew System::Diagnostics::StackFrame(0, true))->GetFileLineNumber();
									ret = TestLib_TestParameter_Math_CastCondition(site, tfSite, siteIndex);
									if (ret != 0) goto EndOfTest;

									result[siteIndex] = (double)CONST_INVALID_RESULT;
									amb7300tl->MathFunction(tfSite, siteIndex, result[siteIndex]);

									// Save to result object
									TPtestResult[siteIndex, resultIndex[siteIndex]] = result[siteIndex];
									resultIndex[siteIndex]++;

								}
							}
							else
							{
								resultIndex[siteIndex]++;
							}
							tpCount++;
						}
					}

					// Cast test result to techFlow3
					ret = tl->UpdateTestResultWithOffsetToTechFlow(site, siteIndex, TPtestResult);
					if (ret != 0) goto EndOfTest;
				}
			}
		}
#pragma endregion

	EndOfTest:
		return ret;
	}
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Threading Helper Function
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestProgram::DoThread(ParameterizedThreadStart^ function, Site^ site)
	{
		int totalSite = tl->glob->tf.NumberOfTestSites;

		List<TestProgramData^>^ tpdatas = gcnew List<TestProgramData^>();
		array<bool>^ run_test = gcnew array<bool>(tl->glob->tf.NumberOfTestSites);

		try
		{
			IsRunTest(site, run_test);

			for (int i = 0; i < totalSite; i++)
			{
				if (run_test[i])
				{
					Util->CreateNewThread(site, function, tpdatas, i);
				}
			}

			for each(TestProgramData^ tpdata in tpdatas)
			{
				tpdata->Thread->Join();

				if (tpdata->Exception != nullptr)
				{
					String^ funcName = function->Method->ToString();

					array<String^>^ ArrStr = gcnew array<String^>(0);
					array<String^>^ Separator = gcnew array<String^>(3);
					Separator[0] = " ";
					Separator[1] = "(System.Object)";
					Separator[2] = "Void";
					ArrStr = funcName->Split(Separator, StringSplitOptions::RemoveEmptyEntries);

					G_RunTimeError[tpdata->siteIndex] = true;
					G_RunTimeErrorMessage[tpdata->siteIndex] += "Function=" + ArrStr[0] + ";" + tpdata->Exception->Message + "\n";
					funcName = String::Empty;
					funcName = nullptr;
				}
			}
		}
		finally
		{
			Util->ClearThread(tpdatas);

			StringBuilder^ sb = gcnew StringBuilder();
			for each(String^ errMsg in G_RunTimeErrorMessage)
			{
				if (!String::IsNullOrEmpty(errMsg))
					sb->Append(errMsg);
			}
			if (sb->Length > 0)
				throw gcnew Exception(sb->ToString());

		}
	}
	void TestProgram::IsRunTest(Site^ site, array<bool>^ run_test)
	{
		/*****************************************************************************************************
		** IsRunTest
		** Arguments:
		**		run_test - Returns an array that tells whether or not to perform test on any of the sites
		**				   (UUTOffset).
		**				   An array of sufficient size must be allocated by the caller function.
		** Descriptions:
		**		This method is to check whether to execute tests on the sites (UUTOffset) based on the
		**		following rules:
		**			If the UUTOffset is active (ED setting):
		**			If jump_on_fail is turned on, check whether any of previous tests has failed:
		**			- False: Continue run next tests (run_test = true)
		**			- True: Do not run remaining tests (run_test = false)
		******************************************************************************************************/

		int totalSite = tl->glob->tf.NumberOfTestSites;

		for (int siteIndex = 0; siteIndex < totalSite; siteIndex++)
		{
			AUUTOffset^ UUTOffset = site->UUTOffsetResolver->UUTOffsets[siteIndex];

			run_test[siteIndex] = false;

			if (UUTOffset->Active)
			{
				if (G_RunTimeError[siteIndex] == true)
				{
					run_test[siteIndex] = false;
				}
				else
				{
					if (G_JumpOnFail == true)
					{
						if (site->ResultsByOffset[UUTOffset]->CurrentResult->IfAnyTestParameterFailed == false)
						{
							run_test[siteIndex] = true;
						}
						else
						{
							run_test[siteIndex] = false;
						}
					}
					else
					{
						run_test[siteIndex] = true;
					}
				}
			}
		}
	}
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Execute each threaded control step item and test parameter item [DC]
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestProgram::ExecuteControlStep_DC(Object^ object)
	{
		int ret = 0;
		TestProgramData^ tpData = (TestProgramData^)object;
		Site^ site = tpData->t_site;
		int siteIndex = tpData->siteIndex;
		AFlowSubItem ^ subFlowItem;

		// Test condition object
		ConditionCollection ^ conditionCollection = gcnew ConditionCollection;
		Condition ^ condition;

		//Control Step & Test Parameter Count
		int csCount = 0;
		int tpCount = 0;

		// Get current test item object
		TestItem ^ testItem = (TestItem^)site->FlowItems[tl->glob->TestProperty[siteIndex].TestItemName];

		// Get current test site
		int tfSite = tl->glob->tf.TestSite;

#pragma region "Get currentFlowName"

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				// Get current test item object
				testItem = (TestItem^)site->FlowItems[tl->glob->TestProperty[siteIndex].TestItemName];

				// Get current test item's name
				tl->glob->currentFlowName = testItem->Name;
				//FirstSaveSnpForMultiFetch = true;
			}
		}

#pragma endregion

#pragma region "Initialize test parameter results storage"

		array<Object ^ > ^ testResult;
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				testResult = gcnew array<Object ^>(tl->glob->TestProperty[siteIndex].totalTestParameter);
			}
		}

#pragma endregion

		// Local result variable
		array<double> ^ result = gcnew array<double>(tl->glob->tf.NumberOfTestSites);
		array<int> ^ resultIndex = gcnew array<int>(tl->glob->tf.NumberOfTestSites);

		result[siteIndex] = 0.0;
		resultIndex[siteIndex] = 0;

		//tl->glob->currentSubItemName = gcnew array<String^>(tl->glob->tf.NumberOfTestSites);

		//// Get current flow item's name
		//	for (int l_siteIndex = 0; l_siteIndex < tl->glob->tf.NumberOfTestSites; l_siteIndex++)
		//	{
		//		tl->glob->currentSubItemName[l_siteIndex] = subFlowItem->Name;
		//	}

		int TotalvnaSiteIndex = 1; //Actually should grab amb7300SystemSetting->vnaSystemCount

		AFlowStep^ flowStep = (AFlowStep^)testItem->FlowSteps[tl->glob->TestProperty[siteIndex].FlowStepItemName[csCount]];

		if (!flowStep->Bypass)
		{
			if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_DC_CONTROL))
			{
				// Cast condition from 'DcControl' @ AMB7300Utility
				ret = TestLib_ControlStep_DcControl_CastCondition(site, tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;

				// Execute 'DcControl' phase @ AMB7300
				ret = amb7300tl->DcControl(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
			else if (tl->glob->currentSubItemName[siteIndex]->Contains(PHASE_CONST_PATTERN_CONTROL))
			{
				// Cast condition from 'PatternControl' @ AMB7300Utility
				ret = TestLib_ControlStep_PatternControl_CastCondition(site, tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;

				// Execute 'PatternControl' phase @ AMB7300
				ret = amb7300tl->PatternControl(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}
		else
		{
		}
		
	EndOfTest:;

	}
	void TestProgram::ExecuteTestParameter_DC(Object^ object)
	{
		int ret = 0;
		TestProgramData^ tpData = (TestProgramData^)object;
		Site^ site = tpData->t_site;
		int siteIndex = tpData->siteIndex;
		AFlowSubItem ^ subFlowItem;

		// Test condition object
		ConditionCollection ^ conditionCollection = gcnew ConditionCollection;
		Condition ^ condition;

		//Control Step & Test Parameter Count
		int csCount = 0;
		int tpCount = 0;

		// Get current test item object
		TestItem ^ testItem = (TestItem^)site->FlowItems[tl->glob->TestProperty[siteIndex].TestItemName];

		// Get current test site
		int tfSite = tl->glob->tf.TestSite;

#pragma region "Get currentFlowName"

		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				// Get current test item object
				testItem = (TestItem^)site->FlowItems[tl->glob->TestProperty[siteIndex].TestItemName];

				// Get current test item's name
				tl->glob->currentFlowName = testItem->Name;
				//FirstSaveSnpForMultiFetch = true;
			}
		}

#pragma endregion

#pragma region "Initialize test parameter results storage"

		array<Object ^ > ^ testResult;
		for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
		{
			if (site->UUTOffsetResolver->UUTOffsets[siteIndex]->Active)
			{
				testResult = gcnew array<Object ^>(tl->glob->TestProperty[siteIndex].totalTestParameter);
			}
		}

#pragma endregion

		// Local result variable
		array<double> ^ result = gcnew array<double>(tl->glob->tf.NumberOfTestSites);

		result[siteIndex] = 0.0;

		//tl->glob->currentSubItemName = gcnew array<String^>(tl->glob->tf.NumberOfTestSites);
		//// Get current flow item's name
		////tl->glob->currentSubItemName[siteIndex] = subFlowItem->Name;
		//tl->glob->currentSubItemName[0] = tf_TestParameter_Name(0);
		//tl->glob->currentSubItemName[1] = tf_TestParameter_Name(0);

		TestParameter^ tp = (TestParameter^)testItem->TestParameters[tl->glob->TestProperty[siteIndex].TestParameterName[tpCount]];

		if (!tp->Bypass)
		{
			// Identify the phase type in the test parameter item
			String ^ currentPhase = String::Empty;
			ret = amb7300tl->IdentifyTestParameterPhaseType(site, tfSite, siteIndex, currentPhase);
			if (ret != 0) goto EndOfTest;

			if (currentPhase == PHASE_CONST_DC_TEST)
			{
				// Cast condition from 'DcTest' @ AMB7300Utility
				ret = TestLib_TestParameter_DcTest_CastCondition(site, tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;

				// Execute 'DcTest' phase @ AMB7300
				result[siteIndex] = (double)CONST_INVALID_RESULT;
				ret = amb7300tl->DcTest(tfSite, siteIndex, result[siteIndex]);
				if (ret != 0) goto EndOfTest;

				// Result logger
				String ^ tpUnit = tf_TestParameter_Unit(tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]]);
				amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[DcTest Result] " + "\n" +
					"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
					"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
					"\t Test Result: " + result[siteIndex].ToString() + tpUnit + "\n");
				amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[DcTest Result] " + "\n" +
					"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
					"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[resultIndex[siteIndex]] + "\n" +
					"\t Test Result: " + result[siteIndex].ToString() + tpUnit + "\n");

				// Save to result object
				tl->glob->TestProperty[siteIndex].IsHardwareInvolved[resultIndex[siteIndex]] = true; //Set true for hardware result duplicate checking
				TPtestResult[siteIndex, resultIndex[siteIndex]] = result[siteIndex];
				resultIndex[siteIndex]++;
			}
			else if (currentPhase == PHASE_CONST_PATTERN_TEST)
			{
				// Cast condition from 'PatternTest' @ AMB7300Utility
				ret = TestLib_TestParameter_PatternTest_CastCondition(site, tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;

				// Execute 'PatternTest' phase @ AMB7300
				int res = (int)CONST_INVALID_RESULT;
				String ^ resultMessage = String::Empty;
				ret = amb7300tl->PatternTest(tfSite, siteIndex, res, resultMessage);
				if (ret != 0) goto EndOfTest;

				// Get final posting result based on testing mode 
				if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == true) && (amb7300tl->PatternControlCSC.isOneVecToMultiResult == false))
				{
					DataType dataType = tf_TestParameter_DataType(tl->glob->currentSubItemName[siteIndex]);
					EvalMode evalMode = tf_TestParameter_EvalMode(tl->glob->currentSubItemName[siteIndex]);
					int lowLimitInt32 = 999;
					int highLimitInt32 = 999;
					if (dataType == DataType::Int32)
					{
						lowLimitInt32 = (int)tf_TestParameter_MinLimit(tl->glob->currentSubItemName[siteIndex]);
						highLimitInt32 = (int)tf_TestParameter_MaxLimit(tl->glob->currentSubItemName[siteIndex]);
					}

					double lowLimitDouble = 999;
					double highLimitDouble = 999;
					if (dataType == DataType::Double)
					{
						lowLimitDouble = (double)tf_TestParameter_MinLimit(tl->glob->currentSubItemName[siteIndex]);
						highLimitDouble = (double)tf_TestParameter_MaxLimit(tl->glob->currentSubItemName[siteIndex]);
					}

					if (dataType == DataType::Int32)
					{
						int finalResult = (int)CONST_INVALID_RESULT;
						if (evalMode == EvalMode::Equal)
						{
							if (res != 1)
								finalResult = (int)CONST_INVALID_RESULT;
							else
								finalResult = highLimitInt32;
						}
						else if (evalMode == EvalMode::Between_IncludeMinAndMax)
						{
							if (res != 1)
								finalResult = (int)CONST_INVALID_RESULT;
							else
								finalResult = highLimitInt32;
						}

						// Result logger
						amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");
						amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");

						// Save to result object
						TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
						resultIndex[siteIndex]++;
					}
					else if (dataType == DataType::Double)
					{
						double finalResult = (double)CONST_INVALID_RESULT;
						if (evalMode == EvalMode::Equal)
						{
							if (res != 1)
								finalResult = (double)CONST_INVALID_RESULT;
							else
								finalResult = highLimitDouble;
						}
						else if (evalMode == EvalMode::Between_IncludeMinAndMax)
						{
							if (res != 1)
								finalResult = (double)CONST_INVALID_RESULT;
							else
								finalResult = highLimitDouble;
						}

						// Result logger
						amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");
						amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");

						// Save to result object
						TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
						resultIndex[siteIndex]++;
					}
					else if (dataType == DataType::Boolean)
					{
						bool finalResult = false;
						if (evalMode == EvalMode::Equal)
						{
							if (res != 1)
								finalResult = false;
							else
								finalResult = true;
						}

						// Result logger
						amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");
						amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
							"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
							"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
							resultMessage + "\n");

						// Save to result object
						TPtestResult[siteIndex, resultIndex[siteIndex]] = finalResult;
						resultIndex[siteIndex]++;
					}
				}
				else if ((amb7300tl->PatternControlCSC.isMultiVecToOneResult == false) && (amb7300tl->PatternControlCSC.isOneVecToMultiResult == true))
				{
					// Result logger
					amb7300tl->tl->WriteToTracerLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
						"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
						"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
						resultMessage + "\n");
					amb7300tl->tl->WriteToFileLogger(tfSite, siteIndex, INFO, "[PatternTest Result] " + "\n" +
						"\t Test Item: " + tl->glob->TestProperty[siteIndex].TestItemName + "\n" +
						"\t Test Parameter: " + tl->glob->TestProperty[siteIndex].TestParameterName[amb7300tl->PatternTestTPC.returnIndex] + "\n" +
						resultMessage + "\n");

					// Save to result object
					TPtestResult[siteIndex, resultIndex[siteIndex]] = res;
					resultIndex[siteIndex]++;
				}
			}
			else if (currentPhase == PHASE_CONST_MATH)
			{
				ret = TestLib_TestParameter_Math_CastCondition(site, tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;

				result[siteIndex] = (double)CONST_INVALID_RESULT;
				amb7300tl->MathFunction(tfSite, siteIndex, result[siteIndex]);

				// Save to result object
				TPtestResult[siteIndex, resultIndex[siteIndex]] = result[siteIndex];
				resultIndex[siteIndex]++;
			}
		}
		else
		{
			resultIndex[siteIndex]++;
		}
		tpCount++;
	EndOfTest:;

	}
}



/*----------------------------------------------------------------------
* Revision Log
* &Log: TestMethod.cpp.rca&

*** Version	: v1.0.0.5
*** Date	: 2 September 2026
*** PIC		: Tham Zhi Kean
* Updated bug on sequential test VnaFetch SaveSnpData part to enabled by AWV

*** Version	: v1.0.0.4
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Add arr_activeUUT

*** Version	: v1.0.0.3
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Merge UpdateTestResultWithOffsetToTechFlow()

*** Version	: v1.0.0.2
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Support project UUT:
 -True Parallel Multi UUT (KeysightVNA)
* DC will be threaded whereas RF Contol/ Test are sequential
* 2D array TPTestResults is created for use of threaded functions

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
*UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Auto detect test flow, perform the control phase and test phase.
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
