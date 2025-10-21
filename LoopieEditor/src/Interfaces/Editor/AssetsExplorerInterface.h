#pragma once 

#include "src/Interfaces/Interface.h"

#include <filesystem>

namespace Loopie {
	class AssetsExplorerInterface : public Interface {
	public:
		AssetsExplorerInterface();
		~AssetsExplorerInterface() = default;
		void Render() override;
	private:
		std::filesystem::path m_currentDirectory;
	};
}