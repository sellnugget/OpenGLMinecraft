#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include <string>
#include <Windows.h>
#include <FreeImage.h>
#include <map>
namespace Asset {

	int GetFileSize(const char* Path);
	const char* ReadAllText(const char* Path);
	std::string GetDirectory();




	

	//loads the entirety of a asset then returns a pointer to a allocated buffer
	uint8_t* LoadEntireAsset(std::string File);


	

	static FILE* AssetFile;

	/*sets up the asset.bin file
	*if in visual studio then it will detect a asset folder and build the asset.bin
	*otherwise it will read the data from asset.bin
	*/
	void InitAssetHandler();
	//builds all assets into a single binary file.
	//this removes the need for file loaders like png or jpg openers.
	void BuildAssetBin(std::vector<std::string> Paths);
	
	std::string LoadTextAsset(std::string asset);

	



}
