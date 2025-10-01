#include "Json.h"



namespace Loopie {

#pragma region Json

	JsonData Json::ReadFromString(const std::string& data)
	{
		JsonData jsonData;

		jsonData.m_data = json::parse(data);

		jsonData.m_empty = jsonData.m_data.is_discarded();

		return jsonData;
	}
	JsonData Json::ReadFromFile(const std::filesystem::path& filePath)
	{
		JsonData jsonData;

		std::ifstream ifs(filePath);
		jsonData.m_data = json::parse(ifs);
		jsonData.m_empty = jsonData.m_data.is_discarded();

		return jsonData;
	}
	bool Json::WriteToFile(const std::filesystem::path& filePath, const JsonData& jsonData, int indent)
	{
		std::ofstream ofs(filePath);
		if (!ofs.is_open()) return false;
		ofs << jsonData.m_data.dump(indent);
		return true;
	}
#pragma endregion

#pragma region JsonNode

	JsonNode JsonNode::Child(const std::string& keyPath) const
	{
		if (!Exists())
			return JsonNode();

		json* node = m_node;

		std::stringstream ss(keyPath);
		std::string token;
		while (std::getline(ss, token, '.')) {
			if (!node->contains(token))
				return JsonNode();
			node = &((*node)[token]);
		}

		return JsonNode(node);
	}

#pragma endregion

	
}