#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>

#include <sstream>
#include <fstream>

using json = nlohmann::json;

namespace Loopie {

	template <typename T>
	struct JsonResult {
		T Result{};
		bool Found = false;
	};



	class JsonNode
	{
	public:
		JsonNode() = default;
		JsonNode(json* node) { m_node = node; }

		bool Exists() const { return m_node != nullptr; }

		template <typename T>
		JsonResult<T> Get(const std::string& keyPath, T defaultValue = {}) const {

			if (keyPath.empty())
				return GetSelf<T>(defaultValue);
	
			JsonNode node = Child(keyPath);
			if (node.Exists()) {
				return node.GetSelf(defaultValue);
			}
			JsonResult<T> result;
			result.Found = false;
			return result;
		}

		template <typename T>
		JsonResult<T> GetSelf( T defaultValue = {}) const {

			JsonResult<T> result;
			if (Exists()) {
				result.Result = m_node->get<T>();
				result.Found = true;
			}
			else
				result.Found = false;

			return result;
		}


		template <typename T>
		bool Set(const std::string& keyPath, T value) {
			if (keyPath.empty())
				return SetSelf<T>(value);

			JsonNode node = Child(keyPath);
			if (node.Exists()) {
				node.SetSelf(value);
				return true;
			}
			else
				return false;
		}

		template <typename T>
		bool SetSelf(T value) {
			if (Exists()) {
				*m_node = value;
				return true;
			}
			return false;
		}

		template <typename T>
		bool AddField(const std::string& keyPath, T value) {
			if (keyPath.empty()) return false;

			auto pos = keyPath.find_last_of('.');
			std::string parentPath = (pos == std::string::npos) ? "" : keyPath.substr(0, pos);
			std::string key = (pos == std::string::npos) ? keyPath : keyPath.substr(pos + 1);

			JsonNode parent = parentPath.empty() ? *this : Child(parentPath);
			if (!parent.Exists() || !parent.m_node->is_object()) return false;

			(*parent.m_node)[key] = value;
			return true;
		}

		template <typename T>
		bool AddArrayElement(const std::string& keyPath, T value) {
			JsonNode node = keyPath.empty() ? *this : Child(keyPath);
			if (!node.Exists() || !node.m_node->is_array()) return false;

			node.m_node->push_back(value);
			return true;
		}

		JsonNode Child(const std::string& keyPath) const;

	private:
		json* m_node = nullptr;
	};



	class JsonData {
	public:
		friend class JsonNode;
		friend class Json;

		bool IsEmpty() const { return m_empty; }

		template <typename T>
		JsonResult<T> Get(const std::string& keyPath, T defaultValue = {}) {

			return Root().Get(keyPath, defaultValue);
		}

		template <typename T>
		bool Set(const std::string& keyPath, T value) {
			return Root().Set(keyPath, value);
		}

		template <typename T>
		bool AddField(const std::string& key, T value) {
			return Root().AddField(key, value);
		}

		template <typename T>
		bool AddArrayElement(const std::string& key, T value) {
			return Root().AddArrayElement(key, value);
		}

		JsonNode Root() { return JsonNode(&m_data); } 

	private:
		json m_data;
		bool m_empty = true;

	};



	class Json {
	public:
		static JsonData ReadFromString(const std::string& data);
		static JsonData ReadFromFile(const std::filesystem::path& filePath);
		static bool WriteToFile(const std::filesystem::path& filePath, const JsonData& jsonData, int indent = 4);

	};
}