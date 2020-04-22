#if defined(WIN32) || defined(__WIN32)
#include <filesystem>

namespace RCP
{
    typedef std::filesystem::path path;
}

#else
#include <experimental/filesystem>

namespace RCP
{
    typedef std::experimental::filesystem::path path;
}
#endif

