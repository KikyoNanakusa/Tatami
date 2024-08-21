#include "ConfigReader.h"

bool isFileExists(const std::string& filename) {
    DWORD fileAttributes = GetFileAttributesA(filename.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        // ファイルが存在しない場合、またはアクセスできない場合
        return false;
    }
    return true;
}

bool createDirectoryIfNotExists(const std::string& dirPath) {
    DWORD attributes = GetFileAttributesA(dirPath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(dirPath.c_str(), NULL)) {
            std::cerr << "ディレクトリの作成に失敗しました: " << GetLastError() << std::endl;
            return false;
        }
        return true;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool createFileIfNotExists(const std::string& filename) {
    if (isFileExists(filename)) {
        std::cout << "ファイルは既に存在します: " << filename << std::endl;
        return true;
    } else {
        HANDLE hFile = CreateFileA(
            filename.c_str(),          // ファイル名
            GENERIC_WRITE,             // 書き込みアクセス
            0,                         // 共有モードなし
            NULL,                      // セキュリティ属性 (デフォルト)
            CREATE_NEW,                // ファイルが存在しない場合は作成
            FILE_ATTRIBUTE_NORMAL,     // 通常のファイル属性
            NULL                       // テンプレートファイルなし
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "ファイルの作成に失敗しました: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "新しいファイルを作成しました: " << filename << std::endl;
        CloseHandle(hFile);
        return true;
    }
}

std::string GetAppDataPath(const std::string& appName) {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        return std::string(path) + "\\" + appName;
    }
    return "";
}

std::string ReadFileContents(const std::string& filename) {
    HANDLE hFile = CreateFileA(
        filename.c_str(),          // ファイル名
        GENERIC_READ,              // 読み取りアクセス
        0,                         // 共有モードなし
        NULL,                      // セキュリティ属性 (デフォルト)
        OPEN_EXISTING,             // 既存のファイルを開く
        FILE_ATTRIBUTE_NORMAL,     // 通常のファイル属性
        NULL                       // テンプレートファイルなし
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "ファイルを開けませんでした: " << GetLastError() << std::endl;
        return "";
    }

    // ファイルサイズを取得
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "ファイルサイズの取得に失敗しました: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return "";
    }

    // ファイル内容を格納するバッファを作成
    std::vector<char> buffer(fileSize);

    // ファイルの内容を読み取る
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL)) {
        std::cerr << "ファイルの読み取りに失敗しました: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return "";
    }

    CloseHandle(hFile);
    return std::string(buffer.begin(), buffer.end());
}

std::string ReadConfigFile() {
    std::string appDataPath = GetAppDataPath("VWindowManager");

    if (appDataPath.empty()) {
        return "";
    }

    if (!createDirectoryIfNotExists(appDataPath)) {
        return "";
    }

    std::string configPath = appDataPath + "\\config.txt";
    if (!createFileIfNotExists(configPath)) {
        return "";
	}

    std::string fileContents = ReadFileContents(configPath);
    if (fileContents.empty()) {
        return "";
    }

	return fileContents;
}
