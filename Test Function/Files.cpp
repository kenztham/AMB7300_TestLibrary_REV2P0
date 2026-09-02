/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			Files.cpp
Purpose:		Contain operation and execution of files and folders
Version:		v1.0.0.3
----------------------------------------------------------------------*/


#include "TestFunction.h"

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Running Production
	**	----------------------------------------------------------------------------------------------------
	*/
	bool TestFunction::IsRunningProduction(Site ^ site)
	{
		/*****************************************************************************************************
		** IsRunningProduction
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to check whether is running production mode as Applications Calibration and
		**		Auto_GU_Correlation_Tool.exe can not run in production mode.
		******************************************************************************************************/

		IProductionSystem^ ps = (IProductionSystem^)site->FlowEngine->EngineManager->GetService(IProductionSystem::typeid);

		if (RemotingServices::IsTransparentProxy(ps))
			return false;
		else
			return true;
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	BoardLoss File Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::CheckExistingBoardLossFileContent(int tfSite, String ^ fileDirectory, String ^ fileName)
	{
		/*****************************************************************************************************
		** CheckExistingBoardLossFileContent
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is boardloss file directory.
		**		fileName		- This is boardloss file name.
		**
		** Descriptions:
		**		This is a function to load the existing 'BoardLoss_TesterID_Sx.csv' file contents.
		**		Get all the info, such as Header, Index, Hardware Path, S0/S1/S2/...
		**		Check the header and file format.
		**		If the existing 'BoardLoss_TesterID_Sx.csv' file format does not match, 
		**		force to generate a new 'BoardLoss_TesterID_Sx.csv' file with empty boardloss factor.
		******************************************************************************************************/

		// Local variable
		int ret										= 0;
		int totalFactorCount						= 0;
		bool isHeaderDeviceNameCorrect				= false;
		bool isHeaderProgramNameCorrect				= false;
		bool isHeaderProgramVersionCorrect			= false;
		bool isHeaderBoardLossFileVersionCorrect	= false;
		bool isHeaderBoardLossFileDateCorrect		= false;
		bool isHeaderTesterIDCorrect				= false;
		bool isHeaderIndexCorrect					= false;
		String ^ csvFileName						= fileName;
		String ^ readLine							= String::Empty;
		array<String^> ^ arrStr						= gcnew array<String ^>(0);
		array<String^> ^ separator					= gcnew array<String ^>(1);
		separator[0]								= ",";
		StreamReader ^ reader;
	
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get total existing boardloss factor & check file format
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == true)
			{
				if (arrStr[0]->Contains("Device Name") == true)
				{
					isHeaderDeviceNameCorrect = true;

					if (arrStr[1] != glob->tf.DeviceName)
					{
						ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_VALUE_INVALID;
						WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						ret = 0;
						glob->BoardLoss.isCreateNewBoardLossFile = true;
						goto EndOfTest;
					}
				}
				else if (arrStr[0]->Contains("Program Name") == true)
				{
					isHeaderProgramNameCorrect = true;

					// [Future Enchancement] To get the correct program name. 
					
					//if (arrStr[1] != glob->tf.ProgramName)
					//{
					//	ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_VALUE_INVALID;
					//	WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	ret = 0;
					//	glob->BoardLoss.isCreateNewBoardLossFile = true;
					//	goto EndOfTest;
					//}
				}
				else if (arrStr[0]->Contains("Program Version") == true)
				{
					isHeaderProgramVersionCorrect = true;

					// [Future Enchancement] To get the correct program revision.

					//if (arrStr[1] != glob->tf.ProgramRevision)
					//{
					//	ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_VALUE_INVALID;
					//	WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	ret = 0;
					//	glob->BoardLoss.isCreateNewBoardLossFile = true;
					//	goto EndOfTest;
					//}
				}
				else if (arrStr[0]->Contains("BoardLoss File Version") == true)
				{
					isHeaderBoardLossFileVersionCorrect = true;
				}
				else if (arrStr[0]->Contains("BoardLoss File Date") == true)
				{
					isHeaderBoardLossFileDateCorrect = true;
				}
				else if (arrStr[0]->Contains("Tester ID") == true)
				{
					isHeaderTesterIDCorrect = true;

					if (arrStr[1] != glob->TesterId)
					{
						ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_VALUE_INVALID;
						WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						ret = 0;
						glob->BoardLoss.isCreateNewBoardLossFile = true;
						goto EndOfTest;
					}
				}
				else if (arrStr[0]->Contains("Index") == true)
				{
					isHeaderIndexCorrect = true;
				}
			}
			else
			{
				totalFactorCount++;
			}
		}
		reader->Close();
		reader = nullptr;

		if ((isHeaderDeviceNameCorrect == false)			||
			(isHeaderProgramNameCorrect == false)			||
			(isHeaderProgramVersionCorrect == false)		||
			(isHeaderBoardLossFileVersionCorrect == false)	||
			(isHeaderBoardLossFileDateCorrect == false)		||
			(isHeaderTesterIDCorrect == false)				||
			(isHeaderIndexCorrect == false))
		{
			ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_NAME_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header name inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] Invalid header name inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			glob->BoardLoss.isCreateNewBoardLossFile = true;
			goto EndOfTest;
		}
			
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->BoardLoss.index			= gcnew array<int>(totalFactorCount);
		glob->BoardLoss.hardwarePath	= gcnew array<String^>(totalFactorCount);
		glob->BoardLoss.factorValue		= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count

		for (int i = 0; i < totalFactorCount; i++)
		{
			glob->BoardLoss.index[i]		= 0;
			glob->BoardLoss.hardwarePath[i] = String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->BoardLoss.factorValue[i] = gcnew array<double>(totalFactorCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get existing boardloss factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		int index = 0;
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				try
				{ 
					// Index
					Util->StringToInteger(arrStr[0], glob->BoardLoss.index[index]);
					// Hardware Path
					glob->BoardLoss.hardwarePath[index]	= arrStr[1];
					// S0
					Util->StringToDouble(arrStr[2], glob->BoardLoss.factorValue[0][index]);
					// S1
					if (glob->tf.NumberOfTestSites == 2)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					}
					// S2
					else if (glob->tf.NumberOfTestSites == 3)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					}
					// S3
					else if (glob->tf.NumberOfTestSites == 4)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					}
					// S4
					else if (glob->tf.NumberOfTestSites == 5)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					}
					// S5
					else if (glob->tf.NumberOfTestSites == 6)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
						Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
					}
					// S6
					else if (glob->tf.NumberOfTestSites == 7)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
						Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
						Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
					}
					// S7
					else if (glob->tf.NumberOfTestSites == 8)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
						Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
						Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
						Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
					}
					// S8
					else if (glob->tf.NumberOfTestSites == 9)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
						Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
						Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
						Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
						Util->StringToDouble(arrStr[10], glob->BoardLoss.factorValue[8][index]);
					}
					// S9
					else if (glob->tf.NumberOfTestSites == 10)
					{
						Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
						Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
						Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
						Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
						Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
						Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
						Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
						Util->StringToDouble(arrStr[10], glob->BoardLoss.factorValue[8][index]);
						Util->StringToDouble(arrStr[11], glob->BoardLoss.factorValue[9][index]);
					}
					index++;
				}
				catch (Exception^ ex)
				{
					reader->Close();
					reader = nullptr;
					ret = ER_CONST_PROJECT_BOARDLOSSFILE_HEADER_NAME_INVALID;
					WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] tF UUT offet count does not match with the UUT offset count inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder -> CheckExistingBoardLossFileContent] tF UUT offet count does not match with the UUT offset count inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					ret = 0;
					glob->BoardLoss.isCreateNewBoardLossFile = true;
					goto EndOfTest;
				}
			}
		}
		reader->Close();
		reader = nullptr;

	EndOfTest:
		ret = 0;
	}
	void TestFunction::GenerateBoardLossFile(int tfSite, String ^ fileDirectory)
	{
		/*****************************************************************************************************
		** GenerateBoardLossFile
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is boardloss file directory.
		**
		** Descriptions:
		**		This is a function to generate the 'BoardLoss_TesterID_Sx.csv' file with empty boardloss factor.
		******************************************************************************************************/

		// Local variable
		StringBuilder ^ strBuilder	= gcnew StringBuilder();
		StreamWriter ^ strWritter	= gcnew StreamWriter(fileDirectory);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		int totalFactorCount				= 3;	// Fix to 3 factor as an exmple only. User will need to open the file and re-configure the 'Hardware Path' & boardloss factor
		glob->BoardLoss.index				= gcnew array<int>(totalFactorCount);
		glob->BoardLoss.hardwarePath		= gcnew array<String^>(totalFactorCount);
		glob->BoardLoss.factorValue			= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count

		for (int i = 0; i < totalFactorCount; i++)
		{
			glob->BoardLoss.index[i]			= 0;
			glob->BoardLoss.hardwarePath[i]		= String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->BoardLoss.factorValue[i] = gcnew array<double>(totalFactorCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Generate BoardLoss file
		**	----------------------------------------------------------------------------------------------------
		*/

		/*
		** BoardLoss File Format:
		**	//Device Name,Sample_DeviceName,,,,,
		**	//Program Name,Sample_ProgramName,,,,,
		**	//Program Version,1,,,,,
		**	//BoardLoss File Version,1,,,,,
		**	//BoardLoss File Date,XX-Jan-20XX,,,,,
		**	//TesterID,XXX-LT,,,,,
		**	//Index,Hardware Path,S0,S1,S2,S3
		**	0, RFC, -0.5, 0.3, 1.2, -2
		*/

		glob->BoardLoss.BoardLossFileVersion = glob->tf.ProgramRevision;
		String ^ headerUutOffsetLine = String::Empty;
		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			if (i == glob->tf.NumberOfTestSites - 1)
			{
				headerUutOffsetLine = headerUutOffsetLine + "S" + i;
			}
			else
			{
				headerUutOffsetLine = headerUutOffsetLine + "S" + i + ",";
			}
		}
		String ^ overallHeaderLine =	"//Device Name,"					+ glob->tf.DeviceName					+ "\n" +
										"//Program Name,"					+ glob->tf.ProgramName					+ "\n" +
										"//Program Version,"				+ glob->tf.ProgramRevision				+ "\n" +
										"//BoardLoss File Version,"			+ glob->BoardLoss.BoardLossFileVersion	+ "\n" +
										"//BoardLoss File Date,"			+ glob->FileNameTime					+ "\n" +
										"//Tester ID,"						+ glob->TesterId						+ "\n" +
										"//Index,Hardware Path,"			+ headerUutOffsetLine;
		strBuilder->AppendLine(overallHeaderLine);

		// Index,Hardware Path,S0/S1/S2/...
		if (glob->tf.NumberOfTestSites == 1)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0");
			strBuilder->AppendLine();
		}
		// S1
		else if (glob->tf.NumberOfTestSites == 2)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S2
		else if (glob->tf.NumberOfTestSites == 3)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S3
		else if (glob->tf.NumberOfTestSites == 4)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S4
		else if (glob->tf.NumberOfTestSites == 5)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S5
		else if (glob->tf.NumberOfTestSites == 6)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S6
		else if (glob->tf.NumberOfTestSites == 7)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S7
		else if (glob->tf.NumberOfTestSites == 8)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S8
		else if (glob->tf.NumberOfTestSites == 9)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		// S9
		else if (glob->tf.NumberOfTestSites == 10)
		{
			strBuilder->Append("0" + "," + "LB_RFC" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("1" + "," + "LB_RF1" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
			strBuilder->Append("2" + "," + "LB_RF2" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0");
			strBuilder->AppendLine();
		}
		strWritter->Write(strBuilder);
		strBuilder = nullptr;
		strWritter->Close();
	}
	void TestFunction::LoadBoardLossFile(int tfSite, String ^ fileDirectory)
	{
		/*****************************************************************************************************
		** LoadBoardLossFile
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is boardloss file directory.
		**
		** Descriptions:
		**		This is a function to load the latest 'BoardLoss_TesterID_Sx.csv' file and its contents.
		******************************************************************************************************/

		// Local variable
		int totalFactorCount			= 0;
		String ^ readLine				= String::Empty;
		array<String^> ^ arrStr			= gcnew array<String ^>(0);
		array<String^> ^ separator		= gcnew array<String ^>(1);
		separator[0]					= ",";
		StreamReader ^ reader;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get total existing boardloss factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				totalFactorCount++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->BoardLoss.index				= gcnew array<int>(totalFactorCount);
		glob->BoardLoss.hardwarePath		= gcnew array<String^>(totalFactorCount);
		glob->BoardLoss.factorValue			= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count
		array<String^> ^ hardwarePathKey	= gcnew array<String^>(totalFactorCount);

		for (int i = 0; i < totalFactorCount; i++)
		{
			glob->BoardLoss.index[i]			= 0;
			glob->BoardLoss.hardwarePath[i]		= String::Empty;
			hardwarePathKey[i]					= String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->BoardLoss.factorValue[i] = gcnew array<double>(totalFactorCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get existing boardloss factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		int index = 0;
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				// Index
				Util->StringToInteger(arrStr[0], glob->BoardLoss.index[index]);
				// Hardware Path
				glob->BoardLoss.hardwarePath[index]	= arrStr[1];
				hardwarePathKey[index]				= arrStr[1];
				// S0
				Util->StringToDouble(arrStr[2], glob->BoardLoss.factorValue[0][index]);
				// S1
				if (glob->tf.NumberOfTestSites == 2)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
				}
				// S2
				else if (glob->tf.NumberOfTestSites == 3)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
				}
				// S3
				else if (glob->tf.NumberOfTestSites == 4)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
				}
				// S4
				else if (glob->tf.NumberOfTestSites == 5)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
				}
				// S5
				else if (glob->tf.NumberOfTestSites == 6)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
				}
				// S6
				else if (glob->tf.NumberOfTestSites == 7)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
					Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
				}
				// S7
				else if (glob->tf.NumberOfTestSites == 8)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
					Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
					Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
				}
				// S8
				else if (glob->tf.NumberOfTestSites == 9)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[5][index]);
					Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
					Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
					Util->StringToDouble(arrStr[10], glob->BoardLoss.factorValue[8][index]);
				}
				// S9
				else if (glob->tf.NumberOfTestSites == 10)
				{
					Util->StringToDouble(arrStr[3], glob->BoardLoss.factorValue[1][index]);
					Util->StringToDouble(arrStr[4], glob->BoardLoss.factorValue[2][index]);
					Util->StringToDouble(arrStr[5], glob->BoardLoss.factorValue[3][index]);
					Util->StringToDouble(arrStr[6], glob->BoardLoss.factorValue[4][index]);
					Util->StringToDouble(arrStr[7], glob->BoardLoss.factorValue[5][index]);
					Util->StringToDouble(arrStr[8], glob->BoardLoss.factorValue[6][index]);
					Util->StringToDouble(arrStr[9], glob->BoardLoss.factorValue[7][index]);
					Util->StringToDouble(arrStr[10], glob->BoardLoss.factorValue[8][index]);
					Util->StringToDouble(arrStr[11], glob->BoardLoss.factorValue[9][index]);
				}
				index++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Transfer to global variable
		**	----------------------------------------------------------------------------------------------------
		*/
		array<double> ^ arrTemp;
		array<double, 2> ^ factorTemp = gcnew array<double, 2>(glob->tf.NumberOfTestSites, totalFactorCount);
		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			for (int j = 0; j < totalFactorCount; j++)
			{
				factorTemp[i, j] = 0.0;
			}
		}

		for (int i = 0; i < totalFactorCount; i++)
		{
			// S0
			factorTemp[0, i] = glob->BoardLoss.factorValue[0][i];
			// S1
			if (glob->tf.NumberOfTestSites == 2)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
			}
			// S2
			else if (glob->tf.NumberOfTestSites == 3)
			{
				factorTemp[1, i] =  glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] =  glob->BoardLoss.factorValue[2][i];
			}
			// S3
			else if (glob->tf.NumberOfTestSites == 4)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
			}
			// S4
			else if (glob->tf.NumberOfTestSites == 5)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
			}
			// S5
			else if (glob->tf.NumberOfTestSites == 6)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
				factorTemp[5, i] = glob->BoardLoss.factorValue[5][i];
			}
			// S6
			else if (glob->tf.NumberOfTestSites == 7)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
				factorTemp[5, i] = glob->BoardLoss.factorValue[5][i];
				factorTemp[6, i] = glob->BoardLoss.factorValue[6][i];
			}
			// S7
			else if (glob->tf.NumberOfTestSites == 8)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
				factorTemp[5, i] = glob->BoardLoss.factorValue[5][i];
				factorTemp[6, i] = glob->BoardLoss.factorValue[6][i];
				factorTemp[7, i] = glob->BoardLoss.factorValue[7][i];
			}
			// S8
			else if (glob->tf.NumberOfTestSites == 9)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
				factorTemp[5, i] = glob->BoardLoss.factorValue[5][i];
				factorTemp[6, i] = glob->BoardLoss.factorValue[6][i];
				factorTemp[7, i] = glob->BoardLoss.factorValue[7][i];
				factorTemp[8, i] = glob->BoardLoss.factorValue[8][i];
			}
			// S9
			else if (glob->tf.NumberOfTestSites == 10)
			{
				factorTemp[1, i] = glob->BoardLoss.factorValue[1][i];
				factorTemp[2, i] = glob->BoardLoss.factorValue[2][i];
				factorTemp[3, i] = glob->BoardLoss.factorValue[3][i];
				factorTemp[4, i] = glob->BoardLoss.factorValue[4][i];
				factorTemp[5, i] = glob->BoardLoss.factorValue[5][i];
				factorTemp[6, i] = glob->BoardLoss.factorValue[6][i];
				factorTemp[7, i] = glob->BoardLoss.factorValue[7][i];
				factorTemp[8, i] = glob->BoardLoss.factorValue[8][i];
				factorTemp[9, i] = glob->BoardLoss.factorValue[9][i];
			}
		
			arrTemp = gcnew array<double>(glob->tf.NumberOfTestSites);
			for (int j = 0; j < glob->tf.NumberOfTestSites; j++)
			{
				arrTemp[j] = factorTemp[j, i];
			}

			if (glob->BoardLoss.BoardLossFactor->ContainsKey(hardwarePathKey[i]))
			{
				glob->BoardLoss.BoardLossFactor[hardwarePathKey[i]] = arrTemp;
			}
			else
			{
				glob->BoardLoss.BoardLossFactor->Add(hardwarePathKey[i], arrTemp);
			}
		}
	}
	double TestFunction::GetBoardLossFactor(int tfSite, int siteIndex, String ^ hardwarePathKey)
	{
		/*****************************************************************************************************
		** GetBoardLossFactor
		**		tfSite			- This is techFlow site index.
		**		siteIndex		- This is resource object index, normally start from 0.
		**		hardwarePathKey	- This is the hardware path name, also served as the KEY.
		**
		** Descriptions:
		**		This is a function to get the boardloss factor from the global variable. 
		******************************************************************************************************/

		// Local variable
		double boardlossFactor = 0.0;

		if (glob->BoardLoss.BoardLossFactor->ContainsKey(hardwarePathKey))
		{
			boardlossFactor = glob->BoardLoss.BoardLossFactor[hardwarePathKey][siteIndex];
		}
		else
		{
			boardlossFactor = 0.0;
		}

		return boardlossFactor;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	DeviceStateFile Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::LoadDeviceStateFileTemplate(int tfSite)
	{
		/*****************************************************************************************************
		** LoadDeviceStateFileTemplate
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to load the device state file (.sta) & mapping file (.xml).
		******************************************************************************************************/

		// Local variable
		int ret	= 0;

		// State file (.csa type)
		glob->DeviceStateFileTemplate.totalStateFileExist = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.csa")->Length;

		if (glob->DeviceStateFileTemplate.totalStateFileExist != 1)
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid = true;
			glob->DeviceStateFileTemplate.stateFileDirectory_csa = String::Empty;

			ret = ER_CONST_PROJECT_DEVICESTATEFILETEMPLATE_STATEFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device state file (.sta type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device state file (.sta type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
		}
		else
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid = false;
			array<String^> ^ temp = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.csa");
			glob->DeviceStateFileTemplate.stateFileDirectory_csa = temp[0];
		}

		// State file (.sta type)
		glob->DeviceStateFileTemplate.totalStateFileExist = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.sta")->Length;

		if (glob->DeviceStateFileTemplate.totalStateFileExist != 1)
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid = true;
			glob->DeviceStateFileTemplate.stateFileDirectory_sta = String::Empty;

			ret = ER_CONST_PROJECT_DEVICESTATEFILETEMPLATE_STATEFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device state file (.sta type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device state file (.sta type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
		}
		else
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid = false;
			array<String^> ^ temp = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.sta");
			glob->DeviceStateFileTemplate.stateFileDirectory_sta = temp[0];
		}

		// Mapping file (.xml type)
		glob->DeviceStateFileTemplate.totalMappingFileExist = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.xml")->Length;

		if (glob->DeviceStateFileTemplate.totalMappingFileExist != 1)
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateMappingFileInvalid = true;
			glob->DeviceStateFileTemplate.mappingFileDirectory = String::Empty;

			ret = ER_CONST_PROJECT_DEVICESTATEFILETEMPLATE_MAPPINGFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device mapping file (.xml type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder -> LoadDeviceStateFileTemplate] Device mapping file (.xml type) should not exist more than 1x or empty in the '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
		}
		else
		{
			glob->DeviceStateFileTemplate.isDeviceStateFileTemplateMappingFileInvalid = false;
			array<String^> ^ temp = Directory::GetFiles(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory, "*.xml");
			glob->DeviceStateFileTemplate.mappingFileDirectory = temp[0];
		}
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	FixedOffset File Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::CheckExistingFixedOffsetFileContent(Site ^ site, int tfSite, String ^ fileDirectory, String ^ fileName)
	{
		/*****************************************************************************************************
		** CheckExistingFixedOffsetFileContent
		**		site			- This is techFlow site object.
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is fixed offset file directory.
		**		fileName		- This is fixed offset file name.
		**
		** Descriptions:
		**		This is a function to load the existing 'FixedOffset_TesterID_Sx.csv' file contents.
		**		Get all the info, such as Header, Index, TestItem, TestParameter, S0/S1/S2/...
		**		Compare and check the contents with the project test recipes contents.
		**		If the existing 'FixedOffset_TesterID_Sx.csv' contents do not match with the project test recipes contents, 
		**		force to generate a new 'FixedOffset_TesterID_Sx.csv' based on the latest project test recipes contents.
		******************************************************************************************************/

		// Local variable
		int ret									= 0;
		int totalOffsetCount					= 0;
		bool isHeaderDeviceNameCorrect			= false;
		bool isHeaderProgramNameCorrect			= false;
		bool isHeaderProgramVersionCorrect		= false;
		bool isHeaderOffsetFileVersionCorrect	= false;
		bool isHeaderOffsetFileDateCorrect		= false;
		bool isHeaderTesterIDCorrect			= false;
		bool isHeaderIndexCorrect				= false;
		String ^ csvFileName					= fileName;
		String ^ readLine						= String::Empty;
		array<String^> ^ arrStr					= gcnew array<String ^>(0);
		array<String^> ^ separator				= gcnew array<String ^>(1);
		separator[0]							= ",";
		StreamReader ^ reader;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get total existing fixed offset factor & check file format
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);
		
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == true)
			{
				if (arrStr[0]->Contains("Device Name") == true)
				{
					isHeaderDeviceNameCorrect = true;

					if (arrStr[1] != glob->tf.DeviceName)
					{
						ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_VALUE_INVALID;
						WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						ret = 0;
						glob->FixedOffset.isCreateNewFixedOffsetFile = true;
						goto EndOfTest;
					}
				}
				else if (arrStr[0]->Contains("Program Name") == true)
				{
					isHeaderProgramNameCorrect = true;

					// [Future Enchancement] To get the correct program name. 
					
					//if (arrStr[1] != glob->tf.ProgramName)
					//{
					//	ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_VALUE_INVALID;
					//	WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	ret = 0;
					//	glob->FixedOffset.isCreateNewFixedOffsetFile = true;
					//	goto EndOfTest;
					//}
				}
				else if (arrStr[0]->Contains("Program Version") == true)
				{
					isHeaderProgramVersionCorrect = true;

					// [Future Enchancement] To get the correct program revision.

					//if (arrStr[1] != glob->tf.ProgramRevision)
					//{
					//	ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_VALUE_INVALID;
					//	WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
					//	ret = 0;
					//	glob->FixedOffset.isCreateNewFixedOffsetFile = true;
					//	goto EndOfTest;
					//}
				}
				else if (arrStr[0]->Contains("Offset File Version") == true)
				{
					isHeaderOffsetFileVersionCorrect = true;
				}
				else if (arrStr[0]->Contains("Offset File Date") == true)
				{
					isHeaderOffsetFileDateCorrect = true;
				}
				else if (arrStr[0]->Contains("Tester ID") == true)
				{
					isHeaderTesterIDCorrect = true;

					if (arrStr[1] != glob->TesterId)
					{
						ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_VALUE_INVALID;
						WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header value inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
						ret = 0;
						glob->FixedOffset.isCreateNewFixedOffsetFile = true;
						goto EndOfTest;
					}
				}
				else if (arrStr[0]->Contains("Index") == true)
				{
					isHeaderIndexCorrect = true;
				}
			}
			else
			{
				totalOffsetCount++;
			}
		}
		reader->Close();
		reader = nullptr;

		if (/*(isHeaderDeviceNameCorrect == false)		||
			(isHeaderProgramNameCorrect == false)		||
			(isHeaderProgramVersionCorrect == false)	||
			(isHeaderOffsetFileVersionCorrect == false) ||
			(isHeaderOffsetFileDateCorrect == false)	||
			(isHeaderTesterIDCorrect == false)			||*/
			(isHeaderIndexCorrect == false))
		{
			ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_NAME_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header name inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Invalid header name inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			glob->FixedOffset.isCreateNewFixedOffsetFile = true;
			goto EndOfTest;
		}
			
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->FixedOffset.index				= gcnew array<int>(totalOffsetCount);
		glob->FixedOffset.testItem			= gcnew array<String^>(totalOffsetCount);
		glob->FixedOffset.testParameter		= gcnew array<String^>(totalOffsetCount);
		glob->FixedOffset.offsetValue		= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count

		for (int i = 0; i < totalOffsetCount; i++)
		{
			glob->FixedOffset.index[i]			= 0;
			glob->FixedOffset.testItem[i]		= String::Empty;
			glob->FixedOffset.testParameter[i]	= String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->FixedOffset.offsetValue[i] = gcnew array<double>(totalOffsetCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get existing fixed offset factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		int index = 0;
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				try
				{ 
					// Index
					Util->StringToInteger(arrStr[0], glob->FixedOffset.index[index]);
					// Test Item
					glob->FixedOffset.testItem[index]		= arrStr[1];
					// Test Parameter
					glob->FixedOffset.testParameter[index]	= arrStr[2];
					// S0
					Util->StringToDouble(arrStr[3], glob->FixedOffset.offsetValue[0][index]);
					// S1
					if (glob->tf.NumberOfTestSites == 2)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					}
					// S2
					else if (glob->tf.NumberOfTestSites == 3)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					}
					// S3
					else if (glob->tf.NumberOfTestSites == 4)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					}
					// S4
					else if (glob->tf.NumberOfTestSites == 5)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					}
					// S5
					else if (glob->tf.NumberOfTestSites == 6)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
						Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
					}
					// S6
					else if (glob->tf.NumberOfTestSites == 7)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
						Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
						Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
					}
					// S7
					else if (glob->tf.NumberOfTestSites == 8)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
						Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
						Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
						Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
					}
					// S8
					else if (glob->tf.NumberOfTestSites == 9)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
						Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
						Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
						Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
						Util->StringToDouble(arrStr[11], glob->FixedOffset.offsetValue[8][index]);
					}
					// S9
					else if (glob->tf.NumberOfTestSites == 10)
					{
						Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
						Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
						Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
						Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
						Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
						Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
						Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
						Util->StringToDouble(arrStr[11], glob->FixedOffset.offsetValue[8][index]);
						Util->StringToDouble(arrStr[12], glob->FixedOffset.offsetValue[9][index]);
					}
					index++;
				}
				catch (Exception^ ex)
				{
					reader->Close();
					reader = nullptr;
					ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_HEADER_NAME_INVALID;
					WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] tF UUT offet count does not match with the UUT offset count inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] tF UUT offet count does not match with the UUT offset count inside the '" + csvFileName + "' file. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					ret = 0;
					glob->FixedOffset.isCreateNewFixedOffsetFile = true;
					goto EndOfTest;
				}
			}
		}
		reader->Close();
		reader = nullptr;
			
		/*
		**	----------------------------------------------------------------------------------------------------
		**	Compare and check exisitng fixed offset file contents with the project test recipes contents
		**	----------------------------------------------------------------------------------------------------
		*/
		FlowItemCollection ^ flowItems = site->FlowItems;
		TestItem ^ testItem;
		TestParameter ^ testParameter;
		AFlowSubItem ^ subFlowItem;

		int offsetIndex	= 0;
		for (int i = 0; i < flowItems->Count; i++)
		{
			Type ^ flowItemType = flowItems[i]->GetType();
			if (flowItemType == TestItem::typeid)
			{
				String ^ testItemName	= flowItems[i]->Name;
				testItem = (TestItem^)site->FlowItems[testItemName];

				for each (subFlowItem in testItem->SubItems)
				{
					if (dynamic_cast<TestParameter^>(subFlowItem->Data) != nullptr)
					{
						try
						{
							testParameter = (TestParameter^)testItem->TestParameters[subFlowItem->Name];
							String ^ testParameterName = testParameter->Name;

							// Test item name not match
							if (glob->FixedOffset.testItem[offsetIndex] != testItemName)
							{
								ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_TEST_ITEM_NAME_NOT_MATCH;
								WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test item name inside the '" + csvFileName + "' file does not match with the test item name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test item name inside the '" + csvFileName + "' file does not match with the test item name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								ret = 0;
								glob->FixedOffset.isCreateNewFixedOffsetFile = true;
								goto EndOfTest;
							}

							// Test parameter name not match
							if (glob->FixedOffset.testParameter[offsetIndex] != testParameterName)
							{
								ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_TEST_PARAMETER_NAME_NOT_MATCH;
								WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test parameter name inside the '" + csvFileName + "' file does not match with the test parameter name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test parameter name inside the '" + csvFileName + "' file does not match with the test parameter name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
								ret = 0;
								glob->FixedOffset.isCreateNewFixedOffsetFile = true;
								goto EndOfTest;
							}

							offsetIndex++;
						}
						catch (Exception^ ex)
						{
							ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_TEST_PARAMETER_NAME_NOT_MATCH;
							WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test parameter name inside the '" + csvFileName + "' file does not match with the test parameter name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder -> CheckExistingFixedOffsetFileContent] Test parameter name inside the '" + csvFileName + "' file does not match with the test parameter name inside the project recipes. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
							ret = 0;
							glob->FixedOffset.isCreateNewFixedOffsetFile = true;
							goto EndOfTest;
						}
					}
				}
			}
		}
			
	EndOfTest:
		ret = 0;
	}
	void TestFunction::GenerateFixedOffsetFile(Site ^ site, int tfSite, String ^ fileDirectory)
	{
		/*****************************************************************************************************
		** GenerateFixedOffsetFile
		**		site			- This is techFlow site object.
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is fixed offset file directory.
		**
		** Descriptions:
		**		This is a function to generate the 'FixedOffset_TesterID_Sx.csv' based on the latest project test recipes.
		******************************************************************************************************/

		// Local variable
		StringBuilder ^ strBuilder	= gcnew StringBuilder();
		StreamWriter ^ strWritter	= gcnew StreamWriter(fileDirectory);

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		int totalTpCount					= site->TestParameters->Count;
		glob->FixedOffset.index				= gcnew array<int>(totalTpCount);
		glob->FixedOffset.testItem			= gcnew array<String^>(totalTpCount);
		glob->FixedOffset.testParameter		= gcnew array<String^>(totalTpCount);
		glob->FixedOffset.offsetValue		= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count

		for (int i = 0; i < totalTpCount; i++)
		{
			glob->FixedOffset.index[i]			= 0;
			glob->FixedOffset.testItem[i]		= String::Empty;
			glob->FixedOffset.testParameter[i]	= String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->FixedOffset.offsetValue[i] = gcnew array<double>(totalTpCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Generate FixedOffset file
		**	----------------------------------------------------------------------------------------------------
		*/

		/*
		** FixedOffset File Format:
		**	//Device Name,Sample_DeviceName,,,,,
		**	//Program Name,Sample_ProgramName,,,,,
		**	//Program Version,1,,,,,
		**	//Offset File Version,1,,,,,
		**	//Offset File Date,XX-Jan-20XX,,,,,
		**	//TesterID,XXX-LT,,,,,
		**	//Index,Test Item,Test Parameter,S0,S1,S2,S3
		**	0, B1_IL, B1_IL_824, -0.5, 0.3, 1.2, -2
		*/

		glob->FixedOffset.OffsetFileVersion = glob->tf.ProgramRevision;
		String ^ headerUutOffsetLine = String::Empty;
		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			if (i == glob->tf.NumberOfTestSites - 1)
			{
				headerUutOffsetLine = headerUutOffsetLine + "S" + i;
			}
			else
			{
				headerUutOffsetLine = headerUutOffsetLine + "S" + i + ",";
			}
		}
		String ^ overallHeaderLine =	/*"//Device Name,"					+ glob->tf.DeviceName					+ "\n" +
										"//Program Name,"					+ glob->tf.ProgramName					+ "\n" +
										"//Program Version,"				+ glob->tf.ProgramRevision				+ "\n" +
										"//Offset File Version,"			+ glob->FixedOffset.OffsetFileVersion	+ "\n" +
										"//Offset File Date,"				+ glob->FileNameTime					+ "\n" +
										"//Tester ID,"						+ glob->TesterId						+ "\n" +*/
										"//Index,Test Item,Test Parameter," + headerUutOffsetLine;
		strBuilder->AppendLine(overallHeaderLine);

		FlowItemCollection ^ flowItems = site->FlowItems;
		TestItem ^ testItem;
		TestParameter ^ testParameter;
		AFlowSubItem ^ subFlowItem;
		int offsetIndex = 0;
		for (int i = 0; i < flowItems->Count; i++)
		{
			Type ^ flowItemType = flowItems[i]->GetType();
			if (flowItemType == TestItem::typeid)
			{
				String ^ testItemName	= flowItems[i]->Name;
				testItem = (TestItem^)site->FlowItems[testItemName];

				for each (subFlowItem in testItem->SubItems)
				{
					if (dynamic_cast<TestParameter^>(subFlowItem->Data) != nullptr)
					{
						testParameter = (TestParameter^)testItem->TestParameters[subFlowItem->Name];
						String ^ testParameterName = testParameter->Name;

						// Index
						strBuilder->Append(offsetIndex.ToString() + ",");
						// Test Item
						strBuilder->Append(testItemName + ",");
						// Test Parameter
						strBuilder->Append(testParameterName);
						// S0/S1/S2/...
						for (int j = 0; j < glob->tf.NumberOfTestSites; j++)
						{
							strBuilder->Append(",0");
						}
						strBuilder->AppendLine();
						
						offsetIndex++;
					}
				}
			}
		}
		strWritter->Write(strBuilder);
		strBuilder = nullptr;
		strWritter->Close();
	}
	void TestFunction::LoadFixedOffsetFile(int tfSite, String ^ fileDirectory)
	{
		/*****************************************************************************************************
		** LoadFixedOffsetFile
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is fixed offset file directory.
		**
		** Descriptions:
		**		This is a function to load the latest 'FixedOffset_TesterID_Sx.csv' file and its contents.
		******************************************************************************************************/

		// Local variable
		int totalOffsetCount			= 0;
		String ^ readLine				= String::Empty;
		array<String^> ^ arrStr			= gcnew array<String ^>(0);
		array<String^> ^ separator		= gcnew array<String ^>(1);
		separator[0]					= ",";
		StreamReader ^ reader;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get total existing fixed offset factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				totalOffsetCount++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->FixedOffset.index				= gcnew array<int>(totalOffsetCount);
		glob->FixedOffset.testItem			= gcnew array<String^>(totalOffsetCount);
		glob->FixedOffset.testParameter		= gcnew array<String^>(totalOffsetCount);
		glob->FixedOffset.offsetValue		= gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count
		array<String^> ^ testParameterKey	= gcnew array<String^>(totalOffsetCount);

		for (int i = 0; i < totalOffsetCount; i++)
		{
			glob->FixedOffset.index[i]			= 0;
			glob->FixedOffset.testItem[i]		= String::Empty;
			glob->FixedOffset.testParameter[i]	= String::Empty;
			testParameterKey[i]					= String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->FixedOffset.offsetValue[i] = gcnew array<double>(totalOffsetCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get existing fixed offset factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		int index = 0;
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				// Index
				Util->StringToInteger(arrStr[0], glob->FixedOffset.index[index]);
				// Test Item
				glob->FixedOffset.testItem[index]		= arrStr[1];
				// Test Parameter
				glob->FixedOffset.testParameter[index]	= arrStr[2];
				testParameterKey[index]					= arrStr[2];
				// S0
				Util->StringToDouble(arrStr[3], glob->FixedOffset.offsetValue[0][index]);
				// S1
				if (glob->tf.NumberOfTestSites == 2)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
				}
				// S2
				else if (glob->tf.NumberOfTestSites == 3)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
				}
				// S3
				else if (glob->tf.NumberOfTestSites == 4)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
				}
				// S4
				else if (glob->tf.NumberOfTestSites == 5)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
				}
				// S5
				else if (glob->tf.NumberOfTestSites == 6)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
				}
				// S6
				else if (glob->tf.NumberOfTestSites == 7)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
					Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
				}
				// S7
				else if (glob->tf.NumberOfTestSites == 8)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
					Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
					Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
				}
				// S8
				else if (glob->tf.NumberOfTestSites == 9)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
					Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
					Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
					Util->StringToDouble(arrStr[11], glob->FixedOffset.offsetValue[8][index]);
				}
				// S9
				else if (glob->tf.NumberOfTestSites == 10)
				{
					Util->StringToDouble(arrStr[4], glob->FixedOffset.offsetValue[1][index]);
					Util->StringToDouble(arrStr[5], glob->FixedOffset.offsetValue[2][index]);
					Util->StringToDouble(arrStr[6], glob->FixedOffset.offsetValue[3][index]);
					Util->StringToDouble(arrStr[7], glob->FixedOffset.offsetValue[4][index]);
					Util->StringToDouble(arrStr[8], glob->FixedOffset.offsetValue[5][index]);
					Util->StringToDouble(arrStr[9], glob->FixedOffset.offsetValue[6][index]);
					Util->StringToDouble(arrStr[10], glob->FixedOffset.offsetValue[7][index]);
					Util->StringToDouble(arrStr[11], glob->FixedOffset.offsetValue[8][index]);
					Util->StringToDouble(arrStr[12], glob->FixedOffset.offsetValue[9][index]);
				}
				index++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Transfer to global variable
		**	----------------------------------------------------------------------------------------------------
		*/
		String^ key;
		array<double> ^ arrTemp;
		array<double, 2> ^ offsetTemp = gcnew array<double, 2>(glob->tf.NumberOfTestSites, totalOffsetCount);
		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			for (int j = 0; j < totalOffsetCount; j++)
			{
				offsetTemp[i, j] = 0.0;
			}
		}

		for (int i = 0; i < totalOffsetCount; i++)
		{
			// S0
			offsetTemp[0, i] = glob->FixedOffset.offsetValue[0][i];
			// S1
			if (glob->tf.NumberOfTestSites == 2)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
			}
			// S2
			else if (glob->tf.NumberOfTestSites == 3)
			{
				offsetTemp[1, i] =  glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] =  glob->FixedOffset.offsetValue[2][i];
			}
			// S3
			else if (glob->tf.NumberOfTestSites == 4)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
			}
			// S4
			else if (glob->tf.NumberOfTestSites == 5)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
			}
			// S5
			else if (glob->tf.NumberOfTestSites == 6)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
				offsetTemp[5, i] = glob->FixedOffset.offsetValue[5][i];
			}
			// S6
			else if (glob->tf.NumberOfTestSites == 7)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
				offsetTemp[5, i] = glob->FixedOffset.offsetValue[5][i];
				offsetTemp[6, i] = glob->FixedOffset.offsetValue[6][i];
			}
			// S7
			else if (glob->tf.NumberOfTestSites == 8)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
				offsetTemp[5, i] = glob->FixedOffset.offsetValue[5][i];
				offsetTemp[6, i] = glob->FixedOffset.offsetValue[6][i];
				offsetTemp[7, i] = glob->FixedOffset.offsetValue[7][i];
			}
			// S8
			else if (glob->tf.NumberOfTestSites == 9)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
				offsetTemp[5, i] = glob->FixedOffset.offsetValue[5][i];
				offsetTemp[6, i] = glob->FixedOffset.offsetValue[6][i];
				offsetTemp[7, i] = glob->FixedOffset.offsetValue[7][i];
				offsetTemp[8, i] = glob->FixedOffset.offsetValue[8][i];
			}
			// S9
			else if (glob->tf.NumberOfTestSites == 10)
			{
				offsetTemp[1, i] = glob->FixedOffset.offsetValue[1][i];
				offsetTemp[2, i] = glob->FixedOffset.offsetValue[2][i];
				offsetTemp[3, i] = glob->FixedOffset.offsetValue[3][i];
				offsetTemp[4, i] = glob->FixedOffset.offsetValue[4][i];
				offsetTemp[5, i] = glob->FixedOffset.offsetValue[5][i];
				offsetTemp[6, i] = glob->FixedOffset.offsetValue[6][i];
				offsetTemp[7, i] = glob->FixedOffset.offsetValue[7][i];
				offsetTemp[8, i] = glob->FixedOffset.offsetValue[8][i];
				offsetTemp[9, i] = glob->FixedOffset.offsetValue[9][i];
			}
		
			//arrTemp = gcnew array<double>(glob->tf.NumberOfTestSites);
			for (int j = 0; j < glob->tf.NumberOfTestSites; j++)
			{
				//arrTemp[j] = offsetTemp[j, i];
				key = glob->FixedOffset.testItem[i] + "_" + glob->FixedOffset.testParameter[i] + "_S" + j.ToString();

				if (glob->FixedOffset.ResultOffset->ContainsKey(key))
				{
					glob->FixedOffset.ResultOffset[key] = offsetTemp[j, i];
				}
				else
				{
					glob->FixedOffset.ResultOffset->Add(key, offsetTemp[j, i]);
				}
			}

		}
	}
	double TestFunction::GetFixedOffsetValue(int tfSite, int siteIndex, String ^ testParameterKey)
	{
		/*****************************************************************************************************
		** GetFixedOffsetValue
		**		tfSite						- This is techFlow site index.
		**		siteIndex					- This is resource's object index, normally start from 0.
		**		TestParameterDisplayName	- This is the display name of the current test paramater.
		**
		** Descriptions:
		**		This is a function to get the fixed offset factor from the global variable. 
		**		Where these offset factor need to be added into the measurement result at UpdateTestResults function.
		******************************************************************************************************/

		// Local variable
		double fixedOffsetValue = 0.0;

		if (glob->FixedOffset.ResultOffset->ContainsKey(testParameterKey))
		{
			fixedOffsetValue = glob->FixedOffset.ResultOffset[testParameterKey];
		}
		else
		{
			fixedOffsetValue = 0.0;
		}

		return fixedOffsetValue;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Modulation File Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::LoadModulationFile(int tfSite)
	{
		/*****************************************************************************************************
		** LoadModulationFile
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to load the modulation file list (.awf and .wfm type).
		******************************************************************************************************/

		// Local variable
		int ret	= 0;

		// Get modulation file list (.awf)
		glob->ModulationFile.totalAwfTypeFileExist = Directory::GetFiles(glob->ModulationFile.ModulationFileFolderDirectory, "*.awf")->Length;

		if (glob->ModulationFile.totalAwfTypeFileExist != 0)
		{
			glob->ModulationFile.awfTypeFilePathList	= gcnew array<String^>(glob->ModulationFile.totalAwfTypeFileExist);
			glob->ModulationFile.awfTypeFilePathList	= Directory::GetFiles(glob->ModulationFile.ModulationFileFolderDirectory, "*.awf");

			// Get modulation file name
			glob->ModulationFile.awfTypeFileName		= gcnew array<String^>(glob->ModulationFile.totalAwfTypeFileExist);
														
			array<String ^> ^ arrStr					= gcnew array<String ^>(0);
			array<String ^> ^ separator					= gcnew array<String ^>(1);
			separator[0]								= "\\";

			for (int j = 0; j < glob->ModulationFile.totalAwfTypeFileExist; j++)
			{
				arrStr										= glob->ModulationFile.awfTypeFilePathList[j]->Split(separator, StringSplitOptions::None);
				glob->ModulationFile.awfTypeFileName[j]		= arrStr[8];
				glob->ModulationFile.awfTypeFileName[j]		= glob->ModulationFile.awfTypeFileName[j]->Replace(".awf", "");
			}

			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.awf type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->ModulationFile.totalAwfTypeFileExist.ToString() + ".");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.awf type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->ModulationFile.totalAwfTypeFileExist.ToString() + ".");
			
			String ^ loggerMessage = "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] ";
			loggerMessage += "\n" + "\t" + "Modulation file available (.awf type): ";
			for (int j = 0; j < glob->ModulationFile.totalAwfTypeFileExist; j++)
			{
				loggerMessage += "\n" + "\t" + glob->ModulationFile.awfTypeFileName[j] + ".awf";
			}
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
		}
		else
		{
			glob->ModulationFile.awfTypeFilePathList	= gcnew array<String^>(0);
			glob->ModulationFile.awfTypeFileName		= gcnew array<String^>(0);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.awf type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.awf type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
		}
		
		// Get modulation file list (.wfm)
		glob->ModulationFile.totalWfmTypeFileExist = Directory::GetFiles(glob->ModulationFile.ModulationFileFolderDirectory, "*.wfm")->Length;

		if (glob->ModulationFile.totalWfmTypeFileExist != 0)
		{
			glob->ModulationFile.wfmTypeFilePathList	= gcnew array<String^>(glob->ModulationFile.totalWfmTypeFileExist);
			glob->ModulationFile.wfmTypeFilePathList	= Directory::GetFiles(glob->ModulationFile.ModulationFileFolderDirectory, "*.wfm");

			// Get modulation file name
			glob->ModulationFile.wfmTypeFileName		= gcnew array<String^>(glob->ModulationFile.totalWfmTypeFileExist);

			array<String ^> ^ arrStr					= gcnew array<String ^>(0);
			array<String ^> ^ separator					= gcnew array<String ^>(1);
			separator[0]								= "\\";

			for (int j = 0; j < glob->ModulationFile.totalWfmTypeFileExist; j++)
			{
				arrStr										= glob->ModulationFile.wfmTypeFilePathList[j]->Split(separator, StringSplitOptions::None);
				glob->ModulationFile.wfmTypeFileName[j]		= arrStr[8];
				glob->ModulationFile.wfmTypeFileName[j]		= glob->ModulationFile.wfmTypeFileName[j]->Replace(".wfm", "");
			}

			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.wfm type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->ModulationFile.totalWfmTypeFileExist.ToString() + ".");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.wfm type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->ModulationFile.totalWfmTypeFileExist.ToString() + ".");
			
			String ^ loggerMessage = "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] ";
			loggerMessage += "\n" + "\t" + "Modulation file available (.wfm type): ";
			for (int j = 0; j < glob->ModulationFile.totalWfmTypeFileExist; j++)
			{
				loggerMessage += "\n" + "\t" + glob->ModulationFile.wfmTypeFileName[j] + ".wfm";
			}
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
		}
		else
		{
			glob->ModulationFile.wfmTypeFilePathList	= gcnew array<String^>(0);
			glob->ModulationFile.wfmTypeFileName		= gcnew array<String^>(0);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total Modulation file (.wfm type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder -> LoadModulationFile] Total modulation file (.wfm type) exist in the '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
		}
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	VectorFile Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::LoadVectorFile(int tfSite)
	{
		/*****************************************************************************************************
		** LoadVectorFile
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to load the vector file list (.vec type). 
		******************************************************************************************************/

		// Local variable
		int ret	= 0;

		// Get vector file list (.vec)
		glob->VectorFile.totalVecFileExist = Directory::GetFiles(glob->VectorFile.VectorFileFolderDirectory, "*.vec")->Length;
		
		if (glob->VectorFile.totalVecFileExist != 0)
		{ 
			glob->VectorFile.vecFilePathList	= gcnew array<String^>(glob->VectorFile.totalVecFileExist);
			glob->VectorFile.vecFilePathList	= Directory::GetFiles(glob->VectorFile.VectorFileFolderDirectory, "*.vec");

			// Get vector file name & vector file number
			glob->VectorFile.vecFileName		= gcnew array<String^>(glob->VectorFile.totalVecFileExist);
			glob->VectorFile.vecFileNumber		= gcnew array<int>(glob->VectorFile.totalVecFileExist);

			array<String ^> ^ arrStr			= gcnew array<String ^>(0);
			array<String ^> ^ separator			= gcnew array<String ^>(1);
			separator[0]						= "\\";

			for (int j = 0; j < glob->VectorFile.totalVecFileExist; j++)
			{
				arrStr								= glob->VectorFile.vecFilePathList[j]->Split(separator, StringSplitOptions::None);
				glob->VectorFile.vecFileName[j]		= arrStr[8];
				glob->VectorFile.vecFileName[j]		= glob->VectorFile.vecFileName[j]->Replace(".vec", "");
				glob->VectorFile.vecFileNumber[j]	= j;
			}

			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder -> LoadVectorFile] Total vector file exist in the '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->VectorFile.totalVecFileExist.ToString() + ".");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder -> LoadVectorFile] Total vector file exist in the '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->VectorFile.totalVecFileExist.ToString() + ".");
			
			String ^ loggerMessage = "[Load -> InitializeProgram -> InitializeVectorFileFolder -> LoadVectorFile] ";
			loggerMessage += "\n" + "\t" + "Vector file available (.vec type): ";
			for (int j = 0; j < glob->VectorFile.totalVecFileExist; j++)
			{
				loggerMessage += "\n" + "\t" + glob->VectorFile.vecFileName[j] + ".vec";
			}
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
		}
		else
		{
			glob->VectorFile.vecFilePathList	= gcnew array<String^>(0);
			glob->VectorFile.vecFileName		= gcnew array<String^>(0);
			glob->VectorFile.vecFileNumber		= gcnew array<int>(0);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder -> LoadVectorFile] Total vector file exist in the '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder -> LoadVectorFile] Total vector file exist in the '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
		}
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	VectorStateFile Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::LoadVectorStateFile(int tfSite)
	{
		/*****************************************************************************************************
		** LoadVectorStateFile
		**		tfSite - This is techFlow site index.
		**
		** Descriptions:
		**		This is a function to load the vector state file list (.csv type). 
		******************************************************************************************************/

		// Local variable
		int ret	= 0;

		// Get vector file list (.vec)
		glob->VectorStateFile.totalVecStateFileExist = Directory::GetFiles(glob->VectorStateFile.VectorStateFileFolderDirectory, "*.csv")->Length;

		if (glob->VectorStateFile.totalVecStateFileExist != 0)
		{
			glob->VectorStateFile.vecStateFilePathList	= gcnew array<String^>(glob->VectorStateFile.totalVecStateFileExist);
			glob->VectorStateFile.vecStateFilePathList	= Directory::GetFiles(glob->VectorStateFile.VectorStateFileFolderDirectory, "*.csv");
		
			// Get vector state file name
			glob->VectorStateFile.vecStateFileName		= gcnew array<String^>(glob->VectorStateFile.totalVecStateFileExist);
			glob->VectorStateFile.vecStateFileNumber	= gcnew array<int>(0);
		
			array<String ^> ^ arrStr					= gcnew array<String ^>(0);
			array<String ^> ^ separator					= gcnew array<String ^>(1);
			separator[0]								= "\\";

			for (int j = 0; j < glob->VectorStateFile.totalVecStateFileExist; j++)
			{
				arrStr										= glob->VectorStateFile.vecStateFilePathList[j]->Split(separator, StringSplitOptions::None);
				glob->VectorStateFile.vecStateFileName[j]	= arrStr[8];
				glob->VectorStateFile.vecStateFileName[j]	= glob->VectorStateFile.vecStateFileName[j]->Replace(".csv", "");
			}
		
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder -> LoadVectorStateFile] Total vector state file exist in the '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->VectorStateFile.totalVecStateFileExist.ToString() + ".");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder -> LoadVectorStateFile] Total vector state file exist in the '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder: " + glob->VectorStateFile.totalVecStateFileExist.ToString() + ".");
			
			String ^ loggerMessage = "[Load -> InitializeProgram -> InitializeVectorStateFileFolder -> LoadVectorStateFile] ";
			loggerMessage += "\n" + "\t" + "Vector state file available (.csv type): ";
			for (int j = 0; j < glob->VectorStateFile.totalVecStateFileExist; j++)
			{
				loggerMessage += "\n" + "\t" + glob->VectorStateFile.vecStateFileName[j] + ".csv";
			}
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, loggerMessage);
		}
		else
		{
			glob->VectorStateFile.vecStateFilePathList	= gcnew array<String^>(0);
			glob->VectorStateFile.vecStateFileName		= gcnew array<String^>(0);
			glob->VectorStateFile.vecStateFileNumber	= gcnew array<int>(0);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder -> LoadVectorStateFile] Total vector state file exist in the '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder -> LoadVectorStateFile] Total vector state file exist in the '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder: 0.");
		}
	}
	
	/*
	**	----------------------------------------------------------------------------------------------------
	**	AppsCalFile Related
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::LoadAppsCalFile(int tfSite, String ^ fileDirectory)
	{
		/*****************************************************************************************************
		** LoadAppsCalFile
		**		tfSite			- This is techFlow site index.
		**		fileDirectory	- This is AppsCal file directory.
		**
		** Descriptions:
		**		This is a function to load the AppsCal file and its contents.
		******************************************************************************************************/

		// Local variable
		int totalOffsetCount = 0;
		String ^ readLine = String::Empty;
		array<String^> ^ arrStr = gcnew array<String ^>(0);
		array<String^> ^ separator = gcnew array<String ^>(1);
		separator[0] = ",";
		StreamReader ^ reader;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get total apps cal factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				totalOffsetCount++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->AppsCalFile.ChannelIndex = gcnew array<int>(totalOffsetCount);
		glob->AppsCalFile.TargetPout = gcnew array<double>(totalOffsetCount);
		glob->AppsCalFile.offsetValue = gcnew array<array<double>^>(glob->tf.NumberOfTestSites); // UUT count
		glob->AppsCalFile.AppsCalFactor = gcnew Dictionary<String^, double>();
		//array<String^> ^ testParameterKey = gcnew array<String^>(totalOffsetCount);

		for (int i = 0; i < totalOffsetCount; i++)
		{
			glob->AppsCalFile.ChannelIndex[i] = 0;
			glob->AppsCalFile.TargetPout[i] = 0.0;
			//glob->AppsCalFile.testParameter[i] = String::Empty;
			//testParameterKey[i] = String::Empty;
		}

		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			glob->AppsCalFile.offsetValue[i] = gcnew array<double>(totalOffsetCount);
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get existing fixed offset factor
		**	----------------------------------------------------------------------------------------------------
		*/
		reader = gcnew StreamReader(fileDirectory);

		int index = 0;
		while ((readLine = reader->ReadLine()) != nullptr)
		{
			arrStr = readLine->Split(separator, StringSplitOptions::None);

			if (arrStr[0]->StartsWith("//") == false)
			{
				// Channel Index
				Util->StringToInteger(arrStr[0], glob->AppsCalFile.ChannelIndex[index]);
				// Target Pout
				Util->StringToDouble(arrStr[1], glob->AppsCalFile.TargetPout[index]);
				// Test Parameter
				//glob->AppsCalFile.testParameter[index] = arrStr[2];
				//testParameterKey[index] = arrStr[2];

				// Freq
				//Util->StringToDouble(arrStr[2], glob->AppsCalFile.Freq[index]);
				// Actual Pout
				//Util->StringToDouble(arrStr[2], glob->AppsCalFile.ActualPout[index]);
				
				// Cal Factor
				
				// S0
				Util->StringToDouble(arrStr[3], glob->AppsCalFile.offsetValue[0][index]);
				// S1 - S9
				for (int j = 1; j < glob->tf.NumberOfTestSites; j++) {
					Util->StringToDouble(arrStr[3 + j], glob->AppsCalFile.offsetValue[j][index]);
				}

				index++;
			}
		}
		reader->Close();
		reader = nullptr;

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Transfer to global variable
		**	----------------------------------------------------------------------------------------------------
		*/
		String^ key;
		array<double> ^ arrTemp;
		array<double, 2> ^ offsetTemp = gcnew array<double, 2>(glob->tf.NumberOfTestSites, totalOffsetCount);
		for (int i = 0; i < glob->tf.NumberOfTestSites; i++)
		{
			for (int j = 0; j < totalOffsetCount; j++)
			{
				offsetTemp[i, j] = 0.0;
			}
		}

		for (int i = 0; i < totalOffsetCount; i++)
		{
			// S0
			offsetTemp[0, i] = glob->AppsCalFile.offsetValue[0][i];
			// S1 - S9
			for (int j = 1; j < glob->tf.NumberOfTestSites; ++j) {
				offsetTemp[j, i] = glob->AppsCalFile.offsetValue[j][i];
			}

			//arrTemp = gcnew array<double>(glob->tf.NumberOfTestSites);
			for (int j = 0; j < glob->tf.NumberOfTestSites; j++)
			{
				//arrTemp[j] = offsetTemp[j, i];
				key = "Ch" + glob->AppsCalFile.ChannelIndex[i] + "_P" + glob->AppsCalFile.TargetPout[i] + "_Site" + j.ToString();

				if (glob->AppsCalFile.AppsCalFactor->ContainsKey(key))
				{
					glob->AppsCalFile.AppsCalFactor[key] = offsetTemp[j, i];
				}
				else
				{
					glob->AppsCalFile.AppsCalFactor->Add(key, offsetTemp[j, i]);
				}
			}

		}
	}
}


/*----------------------------------------------------------------------
* Revision Log
* &Log: Files.cpp.rca&

*** Version	: v1.0.0.3
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Added LoadAppsCalFile()

*** Version	: v1.0.0.2
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Sync format for FixedOffsetFile with AutoGUTool 2.0.1.5

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support BoardLoss file check, create, load.
* Support device state file and mapping file check.
* Support FixedOffset file check, create, load.
* Support modulation file check, load.
* Support vector file check, load.
* Support vector state file check, load.
----------------------------------------------------------------------*/
