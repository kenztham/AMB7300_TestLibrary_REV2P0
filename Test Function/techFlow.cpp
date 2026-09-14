/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			techFlow.cpp
Purpose:		Contain techFlow3 opearion functions
Version:		v1.0.0.3
----------------------------------------------------------------------*/


#include "TestFunction.h"
#include "TF-Macro.h"
#include "../AMB7300/AMB7300.h"
#include "..\\TestProgram.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Update Test Property
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::UpdateTestProperty(Site ^ site, int siteIndex)
	{
		/*****************************************************************************************************
		** UpdateTestProperty
		**		site		- This is techFlow site object.
		**		siteIndex	- This is VNA object index, normally start from 0.
		**
		** Descriptions:
		**		This is a function to update test properties for all test parameters for each test item.
		**		The test properties included:
		**		(1) TestParaNameWithSiteIndex is a string dictionary to add "TestParameterName with siteIndex"
		**			as Key and "TestParameterName" as value.
		**			- TestParNameWithSiteIndexIdentifier format: TestParameterName + siteIndex eg: TI1_TP1_POUT_S0
		**
		**		Purpose: The "TestParameterName" property will be use when call tf_SetResult or
		**				 tf_SetResult_UUTOffset in UpdateTestResults function.
		******************************************************************************************************/

		// Local variable
		int ret														= 0;
		int iSubItemIndex											= 0;
		int iFlowStepIndex											= 0;
		int iTPIndex												= 0;
		String ^ TestParNameWithSiteIndexIdentifier					= String::Empty;

		AFlowItem ^ item											= site->CurrentFlowItem;
		Type ^ FlowType												= item->GetType();

		// Test Item
		glob->TestProperty[siteIndex].TestItemName					= (String^)tf_TestItem_Name();
		glob->TestProperty[siteIndex].TestItemDisplayName			= (String^)tf_TestItem_DisplayName();

		// Sub Item (Test Step, Control Step, Test Parameter) 
		glob->TestProperty[siteIndex].totalSubItem					= (int)tf_Flow_SubItemCount();
		glob->TestProperty[siteIndex].SubFlowItemTypeId				= gcnew array<Type ^>(glob->TestProperty[siteIndex].totalSubItem);

		// Test Parameter
		glob->TestProperty[siteIndex].totalTestParameter			= (int)tf_TestParameter_Count();
		glob->TestProperty[siteIndex].TestParameterName				= gcnew array<String ^>(glob->TestProperty[siteIndex].totalTestParameter);
		glob->TestProperty[siteIndex].TestParameterDisplayName		= gcnew array<String ^>(glob->TestProperty[siteIndex].totalTestParameter);
		glob->TestProperty[siteIndex].TestParameterTypeId			= gcnew array<Type ^>(glob->TestProperty[siteIndex].totalTestParameter);
		glob->TestProperty[siteIndex].TestParameterExecuted			= gcnew Dictionary<String ^, bool>();
		glob->TestProperty[siteIndex].TestParameterTestStatus		= gcnew array<int>(glob->TestProperty[siteIndex].totalTestParameter);
		glob->TestProperty[siteIndex].TestParameterUpdateResStatus	= gcnew Dictionary<String ^, bool>();
		glob->TestProperty[siteIndex].IsCurrentTPBypassed			= gcnew array<bool>(glob->TestProperty[siteIndex].totalTestParameter);
		glob->TestProperty[siteIndex].IsHardwareInvolved			= gcnew array<bool>(glob->TestProperty[siteIndex].totalTestParameter);

		// Flow Step (Control Step, Test Step)
		glob->TestProperty[siteIndex].totalFlowStep					= (int)tf_FlowStep_Count();
		glob->TestProperty[siteIndex].FlowStepItemName				= gcnew array<String ^>(glob->TestProperty[siteIndex].totalFlowStep);
		glob->TestProperty[siteIndex].FlowStepItemDisplayName		= gcnew array<String ^>(glob->TestProperty[siteIndex].totalFlowStep);
		glob->TestProperty[siteIndex].FlowStepItemExecuted			= gcnew array<bool>(glob->TestProperty[siteIndex].totalFlowStep);
	
		WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Test Property] Update test property.");
		WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Test Property] Update test property.");

		try
		{
			if (FlowType == TestItem::typeid)
			{
				TestItem ^ testItem = (TestItem^)site->FlowItems[glob->TestProperty[siteIndex].TestItemName];
				
				for each(AFlowSubItem ^ subFlowItem in testItem->SubItems)
				{
					// Control Step Item & Test Step Item
					if ((dynamic_cast<ControlStep^>(subFlowItem->Data) != nullptr) || (dynamic_cast<TestStep^>(subFlowItem->Data) != nullptr)) 
					{
						glob->TestProperty[siteIndex].FlowStepItemName[iFlowStepIndex]			= subFlowItem->Name;
						glob->TestProperty[siteIndex].FlowStepItemDisplayName[iFlowStepIndex]	= subFlowItem->Name;
						glob->TestProperty[siteIndex].FlowStepItemExecuted[iFlowStepIndex]		= false;
						
						if (dynamic_cast<TestStep^>(subFlowItem->Data) != nullptr)
						{
							TestStep^ testStep = (TestStep^)testItem->FlowSteps[subFlowItem->Name];
							for (int iTsTp = 0; iTsTp < glob->TestProperty[siteIndex].totalTestParameter; iTsTp++)
							{
								if (glob->TestProperty[siteIndex].TestParameterName[iTsTp] == testStep->TestParameter->Name)
								{
									// Replace test parameter under test step with TestStep::typeid 
									glob->TestProperty[siteIndex].TestParameterTypeId[iTsTp] = subFlowItem->Data->GetType();
								}
							}
						}
						iFlowStepIndex++;
					}
					// Test Parameter Item
					else if (dynamic_cast<TestParameter^>(subFlowItem->Data) != nullptr)
					{
						TestParameter^ tp = (TestParameter^)testItem->TestParameters[subFlowItem->Name];

						glob->TestProperty[siteIndex].TestParameterExecuted->Add(subFlowItem->Name, false);
						glob->TestProperty[siteIndex].TestParameterUpdateResStatus->Add(subFlowItem->Name, false);

						glob->TestProperty[siteIndex].TestParameterName[iTPIndex]			= tp->Name;
						glob->TestProperty[siteIndex].TestParameterDisplayName[iTPIndex]	= tp->DisplayName;
						glob->TestProperty[siteIndex].TestParameterTypeId[iTPIndex]			= subFlowItem->Data->GetType();
						TestParNameWithSiteIndexIdentifier									= glob->TestProperty[siteIndex].TestParameterDisplayName[iTPIndex] + "_S" + siteIndex.ToString();

						// Eliminate "An item with the same key has already been added" error 
						if (!glob->TestProperty[siteIndex].TestParaNameWithSiteIndex->ContainsKey(TestParNameWithSiteIndexIdentifier))
						{
							glob->TestProperty[siteIndex].TestParaNameWithSiteIndex->Add(TestParNameWithSiteIndexIdentifier, glob->TestProperty[siteIndex].TestParameterName[iTPIndex]);
						}

						// Eliminate "An item with the same key has already been added" error 
						if (!glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex->ContainsKey(TestParNameWithSiteIndexIdentifier))
						{
							glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex->Add(TestParNameWithSiteIndexIdentifier, glob->TestProperty[siteIndex].TestParameterDisplayName[iTPIndex]);
						}

						glob->TestProperty[siteIndex].IsCurrentTPBypassed[iTPIndex] = tp->Bypass;
						iTPIndex++;
					}

					// ControlStep::typeid or TestStep::typeid or TestParameter::typeid
					glob->TestProperty[siteIndex].SubFlowItemTypeId[iSubItemIndex] = subFlowItem->GetType();
					iSubItemIndex++;
				}
			}

			//(glob->AWV.Debug == 1) ? glob->TestProperty[siteIndex].DebugEnable = true : false;
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UPDATE_TEST_PROPERTY_FAIL;
			WriteToTracerLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateTestProperty] Fail to update test property." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			WriteToFileLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateTestProperty] Fail to update test property." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			throw gcnew Aemulus::Hardware::AlarmException(ex->ToString(), ex->HResult);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Update Test Result To Dicitionary
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::UpdateControlStepTestResulToDictionary(Site^ site, int siteIndex, String^ Identifier, double CSTestResult)
	{
		int ret = 0;
		int tfSite = glob->tf.TestSite;
		//String ^ Identifier = String::Empty;
		bool isCurrentTPByPassed = false;
		String ^ ResultUnit = String::Empty;
		String ^ ResultUnitPrefix = String::Empty;
		bool IsInfinityStatus = false;
		double OffsetFactor = 0.0;
		String^ key;

		WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update CS Result To Dict] Updating control step result to Dictionary.");
		WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update CS Result To Dict] Updating test result to Dictionary.");

		try {

			//Identifier = glob->TestProperty[siteIndex].TestParameterDisplayName[i] + "_S" + siteIndex.ToString();
			key = Identifier;
			//OffsetFactor			= GetFixedOffsetValue(tfSite, siteIndex, key);
			glob->ResultWithDataType[siteIndex].DoubleTypeResult = Convert::ToDouble(CSTestResult);// + OffsetFactor;

			if (glob->TestProperty[siteIndex].TestResults[Identifier] ==
				glob->TestProperty[siteIndex].PreviousTestResults[Identifier])
			{
				glob->ResultWithDataType[siteIndex].DoubleTypeResult = ER_CONST_ERROR_HARDWARE_DUPLICATED_RESULT;
			}

			if (glob->TestProperty[siteIndex].TestResults->ContainsKey(Identifier))
			{
				glob->TestProperty[siteIndex].TestResults[Identifier] = glob->ResultWithDataType[siteIndex].DoubleTypeResult;
				glob->TestProperty[siteIndex].PreviousTestResults[Identifier] = glob->ResultWithDataType[siteIndex].DoubleTypeResult;
			}
			else
			{
				glob->TestProperty[siteIndex].TestResults->Add(Identifier, glob->ResultWithDataType[siteIndex].DoubleTypeResult);
				glob->TestProperty[siteIndex].PreviousTestResults->Add(Identifier, glob->ResultWithDataType[siteIndex].DoubleTypeResult);
			}
		}
		catch (Exception ^ ex)
		{
			ret = ER_CONST_UPDATE_CONTROL_STEP_RESULT_TO_DICT_FAIL;
			WriteToTracerLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateControlStepTestResulToDictionary] Fail to update control step result to Dictionary." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			WriteToFileLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateControlStepTestResulToDictionary] Fail to update control step result to Dictionary." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

		EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Update Test Result To techFlow
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::UpdateTestResultWithOffsetToTechFlow(Site^ site, int siteIndex, array<Object ^, 2> ^ TPtestResult)
	{
		/*****************************************************************************************************
		** UpdateTestResultWithOffsetToTechFlow
		**		site		- This is techFlow site object.
		**		siteIndex	- This is selected physical site. (Note: UUT offset index for the MultiUUTOffsets
		**					  project or techFlow sites for Index Parallel project.
		**		TPtestResult	- This is a double type array of test result, store test result of each test parameter.
		**
		** Descriptions:
		**		This is a function to update the test results together with the offset value (get from FixedOffset.csv)
		**		to techFlow.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = glob->tf.TestSite;
		String ^ Identifier = String::Empty;
		bool isCurrentTPByPassed = false;
		String ^ ResultUnit = String::Empty;
		String ^ ResultUnitPrefix = String::Empty;
		bool IsInfinityStatus = false;
		double OffsetFactor = 0.0;
		String^ key;

		WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] Updating test result to techFlow.");
		WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] Updating test result to techFlow.");

		try
		{
			for (int i = 0; i < glob->TestProperty[siteIndex].totalTestParameter; i++)
			{
				Identifier = glob->TestProperty[siteIndex].TestParameterDisplayName[i] + "_S" + siteIndex.ToString();
				key = glob->TestProperty[siteIndex].TestItemDisplayName + "_" + Identifier;

				if ((glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex->ContainsKey(Identifier) == true)	&& 
					(glob->TestProperty[siteIndex].IsCurrentTPBypassed[i] == false)										&& 
					(glob->TestProperty[siteIndex].TestParameterTypeId[i] == TestParameter::typeid))
				{
					ResultUnit				= ((TestItem^)site->FlowItems[glob->TestProperty[siteIndex].TestItemName])->TestParameters[glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex[Identifier]]->Unit;
					ResultUnitPrefix		= GetStringUnitPrefix(site, glob->TestProperty[siteIndex].TestItemName, glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex[Identifier]);
					DataType ResultDataType = ((TestItem^)site->FlowItems[glob->TestProperty[siteIndex].TestItemName])->TestParameters[glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex[Identifier]]->DataType;
					OffsetFactor			= GetFixedOffsetValue(tfSite, siteIndex, key);

					switch (ResultDataType)
					{
					case DataType::Double:

						glob->ResultWithDataType[siteIndex].DoubleTypeResult = Convert::ToDouble(TPtestResult[siteIndex,i]) + OffsetFactor;
						Util->IsInfinity(glob->ResultWithDataType[siteIndex].DoubleTypeResult, IsInfinityStatus);

						if (IsInfinityStatus == true)
						{
							glob->ResultWithDataType[siteIndex].DoubleTypeResult = ER_CONST_ERROR_CATCH;
						}
						if (glob->TestProperty[siteIndex].IsHardwareInvolved[i] == true) //Hardware result duplicate checking
						{
							if (glob->TestProperty[siteIndex].TestResults[glob->TestProperty[siteIndex].TestParameterDisplayName[i]] ==
								glob->TestProperty[siteIndex].PreviousTestResults[glob->TestProperty[siteIndex].TestParameterDisplayName[i]])
							{
								glob->ResultWithDataType[siteIndex].DoubleTypeResult = ER_CONST_ERROR_HARDWARE_DUPLICATED_RESULT;
							}
						}
						//Store Test Results into global Dictionary
						if (glob->TestProperty[siteIndex].TestResults->ContainsKey(glob->TestProperty[siteIndex].TestParameterDisplayName[i]))
						{
							glob->TestProperty[siteIndex].TestResults[glob->TestProperty[siteIndex].TestParameterDisplayName[i]] = glob->ResultWithDataType[siteIndex].DoubleTypeResult;
							glob->TestProperty[siteIndex].PreviousTestResults[glob->TestProperty[siteIndex].TestParameterDisplayName[i]] = glob->ResultWithDataType[siteIndex].DoubleTypeResult;
						}
						else
						{
							glob->TestProperty[siteIndex].TestResults->Add(glob->TestProperty[siteIndex].TestParameterDisplayName[i], glob->ResultWithDataType[siteIndex].DoubleTypeResult);
							glob->TestProperty[siteIndex].PreviousTestResults->Add(glob->TestProperty[siteIndex].TestParameterDisplayName[i], glob->ResultWithDataType[siteIndex].DoubleTypeResult);
						}

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].DoubleTypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].DoubleTypeResult, siteIndex);
						}
						break;

					case DataType::Int32:

						glob->ResultWithDataType[siteIndex].IntTypeResult = Convert::ToInt32(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Store Test Results into global Dictionary
						if (glob->TestProperty[siteIndex].TestResults->ContainsKey(glob->TestProperty[siteIndex].TestParameterDisplayName[i]))
						{
							glob->TestProperty[siteIndex].TestResults[glob->TestProperty[siteIndex].TestParameterDisplayName[i]] = glob->ResultWithDataType[siteIndex].IntTypeResult;
						}
						else
						{
							glob->TestProperty[siteIndex].TestResults->Add(glob->TestProperty[siteIndex].TestParameterDisplayName[i], glob->ResultWithDataType[siteIndex].IntTypeResult);
						}


						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].IntTypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].IntTypeResult, siteIndex);
						}
						break;

					case DataType::UInt32:

						glob->ResultWithDataType[siteIndex].UIntTypeResult = Convert::ToUInt32(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].UIntTypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].UIntTypeResult, siteIndex);
						}
						break;

					case DataType::Int64:

						glob->ResultWithDataType[siteIndex].Int64TypeResult = Convert::ToInt64(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int64TypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int64TypeResult, siteIndex);
						}
						break;

					case DataType::UInt64:

						glob->ResultWithDataType[siteIndex].UInt64TypeResult = Convert::ToUInt64(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].UInt64TypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].UInt64TypeResult, siteIndex);
						}
						break;

					case DataType::Int16:

						glob->ResultWithDataType[siteIndex].Int16TypeResult = Convert::ToInt16(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int16TypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int16TypeResult, siteIndex);
						}
						break;

					case DataType::UInt16:

						glob->ResultWithDataType[siteIndex].UInt16TypeResult = Convert::ToUInt16(TPtestResult[siteIndex,i]) + (int)OffsetFactor;

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int16TypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].Int16TypeResult, siteIndex);
						}
						break;

					case DataType::String:

						glob->ResultWithDataType[siteIndex].StringTyperesult = Convert::ToString(TPtestResult[siteIndex,i]);

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].StringTyperesult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].StringTyperesult, siteIndex);
						}
						break;

					case DataType::Boolean:

						glob->ResultWithDataType[siteIndex].BoolTypeResult = Convert::ToBoolean(TPtestResult[siteIndex,i]);

						//Index Parallel 
						if (glob->tf.StageCount > 1)
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].BoolTypeResult);
						}
						//Multi UUTOffsets
						else
						{
							glob->TestProperty[siteIndex].TestParameterTestStatus[i] = tf_SetResult_UUTOffset(glob->TestProperty[siteIndex].TestParameterName[i], glob->ResultWithDataType[siteIndex].BoolTypeResult, siteIndex);
						}
						break;
					}

					glob->TestProperty[siteIndex].TestParameterUpdateResStatus[glob->TestProperty[siteIndex].TestParameterName[i]] = true;

					if (glob->AWV.Debug == 1)
					{
						if (ResultDataType == DataType::Double)
						{
							WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + (Convert::ToDouble(TPtestResult[siteIndex,i]) / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Offset Factor: " + (OffsetFactor / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Final Test Result: " + (glob->ResultWithDataType[siteIndex].DoubleTypeResult / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n");

							WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + (Convert::ToDouble(TPtestResult[siteIndex,i]) / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Offset Factor: " + (OffsetFactor / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Final Test Result: " + (glob->ResultWithDataType[siteIndex].DoubleTypeResult / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n");
						}
						else if (ResultDataType == DataType::Int32)
						{
							WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + (Convert::ToDouble(TPtestResult[siteIndex,i]) / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Offset Factor: " + (OffsetFactor / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Final Test Result: " + (glob->ResultWithDataType[siteIndex].IntTypeResult / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n");

							WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + (Convert::ToDouble(TPtestResult[siteIndex,i]) / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Offset Factor: " + (OffsetFactor / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n" +
								"\t Final Test Result: " + (glob->ResultWithDataType[siteIndex].IntTypeResult / glob->TcrLgr.PrefixValue).ToString() + ResultUnitPrefix + ResultUnit + "\n");
						}
						else if (ResultDataType == DataType::String)
						{
							WriteToTracerLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + glob->ResultWithDataType[siteIndex].StringTyperesult + "\n" +
								"\t Offset Factor: " + "NA" + "\n" +
								"\t Final Test Result: " + glob->ResultWithDataType[siteIndex].StringTyperesult + "\n");

							WriteToFileLogger(glob->tf.TestSite, siteIndex, INFO, "[Update Result To TF] " + "\n" +
								"\t Test Parameter Identifier: " + Identifier + "\n" +
								"\t techFlow Site: Site " + glob->tf.TestSite.ToString() + "\n" +
								"\t UUTOffset: Site " + siteIndex.ToString() + "\n" +
								"\t Raw Test Result: " + glob->ResultWithDataType[siteIndex].StringTyperesult + "\n" +
								"\t Offset Factor: " + "NA" + "\n" +
								"\t Final Test Result: " + glob->ResultWithDataType[siteIndex].StringTyperesult + "\n");
						}
					}

				}
			}
		}
		catch (Exception^ ex)
		{
			ret = ER_CONST_UPDATE_RESULT_TO_TF3_FAIL;
			WriteToTracerLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateTestResultWithOffsetToTechFlow] Fail to update test result to techFlow." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			WriteToFileLogger(glob->tf.TestSite, siteIndex, ERROR, "[UpdateTestResultWithOffsetToTechFlow] Fail to update test result to techFlow." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
			goto EndOfTest;
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Get Unit Prefix (in String format) 
	**	----------------------------------------------------------------------------------------------------
	*/
	String^ TestFunction::GetStringUnitPrefix(Site^ site, String ^ TestItemName, String ^ TestParameterName)
	{
		/*****************************************************************************************************
		** GetStringUnitPrefix
		**		site				- This is techFlow site object.
		**		TestItemName		- This is the name of current test item.
		**		TestParameterName	- This is the name of current test item specified test parameter.
		**
		** Descriptions:
		**		This is a function to get the unit prefix for each test parameter which is needed for file and
		**		tracer logging.
		******************************************************************************************************/

		// Local variable
		String ^ Prefix				= String::Empty;
		UnitPrefix ResultUnitPrefix = ((TestItem^)site->FlowItems[TestItemName])->TestParameters[TestParameterName]->Prefix;

		if (ResultUnitPrefix == UnitPrefix::Milli)
		{
			Prefix = "m";
			glob->TcrLgr.PrefixValue = 1e-3;
		}
		else if (ResultUnitPrefix == UnitPrefix::Micro)
		{
			Prefix = "u";
			glob->TcrLgr.PrefixValue = 1e-6;
		}
		else if (ResultUnitPrefix == UnitPrefix::Nano)
		{
			Prefix = "n";
			glob->TcrLgr.PrefixValue = 1e-9;
		}
		else if (ResultUnitPrefix == UnitPrefix::Pico)
		{
			Prefix = "p";
			glob->TcrLgr.PrefixValue = 1e-12;
		}
		else if (ResultUnitPrefix == UnitPrefix::Femto)
		{
			Prefix = "f";
			glob->TcrLgr.PrefixValue = 1e-15;
		}
		else if (ResultUnitPrefix == UnitPrefix::Deci)
		{
			Prefix = "d";
			glob->TcrLgr.PrefixValue = 1e-1;
		}
		else if (ResultUnitPrefix == UnitPrefix::Atto)
		{
			Prefix = "a";
			glob->TcrLgr.PrefixValue = 1e-18;
		}
		else if (ResultUnitPrefix == UnitPrefix::Centi)
		{
			Prefix = "c";
			glob->TcrLgr.PrefixValue = 1e-2;
		}
		else if (ResultUnitPrefix == UnitPrefix::Deca)
		{
			Prefix = "da";
			glob->TcrLgr.PrefixValue = 1e1;
		}
		else if (ResultUnitPrefix == UnitPrefix::Exa)
		{
			Prefix = "E";
			glob->TcrLgr.PrefixValue = 1e18;
		}
		else if (ResultUnitPrefix == UnitPrefix::Giga)
		{
			Prefix = "G";
			glob->TcrLgr.PrefixValue = 1e9;
		}
		else if (ResultUnitPrefix == UnitPrefix::Hecto)
		{
			Prefix = "h";
			glob->TcrLgr.PrefixValue = 1e2;
		}
		else if (ResultUnitPrefix == UnitPrefix::Kilo)
		{
			Prefix = "k";
			glob->TcrLgr.PrefixValue = 1e3;
		}
		else if (ResultUnitPrefix == UnitPrefix::Mega)
		{
			Prefix = "M";
			glob->TcrLgr.PrefixValue = 1e6;
		}
		else if (ResultUnitPrefix == UnitPrefix::Peta)
		{
			Prefix = "P";
			glob->TcrLgr.PrefixValue = 1e15;
		}
		else if (ResultUnitPrefix == UnitPrefix::Tera)
		{
			Prefix = "T";
			glob->TcrLgr.PrefixValue = 1e12;
		}
		else if (ResultUnitPrefix == UnitPrefix::Yocto)
		{
			Prefix = "y";
			glob->TcrLgr.PrefixValue = 1e-24;
		}
		else if (ResultUnitPrefix == UnitPrefix::Yotta)
		{
			Prefix = "Y";
			glob->TcrLgr.PrefixValue = 1e24;
		}
		else if (ResultUnitPrefix == UnitPrefix::Zepto)
		{
			Prefix = "z";
			glob->TcrLgr.PrefixValue = 1e-21;
		}
		else if (ResultUnitPrefix == UnitPrefix::Zetta)
		{
			Prefix = "Z";
			glob->TcrLgr.PrefixValue = 1e21;
		}
		else //None 
		{
			Prefix = "";
			glob->TcrLgr.PrefixValue = 1.0;
		}

		

		return Prefix;
	}

	
}


/*----------------------------------------------------------------------
* Revision Log
* &Log: techFlow.cpp.rca&

*** Version	: v1.0.0.3
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Update UpdateTestResultWithOffsetToTechFlow()

*** Version	: v1.0.0.2
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Add UpdateTestResultWithOffsetToTechFlow_TrueParellel() which utilize for True Parallel Multi UUT project (TPtestResult as 2D array)

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support update test property.
* Support update test result to tF3.
----------------------------------------------------------------------*/