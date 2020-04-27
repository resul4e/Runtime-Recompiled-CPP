#if defined(WIN32) || defined(__WIN32)
#include <filesystem>

namespace RCP
{
    typedef std::filesystem::path path;
    typedef std::filesystem::directory_iterator directory_iterator;
    typedef std::filesystem::file_time_type file_time_type;
    typedef std::filesystem::recursive_directory_iterator recursive_directory_iterator;

}

#else
#include <experimental/filesystem>

namespace RCP
{
    typedef std::experimental::filesystem::path path;
    typedef std::experimental::filesystem::directory_iterator directory_iterator;
    typedef std::experimental::filesystem::file_time_type file_time_type;
    typedef std::experimental::filesystem::recursive_directory_iterator recursive_directory_iterator;
}
#endif

