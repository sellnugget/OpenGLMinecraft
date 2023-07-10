#pragma once
#include "AssetLoader.h"


struct stat result;


namespace Asset {


	std::map<std::string, std::pair<int, int>> FileHeader;


	void LoadImageAsset(std::string AssetName);



	int GetFileSize(const char* Path)
	{
		FILE* file;
		fopen_s(&file, Path, "r");
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);
		fclose(file);
		return size;
	}
	const char* ReadAllText(const char* Path)
	{
		FILE* file = (FILE*)1;
		fopen_s(&file, Path, "r");

		if (!file) {
			return "";
		}
		int size = 0;
		fseek(file, 0L, SEEK_END);
		size = ftell(file);
		//reset to origin
		fseek(file, 0L, SEEK_SET);

		char* Text = new char[size];
		int index = 0;
		char ch;
		do {
			ch = fgetc(file);
			Text[index] = ch;
			index++;
		} while (ch != EOF);
		Text[index - 1] = '\0';
		fclose(file);
		return Text;
	}
	std::string GetDirectory() {
		
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");

		return std::string(buffer).substr(0, pos);
	}
	
	std::vector<std::string> GetAllFilePaths(std::string path) {
		std::vector<std::string> Paths;

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			Paths.push_back(entry.path().string());
		}
		std::vector<std::string> Output;
		for (int i = 0; i < Paths.size(); i++) {

			std::string currentpath = Paths[i];
			bool Folder = false;
			for (int i = 0; i < currentpath.size(); i++) {
				if (currentpath[i] == '.') {
					break;
				}
				if (i == currentpath.size() - 1) {
					Folder = true;
					break;
				}
			}
			if (Folder) {
				std::vector<std::string> FolderPaths = GetAllFilePaths(currentpath);
				for (int i = 0; i < FolderPaths.size(); i++) {
					Paths.push_back(FolderPaths[i]);
				}
				continue;
			}
			else {
				Output.push_back(Paths[i]);
			}
		}
		return Output;
	}

	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
			+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}


	void InitAssetHandler()
	{
		//first we try to open the Asset Handler file. if this is openable then we must be in visual studio
		FILE* file;
		fopen_s(&file, "Assets\\VisualStudio.checker", "r");

		
		//this means were in visual studio
		if(file) {
			fclose(file);
			//before we waste a ton of time building assets
			//check the build log to see if the assets we are building are the same as last build

			//first generate the log for comparison

			std::vector<std::string> AssetPaths;

			std::string BuildLog;

			std::string path = "Assets";

			std::string blacklist[] =
			{
				"Assets\\VisualStudio.checker", "Assets\\BuildLog.txt"
			};

			BuildLog += "Built to " + GetDirectory() + "\n";

			std::vector<std::string> Paths = GetAllFilePaths(path);


			
			for (int i = 0; i < Paths.size(); i++)
			{
				std::string currentpath = Paths[i];

				
				bool isblacklisted = false;
				for (int i = 0; i < 2; i++) {
					if (currentpath.find(blacklist[i]) != std::string::npos) {
						isblacklisted = true;
						break;
					}
				}
				
				if (isblacklisted) {
					continue;
				}



				auto p = currentpath;
				//get last modification date
				std::filesystem::file_time_type ftime = std::filesystem::last_write_time(p);

				std::time_t ctime = to_time_t(ftime);
				std::tm* gmt = std::gmtime(&ctime);
				std::stringstream buffer;
				
				buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");

				std::string formattedFileTime = buffer.str();

				AssetPaths.push_back(currentpath);
				BuildLog += currentpath;
				BuildLog += " ";
				BuildLog += GetFileSize(currentpath.c_str());
				BuildLog += " " + formattedFileTime;
				BuildLog += "\n";
			}
			std::string CurrentLog = ReadAllText("Assets\\BuildLog.txt");
			bool alwaysbuild = 1;
			if (CurrentLog == "" || CurrentLog != BuildLog || alwaysbuild) {
				BuildAssetBin(AssetPaths);
				FILE* LogFile;
				fopen_s(&LogFile, "Assets\\BuildLog.txt", "w");
				fwrite(BuildLog.c_str(), 1, BuildLog.size(), LogFile);
				fclose(LogFile);
			}
		}


		//after this no matter what we load the header of asset.bin
		std::string path = GetDirectory() + "\\Asset.bin";

		fopen_s(&AssetFile, path.c_str(), "rb");



		if (!AssetFile) {
			throw std::exception("Asset.bin does not exist. Please either include or rebuild asset.bin");
		}

		int Count = 0;
		fread(&Count, 4, 1, AssetFile);

		for (int i = 0; i < Count; i++) {
			char current = ' ';
			std::string PathName;

			while (current != '\0') {
				current = fgetc(AssetFile);
				PathName += current;
			}
			PathName = PathName.substr(0, PathName.size() - 1);
			int location;
			int size;
			fread(&location, 4, 1, AssetFile);
			fread(&size, 4, 1, AssetFile);

			std::pair<int, int> Fileinfo = { location, size };

			FileHeader.insert({PathName, Fileinfo });


		}
		
	

	}
	void BuildAssetBin(std::vector<std::string> Paths)
	{
		std::string Path = GetDirectory() + "\\Asset.bin";

		//create or open a file named asset.bin
		FILE* file;
		fopen_s(&file, Path.c_str(), "wb");
		
		int* Value = new int[Paths.size()];

		int Files = Paths.size();

		fwrite(&Files, 4, 1, file);
		//first write all paths 

		for (int i = 0; i < Paths.size(); i++) {
			//write the path name with a null terminator
			fwrite(Paths[i].c_str(), 1, Paths[i].size(), file);
			fwrite({ "\0" }, 1, 1, file);

			Value[i] = ftell(file);
			//then allocate 4 bytes for file location then  4 bytes for file size
			fseek(file, 8, SEEK_CUR);
		}

		//next write the file data to Asset.bin
		
		//we write memory in 1mb chunks just incase a file is huge
		uint8_t* Buffer = new uint8_t[0xfffff];
		for (int i = 0; i < Paths.size(); i++) {


		


			//first get the file size
			int FileSize = GetFileSize(Paths[i].c_str());

			//then open the file as binary
			FILE* Current;
			fopen_s(&Current, Paths[i].c_str(), "rb");

			//then we get the current location in the main asset.bin file
			int location = ftell(file);
			//next go back to the file header and write its location in asset.bin and its size
			fseek(file, Value[i], SEEK_SET);

			fwrite(&location, 4, 1, file);
			fwrite(&FileSize, 4, 1, file);

			//then we come back 
			fseek(file, location, SEEK_SET);

			//this loop writes 1mb chunks 
			for (int writecount = 0; writecount < FileSize; writecount += 0xfffff) {

				int readamount = 0xfffff;
				if (0xfffff + writecount > FileSize) {
					readamount -= (0xfffff + writecount - FileSize);
				}
				fread_s(Buffer, 0xfffff, 1, readamount, Current);

				fwrite(Buffer, 1, readamount, file);
			}
			fclose(Current);
			//write null terminator to split each file
			char nll = '\0';
			fwrite(&nll, 1, 1, file);
		}
		delete[] Buffer;
		delete[] Value;
 		fclose(file);

	}
	uint8_t* LoadEntireAsset(std::string File)
	{
		if (!FileHeader.count(File)) {

			throw std::runtime_error("Error: Asset " + File + " cannot be found");
			return nullptr;
		}
		std::pair<int, int> fileinfo = FileHeader[File];
		fseek(AssetFile, fileinfo.first, SEEK_SET);
		uint8_t* FileBuffer = new uint8_t[fileinfo.second];
		fread(FileBuffer, 1, fileinfo.second, AssetFile);
		return FileBuffer;
	}

	std::string LoadTextAsset(std::string asset)
	{
		if (!FileHeader.count(asset)) {

			throw std::runtime_error("Error: Asset " + asset + " cannot be found");
			return nullptr;
		}
		std::pair<int, int> fileinfo = FileHeader[asset];
		fseek(AssetFile, fileinfo.first, SEEK_SET);


		std::string rtn = "";

		char ch = ' ';
		while (ch != '\0') {
			fread(&ch, 1, 1, AssetFile);
			rtn += ch;
		}
		return rtn;
	}
	void LoadImageAsset(std::string AssetName)
	{
		//since we need the whole image anyway i just load the entire damn thing

		
	}
}



