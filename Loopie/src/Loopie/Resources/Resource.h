#pragma once

#include "Loopie/Core/UUID.h"

namespace Loopie {
	class Resource {
	public:
		Resource(UUID uuid) : m_uuid(uuid) {}
		~Resource() = default;

		const UUID& GetUUID() { return m_uuid; }

		virtual void LoadFromFile(const std::string path) = 0;
		virtual void Reload() = 0;

	private:
		UUID m_uuid;
	};
}