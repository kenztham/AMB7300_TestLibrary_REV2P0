/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			TF-Macro.h
Purpose:		Define techFlow macros.
Version:		v1.0.0.1
----------------------------------------------------------------------*/


#pragma region "App-Wide-Variables"

//Return boolean, true if app-wide-variable found
#define tf_AppWideVariable_Exist(VariableName) site->FlowEngine->AppWideVariables->ContainsKey(VariableName)

//Return app-wide-variable value
#define tf_AppWideVariable_Cast(VariableName) site->FlowEngine->AppWideVariables[VariableName]->Value

#pragma endregion

//--------------------------------------------------

#pragma region "techFlow Variable"

//Return String that containing site name
#define tf_SiteName() site->Name

#pragma endregion

//--------------------------------------------------

#pragma region "Flow Item (general)"

//Return flow item name
#define tf_Flow_Name() site->Flow->Name

//Return boolean, true if flow condition found
#define tf_Flow_ConditionExist(ConditionName) site->Conditions->ContainsKey(ConditionName)

//Return object containing flow"s condition value (Value casting is required)
#define tf_Flow_ConditionCast(ConditionName) site->Conditions[ConditionName][site]->Value

//Return total sub-itmens such as test step, control step and test parameter count in current test item
#define tf_Flow_SubItemCount() ((TestItem^)site->CurrentFlowItem)->SubItems->Count

#pragma endregion

//--------------------------------------------------

#pragma region "Control Item"

//Return control item name
#define tf_ControlItem_Name() ((ControlItem^)site->CurrentFlowItem)->Name

//Return control item display name
#define tf_ControlItem_DisplayName() ((ControlItem^)site->CurrentFlowItem)->DisplayName

//Return control item
#define tf_ControlItem() ((ControlItem ^)site->CurrentFlowItem)

//Return ConditionCollection
#define tf_ControlItem_ConditionList() ((ControlItem^)site->CurrentFlowItem)->Conditions

//Return boolean, true if condition is exist
#define tf_ControlItem_ConditionExist(ConditionName) ((ControlItem ^)site->CurrentFlowItem)->Conditions->ContainsKey(ConditionName)

//Return control item's condition value
#define tf_ControlItem_ConditionCast(ConditionName) ((ControlItem ^)site->CurrentFlowItem)->Conditions[ConditionName][site]->Value

//Return control item's bypass status
#define tf_ControlItem_BypassStatus() ((ControlItem^)site->CurrentFlowItem)->Bypass

#pragma endregion

//--------------------------------------------------

#pragma region "Test Item"

//Return test item name
#define tf_TestItem_Name() ((TestItem^)site->CurrentFlowItem)->Name

//Return test item display name
#define tf_TestItem_DisplayName() ((TestItem^)site->CurrentFlowItem)->DisplayName

//Return test item
#define tf_TestItem() ((TestItem^)site->CurrentFlowItem)

//Return ConditionCollection
#define tf_TestItem_ConditionList() ((TestItem^)site->CurrentFlowItem)->Conditions

//Return boolean, true if condition is exist
#define tf_TestItem_ConditionExist(ConditionName) ((TestItem^)site->CurrentFlowItem)->Conditions->ContainsKey(ConditionName)

//Return test item's condition value
#define tf_TestItem_ConditionCast(ConditionName) ((TestItem^)site->CurrentFlowItem)->Conditions[ConditionName][site]->Value

//Return test item's bypass status
#define tf_TestItem_BypassStatus() ((TestItem^)site->CurrentFlowItem)->Bypass

#pragma endregion

//--------------------------------------------------

#pragma region "Test Parameter"

//Return test parameter name by test parameter index
#define tf_TestParameter_Name(TPIndex) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPIndex]->Name

//Return test parameter display name by test parameter index
#define tf_TestParameter_DisplayName(TPIndex) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPIndex]->DisplayName

//Return test parameter full name by test parameter index
#define tf_TestParameter_FullName(TPIndex) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPIndex]->FullName

//Return test parameter by test parameter name
#define tf_TestParameter(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]

//Return ConditionCollection by test parameter name
#define tf_TestParameter_ConditionList(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Conditions

