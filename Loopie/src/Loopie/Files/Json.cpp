#include "Json.h"

#include <fstream>

namespace Loopie {

	JsonData Json::ReadFromString(const std::string& data)
	{
		JsonData jsonData;

		jsonData.Data = json::parse(data);

		if (jsonData.Data.is_discarded())
			jsonData.Done = false;

		return jsonData;
	}
	JsonData Json::ReadFromFile(const std::filesystem::path& filePath)
	{
		JsonData jsonData;

		std::ifstream ifs(filePath);
		jsonData.Data = json::parse(ifs);

		if (jsonData.Data.is_discarded())
			jsonData.Done = false;

		return jsonData;
	}
}