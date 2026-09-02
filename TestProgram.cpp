#include "TestProgram.h"

namespace AMB7300_TestLibrary_REV2P0
{
	// TestProgram Constructor
	TestProgram::TestProgram(void)
	{
		tl			= gcnew TestFunction();
		amb7300tl	= gcnew AMB7300TestLibrary(tl);
	}
	TestProgram::~TestProgram(void)
	{
	}

	// Public Methods
	int TestProgram::Load(Site ^ site)
	{
		/*****************************************************************************************************
		** Load
		**		site - This is techFlow site object.
		** 
		** Descriptions:
		**		This is a function to initialize all the test program related functions.
		**		Initialize all module's session in the system.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = tl->glob->tf.TestSite;

		amb7300tl = gcnew AMB7300TestLibrary(tl);

		// Get current phase name
		tl->glob->tf.CurrentPhase = site->CurrentPhase->Name;

#pragma region "Initialize Program"		

		ret = tl->InitializeProgram(site);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Initialize Tester & Hardware"

		ret = amb7300tl->InitializeTester(site);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Global Error Message"

		G_JumpOnFail = false;
		G_RunTimeErrorMessage = gcnew array<String^>(tl->glob->tf.NumberOfTestSites);
		G_RunTimeError = gcnew array<bool>(tl->glob->tf.NumberOfTestSites);
		G_RunTimeErrorCode = gcnew array<int>(tl->glob->tf.NumberOfTestSites);

#pragma endregion

#pragma region "Committing Result"
		site->CommittingResult += gcnew CommitResult(this, &TestProgram::SaveSnpToBinAfterCommitResults);
#pragma endregion

#pragma region "HighPwrTest - Source Low on Load"
		if (tl->glob->AWV.HighPwrTest_EN == true)
		{
			tl->LoadAppsCalFile(tfSite, tl->glob->AWV.HighPwrTest_AppsCalFile);
			for (int siteIndex = 0; siteIndex < tl->glob->tf.NumberOfTestSites; siteIndex++)
			{
				ret = amb7300tl->HighPwrTest_VNASourceLow(tfSite, siteIndex);
				if (ret != 0) goto EndOfTest;
			}
		}
#pragma endregion

		tl->Util->WaitSecond(100.0 mS);

	EndOfTest:
		return ret;
	}
	int TestProgram::Unload(Site ^ site)
	{
		/*****************************************************************************************************
		** Unload
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This function is used to release and reset all the sessions attributes to default value on hardware, 
		**		turn off all pins and uninitialized the system modules.  
		**		Finally closes the session specified in instrument handle for all the modules,
		**		and the instrument will maintain its last running state.	
		******************************************************************************************************/

		// Local variable
		int ret = 0;

#pragma region "Committing Result"
		site->CommittingResult -= gcnew CommitResult(this, &TestProgram::SaveSnpToBinAfterCommitResults);
#pragma endregion

#pragma region "Swap S parameter"

		if (tl->glob->AWV.EnableSaveSnpData  &&
			tl->glob->AWV.isSwapS2PData == true)
		{
			//WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "开始SNP文件的转换！禁止任何操作！");
			String ^ SNPFilesDirectory = String::Empty;
			String ^ targeted_FilePath = String::Empty;
			String ^ targeted_newFilePath = String::Empty;
			String^ newSNPFileFolder;

			array<String^>^ arr_BinSubFolder;
			String^ BinSubFolder_Name;

			array<String^>^ arr_FailTi_SiteSubFolder;
			String^ FailTi_SiteSubFolder_Name;

			List<String^>^ list_Bin_FailTI_Site_FullPath = gcnew List<String^>();

			int SNPFilesDirectory_SubFolder_Count = 0;
			long SNPFilesTotal = 0;

			SNPFilesDirectory = amb7300tl->saveRecallSetting->touchstoneFolder;//获取SNP保存的文件夹
			if ((Directory::Exists(SNPFilesDirectory)))
			{
				if (tl->glob->AWV.isSaveBinFolder == true)
				{
					arr_BinSubFolder = Directory::GetDirectories(SNPFilesDirectory);//获取分Bin的子文件夹目录路径
					for (int i = 0; i < arr_BinSubFolder->Length; i++)
					{
						arr_FailTi_SiteSubFolder = Directory::GetDirectories(arr_BinSubFolder[i]);

						for each (String^ subFolder in arr_FailTi_SiteSubFolder)
						{
							list_Bin_FailTI_Site_FullPath->Add(subFolder);
							SNPFilesDirectory_SubFolder_Count++;
						}
					}
				}
				else
				{
					SNPFilesDirectory_SubFolder_Count = 1;
				}

				//对各个子文件夹内的SNP文件进行转换
				for (int i = 0; i < SNPFilesDirectory_SubFolder_Count; i++)
				{
					if (tl->glob->AWV.isSaveBinFolder == true)
					{
						targeted_FilePath = list_Bin_FailTI_Site_FullPath[i];

						// Extract BinSubFolder_Name and FailTi_SiteSubFolder_Name for the current path
						BinSubFolder_Name = Path::GetFileName(Path::GetDirectoryName(targeted_FilePath));
						FailTi_SiteSubFolder_Name = Path::GetFileName(targeted_FilePath);

						// Correctly define newSNPFileFolder for each site
						newSNPFileFolder = SNPFilesDirectory + "\\" + "NEW" + "\\" + BinSubFolder_Name + "\\" + FailTi_SiteSubFolder_Name;

						// Create NEW snp folder
						if (!Directory::Exists(newSNPFileFolder))
						{
							Directory::CreateDirectory(newSNPFileFolder);
						}
					}
					else
					{
						targeted_FilePath = SNPFilesDirectory;
						newSNPFileFolder = SNPFilesDirectory + "\\" + "NEW";

						// Create NEW snp folder
						if (!Directory::Exists(newSNPFileFolder))
						{
							Directory::CreateDirectory(newSNPFileFolder);
						}
						
					}

					SNPFilesTotal = Directory::GetFiles(targeted_FilePath, "*.s2p")->Length;//获取分Bin文件夹下的SNP文件个数
																						  //如果SNP个数不大于0，表示分Bin文件夹中没有SNP文件，则不进行处理
					if (SNPFilesTotal > 0)
					{
#pragma region	"get  S2P files"
						array<String^>^ SNPFilePathList = gcnew array<String^>(SNPFilesTotal);
						array<String^>^ SNPFileName = gcnew array<String^>(SNPFilesTotal);
						array<String^>^ arr_spilt_SNPFile = gcnew array<String^>(0);
						array<String^>^ arrSeparator = gcnew array<String^>(1);
						SNPFilePathList = Directory::GetFiles(targeted_FilePath, "*.s2p");//获取全路径的SNP文件名
						arrSeparator[0] = "\\";
						int split_SNPFilePath_count = 0;
						for (int j = 0; j < SNPFilesTotal; j++)
						{
							arr_spilt_SNPFile = SNPFilePathList[j]->Split(arrSeparator, StringSplitOptions::None);//将每一个SNP全路径按照\分离
							split_SNPFilePath_count = arr_spilt_SNPFile->Length;//获取分离的数量
							SNPFileName[j] = arr_spilt_SNPFile[split_SNPFilePath_count - 1];
							SNPFileName[j] = SNPFileName[j]->Replace(".s2p", "");//存放了SNP文件名
						}
#pragma endregion

						String^ readLine = String::Empty;
						StreamReader^ reader;
						array<String^>^ arr_SNPContent = gcnew array<String^>(0);
						array<String^>^ arr_SNPSeparator = gcnew array<String^>(1);
						arr_SNPSeparator[0] = " ";
						int SNPLineCount = 0;//理论SNP文件中的行数应该少于int最大限制，暂定使用int类型
						int SNPLineIndex = 0;

						array<String^>^ SNP_Freq;
						array<String^>^ SNP_S11_dB;
						array<String^>^ SNP_S11_Ang;
						array<String^>^ SNP_S21_dB;
						array<String^>^ SNP_S21_Ang;
						array<String^>^ SNP_S12_dB;
						array<String^>^ SNP_S12_Ang;
						array<String^>^ SNP_S22_dB;
						array<String^>^ SNP_S22_Ang;
						String^ arr_FirstLine = String::Empty;
						String^ arr_SecondLine = String::Empty;
						String^ arr_ThirdLine = String::Empty;
						String^ arr_FourthLine = String::Empty;
						String^ arr_FifthLine = String::Empty;
						String^ arr_SixthLine = String::Empty;
						String^ arr_SeventhLine = String::Empty;
						String^ arr_EighthLine = String::Empty;
						String^ str_Sep = "	";

						for (int j = 0; j < SNPFilesTotal; j++)
						{

#pragma region	"read S2P file"
							String^ arr_FirstLine = String::Empty;
							String^ arr_SecondLine = String::Empty;
							String^ arr_ThirdLine = String::Empty;
							String^ arr_FourthLine = String::Empty;
							String^ arr_FifthLine = String::Empty;
							String^ arr_SixthLine = "! PortZ  Port1:50+j0    Port2:50+j0";
							String^ arr_SeventhLine = "! Above PortZ is port z conversion or system Z0 setting when saving the data.";
							String^ arr_EighthLine = "! When reading, reference impedance value at option line is always used.";
							int headerLineIndex = 0;

							SNPLineCount = 0;
							reader = gcnew StreamReader(SNPFilePathList[j]);//将单个SNP文件放入streamReader中准备处理

							while ((readLine = reader->ReadLine()) != nullptr)
							{
								if (readLine->StartsWith("#"))
								{
									arr_FirstLine = readLine;
								}
								else if (readLine->StartsWith("!"))
								{
									switch (headerLineIndex)
									{
									case 0:
										arr_SecondLine = readLine;
										break;
									case 1:
										arr_ThirdLine = readLine;
										break;
									case 2:
										arr_FourthLine = readLine;
										break;
									case 3:
										arr_FifthLine = readLine;
										break;
									default:
										break;
									}
									headerLineIndex++;
								}
								else
								{
									SNPLineCount++;
								}

							}
							reader->Close();
							reader = nullptr;
							//SNPLineCount = readLine->Length;//获取SNP文件中的总行数
							SNPLineIndex = 0;

							SNP_Freq = gcnew array<String^>(SNPLineCount);
							SNP_S11_dB = gcnew array<String^>(SNPLineCount);
							SNP_S11_Ang = gcnew array<String^>(SNPLineCount);
							SNP_S21_dB = gcnew array<String^>(SNPLineCount);
							SNP_S21_Ang = gcnew array<String^>(SNPLineCount);
							SNP_S12_dB = gcnew array<String^>(SNPLineCount);
							SNP_S12_Ang = gcnew array<String^>(SNPLineCount);
							SNP_S22_dB = gcnew array<String^>(SNPLineCount);
							SNP_S22_Ang = gcnew array<String^>(SNPLineCount);

							for (int ii = 0; ii < SNPLineCount; ii++)
							{
								SNP_Freq[ii] = String::Empty;
								SNP_S11_dB[ii] = String::Empty;
								SNP_S11_Ang[ii] = String::Empty;
								SNP_S21_dB[ii] = String::Empty;
								SNP_S21_Ang[ii] = String::Empty;
								SNP_S12_dB[ii] = String::Empty;
								SNP_S12_Ang[ii] = String::Empty;
								SNP_S22_dB[ii] = String::Empty;
								SNP_S22_Ang[ii] = String::Empty;
							}

							reader = gcnew StreamReader(SNPFilePathList[j]);
							while ((readLine = reader->ReadLine()) != nullptr)
							{
								arr_SNPContent = readLine->Split(arr_SNPSeparator, StringSplitOptions::None);//按照“ ”分隔每一行的string字符

								if (arr_SNPContent[0]->StartsWith("#") == false && arr_SNPContent[0]->StartsWith("!") == false)
								{
									//Util->StringToDouble(arr_SNPContent[0],	SNP_Freq[SNPLineIndex]); //存放freq
									//Util->StringToDouble(arr_SNPContent[1], SNP_S11_dB[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[2], SNP_S11_Ang[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[3], SNP_S21_dB[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[4], SNP_S21_Ang[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[5], SNP_S12_dB[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[6], SNP_S12_Ang[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[7], SNP_S22_dB[SNPLineIndex]);
									//Util->StringToDouble(arr_SNPContent[8], SNP_S22_Ang[SNPLineIndex]);
									SNP_Freq[SNPLineIndex] = arr_SNPContent[0];
									SNP_S11_dB[SNPLineIndex] = arr_SNPContent[1];
									SNP_S11_Ang[SNPLineIndex] = arr_SNPContent[2];
									SNP_S21_dB[SNPLineIndex] = arr_SNPContent[3];
									SNP_S21_Ang[SNPLineIndex] = arr_SNPContent[4];
									SNP_S12_dB[SNPLineIndex] = arr_SNPContent[5];
									SNP_S12_Ang[SNPLineIndex] = arr_SNPContent[6];
									SNP_S22_dB[SNPLineIndex] = arr_SNPContent[7];
									SNP_S22_Ang[SNPLineIndex] = arr_SNPContent[8];
									SNPLineIndex++;

								}
							}
#pragma endregion

#pragma region	"write S2P file"
							// Local variable
							String^ newSNPFilePath = String::Empty;
							String^ newSnpFileName = String::Empty;
							newSnpFileName = SNPFileName[j] + "_New.s2p";

							newSNPFilePath = newSNPFileFolder + "\\" + newSnpFileName;
							StringBuilder ^ strS2PBuilder = gcnew StringBuilder();
							StreamWriter ^ strS2PWritter = gcnew StreamWriter(newSNPFilePath);

							String^ overHeaderLine = arr_SecondLine + "\n" +
								arr_ThirdLine + "\n" +
								arr_FourthLine + "\n" +
								arr_FifthLine + "\n" +
								arr_SixthLine + "\n" +
								arr_SeventhLine + "\n" +
								arr_EighthLine + "\n" +
								arr_FirstLine;
							/*String^ overHeaderLine =	arr_FirstLine	+ "\n" +
							arr_SecondLine	+ "\n" +
							arr_ThirdLine	+ "\n" +
							arr_FourthLine	+ "\n" +
							arr_FifthLine;*/

							strS2PBuilder->AppendLine(overHeaderLine);
							for (int jj = 0; jj < SNPLineIndex; jj++)
							{
								strS2PBuilder->Append(SNP_Freq[jj] + "	" + SNP_S22_dB[jj] + "	" + SNP_S22_Ang[jj] + "	" + SNP_S12_dB[jj] + "	" + SNP_S12_Ang[jj] + "	" + SNP_S21_dB[jj] + "	" + SNP_S21_Ang[jj] + "	" + SNP_S11_dB[jj] + "	" + SNP_S11_Ang[jj]);
								strS2PBuilder->AppendLine();
							}
							strS2PWritter->Write(strS2PBuilder);
							strS2PBuilder = nullptr;
							strS2PWritter->Close();
#pragma endregion
						}


					}

					//WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "第" + j.ToString() + "个SNP文件转换完成！");
				}


			}
		}
		tl->WriteToFileLogger(tl->glob->tf.TestSite, tl->glob->TcrLgr.tracerMainTab, INFO, "所有的SNP文件转换完成！！！");
#pragma endregion

#pragma region "Uninitialize Tester"

		ret = amb7300tl->UninitializeTester(site);
		if (ret != 0) goto EndOfTest;

#pragma endregion

#pragma region "Uninitialize File Logger"

		tl->UninitializeFileLogger();

#pragma endregion
		
#pragma region "Uninitialize Tracer Logger"

		tl->UninitializeTracerLogger();

#pragma endregion

		tl->Util->WaitSecond(100.0 mS);

	EndOfTest:
		return ret;
	}
	int TestProgram::PreProcessing(Site ^ site)
	{
		/*****************************************************************************************************
		** PreProcessing
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to perform all the pre-settings before execute test method.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = tl->glob->tf.TestSite;

		String^ CorrFactorDirectory;

		ret = amb7300tl->PreProcessingTester(site);
		if (ret != 0) goto EndOfTest;

		if (tf_ControlItem_ConditionExist(PreProcessing_CorrFactorDirectory))
		{
			CorrFactorDirectory = (String^)tf_ControlItem_ConditionCast(PreProcessing_CorrFactorDirectory);
		}
		else
		{
			CorrFactorDirectory = tl->glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "\\" + tl->glob->TesterId + "_" + tl->glob->tf.ProjectName + "_CorrFactor_S" + tfSite.ToString() + ".csv";
		}

		tl->LoadFixedOffsetFile(tfSite, CorrFactorDirectory);

	EndOfTest:
		return ret;
	}
	int TestProgram::PostProcessing(Site ^ site)
	{
		/*****************************************************************************************************
		** PostProcessing
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to perform all the post-settings after execute test method.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		ret = amb7300tl->PostProcessingTester(site);
		if (ret != 0) goto EndOfTest;

	EndOfTest:
		return ret;
	}

	void TestProgram::SaveSnpToBinAfterCommitResults(Site^ site)
	{
		int current_site = 0;

		if (tl->glob->AWV.EnableSaveSnpData && tl->glob->AWV.isSaveBinFolder && amb7300tl->vnaDataAnalysisTPC.saveSnpData == true)
		{
			for each(ResultPerDUTCollection ^ resultCollection in site->ResultsByOffset)
			{
				for each (ResultPerDUT ^ dutResult in resultCollection)
				{
					if (dutResult->Active)
					{

#pragma region "Generate touchstoneFolder\\Bin[bin] folder if create_S2PpathByBin_flag not flagged"

						if (!tl->glob->tf.create_S2PpathByBin_flag)
						{
							array<String^>^ touchstoneFolder_HardBin = gcnew array<String^>(tl->glob->tf.HardBinCount);

							for (int i = 0; i < tl->glob->tf.HardBinCount; i++)
							{
								touchstoneFolder_HardBin[i] = amb7300tl->saveRecallSetting->touchstoneFolder + "\\" + "Bin" + tl->glob->tf.str_arrHBin[i];

								// Create S2Ppath > [bin] folder if not exist
								if (!(Directory::Exists(touchstoneFolder_HardBin[i])))
								{
									Directory::CreateDirectory(touchstoneFolder_HardBin[i]);
								}

								// Store S2Ppath_Bin folder path to BinString_by_BinPath Dictionary
								if (tl->glob->tf.BinString_by_BinPath->ContainsKey(tl->glob->tf.str_arrHBin[i]))
								{
									tl->glob->tf.BinString_by_BinPath[tl->glob->tf.str_arrHBin[i]] = touchstoneFolder_HardBin[i];
								}
								else
								{
									tl->glob->tf.BinString_by_BinPath->Add(tl->glob->tf.str_arrHBin[i], touchstoneFolder_HardBin[i]);
								}
							}
							tl->glob->tf.create_S2PpathByBin_flag = true;
						}
#pragma endregion

#pragma region "Move failed S2P file to respective BinFolder"
						array<String^>^ arr_Failed_TiTpName = gcnew array<String^>(0);
						array<String^>^ arrSeparator_F = gcnew array<String^>(1);
						arrSeparator_F[0] = ".";

						int arr_FailedTP_TotalCount = dutResult->FailedTestParameters->Count;

						String^ FailedTi = String::Empty;
						String^ FailedTp = String::Empty;
						int FailTp_Hbin = -9999;
						String^ FailTi_S2PFilename = String::Empty;
						String^ BinPath_FailedTiBySite_Folder = String::Empty;
						String^ temp_S2P_FullFilePath = String::Empty;


						for each (String^ Failed_TiTpName in dutResult->FailedTestParameters)	// Failed_TiTpName = testItem.testParameter
						{

							arr_Failed_TiTpName = Failed_TiTpName->Split(arrSeparator_F, StringSplitOptions::None);

							if (FailedTi != arr_Failed_TiTpName[0])
							{
								FailedTi = arr_Failed_TiTpName[0];

								if (tl->glob->tf.Ti_by_S2PFilename->ContainsKey(FailedTi + "_S" + current_site))
								{
									if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_CMT_SC5090)
									{
										// Get snp file name from Ti_by_S2PFilename Dictionary
										FailTi_S2PFilename = tl->glob->tf.Ti_by_S2PFilename[FailedTi + "_S" + current_site] + ".s2p";
									}
									else if (amb7300tl->sysConfigInfo.moduleConfigurationName == VnaModel_Keysight_M9804A)
									{
										// Get snp file name from Ti_by_S2PFilename Dictionary
										FailTi_S2PFilename = tl->glob->tf.Ti_by_S2PFilename[FailedTi + "_S" + current_site];// +".s2p";
									}

									// Update selected S2P file path
									temp_S2P_FullFilePath = amb7300tl->saveRecallSetting->touchstoneFolder + "\\" + FailTi_S2PFilename;

									// Get HardBin number from TiTpRule_by_HBin Dictionary
									FailTp_Hbin						= tl->glob->tf.TiTpRule_by_HBin[Failed_TiTpName];

									// Create snp folder by HBin and FailedTi
									BinPath_FailedTiBySite_Folder	= tl->glob->tf.BinString_by_BinPath[FailTp_Hbin.ToString()] + "\\" + FailedTi + "_S" + current_site;

									if (!(Directory::Exists(BinPath_FailedTiBySite_Folder)))
									{
										Directory::CreateDirectory(BinPath_FailedTiBySite_Folder);
									}

									// Move respective S2P file to failed HardBin Folder
									File::Move(temp_S2P_FullFilePath, BinPath_FailedTiBySite_Folder + "\\" + FailTi_S2PFilename);
								}
							}
						}
#pragma endregion

#pragma region "Move remaining S2P file to respective Pass HardBin Folder"

						int Pass_SNPFiles_count = 0;
						int split_SNPFilePath_count = 0;
						//int FailTp_count = dutResult->FailedTestParameters->Count;

						array<String^>^ Separator1 = gcnew array<String^>(1);
						Separator1[0] = "\\";
						array<String^>^ Separator2 = gcnew array<String^>(1);
						Separator2[0] = "_";

						String^ S2PFile_ItemName = String::Empty;
						String^ mod_Pass_SNPFilepath_byBin1 = String::Empty;

						Pass_SNPFiles_count = Directory::GetFiles(amb7300tl->saveRecallSetting->touchstoneFolder, "*.s2p")->Length;
						array<String^> ^ arr_Pass_SNPFilePath = gcnew array<String^>(Pass_SNPFiles_count);
						array<String^>^ arr_spilt_SNPFile = gcnew array<String^>(0);
						array<String^>^ SNPFileName = gcnew array<String^>(Pass_SNPFiles_count);

						arr_Pass_SNPFilePath = Directory::GetFiles(amb7300tl->saveRecallSetting->touchstoneFolder, "*.s2p");	// e.g. "C:\\snp\\ProjectName\\LOTID\\WaferID\\abc1234.s2p"

						for (int j = 0; j < Pass_SNPFiles_count; j++)
						{
							arr_spilt_SNPFile = arr_Pass_SNPFilePath[j]->Split(Separator1, StringSplitOptions::None);	// e.g. arr_spilt_SNPFile = {"C:","snp","ProjectName","LOTID","WaferID","abc1234.s2p"}

							split_SNPFilePath_count = arr_spilt_SNPFile->Length;

							SNPFileName[j] = arr_spilt_SNPFile[split_SNPFilePath_count - 1];							//e.g. "abc1234.s2p"

							// Move to Bin1 folder only if Bin1 exist in BinSorter
							if (tl->glob->tf.BinString_by_BinPath->ContainsKey("1"))
							{
								//if (FailTp_count > 0)
								//{
								//	mod_Pass_SNPFilepath_byBin1 = tl->glob->tf.BinString_by_BinPath["1.1"] + "\\" + S2PFile_ItemName;
								//	if (!(Directory::Exists(mod_Pass_SNPFilepath_byBin1)))
								//	{
								//		Directory::CreateDirectory(mod_Pass_SNPFilepath_byBin1);
								//	}
								//	File::Move(arr_Pass_SNPFilePath[j], mod_Pass_SNPFilepath_byBin1 + "\\" + SNPFileName[j]);
								//}
								//else
								{
									File::Move(arr_Pass_SNPFilePath[j], tl->glob->tf.BinString_by_BinPath["1"] + "\\" + SNPFileName[j]);
								}
							}
							else
							{
								// Do nothing because Bin1 is not set in BinSorter
							}
						}
#pragma endregion

					}
				}
				current_site++;
			}
		}
	}
}