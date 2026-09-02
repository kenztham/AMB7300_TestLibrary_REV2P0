/*----------------------------------------------------------------------
Copyright (c) Aemulus Corporation Sdn Bhd
Title:			Globals.cpp
Purpose:		To manage global variables and functions (standard)
Version:		v1.0.0.6
----------------------------------------------------------------------*/


#include "TestFunction.h"  

namespace Functions
{
	/*
	**	----------------------------------------------------------------------------------------------------
	**	Initiliaze Program
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeProgram(Site ^ site)
	{
		/*****************************************************************************************************
		** InitializeProgram
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize all the program and global property at the load phase.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		int tfSite = glob->tf.TestSite;
		glob->messageBoxIcon = MessageBoxIconFormat::Warning;


#pragma region "Get 'debug' variable at first for file logging purpose"

		if (tf_AppWideVariable_Exist("debug"))
		{
			glob->AWV.Debug = (int)(tf_AppWideVariable_Cast("debug"));
		}
		else
		{
			ret = ER_CONST_AWV_OFFLINE_NOT_FOUND;
			return ret;
		}

		if ((glob->AWV.Debug < 0) || (glob->AWV.Debug > 1))
		{
			ret = ER_CONST_AWV_OFFLINE_INPUT_INVALID;
			return ret;
		}

#pragma endregion

#pragma region "Assembly Resolver"

		// Set True to register a handler for the AssemblyResolve event if the loading of.dll / .exe turns out to be unsucessfull due to different DLL Encryptor used for encryption, else False to by-pass the Assembly Resolve via current domain event handler.
		if ((bool)tf_Flow_ConditionExist("AssemblyResolver"))
		{
			glob->FLOWVAR.AssemblyResolver = (bool)tf_Flow_ConditionCast("AssemblyResolver");
		}
		else
		{
			glob->FLOWVAR.AssemblyResolver = false;
		}

		// Perform assembly resolve if the loading of.dll / .exe turns out to be unsucessfull 
		if (glob->FLOWVAR.AssemblyResolver)
		{
			currentDomain = AppDomain::CurrentDomain;
			this->currentDomain->AssemblyResolve += gcnew System::ResolveEventHandler(this, &TestFunction::currentDomain_AssemblyResolve);
		}

#pragma endregion

		// Get techFlow site property
		GetTechFlowSiteProperty(site);

		// Get techFlow project type
		GetTechFlowProjectType(site);

		// Get project's folder directory
		GetTechFlowFilePathProperty(site);

		// Get techFlow binning property
		GetTechFlowBinningProperty(site);

#pragma region "Setting instantiation site index based on techFlow project type"

		if (glob->tf.StageCount == 1)
		{
			if ((glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) ||
				(glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
			{
				glob->tf.NumberOfTestSites = 1;
				glob->tf.arr_activeUUT = gcnew array<bool>(1);
			}
			else if ((glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) ||
				(glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset)) || (glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA)))
			{
				glob->tf.NumberOfTestSites = glob->tf.TotalUUTOffsets;
				glob->tf.arr_activeUUT = gcnew array<bool>(glob->tf.TotalUUTOffsets);
			}
		}
		else
		{
			// [Future Enchancement] To handle instantiation site index for index parallel project.
		}

#pragma endregion

		// Init tracer logger
		ret = InitializeTracerLogger(site, tfSite);
		if (ret != 0) goto EndOfTest;

		// Get tester ID
		GetTesterID(site, tfSite);

		// Init file logger
		ret = InitializeFileLogger(tfSite);
		if (ret != 0) goto EndOfTest;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram] Execute 'Load' phase. Initialize program.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram] Execute 'Load' phase. Initialize program.");
		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Tester ID | Tester Name | Station Name: " + glob->TesterId + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Tester ID | Tester Name | Station Name: " + glob->TesterId + ".");

		// Get App-Wide-Variable
		ret = GetTechFlowAppsWideVariable(site, tfSite);
		if (ret != 0) goto EndOfTest;

		// C --> Aemlus --> [Init Related Variables]
		InitializeDebugFolder(tfSite);
		InitializeTesterInfoFolder(tfSite);
		InitializeWolferFolder(tfSite);

		// C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> [Init Related Variables]
		ret = InitializeBoardLossFileFolder(tfSite);
		if (ret != 0) goto EndOfTest;
		ret = InitializeDeviceStateFileTemplateFolder(tfSite);
		if (ret != 0) goto EndOfTest;
		ret = InitializeFixedOffsetFileFolder(site, tfSite);
		if (ret != 0) goto EndOfTest;
		ret = InitializeModulationFileFolder(tfSite);
		if (ret != 0) goto EndOfTest;
		ret = InitializeVectorFileFolder(tfSite);
		if (ret != 0) goto EndOfTest;
		ret = InitializeVectorStateFileFolder(tfSite);
		if (ret != 0) goto EndOfTest;


		// Get resource management property (AEM DC Module)
		ret = InitializeResourceManagerProperty(site, tfSite);
		if (ret != 0) goto EndOfTest;

		// Init general global variables
		InitializeGlobalVariables(tfSite);

		//#if SMU_ENABLE
		//		glob->smu = gcnew array<SMU ^>(glob->tf.NumberOfTestSites);
		//#endif 
		//
		//#if DM_ENABLE 
		//		glob->dm = gcnew array<DM ^>(glob->tf.NumberOfTestSites);
		//#endif 
		//
		//#if CM_ENABLE 
		////		glob->cm = gcnew array<CM ^>(glob->tf.NumberOfTestSites);
		//#endif 
		//
		//#if DIO_ENABLE 
		////		glob->iom = gcnew array<DIO ^>(glob->tf.NumberOfTestSites);
		//#endif 
		//
		//#if ACM_ENABLE 
		////		glob->acm = gcnew array<ACM ^>(glob->tf.NumberOfTestSites);
		//#endif 
		//
		//#if TMU_ENABLE 
		////		glob->tm = gcnew array<TM ^>(glob->tf.NumberOfTestSites);
		//#endif 

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram] Execute 'Load' phase. Initialize program succeed !!!");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram] Execute 'Load' phase. Initialize program succeed !!!");

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	techFlow Property | techFlow Project Type | techFlow File/Folder Directory
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::GetTechFlowSiteProperty(Site ^ site)
	{
		/*****************************************************************************************************
		** GetTechFlowSiteProperty
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to get techFlow site properties.
		******************************************************************************************************/

		glob->tf.StageCount = 1;// = site->FlowEngine->SiteGroupManager->SiteStages->StageCount;	// Get current tF stage count				| eg: 1 = single/parallel project, 2 = index parallel project
		glob->tf.TestHead = site->FlowEngine->HeadNumber;									// Get current tF head number				| eg: H0 / H1 / ...
		glob->tf.TestSite = site->Index;													// Get current tF running site				| eg: 0 = site0, 1 = site1, ...		
		glob->tf.TotalUUTOffsets = site->UUTOffsetResolver->UUTOffsets->Count;					// Get total uut count in a techFlow site	| eg: 1 = single uut offset, 2 = 2uut ofset, ...
		glob->tf.TotalTestSite = site->FlowEngine->Sites->Count;								// Get total site count in techFlow			| eg: 1 = single site S0, 2 = parallel site S0 S1 or index parallel

																								/*
																								** tf.TestSite = Current tf site index.
																								** If tF running S0, the site index is 0 | If tF running S1, the site index is 1
																								** Unable to use this as the termination parameter in a FOR loop
																								** Need to use another variable as the parameter, where it is always set to "1" in the program, so that each tF S0/S1/S2... able to run the program's FOR loop.
																								*/

