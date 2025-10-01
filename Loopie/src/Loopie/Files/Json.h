#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

namespace Loopie {

	template <typename T>
	struct JsonResult {
		T Result;
	};

	struct JsonData {
		json Data;
		bool Done;

		template <typename T>
		JsonResult<T> Get(std::string key) {

			JsonResult<T> result;
			result.Result = Data.at(key);

			return result;
		}

		template <typename T>
		bool Set(std::string key, T value) {
			Data.at(key) = value;
		}
	};

	struct JsonNode {
		JsonData* Data
	};

	

	class Json {
	public:
		static JsonData ReadFromString(const std::string& data);
		static JsonData ReadFromFile(const std::filesystem::path& filePath);
	};
}