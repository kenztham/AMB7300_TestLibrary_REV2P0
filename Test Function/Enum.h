/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			Enum.h
Purpose:		Define global enumerations
Version:		v1.0.0.1
----------------------------------------------------------------------*/


#ifndef ENUM_H
#define ENUM_H

#pragma once

using namespace System;

namespace Functions
{

#pragma region "General"

	public enum class MessageBoxIconFormat
	{
		Asterisk		= 64,
		Error			= 16,
		Exclamation		= 48,
		Hand			= 16,
		None			= 0,
		Information		= 64,
		Question		= 32,
		Stop			= 16,
		Warning			= 48,
	}; 

	public enum class MEAS_MODULE_TYPE
	{
		AM = 0,
		DM = 1,
		CM = 2,
	};

	//TestParameterResultStatus enumeration is different with TestParameterPassFail, where it"s return value of tf_SetResult/tf_SetResult_UUTOffset macro.
	public enum class TestParameterResultStatus
	{
		Fail = 0,		//Test parameter result status = Fail
		Pass = 1,		//Test parameter result status = Pass 
	};

	public enum class MeasureOption
	{
		SHORT,
		OPEN
	};

#pragma endregion

#pragma region "AMB7300"

#pragma region "General & System Info & Project Info"

	public enum class SystemConfig
	{
		A0,			// CMT + MPEX
		A1,			// CMT + MPEX
		A2,			// CMT + MPEX
		A3,			// CMT/VNA + MPEX
		A4,			// CMT/VNA + MPEX
		A5,			// CMT + MPEX
		S2P1D,		// Keysight VNA direct
		S4P1D,		// Keysight VNA direct
		S6P1D,		// Keysight VNA direct
		S6P22N		// Keysight VNA + MPEX(A5)
	};

	//Specifies the project type.
	public enum class ProjectType
	{
		SingleTFSiteSingleUUTOffset,
		SingleTFSiteMultiUUTOffset,
		SingleTFSiteMultiUUTOffsetSharedVNA,
		TrueParallelSingleUUTOffset,
		TrueParallelMultiUUTOffset,
		SingleTFSiteSingleUUTOffsetWithThread,
		SingleTFSiteMultiUUTOffsetWithThread,
		IndexParallel
	};

	// Specifies the VNA test site in the system. 
	// <Note> Only an AMB7300 object per techFlow Site in TrueParallel with UUTOffset project. 
	// <Note> For MultiUUTOffset with single techFlow test site, it's recommended to uses site->index macro. 
	public enum class VNATestSite
	{
		Site0,
		Site1,
		Site2,
		Site3
	};

	//// <Info> The F1, F2 values are calculated as ¼ and ¾ of the frequency range set by one of the following two methods : current or user - defined.
	////		   If the frequency range is defined by a marker, frequency point F2 is not calculated.
	public enum class MethodOption
	{
		CURRENT_SPAN,
		ACTIVE_MARKER,
		USER_SPAN
	};

#pragma endregion

#pragma region "Vna Trigger Setting"
	
	public enum class TRIG_SOURCE
	{
		INTERNAL	= 0,
		EXTERNAL	= 1,
		MANUAL		= 2,
		BUS			= 3
	};

#pragma endregion

#pragma region "Vna Segment Setting"

	public enum class SEGMENT_TRIGGER
	{
		SEGMENT_A		= 1,
		SEGMENT_B		= 1,
		SEGMENT_C		= 1,
		SEGMENT_AB		= 2,
		SEGMENT_BA		= 3,
		SEGMENT_BC		= 4,
		SEGMENT_CB		= 5,
		SEGMENT_ABC		= 6,
		SEGMENT_CBA		= 7,
	};

#pragma endregion

#pragma region "Vna Channel Setting"

	// Specifies/sets the active channel in the system. Maximum up to 16 channels. 
	// <Note> At attempt to set to the active channel the channel, which is not displayed by the DISP:SPL command, an error occurs.
	public enum class VNAChannel
	{
		RESERVED,
		CH1,
		CH2,
		CH3,
		CH4,
		CH5,
		CH6,
		CH7,
		CH8,
		CH9,
		CH10,
		CH11,
		CH12,
		CH13,
		CH14,
		CH15,
		CH16
	};

#pragma endregion

#pragma region "Vna Trace Setting"

	// Specifies the trace number.
	public enum class VNATrace
	{
		RESERVED,
		VNA_TRACE1,	
		VNA_TRACE2,	
		VNA_TRACE3,	
		VNA_TRACE4,	
		VNA_TRACE5,
		VNA_TRACE6,
		VNA_TRACE7,
		VNA_TRACE8,
		VNA_TRACE9,
		VNA_TRACE10,
		VNA_TRACE11,
		VNA_TRACE12,
		VNA_TRACE13,
		VNA_TRACE14,
		VNA_TRACE15,
		VNA_TRACE16
	};

