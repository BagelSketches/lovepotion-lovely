#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <vector>

namespace love {

struct Patch
{
    enum Type { PATTERN, REGEX, COPY, MODULE, OVERRIDE } type;
    std::string target;
    std::string pattern; // pattern string or regex
    std::string position; // before/after/at/append/prepend
    std::string payload; // payload for pattern
    bool match_indent = false;
    int times = -1; // -1 means all
    std::vector<std::string> sources; // copy sources
    int priority = 0;
    // For module
    std::string module_name;
    std::string source_file;
};

class ModManager
{
public:
    static ModManager& get();

    // Scan mods directory (lazy, idempotent)
    void scan();

    const std::vector<Patch>& getPatchesFor(const std::string& path) const;
    const std::vector<Patch>& getAllPatches() const { return patches; }

    // Overlay mount point used for modules/copy results
    std::string getOverlayDir() const { return overlayDir; }

private:
    ModManager();

    void parseLovelyToml(const std::string& tomlPath, const std::filesystem::path& baseDir);
    void mountOverlay();

    std::vector<Patch> patches;
    std::string overlayDir;

    bool scanned = false;
};

} // namespace love
