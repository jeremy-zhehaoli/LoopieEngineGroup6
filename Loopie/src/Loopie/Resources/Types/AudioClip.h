#pragma once

#include "Loopie/Resources/Resource.h"

#include <string>

namespace Loopie {

	class AudioClip : public Resource {
		friend class AudioImporter;
	public:
		DEFINE_TYPE(AudioClip)

			AudioClip(const UUID& id);
		~AudioClip() = default;

		bool Load() override;

		const std::string& GetCachePath() const { return m_cachePath; }
		const std::string& GetAbsolutePath() const { return m_absolutePath; }

	private:
		std::string m_cachePath;
		std::string m_absolutePath;
	};
}