	public enum class VNATraceFormat
	{
		MLOGarithmic,	// Logarithmic magnitude.			//1 parameter | real = para1 db
		PHASe,			// Phase.							//1 parameter | real = para1 deg
		GDELay,			// Group delay time.				//1 parameter | real = para1 s
		SLINear,		// Smith chart format (Lin).		//2 parameter | para1 u				| para2 deg											// [Future Enchancement] Wrapper return error, need further investigate and fix it.
		SLOGarithmic,	// Smith chart format (Log).		//2 parameter | real = para1 db		| img = para2 deg
		SCOMplex,		// Smith chart format (Real/Imag).	//2 parameter | real = para1 u		| img = para2 u
		SMITh,			// Smith chart format (R + jX).		//3 parameter | real = para1 ohm	| img = para2 ohm	| para3 F not yet handle		// [Future Enchancement] Wrapper return error, need further investigate and fix it.
		SADMittance,	// Smith chart format (G + jB).		//3 parameter | real = para1 s		| img = para2 s		| para3 F not yet handle		// [Future Enchancement] Wrapper return error, need further investigate and fix it.
		PLINear,		// Polar format (Lin).				//2 parameter | real = para2 deg	| img = ?			| para1 on sfp but not grab		// [Future Enchancement] Wrapper return error, need further investigate and fix it.
		PLOGarithmic,	// Polar format (Log).				//2 parameter | real = para1 db		| img = para2 deg	
		POLar,			// Polar format(Real / Imag).		//2 parameter | real = para1 u		| img = para2 u
		MLINear,		// Linear magnitude.				//1 parameter | real = para1 u		
		SWR,			// Voltage standing wave ratio.		//1 parameter | real = para1
		REAL,			// Real part.						//1 parameter | real = para1 u
		IMAGinary,		// Imaginary part.					//1 parameter | real = para1 u
		UPHase			// Unwrapped phase.					//1 parameter | real = para1 deg
	};
	
	// Specifies the trace parameter type.
	public enum class VNATraceParameterType
	{
		TraceParameterType_S,
		TraceParameterType_T,
		TraceParameterType_R,
		TraceParameterType_A,
		TraceParameterType_B
	};

#pragma endregion

#pragma region "Specified Measure SNP Type"

	public enum class Measure_SNP_Type
	{
		RESERVED0,
		Measure_S1P,
		Measure_S2P,
		Measure_S3P,
		Measure_S4P,
		Measure_S5P,
		Measure_S6P
	};

#pragma endregion

#pragma region "Marker Search Min Max"

	// Sets the type of the trace hold function. The function holds the trace at the maximum or minimum point. 
	public enum class MarkerSearchType
	{
		FUNC_MAX_TYPE,				// Maximum value search 
		FUNC_MIN_TYPE				// Minimum value search 
	};

#pragma endregion

#pragma region "Marker Search Peak"

	// Specifies the marker search peak type.  
	public enum class PeakSearchType
	{
		RESERVED0,
		RESERVED1,
		FUNC_PEAK_TYPE,				// Peak search 
		FUNC_LEFT_PEAK_TYPE,		// Peak search to the left from the marker 
		FUNC_RIGHT_PEAK_TYPE,		// Peak search to the right from the marker
		RESERVED5,
		RESERVED6,
		RESERVED7
	};

	// Specifies the polarity to perform the peak search.   
	public enum class PPOLType
	{
		PPOL_POSITIVE_TYPE,			// Positive peaks   
		PPOL_NEGATIVE_TYPE,			// Negative peaks 
		PPOL_BOTH_POS_NEG_TYPE		// Both positive peaks and negative peaks
	};

#pragma endregion

#pragma region "Marker Search Target"

	// Specifies the marker search target type.  
	public enum class TargetMarkerSearchType
	{
		RESERVED0,
		RESERVED1,
		RESERVED2,
		RESERVED3,
		RESERVED4,
		FUNC_TARGET_TYPE,			// Target search 
		FUNC_LEFT_TARGET_TYPE,		// Target search to the left from the marker
		FUNC_RIGHT_TARGET_TYPE		// Target search to the right from the marker
	};


	// Specifies the target transition type. 
	public enum class TTRType
	{
		TTR_POSITIVE_TYPE,			// Positive target transition   
		TTR_NEGATIVE_TYPE,			// Negative target transition  
		TTR_BOTH_POS_NEG_TYPE		// Both positive and target transition 
	};

#pragma endregion

#pragma region "Marker Math"