//Return boolean, true if condition is exist
#define tf_TestParameter_ConditionExist(TPName, ConditionName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Conditions->ContainsKey(ConditionName)

//Return test parameter's condition value
#define tf_TestParameter_ConditionCast(TPName, ConditionName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Conditions[ConditionName][site]->Value

//Return total test parameter count in current test item
#define tf_TestParameter_Count() ((TestItem^)site->CurrentFlowItem)->TestParameters->Count

//Return test parameter's max limit by test parameter name
#define tf_TestParameter_MaxLimit(TPName) ((TestItem ^)site->CurrentFlowItem)->TestParameters[TPName]->Limit[site]->LimitMax

//Return test parameter's min limit by test parameter name
#define tf_TestParameter_MinLimit(TPName) ((TestItem ^)site->CurrentFlowItem)->TestParameters[TPName]->Limit[site]->LimitMin

//Return test parameter's evel mode by test parameter name
#define tf_TestParameter_EvalMode(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->EvalMode

//Return test parameter's data type by test parameter name
#define tf_TestParameter_DataType(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->DataType

//Return test parameter's prefix by test parameter name
#define tf_TestParameter_Prefix(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Prefix

//Return test parameter's unit by test parameter name
#define tf_TestParameter_Unit(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Unit

//Return test parameter's bypass status by test parameter name
#define tf_TestParameter_BypassStatus(TPName) ((TestItem^)site->CurrentFlowItem)->TestParameters[TPName]->Bypass

#pragma endregion

//--------------------------------------------------

#pragma region "Flow Step (Control Step & Test Step)"

//Return total flow step count (Control Step & Test Step) in current test item
#define tf_FlowStep_Count() ((TestItem^)site->CurrentFlowItem)->FlowSteps->Count;

//Return ConditionCollection by flow step name
#define tf_FlowStep_ConditionList(FlowStepName) ((TestItem^)site->CurrentFlowItem)->FlowSteps[FlowStepName]->Conditions

//Return boolean, true if condition is exist
#define tf_FlowStep_ConditionExist(FlowStepName, ConditionName) ((TestItem^)site->CurrentFlowItem)->FlowSteps[FlowStepName]->Conditions->ContainsKey(ConditionName)

//Return flow step's condition value
#define tf_FlowStep_ConditionCast(FlowStepName, ConditionName) ((TestItem^)site->CurrentFlowItem)->FlowSteps[FlowStepName]->Conditions[ConditionName][site]->Value

//Return flow step's condition count
#define tf_FlowStep_ConditionCount(FlowStepName) ((TestItem^)site->CurrentFlowItem)->FlowSteps[FlowStepName]->Conditions->Count

#pragma endregion

//--------------------------------------------------

#pragma region "Set Result"

//Set test parameter's result
#define tf_SetResult(TPName, Result) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result)

//Set test parameter's result with uut offsets
#define tf_SetResult_UUTOffset(TPName, Result, uutOffset) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result,  site->UUTOffsetResolver->UUTOffsets[uutOffset])

//Set test parameter's result wiht uut offset & pass fail
#define tf_SetResult_UUTOffset_PassFail(TPName, Result, uutOffset, passFail) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result,  site->UUTOffsetResolver->UUTOffsets[uutOffset], passFail)

//Set test parameter's result with defined prefix
#define tf_SetResultPrefix(TPName, Result, AemulusUnitPrefix) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result, AemulusUnitPrefix)

//Set test parameter's result with test time
#define tf_SetResultnTestTime(TPName, Result, TestTime) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result, TestTime)

//Set test parameter's result with uut offsets & test time
#define tf_SetResultnTestTime_byUUTOffsetName(TPName, Result, TestTime, UUTOffsetName) site->SetResult(((TestItem^)site->CurrentFlowItem)->TestParameters[TPName], Result, TestTime, site->UUTOffsetResolver->UUTOffsets[UUTOffsetName])

#pragma endregion

//--------------------------------------------------


/*----------------------------------------------------------------------
* Revision Log
* &Log: TF-Macro.cpp.rca&


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
* Support App-Wide-Variables.
* Support Flow Item (general).
* Support Control Item, Test Item, Test Parameter.
* Support Flow Step (Control Step, Test Step).
* Support tF3 result posting.
----------------------------------------------------------------------*/