		glob->tf.NumberOfTestSites = 1; // Default value
	}
	void TestFunction::GetTechFlowProjectType(Site ^ site)
	{
		/*****************************************************************************************************
		** GetTechFlowProjectType
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to auto detect techFlow3 project type.
		**		- single site single uut offset
		**		- single site multi uut offset
		**		- single tf site multi uut offset with shared VNA
		**		- true parallel single uut offset
		**		- true parallel multi uut offset
		**		- index parallel
		******************************************************************************************************/

#pragma region "Get 'SharedVNA' variable"

		if (tf_AppWideVariable_Exist("SharedVNA"))
		{
			glob->AWV.SharedVNA = (bool)(tf_AppWideVariable_Cast("SharedVNA"));
			glob->tf.SharedVNA = glob->AWV.SharedVNA;
		}
		else
		{
			glob->AWV.SharedVNA = false;
			glob->tf.SharedVNA = glob->AWV.SharedVNA;
		}

#pragma endregion

		// Single or True-parallel site tF project
		if (glob->tf.StageCount == 1)
		{
			// Single site
			if (glob->tf.TotalTestSite == 1)
			{
				// Single UUT offset
				if (glob->tf.TotalUUTOffsets == 1)
				{
					glob->tf.ProjectType = int(ProjectType::SingleTFSiteSingleUUTOffset);
				}
				// Multi UUT offset
				else
				{
					if (glob->tf.SharedVNA)
					{
						glob->tf.ProjectType = int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA);
					}
					else
					{
						glob->tf.ProjectType = int(ProjectType::SingleTFSiteMultiUUTOffset);
					}
				}
			}
			// True-parallel site
			else
			{
				// Single UUT offset
				if (glob->tf.TotalUUTOffsets == 1)
				{
					glob->tf.ProjectType = int(ProjectType::TrueParallelSingleUUTOffset);
				}
				// Multi UUT offset
				else
				{
					glob->tf.ProjectType = int(ProjectType::TrueParallelMultiUUTOffset);
				}
			}
		}
		// Index-Parallel project
		else
		{	// [Future Enchancement] reserved for index parallel project.
		}
	}
	void TestFunction::GetTechFlowFilePathProperty(Site ^ site)
	{
		/*****************************************************************************************************
		** GetTechFlowFilePathProperty
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to get the project related file & folder name and directory.
		******************************************************************************************************/

		// Get Test Project's recipe path 
		glob->tf.RecipeFilePathDirectory = System::IO::Path::GetDirectoryName(site->Recipe->FlowFilePath);
		glob->tf.RecipeFileName = Path::GetFileName(glob->tf.RecipeFilePathDirectory);

		// Get Profile Name
		array<String^> ^ ArrString;
		array<Char>^ SpliterFormat = { '\\' };
		ArrString = glob->tf.RecipeFilePathDirectory->Split(SpliterFormat, System::StringSplitOptions::None);
		glob->tf.ProfileName = ArrString[5];

		// Get Project Name
		glob->tf.ProjectName = Path::GetFileName(glob->tf.RecipeFilePathDirectory);

		// Get Device Name
		glob->tf.DeviceName = site->DBPath;
		ArrString = glob->tf.DeviceName->Split(SpliterFormat, System::StringSplitOptions::None);
		glob->tf.DeviceName = ArrString[5];

		// Get Device Revision
		glob->tf.DeviceRevision = "";		// [Future Enchancement] To get the correct device revision. For now setting default empty.

											// Get Program Name						
		glob->tf.ProgramName = "";		// [Future Enchancement] To get the correct program name. For now setting default empty.

										// Get Program Revision
		glob->tf.ProgramRevision = "";		// [Future Enchancement] To get the correct program revision. For now setting default empty.

											// Get Test Project's VSProject path 
		glob->tf.HandlerPathDirectory = FOLDER_CONST_AEM_TF3_PROJECTS_HANDLERS;																										// C:\Aemulus\techFlow3\Projects\Handlers
		glob->tf.PSRecipePathDirectory = FOLDER_CONST_AEM_TF3_PROJECTS_PSRECIPES + "\\" + glob->tf.ProfileName + "\\" + glob->tf.ProjectName;									// C:\Aemulus\techFlow3\Projects\PSRecipes\'Profile'\'ProjectName'
		glob->tf.TestProgramPathDirectory = FOLDER_CONST_AEM_TF3_PROJECTS_TESTPROGRAMS + "\\" + glob->tf.ProfileName + "\\" + glob->tf.ProjectName;									// C:\Aemulus\techFlow3\Projects\TestPrograms\'Profile'\'ProjectName'
		glob->tf.RecipeFilePathDirectory = FOLDER_CONST_AEM_TF3_PROJECTS_TESTRECIPES + "\\" + glob->tf.ProfileName + "\\" + glob->tf.ProjectName;									// C:\Aemulus\techFlow3\Projects\TestRecipes\'Profile'\'ProjectName'
		glob->tf.VSProjectPathDirectory = FOLDER_CONST_AEM_TF3_PROJECTS_VSPROJECTS + "\\" + glob->tf.ProfileName + "\\" + glob->tf.ProjectName + "\\" + glob->tf.ProjectName;	// C:\Aemulus\techFlow3\Projects\VSProjects\'Profile'\'ProjectName'\'ProjectName'
	}
	void TestFunction::GetTechFlowBinningProperty(Site ^ site)
	{
		/*****************************************************************************************************
		** GetTechFlowBinningProperty
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to get techFlow binning rules, hard & soft bin info.
		******************************************************************************************************/

		glob->tf.SoftBinCount = site->SoftBins->Count;				// Get total soft bin count      
		glob->tf.arr_HBin = gcnew array<int>(glob->tf.SoftBinCount);						// Initiallize int array to store hard bin value
		glob->tf.TiTpRule_by_HBin = gcnew Dictionary<String^, int>();			// Initiallize dictionary
		glob->tf.Ti_by_S2PFilename = gcnew Dictionary<String^, String^>();		// Initiallize dictionary
		glob->tf.BinString_by_BinPath = gcnew Dictionary<String^, String^>();	// Initiallize dictionary
		System::Collections::Generic::HashSet<int> HashSet_HardBin = gcnew System::Collections::Generic::HashSet<int>();	// Use HashSet to remove duplicate HardBin element

		int arr_HBin_Index = 0;
		int str_arrHBin_Index = 0;

		for each (SoftBin^ SoftBin_object in site->SoftBins)
		{
			glob->tf.arr_HBin[arr_HBin_Index++] = SoftBin_object->HardBin;
		}


		for each (int value in glob->tf.arr_HBin)
		{
			HashSet_HardBin.Add(value);
		}

		// Convert int value in HashSet_HardBin into String^
		glob->tf.HardBinCount = HashSet_HardBin.Count;					// Get total hard bin count  
		glob->tf.str_arrHBin = gcnew array<String^>(glob->tf.HardBinCount); // Initiallize String array to store hard bin value

		for each (int value in HashSet_HardBin)
		{
			glob->tf.str_arrHBin[str_arrHBin_Index++] = value.ToString();
		}

		// Store bin rules into TP_Hbin Dictionary
		for each(BinSorterRule ^ rules in site->BinSorter->Rules)
		{
			if (rules->Status != Aemulus::Tech::Flow::TestParameterPassFail::Pass)
			{
				for each(BinSorterRuleSetting ^ setting in rules->Settings)
				{
					glob->tf.TiTpRule_by_HBin->Add(setting->TestParameter->FullDisplayName, rules->SoftBin->HardBin);
				}
			}
		}
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Tester ID
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::GetTesterID(Site ^ site, int tfSite)
	{
		/*****************************************************************************************************
		** GetTesterID
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to get Tester ID | Tester Name | Station Name.
		******************************************************************************************************/

		// Local variable
		//int tfSite = glob->tf.TestSite;

		//WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Load tester ID.");
		//WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Load tester ID.");

		// Tester ID | Tester Name | Station Name
		glob->TesterId = site->FlowEngine->StationName;

		//WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Tester ID | Tester Name | Station Name: " + glob->TesterId + ".");
		//WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTesterId] Tester ID | Tester Name | Station Name: " + glob->TesterId + ".");
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	App-Wide-Variable
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::GetTechFlowAppsWideVariable(Site ^ site, int tfSite)
	{
		/*****************************************************************************************************
		** GetTechFlowAppsWideVariable
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to load the generic App-Wide-Variables from techFlow.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] Load App-Wide-Variables.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] Load App-Wide-Variables.");

#pragma region "debug"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'debug' App Wide Variables
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_debug))
		{
			glob->AWV.Debug = (int)(tf_AppWideVariable_Cast(AppWideVariableName_debug));
		}
		else
		{
			ret = ER_CONST_AWV_DEBUG_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.Debug < 0) || (glob->AWV.Debug > 1))
		{
			ret = ER_CONST_AWV_DEBUG_INPUT_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' = " + glob->AWV.Debug.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_debug + "' = " + glob->AWV.Debug.ToString());
		}
#pragma endregion

#pragma region "offline"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'offline' App Wide Variables
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_offline))
		{
			glob->AWV.Offline = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_offline));
		}
		else
		{
			ret = ER_CONST_AWV_OFFLINE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.Offline != true) && (glob->AWV.Offline != false))
		{
			ret = ER_CONST_AWV_OFFLINE_INPUT_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' = " + glob->AWV.Offline.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_offline + "' = " + glob->AWV.Offline.ToString());
		}
#pragma endregion

#pragma region "PowerLineFrequency"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'PowerLineFrequency' App Wide Variables
		**	Thailand = 50Hz
		**	United States of America = 60Hz
		**	Malaysia = 50Hz
		**	China = 50Hz
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_PowerLineFrequency))
		{
			// AWV: 0 = 50Hz | 1 = 60Hz
			int PowerLineFreqGet = 0;
			PowerLineFreqGet = (int)(tf_AppWideVariable_Cast(AppWideVariableName_PowerLineFrequency));

			if (PowerLineFreqGet == 0)
				glob->AWV.PowerLineFreq = 50.0 Hz;
			else if (PowerLineFreqGet == 1)
				glob->AWV.PowerLineFreq = 60.0 Hz;
		}
		else
		{
			ret = ER_CONST_AWV_PLF_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.PowerLineFreq != 50.0 Hz) && (glob->AWV.PowerLineFreq != 60.0 Hz))
		{
			ret = ER_CONST_AWV_PLF_INPUT_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' = " + glob->AWV.PowerLineFreq.ToString() + "Hz");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_PowerLineFrequency + "' = " + glob->AWV.PowerLineFreq.ToString() + "Hz");
		}
#pragma endregion

#pragma region "gCreateFixedOffsetFile"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'gCreateFixedOffsetFile' App Wide Variables
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_gCreateFixedOffsetFile))
		{
			glob->AWV.CreateFixedOffsetFile = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_gCreateFixedOffsetFile));
		}
		else
		{
			ret = ER_CONST_AWV_CREATE_FIXED_OFFSET_FILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.CreateFixedOffsetFile != true) && (glob->AWV.CreateFixedOffsetFile != false))
		{
			ret = ER_CONST_AWV_CREATE_FIXED_OFFSET_FILE_INPUT_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_gCreateFixedOffsetFile + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
		}
#pragma endregion

#pragma region "SaveSnpData_Format"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'SaveSSaveSnpData_FormatnpData' App Wide Variables
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_SaveSnpData))
		{
			glob->AWV.SaveSnpData = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_SaveSnpData));
		}
		else
		{
			ret = ER_CONST_AWV_SaveSnpData_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveSnpData + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveSnpData + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		array <String^>^ arrStr = gcnew array<String^>(0);
		array<String^>^ separator = gcnew array<String^>(1);
		separator[0] = "/";
		arrStr = glob->AWV.SaveSnpData->Split(separator, StringSplitOptions::None);

		// Validate AWV value input --> 'SaveToTouchstoneFile'
		// SaveSnpData parameter: SaveToTouchstoneFileEnable (SaveSnpDataOn | SaveSnpDataOff), TouchstoneSNPFormat (Re_Imag | Mag_Angle | dB_Angle)
		if ((arrStr[0] != Cond_SaveSnpData_Enable_SaveSnpDataOn) &&
			(arrStr[0] != Cond_SaveSnpData_Enable_SaveSnpDataOff))
		{
			ret = ER_CONST_AWV_SaveSnpData_INVALID;
			String ^ additionalMessage = "'SaveSnpData' 1st parameter available settings: " + "\n" +
				"-> SaveSnpDataOn" + "\n" +
				"-> SaveSnpDataOff";
			MessageBox::Show(additionalMessage, TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE, MessageBoxButtons::OK, MessageBoxIcon::Warning);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['VnaFetch' condition value verification] 'SaveSnpData' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['VnaFetch' condition value verification] 'SaveSnpData' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		if ((arrStr[1] != Cond_SaveSnpData_SNPFormat_ReImag) &&
			(arrStr[1] != Cond_SaveSnpData_SNPFormat_MagAngle) &&
			(arrStr[1] != Cond_SaveSnpData_SNPFormat_dBAngle))
		{
			ret = ER_CONST_AWV_SaveSnpData_INVALID;
			String ^ additionalMessage = "'SaveSnpData' 2nd parameter available settings: " + "\n" +
				"-> Re_Imag" + "\n" +
				"-> Mag_Angle" + "\n" +
				"-> dB_Angle";
			MessageBox::Show(additionalMessage, TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE, MessageBoxButtons::OK, MessageBoxIcon::Warning);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['AppWideVariable' condition value verification] 'SaveSnpData' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['AppWideVariable' condition value verification] 'SaveSnpData' condition value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		if (arrStr->Length != 2)
		{
			ret = ER_CONST_AWV_SaveSnpData_INVALID;
			String ^ additionalMessage = "'SaveSnpData' contain 2x parameter which is differentiate by '/'." + "\n" +
				"1st parameter: Enable save data to touchstone file." + "\n" +
				"2nd parameter: Select touchstone SNP format." + "\n" +
				"Example: SaveSnpDataOn/dB_Angle --> meaning enable save data to touchstone file (.snp), with dB_Angle SNP format.";
			MessageBox::Show(additionalMessage, TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE, MessageBoxButtons::OK, MessageBoxIcon::Warning);
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['AppWideVariable' condition value verification] 'SaveSnpData' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "['AppWideVariable' condition value verification] 'SaveSnpData' condition value invalid. It should contain 2x value parameter, which is differentiate by a '/' symbol." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else if (arrStr->Length == 2)
		{
			if (arrStr[0] == Cond_SaveSnpData_Enable_SaveSnpDataOn)
				glob->AWV.EnableSaveSnpData = true;
			else if (arrStr[0] == Cond_SaveSnpData_Enable_SaveSnpDataOff)
				glob->AWV.EnableSaveSnpData = false;

			if (arrStr[1] == Cond_SaveSnpData_SNPFormat_ReImag)
				glob->AWV.touchstoneFileDataFormat = int(SnPFormat::SNP_RI_FORMAT);
			else if (arrStr[1] == Cond_SaveSnpData_SNPFormat_MagAngle)
				glob->AWV.touchstoneFileDataFormat = int(SnPFormat::SNP_MA_FORMAT);
			else if (arrStr[1] == Cond_SaveSnpData_SNPFormat_dBAngle)
				glob->AWV.touchstoneFileDataFormat = int(SnPFormat::SNP_DB_FORMAT);
		}

#pragma endregion

#pragma region "Get 'S2P' pathway"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'S2Ppath' App Wide Variables
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_S2Ppath))
		{
			glob->AWV.S2Ppath = site->FlowEngine->AppWideVariables["S2Ppath"]->ToString();

			////create SaveSnpFilePath
			//if (!(Directory::Exists(glob->AWV.S2Ppath)) && !(glob->AWV.S2Ppath == String::Empty))
			//{
			//	Directory::CreateDirectory(glob->AWV.S2Ppath);
			//}

		}
		else
		{
			ret = ER_CONST_AWV_S2Ppath_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[AppWideVariables dont have the S2Ppath variable. Please check accordingly.");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[AppWideVariables dont have the S2Ppath variable. Please check accordingly.");
			return ret;
		}
#pragma endregion

#pragma region "S2Prename_EN"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'S2Prename_EN' App Wide Variables
		**	-use to define if custom snp file name is used
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_S2P_rename))
		{
			glob->AWV.EnableRenameSnpData = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_S2P_rename));
		}
		else
		{
			ret = ER_CONST_AWV_S2P_rename_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input 'UseGenericStateFile'
		if ((glob->AWV.EnableRenameSnpData != true) && (glob->AWV.EnableRenameSnpData != false))
		{
			ret = ER_CONST_AWV_S2P_rename_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' = " + glob->AWV.EnableRenameSnpData.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_S2P_rename + "' = " + glob->AWV.EnableRenameSnpData.ToString());
		}
#pragma endregion

#pragma region "S2Prename_Name"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'S2Prename_Name' App Wide Variables
		**	-use to define customized snp file name
		**	If 'S2P_rename' TRUE -> use defined file name specified by 'snpFileName', support Macros
		**  Elseif 'S2P_rename' FALSE -> use generic file name
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_snpFileName))
		{
			glob->AWV.snpFileName = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_snpFileName));
		}
		else
		{
			ret = ER_CONST_AWV_snpFileName_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_snpFileName + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_snpFileName + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "SwapS2PData"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'isSwapS2PData' App Wide Variables
		**	-use to manually swap data for Port1 and Port2 within a S2P.
		**	-Usual case Port17 and Port18 as output; Port1~16 as input.
		**	If isSwapS2PData TRUE -> Port17 and Port18 as Input; Port1~16 as Output.
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_SwapS2PData))
		{
			glob->AWV.isSwapS2PData = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_SwapS2PData));
		}
		else
		{
			ret = ER_CONST_AWV_SwapS2PData_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.isSwapS2PData != true) && (glob->AWV.isSwapS2PData != false))
		{
			ret = ER_CONST_AWV_SwapS2PData_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SwapS2PData + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
		}
#pragma endregion

#pragma region "SaveBinFolder"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'isSaveBinFolder' App Wide Variables
		**	-Use binning [Hard Bin] in the folder pathway.
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_SaveBinFolder))
		{
			glob->AWV.isSaveBinFolder = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_SaveBinFolder));
		}
		else
		{
			ret = ER_CONST_AWV_SaveBinFolder_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input
		if ((glob->AWV.isSaveBinFolder != true) && (glob->AWV.isSaveBinFolder != false))
		{
			ret = ER_CONST_AWV_SaveBinFolder_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' = " + glob->AWV.CreateFixedOffsetFile.ToString());
		}

		// Cross check with EnableSaveSnpData
		if ((glob->AWV.EnableSaveSnpData == false) && (glob->AWV.isSaveBinFolder == true))
		{
			String ^ additionalMessage = "'SaveSnpData' is Off but 'SaveBinFolder' is True " + "\n" +
				"Please make sure [snp] settings in AppWideVariable are valid!";
			MessageBox::Show(additionalMessage, TITLE_CONST_INVALID_APPWIDE_VARIABLE_INPUT_VALUE, MessageBoxButtons::OK, MessageBoxIcon::Warning);

			ret = ER_CONST_AWV_SaveBinFolder_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_SaveBinFolder + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "CalibrationValidityDay"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'CalibrationValidityDay' App Wide Variables
		**	-Set a cal validity reminder. If exceed, prompt user to re-cal
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_CalibrationValidityDay))
		{
			glob->AWV.CalibrationValidityDay = (int)(tf_AppWideVariable_Cast(AppWideVariableName_CalibrationValidityDay));
		}
		else
		{
			ret = ER_CONST_AWV_CalibrationValidityDay_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_CalibrationValidityDay + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_CalibrationValidityDay + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "GenericStateMappingFile_EN"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'GenericStateMappingFile_EN' App Wide Variables
		**	-use to define if project is loading generic state file instead of "AMB7300Config_DeviceName"
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_GenericStateMappingFile_EN))
		{
			glob->AWV.GenericStateMappingFile_EN = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_GenericStateMappingFile_EN));
		}
		else
		{
			ret = ER_CONST_AWV_GenericStateMappingFile_EN_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input 'UseGenericStateFile'
		if ((glob->AWV.GenericStateMappingFile_EN != true) && (glob->AWV.GenericStateMappingFile_EN != false))
		{
			ret = ER_CONST_AWV_GenericStateMappingFile_EN_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' = " + glob->AWV.GenericStateMappingFile_EN.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_EN + "' = " + glob->AWV.GenericStateMappingFile_EN.ToString());
		}
#pragma endregion

#pragma region "GenericStateMappingFile_Name"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'GenericStateMappingFile_Name' App Wide Variables
		**	-use to define Generic State Filepath
		**	If 'GenericStateMappingFile_EN' TRUE -> use defined file name specified by 'GenericStateMappingFile_EN'
		**  Elseif 'GenericStateMappingFile_EN' FALSE -> use 'AMB7300Config_DeviceName' state file
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_GenericStateMappingFile_Name))
		{
			glob->AWV.GenericStateMappingFile_Name = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_GenericStateMappingFile_Name));
		}
		else
		{
			ret = ER_CONST_AWV_GenericStateMappingFile_Name_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_Name + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_GenericStateMappingFile_Name + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "HighPwrTest_EN"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'HighPwrTest_EN' App Wide Variables
		**	-use to define if high power test is enabled, 0 = Default | 1 = High power test enable & expected external PA conneected & separate AppsPowerCal done
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_HighPwrTest_EN))
		{
			glob->AWV.HighPwrTest_EN = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_HighPwrTest_EN));
		}
		else
		{
			ret = ER_CONST_AWV_HighPwrTest_EN_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input 'UseGenericStateFile'
		if ((glob->AWV.HighPwrTest_EN != true) && (glob->AWV.HighPwrTest_EN != false))
		{
			ret = ER_CONST_AWV_HighPwrTest_EN_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' = " + glob->AWV.HighPwrTest_EN.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_EN + "' = " + glob->AWV.HighPwrTest_EN.ToString());
		}
#pragma endregion

#pragma region "HighPwrTest_AppsCalFile"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'HighPwrTest_AppsCalFile' App Wide Variables
		**	-use to define AppsCalFile path if 'HighPwrTest_EN' = TRUE
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_HighPwrTest_AppsCalFile))
		{
			glob->AWV.HighPwrTest_AppsCalFile = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_HighPwrTest_AppsCalFile));
		}
		else
		{
			ret = ER_CONST_AWV_HighPwrTest_AppsCalFile_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_AppsCalFile + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_HighPwrTest_AppsCalFile + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
#pragma endregion

#pragma region "VNA_Mutex_EN"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'VNA_Mutex_EN' App Wide Variables
		**	-use to define if Mutex feature is enabled, 0 = Off | 1 = Enable enable ping-pong testing, use this for index parallel testing when VNA resource is shared for multiple sites
		**	----------------------------------------------------------------------------------------------------
		*/
		if (tf_AppWideVariable_Exist(AppWideVariableName_VNA_Mutex_EN))
		{
			glob->AWV.VNA_Mutex_EN = (bool)(tf_AppWideVariable_Cast(AppWideVariableName_VNA_Mutex_EN));
		}
		else
		{
			ret = ER_CONST_AWV_VNA_Mutex_EN_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' variable name not found." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// Check user input 'UseGenericStateFile'
		if ((glob->AWV.VNA_Mutex_EN != true) && (glob->AWV.VNA_Mutex_EN != false))
		{
			ret = ER_CONST_AWV_VNA_Mutex_EN_INVALID;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, WARNING, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' variable value invalid." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}
		else
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' = " + glob->AWV.VNA_Mutex_EN.ToString());
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> GetTechFlowAppsWideVariable] App-Wide-Variable '" + AppWideVariableName_VNA_Mutex_EN + "' = " + glob->AWV.VNA_Mutex_EN.ToString());
		}
#pragma endregion

#pragma region "PortMatchingX"
		/*
		**	----------------------------------------------------------------------------------------------------
		**	'PortMatchingX' App Wide Variables
		**	-use to define snp filepath of matching circuit for Port 'X'
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->AWV.PortMatching_EN = gcnew array<bool>(6);

		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching1))
		{
			glob->AWV.PortMatching1 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching1));
			if (glob->AWV.PortMatching1->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[0] = true;
			}
		}
		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching2))
		{
			glob->AWV.PortMatching2 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching2));
			if (glob->AWV.PortMatching2->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[1] = true;
			}
		}
		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching3))
		{
			glob->AWV.PortMatching3 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching3));
			if (glob->AWV.PortMatching3->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[2] = true;
			}
		}
		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching4))
		{
			glob->AWV.PortMatching4 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching4));
			if (glob->AWV.PortMatching4->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[3] = true;
			}
		}
		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching5))
		{
			glob->AWV.PortMatching5 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching5));
			if (glob->AWV.PortMatching5->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[4] = true;
			}
		}
		if (tf_AppWideVariable_Exist(AppWideVariableName_PortMatching6))
		{
			glob->AWV.PortMatching6 = (String^)(tf_AppWideVariable_Cast(AppWideVariableName_PortMatching6));
			if (glob->AWV.PortMatching6->Trim() != String::Empty)
			{
				glob->AWV.PortMatching_EN[5] = true;
			}
		}
#pragma endregion
		EndOfTest:
				 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemlus --> [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::InitializeDebugFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeDebugFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'Debug' folder.
		**		Target directory: C:\Aemulus\Debug
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDebugFolder] Initialize '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_DEBUG + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDebugFolder] Initialize '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_DEBUG + ".");

		// Check 'Debug' folder existence
		if (!Directory::Exists(FOLDER_CONST_AEM_DEBUG))
		{
			ret = ER_CONST_AEM_DEBUG_FOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDebugFolder] '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDebugFolder] '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_DEBUG_FOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'Debug' folder
			Directory::CreateDirectory(FOLDER_CONST_AEM_DEBUG);
		}

		// [Future Enchancement] Reserved for future development or tester platform combination.
	}
	void TestFunction::InitializeTesterInfoFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeTesterInfoFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'TesterInfo' folder.
		**		Target directory: C:\Aemulus\TesterInfo
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeTesterInfoFolder] Initialize '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_TESTERINFO + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeTesterInfoFolder] Initialize '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_TESTERINFO + ".");

		// Check 'TesterInfo' folder existence
		if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO))
		{
			ret = ER_CONST_AEM_TESTERINFO_FOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeTesterInfoFolder] '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder and sub-folders will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeTesterInfoFolder] '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_TESTERINFO_FOLDER + "' folder and sub-folders will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'TesterInfo' folder
			Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO);

			// Create related sub-folders
			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_APPSCAL))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_APPSCAL);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_APPSCAL_ARCHIVED))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_APPSCAL_ARCHIVED + "\\" + glob->tf.ProfileName);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_BOARDLOSS))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_BOARDLOSS);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_CORRFACTOR))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_CORRFACTOR);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_CORRFACTOR_ARCHIVED))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_CORRFACTOR_ARCHIVED + "\\" + glob->tf.ProfileName);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_GUCAL))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_GUCAL + "\\" + glob->tf.ProfileName);

			if (!Directory::Exists(FOLDER_CONST_AEM_TESTERINFO_KGUDATA))
				Directory::CreateDirectory(FOLDER_CONST_AEM_TESTERINFO_KGUDATA);
		}

		// [Future Enchancement] Reserved for future development or tester platform combination.
	}
	void TestFunction::InitializeWolferFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeWolferFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'Wolfer' folder.
		**		Target directory: C:\Aemulus\Wolfer
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeWolferFolder] Initialize '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_WOLFER + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeWolferFolder] Initialize '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder. Target directory: " + FOLDER_CONST_AEM_WOLFER + ".");

		// Check 'Wolfer' folder existence
		if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER))
		{
			ret = ER_CONST_AEM_WOLFER_FOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeWolferFolder] '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder and sub-folders will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeWolferFolder] '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_AEM_WOLFER_FOLDER + "' folder and sub-folders will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'Wolfer' folder
			Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER);

			// Create related sub-folders
			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_C0))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_C0);

			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_C1U))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_C1U);

			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_C2))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_C2);

			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_SKY01))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_SKY01);

			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_V1))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_V1);

			if (!Directory::Exists(FOLDER_CONST_AEM_WOLFER_GUOBOPA))
				Directory::CreateDirectory(FOLDER_CONST_AEM_WOLFER_GUOBOPA);
		}

		// [Future Enchancement] Reserved for future development or tester platform combination.
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> BoardLossFileFolder [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeBoardLossFileFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeBoardLossFileFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'BoardLossFileFolder' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\BoardLossFileFolder
		**		This function support file/folder checking, file/folder creation, content checking, and factor loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->BoardLoss.isCreateNewBoardLossFile = false;
		glob->BoardLoss.BoardLossFileFolderDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER;
		glob->BoardLoss.BoardLossFileDirectory = glob->BoardLoss.BoardLossFileFolderDirectory + "\\" + glob->TesterId + "_" + FILENAME_CONST_PROJECT_BOARDLOSS + "_" + glob->tf.ProjectName + "_Site" + tfSite.ToString() + ".csv";
		String ^ csvFileName = glob->TesterId + "_" + FILENAME_CONST_PROJECT_BOARDLOSS + "_" + glob->tf.ProjectName + "_Site" + tfSite.ToString() + ".csv";

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Initialize '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder. Target directory: " + glob->BoardLoss.BoardLossFileFolderDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Initialize '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder. Target directory: " + glob->BoardLoss.BoardLossFileFolderDirectory + ".");

		// Check 'BoardLossFileFolder' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->BoardLoss.BoardLossFileFolderDirectory))
		{
			ret = ER_CONST_PROJECT_BOARDLOSSFILEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'BoardLossFileFolder' folder
			Directory::CreateDirectory(glob->BoardLoss.BoardLossFileFolderDirectory);
			glob->BoardLoss.isCreateNewBoardLossFile = true;
			goto CreateNewBoardLossFile;
		}

		// Check 'BoardLossFileFolder' -> 'Archive' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->BoardLoss.BoardLossFileFolderDirectory + "\\Archive"))
		{
			ret = ER_CONST_PROJECT_BOARDLOSSFILEFOLDER_ARCHIVEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + FILENAME_CONST_ARCHIVE + "' folder does not exist in the '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder in the target directory. A new '" + FILENAME_CONST_ARCHIVE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + FILENAME_CONST_ARCHIVE + "' folder does not exist in the '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder in the target directory. A new '" + FILENAME_CONST_ARCHIVE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'BoardLossFileFolder' -> 'Archive' folder
			Directory::CreateDirectory(glob->BoardLoss.BoardLossFileFolderDirectory + "\\Archive");
		}

		// Check 'BoardLoss_TesterID_Sx.csv' file existence in the project 'TestRecipes\BoardLossFileFolder' folder
		if (!File::Exists(glob->BoardLoss.BoardLossFileDirectory))
		{
			ret = ER_CONST_PROJECT_BOARDLOSSFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + csvFileName + "' file does not exist in the '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] '" + csvFileName + "' file does not exist in the '" + FILENAME_CONST_PROJECT_BOARDLOSSFILEFOLDER + "' folder. A new '" + csvFileName + "' file will be generated in the target folder (with empty boardloss factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			glob->BoardLoss.isCreateNewBoardLossFile = true;
			goto CreateNewBoardLossFile;
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Check '" + csvFileName + "' file format and content.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Check '" + csvFileName + "' file format and content.");
		// Check the existing 'BoardLoss_TesterID_Sx.csv' file contents
		CheckExistingBoardLossFileContent(tfSite, glob->BoardLoss.BoardLossFileDirectory, csvFileName);

	CreateNewBoardLossFile:

		// To check whether need to generate a new 'BoardLoss_TesterID_Sx.csv'
		if (glob->BoardLoss.isCreateNewBoardLossFile == true)
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Generate new '" + csvFileName + "' file (with empty boardloss factor).");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Generate new '" + csvFileName + "' file (with empty boardloss factor).");
			// Copy all the existing files into the 'Archive' folder, delete all files
			int temp_FileCount = Directory::GetFiles(glob->BoardLoss.BoardLossFileFolderDirectory, "*.csv")->Length;
			array<String^> ^ temp_FileDirectory = gcnew array<String^>(temp_FileCount);
			temp_FileDirectory = Directory::GetFiles(glob->BoardLoss.BoardLossFileFolderDirectory, "*.csv");
			for (int i = 0; i < temp_FileCount; i++)
			{
				array<String^> ^ temp_ArrStr = gcnew array<String ^>(0);
				array<String^> ^ temp_Separator = gcnew array<String ^>(1);
				temp_Separator[0] = "\\";
				temp_ArrStr = temp_FileDirectory[i]->Split(temp_Separator, StringSplitOptions::None);
				File::Copy(temp_FileDirectory[i], glob->BoardLoss.BoardLossFileFolderDirectory + "\\Archive\\" + temp_ArrStr[8], true);
			}
			for (int i = 0; i < temp_FileCount; i++)
			{
				File::Delete(temp_FileDirectory[i]);
			}
			// Generate 'BoardLoss_TesterID_Sx.csv'
			GenerateBoardLossFile(tfSite, glob->BoardLoss.BoardLossFileDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Load boardloss factor from '" + csvFileName + "' file.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeBoardLossFileFolder] Load boardloss factor from '" + csvFileName + "' file.");
		// Load boardloss factor from 'BoardLoss_TesterID_Sx.csv' file and the key is hardware path name
		glob->BoardLoss.BoardLossFactor = gcnew Dictionary<String^, array<double>^>();
		LoadBoardLossFile(tfSite, glob->BoardLoss.BoardLossFileDirectory);
		glob->BoardLoss.isCreateNewBoardLossFile = false;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> DeviceStateFileTemplate [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeDeviceStateFileTemplateFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeDeviceStateFileTemplateFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'DeviceStateFileTemplate' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\DeviceStateFileTemplate
		**		This function support file/folder checking and file loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] Initialize '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. Target directory: " + glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] Initialize '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder. Target directory: " + glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory + ".");

		// Check 'DeviceStateFileTemplate' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory))
		{
			ret = ER_CONST_PROJECT_DEVICESTATEFILETEMPLATE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder does not exist in the target director. A new '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'DeviceStateFileTemplate' folder
			Directory::CreateDirectory(glob->DeviceStateFileTemplate.DeviceStateFileTemplateDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] Load device state file and mapping file from '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeDeviceStateFileTemplateFolder] Load device state file and mapping file from '" + FILENAME_CONST_PROJECT_DEVICESTATEFILETEMPLATE + "' folder.");
		// Load device state file & mapping file directory from 'DeviceStateFileTemplate' folder
		glob->DeviceStateFileTemplate.isDeviceStateFileTemplateStateFIleInvalid = false;
		glob->DeviceStateFileTemplate.isDeviceStateFileTemplateMappingFileInvalid = false;
		LoadDeviceStateFileTemplate(tfSite);
		// [Info] For the local device state file and mapping file verification and operation, will be handle at InitializeTester()

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> FixedOffsetFileFolder [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeFixedOffsetFileFolder(Site ^ site, int tfSite)
	{
		/*****************************************************************************************************
		** InitializeFixedOffsetFileFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'FixedOffsetFileFolder' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\FixedOffsetFileFolder
		**		This function support file/folder checking, file/folder creation, content checking, and factor loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->FixedOffset.isCreateNewFixedOffsetFile = false;
		glob->FixedOffset.FixedOffsetFileFolderDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER;
		//glob->FixedOffset.FixedOffsetFileDirectory			= glob->FixedOffset.FixedOffsetFileFolderDirectory + "\\" + glob->TesterId + "_" + FILENAME_CONST_PROJECT_FIXEDOFFSET + "_" + glob->tf.ProjectName + "_Site" + tfSite.ToString() + ".csv";
		//String ^ csvFileName								= glob->TesterId + "_" + FILENAME_CONST_PROJECT_FIXEDOFFSET + "_" + glob->tf.ProjectName + "_Site" + tfSite.ToString() + ".csv";
		glob->FixedOffset.FixedOffsetFileDirectory = glob->FixedOffset.FixedOffsetFileFolderDirectory + "\\" + glob->TesterId + "_" + glob->tf.ProjectName + "_CorrFactor_S" + tfSite.ToString() + ".csv";
		String ^ csvFileName = glob->TesterId + "_" + glob->tf.ProjectName + "_CorrFactor_S" + tfSite.ToString() + ".csv";

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Initialize '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder. Target directory: " + glob->FixedOffset.FixedOffsetFileFolderDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Initialize '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder. Target directory: " + glob->FixedOffset.FixedOffsetFileFolderDirectory + ".");

		// Check 'FixedOffsetFileFolder' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->FixedOffset.FixedOffsetFileFolderDirectory))
		{
			ret = ER_CONST_PROJECT_FIXEDOFFSETFILEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder does not exist in the target directory. A new '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'FixedOffsetFileFolder' folder
			Directory::CreateDirectory(glob->FixedOffset.FixedOffsetFileFolderDirectory);
			glob->FixedOffset.isCreateNewFixedOffsetFile = true;
			goto CreateNewFixedOffsetFile;
		}

		// Check 'FixedOffsetFileFolder' -> 'Archive' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->FixedOffset.FixedOffsetFileFolderDirectory + "\\Archive"))
		{
			ret = ER_CONST_PROJECT_FIXEDOFFSETFILEFOLDER_ARCHIVEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + FILENAME_CONST_ARCHIVE + "' folder does not exist in the '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder in the target directory. A new '" + FILENAME_CONST_ARCHIVE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + FILENAME_CONST_ARCHIVE + "' folder does not exist in the '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder in the target directory. A new '" + FILENAME_CONST_ARCHIVE + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'FixedOffsetFileFolder' -> 'Archive' folder
			Directory::CreateDirectory(glob->FixedOffset.FixedOffsetFileFolderDirectory + "\\Archive");
		}

		// Check 'FixedOffset_TesterID_Sx.csv' file existence in the project 'TestRecipes\FixedOffsetFileFolder' folder
		if (!File::Exists(glob->FixedOffset.FixedOffsetFileDirectory))
		{
			ret = ER_CONST_PROJECT_FIXEDOFFSETFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + csvFileName + "' file does not exist in the '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] '" + csvFileName + "' file does not exist in the '" + FILENAME_CONST_PROJECT_FIXEDOFFSETFILEFOLDER + "' folder. A new '" + csvFileName + "' file will be generated in the target folder (with empty fixed offset factor)." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			glob->FixedOffset.isCreateNewFixedOffsetFile = true;
			goto CreateNewFixedOffsetFile;
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Check '" + csvFileName + "' file format and content.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Check '" + csvFileName + "' file format and content.");
		// Compare and check the existing 'FixedOffset_TesterID_Sx.csv' contents with the project test recipes contents
		CheckExistingFixedOffsetFileContent(site, tfSite, glob->FixedOffset.FixedOffsetFileDirectory, csvFileName);

	CreateNewFixedOffsetFile:

		// To check whether need to generate a new 'FixedOffset_TesterID_Sx.csv'
		if ((glob->FixedOffset.isCreateNewFixedOffsetFile == true) || (glob->AWV.CreateFixedOffsetFile == true))
		{
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Generate new '" + csvFileName + "' file (with empty fixed offset factor).");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Generate new '" + csvFileName + "' file (with empty fixed offset factor).");
			// Copy all the existing files into the 'Archive' folder, delete all files
			int temp_FileCount = Directory::GetFiles(glob->FixedOffset.FixedOffsetFileFolderDirectory, "*.csv")->Length;
			array<String^> ^ temp_FileDirectory = gcnew array<String^>(temp_FileCount);
			temp_FileDirectory = Directory::GetFiles(glob->FixedOffset.FixedOffsetFileFolderDirectory, "*.csv");
			for (int i = 0; i < temp_FileCount; i++)
			{
				array<String^> ^ temp_ArrStr = gcnew array<String ^>(0);
				array<String^> ^ temp_Separator = gcnew array<String ^>(1);
				temp_Separator[0] = "\\";
				temp_ArrStr = temp_FileDirectory[i]->Split(temp_Separator, StringSplitOptions::None);
				File::Copy(temp_FileDirectory[i], glob->FixedOffset.FixedOffsetFileFolderDirectory + "\\Archive\\" + temp_ArrStr[8], true);
			}
			for (int i = 0; i < temp_FileCount; i++)
			{
				File::Delete(temp_FileDirectory[i]);
			}
			// Generate 'FixedOffset_TesterID_Sx.csv'
			GenerateFixedOffsetFile(site, tfSite, glob->FixedOffset.FixedOffsetFileDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Load fixed offset factor from '" + csvFileName + "' file.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFixedOffsetFileFolder] Load fixed offset factor from '" + csvFileName + "' file.");
		// Load fixed offset factor from 'FixedOffset_TesterID_Sx.csv' file and the key is test parameter name
		//glob->FixedOffset.ResultOffset = gcnew Dictionary<String^, array<double>^>();
		glob->FixedOffset.ResultOffset = gcnew Dictionary<String^, double>();

		//LoadFixedOffsetFile(tfSite, glob->FixedOffset.FixedOffsetFileDirectory);
		glob->FixedOffset.isCreateNewFixedOffsetFile = false;

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> ModulationFileFolderSitex [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeModulationFileFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeModulationFileFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'ModulationFileFolderSitex' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\ModulationFileFolderSitex
		**		This function support file/folder checking and file loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->ModulationFile.ModulationFileFolderDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString();

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder] Initialize '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDERSITEX + "' folder. Target directory: " + glob->ModulationFile.ModulationFileFolderDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder] Initialize '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDERSITEX + "' folder. Target directory: " + glob->ModulationFile.ModulationFileFolderDirectory + ".");

		if (!Directory::Exists(glob->ModulationFile.ModulationFileFolderDirectory))
		{
			ret = ER_CONST_PROJECT_MODULATIONFILEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeModulationFileFolder] '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeModulationFileFolder] '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'ModulationFileFolderSitex' folder
			Directory::CreateDirectory(glob->ModulationFile.ModulationFileFolderDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder] Load modulation file list from '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeModulationFileFolder] Load modulation file list from '" + FILENAME_CONST_PROJECT_MODULATIONFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		// Load modulation file list from 'ModulationFileFolderSitex' folder
		LoadModulationFile(tfSite);

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> VectorFileFolderSiteX [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeVectorFileFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeVectorFileFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'VectorFileFolderSitex' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\VectorFileFolderSitex
		**		This function support file/folder checking and file loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->VectorFile.VectorFileFolderDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString();

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder] Initialize '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDERSITEX + "' folder. Target directory: " + glob->VectorFile.VectorFileFolderDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder] Initialize '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDERSITEX + "' folder. Target directory: " + glob->VectorFile.VectorFileFolderDirectory + ".");

		// Check 'VectorFileFolderSitex' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->VectorFile.VectorFileFolderDirectory))
		{
			ret = ER_CONST_PROJECT_VECTORFILEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder] '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder] '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'VectorFileFolderSitex' folder
			Directory::CreateDirectory(glob->VectorFile.VectorFileFolderDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder] Load vector file list from '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorFileFolder] Load vector file list from '" + FILENAME_CONST_PROJECT_VECTORFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		// Load vector file list from 'VectorFileFolderSitex' folder
		LoadVectorFile(tfSite);

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	C --> Aemulus --> techFlow3 --> Projects --> TestRecipes --> 'SampleProfile' --> 'Project' --> VectorStateFileFolderSiteX [Init Related Variables]
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeVectorStateFileFolder(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeVectorStateFileFolder
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize helper function/variables that are related to the 'VectorStateFileFolderSitex' folder.
		**		Target directory: C:\Aemulus\techFlow3\Projects\TestRecipes\'SampleProfile'\'Project'\VectorStateFileFolderSitex
		**		This function support file/folder checking and file loading.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		glob->VectorStateFile.VectorStateFileFolderDirectory = glob->tf.RecipeFilePathDirectory + "\\" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString();

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] Initialize '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDERSITEX + "' folder. Target directory: " + glob->VectorStateFile.VectorStateFileFolderDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] Initialize '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDERSITEX + "' folder. Target directory: " + glob->VectorStateFile.VectorStateFileFolderDirectory + ".");

		// Check 'VectorStateFileFolderSitex' folder existence in the project 'TestRecipes' folder
		if (!Directory::Exists(glob->VectorStateFile.VectorStateFileFolderDirectory))
		{
			ret = ER_CONST_PROJECT_VECTORSTATEFILEFOLDER_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder does not exist in the target directory. A new '" FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder will be generated in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			ret = 0;
			// Create 'VectorStateFileFolderSitex' folder
			Directory::CreateDirectory(glob->VectorStateFile.VectorStateFileFolderDirectory);
		}

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] Load vector state file list from '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeVectorStateFileFolder] Load vector state file list from '" + FILENAME_CONST_PROJECT_VECTORSTATEFILEFOLDER + "Site" + tfSite.ToString() + "' folder.");
		// Load vector state file list from 'VectorStateFileFolderSitex' folder
		LoadVectorStateFile(tfSite);

#pragma region "Verification: check VectorFile & VectorStateFile Info"

		if (glob->VectorFile.totalVecFileExist != glob->VectorStateFile.totalVecStateFileExist)
		{
			ret = ER_CONST_PROJECT_VECTORFILE_VECTORSTATEFILE_CONTENT_NOT_MATCH;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' and 'VectorStateFile' files amount are not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' and 'VectorStateFile' files amount are not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		for (int i = 0; i < glob->VectorFile.totalVecFileExist; i++)
		{
			if (glob->VectorFile.vecFileName[i] != glob->VectorStateFile.vecStateFileName[i])
			{
				ret = ER_CONST_PROJECT_VECTORFILE_VECTORSTATEFILE_CONTENT_NOT_MATCH;
				WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' and 'VectorStateFile' filename are not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' and 'VectorStateFile' filename are not match." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
				return ret;
			}
		}

		if (glob->VectorFile.totalVecFileExist > 1024)
		{
			ret = ER_CONST_PROJECT_VECTORFILE_EXCEED_MAX_COUNT;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' amount cannot exceed 1024x file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeVectorFileFolder & InitializeVectorStateFileFolder] 'VectorFile' amount cannot exceed 1024x file." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

#pragma endregion

		EndOfTest:
				 return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Resource Manager Property (AEM DC Module)
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeResourceManagerProperty(Site ^ site, int tfSite)
	{
		/*****************************************************************************************************
		** InitializeResourceManagerProperty
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to get project resource properties.
		**		Including module alias, pin alias, pin map name, alias info.
		**		Only for AEM DC module, such as CM, AM, DM, IOM, ACM, TM.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeResourceManagerProperty] Initialize resource management property. Target directory: " + glob->tf.RecipeFilePathDirectory + ".");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeResourceManagerProperty] Initialize resource management property. Target directory: " + glob->tf.RecipeFilePathDirectory + ".");

		// Get project hardware profile
		if (String::IsNullOrEmpty(site->Recipe->ResourceMappingFilePath) == false)
		{
			glob->HardwareProfile = site->Recipe->ResourceMappingFilePath;
		}
		else
		{
			ret = ER_CONST_PROJECT_HARDWARE_PROFILE_NOT_FOUND;
			WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeResourceManagerProperty] Hardware profile (project AMAP) does not exist in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeResourceManagerProperty] Hardware profile (project AMAP) does not exist in the target directory." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: ");
			return ret;
		}

		// To get all the info from the resource planner
		int totalSiteIndex = 0;
		if ((glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset)) || (glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset)))
		{
			totalSiteIndex = glob->tf.TotalTestSite;
		}
		else if ((glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset)) || (glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset) || glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA)))
		{
			totalSiteIndex = glob->tf.TotalUUTOffsets;
		}
		else if (glob->tf.ProjectType == int(ProjectType::IndexParallel))
		{
			// [Future Enchancement] reserved for index parallel project.
		}

		// Resource Manager
		glob->RsrcManager.RsrcMngr = gcnew array<ResourceManager^>(totalSiteIndex);
		for (int i = 0; i < totalSiteIndex; i++)
		{
			glob->RsrcManager.RsrcMngr[i] = nullptr;
			glob->RsrcManager.RsrcMngr[i] = gcnew ResourceManager(glob->HardwareProfile, glob->tf.TestHead, i);
		}

#pragma region "CM400 Series Module Alias & Pin Alias"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.cmModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_CM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_CM200e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM201e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM400e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM401e) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_CM200e)) ||
					(hardwareResource->Type->StartsWith(ModuleName_CM201e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_CM400e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_CM401e))))
				{
					glob->RsrcManager.cmModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_CM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.cmModuleCount);
		glob->RsrcManager.moduleAlias_CM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.cmModuleCount);
		glob->RsrcManager.hardwareStatus_CM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.cmModuleCount);

		if (glob->RsrcManager.cmModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.cmModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_CM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_CM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_CM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int cmModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			cmModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_CM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_CM200e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM201e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM400e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM401e) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_CM200e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_CM201e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_CM400e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_CM401e))))
					{
						glob->RsrcManager.moduleAlias_CM_PinMapName[i, cmModuleIndex] = Rsrc->ToString();						// "Module Alias Name"
						glob->RsrcManager.moduleAlias_CM_Address[i, cmModuleIndex] = hardwareResource->Value->ToString();	// "PXIXX::0::INSTR"
						glob->RsrcManager.hardwareStatus_CM[i, cmModuleIndex] = true;
						cmModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.cmPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_CM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_CM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_CM200e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM201e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM400e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_CM401e) == true)) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.cmPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_CM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.cmPinCount);
		glob->RsrcManager.pinAlias_CM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.cmPinCount);
		glob->RsrcManager.pinAlias_CM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.cmPinCount);

		if (glob->RsrcManager.cmPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.cmPinCount; j++)
				{
					glob->RsrcManager.pinAlias_CM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_CM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_CM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int cmPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			cmPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_CM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_CM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_CM200e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM201e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM400e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_CM401e) == true)) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_CM_PinMapName[i, cmPinIndex] = Rsrc->ToString();								// "Pin Alias Name"
						glob->RsrcManager.pinAlias_CM_Address[i, cmPinIndex] = hardwareResource->Hierarchy[0]->ToString();	// "XMXXXe=PXIXX::0::INSTR"
						glob->RsrcManager.pinAlias_CM_HwResourceAlias[i, cmPinIndex] = hardwareResource->Alias->ToString();			// "XMXXXe_X_ChX"
						cmPinIndex++;
					}
				}
			}
		}

#pragma endregion

#pragma region "AM400 Series Module Alias & Pin Alias"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.amModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_AM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_AM430e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM450e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM451e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM471e) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_AM430e)) ||
					(hardwareResource->Type->StartsWith(ModuleName_AM450e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_AM451e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_AM471e))))
				{
					glob->RsrcManager.amModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_AM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.amModuleCount);
		glob->RsrcManager.moduleAlias_AM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.amModuleCount);
		glob->RsrcManager.hardwareStatus_AM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.amModuleCount);

		if (glob->RsrcManager.amModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.amModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_AM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_AM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_AM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int amModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			amModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_AM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_AM430e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM450e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM451e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM471e) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_AM430e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_AM450e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_AM451e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_AM471e))))
					{
						glob->RsrcManager.moduleAlias_AM_PinMapName[i, amModuleIndex] = Rsrc->ToString();
						glob->RsrcManager.moduleAlias_AM_Address[i, amModuleIndex] = hardwareResource->Value->ToString();
						glob->RsrcManager.hardwareStatus_AM[i, amModuleIndex] = true;
						amModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.amPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_AM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_AM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_AM430e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM450e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM451e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_AM471e) == true)) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.amPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_AM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.amPinCount);
		glob->RsrcManager.pinAlias_AM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.amPinCount);
		glob->RsrcManager.pinAlias_AM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.amPinCount);

		if (glob->RsrcManager.amPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.amPinCount; j++)
				{
					glob->RsrcManager.pinAlias_AM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_AM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_AM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int amPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			amPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_AM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_AM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_AM430e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM450e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM451e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_AM471e) == true)) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_AM_PinMapName[i, amPinIndex] = Rsrc->ToString();
						glob->RsrcManager.pinAlias_AM_Address[i, amPinIndex] = hardwareResource->Hierarchy[0]->ToString();
						glob->RsrcManager.pinAlias_AM_HwResourceAlias[i, amPinIndex] = hardwareResource->Alias->ToString();
						amPinIndex++;
					}
				}
			}
		}

#pragma endregion

#pragma region "DM400 Series Module Alias & Pin Alias"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.dmModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_DM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_DM481e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_DM482e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_DM483e) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_DM481e)) ||
					(hardwareResource->Type->StartsWith(ModuleName_DM482e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_DM483e))))
				{
					glob->RsrcManager.dmModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_DM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.dmModuleCount);
		glob->RsrcManager.moduleAlias_DM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.dmModuleCount);
		glob->RsrcManager.hardwareStatus_DM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.dmModuleCount);

		if (glob->RsrcManager.dmModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.dmModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_DM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_DM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_DM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int dmModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			dmModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_DM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_DM481e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_DM482e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_DM483e) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_DM481e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_DM482e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_DM483e))))
					{
						glob->RsrcManager.moduleAlias_DM_PinMapName[i, dmModuleIndex] = Rsrc->ToString();
						glob->RsrcManager.moduleAlias_DM_Address[i, dmModuleIndex] = hardwareResource->Value->ToString();
						glob->RsrcManager.hardwareStatus_DM[i, dmModuleIndex] = true;
						dmModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.dmPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_DM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_DM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_DM481e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_DM482e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_DM483e) == true)) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.dmPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_DM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.dmPinCount);
		glob->RsrcManager.pinAlias_DM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.dmPinCount);
		glob->RsrcManager.pinAlias_DM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.dmPinCount);

		if (glob->RsrcManager.dmPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.dmPinCount; j++)
				{
					glob->RsrcManager.pinAlias_DM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_DM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_DM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int dmPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			dmPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_DM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_DM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_DM481e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_DM482e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_DM483e) == true)) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_DM_PinMapName[i, dmPinIndex] = Rsrc->ToString();
						glob->RsrcManager.pinAlias_DM_Address[i, dmPinIndex] = hardwareResource->Hierarchy[0]->ToString();
						glob->RsrcManager.pinAlias_DM_HwResourceAlias[i, dmPinIndex] = hardwareResource->Alias->ToString();
						dmPinIndex++;
					}
				}
			}
		}

#pragma endregion

#pragma region "IOM400 Series Module Alias & Pin Alias"

		// [Future Enchancement] To cater IOM module alias, port alias, pin alias.

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.iomModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_IOM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_IOM420e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_IOM421) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_IOM420e)) ||
					(hardwareResource->Type->StartsWith(ModuleName_IOM421))))
				{
					glob->RsrcManager.iomModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_IOM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.iomModuleCount);
		glob->RsrcManager.moduleAlias_IOM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.iomModuleCount);
		glob->RsrcManager.hardwareStatus_IOM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.iomModuleCount);

		if (glob->RsrcManager.iomModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.iomModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_IOM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_IOM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_IOM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int iomModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			iomModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_IOM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_IOM420e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_IOM421) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_IOM420e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_IOM421))))
					{
						glob->RsrcManager.moduleAlias_IOM_PinMapName[i, iomModuleIndex] = Rsrc->ToString();
						glob->RsrcManager.moduleAlias_IOM_Address[i, iomModuleIndex] = hardwareResource->Value->ToString();
						glob->RsrcManager.hardwareStatus_IOM[i, iomModuleIndex] = true;
						iomModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.iomPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_IOM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_IOM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_IOM420e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_IOM421) == true)) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.iomPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_IOM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.iomPinCount);
		glob->RsrcManager.pinAlias_IOM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.iomPinCount);
		glob->RsrcManager.pinAlias_IOM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.iomPinCount);

		if (glob->RsrcManager.iomPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.iomPinCount; j++)
				{
					glob->RsrcManager.pinAlias_IOM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_IOM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_IOM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int iomPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			iomPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_IOM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_IOM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_IOM420e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_IOM421) == true)) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_IOM_PinMapName[i, iomPinIndex] = Rsrc->ToString();
						glob->RsrcManager.pinAlias_IOM_Address[i, iomPinIndex] = hardwareResource->Hierarchy[0]->ToString();
						glob->RsrcManager.pinAlias_IOM_HwResourceAlias[i, iomPinIndex] = hardwareResource->Alias->ToString();
						iomPinIndex++;
					}
				}
			}
		}

#pragma endregion

#pragma region "ACM400 Series Module Alias & Pin Alias"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.acmModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_ACM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_ACM432e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_ACM433e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_ACM434e) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_ACM432e)) ||
					(hardwareResource->Type->StartsWith(ModuleName_ACM433e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_ACM434e))))
				{
					glob->RsrcManager.acmModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_ACM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.acmModuleCount);
		glob->RsrcManager.moduleAlias_ACM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.acmModuleCount);
		glob->RsrcManager.hardwareStatus_ACM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.acmModuleCount);

		if (glob->RsrcManager.acmModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.acmModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_ACM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_ACM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_ACM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int acmModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			acmModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_ACM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_ACM432e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_ACM433e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_ACM434e) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_ACM432e)) ||
						(hardwareResource->Type->StartsWith(ModuleName_ACM433e)) ||
							(hardwareResource->Type->StartsWith(ModuleName_ACM434e))))
					{
						glob->RsrcManager.moduleAlias_ACM_PinMapName[i, acmModuleIndex] = Rsrc->ToString();
						glob->RsrcManager.moduleAlias_ACM_Address[i, acmModuleIndex] = hardwareResource->Value->ToString();
						glob->RsrcManager.hardwareStatus_ACM[i, acmModuleIndex] = true;
						acmModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.acmPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_ACM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_ACM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_ACM432e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_ACM433e) == true) ||
					(hardwareResource->Alias->StartsWith(ModuleName_ACM434e) == true)) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.acmPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_ACM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.acmPinCount);
		glob->RsrcManager.pinAlias_ACM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.acmPinCount);
		glob->RsrcManager.pinAlias_ACM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.acmPinCount);

		if (glob->RsrcManager.acmPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.acmPinCount; j++)
				{
					glob->RsrcManager.pinAlias_ACM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_ACM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_ACM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int acmPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			acmPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_ACM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_ACM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_ACM432e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_ACM433e) == true) ||
						(hardwareResource->Alias->StartsWith(ModuleName_ACM434e) == true)) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_ACM_PinMapName[i, acmPinIndex] = Rsrc->ToString();
						glob->RsrcManager.pinAlias_ACM_Address[i, acmPinIndex] = hardwareResource->Hierarchy[0]->ToString();
						glob->RsrcManager.pinAlias_ACM_HwResourceAlias[i, acmPinIndex] = hardwareResource->Alias->ToString();
						acmPinIndex++;
					}
				}
			}
		}

#pragma endregion

#pragma region "TM400 Series Module Alias & Pin Alias"

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.tmModuleCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((hardwareResource->Hierarchy->Length == 1) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_TM) == true))
			{
				if (((hardwareResource->Alias->StartsWith(ModuleName_TM460e) == true)) &&
					((hardwareResource->Type->StartsWith(ModuleName_TM460e))))
				{
					glob->RsrcManager.tmModuleCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.moduleAlias_TM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.tmModuleCount);
		glob->RsrcManager.moduleAlias_TM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.tmModuleCount);
		glob->RsrcManager.hardwareStatus_TM = gcnew array<bool, 2>(totalSiteIndex, glob->RsrcManager.tmModuleCount);

		if (glob->RsrcManager.tmModuleCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.tmModuleCount; j++)
				{
					glob->RsrcManager.moduleAlias_TM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.moduleAlias_TM_Address[i, j] = String::Empty;
					glob->RsrcManager.hardwareStatus_TM[i, j] = false;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get module alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int tmModuleIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			tmModuleIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((hardwareResource->Hierarchy->Length == 1) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_TM) == true))
				{
					if (((hardwareResource->Alias->StartsWith(ModuleName_TM460e) == true)) &&
						((hardwareResource->Type->StartsWith(ModuleName_TM460e))))
					{
						glob->RsrcManager.moduleAlias_TM_PinMapName[i, tmModuleIndex] = Rsrc->ToString();
						glob->RsrcManager.moduleAlias_TM_Address[i, tmModuleIndex] = hardwareResource->Value->ToString();
						glob->RsrcManager.hardwareStatus_TM[i, tmModuleIndex] = true;
						tmModuleIndex++;
					}
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias type count
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.tmPinCount = 0;
		// Apply RsrcMngr[0] because don't care about the site, just collect all the pin/module alias
		for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[0]->MapNames)
		{
			Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[0]->ResolveResource(Rsrc)[0];

			if ((glob->RsrcManager.hardwareStatus_TM->Length != 0) &&
				(hardwareResource->Hierarchy->Length == 2) &&
				(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_TM) == true) &&
				(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
			{
				if ((hardwareResource->Alias->StartsWith(ModuleName_TM460e) == true) &&
					(hardwareResource->Type->StartsWith("Pin")))
				{
					glob->RsrcManager.tmPinCount++;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Setup storage for pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		glob->RsrcManager.pinAlias_TM_PinMapName = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.tmPinCount);
		glob->RsrcManager.pinAlias_TM_Address = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.tmPinCount);
		glob->RsrcManager.pinAlias_TM_HwResourceAlias = gcnew array<String^, 2>(totalSiteIndex, glob->RsrcManager.tmPinCount);

		if (glob->RsrcManager.tmPinCount != 0)
		{
			for (int i = 0; i < totalSiteIndex; i++)
			{
				for (int j = 0; j < glob->RsrcManager.tmPinCount; j++)
				{
					glob->RsrcManager.pinAlias_TM_PinMapName[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_TM_Address[i, j] = String::Empty;
					glob->RsrcManager.pinAlias_TM_HwResourceAlias[i, j] = String::Empty;
				}
			}
		}

		/*
		**	----------------------------------------------------------------------------------------------------
		**	Get pin alias
		**	----------------------------------------------------------------------------------------------------
		*/
		int tmPinIndex = 0;
		for (int i = 0; i < totalSiteIndex; i++)
		{
			tmPinIndex = 0;
			for each (String ^ Rsrc in glob->RsrcManager.RsrcMngr[i]->MapNames)
			{
				Resource ^ hardwareResource = glob->RsrcManager.RsrcMngr[i]->ResolveResource(Rsrc)[0];

				if ((glob->RsrcManager.hardwareStatus_TM->Length != 0) &&
					(hardwareResource->Hierarchy->Length == 2) &&
					(hardwareResource->Hierarchy[0]->StartsWith(ModuleType_TM) == true) &&
					(hardwareResource->Hierarchy[1]->StartsWith("Pin") == true))
				{
					if ((hardwareResource->Alias->StartsWith(ModuleName_TM460e) == true) &&
						(hardwareResource->Type->StartsWith("Pin")))
					{
						glob->RsrcManager.pinAlias_TM_PinMapName[i, tmPinIndex] = Rsrc->ToString();
						glob->RsrcManager.pinAlias_TM_Address[i, tmPinIndex] = hardwareResource->Hierarchy[0]->ToString();
						glob->RsrcManager.pinAlias_TM_HwResourceAlias[i, tmPinIndex] = hardwareResource->Alias->ToString();
						tmPinIndex++;
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
	**	General Global Variables
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::InitializeGlobalVariables(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeGlobalVariables
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initialize all the general global variables.
		******************************************************************************************************/

		WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeGlobalVariables] Initialize general global variable.");
		WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeGlobalVariables] Initialize general global variable.");

#pragma region "Test Property Variables"

		glob->TestProperty = gcnew array<Globals::TestPropertyStruct>(glob->tf.NumberOfTestSites);
		for (int siteIndex = 0; siteIndex < glob->tf.NumberOfTestSites; siteIndex++)
		{
			// Test Property
			glob->TestProperty[siteIndex].TestParaNameWithSiteIndex = gcnew Dictionary<String^, String^>();
			glob->TestProperty[siteIndex].TestParaDisplayNameWithSiteIndex = gcnew Dictionary<String^, String^>();
			glob->TestProperty[siteIndex].TestResults = gcnew Dictionary<String^, Object^>();
			glob->TestProperty[siteIndex].PreviousTestResults = gcnew Dictionary<String^, Object^>();

			// Sub Item
			glob->TestProperty[siteIndex].ControlItemName = String::Empty;
			glob->TestProperty[siteIndex].FlowItemName = String::Empty;

			// Test Item
			glob->TestProperty[siteIndex].TestItemName = String::Empty;
			glob->TestProperty[siteIndex].TestItemDisplayName = String::Empty;

			// Sub Item (Test Step, Control Step, Test Parameter) 
			glob->TestProperty[siteIndex].totalSubItem = 0;

			// Test Parameter
			glob->TestProperty[siteIndex].totalTestParameter = 0;
			glob->TestProperty[siteIndex].TestParameterName = gcnew array<String^>(0);
			glob->TestProperty[siteIndex].TestParameterDisplayName = gcnew array<String^>(0);
			glob->TestProperty[siteIndex].TestParameterExecuted = gcnew Dictionary<String^, bool>();
			glob->TestProperty[siteIndex].TestParameterTestStatus = gcnew array<int>(0);
			glob->TestProperty[siteIndex].TestParameterUpdateResStatus = gcnew Dictionary<String^, bool>();
			glob->TestProperty[siteIndex].IsCurrentTPBypassed = gcnew array<bool>(0);
			glob->TestProperty[siteIndex].IsHardwareInvolved = gcnew array<bool>(0);

			// Flow Step (Control Step, Test Step)
			glob->TestProperty[siteIndex].totalFlowStep = 0;
			glob->TestProperty[siteIndex].FlowStepItemName = gcnew array<String^>(0);
			glob->TestProperty[siteIndex].FlowStepItemDisplayName = gcnew array<String^>(0);
			glob->TestProperty[siteIndex].FlowStepItemExecuted = gcnew array<bool>(0);
		}

#pragma endregion 

#pragma region "Test Parameter DataType Variables (Result)"

		glob->ResultWithDataType = gcnew array<Globals::TestParameterDataTypeProperty>(glob->tf.NumberOfTestSites);
		for (int siteIndex = 0; siteIndex < glob->tf.NumberOfTestSites; siteIndex++)
		{
			glob->ResultWithDataType[siteIndex].StringTyperesult = String::Empty;
			//glob->ResultWithDataType[siteIndex].Int16TypeResult	= 999;
			//glob->ResultWithDataType[siteIndex].Int32TypeResult	= 999;
			//glob->ResultWithDataType[siteIndex].Int64TypeResult	= 999;
			glob->ResultWithDataType[siteIndex].FloatTyperesult = 999.99;
			glob->ResultWithDataType[siteIndex].DoubleTypeResult = 999.99;
			glob->ResultWithDataType[siteIndex].BoolTypeResult = false;
			//glob->ResultWithDataType[siteIndex].UInt16TypeResult	= 999;
			//glob->ResultWithDataType[siteIndex].UInt32TypeResult	= 999;
			//glob->ResultWithDataType[siteIndex].UInt64TypeResult	= 999;
			glob->ResultWithDataType[siteIndex].IntTypeResult = 999;
			//glob->ResultWithDataType[siteIndex].UIntTypeResult	= 999;
		}

#pragma endregion 

	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Tracer Logger
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeTracerLogger(Site ^ site, int tfSite)
	{
		/*****************************************************************************************************
		** InitializeTracerLogger
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initiate tracer logger when debug mode is enabled.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// Instanstiate Tracer Logger in 'Debug' mode only.
		if (glob->AWV.Debug == 1)
		{
			if (glob->tf.StageCount == 1)
			{
				try
				{
					// Initialize
					glob->TcrLgr.TracerLog = gcnew Aemulus::TestLib::Utility::TracerLogger(site);
				}
				catch (Exception^ ex)
				{
					ret = ER_CONST_FAIL_TO_INIT_TRACER_LOGGER;
					return ret;
				}

				// Default set as "None" prefix unit
				glob->TcrLgr.PrefixValue = 1.0;

				WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeTracerLogger] Initialize tracer logger.");
				//WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeTracerLogger] Initialize tracer logger.");
			}
			else
			{
				// [Future Enchancement] reserved for index parallel project.
			}
		}

	EndOfTest:
		return ret;
	}
	void TestFunction::WRITETOTRACERLOGGER(int tfSite, int siteIndex, String ^ messageType, String ^ message, int programLineNumber, String ^ programFileName, String ^ programFunctionName)
	{
		/*****************************************************************************************************
		** WRITETOTRACERLOGGER
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is resource object index, normally start from 0.
		**		messageType				- This is message type, such as INFO / ERROR / WARNING.
		**		message					- This is message to display.
		**		programLineNumber		- This is program's line number where exception happened.
		**		programFileName			- This is program's file name where exception happened.
		**		programFunctionName		- This is program's function name where exception happened.
		**
		** Descriptions:
		**		This is a function to log all every single action into log message to the tracer window.
		**		Only applicable in debug mode.
		**		Main purpose is to ease the debug action.
		**		This is useful when user intended to trace the single API before executing and after execited effect.
		******************************************************************************************************/

		if (glob->AWV.Debug == 1)
		{
			// Get tracer's tab name
			if (siteIndex == glob->TcrLgr.tracerMainTab)
			{
				glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString() + " Main";
			}
			else
			{
				if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset))
				{
					glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString();
				}
				else if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset))
				{
					glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString() + " UUT OFFSET " + siteIndex.ToString();
				}
				else if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
				{
					glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString() + " UUT OFFSET " + siteIndex.ToString();
				}
				else if (glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset))
				{
					glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString();
				}
				else if (glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset))
				{
					glob->TcrLgr.TracerTabNamePhysicalSite = "SITE " + tfSite.ToString() + " UUT OFFSET " + siteIndex.ToString();
				}
				else if (glob->tf.ProjectType == int(ProjectType::IndexParallel))
				{
					// [Future Enchancement] reserved for index parallel project.
				}
			}

			// Get time now
			glob->TimeNow = DateTime::Now.ToString("yyyy/MM/dd' 'h:mm:ss tt");

			// Write to tracer logger
			if (messageType == WARNING)
			{
				glob->TcrLgr.TracerMessage = LOGGER_TYPE_WARNING + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToTcrLgr(glob->TcrLgr.TracerTabNamePhysicalSite, glob->TcrLgr.TracerMessage);
			}
			else if (messageType == ERROR)
			{
				glob->TcrLgr.TracerMessage = LOGGER_TYPE_ERROR + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToTcrLgr(glob->TcrLgr.TracerTabNamePhysicalSite, glob->TcrLgr.TracerMessage);
			}
			else if (messageType == INFO)
			{
				glob->TcrLgr.TracerMessage = LOGGER_TYPE_INFO + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToTcrLgr(glob->TcrLgr.TracerTabNamePhysicalSite, glob->TcrLgr.TracerMessage);
			}
		}
	}
	void TestFunction::WriteToTcrLgr(String ^ tracerTabName, String ^ message)
	{
		/*****************************************************************************************************
		** WriteToTcrLgr
		**		tracerTabName	- This is tracer window's tab name.
		**		message			- This is message to display.
		**
		** Descriptions:
		**		This is a function to write the detail/message to the tracer window.
		**		This is to ease the user in debugging process.
		**		Only applicable in debug mode.
		******************************************************************************************************/

		if (glob->AWV.Debug == 1)
		{
			glob->TcrLgr.TracerLog->WriteToTracer(tracerTabName, message);
		}
	}
	int TestFunction::UninitializeTracerLogger()
	{
		/*****************************************************************************************************
		** UninitializeTracerLogger
		**
		**
		** Descriptions:
		**		This is a function to clear and remove all the tracer tabsof tracer window.
		**		Only valid when tracer logger is instantaited at first.
		**		Only available when debug mode.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		if (glob->AWV.Debug == 1)
		{
			if (glob->tf.StageCount == 1)
			{
				WriteToTracerLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeProgram -> UninitializeTracerLogger] Uninitialize tracer logger.");
				//WriteToFileLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeProgram -> UninitializeTracerLogger] Uninitialize tracer logger.");

				try
				{
					// Uninitialize
					Util->WaitSecond(2.0 S);
					glob->TcrLgr.TracerLog->RemoveAllTracer();
				}
				catch (Exception^ ex)
				{
					ret = ER_CONST_FAIL_TO_UNINIT_TRACER_LOGGER;
					WriteToTracerLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, ERROR, "[Unload -> UninitializeProgram -> UninitializeTracerLogger] Fail to uninitialize tracer logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					//WriteToFileLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, ERROR, "[Unload -> UninitializeProgram -> UninitializeTracerLogger] Fail to uninitialize tracer logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					return ret;
				}
			}
			else
			{
				// [Future Enchancement] reserved for index parallel project.
			}
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	File Logger
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::InitializeFileLogger(int tfSite)
	{
		/*****************************************************************************************************
		** InitializeFileLogger
		**		site - This is techFlow site object.
		**
		** Descriptions:
		**		This is a function to initiate file logger when debug mode is enabled.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		// [Future Enchancement] To support file logger is able to log only error code in 'Release' mode.

		// Instanstiate File Logger in 'Debug' mode only.
		if (glob->AWV.Debug == 1)
		{
			if (glob->tf.StageCount == 1)
			{
				try
				{
					// Initialize
					glob->FileLog.FileLgr = gcnew Aemulus::TestLib::Utility::FileLogger(FOLDER_CONST_AEM_DEBUG);
					//glob->FileLog.fi			= gcnew FileInfo(FOLDER_CONST_AEM_DEBUG + "\\" + glob->FileLog.FileNameDebugLog);
					//glob->FileLog.IsWarning	= false;
				}
				catch (Exception^ ex)
				{
					ret = ER_CONST_FAIL_TO_INIT_FILE_LOGGER;
					WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFileLogger] Fail to initialize file logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, ERROR, "[Load -> InitializeProgram -> InitializeFileLogger] Fail to initialize file logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					return ret;
				}

				WriteToTracerLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFileLogger] Initialize file logger.");
				WriteToFileLogger(tfSite, glob->TcrLgr.tracerMainTab, INFO, "[Load -> InitializeProgram -> InitializeFileLogger] Initialize file logger.");
			}
			else
			{
				// [Future Enchancement] reserved for index parallel project.
			}
		}

	EndOfTest:
		return ret;
	}
	void TestFunction::WRITETOFILELOGGER(int tfSite, int siteIndex, String ^ messageType, String ^ message, int programLineNumber, String ^ programFileName, String ^ programFunctionName)
	{
		/*****************************************************************************************************
		** WRITETOFILELOGGER
		**		tfSite					- This is techFlow site index.
		**		siteIndex				- This is resource object index, normally start from 0.
		**		messageType				- This is message type, such as INFO / ERROR / WARNING.
		**		message					- This is message to display.
		**		programLineNumber		- This is program's line number where exception happened.
		**		programFileName			- This is program's file name where exception happened.
		**		programFunctionName		- This is program's function name where exception happened.
		**
		** Descriptions:
		**		This is a function to log all every single action into log message to the file logger.
		**		Only applicable in debug mode.
		**		This is to record all the actions of the debugging process.
		**		This is useful when user intended to trace the single API before executing and after execited effect.
		******************************************************************************************************/

		if (glob->AWV.Debug == 1)
		{
			// Get debug's file name
			if (siteIndex == glob->TcrLgr.tracerMainTab)
			{
				glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + "Main" + "_" + glob->FileNameTime + ".csv";
			}
			else
			{
				if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteSingleUUTOffset))
				{
					glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + glob->FileNameTime + ".csv";
				}
				else if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffset))
				{
					glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + "UUTOFFSET" + siteIndex.ToString() + "_" + glob->FileNameTime + ".csv";
				}
				else if (glob->tf.ProjectType == int(ProjectType::SingleTFSiteMultiUUTOffsetSharedVNA))
				{
					glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + "UUTOFFSET" + siteIndex.ToString() + "_" + glob->FileNameTime + ".csv";
				}
				else if (glob->tf.ProjectType == int(ProjectType::TrueParallelSingleUUTOffset))
				{
					glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + glob->FileNameTime + ".csv";
				}
				else if (glob->tf.ProjectType == int(ProjectType::TrueParallelMultiUUTOffset))
				{
					glob->FileLog.FileNameDebugLog = "DebugLog" + "_" + glob->TesterId + "_" + glob->tf.ProjectName + "_" + "SITE" + tfSite.ToString() + "_" + "UUTOFFSET" + siteIndex.ToString() + "_" + glob->FileNameTime + ".csv";
				}
				else if (glob->tf.ProjectType == int(ProjectType::IndexParallel))
				{
					// [Future Enchancement] reserved for index parallel project.
				}
			}

			// Get time now
			glob->TimeNow = DateTime::Now.ToString("yyyy/MM/dd' 'h:mm:ss tt");

			// Write to file logger
			if (messageType == WARNING)
			{
				glob->FileLog.LogMessage = LOGGER_TYPE_WARNING + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToFileLgr(glob->FileLog.FileNameDebugLog, glob->FileLog.LogMessage);
			}
			else if (messageType == ERROR)
			{
				glob->FileLog.LogMessage = LOGGER_TYPE_ERROR + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToFileLgr(glob->FileLog.FileNameDebugLog, glob->FileLog.LogMessage);
			}
			else if (messageType == INFO)
			{
				glob->FileLog.LogMessage = LOGGER_TYPE_INFO + "[" + glob->TimeNow + "] " + message + " at line #" + programLineNumber + " [" + programFileName + " @ " + programFunctionName + "].";
				WriteToFileLgr(glob->FileLog.FileNameDebugLog, glob->FileLog.LogMessage);
			}
		}
	}
	void TestFunction::WriteToFileLgr(String ^ fileDirectory, String ^ message)
	{
		/*****************************************************************************************************
		** WriteToFileLgr
		**		fileDirectory	- This is debug logger file's directory.
		**		message			- This is message to display.
		**
		** Descriptions:
		**		This is a function to write the detail/message to the file logger.
		**		This is to record all the actions of the debugging process.
		**		Only applicable in debug mode.
		******************************************************************************************************/

		if (glob->AWV.Debug == 1)
		{
			glob->FileLog.FileLgr->WriteToFile(fileDirectory, message, LOGGER_CONST_APPEND);
			Util->WaitSecond(5.0 mS); // ori setting 2S
		}
	}
	int TestFunction::UninitializeFileLogger()
	{
		/*****************************************************************************************************
		** UninitializeFileLogger
		**
		**
		** Descriptions:
		**		This is a function to close file logger session.
		******************************************************************************************************/

		// Local variable
		int ret = 0;

		if (glob->AWV.Debug == 1)
		{
			if (glob->tf.StageCount == 1)
			{
				WriteToTracerLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeFileLogger] Uninitialize file logger.");
				WriteToFileLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, INFO, "[Unload -> UninitializeFileLogger] Uninitialize file logger.");

				try
				{
					// [Future Enchancement] Closing file logger and handle situation if got error occur.
					// Uninitialize
				}
				catch (Exception^ ex)
				{
					ret = ER_CONST_FAIL_TO_UNINIT_FILE_LOGGER;
					WriteToTracerLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, ERROR, "[Unload -> UninitializeFileLogger] Fail to uninitialize file logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					WriteToFileLogger(glob->tf.TestSite, glob->TcrLgr.tracerMainTab, ERROR, "[Unload -> UninitializeFileLogger] Fail to uninitialize file logger." + " | " + "Error Code: " + ret.ToString() + " | " + "Detail: " + ex->Message);
					return ret;
				}
			}
			else
			{
				// [Future Enchancement] reserved for index parallel project.
			}
		}

	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Check Error
	**	----------------------------------------------------------------------------------------------------
	*/
	int TestFunction::CHECKERROR(int siteIndex, int errorCode, int ErrorLineNumber, String ^ FileName)
	{
		/*****************************************************************************************************
		** CHECKERROR
		** Arguments:
		**		siteIndex		- This is selected physical site. (Note: UUT offset index for the MultiUUTOffsets
		**						  project or techFlow sites for Index Parallel project.)
		**		errorCode		- This is the error code.
		**		ErrorLineNumber - This is the line number in test program where last error happens.
		**		FileName		- This is the file name of current executing function in test program where the
		**						  last error happens.
		** Descriptions:
		**		This method is to check whether the last operation has any error, by interpreting the given
		**		errorCode. If error happens (non-zero return code), an exception will be thrown to techFlow,
		**		together with the error message as well as the line where it happens.
		******************************************************************************************************/

		// Local variable
		int ret = 0;
		String ^ errorCodeMessage = String::Empty;
		String ^ ErrorMessage = String::Empty;

		if (errorCode != 0)
		{
			/*
			**	----------------------------------------------------------------------------------------------------
			**	AEM resources (error code start with "0xAE")
			**	----------------------------------------------------------------------------------------------------
			*/
			if (errorCode.ToString("X")->StartsWith("AE"))
			{
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 0 [General]
				**	----------------------------------------------------------------------------------------------------
				*/
				if (errorCode.ToString("X")->StartsWith("AE00"))
				{
					switch (errorCode)
					{
					case ER_S_OK: errorCodeMessage = "S_OK";												//break;
					case ER_ERROR_NOT_ENOUGH_MEMORY: errorCodeMessage = "ERROR_NOT_ENOUGH_MEMORY";								//break;
					case ER_ERROR_BAD_ARGUMENTS: errorCodeMessage = "ERROR_BAD_ARGUMENTS";									//break;
					case ER_ERROR_OPEN_FAILED: errorCodeMessage = "ERROR_OPEN_FAILED";									//break;
					case ER_AEM_INFO: errorCodeMessage = "AEM_INFO";											//break;
					case ER_AEM_WARNING: errorCodeMessage = "AEM_WARNING";											//break;
					case ER_AEM_ERROR: errorCodeMessage = "AEM_ERROR";											//break;
					case ER_AEM_CRITICAL: errorCodeMessage = "AEM_CRITICAL";										//break;
					}
					ErrorMessage = "[AEM Facility Code 0: General] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 1 [AemIPC]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE01"))
				{
					switch (errorCode)
					{
					case ER_AEMIPC_E_NOT_OPEN: errorCodeMessage = "AEMIPC_E_NOT_OPEN";									//break;
					case ER_AEMIPC_E_ALREADY_OPEN: errorCodeMessage = "AEMIPC_E_ALREADY_OPEN";								//break;
					}
					ErrorMessage = "[AEM Facility Code 1: AemIPC] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 2 [AemDAPI]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE02"))
				{
					switch (errorCode)
					{
					case ER_AEMHW_E_SYSTYPE_MISMATCH: errorCodeMessage = "AEMHW_E_SYSTYPE_MISMATCH";							//break;
					case ER_AEMHW_E_INVALID_HANDLE: errorCodeMessage = "AEMHW_E_INVALID_HANDLE";								//break;
					case ER_AEMHW_E_INVALID_RESULT: errorCodeMessage = "AEMHW_E_INVALID_RESULT";								//break;
					case ER_AEMHW_E_LOAD_AVEC_OVERFLOW: errorCodeMessage = "AEMHW_E_LOAD_AVEC_OVERFLOW";							//break;
					case ER_AEMDUSB_E_CANNOT_WRITE_TO_DEVICE: errorCodeMessage = "AEMDUSB_E_CANNOT_WRITE_TO_DEVICE";					//break;
					case ER_AEMDUSB_E_CANNOT_READ_FROM_DEVICE: errorCodeMessage = "AEMDUSB_E_CANNOT_READ_FROM_DEVICE";					//break;
					case ER_AEMDUSB_E_EXT_SIZE_NOT_MATCH_IPC: errorCodeMessage = "AEMDUSB_E_EXT_SIZE_NOT_MATCH_IPC";					//break;
					case ER_AEMDUSB_E_GET_SITEID_FAILED: errorCodeMessage = "AEMDUSB_E_GET_SITEID_FAILED";							//break;
					case ER_AEMDUSB_E_SITEID_ALREADY_EXIST: errorCodeMessage = "AEMDUSB_E_SITEID_ALREADY_EXIST";						//break;
					case ER_AEMDUSB_E_SITEID_NOT_FOUND: errorCodeMessage = "AEMDUSB_E_SITEID_NOT_FOUND";							//break;
					case ER_AEMDUSB_E_MODULEID_NOT_FOUND: errorCodeMessage = "AEMDUSB_E_MODULEID_NOT_FOUND";						//break;
					case ER_AEMDUSB_E_PORTID_NOT_FOUND: errorCodeMessage = "AEMDUSB_E_PORTID_NOT_FOUND";							//break;
					case ER_AEMDUSB_E_RESPINID_NOT_FOUND: errorCodeMessage = "AEMDUSB_E_RESPINID_NOT_FOUND";						//break;
					}
					ErrorMessage = "[AEM Facility Code 2: AemDAPI] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 3 [AemFlow]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE03"))
				{
					switch (errorCode)
					{
					case ER_AEMPARAMS_E_PARAM_NOT_FOUND: errorCodeMessage = "AEMPARAMS_E_PARAM_NOT_FOUND";							//break;
					case ER_AEMPARAMS_E_TEST_NOT_FOUND: errorCodeMessage = "AEMPARAMS_E_TEST_NOT_FOUND";							//break;
					case ER_AEMPARAMS_E_EMPTY_PARAM_VALUE: errorCodeMessage = "AEMPARAMS_E_EMPTY_PARAM_VALUE";						//break;
					case ER_AEMFLOW_E_OBJECT_NOT_FOUND: errorCodeMessage = "AEMFLOW_E_OBJECT_NOT_FOUND";							//break;
					case ER_AEMFLOW_E_OBJECT_ALREADY_EXISTS: errorCodeMessage = "AEMFLOW_E_OBJECT_ALREADY_EXISTS";						//break;
					case ER_AEMFLOW_E_NULL_OBJECT: errorCodeMessage = "AEMFLOW_E_NULL_OBJECT";								//break;
					case ER_AEMFLOW_E_DLL_LOAD_FAIL: errorCodeMessage = "AEMFLOW_E_DLL_LOAD_FAIL";								//break;
					case ER_AEMFLOW_E_DLL_FUNCTION_NOT_FOUND: errorCodeMessage = "AEMFLOW_E_DLL_FUNCTION_NOT_FOUND";					//break;
					case ER_AEMFLOW_E_DLL_FUNCTION_ERROR: errorCodeMessage = "AEMFLOW_E_DLL_FUNCTION_ERROR";						//break;
					}
					ErrorMessage = "[AEM Facility Code 3: AemFlow] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 4 [AemVector]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE04"))
				{
					switch (errorCode)
					{
					case ER_AEMVECTOR_UNSUPPORTED_EXT: errorCodeMessage = "AEMVECTOR_UNSUPPORTED_EXT";							//break;
					case ER_AEMVECTOR_SYNTAX_ERROR: errorCodeMessage = "AEMVECTOR_SYNTAX_ERROR";								//break;
					case ER_AEMVECTOR_FILE_CORRUPTED: errorCodeMessage = "AEMVECTOR_FILE_CORRUPTED";							//break;
					case ER_AEMVECTOR_CHECKSUM_ERROR: errorCodeMessage = "AEMVECTOR_CHECKSUM_ERROR";							//break;
					case ER_AEMVECTOR_LOADDLL_FAIL: errorCodeMessage = "AEMVECTOR_LOADDLL_FAIL";								//break;
					case ER_AEMVECTOR_LOADFUNCTION_FAIL: errorCodeMessage = "AEMVECTOR_LOADFUNCTION_FAIL";							//break;
					case ER_AEMVECTOR_TESTER_EXCEPTION: errorCodeMessage = "AEMVECTOR_TESTER_EXCEPTION";							//break;
					case ER_AEMVECTOR_NOT_FOUND: errorCodeMessage = "AEMVECTOR_NOT_FOUND";									//break;
					case ER_AEMVECTOR_DOBV_NOT_FOUND: errorCodeMessage = "AEMVECTOR_DOBV_NOT_FOUND";							//break;
					case ER_AEMVECTOR_LOAD_DVEC_OVERFLOW: errorCodeMessage = "AEMVECTOR_LOAD_DVEC_OVERFLOW";						//break;
					case ER_AEMVECTOR_INVALID_RESULT: errorCodeMessage = "AEMVECTOR_INVALID_RESULT";							//break;
					case ER_AEMVECTOR_INVALID_ARGUMENT: errorCodeMessage = "AEMVECTOR_INVALID_ARGUMENT";							//break;
					case ER_AEMVECTOR_INVALID_CH: errorCodeMessage = "AEMVECTOR_INVALID_CH";								//break;
					case ER_AEMVECTOR_UNDEFINED_PIN: errorCodeMessage = "AEMVECTOR_UNDEFINED_PIN";								//break;
					case ER_AEMVECTOR_INVALID_HEADER: errorCodeMessage = "AEMVECTOR_INVALID_HEADER";							//break;
					}
					ErrorMessage = "[AEM Facility Code 4: AemVector] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 5 [AemHW]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE05"))
				{
					switch (errorCode)
					{
					case ER_AEMHW_E_CANNOT_WRITE_TO_HW: errorCodeMessage = "AEMHW_E_CANNOT_WRITE_TO_HW";							//break;
					case ER_AEMHW_E_CANNOT_READ_FROM_HW: errorCodeMessage = "AEMHW_E_CANNOT_READ_FROM_HW";							//break;
					case ER_AEMHW_E_COMM_TIMED_OUT: errorCodeMessage = "AEMHW_E_COMM_TIMED_OUT";								//break;
					case ER_AEMHW_E_TEMPERATURE_OVERHEAT: errorCodeMessage = "AEMHW_E_TEMPERATURE_OVERHEAT";						//break;
					case ER_AEMHW_E_FAILED_UPGRADE_FIRMWARE: errorCodeMessage = "AEMHW_E_FAILED_UPGRADE_FIRMWARE";						//break;
					case ER_AEMHW_E_WRONG_MSG_SIZE: errorCodeMessage = "AEMHW_E_WRONG_MSG_SIZE";								//break;
					case ER_AEMHW_E_API_NOT_IMPLEMENTED: errorCodeMessage = "AEMHW_E_API_NOT_IMPLEMENTED";							//break;
					case ER_AEMHW_E_INVALID_CARD_TYPE: errorCodeMessage = "AEMHW_E_INVALID_CARD_TYPE";							//break;
					case ER_AEMHW_E_FIRMWARE_FILE_CHECKSUM_ERROR: errorCodeMessage = "AEMHW_E_FIRMWARE_FILE_CHECKSUM_ERROR";				//break;
					case ER_AEMHW_E_FIRMWARE_FILESIZE_ERROR: errorCodeMessage = "AEMHW_E_FIRMWARE_FILESIZE_ERROR";						//break;
					case ER_AEMHW_E_WAIT4UNLOCK_TIMEOUT: errorCodeMessage = "AEMHW_E_WAIT4UNLOCK_TIMEOUT";							//break;
					case ER_AEMHW_E_WAIT4READY_TIMEOUT: errorCodeMessage = "AEMHW_E_WAIT4READY_TIMEOUT";							//break;
					}
					ErrorMessage = "[AEM Facility Code 5: AemHW] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 6 [AemDig]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE06"))
				{
					switch (errorCode)
					{
					case ER_AEMDIG_E_READ_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_READ_EPCS_FAIL";								//break;
					case ER_AEMDIG_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_ERASE_EPCS_FAIL";							//break;
					case ER_AEMDIG_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_WRITE_EPCS_FAIL";							//break;
					case ER_AEMDIG_E_OPEN_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_OPEN_EPCS_FAIL";								//break;
					case ER_AEMDIG_E_WRITE_EEPROM_FAIL: errorCodeMessage = "AEMDIG_E_WRITE_EEPROM_FAIL";							//break;
					case ER_AEMDIG_E_READ_EEPROM_FAIL: errorCodeMessage = "AEMDIG_E_READ_EEPROM_FAIL";							//break;
					case ER_AEMDIG_E_VEC_VARNUM_NOT_MATCH: errorCodeMessage = "AEMDIG_E_VEC_VARNUM_NOT_MATCH";						//break;
					case ER_AEMDIG_E_HA_ERASE_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_HA_ERASE_EPCS_FAIL";							//break;
					case ER_AEMDIG_E_HA_WRITE_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_HA_WRITE_EPCS_FAIL";							//break;
					case ER_AEMDIG_E_HA_READ_EPCS_FAIL: errorCodeMessage = "AEMDIG_E_HA_READ_EPCS_FAIL";							//break;
					case ER_AEMDIG_E_HA_ERASE_UFM_FAIL: errorCodeMessage = "AEMDIG_E_HA_ERASE_UFM_FAIL";							//break;
					case ER_AEMDIG_E_HA_WRITE_UFM_FAIL: errorCodeMessage = "AEMDIG_E_HA_WRITE_UFM_FAIL";							//break;
					case ER_AEMDIG_E_HA_READ_UFM_FAIL: errorCodeMessage = "AEMDIG_E_HA_READ_UFM_FAIL";							//break;
					case ER_AEMDIG_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMDIG_E_FIRMWARE_FILE_NOT_FOUND";					//break;
					case ER_AEMDIG_E_COMM_TIMED_OUT: errorCodeMessage = "AEMDIG_E_COMM_TIMED_OUT";								//break;
					case ER_AEMDIG_E_FREQ_MEAS_TIMED_OUT: errorCodeMessage = "AEMDIG_E_FREQ_MEAS_TIMED_OUT";						//break;
					case ER_AEMDIG_E_DC_MEAS_TIMED_OUT: errorCodeMessage = "AEMDIG_E_DC_MEAS_TIMED_OUT";							//break;
					case ER_AEMDIG_E_UPGRADE_TOOL_OUTDATED: errorCodeMessage = "AEMDIG_E_UPGRADE_TOOL_OUTDATED";						//break;
					case ER_AEMDIG_E_RESOURCE_UNAVAILABLE: errorCodeMessage = "AEMDIG_E_RESOURCE_UNAVAILABLE";						//break;
					case ER_AEMDIG_E_RESOURCE_ALREADY_RELEASED: errorCodeMessage = "AEMDIG_E_RESOURCE_ALREADY_RELEASED";					//break;
					case ER_AEMDIG_E_COMM_BUS_OCCUPIED: errorCodeMessage = "AEMDIG_E_COMM_BUS_OCCUPIED";							//break;
					case ER_AEMDIG_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMDIG_E_CAL_HEADER_NOT_FOUND";						//break;
					case ER_AEMDIG_E_CAL_SIZE_INVALID: errorCodeMessage = "AEMDIG_E_CAL_SIZE_INVALID";							//break;
					case ER_AEMDIG_E_CAL_DATA_OUT_OF_RANGE: errorCodeMessage = "AEMDIG_E_CAL_DATA_OUT_OF_RANGE";						//break;
					case ER_AEMDIG_E_JTAG_INVALID_SIZE: errorCodeMessage = "AEMDIG_E_JTAG_INVALID_SIZE";							//break;
					case ER_AEMDIG_E_JTAG_INVALID_COMMAND: errorCodeMessage = "AEMDIG_E_JTAG_INVALID_COMMAND";						//break;
					case ER_AEMDIG_E_JTAG_NOT_DONE: errorCodeMessage = "AEMDIG_E_JTAG_NOT_DONE";								//break;
					}
					ErrorMessage = "[AEM Facility Code 6: AemDig] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 7 [AemSmu & AemDCPwr]
				**	----------------------------------------------------------------------------------------------------
				*/
#if SMU_ENABLE 
				else if (errorCode.ToString("X")->StartsWith("AE07"))
				{
					switch (errorCode)
					{
						// 7-AemSmu
					case ER_AEMSMU_E_CURRENT_OVERDRIVE: errorCodeMessage = "AEMSMU_E_CURRENT_OVERDRIVE";							//break;
					case ER_AEMSMU_E_COMM_TIMED_OUT: errorCodeMessage = "AEMSMU_E_COMM_TIMED_OUT";								//break;
					case ER_AEMSMU_E_INTERLOCKOFF: errorCodeMessage = "AEMSMU_E_INTERLOCKOFF";								//break;
					case ER_AEMSMU_E_SMUOFF: errorCodeMessage = "AEMSMU_E_SMUOFF";										//break;
					case ER_AEMSMU_E_SMU_NOT_FOUND: errorCodeMessage = "AEMSMU_E_SMU_NOT_FOUND";								//break;
					case ER_AEMSMU_E_MEASCOUNT_OVERFLOW: errorCodeMessage = "AEMSMU_E_MEASCOUNT_OVERFLOW";							//break;
					case ER_AEMSMU_E_VEC_VARNUM_NOT_MATCH: errorCodeMessage = "AEMSMU_E_VEC_VARNUM_NOT_MATCH";						//break;
					case ER_AEMSMU_E_WRONG_PIN_NUM: errorCodeMessage = "AEMSMU_E_WRONG_PIN_NUM";								//break;
					case ER_AEMSMU_E_RDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_RDAC_TIMED_OUT";								//break;
					case ER_AEMSMU_E_OPDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_OPDAC_TIMED_OUT";							//break;
					case ER_AEMSMU_E_WRONG_DAC_CH: errorCodeMessage = "AEMSMU_E_WRONG_DAC_CH";								//break;
					case ER_AEMSMU_E_OP_DAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_OP_DAC_TIMED_OUT";							//break;
					case ER_AEMSMU_E_MISC_DAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_MISC_DAC_TIMED_OUT";							//break;
					case ER_AEMSMU_E_EXTIN_MDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_EXTIN_MDAC_TIMED_OUT";						//break;
					case ER_AEMSMU_E_ADC0_TIMED_OUT: errorCodeMessage = "AEMSMU_E_ADC0_TIMED_OUT";								//break;
					case ER_AEMSMU_E_ADC1_TIMED_OUT: errorCodeMessage = "AEMSMU_E_ADC1_TIMED_OUT";								//break;
					case ER_AEMSMU_E_ADC2_TIMED_OUT: errorCodeMessage = "AEMSMU_E_ADC2_TIMED_OUT";								//break;
					case ER_AEMSMU_E_EPCS_SE_TIMED_OUT: errorCodeMessage = "AEMSMU_E_EPCS_SE_TIMED_OUT";							//break;
					case ER_AEMSMU_E_EPCS_RD_TIMED_OUT: errorCodeMessage = "AEMSMU_E_EPCS_RD_TIMED_OUT";							//break;
					case ER_AEMSMU_E_EPCS_WR_TIMED_OUT: errorCodeMessage = "AEMSMU_E_EPCS_WR_TIMED_OUT";							//break;
					case ER_AEMSMU_E_AWG_TIMED_OUT: errorCodeMessage = "AEMSMU_E_AWG_TIMED_OUT";								//break;
					case ER_AEMSMU_E_WRONG_CALTYPE: errorCodeMessage = "AEMSMU_E_WRONG_CALTYPE";								//break;
					case ER_AEMSMU_E_ERASE_FLASH_FAIL: errorCodeMessage = "AEMSMU_E_ERASE_FLASH_FAIL";							//break;
					case ER_AEMSMU_E_WRITE_FLASH_FAIL: errorCodeMessage = "AEMSMU_E_WRITE_FLASH_FAIL";							//break;
					case ER_AEMSMU_E_READ_FLASH_FAIL: errorCodeMessage = "AEMSMU_E_READ_FLASH_FAIL";							//break;
					case ER_AEMSMU_E_WRONG_VI_READ: errorCodeMessage = "AEMSMU_E_WRONG_VI_READ";								//break;
					case ER_AEMSMU_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMSMU_E_FIRMWARE_FILE_NOT_FOUND";					//break;
					case ER_AEMSMU_E_FIRMWARE_FILE_WRONG_SIZE: errorCodeMessage = "AEMSMU_E_FIRMWARE_FILE_WRONG_SIZE";					//break;
					case ER_AEMSMU_E_VDET_MDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_VDET_MDAC_TIMED_OUT";						//break;
					case ER_AEMSMU_E_IDET_MDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_IDET_MDAC_TIMED_OUT";						//break;
					case ER_AEMSMU_E_VBUF_HI_MDAC_TIMED_OUT: errorCodeMessage = "AEMSMU_E_VBUF_HI_MDAC_TIMED_OUT";						//break;
					case ER_AEMSMU_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMSMU_E_CAL_HEADER_NOT_FOUND";						//break;
					case ER_AEMSMU_E_CAL_SIZE_NOT_MATCH: errorCodeMessage = "AEMSMU_E_CAL_SIZE_NOT_MATCH";							//break;
					case ER_AEMSMU_E_INVALID_BOOST_STEP: errorCodeMessage = "AEMSMU_E_INVALID_BOOST_STEP";							//break;
					case ER_AEMSMU_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMSMU_E_WRONG_FIRMWARE_FILE_USED";					//break;
					case ER_AEMSMU_E_COMMON_MODE_CAL_OUT_OF_RANGE: errorCodeMessage = "AEMSMU_E_COMMON_MODE_CAL_OUT_OF_RANGE";				//break;
					case ER_AEMSMU_E_BANDWIDTH_OUT_OF_RANGE: errorCodeMessage = "AEMSMU_E_BANDWIDTH_OUT_OF_RANGE";						//break;
					case ER_AEMSMU_E_NPLC_TOO_BIG_WARNING: errorCodeMessage = "AEMSMU_E_NPLC_TOO_BIG_WARNING";						//break;
					case ER_AEMSMU_E_CAL_FILE_NOT_FOUND: errorCodeMessage = "AEMSMU_E_CAL_FILE_NOT_FOUND";							//break;
					case ER_AEMSMU_E_INVALID_CAL_FILE: errorCodeMessage = "AEMSMU_E_INVALID_CAL_FILE";							//break;
					case ER_AEMSMU_E_INVALID_RELAY_SIZE: errorCodeMessage = "AEMSMU_E_INVALID_RELAY_SIZE";							//break;
					case ER_AEMSMU_E_INVALID_HEADER: errorCodeMessage = "AEMSMU_E_INVALID_HEADER";								//break;
					case ER_AEMSMU_E_LOAD_CAL_FAIL: errorCodeMessage = "AEMSMU_E_LOAD_CAL_FAIL";								//break;
					case ER_AEMSMU_E_BW_TOO_HIGH_WARNING: errorCodeMessage = "AEMSMU_E_BW_TOO_HIGH_WARNING";						//break;
					case ER_AEMSMU_E_BW_TOO_LOW_WARNING: errorCodeMessage = "AEMSMU_E_BW_TOO_LOW_WARNING";							//break;
					case ER_AEMSMU_E_OVER_TEMPERATURE: errorCodeMessage = "AEMSMU_E_OVER_TEMPERATURE";							//break;
					case ER_AEMSMU_E_RESOURCE_OCCUPIED: errorCodeMessage = "AEMSMU_E_RESOURCE_OCCUPIED";							//break;
					case ER_AEMSMU_E_RESOURCE_NOT_READY: errorCodeMessage = "AEMSMU_E_RESOURCE_NOT_READY";							//break;
					case ER_AEMSMU_E_HS_PROTECTION_ENGAGE_OR_LV_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HS_PROTECTION_ENGAGE_OR_LV_NOT_FOUND";		//break;
																																					//case ER_AEMSMU_E_LV_NOT_FOUND								: errorCodeMessage = "AEMSMU_E_LV_NOT_FOUND";								//break;
					case ER_AEMSMU_E_HV_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HV_NOT_FOUND";								//break;
					case ER_AEMSMU_E_HAM_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HAM_NOT_FOUND";								//break;
					case ER_AEMSMU_E_DTM130_NOT_FOUND: errorCodeMessage = "AEMSMU_E_DTM130_NOT_FOUND";							//break;
					case ER_AEMSMU_E_SCOPE_DATA_INVALID: errorCodeMessage = "AEMSMU_E_SCOPE_DATA_INVALID";							//break;
					case ER_AEMSMU_E_HAM320_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HAM320_NOT_FOUND";							//break;
					case ER_AEMSMU_E_HAM330_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HAM330_NOT_FOUND";							//break;
					case ER_AEMSMU_E_HAM340_NOT_FOUND: errorCodeMessage = "AEMSMU_E_HAM340_NOT_FOUND";							//break;
					case ER_AEMSMU_E_SITE01_NOT_FOUND: errorCodeMessage = "AEMSMU_E_SITE01_NOT_FOUND";							//break;
					case ER_AEMSMU_E_SITE23_NOT_FOUND: errorCodeMessage = "AEMSMU_E_SITE23_NOT_FOUND";							//break;
					case ER_AEMSMU_E_OVER_POWER_LIMIT: errorCodeMessage = "AEMSMU_E_OVER_POWER_LIMIT";							//break;
					case ER_AEMSMU_E_COMM_ERROR: errorCodeMessage = "AEMSMU_E_COMM_ERROR";									//break;
					case ER_AEMSMU_E_SPI_RDLENGTH_MISTMATCH: errorCodeMessage = "AEMSMU_E_SPI_RDLENGTH_MISTMATCH";						//break;
					case ER_AEMSMU_E_INVALID_CAL_ACCUM_COUNT: errorCodeMessage = "AEMSMU_E_INVALID_CAL_ACCUM_COUNT";					//break;
					case ER_AEMSMU_E_INVALID_CAL_DATE: errorCodeMessage = "AEMSMU_E_INVALID_CAL_DATE";							//break;
					case ER_AEMSMU_E_INVALID_CAL_TIME: errorCodeMessage = "AEMSMU_E_INVALID_CAL_TIME";							//break;
					case ER_AEMSMU_E_INVALID_CAL_TEMPERATURE: errorCodeMessage = "AEMSMU_E_INVALID_CAL_TEMPERATURE";					//break;
					case ER_AEMSMU_E_INVALID_CAL_INTERVAL_MONTHS: errorCodeMessage = "AEMSMU_E_INVALID_CAL_INTERVAL_MONTHS";				//break;
					case ER_AEMSMU_E_CAL_DATA_OUT_OF_RANGE: errorCodeMessage = "AEMSMU_E_CAL_DATA_OUT_OF_RANGE";						//break;
					case ER_AEMSMU_E_MODEL_NOT_MATCH: errorCodeMessage = "AEMSMU_E_MODEL_NOT_MATCH";							//break;
					case ER_AEMSMU_E_CASCADE_MODE_ERROR: errorCodeMessage = "AEMSMU_E_CASCADE_MODE_ERROR";							//break;
					case ER_AEMSMU_E_MODEL_NOT_SUPPORTED: errorCodeMessage = "AEMSMU_E_MODEL_NOT_SUPPORTED";						//break;
					case ER_AEMSMU_E_PULSE_DURATION_ERROR: errorCodeMessage = "AEMSMU_E_PULSE_DURATION_ERROR";						//break;
					case ER_AEMSMU_E_AIBANDWIDTH_INVALID_LOAD: errorCodeMessage = "AEMSMU_E_AIBANDWIDTH_INVALID_LOAD";					//break;
					case ER_AEMSMU_E_AIBANDWIDTH_INVALID_SETTLING_TIME: errorCodeMessage = "AEMSMU_E_AIBANDWIDTH_INVALID_SETTLING_TIME";			//break;
					case ER_AEMSMU_E_PULSE_VALUE_ERROR: errorCodeMessage = "AEMSMU_E_PULSE_VALUE_ERROR";							//break;
					case ER_AEMSMU_E_ADAPTER_BOARD_NOT_DETECTED: errorCodeMessage = "AEMSMU_E_ADAPTER_BOARD_NOT_DETECTED";					//break;
					case ER_AEMSMU_E_INVALID_SETTING: errorCodeMessage = "AEMSMU_E_INVALID_SETTING";							//break;
					case ER_AEMSMU_E_INVALID_INTERVAL: errorCodeMessage = "AEMSMU_E_INVALID_INTERVAL";							//break;
					case ER_AEMSMU_E_PULSE_OPERATION_IS_ARMED: errorCodeMessage = "AEMSMU_E_PULSE_OPERATION_IS_ARMED	";					//break;
					case ER_AEMSMU_E_PULSE_OPERATION_IS_UNDONE: errorCodeMessage = "AEMSMU_E_PULSE_OPERATION_IS_UNDONE";					//break;
					case ER_AEMSMU_E_INVALID_BW_FV: errorCodeMessage = "AEMSMU_E_INVALID_BW_FV";								//break;
					case ER_AEMSMU_E_INVALID_BW_FI: errorCodeMessage = "AEMSMU_E_INVALID_BW_FI";								//break;
					case ER_AEMSMU_E_DPS_NOT_FOUND: errorCodeMessage = "AEMSMU_E_DPS_NOT_FOUND";								//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MV: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MV";							//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_LSB: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_LSB";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MSB_D13: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MSB_D13";					//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MSB_D14: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MSB_D14";					//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MSB_D15: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MSB_D15";					//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_OFFDAC: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_OFFDAC";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_GAINDAC: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_GAINDAC";					//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_FV: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_FV";							//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_ADJ0: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_ADJ0";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_ADJ1: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_ADJ1";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_ADJ2: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_ADJ2";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_LADJ3: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_LADJ3";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_HADJ3: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_HADJ3";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_LADJ4: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_LADJ4";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_HADJ4: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_HADJ4";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_ADJ5: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_ADJ5";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MISRC: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MISRC";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_MISNK: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_MISNK";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_CISRC: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_CISRC";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_CISNK: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_CISNK";						//break;
					case ER_AEMSMU_E_DPS_INVALID_CAL_FI: errorCodeMessage = "AEMSMU_E_DPS_INVALID_CAL_FI";							//break;
					case ER_AEMSMU_E_PROTECTION_ACTIVATED: errorCodeMessage = "AEMSMU_E_PROTECTION_ACTIVATED";						//break;
					case ER_AEMSMU_E_OVI_PROTECTION_LATCHED: errorCodeMessage = "AEMSMU_E_OVI_PROTECTION_LATCHED";						//break;
					case ER_AEMSMU_E_BAD_ARGUMENTS: errorCodeMessage = "AEMSMU_E_BAD_ARGUMENTS";								//break;
					case ER_AEMSMU_E_INVALID_APERTURE_TIME_UNIT: errorCodeMessage = "AEMSMU_E_INVALID_APERTURE_TIME_UNIT";					//break;
					case ER_AEMSMU_E_INVALID_ARGUMENTS: errorCodeMessage = "AEMSMU_E_INVALID_ARGUMENTS";							//break;
					case ER_AEMSMU_E_DLL_NOT_FOUND: errorCodeMessage = "AEMSMU_E_DLL_NOT_FOUND";								//break;
					case ER_AEMSMU_E_API_NOT_FOUND: errorCodeMessage = "AEMSMU_E_API_NOT_FOUND";								//break;
					case ER_AEMSMU_E_CARD_NOT_FOUND: errorCodeMessage = "AEMSMU_E_CARD_NOT_FOUND";								//break;
					case ER_AEMSMU_E_OPEN_FAIL: errorCodeMessage = "AEMSMU_E_OPEN_FAIL";									//break;
					case ER_AEMSMU_E_SYS_ID_NOT_FOUND: errorCodeMessage = "AEMSMU_E_SYS_ID_NOT_FOUND";							//break;
					case ER_AEMSMU_E_FIRMWARE_FILE_CHECKSUM_ERROR: errorCodeMessage = "AEMSMU_E_FIRMWARE_FILE_CHECKSUM_ERROR";				//break;
					case ER_AEMSMU_E_INVALID_ACTION: errorCodeMessage = "AEMSMU_E_INVALID_ACTION";								//break;
					case ER_AEMSMU_RESERVED1: errorCodeMessage = "AEMSMU_RESERVED1";									//break;
					case ER_AEMSMU_RESERVED2: errorCodeMessage = "AEMSMU_RESERVED2";									//break;
					case ER_AEMSMU_RESERVED3: errorCodeMessage = "AEMSMU_RESERVED3";									//break;
					case ER_AEMSMU_E_EXTTRIGARM_TIMEOUT: errorCodeMessage = "AEMSMU_E_EXTTRIGARM_TIMEOUT";							//break;
					case ER_AEMSMU_E_EXTTRIGARM_ARMED: errorCodeMessage = "AEMSMU_E_EXTTRIGARM_ARMED";							//break;
					case ER_AEMSMU_E_EXTTRIGGET_BUSY: errorCodeMessage = "AEMSMU_E_EXTTRIGGET_BUSY";							//break;
					case ER_AEMSMU_E_READCURR10x_RANGE_NOT_SUPPORTED: errorCodeMessage = "AEMSMU_E_READCURR10x_RANGE_NOT_SUPPORTED";			//break;
																																				// 7-AemDCPwr
					case ER_AEMDCPWR_E_DCLC_DEBUG_IDLE: errorCodeMessage = "AEMDCPWR_E_DCLC_DEBUG_IDLE";							//break;
					case ER_AEMDCPWR_E_DCLC_DEBUG_BUSY: errorCodeMessage = "AEMDCPWR_E_DCLC_DEBUG_BUSY";							//break;
					case ER_AEMDCPWR_E_EXCEED_NULL_LIMIT: errorCodeMessage = "AEMDCPWR_E_EXCEED_NULL_LIMIT";						//break;
					case ER_AEMDCPWR_E_INVALID_CHANNEL: errorCodeMessage = "AEMDCPWR_E_INVALID_CHANNEL";							//break;
					case ER_AEMDCPWR_E_INVALID_CURRENT_LEVEL: errorCodeMessage = "AEMDCPWR_E_INVALID_CURRENT_LEVEL";					//break;
					case ER_AEMDCPWR_E_INVALID_CURRENT_LIMIT: errorCodeMessage = "AEMDCPWR_E_INVALID_CURRENT_LIMIT";					//break;
					case ER_AEMDCPWR_E_INVALID_VOLTAGE_LEVEL: errorCodeMessage = "AEMDCPWR_E_INVALID_VOLTAGE_LEVEL";					//break;
					case ER_AEMDCPWR_E_INVALID_VOLTAGE_LIMIT: errorCodeMessage = "AEMDCPWR_E_INVALID_VOLTAGE_LIMIT";					//break;
					case ER_AEMDCPWR_E_INVALID_VOLTAGE_RANGE: errorCodeMessage = "AEMDCPWR_E_INVALID_VOLTAGE_RANGE";					//break;
					case ER_AEMDCPWR_E_INVALID_CURRENT_RANGE: errorCodeMessage = "AEMDCPWR_E_INVALID_CURRENT_RANGE";					//break;
					case ER_AEMDCPWR_E_EXCEED_CHANNEL_LIMIT: errorCodeMessage = "AEMDCPWR_E_EXCEED_CHANNEL_LIMIT";						//break;
					case ER_AEMDCPWR_E_INVALID_ACTION: errorCodeMessage = "AEMDCPWR_E_INVALID_ACTION";							//break;
					case ER_AEMDCPWR_E_INVALID_OUTPUT_STATE: errorCodeMessage = "AEMDCPWR_E_INVALID_OUTPUT_STATE";						//break;
					case ER_AEMDCPWR_E_INVALID_POWER_LINE_FREQUENCY: errorCodeMessage = "AEMDCPWR_E_INVALID_POWER_LINE_FREQUENCY";				//break;
					case ER_AEMDCPWR_E_INVALID_OUTPUT_FUNCTION: errorCodeMessage = "AEMDCPWR_E_INVALID_OUTPUT_FUNCTION";					//break;
					case ER_AEMDCPWR_E_INVALID_SENSE: errorCodeMessage = "AEMDCPWR_E_INVALID_SENSE";							//break;
					case ER_AEMDCPWR_E_INVALID_CHANNEL_COUNT: errorCodeMessage = "AEMDCPWR_E_INVALID_CHANNEL_COUNT";					//break;
					case ER_AEMDCPWR_E_INVALID_CHANNEL_NAME: errorCodeMessage = "AEMDCPWR_E_INVALID_CHANNEL_NAME";						//break;
					case ER_AEMDCPWR_E_INVALID_MEASUREMENT_TYPE: errorCodeMessage = "AEMDCPWR_E_INVALID_MEASUREMENT_TYPE";					//break;
					case ER_AEMDCPWR_E_INVALID_SOURCE_MODE: errorCodeMessage = "AEMDCPWR_E_INVALID_SOURCE_MODE";						//break;
					case ER_AEMDCPWR_E_INVALID_APERTURE_TIME_UNIT: errorCodeMessage = "AEMDCPWR_E_INVALID_APERTURE_TIME_UNIT";				//break;
					case ER_AEMDCPWR_E_INVALID_EDGE_SETTING: errorCodeMessage = "AEMDCPWR_E_INVALID_EDGE_SETTING";						//break;
					case ER_AEMDCPWR_E_INVALID_TIMEOUT_VALUE: errorCodeMessage = "AEMDCPWR_E_INVALID_TIMEOUT_VALUE";					//break;
					case ER_AEMDCPWR_E_FETCHMULTIPLE_TIMEOUT: errorCodeMessage = "AEMDCPWR_E_FETCHMULTIPLE_TIMEOUT";					//break;
					case ER_AEMDCPWR_E_INVALID_OUTPUT_TRANSIENT: errorCodeMessage = "AEMDCPWR_E_INVALID_OUTPUT_TRANSIENT";					//break;
					case ER_AEMDCPWR_E_INVALID_OUTPUT_RESISTANCE: errorCodeMessage = "AEMDCPWR_E_INVALID_OUTPUT_RESISTANCE";				//break;
					case ER_AEMDCPWR_E_INVALID_ACQUIRE_LENGTH: errorCodeMessage = "AEMDCPWR_E_INVALID_ACQUIRE_LENGTH";					//break;
					case ER_AEMDCPWR_E_INVALID_ACQUIRE_DELTA_TIME: errorCodeMessage = "AEMDCPWR_E_INVALID_ACQUIRE_DELTA_TIME";				//break;
					case ER_AEMDCPWR_E_INVALID_MULTI_SITE_MODE: errorCodeMessage = "AEMDCPWR_E_INVALID_MULTI_SITE_MODE";					//break;
					case ER_AEMDCPWR_E_SMU_OFF: errorCodeMessage = "AEMDCPWR_E_SMU_OFF";									//break;
					case ER_AEMDCPWR_E_AIBANDWIDTH_OUT_OF_RANGE: errorCodeMessage = "AEMDCPWR_E_AIBANDWIDTH_OUT_OF_RANGE";					//break;
					case ER_AEMDCPWR_E_INVALID_SSRAM_ADDRESS: errorCodeMessage = "AEMDCPWR_E_INVALID_SSRAM_ADDRESS";					//break;
					case ER_AEMDCPWR_E_INVALID_MEASURE_MODE: errorCodeMessage = "AEMDCPWR_E_INVALID_MEASURE_MODE";						//break;
					case ER_AEMDCPWR_E_EXCEED_AWG_SIZE_LIMIT: errorCodeMessage = "AEMDCPWR_E_EXCEED_AWG_SIZE_LIMIT";					//break;
					case ER_AEMDCPWR_E_INVALID_AWG_FREQUENCY: errorCodeMessage = "AEMDCPWR_E_INVALID_AWG_FREQUENCY";					//break;
					case ER_AEMDCPWR_E_INVALID_AWG_CYCLE: errorCodeMessage = "AEMDCPWR_E_INVALID_AWG_CYCLE";						//break;
					case ER_AEMDCPWR_E_AWG_BUSY: errorCodeMessage = "AEMDCPWR_E_AWG_BUSY";									//break;
					case ER_AEMDCPWR_E_INVALID_AWG_MODE: errorCodeMessage = "AEMDCPWR_E_INVALID_AWG_MODE";							//break;
					case ER_AEMDCPWR_E_EXCEED_LIMIT: errorCodeMessage = "AEMDCPWR_E_EXCEED_LIMIT";								//break;
					}
					//ret = glob->smu[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcSMU[siteIndex], errorCode, ErrorMessage);
					ErrorMessage = "[AEM Facility Code 7: AemSmu & AemDCPwr] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif 
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 8 [AemIom]
				**	----------------------------------------------------------------------------------------------------
				*/
#if DIO_ENABLE 
				else if (errorCode.ToString("X")->StartsWith("AE08"))
				{
					switch (errorCode)
					{
					case ER_AEMIOM_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMIOM_E_ERASE_EPCS_FAIL";							//break;
					case ER_AEMIOM_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMIOM_E_WRITE_EPCS_FAIL";							//break;
					case ER_AEMIOM_E_READ_EPCS_FAIL: errorCodeMessage = "AEMIOM_E_READ_EPCS_FAIL";								//break;
					case ER_AEMIOM_E_IOM_NOT_FOUND: errorCodeMessage = "AEMIOM_E_IOM_NOT_FOUND";								//break;
					case ER_AEMIOM_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMIOM_E_FIRMWARE_FILE_NOT_FOUND";					//break;
					case ER_AEMIOM_E_INVALID_PORT: errorCodeMessage = "AEMIOM_E_INVALID_PORT";								//break;
					case ER_AEMIOM_E_INVALID_PIN: errorCodeMessage = "AEMIOM_E_INVALID_PIN";								//break;
					case ER_AEMIOM_E_INVALID_OPTION: errorCodeMessage = "AEMIOM_E_INVALID_OPTION";								//break;
					case ER_AEMIOM_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMIOM_E_WRONG_FIRMWARE_FILE_USED";					//break;
					case ER_AEMIOM_E_COMM_TIMED_OUT: errorCodeMessage = "AEMIOM_E_COMM_TIMED_OUT";								//break;
					case ER_AEMIOM_E_INVALID_MULTI_SITE_MODE: errorCodeMessage = "AEMIOM_E_INVALID_MULTI_SITE_MODE";					//break;
					case ER_AEMIOM_E_INVALID_SPI_CHSEL: errorCodeMessage = "AEMIOM_E_INVALID_SPI_CHSEL";							//break;
					case ER_AEMIOM_E_INVALID_I2C_CHSEL: errorCodeMessage = "AEMIOM_E_INVALID_I2C_CHSEL";							//break;
					case ER_AEMIOM_E_INVALID_HEADER: errorCodeMessage = "AEMIOM_E_INVALID_HEADER";								//break;
					case ER_AEMIOM_E_VE_TIMED_OUT: errorCodeMessage = "AEMIOM_E_VE_TIMED_OUT";								//break;
					case ER_AEMIOM_E_INVALID_SPI_CORE: errorCodeMessage = "AEMIOM_E_INVALID_SPI_CORE";							//break;
					case ER_AEMIOM_E_INVALID_I2C_CORE: errorCodeMessage = "AEMIOM_E_INVALID_I2C_CORE";							//break;
					case ER_AEMIOM_E_FILE_NOT_FOUND: errorCodeMessage = "AEMIOM_E_FILE_NOT_FOUND";								//break;
					case ER_AEMIOM_E_FILE_CORRUPTED: errorCodeMessage = "AEMIOM_E_FILE_CORRUPTED";								//break;
					case ER_AEMIOM_E_BAD_ARGUMENTS: errorCodeMessage = "AEMIOM_E_BAD_ARGUMENTS";								//break;
					}
					//ret = glob->iom[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcIOM[siteIndex], errorCode, ErrorMessage);
					ErrorMessage = "[AEM Facility Code 8: AemIom] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 9 [AemPXIe]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE09"))
				{
					switch (errorCode)
					{
					case ER_AEMPXIE_E_OPEN_FAIL: errorCodeMessage = "AEMPXIE_E_OPEN_FAIL";									//break;
					case ER_AEMPXIE_E_DLL_NOT_FOUND: errorCodeMessage = "AEMPXIE_E_DLL_NOT_FOUND";								//break;
					case ER_AEMPXIE_E_API_NOT_FOUND: errorCodeMessage = "AEMPXIE_E_API_NOT_FOUND";								//break;
					case ER_AEMPXIE_E_INVALID_MULTI_SITE_MODE: errorCodeMessage = "AEMPXIE_E_INVALID_MULTI_SITE_MODE";					//break;
					case ER_AEMPXIE_E_OPEN_RM_FAIL: errorCodeMessage = "AEMPXIE_E_OPEN_RM_FAIL";								//break;
					case ER_AEMPXIE_E_DMA_READ_TIMEOUT: errorCodeMessage = "AEMPXIE_E_DMA_READ_TIMEOUT";							//break;
					case ER_AEMPXIE_E_INVALID_SUBDID: errorCodeMessage = "AEMPXIE_E_INVALID_SUBDID";							//break;
					case ER_AEMPXIE_E_RESOURCE_NOT_FOUND: errorCodeMessage = "AEMPXIE_E_RESOURCE_NOT_FOUND";						//break;
					case ER_AEMPXIE_E_MODULE_ALREAD_INIT: errorCodeMessage = "AEMPXIE_E_MODULE_ALREAD_INIT";						//break;
					case ER_AEMPXIE_E_API_NOT_FOUND_IN_DLL: errorCodeMessage = "AEMPXIE_E_API_NOT_FOUND_IN_DLL";						//break;
					case ER_AEMPXIE_E_BAD_ARGUMENTS: errorCodeMessage = "AEMPXIE_E_BAD_ARGUMENTS";								//break;
					case ER_AEMPXIE_RESERVED1: errorCodeMessage = "AEMPXIE_RESERVED1";									//break;
					}
					ErrorMessage = "[AEM Facility Code 9: AemPXIe] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 10 [AemDM]
				**	----------------------------------------------------------------------------------------------------
				*/
#if DM_ENABLE	
				else if (errorCode.ToString("X")->Contains("AE0A"))
				{
					switch (errorCode)
					{

					}
					ErrorMessage = "[AEM Facility Code 10: AemDM] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 11 [AemDM]
				**	----------------------------------------------------------------------------------------------------
				*/
#if DM_ENABLE 
				else if (errorCode.ToString("X")->Contains("AE0B"))
				{
					switch (errorCode)
					{
					case ER_AEMDM_E_INVALID_ARGUMENTS: errorCodeMessage = "AEMDM_E_INVALID_ARGUMENTS";								//break;
					case ER_AEMDM_E_INVALID_CALIBRATION_ARGUMENTS: errorCodeMessage = "AEMDM_E_INVALID_CALIBRATION_ARGUMENTS";					//break;
					case ER_AEMDM_E_UNDEFINED_VECTOR_SET: errorCodeMessage = "AEMDM_E_UNDEFINED_VECTOR_SET";							//break;
					case ER_AEMDM_E_CHANNEL_NOT_IN_PMU_STATE: errorCodeMessage = "AEMDM_E_CHANNEL_NOT_IN_PMU_STATE";						//break;
					case ER_AEMDM_E_ADATE_TIMEOUT: errorCodeMessage = "AEMDM_E_ADATE_TIMEOUT";									//break;
					case ER_AEMDM_E_ADC_TIMEOUT: errorCodeMessage = "AEMDM_E_ADC_TIMEOUT";										//break;
					case ER_AEMDM_E_DDS_TIMEOUT: errorCodeMessage = "AEMDM_E_DDS_TIMEOUT";										//break;
					case ER_AEMDM_E_PATTERN_MEMORY_TIMEOUT: errorCodeMessage = "AEMDM_E_PATTERN_MEMORY_TIMEOUT";							//break;
					case ER_AEMDM_E_HISTORY_RAM_TIMEOUT: errorCodeMessage = "AEMDM_E_HISTORY_RAM_TIMEOUT";								//break;
					case ER_AEMDM_E_D2R_FAIL_CALIBRATION_STAGE1: errorCodeMessage = "AEMDM_E_D2R_FAIL_CALIBRATION_STAGE1";						//break;
					case ER_AEMDM_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMDM_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMDM_E_CAL_SIZE_NOT_MATCH: errorCodeMessage = "AEMDM_E_CAL_SIZE_NOT_MATCH";								//break;
					case ER_AEMDM_E_INVALID_CAL_ACCUM_COUNT: errorCodeMessage = "AEMDM_E_INVALID_CAL_ACCUM_COUNT";							//break;
					case ER_AEMDM_E_INVALID_CAL_DATE: errorCodeMessage = "AEMDM_E_INVALID_CAL_DATE";								//break;
					case ER_AEMDM_E_VECTOR_ENGINE_BUSY: errorCodeMessage = "AEMDM_E_VECTOR_ENGINE_BUSY";								//break;
					case ER_AEMDM_E_EXCEED_VECTOR_MEMORY_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_VECTOR_MEMORY_LIMIT";						//break;
					case ER_AEMDM_E_EXCEED_TOTAL_VECTOR_MEMORY_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_TOTAL_VECTOR_MEMORY_LIMIT";				//break;
					case ER_AEMDM_E_DM482E_NOT_FOUND: errorCodeMessage = "AEMDM_E_DM482E_NOT_FOUND";								//break;
					case ER_AEMDM_E_D2R_FAIL_CALIBRATION_STAGE2: errorCodeMessage = "AEMDM_E_D2R_FAIL_CALIBRATION_STAGE2";						//break;
					case ER_AEMDM_E_INPUT_DELAY_OUT_OF_RANGE: errorCodeMessage = "AEMDM_E_INPUT_DELAY_OUT_OF_RANGE";						//break;
					case ER_AEMDM_E_EXCEED_PERIOD_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_PERIOD_LIMIT";								//break;
					case ER_AEMDM_E_EXCEED_READ_VECTOR_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_READ_VECTOR_LIMIT";						//break;
					case ER_AEMDM_E_CHANNEL_NOT_IN_DIO_STATE: errorCodeMessage = "AEMDM_E_CHANNEL_NOT_IN_DIO_STATE";						//break;
					case ER_AEMDM_E_INVALID_DIO_VALUE: errorCodeMessage = "AEMDM_E_INVALID_DIO_VALUE";								//break;
					case ER_AEMDM_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMDM_E_FIRMWARE_FILE_NOT_FOUND";							//break;
					case ER_AEMDM_E_DM_NOT_FOUND: errorCodeMessage = "AEMDM_E_DM_NOT_FOUND";									//break;
					case ER_AEMDM_E_I2C_TIME_OUT: errorCodeMessage = "AEMDM_E_I2C_TIME_OUT";									//break;
					case ER_AEMDM_E_INVALID_TRIGGER_IN: errorCodeMessage = "AEMDM_E_INVALID_TRIGGER_IN";								//break;
					case ER_AEMDM_E_INVALID_TRIGGER_OUT: errorCodeMessage = "AEMDM_E_INVALID_TRIGGER_OUT";								//break;
					case ER_AEMDM_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMDM_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMDM_E_INVALID_INSTRUCTION_CORE: errorCodeMessage = "AEMDM_E_INVALID_INSTRUCTION_CORE";						//break;
					case ER_AEMDM_E_INSTRUCTION_LENGTH_OVERFLOW: errorCodeMessage = "AEMDM_E_INSTRUCTION_LENGTH_OVERFLOW";						//break;
					case ER_AEMDM_E_RECORDING_IN_PROGRESS: errorCodeMessage = "AEMDM_E_RECORDING_IN_PROGRESS";							//break;
					case ER_AEMDM_E_RECORDING_IDLE: errorCodeMessage = "AEMDM_E_RECORDING_IDLE";									//break;
					case ER_AEMDM_E_INVALID_RESULTS_TAG: errorCodeMessage = "AEMDM_E_INVALID_RESULTS_TAG";								//break;
					case ER_AEMDM_E_INVALID_RESULTS_REG: errorCodeMessage = "AEMDM_E_INVALID_RESULTS_REG";								//break;
					case ER_AEMDM_E_INSTRUCTION_LENGTH_ZERO: errorCodeMessage = "AEMDM_E_INSTRUCTION_LENGTH_ZERO";							//break;
					case ER_AEMDM_E_INVALID_SAMPLE_SIZE: errorCodeMessage = "AEMDM_E_INVALID_SAMPLE_SIZE";								//break;
					case ER_AEMDM_E_INVALID_PMU_STATE: errorCodeMessage = "AEMDM_E_INVALID_PMU_STATE";								//break;
					case ER_AEMDM_E_VECTOR_NOT_RUN: errorCodeMessage = "AEMDM_E_VECTOR_NOT_RUN";									//break;
					case ER_AEMDM_E_INVALID_CAL_TIME: errorCodeMessage = "AEMDM_E_INVALID_CAL_TIME";								//break;
					case ER_AEMDM_E_INVALID_CAL_INTERVAL_MONTHS: errorCodeMessage = "AEMDM_E_INVALID_CAL_INTERVAL_MONTHS";						//break;
					case ER_AEMDM_E_INVALID_PE_WRDATA: errorCodeMessage = "AEMDM_E_INVALID_PE_WRDATA";								//break;
					case ER_AEMDM_E_INVALID_PE_WRADDR: errorCodeMessage = "AEMDM_E_INVALID_PE_WRADDR";								//break;
					case ER_AEMDM_E_INVALID_PE_RDADDR: errorCodeMessage = "AEMDM_E_INVALID_PE_RDADDR";								//break;
					case ER_AEMDM_E_INVALID_RANGE: errorCodeMessage = "AEMDM_E_INVALID_RANGE";									//break;
					case ER_AEMDM_E_CAL_FILE_NOT_FOUND: errorCodeMessage = "AEMDM_E_CAL_FILE_NOT_FOUND";								//break;
					case ER_AEMDM_E_INVALID_CAL_FILE: errorCodeMessage = "AEMDM_E_INVALID_CAL_FILE";								//break;
					case ER_AEMDM_E_INVALID_CAL_TEMPERATURE: errorCodeMessage = "AEMDM_E_INVALID_CAL_TEMPERATURE";							//break;
					case ER_AEMDM_E_WR_RAM_TIMEOUT: errorCodeMessage = "AEMDM_E_WR_RAM_TIMEOUT";									//break;
					case ER_AEMDM_E_RD_RAM_TIMEOUT: errorCodeMessage = "AEMDM_E_RD_RAM_TIMEOUT";									//break;
					case ER_AEMDM_E_INVALID_LEVEL: errorCodeMessage = "AEMDM_E_INVALID_LEVEL";									//break;
					case ER_AEMDM_E_INVALID_EDGE_VALUE: errorCodeMessage = "AEMDM_E_INVALID_EDGE_VALUE";								//break;
					case ER_AEMDM_E_INVALID_FORCE_ACTION: errorCodeMessage = "AEMDM_E_INVALID_FORCE_ACTION";							//break;
					case ER_AEMDM_E_INVALID_DRIVE_ACTION: errorCodeMessage = "AEMDM_E_INVALID_DRIVE_ACTION";							//break;
					case ER_AEMDM_E_INVALID_RECEIVE_ACTION: errorCodeMessage = "AEMDM_E_INVALID_RECEIVE_ACTION";							//break;
					case ER_AEMDM_E_CAL_DATA_OUT_OF_RANGE: errorCodeMessage = "AEMDM_E_CAL_DATA_OUT_OF_RANGE";							//break;
					case ER_AEMDM_E_INVALID_INDEX: errorCodeMessage = "AEMDM_E_INVALID_INDEX";									//break;
					case ER_AEMDM_E_INVALID_PIN_NAME: errorCodeMessage = "AEMDM_E_INVALID_PIN_NAME";								//break;
					case ER_AEMDM_E_INVALID_OPCODE_DM481e: errorCodeMessage = "AEMDM_E_INVALID_OPCODE_DM481e";							//break;
					case ER_AEMDM_E_INVALID_WAVESET: errorCodeMessage = "AEMDM_E_INVALID_WAVESET";									//break;
					case ER_AEMDM_E_INVALID_CLK_FREQ: errorCodeMessage = "AEMDM_E_INVALID_CLK_FREQ";								//break;
					case ER_AEMDM_E_INVALID_CH: errorCodeMessage = "AEMDM_E_INVALID_CH";										//break;
					case ER_AEMDM_E_INVALID_MIPI_COMMAND: errorCodeMessage = "AEMDM_E_INVALID_MIPI_COMMAND";							//break;
					case ER_AEMDM_E_INVALID_MIPI_DATA: errorCodeMessage = "AEMDM_E_INVALID_MIPI_DATA";								//break;
					case ER_AEMDM_E_INVALID_MIPI_CONNECT: errorCodeMessage = "AEMDM_E_INVALID_MIPI_CONNECT";							//break;
					case ER_AEMDM_E_INVALID_MIPI_SPEED: errorCodeMessage = "AEMDM_E_INVALID_MIPI_SPEED";								//break;
					case ER_AEMDM_E_MIPI_STATUS_TIMEOUT: errorCodeMessage = "AEMDM_E_MIPI_STATUS_TIMEOUT";								//break;
					case ER_AEMDM_E_INVALID_MIPI_RDDATA_COUNT: errorCodeMessage = "AEMDM_E_INVALID_MIPI_RDDATA_COUNT";						//break;
					case ER_AEMDM_E_INVALID_MIPI_RDDATA: errorCodeMessage = "AEMDM_E_INVALID_MIPI_RDDATA";								//break;
					case ER_AEMDM_E_INVALID_MIPI_DELAY: errorCodeMessage = "AEMDM_E_INVALID_MIPI_DELAY";								//break;
					case ER_AEMDM_E_INVALID_SPI_CHSEL: errorCodeMessage = "AEMDM_E_INVALID_SPI_CHSEL";								//break;
					case ER_AEMDM_E_DIG_BUSY: errorCodeMessage = "AEMDM_E_DIG_BUSY";										//break;
					case ER_AEMDM_E_DIG_IDLE: errorCodeMessage = "AEMDM_E_DIG_IDLE";										//break;
					case ER_AEMDM_E_DIG_INVALID_STATUS: errorCodeMessage = "AEMDM_E_DIG_INVALID_STATUS";								//break;
					case ER_AEMDM_E_DIG_ARMED: errorCodeMessage = "AEMDM_E_DIG_ARMED";										//break;
					case ER_AEMDM_E_FREQ_COUNTER_BUSY: errorCodeMessage = "AEMDM_E_FREQ_COUNTER_BUSY";								//break;
					case ER_AEMDM_E_DIG_TIMEOUT: errorCodeMessage = "AEMDM_E_DIG_TIMEOUT";										//break;
					case ER_AEMDM_E_NOT_ENOUGH_MEMORY: errorCodeMessage = "AEMDM_E_NOT_ENOUGH_MEMORY	";								//break;
					case ER_AEMDM_E_INVALID_I2C_CHSEL: errorCodeMessage = "AEMDM_E_INVALID_I2C_CHSEL";								//break;
					case ER_AEMDM_E_UPGRADE_FIRMWARE_FAIL: errorCodeMessage = "AEMDM_E_UPGRADE_FIRMWARE_FAIL";							//break;
					case ER_AEMDM_E_UPGRADE_FIRMWARE_TIMEOUT: errorCodeMessage = "AEMDM_E_UPGRADE_FIRMWARE_TIMEOUT";						//break;
					case ER_AEMDM_E_DIG_PRETRIG_SAMPLE_INVALID: errorCodeMessage = "AEMDM_E_DIG_PRETRIG_SAMPLE_INVALID";						//break;
					case ER_AEMDM_E_COMM_TIMED_OUT: errorCodeMessage = "AEMDM_E_COMM_TIMED_OUT";									//break;
					case ER_AEMDM_E_BUFFERED_MODULE_INIT_FAIL: errorCodeMessage = "AEMDM_E_BUFFERED_MODULE_INIT_FAIL";						//break;
					case ER_AEMDM_E_RX_FAIL_CALIBRATION: errorCodeMessage = "AEMDM_E_RX_FAIL_CALIBRATION";								//break;
					case ER_AEMDM_E_OUTPUT_DELAY_OUT_OF_RANGE: errorCodeMessage = "AEMDM_E_OUTPUT_DELAY_OUT_OF_RANGE";						//break;
					case ER_AEMDM_E_WRONG_CALTYPE: errorCodeMessage = "AEMDM_E_WRONG_CALTYPE";									//break;
					case ER_AEMDM_E_INVALID_LENGTH: errorCodeMessage = "AEMDM_E_INVALID_LENGTH";									//break;
					case ER_AEMDM_E_MODULE_NOT_SUPPORTED_FOR_THIS_PATFILE: errorCodeMessage = "AEMDM_E_MODULE_NOT_SUPPORTED_FOR_THIS_PATFILE";			//break;
					case ER_AEMDM_E_THIS_PATFILE_VERSION_IS_NOT_SUPPORTED: errorCodeMessage = "AEMDM_E_THIS_PATFILE_VERSION_IS_NOT_SUPPORTED";			//break;
					case ER_AEMDM_E_INVALID_HEADER: errorCodeMessage = "AEMDM_E_INVALID_HEADER";									//break;
					case ER_AEMDM_E_INVALID_RTD_DELAY: errorCodeMessage = "AEMDM_E_INVALID_RTD_DELAY";								//break;
					case ER_AEMDM_E_UNDEFINED_VECTOR_SETS: errorCodeMessage = "AEMDM_E_UNDEFINED_VECTOR_SETS";							//break;
					case ER_AEMDM_E_THIS_PATFILE_HAS_NO_PIN_USING_THIS_MODULE: errorCodeMessage = "AEMDM_E_THIS_PATFILE_HAS_NO_PIN_USING_THIS_MODULE";		//break;
					case ER_AEMDM_E_WARNING_PERIOD_GREATER_THAN_EDGE: errorCodeMessage = "AEMDM_E_WARNING_PERIOD_GREATER_THAN_EDGE";				//break;
					case ER_AEMDM_E_WARNING_EDGE_GREATER_THAN_PERIOD: errorCodeMessage = "AEMDM_E_WARNING_EDGE_GREATER_THAN_PERIOD";				//break;
					case ER_AEMDM_E_TMU_NOT_IN_DONE_STATE: errorCodeMessage = "AEMDM_E_TMU_NOT_IN_DONE_STATE";							//break;
					case ER_AEMDM_E_EXCEED_ALLOCATED_MEMORY: errorCodeMessage = "AEMDM_E_EXCEED_ALLOCATED_MEMORY";							//break;
					case ER_AEMDM_E_CANNOT_OPEN_FILE: errorCodeMessage = "AEMDM_E_CANNOT_OPEN_FILE";								//break;
					case ER_AEMDM_E_INVALID_ATTRIBUTE: errorCodeMessage = "AEMDM_E_INVALID_ATTRIBUTE";								//break;
					case ER_AEMDM_E_TDR_NO_UNDEFINED_REGION_DETECTED: errorCodeMessage = "AEMDM_E_TDR_NO_UNDEFINED_REGION_DETECTED";				//break;
					case ER_AEMDM_E_LOGIC_ANALYZER_BUSY: errorCodeMessage = "AEMDM_E_LOGIC_ANALYZER_BUSY";								//break;
					case ER_AEMDM_E_EXCEED_READ_LOGIC_ANALYZER_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_READ_LOGIC_ANALYZER_LIMIT";				//break;
					case ER_AEMDM_E_VECTOR_FILEPATH_EXCEED_1024B: errorCodeMessage = "AEMDM_E_VECTOR_FILEPATH_EXCEED_1024B";					//break;
					case ER_AEMDM_E_PIN_NAME_IS_USED: errorCodeMessage = "AEMDM_E_PIN_NAME_IS_USED";								//break;
					case ER_AEMDM_E_INVALID_PINGROUP: errorCodeMessage = "AEMDM_E_INVALID_PINGROUP";								//break;
					case ER_AEMDM_E_EXCEED_PIN_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_PIN_LIMIT";								//break;
					case ER_AEMDM_E_EPCS_BUSY: errorCodeMessage = "AEMDM_E_EPCS_BUSY";										//break;
					case ER_AEMDM_E_INSUFFICIENT_ALLOCATED_MEMORY: errorCodeMessage = "AEMDM_E_INSUFFICIENT_ALLOCATED_MEMORY";					//break;
					case ER_AEMDM_E_VEC_ENGINE_NOT_IN_SYNC: errorCodeMessage = "AEMDM_E_VEC_ENGINE_NOT_IN_SYNC";							//break;
					case ER_AEMDM_E_EXCEED_TMU_CORE_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_TMU_CORE_LIMIT";							//break;
					case ER_AEMDM_E_INVALID_PIN_FOR_TMU: errorCodeMessage = "AEMDM_E_INVALID_PIN_FOR_TMU";								//break;
					case ER_AEMDM_E_INVALID_PIN_FOR_LOGIC_ANALYZER: errorCodeMessage = "AEMDM_E_INVALID_PIN_FOR_LOGIC_ANALYZER";					//break;
					case ER_AEMDM_E_LOGIC_ANALYZER_NOT_IN_SYNC: errorCodeMessage = "AEMDM_E_LOGIC_ANALYZER_NOT_IN_SYNC";						//break;
					case ER_AEMDM_E_MC_DC_COMM_TIMEOUT: errorCodeMessage = "AEMDM_E_MC_DC_COMM_TIMEOUT";								//break;
					case ER_AEMDM_E_SRAM_TIMEOUT: errorCodeMessage = "AEMDM_E_SRAM_TIMEOUT";									//break;
					case ER_AEMDM_E_MC_TIMEOUT: errorCodeMessage = "AEMDM_E_MC_TIMEOUT";										//break;
					case ER_AEMDM_E_DM581X_UNSUPPORTED_DC_CONFIGURATION: errorCodeMessage = "AEMDM_E_DM581X_UNSUPPORTED_DC_CONFIGURATION";				//break;
					case ER_AEMDM_E_SRAM_CAL_FAIL: errorCodeMessage = "AEMDM_E_SRAM_CAL_FAIL";									//break;
					case ER_AEMDM_E_EXCEED_DIO_PORT_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_DIO_PORT_LIMIT";							//break;
					case ER_AEMDM_E_ADC_BUSY: errorCodeMessage = "AEMDM_E_ADC_BUSY";										//break;
					case ER_AEMDM_E_ADC_ERROR: errorCodeMessage = "AEMDM_E_ADC_ERROR";										//break;
					case ER_AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN: errorCodeMessage = "AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN";							//break;
					case ER_AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN_PER_DC: errorCodeMessage = "AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN_PER_DC";					//break;
					case ER_AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN_PER_BLOCK: errorCodeMessage = "AEMDM_E_EXCEED_TOTAL_SCAN_CHAIN_PER_BLOCK";				//break;
					case ER_AEMDM_E_NO_IMAGE_CAPTURED: errorCodeMessage = "AEMDM_E_NO_IMAGE_CAPTURED";								//break;
					case ER_AEMDM_E_EXCEED_IMAGE_PROC_DIMENSION: errorCodeMessage = "AEMDM_E_EXCEED_IMAGE_PROC_DIMENSION";						//break;
					case ER_AEMDM_E_EXCEED_IMAGE_CORE_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_IMAGE_CORE_LIMIT";							//break;
					case ER_AEMDM_E_EXCEED_LABEL_LIMIT: errorCodeMessage = "AEMDM_E_EXCEED_LABEL_LIMIT";								//break;
					case ER_AEMDM_E_EXCEED_FRAME_MAX: errorCodeMessage = "AEMDM_E_EXCEED_FRAME_MAX";								//break;
					case ER_AEMDM_E_INVALID_I2C_COMMAND: errorCodeMessage = "AEMDM_E_INVALID_I2C_COMMAND";								//break;
					case ER_AEMDM_E_LOGIC_ANALYZER_WRONG_MODE: errorCodeMessage = "AEMDM_E_LOGIC_ANALYZER_WRONG_MODE";						//break;
					case ER_AEMDM_E_IMAGE_PROC_BUSY: errorCodeMessage = "AEMDM_E_IMAGE_PROC_BUSY";									//break;
					case ER_AEMDM_E_INVALID_LOGIC_ANALYZER_CH: errorCodeMessage = "AEMDM_E_INVALID_LOGIC_ANALYZER_CH";						//break;
					case ER_AEMDM_E_CAL_DATA_MISMATCH: errorCodeMessage = "AEMDM_E_CAL_DATA_MISMATCH";								//break;
					case ER_AEMDM_E_MEMORY_SELFTEST_TIMEOUT: errorCodeMessage = "AEMDM_E_MEMORY_SELFTEST_TIMEOUT";							//break;
					case ER_AEMDM_E_INVALID_CAPTURE_PIN: errorCodeMessage = "AEMDM_E_INVALID_CAPTURE_PIN";								//break;
					case ER_AEMDM_E_INVALID_SOURCE_PIN: errorCodeMessage = "AEMDM_E_INVALID_SOURCE_PIN";								//break;
					case ER_AEMDM_E_TRIGGER_CAL_FAIL: errorCodeMessage = "AEMDM_E_TRIGGER_CAL_FAIL";								//break;
					case ER_AEMDM_E_FINE_DELAY_CAL_ERROR: errorCodeMessage = "AEMDM_E_FINE_DELAY_CAL_ERROR";							//break;
					case ER_AEMDM_E_API_NOT_SUPPORTED_FOR_THIS_MODEL: errorCodeMessage = "AEMDM_E_API_NOT_SUPPORTED_FOR_THIS_MODEL";				//break;
					case ER_AEMDM_E_INVALID_MULTIPLEX_PIN: errorCodeMessage = "AEMDM_E_INVALID_MULTIPLEX_PIN";							//break;
					case ER_AEMDM_E_TRIGGER_PLL_TIMEOUT: errorCodeMessage = "AEMDM_E_TRIGGER_PLL_TIMEOUT";								//break;
					case ER_AEMDM_E_INVALID_PIN: errorCodeMessage = "AEMDM_E_INVALID_PIN";										//break;
					case ER_AEMDM_E_WARNING_TDR_THRESHOLD_OUT_OF_RANGE: errorCodeMessage = "AEMDM_E_WARNING_TDR_THRESHOLD_OUT_OF_RANGE";				//break;
					case ER_AEMDM_E_INVALID_OPCODE: errorCodeMessage = "AEMDM_E_INVALID_OPCODE";									//break;
					case ER_AEMDM_E_INVALID_OPERAND: errorCodeMessage = "AEMDM_E_INVALID_OPERAND";									//break;
					}
					//ret = glob->dm[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcDM[siteIndex], errorCode, ReturnErrorMessage);
					//ErrorMessage = ReturnErrorMessage->ToString();
					ErrorMessage = "[AEM Facility Code 11: AemDM] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif 
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 12 [AemTmu]
				**	----------------------------------------------------------------------------------------------------
				*/
#if TMU_ENABLE 
				else if (errorCode.ToString("X")->StartsWith("AE0C"))
				{
					switch (errorCode)
					{
					case ER_AEMTMU_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMTMU_E_ERASE_EPCS_FAIL";								//break;
					case ER_AEMTMU_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMTMU_E_WRITE_EPCS_FAIL";								//break;
					case ER_AEMTMU_E_READ_EPCS_FAIL: errorCodeMessage = "AEMTMU_E_READ_EPCS_FAIL";									//break;
					case ER_AEMTMU_E_TMU_NOT_FOUND: errorCodeMessage = "AEMTMU_E_TMU_NOT_FOUND";									//break;
					case ER_AEMTMU_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMTMU_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMTMU_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMTMU_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMTMU_E_COMM_TIMED_OUT: errorCodeMessage = "AEMTMU_E_COMM_TIMED_OUT";									//break;
					case ER_AEMTMU_E_ADC_TIMED_OUT: errorCodeMessage = "AEMTMU_E_ADC_TIMED_OUT";									//break;
					case ER_AEMTMU_E_RETRIEVE_RESULTS_TIMED_OUT: errorCodeMessage = "AEMTMU_E_RETRIEVE_RESULTS_TIMED_OUT";						//break;
					case ER_AEMTMU_E_INVALID_RESULTS: errorCodeMessage = "AEMTMU_E_INVALID_RESULTS";								//break;
					case ER_AEMTMU_E_INVALID_TIMEOUT: errorCodeMessage = "AEMTMU_E_INVALID_TIMEOUT";								//break;
					case ER_AEMTMU_E_WRONG_CALTYPE: errorCodeMessage = "AEMTMU_E_WRONG_CALTYPE";									//break;
					case ER_AEMTMU_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMTMU_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMTMU_E_CAL_SIZE_NOT_MATCH: errorCodeMessage = "AEMTMU_E_CAL_SIZE_NOT_MATCH";								//break;
					case ER_AEMTMU_E_CAL_FILE_NOT_FOUND: errorCodeMessage = "AEMTMU_E_CAL_FILE_NOT_FOUND";								//break;
					case ER_AEMTMU_E_INVALID_CAL_FILE: errorCodeMessage = "AEMTMU_E_INVALID_CAL_FILE";								//break;
					case ER_AEMTMU_E_INVALID_CAL_ACCUM_COUNT: errorCodeMessage = "AEMTMU_E_INVALID_CAL_ACCUM_COUNT";						//break;
					case ER_AEMTMU_E_INVALID_CAL_DATE: errorCodeMessage = "AEMTMU_E_INVALID_CAL_DATE";								//break;
					case ER_AEMTMU_E_INVALID_CAL_TIME: errorCodeMessage = "AEMTMU_E_INVALID_CAL_TIME";								//break;
					case ER_AEMTMU_E_INVALID_CAL_TEMPERATURE: errorCodeMessage = "AEMTMU_E_INVALID_CAL_TEMPERATURE";						//break;
					case ER_AEMTMU_E_INVALID_CAL_INTERVAL_MONTHS: errorCodeMessage = "AEMTMU_E_INVALID_CAL_INTERVAL_MONTHS";					//break;
					case ER_AEMTMU_E_CAL_DATA_OUT_OF_RANGE: errorCodeMessage = "AEMTMU_E_CAL_DATA_OUT_OF_RANGE";							//break;
					case ER_AEMTMU_E_INVALID_ATTRIBUTE: errorCodeMessage = "AEMTMU_E_INVALID_ATTRIBUTE";								//break;
					case ER_AEMTMU_E_INVALID_ARGUMENTS: errorCodeMessage = "AEMTMU_E_INVALID_ARGUMENTS";								//break;
					}
					//ret = tmu[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcTMU[siteIndex], errorCode, ReturnErrorMessage);
					//ErrorMessage = ReturnErrorMessage->ToString();
					ErrorMessage = "[AEM Facility Code 12: AemTmu] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x";
				}
#endif 
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 13 [AemSwitch]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE0D"))
				{
					switch (errorCode)
					{
					case ER_AEMSWM_E_SWM_NOT_FOUND: errorCodeMessage = "AEMSWM_E_SWM_NOT_FOUND";									//break;
					case ER_AEMSWM_E_ERASE_FLASH_FAIL: errorCodeMessage = "AEMSWM_E_ERASE_FLASH_FAIL";								//break;
					case ER_AEMSWM_E_WRITE_FLASH_FAIL: errorCodeMessage = "AEMSWM_E_WRITE_FLASH_FAIL";								//break;
					case ER_AEMSWM_E_READ_FLASH_FAIL: errorCodeMessage = "AEMSWM_E_READ_FLASH_FAIL";								//break;
					case ER_AEMSWM_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMSWM_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMSWM_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMSWM_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMSWM_E_COMM_TIMED_OUT: errorCodeMessage = "AEMSWM_E_COMM_TIMED_OUT";									//break;
					case ER_AEMSWM_E_INVALID_HEADER: errorCodeMessage = "AEMSWM_E_INVALID_HEADER";									//break;
					case ER_AEMSWM_E_INVALID_CH_SEL: errorCodeMessage = "AEMSWM_E_INVALID_CH_SEL";									//break;
					case ER_AEMSWM_E_INVALID_SWITCH_VALUE: errorCodeMessage = "AEMSWM_E_INVALID_SWITCH_VALUE";							//break;
					case ER_AEMSWM_E_INVALID_MULTI_SITE_MODE: errorCodeMessage = "AEMSWM_E_INVALID_MULTI_SITE_MODE";						//break;
					}
					ErrorMessage = "[AEM Facility Code 13: AemSwitch] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 14 [AemCM]
				**	----------------------------------------------------------------------------------------------------
				*/
#if CM_ENABLE 
				else if (errorCode.ToString("X")->StartsWith("AE0E"))
				{
					switch (errorCode)
					{
					case ER_AEMCM_E_CM_NOT_FOUND: errorCodeMessage = "AEMCM_E_CM_NOT_FOUND";									//break;
					case ER_AEMCM_E_ERASE_SFLASH_FAIL: errorCodeMessage = "AEMCM_E_ERASE_SFLASH_FAIL";								//break;
					case ER_AEMCM_E_WRITE_SFLASH_FAIL: errorCodeMessage = "AEMCM_E_WRITE_SFLASH_FAIL";								//break;
					case ER_AEMCM_E_READ_SFLASH_FAIL: errorCodeMessage = "AEMCM_E_READ_SFLASH_FAIL";								//break;
					case ER_AEMCM_E_TIMED_OUT: errorCodeMessage = "AEMCM_E_TIMED_OUT";										//break;
					case ER_AEMCM_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMCM_E_FIRMWARE_FILE_NOT_FOUND";							//break;
					case ER_AEMCM_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMCM_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMCM_E_INVALID_NAME: errorCodeMessage = "AEMCM_E_INVALID_NAME";									//break;
					case ER_AEMCM_E_INVALID_OPTION: errorCodeMessage = "AEMCM_E_INVALID_OPTION";									//break;
					case ER_AEMCM_E_EXCEED_LIMIT: errorCodeMessage = "AEMCM_E_EXCEED_LIMIT";									//break;
					case ER_AEMCM_E_DIRECTORY_NAME_TOO_LONG: errorCodeMessage = "AEMCM_E_DIRECTORY_NAME_TOO_LONG";							//break;
					case ER_AEMCM_E_INVALID_SPI_CHSEL: errorCodeMessage = "AEMCM_E_INVALID_SPI_CHSEL";								//break;
					case ER_AEMCM_E_INVALID_ATTRIBUTE: errorCodeMessage = "AEMCM_E_INVALID_ATTRIBUTE";								//break;
					case ER_AEMCM_E_INVALID_SPI_CORE: errorCodeMessage = "AEMCM_E_INVALID_SPI_CORE";								//break;
					case ER_AEMCM_E_INVALID_TRIGGER_IN: errorCodeMessage = "AEMCM_E_INVALID_TRIGGER_IN";								//break;
					case ER_AEMCM_E_INVALID_TRIGGER_OUT: errorCodeMessage = "AEMCM_E_INVALID_TRIGGER_OUT";								//break;
					case ER_AEMCM_E_SYS_ID_NOT_FOUND: errorCodeMessage = "AEMCM_E_SYS_ID_NOT_FOUND";								//break;
					case ER_AEMCM_E_BAD_ARGUMENTS: errorCodeMessage = "AEMCM_E_BAD_ARGUMENTS";									//break;
					}
					//ret = glob->cm[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcCM[siteIndex], errorCode, ReturnErrorMessage);
					//ErrorMessage = ReturnErrorMessage->ToString();
					ErrorMessage = "[AEM Facility Code 14: AemCM] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif 
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 15 [AemAWG]
				**	----------------------------------------------------------------------------------------------------
				*/
#if ACM_ENABLE
				else if (errorCode.ToString("X")->StartsWith("AE0F"))
				{
					switch (errorCode)
					{
					case ER_AEMAWG_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMAWG_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMAWG_E_CAL_SIZE_NOT_MATCH: errorCodeMessage = "AEMAWG_E_CAL_SIZE_NOT_MATCH";								//break;
					case ER_AEMAWG_E_INVALID_CAL_ACCUM_COUNT: errorCodeMessage = "AEMAWG_E_INVALID_CAL_ACCUM_COUNT";						//break;
					case ER_AEMAWG_E_INVALID_CAL_DATE: errorCodeMessage = "AEMAWG_E_INVALID_CAL_DATE";								//break;
					case ER_AEMAWG_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMAWG_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMAWG_E_DDS_TIMEOUT: errorCodeMessage = "AEMAWG_E_DDS_TIMEOUT";									//break;
					case ER_AEMAWG_E_WAVEFORM_MEMORY_TIMEOUT: errorCodeMessage = "AEMAWG_E_WAVEFORM_MEMORY_TIMEOUT";						//break;
					case ER_AEMAWG_E_AWG_NOT_FOUND: errorCodeMessage = "AEMAWG_E_AWG_NOT_FOUND";									//break;
					case ER_AEMAWG_E_INVALID_CHANNEL_COUNT: errorCodeMessage = "AEMAWG_E_INVALID_CHANNEL_COUNT";							//break;
					case ER_AEMAWG_E_INVALID_CHANNEL_NAME: errorCodeMessage = "AEMAWG_E_INVALID_CHANNEL_NAME";							//break;
					case ER_AEMAWG_E_INVALID_SAMPLING_TIME: errorCodeMessage = "AEMAWG_E_INVALID_SAMPLING_TIME";							//break;
					case ER_AEMAWG_E_ADC_TIMEOUT: errorCodeMessage = "AEMAWG_E_ADC_TIMEOUT";									//break;
					case ER_AEMAWG_E_INVALID_OPERATION_MODE: errorCodeMessage = "AEMAWG_E_INVALID_OPERATION_MODE";							//break;
					case ER_AEMAWG_E_EXCEED_FREQUENCY_SPEC: errorCodeMessage = "AEMAWG_E_EXCEED_FREQUENCY_SPEC";							//break;
					case ER_AEMAWG_E_EXCEED_VOLTAGE_SPEC: errorCodeMessage = "AEMAWG_E_EXCEED_VOLTAGE_SPEC";							//break;
					case ER_AEMAWG_E_EXCEED_STARTPHASE_SPEC: errorCodeMessage = "AEMAWG_E_EXCEED_STARTPHASE_SPEC";							//break;
					case ER_AEMAWG_E_EXCEED_DUTYCYCLE_SPEC: errorCodeMessage = "AEMAWG_E_EXCEED_DUTYCYCLE_SPEC";							//break;
					case ER_AEMAWG_E_WRONG_MODULE_GAIN: errorCodeMessage = "AEMAWG_E_WRONG_MODULE_GAIN";								//break;
					case ER_AEMAWG_E_FUNCTION_GENERATOR_BUSY: errorCodeMessage = "AEMAWG_E_FUNCTION_GENERATOR_BUSY";						//break;
					case ER_AEMAWG_E_EXCEED_TOTAL_WAVEFORM_MEMORY_LIMIT: errorCodeMessage = "AEMAWG_E_EXCEED_TOTAL_WAVEFORM_MEMORY_LIMIT";				//break;
					case ER_AEMAWG_E_UNALLOCATED_RESOURCE: errorCodeMessage = "AEMAWG_E_UNALLOCATED_RESOURCE";							//break;
					case ER_AEMAWG_E_EXCEED_MEMORY_LIMIT_OF_RESOURCE: errorCodeMessage = "AEMAWG_E_EXCEED_MEMORY_LIMIT_OF_RESOURCE";				//break;
					case ER_AEMAWG_E_FILE_NOT_FOUND: errorCodeMessage = "AEMAWG_E_FILE_NOT_FOUND";									//break;
					case ER_AEMAWG_E_FILE_CORRUPTED: errorCodeMessage = "AEMAWG_E_FILE_CORRUPTED";									//break;
					case ER_AEMAWG_E_EXCEED_LEVEL_LIMIT: errorCodeMessage = "AEMAWG_E_EXCEED_LEVEL_LIMIT";								//break;
					case ER_AEMAWG_E_INVALID_CHANNEL: errorCodeMessage = "AEMAWG_E_INVALID_CHANNEL";								//break;
					case ER_AEMAWG_E_MARKER_OVERLAP: errorCodeMessage = "AEMAWG_E_MARKER_OVERLAP";									//break;
					case ER_AEMAWG_E_CHANNEL_IS_ARMED: errorCodeMessage = "AEMAWG_E_CHANNEL_IS_ARMED";								//break;
					case ER_AEMAWG_E_FILTER_SET_UNINITIALIZED: errorCodeMessage = "AEMAWG_E_FILTER_SET_UNINITIALIZED";						//break;
					case ER_AEMAWG_E_INVALID_INTERPOLATOR_SETTING: errorCodeMessage = "AEMAWG_E_INVALID_INTERPOLATOR_SETTING";					//break;
					case ER_AEMAWG_E_INVALID_FILTER_SETTING: errorCodeMessage = "AEMAWG_E_INVALID_FILTER_SETTING";							//break;
					case ER_AEMAWG_E_EXCEED_TRANSITION_SPEC: errorCodeMessage = "AEMAWG_E_EXCEED_TRANSITION_SPEC";							//break;
					case ER_AEMAWG_E_AWG_BUSY: errorCodeMessage = "AEMAWG_E_AWG_BUSY";										//break;
					case ER_AEMAWG_E_INVALID_CALIBRATION_ARGUMENTS: errorCodeMessage = "AEMAWG_E_INVALID_CALIBRATION_ARGUMENTS";					//break;
					case ER_AEMAWG_E_TIMED_OUT: errorCodeMessage = "AEMAWG_E_TIMED_OUT";										//break;
					case ER_AEMAWG_E_READ_FLASH_FAIL: errorCodeMessage = "AEMAWG_E_READ_FLASH_FAIL";								//break;
					case ER_AEMAWG_E_INVALID_CAL_TEMPERATURE: errorCodeMessage = "AEMAWG_E_INVALID_CAL_TEMPERATURE";						//break;
					case ER_AEMAWG_E_INVALID_CAL_INTERVAL_MONTHS: errorCodeMessage = "AEMAWG_E_INVALID_CAL_INTERVAL_MONTHS";					//break;
					case ER_AEMAWG_E_CAL_DATA_OUT_OF_RANGE: errorCodeMessage = "AEMAWG_E_CAL_DATA_OUT_OF_RANGE";							//break;
					case ER_AEMAWG_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMAWG_E_WRITE_EPCS_FAIL";								//break;
					case ER_AEMAWG_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMAWG_E_ERASE_EPCS_FAIL";								//break;
					case ER_AEMAWG_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMAWG_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMAWG_E_WRONG_CALTYPE: errorCodeMessage = "AEMAWG_E_WRONG_CALTYPE";									//break;
					case ER_AEMAWG_E_INVALID_HEADER: errorCodeMessage = "AEMAWG_E_INVALID_HEADER";									//break;
					case ER_AEMAWG_E_NULL_ERROR: errorCodeMessage = "AEMAWG_E_NULL_ERROR";										//break;
					case ER_AEMAWG_E_CALIBRATED_GAIN_OUT_OF_RANGE: errorCodeMessage = "AEMAWG_E_CALIBRATED_GAIN_OUT_OF_RANGE";					//break;
					case ER_AEMAWG_E_CALIBRATION_DATA_SIZE_MISMATCH: errorCodeMessage = "AEMAWG_E_CALIBRATION_DATA_SIZE_MISMATCH";					//break;
					case ER_AEMAWG_E_INVALID_OPTION: errorCodeMessage = "AEMAWG_E_INVALID_OPTION";									//break;
					case ER_AEMAWG_E_FILTER_SET_IS_IIR: errorCodeMessage = "AEMAWG_E_FILTER_SET_IS_IIR";								//break;
					case ER_AEMAWG_E_FILTER_SET_IS_FIR: errorCodeMessage = "AEMAWG_E_FILTER_SET_IS_FIR";								//break;
					case ER_AEMAWG_E_API_NOT_SUPPORTED_FOR_THIS_MODEL: errorCodeMessage = "AEMAWG_E_API_NOT_SUPPORTED_FOR_THIS_MODEL";				//break;
					case ER_AEMAWG_E_EXCEED_IMPEDANCE_LIMIT: errorCodeMessage = "AEMAWG_E_EXCEED_IMPEDANCE_LIMIT";							//break;
					case ER_AEMAWG_E_EXCEED_CURRENT_LIMIT: errorCodeMessage = "AEMAWG_E_EXCEED_CURRENT_LIMIT";							//break;
					}
					//ret = glob->acm[siteIndex]->GetErrorMessage(glob->RsrcManager.RsrcACM[siteIndex], errorCode, ReturnErrorMessage);
					//ErrorMessage = ReturnErrorMessage->ToString();
					ErrorMessage = "[AEM Facility Code 15: AemAWG] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
#endif 
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 16 [Reserved for wrapper use]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE10"))
				{
					switch (errorCode)
					{
						// AMB7600
					case ER_OPERATION_HALTED_BY_USER: errorCodeMessage = "OPERATION_HALTED_BY_USER";								//break;
					case ER_PREREQUISITE_NOT_MET: errorCodeMessage = "PREREQUISITE_NOT_MET";									//break;
					case ER_ILLEGAL_OPERATION: errorCodeMessage = "ILLEGAL_OPERATION";										//break;
					case ER_NO_CALIBRATION_RECORD: errorCodeMessage = "NO_CALIBRATION_RECORD";									//break;
					case ER_CALIBRATION_DUE: errorCodeMessage = "CALIBRATION_DUE";											//break;
					case ER_INVALID_USER_INPUT: errorCodeMessage = "INVALID_USER_INPUT";										//break;
					case ER_INVALID_QUERY: errorCodeMessage = "INVALID_QUERY";											//break;
					case ER_STATION_ID_OCCUPIED: errorCodeMessage = "STATION_ID_OCCUPIED";										//break;
					case ER_MODULATION_OVERLOADED: errorCodeMessage = "MODULATION_OVERLOADED";									//break;
					case ER_MODULATION_NOT_LOADED: errorCodeMessage = "MODULATION_NOT_LOADED";									//break;
					case ER_MODULATION_ALREADY_LOADED: errorCodeMessage = "MODULATION_ALREADY_LOADED";								//break;
					case ER_RESOURCE_ALREADY_GRANTED: errorCodeMessage = "RESOURCE_ALREADY_GRANTED";								//break;
					case ER_CORRELATION_ERROR: errorCodeMessage = "CORRELATION_ERROR";										//break;
					case ER_INVALID_SETTING: errorCodeMessage = "INVALID_SETTING";											//break;
					case ER_CALIBRATION_OUT_OF_SPECS: errorCodeMessage = "CALIBRATION_OUT_OF_SPECS";								//break;
					case ER_UNSUPPORTED_OFFLINE: errorCodeMessage = "UNSUPPORTED_OFFLINE";										//break;
					case ER_ENFORCED_OFFLINE: errorCodeMessage = "ENFORCED_OFFLINE";										//break;
					case ER_SOFTWARE_TIMEOUT: errorCodeMessage = "SOFTWARE_TIMEOUT";										//break;
					case ER_HADWARE_TIMEOUT: errorCodeMessage = "HADWARE_TIMEOUT";											//break;
					case ER_EXCEED_MAX_SAMPLE_SZIE: errorCodeMessage = "EXCEED_MAX_SAMPLE_SZIE";									//break;
					case ER_MISSING_CALIBRATION_FILE: errorCodeMessage = "MISSING_CALIBRATION_FILE";								//break;
					case ER_NO_TESTHEAD: errorCodeMessage = "NO_TESTHEAD";												//break;
					case ER_NO_TESTSITE: errorCodeMessage = "NO_TESTSITE";												//break;
					case ER_API_NOT_SUPPORTED: errorCodeMessage = "API_NOT_SUPPORTED";										//break;
					case ER_LOAD_DLL_FILE_ERROR: errorCodeMessage = "LOAD_DLL_FILE_ERROR";										//break;
					case ER_LOAD_DLL_FUNCTION_ERROR: errorCodeMessage = "LOAD_DLL_FUNCTION_ERROR";									//break;
					case ER_MEMORY_ALLOCATION_ERROR: errorCodeMessage = "MEMORY_ALLOCATION_ERROR";									//break;
					case ER_FILE_IO_ERROR: errorCodeMessage = "FILE_IO_ERROR";											//break;
					case ER_MISSING_GLOBAL_COND: errorCodeMessage = "MISSING_GLOBAL_COND";										//break;
					case ER_MISSING_FLOWITEM_COND: errorCodeMessage = "MISSING_FLOWITEM_COND";									//break;
					case ER_MISSING_TESTPARAM_COND: errorCodeMessage = "MISSING_TESTPARAM_COND";									//break;
					case ER_INVALID_COMMAND: errorCodeMessage = "INVALID_COMMAND";											//break;
					case ER_INVALID_COMMAND_ARGUMENTS: errorCodeMessage = "INVALID_COMMAND_ARGUMENTS";								//break;
					case ER_REGISTER_INSTRUMENT_ERROR: errorCodeMessage = "REGISTER_INSTRUMENT_ERROR";								//break;
					case ER_INIT_INSTRUMENT_ERROR_OR_INCORRECT_RESOURCE_CONFIG: errorCodeMessage = "INIT_INSTRUMENT_ERROR_OR_INCORRECT_RESOURCE_CONFIG";		//break;
					case ER_INSTRUMENT_READ_ERROR: errorCodeMessage = "INSTRUMENT_READ_ERROR";									//break;
					case ER_INSTRUMENT_WRITE_ERROR: errorCodeMessage = "INSTRUMENT_WRITE_ERROR";									//break;
					case ER_INCOMPATIBLE_HARDWARE: errorCodeMessage = "INCOMPATIBLE_HARDWARE";									//break;
					case ER_UNINIT_INSTRUMENT_ERROR: errorCodeMessage = "UNINIT_INSTRUMENT_ERROR";									//break;
					case ER_VOLTAGE_OUT_OF_RANGE: errorCodeMessage = "VOLTAGE_OUT_OF_RANGE";									//break;
					case ER_CURRENT_OUT_OF_RANGE: errorCodeMessage = "CURRENT_OUT_OF_RANGE";									//break;
					case ER_INVALID_RESOURCE_PIN: errorCodeMessage = "INVALID_RESOURCE_PIN";									//break;
																																// AMB7300 
					case ER_PORTPAIR_ALIAS_NOT_MATCHED: errorCodeMessage = "PORTPAIR_ALIAS_NOT_MATCHED (Port-Pair Combination Does Not Exist in AMB7300AxMapping.xml File)";						//break;
					case ER_MISSING_INSTR_CONFIG_FILE: errorCodeMessage = "MISSING_INSTR_CONFIG_FILE";																							//break;
					case ER_X86_NOT_COMPATIBLE: errorCodeMessage = "X86_NOT_COMPATIBLE";																									//break;
					case ER_MISSING_STATE_FILE: errorCodeMessage = "MISSING_STATE_FILE (State File (*.sta) does not exit in C:\\ProgramData\\Aemulus\\AMB7300\\State)";					//break;
					case ER_MISSING_MAPPING_FILE: errorCodeMessage = "MISSING_MAPPING_FILE";																								//break;
					case ER_CONST_DUPLICATED_MEAS_TYPE: errorCodeMessage = "CONST_DUPLICATED_MEAS_TYPE (Duplicated measurement type been set with amb7300tl->vnaSetting[siteIndex].measType)";	//break;
																																																//case ER_CONST_INCORRECT_RESOURCE_CONFIG					: errorCodeMessage = "ER_CONST_INCORRECT_RESOURCE_CONFIG (Platform resource module and system configuration module not match! Please check AMB7300Configuration.xml in the directory of C:\\ProgramData\\Aemulus\\AMB7300\\Config)";	break;
					}
					ErrorMessage = "[AEM Facility Code 16: Reserved for wrapper use] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}

				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 17 [AemDBG_1]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE11"))
				{
					switch (errorCode)
					{
					case ER_AEMDBG_E_DBG_NOT_FOUND: errorCodeMessage = "AEMDBG_E_DBG_NOT_FOUND";									//break;
					case ER_AEMDBG_E_INVALID_RESOURCE_OUT1: errorCodeMessage = "AEMDBG_E_INVALID_RESOURCE_OUT1";							//break;
					case ER_AEMDBG_E_INVALID_RESOURCE_OUT2: errorCodeMessage = "AEMDBG_E_INVALID_RESOURCE_OUT2";							//break;
					case ER_AEMDBG_E_INVALID_PC_OUT1: errorCodeMessage = "AEMDBG_E_INVALID_PC_OUT1";								//break;
					case ER_AEMDBG_E_INVALID_COL_OUT1: errorCodeMessage = "AEMDBG_E_INVALID_COL_OUT1";								//break;
					case ER_AEMDBG_E_INVALID_PAD_OUT1: errorCodeMessage = "AEMDBG_E_INVALID_PAD_OUT1";								//break;
					case ER_AEMDBG_E_INVALID_PC_OUT2: errorCodeMessage = "AEMDBG_E_INVALID_PC_OUT2";								//break;
					case ER_AEMDBG_E_INVALID_COL_OUT2: errorCodeMessage = "AEMDBG_E_INVALID_COL_OUT2";								//break;
					case ER_AEMDBG_E_INVALID_PAD_OUT2: errorCodeMessage = "AEMDBG_E_INVALID_PAD_OUT2";								//break;
					case ER_AEMDBG_E_INVALID_MUX_CONDITION: errorCodeMessage = "AEMDBG_E_INVALID_MUX_CONDITION";							//break;
					case ER_AEMDBG_E_READSUBID_FAIL: errorCodeMessage = "AEMDBG_E_READSUBID_FAIL";									//break;
					case ER_AEMDBG_E_READMODEL_FAIL: errorCodeMessage = "AEMDBG_E_READMODEL_FAIL";									//break;
					case ER_AEMDBG_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMDBG_E_WRITE_EPCS_FAIL";								//break;
					case ER_AEMDBG_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMDBG_E_ERASE_EPCS_FAIL";								//break;
					case ER_AEMDBG_E_READ_EPCS_FAIL: errorCodeMessage = "AEMDBG_E_READ_EPCS_FAIL";									//break;
					case ER_AEMDBG_E_DLL_NOT_FOUND: errorCodeMessage = "AEMDBG_E_DLL_NOT_FOUND";									//break;
					case ER_AEMDBG_E_API_NOT_FOUND: errorCodeMessage = "AEMDBG_E_API_NOT_FOUND";									//break;
					case ER_AEMDBG_E_OPEN_FAIL: errorCodeMessage = "AEMDBG_E_OPEN_FAIL";										//break;
					case ER_AEMDBG_E_RESET_FAIL: errorCodeMessage = "AEMDBG_E_RESET_FAIL";										//break;
					case ER_AEMDBG_E_WRITE_FLASH_FAIL: errorCodeMessage = "AEMDBG_E_WRITE_FLASH_FAIL";								//break;
					case ER_AEMDBG_E_READ_FLASH_FAIL: errorCodeMessage = "AEMDBG_E_READ_FLASH_FAIL";								//break;
					case ER_AEMDBG_E_ERASE_FLASH_FAIL: errorCodeMessage = "AEMDBG_E_ERASE_FLASH_FAIL";								//break;
					case ER_AEMDBG_E_SIGNATURE_NOT_MATCH: errorCodeMessage = "AEMDBG_E_SIGNATURE_NOT_MATCH";							//break;
					case ER_AEMDBG_E_BAD_ARGUMENTS: errorCodeMessage = "AEMDBG_E_BAD_ARGUMENTS";									//break;
					case ER_AEMDBG_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMDBG_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMDBG_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMDBG_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMDBG_E_RS232_FAIL: errorCodeMessage = "AEMDBG_E_RS232_FAIL";										//break;
					}
					ErrorMessage = "[AEM Facility Code 17: AemDBG_1] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 18 [AemDBG_2]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE12"))
				{
					switch (errorCode)
					{
					case ER_AEMDBG_E_INSTRUMENT_SIGNATURE_NOT_MATCH: errorCodeMessage = "AEMDBG_E_INSTRUMENT_SIGNATURE_NOT_MATCH";					//break;
					case ER_AEMDBG_E_DIAGNOSTIC_SIGNATURE_NOT_MATCH: errorCodeMessage = "AEMDBG_E_DIAGNOSTIC_SIGNATURE_NOT_MATCH";					//break;
					case ER_AEMDBG_E_READSUBID_FAIL_2: errorCodeMessage = "ER_AEMDBG_E_READSUBID_FAIL_2";							//break;
					case ER_AEMDBG_E_INVALID_ARGUMENTS: errorCodeMessage = "AEMDBG_E_INVALID_ARGUMENTS";								//break;
					case ER_AEMDBG_E_ADC_TIMED_OUT: errorCodeMessage = "AEMDBG_E_ADC_TIMED_OUT";									//break;
					case ER_AEMDBG_E_EXCEED_VOLTAGE_SPEC: errorCodeMessage = "AEMDBG_E_EXCEED_VOLTAGE_SPEC";							//break;
					case ER_AEMDBG_E_CALIBRATED_GAIN_OUT_OF_RANGE: errorCodeMessage = "AEMDBG_E_CALIBRATED_GAIN_OUT_OF_RANGE";					//break;
					case ER_AEMDBG_E_TDC_TRIGGER_STATUS_FAILED: errorCodeMessage = "AEMDBG_E_TDC_TRIGGER_STATUS_FAILED";						//break;
					case ER_AEMDBG_E_DBG_WRONG_SITE: errorCodeMessage = "AEMDBG_E_DBG_WRONG_SITE";									//break;
					case ER_AEMDBG_E_DBG_FLASH_BUSY_TIMEOUT: errorCodeMessage = "AEMDBG_E_DBG_FLASH_BUSY_TIMEOUT";							//break;
					case ER_AEMDBG_E_DBG_SIGNATURES0_MISMATCH: errorCodeMessage = "AEMDBG_E_DBG_SIGNATURES0_MISMATCH";						//break;
					case ER_AEMDBG_E_DBG_SIGNATURES1_MISMATCH: errorCodeMessage = "AEMDBG_E_DBG_SIGNATURES1_MISMATCH";						//break;
					case ER_AEMDBG_E_DBG_SIGNATURES2_MISMATCH: errorCodeMessage = "AEMDBG_E_DBG_SIGNATURES2_MISMATCH";						//break;
					case ER_AEMDBG_E_DBG_SIGNATURES3_MISMATCH: errorCodeMessage = "AEMDBG_E_DBG_SIGNATURES3_MISMATCH";						//break;
					case ER_AEMDBG_E_INVALID_HEADER: errorCodeMessage = "AEMDBG_E_INVALID_HEADER";									//break;
					case ER_AEMDBG_E_WRONG_CALTYPE: errorCodeMessage = "AEMDBG_E_WRONG_CALTYPE";									//break;
					case ER_AEMDBG_E_INVALID_CAL_INDEX: errorCodeMessage = "AEMDBG_E_INVALID_CAL_INDEX";								//break;
					case ER_AEMDBG_E_NOT_ENOUGH_MEMORY: errorCodeMessage = "AEMDBG_E_NOT_ENOUGH_MEMORY";								//break;
					case ER_AEMDBG_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMDBG_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMDBG_E_INVALID_CAL_ARRAYMODE: errorCodeMessage = "AEMDBG_E_INVALID_CAL_ARRAYMODE";							//break;
					case ER_AEMDBG_E_INVALID_SITE_NUMBER: errorCodeMessage = "AEMDBG_E_INVALID_SITE_NUMBER";							//break;
					case ER_AEMDBG_E_FILE_NOT_FOUND: errorCodeMessage = "AEMDBG_E_FILE_NOT_FOUND";									//break;
					case ER_AEMDBG_E_FILE_CORRUPTED: errorCodeMessage = "AEMDBG_E_FILE_CORRUPTED";									//break;
					case ER_AEMDBG_E_FIRMWARE_FILE_NOT_FOUND_2: errorCodeMessage = "ER_AEMDBG_E_FIRMWARE_FILE_NOT_FOUND_2";					//break;
					case ER_AEMDBG_E_FILE_CORRUPTED_2: errorCodeMessage = "ER_AEMDBG_E_FILE_CORRUPTED_2";							//break;
					case ER_AEMDBG_E_WRONG_FIRMWARE_FILE_USED_2: errorCodeMessage = "ER_AEMDBG_E_WRONG_FIRMWARE_FILE_USED_2";					//break;
					case ER_AEMDBG_E_FIRMWARE_FILE_CHECKSUM_ERROR: errorCodeMessage = "AEMDBG_E_FIRMWARE_FILE_CHECKSUM_ERROR";					//break;
					case ER_AEMDBG_E_WRONG_CAL_MODE: errorCodeMessage = "AEMDBG_E_WRONG_CAL_MODE";									//break;
					case ER_AEMDBG_E_COL_INVALID: errorCodeMessage = "AEMDBG_E_COL_INVALID";									//break;
					case ER_AEMDBG_E_INVALID_DBG_ADDR: errorCodeMessage = "AEMDBG_E_INVALID_DBG_ADDR";								//break;
					}
					ErrorMessage = "[AEM Facility Code 18: AemDBG_2] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 19 [AemRFM]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE13"))
				{
					switch (errorCode)
					{
					case ER_AEMRFM_E_RFM_NOT_FOUND: errorCodeMessage = "AEMRFM_E_RFM_NOT_FOUND";									//break;
					case ER_AEMRFM_E_ERASE_SFLASH_FAIL: errorCodeMessage = "AEMRFM_E_ERASE_SFLASH_FAIL";								//break;
					case ER_AEMRFM_E_WRITE_SFLASH_FAIL: errorCodeMessage = "AEMRFM_E_WRITE_SFLASH_FAIL";								//break;
					case ER_AEMRFM_E_READ_SFLASH_FAIL: errorCodeMessage = "AEMRFM_E_READ_SFLASH_FAIL";								//break;
					case ER_AEMRFM_E_TIMED_OUT: errorCodeMessage = "AEMRFM_E_TIMED_OUT";										//break;
					case ER_AEMRFM_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMRFM_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMRFM_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMRFM_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMRFM_E_INVALID_NAME: errorCodeMessage = "AEMRFM_E_INVALID_NAME";									//break;
					case ER_AEMRFM_E_INVALID_PORT: errorCodeMessage = "AEMRFM_E_INVALID_PORT";									//break;
					case ER_AEMRFM_E_INVALID_PIN: errorCodeMessage = "AEMRFM_E_INVALID_PIN";									//break;
					case ER_AEMRFM_E_INVALID_CHANNEL: errorCodeMessage = "AEMRFM_E_INVALID_CHANNEL";								//break;
					case ER_AEMRFM_E_ADC_TIMEOUT: errorCodeMessage = "AEMRFM_E_ADC_TIMEOUT";									//break;
					case ER_AEMRFM_E_FILE_NOT_FOUND: errorCodeMessage = "AEMRFM_E_FILE_NOT_FOUND";									//break;
					case ER_AEMRFM_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMRFM_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMRFM_E_BAD_ARGUMENTS: errorCodeMessage = "AEMRFM_E_BAD_ARGUMENTS";									//break;
					case ER_AEMRFM_E_M_OUTRANGE: errorCodeMessage = "AEMRFM_E_M_OUTRANGE";										//break;
					case ER_AEMRFM_E_C_OUTRANGE: errorCodeMessage = "AEMRFM_E_C_OUTRANGE";										//break;
					case ER_EMRFM_E_INVALID_SPI_CHSEL: errorCodeMessage = "EMRFM_E_INVALID_SPI_CHSEL";								//break;
					case ER_AEMRFM_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMRFM_E_WRITE_EPCS_FAIL";								//break;
					case ER_AEMRFM_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMRFM_E_ERASE_EPCS_FAIL";								//break;
					case ER_AEMRFM_E_READ_EPCS_FAIL: errorCodeMessage = "AEMRFM_E_READ_EPCS_FAIL";									//break;
					case ER_AEMRFM_E_WRONG_CALTYPE: errorCodeMessage = "AEMRFM_E_WRONG_CALTYPE";									//break;
					case ER_AEMRFM_E_EXCEED_ALLOCATED_SPACE: errorCodeMessage = "AEMRFM_E_EXCEED_ALLOCATED_SPACE";							//break;
					case ER_AEMRFM_E_FILE_CORRUPTED: errorCodeMessage = "AEMRFM_E_FILE_CORRUPTED";									//break;
					case ER_AEMRFM_E_INVALID_HEADER: errorCodeMessage = "AEMRFM_E_INVALID_HEADER";									//break;
					}
					ErrorMessage = "[AEM Facility Code 19: AemRFM] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 20 [AemCAL]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE14"))
				{
					switch (errorCode)
					{
					case ER_AEMCAL_E_OPEN_FAIL: errorCodeMessage = "AEMCAL_E_OPEN_FAIL";										//break;
					case ER_AEMCAL_E_RESET_FAIL: errorCodeMessage = "AEMCAL_E_RESET_FAIL";										//break;
					case ER_AEMCAL_E_WRITE_ERROR: errorCodeMessage = "AEMCAL_E_WRITE_ERROR";									//break;
					case ER_AEMCAL_E_WRITE_LENGTH_NOT_MATCH: errorCodeMessage = "AEMCAL_E_WRITE_LENGTH_NOT_MATCH";							//break;
					case ER_AEMCAL_E_READ_ERROR: errorCodeMessage = "AEMCAL_E_READ_ERROR";										//break;
					case ER_AEMCAL_E_READ_LENGTH_NOT_MATCH: errorCodeMessage = "AEMCAL_E_READ_LENGTH_NOT_MATCH";							//break;
					case ER_AEMCAL_E_SIGNATURE_NOT_MATCH: errorCodeMessage = "AEMCAL_E_SIGNATURE_NOT_MATCH";							//break;
					case ER_AEMCAL_E_CLOSE_FAIL: errorCodeMessage = "AEMCAL_E_CLOSE_FAIL";										//break;
					case ER_AEMCAL_E_INVALID_SLOT: errorCodeMessage = "AEMCAL_E_INVALID_SLOT";									//break;
					case ER_AEMCAL_E_READ_SFLASH_FAIL: errorCodeMessage = "AEMCAL_E_READ_SFLASH_FAIL";								//break;
					case ER_AEMCAL_E_API_NOT_SUPPORTED: errorCodeMessage = "AEMCAL_E_API_NOT_SUPPORTED";								//break;
					case ER_AEMCAL_E_DLL_NOT_FOUND: errorCodeMessage = "AEMCAL_E_DLL_NOT_FOUND";									//break;
					case ER_AEMCAL_E_API_NOT_FOUND: errorCodeMessage = "AEMCAL_E_API_NOT_FOUND";									//break;
					case ER_AEMCAL_E_CARD_NOT_FOUND: errorCodeMessage = "AEMCAL_E_CARD_NOT_FOUND";									//break;
					case ER_AEMCAL_E_SUB_ID_NOT_MATCH: errorCodeMessage = "AEMCAL_E_SUB_ID_NOT_MATCH";								//break;
					case ER_AEMCAL_E_INVALID_ARGUMENTS: errorCodeMessage = "AEMCAL_E_INVALID_ARGUMENTS";								//break;
					case ER_AEMCAL_E_EXCEED_VOLTAGE_SPEC: errorCodeMessage = "AEMCAL_E_EXCEED_VOLTAGE_SPEC";							//break;
					case ER_AEMCAL_E_EXCEED_FREQUENCY_SPEC: errorCodeMessage = "AEMCAL_E_EXCEED_FREQUENCY_SPEC";							//break;
					case ER_AEMCAL_E_EXCEED_STARTPHASE_SPEC: errorCodeMessage = "AEMCAL_E_EXCEED_STARTPHASE_SPEC";							//break;
					case ER_AEMCAL_E_CALIBRATED_GAIN_OUT_OF_RANGE: errorCodeMessage = "AEMCAL_E_CALIBRATED_GAIN_OUT_OF_RANGE";					//break;
					case ER_AEMCAL_E_FUNCTION_GENERATOR_BUSY: errorCodeMessage = "AEMCAL_E_FUNCTION_GENERATOR_BUSY";						//break;
					case ER_AEMCAL_E_CAL_HEADER_NOT_FOUND: errorCodeMessage = "AEMCAL_E_CAL_HEADER_NOT_FOUND";							//break;
					case ER_AEMCAL_E_INVALID_HEADER: errorCodeMessage = "AEMCAL_E_INVALID_HEADER";									//break;
					case ER_AEMCAL_E_ERASE_EPCS_FAIL: errorCodeMessage = "AEMCAL_E_ERASE_EPCS_FAIL";								//break;
					case ER_AEMCAL_E_FIRMWARE_FILE_NOT_FOUND: errorCodeMessage = "AEMCAL_E_FIRMWARE_FILE_NOT_FOUND";						//break;
					case ER_AEMCAL_E_FILE_NOT_FOUND: errorCodeMessage = "AEMCAL_E_FILE_NOT_FOUND";									//break;
					case ER_AEMCAL_E_FILE_CORRUPTED: errorCodeMessage = "AEMCAL_E_FILE_CORRUPTED";									//break;
					case ER_AEMCAL_E_WRONG_FIRMWARE_FILE_USED: errorCodeMessage = "AEMCAL_E_WRONG_FIRMWARE_FILE_USED";						//break;
					case ER_AEMCAL_E_FIRMWARE_FILE_CHECKSUM_ERROR: errorCodeMessage = "AEMCAL_E_FIRMWARE_FILE_CHECKSUM_ERROR";					//break;
					case ER_AEMCAL_E_NOT_ENOUGH_MEMORY: errorCodeMessage = "AEMCAL_E_NOT_ENOUGH_MEMORY";								//break;
					case ER_AEMCAL_E_WRONG_CALTYPE: errorCodeMessage = "AEMCAL_E_WRONG_CALTYPE";									//break;
					case ER_AEMCAL_E_NO_VOLTAGERANGE: errorCodeMessage = "AEMCAL_E_NO_VOLTAGERANGE";								//break;
					case ER_AEMCAL_E_NO_CURRENTRANGE: errorCodeMessage = "AEMCAL_E_NO_CURRENTRANGE";								//break;
					case ER_AEMCAL_E_ADC_TIMED_OUT: errorCodeMessage = "AEMCAL_E_ADC_TIMED_OUT";									//break;
					case ER_AEMCAL_E_SPI_STATUS_TIMEOUT: errorCodeMessage = "AEMCAL_E_SPI_STATUS_TIMEOUT";								//break;
					case ER_AEMCAL_E_READ_EPCS_FAIL: errorCodeMessage = "AEMCAL_E_READ_EPCS_FAIL";									//break;
					case ER_AEMCAL_E_DELAYIC_VALUE_GREEN_MISMATCH: errorCodeMessage = "AEMCAL_E_DELAYIC_VALUE_GREEN_MISMATCH";					//break;
					case ER_AEMCAL_E_DELAYIC_VALUE_RED_MISMATCH: errorCodeMessage = "AEMCAL_E_DELAYIC_VALUE_RED_MISMATCH";						//break;
					case ER_AEMCAL_E_WRONG_SLOT: errorCodeMessage = "AEMCAL_E_WRONG_SLOT";										//break;
					case ER_AEMCAL_E_CLR_TRIGGER_STATUS_TIMEOUT: errorCodeMessage = "AEMCAL_E_CLR_TRIGGER_STATUS_TIMEOUT";						//break;
					case ER_AEMCAL_E_RETRIEVE_RESULTS_TIMED_OUT: errorCodeMessage = "AEMCAL_E_RETRIEVE_RESULTS_TIMED_OUT";						//break;
					case ER_AEMCAL_E_INVALID_RESULTS: errorCodeMessage = "AEMCAL_E_INVALID_RESULTS";								//break;
					case ER_AEMCAL_E_WRITE_EPCS_FAIL: errorCodeMessage = "AEMCAL_E_WRITE_EPCS_FAIL";								//break;
					case ER_AEMCAL_E_DUPLICATE_ENTRIES_FOUND: errorCodeMessage = "AEMCAL_E_DUPLICATE_ENTRIES_FOUND";						//break;
					case ER_AEMCAL_E_SYS_ID_NOT_FOUND: errorCodeMessage = "AEMCAL_E_SYS_ID_NOT_FOUND";								//break;
					}
					ErrorMessage = "[AEM Facility Code 20: AemCAL] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				/*
				**	----------------------------------------------------------------------------------------------------
				**	Facility Code -> 21 [AemPXIScan]
				**	----------------------------------------------------------------------------------------------------
				*/
				else if (errorCode.ToString("X")->StartsWith("AE15"))
				{
					switch (errorCode)
					{
					case ER_AEMPXISCAN_E_GET_CLASS_DEVICE_FAILED: errorCodeMessage = "AEMPXISCAN_E_GET_CLASS_DEVICE_FAILED";					//break;
					case ER_AEMPXISCAN_E_CM_FUNCTION_FAILED: errorCodeMessage = "AEMPXISCAN_E_CM_FUNCTION_FAILED";							//break;
					case ER_AEMPXISCAN_E_INVALID_CARD_INDEX: errorCodeMessage = "AEMPXISCAN_E_INVALID_CARD_INDEX";							//break;
					case ER_AEMPXISCAN_E_INVALID_CARD_ADDRESS: errorCodeMessage = "AEMPXISCAN_E_INVALID_CARD_ADDRESS";						//break;
					case ER_AEMPXISCAN_E_NO_CARD_FOUND: errorCodeMessage = "AEMPXISCAN_E_NO_CARD_FOUND";								//break;
					}
					ErrorMessage = "[AEM Facility Code 21: AemPXIScan] " + errorCodeMessage + ". Refer to \"Error Codes Reference Guide.pdf\" for details." + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
				else
				{
					errorCodeMessage = "Unknown error.";
					ErrorMessage = "[AEM Facility Code --: Unknown Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
				}
			}
			/*
			**	----------------------------------------------------------------------------------------------------
			**	Process Error
			**	----------------------------------------------------------------------------------------------------
			*/
			else if (errorCode == ER_CONST_TERMINATION_CHARACTER_VISA_ERROR)
			{
				// Reset return 0 to solve 0x3FFF0005 termination character VISA error
				errorCode = 0;
				goto SkipCheckError;
			}
			else if (errorCode == ER_CONST_DICTIONARY_KEY_NOT_PRESENT)
			{
				// Error Code:-16(0xFFFFFFF0) Message:Aemulus.Hardware.AlarmException: System.Collections.Generic.KeyNotFoundException: The given key was not present in the dictionary. 
				errorCodeMessage = "The given key was not present in the dictionary.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_VALUE_CANNOT_BE_NULL)
			{
				errorCodeMessage = "Value cannot be null. (System.ArgumentNullException: Value cannot be null.)";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_ERROR_CATCH)
			{
				errorCodeMessage = "Error catch.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_BAD_ARGUMENTS)
			{
				errorCodeMessage = "Bad arguments.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_NOT_ENOUGH_MEMORY)
			{
				errorCodeMessage = "Not enough memory.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_OPEN_FAILED)
			{
				errorCodeMessage = "Open failed.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_RTPLOTTER)
			{
				errorCodeMessage = "RTPlotter error.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_NOT_FOUND)
			{
				errorCodeMessage = "Error not found.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_FILE_NOT_FOUND)
			{
				errorCodeMessage = "Error file not found.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_READ_FILE_ERROR)
			{
				errorCodeMessage = "Read file error.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_INVALID_SETTING)
			{
				errorCodeMessage = "Invalid setting.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_GENERAL)
			{
				errorCodeMessage = "General error.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_TIMEOUT)
			{
				errorCodeMessage = "Time out error.";
				ErrorMessage = "[Process Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else if (errorCode == ER_CONST_INVALID_RESULT)
			{
				errorCodeMessage = "Invalid result.";
				ErrorMessage = "[Result Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}
			else
			{
				errorCodeMessage = "Unknown error.";
				ErrorMessage = "[Unknown Error] " + errorCodeMessage + " | " + "Error Code: 0x" + errorCode.ToString("X");
			}

			// Write to tracer logger
			WriteToTracerLogger(glob->tf.TestSite, siteIndex, ERROR, ErrorMessage);
			// Write to file logger
			WriteToFileLogger(glob->tf.TestSite, siteIndex, ERROR, ErrorMessage);

			throw gcnew Aemulus::Hardware::AlarmException(ErrorMessage + " at line " + ErrorLineNumber, errorCode);
		}

	SkipCheckError:
	EndOfTest:
		return ret;
	}

	/*
	**	----------------------------------------------------------------------------------------------------
	**	Warning Message Box
	**	----------------------------------------------------------------------------------------------------
	*/
	void TestFunction::WarningMessageBox(String ^ MssgContent, String ^ WarningMssgType)
	{
		/*****************************************************************************************************
		** WarningMessageBox
		**		MssgContent		- Message to display.
		**		WarningMssgType	- Caption of the message box. eg: "Apps Calibration Setup".
		**
		** Descriptions:
		**		This is a function to displays a message box with specified text.
		******************************************************************************************************/

		MessageBoxIcon messageBoxIconDisplay = MessageBoxIcon::Information;

		if (glob->messageBoxIcon == MessageBoxIconFormat::Asterisk) messageBoxIconDisplay = MessageBoxIcon::Asterisk;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Error) messageBoxIconDisplay = MessageBoxIcon::Error;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Exclamation) messageBoxIconDisplay = MessageBoxIcon::Exclamation;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Hand) messageBoxIconDisplay = MessageBoxIcon::Hand;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Information) messageBoxIconDisplay = MessageBoxIcon::Information;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::None) messageBoxIconDisplay = MessageBoxIcon::None;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Question) messageBoxIconDisplay = MessageBoxIcon::Question;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Stop) messageBoxIconDisplay = MessageBoxIcon::Stop;
		else if (glob->messageBoxIcon == MessageBoxIconFormat::Warning) messageBoxIconDisplay = MessageBoxIcon::Warning;

		MessageBox::Show(MssgContent, WarningMssgType,
			MessageBoxButtons::OK,
			messageBoxIconDisplay,
			MessageBoxDefaultButton::Button2,
			MessageBoxOptions::DefaultDesktopOnly, false);
	}

}


/*----------------------------------------------------------------------
* Revision Log
* $Log: Globals.cpp.rca$

*** Version	: v1.0.0.6
*** Date	: 2 September 2026
*** PIC		: Zhi Kean
* Add handling for empty PortMatching AWV

*** Version	: v1.0.0.5
*** Date	: 4 April 2025
*** PIC		: Tham Zhi Kean
* Add arr_activeUUT
* Add AWV for HighPwrTest_EN & HighPwrTest_AppCalFile
* Add AWV for PortMatchingX

*** Version	: v1.0.0.4
*** Date	: 18 February 2025
*** PIC		: Tham Zhi Kean
* Modified GetTechFlowAppsWideVariable()
* Add UseGenericStateFileMapping & GenericStateFileMappingFileName
* Support GetTechFlowBinningProperty()

*** Version	: v1.0.0.3
*** Date	: 13 January 2025
*** PIC		: Tham Zhi Kean
* Move up sequence for Get App-Wide-Variable
* GetTechFlowAppsWideVariable() - Add SaveSnpData validation from AMB7300Utility.cpp

*** Version	: v1.0.0.2
*** Date	: 9 April 2024
*** PIC		: Ng Chen Yang
* Support S2Ppath and SaveSnpData at AppWideVariable

*** Version	: v1.0.0.1
*** Date	: 31 March 2023
*** PIC		: Ng Chen Yang
* UP REV

*** Version	: v1.0.0.0
*** Date	: 31 December 2022
*** PIC		: Ooi Jing Yao
* Initial release version.
* Support acquire tF3 site property, project property.
* Support acquire station ID / tester ID.
* Support BoardLoss file and folder initialization.
* Support DeviceStateFileTemplate folder initialization.
* Support FixedOffset file and folder initialization.
* Support ModulationFile folder initialization.
* Support VectorFile folder initialization.
* Support VectorStateFile folder initialization.
* Support acquire App-Wide-Variable.
* Support resource manager property initialization.
* Support tracer logger.
* Support file logger.
* Support check error function.
* Support message box function.
----------------------------------------------------------------------*/