	public enum class MarkerMathType
	{
		BWSearch,
		Flatness,
		Statistics
	};

#pragma endregion

#pragma region "Marker"

	// Specifies the marker index.
	public enum class VNAMARKER
	{
		RESERVED,
		MARKER1,
		MARKER2,
		MARKER3,
		MARKER4,
		MARKER5,
		MARKER6,
		MARKER7,
		MARKER8,
		MARKER9,
		MARKER10,
		MARKER11,
		MARKER12,
		MARKER13,
		MARKER14,
		MARKER15,
		MARKER16
	};

#pragma endregion

#pragma region "Ripple Limit"

	public enum class RippleLimitType
	{
		Off,
		On
	};

	public enum class RippleValueType
	{
		Absolute,
		Margin
	};

#pragma endregion

#pragma region "Limit Test"

	public enum class LimitTestControlType
	{
		Off,
		On
	};

	// Specifies the limit test type.   
	public enum class LimitTestType
	{
		OFF,				// Segment not used for the limit test  
		UPPER_LIMIT,		// The MAX limit type check if the trace falls outside of the upper limit
		LOWER_LIMIT,		// The MIN limit type check if the trace falls outside of the lower limit
		SINGLE_POINT_LIMIT,	// The SINGLE limit type check if the trace (only take effect on specified single point of Start stimulus) falls outside of the response limit
	};

#pragma endregion

#pragma region "Save To Touchstone File"

	// Specifies the Touchstone file separator symbol when the S–parameters are saved.
	public enum class SnPSeparator
	{
		TAB,				// Tab type "	" file separator symbol. 
		SPACE				// Space type " " file separator symbol. 
	};

	// Specifies the data format for the S–parameter saving by the MMEM:STOR:SNP command. 
	public enum class SnPFormat
	{
		SNP_MA_FORMAT,		//	S2VNA: Magnitude-Angle	| Vna Measurement Format: Lin Mag		
		SNP_DB_FORMAT,		//	S2VNA: dB-Angle			| Vna Measurement Format: Log Mag		
		SNP_RI_FORMAT		//	S2VNA: Real-Imaginary	| Vna Measurement Format: Real/Imag	
	};

	// Specifies the touchstone file format.
	public enum class TouchstoneFileFormat
	{
		Time_Domain,		// Time domain touchstone
		Frequency_Domain	// Frequency domain touchstone
	};

#pragma endregion

#pragma endregion

#pragma region "CM400x"
#pragma endregion

#pragma region "AM400x"
#pragma endregion

#pragma region "DM400x"

	public enum class PMU_MEAS_TYPE
	{
		PMU_STATIC_MEAS		= 0,
		PMU_DYNAMIC_MEAS	= 1,
	};

	public enum class PMU_MEAS_MODE
	{
		DM_MEASURECURRENT	= 0,
		DM_MEASUREVOLTAGE	= 1,
	};

	public enum class PMU_ARMMEAS_DYNMEAS
	{
		SINGLE_TRIG_ARM_DYN_MEAS	= 0,
		MULTIPLE_TRIG_ARM_DYN_MEAS	= 1,
	};

#pragma endregion

#pragma region "IOM400x"
#pragma endregion

#pragma region "ACM400x"
#pragma endregion

#pragma region "TM400x"

	public enum class TMU_SLOPE_EDGE
	{
		POSITIVE_EDGE	= 0,
		NEGATIVE_EDGE	= 1,
	};

	public enum class TMU_COMPARE_SEL
	{
		COMPARE_LOW		= 0,
		COMPARE_HIGH	= 1,
	};

	public enum class TMUMode
	{
		BOTH_START_STOP							= 0,	// Uses both START and STOP signals
		START_INPUT_TRIG						= 1,	// Uses TMU input trigger to start the timestamp and use STOP signal to stop the timestamp.Accuracy of the TMU results for mode 1 is within 100ns as the input trigger signal is sampled by backplane 10MHz clock.
		START_ONLY_MEAS_PERIOD_IMMEDIATELY		= 2,	// Performs period measurement immediately.Uses START signal only.
		START_ONLY_MEAS_PERIOD_RECEIVED_TRIG	= 3,	// Performs period measurement once trigger is received.Uses START signal only.
		START_ONLY_MEAS_PULSE_IMMEDIATELY		= 4,	// Performs pulse counting immediately.Uses START signal only.
		START_ONLY_MEAS_PULSE_RECEIVED_TRIG		= 5,	// Performs pulse counting once trigger is received.Uses START signal only.
	};

#pragma endregion

}

#endif


/*----------------------------------------------------------------------
* Revision Log
* $Log: Enum.h.rca$


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
----------------------------------------------------------------------*/