#include "AudioClip.h"

#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Importers/AudioImporter.h"

namespace Loopie {

	AudioClip::AudioClip(const UUID& id) : Resource(id, ResourceType::AUDIO) {
		Load();
	}

	bool AudioClip::Load()
	{
		Metadata* metadata = AssetRegistry::GetMetadata(GetUUID());
		if (!metadata || !metadata->HasCache || metadata->CachesPath.empty()) {
			return false;
		}

		AudioImporter::LoadAudio(metadata->CachesPath[0], *this);
		return !m_absolutePath.empty();
	}
}