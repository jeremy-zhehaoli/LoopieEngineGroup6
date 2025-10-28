#pragma once

#include "Loopie/Core/UUID.h"

#include <filesystem>
#include <string>

namespace Loopie {

    struct Metadata {
        UUID UUID;
        std::vector<std::string> CachesPath;

        bool HasCache = false;
    };

    class MetadataRegistry {
    public:
        static Metadata GetMetadataAsset(const std::string& assetPath);
        static void SaveMetadata(const std::filesystem::path& assetPath, const Metadata& metadata);

        static bool IsMetadataFile(const std::filesystem::path& assetPath);
    };
}