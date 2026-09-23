// Generates a unique "adjective-noun" codename directory, e.g. mkdir -p ./radiant-labrys
#include <array>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>

namespace fs = std::filesystem;

static constexpr std::array<const char*, 40> kAdjectives = {
    "sapphic",  "violet",   "lavender", "amethyst", "orchid",   "magenta",  "fuchsia",
    "rosy",     "dusky",    "tender",   "fierce",   "proud",    "radiant",  "luminous",
    "unapologetic","femme", "butch",    "androgynous","bold",   "gentle",   "wild",
    "free",     "warm",     "soft",     "vivid",    "blush",    "coral",    "plum",
    "wine",     "dawnlit",  "sunset",   "golden",   "dusty",    "velvet",   "silken",
    "starry",   "moonlit",  "brazen",   "devoted",  "loving",
};

static constexpr std::array<const char*, 40> kNouns = {
    "labrys",   "violet",   "orchid",   "iris",     "lilac",    "wisteria", "hyacinth",
    "peony",    "aster",    "tulip",    "magnolia", "jasmine",  "hydrangea","camellia",
    "dahlia",   "marigold", "poppy",    "zinnia",   "freesia",  "gardenia", "azalea",
    "begonia",  "calla",    "daffodil", "foxglove", "geranium", "heather",  "primrose",
    "sunflower","tuberose", "sappho",   "lesbos",   "amazon",   "muse",     "siren",
    "valkyrie", "nymph",    "goddess",  "vixen",    "rose",
};

std::string RandomCodename(std::mt19937& rng) {
  std::uniform_int_distribution<size_t> adj_dist(0, kAdjectives.size() - 1);
  std::uniform_int_distribution<size_t> noun_dist(0, kNouns.size() - 1);
  return std::string(kAdjectives[adj_dist(rng)]) + "-" + kNouns[noun_dist(rng)];
}

int main(int argc, char** argv) {
  fs::path base = ".";
  bool dry_run = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--dry-run") {
      dry_run = true;
    } else {
      base = arg;
    }
  }

  std::random_device rd;
  std::mt19937 rng(rd());

  constexpr int kMaxAttempts = 1000;
  for (int attempt = 0; attempt < kMaxAttempts; ++attempt) {
    fs::path candidate = base / RandomCodename(rng);

    if (dry_run) {
      if (!fs::exists(candidate)) {
        std::cout << candidate.string() << "\n";
        return 0;
      }
      continue;
    }

    std::error_code ec;
    if (fs::create_directories(candidate, ec)) {
      std::cout << candidate.string() << "\n";
      return 0;
    }
    if (ec && !fs::exists(candidate)) {
      std::cerr << "codegen: failed to create " << candidate << ": " << ec.message() << "\n";
      return 1;
    }
    // else: name collision, try again
  }

  std::cerr << "codegen: exhausted " << kMaxAttempts << " attempts without finding a unique name\n";
  return 1;
}
