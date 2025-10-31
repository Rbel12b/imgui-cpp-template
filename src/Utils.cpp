#include "Utils.hpp"
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <array>
#include <memory>
#include <vector>
#include <curl/curl.h>
#include "AppState.hpp"
#include <functional>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <limits.h>
#include <fcntl.h>
#endif

namespace Utils
{
    static const std::string appName = "{{PROJECT_NAME}}";

// Windows helper
#ifdef _WIN32
    std::string getKnownFolder(REFKNOWNFOLDERID folderId)
    {
        PWSTR path = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(folderId, 0, nullptr, &path)))
        {
            char buffer[MAX_PATH];
            wcstombs(buffer, path, MAX_PATH);
            CoTaskMemFree(path);
            return std::string(buffer);
        }
        return {};
    }
#endif

    std::string getVideosDir()
    {
#ifdef _WIN32
        return getKnownFolder(FOLDERID_Videos);
#else
        const char *home = std::getenv("HOME");
        if (!home)
            home = "";
        return std::filesystem::path(home) / "Videos";
#endif
    }

    std::string getMusicDir()
    {
#ifdef _WIN32
        return getKnownFolder(FOLDERID_Music);
#else
        const char *home = std::getenv("HOME");
        if (!home)
            home = "";
        return std::filesystem::path(home) / "Music";
#endif
    }

    std::string getDownloadsDir()
    {
#ifdef _WIN32
        return getKnownFolder(FOLDERID_Downloads);
#else
        const char *home = std::getenv("HOME");
        if (!home)
            home = "";
        return std::filesystem::path(home) / "Downloads";
#endif
    }

    std::filesystem::path getExecutableDir()
    {
        return getExecutable().parent_path();
    }

    std::filesystem::path getExecutable()
    {
        std::filesystem::path exePath;
#ifdef _WIN32
        char buffer[MAX_PATH];
        DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        if (len != 0)
        {
            exePath = std::filesystem::path(buffer);
        }
#else
        // AppImage provides its own path in APPIMAGE
        if (const char *appImage = std::getenv("APPIMAGE"))
        {
            exePath = std::filesystem::path(appImage);
        }
        else
        {
            char buffer[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (len != -1)
            {
                buffer[len] = '\0';
                exePath = std::filesystem::path(buffer);
            }
        }
#endif
        return exePath;
    }

    std::filesystem::path getUserDataDir()
    {
        std::filesystem::path dataDir;

#ifdef _WIN32
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, path)))
        {
            dataDir = path;
            dataDir /= appName;
        }
#else
        const char *home = getenv("HOME");
        if (!home)
        {
            struct passwd *pw = getpwuid(getuid());
            if (pw)
                home = pw->pw_dir;
        }
        if (home)
        {
            dataDir = home;
            dataDir /= ".config";
            dataDir /= appName;
        }
#endif

        if (!dataDir.empty() && !std::filesystem::exists(dataDir))
        {
            std::filesystem::create_directories(dataDir);
        }

        return dataDir;
    }

    std::filesystem::path getTempDir()
    {
        std::filesystem::path tempDir;

#ifdef _WIN32
        char path[MAX_PATH];
        DWORD len = GetTempPathA(MAX_PATH, path);
        if (len > 0 && len < MAX_PATH)
        {
            tempDir = path;
            tempDir /= appName;
        }
#else
        const char *tmp = getenv("TMPDIR");
        if (!tmp || *tmp == '\0')
            tmp = "/tmp";
        tempDir = tmp;
        tempDir /= appName;
#endif

        if (!tempDir.empty() && !std::filesystem::exists(tempDir))
        {
            std::filesystem::create_directories(tempDir);
        }

        return tempDir;
    }

    std::filesystem::path getBundledExePath(const std::string &name)
    {
#ifdef _WIN32
        return getExecutableDir() / "bin" / (name + ".exe");
#else
        return name;
#endif
    }

    std::filesystem::path getBundledFilePath(const std::filesystem::path &name)
    {
#ifdef _WIN32
        return getExecutableDir() / name;
#else
        std::filesystem::path path;
        const char *appdir = std::getenv("APPDIR");
        if (!appdir)
        {
            std::cerr << "APPDIR not set!\n";
            path = "." / name;
        }
        else
        {
            path = std::filesystem::path(appdir) / "usr/bin" / name;
        }
        return path;
#endif
    }

} // namespace Utils
