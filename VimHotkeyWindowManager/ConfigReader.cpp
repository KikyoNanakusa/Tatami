#include "ConfigReader.h"

bool isFileExists(const std::string& filename) {
    DWORD fileAttributes = GetFileAttributesA(filename.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        // �t�@�C�������݂��Ȃ��ꍇ�A�܂��̓A�N�Z�X�ł��Ȃ��ꍇ
        return false;
    }
    return true;
}

bool createDirectoryIfNotExists(const std::string& dirPath) {
    DWORD attributes = GetFileAttributesA(dirPath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(dirPath.c_str(), NULL)) {
            std::cerr << "�f�B���N�g���̍쐬�Ɏ��s���܂���: " << GetLastError() << std::endl;
            return false;
        }
        return true;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool createFileIfNotExists(const std::string& filename) {
    if (isFileExists(filename)) {
        std::cout << "�t�@�C���͊��ɑ��݂��܂�: " << filename << std::endl;
        return true;
    } else {
        HANDLE hFile = CreateFileA(
            filename.c_str(),          // �t�@�C����
            GENERIC_WRITE,             // �������݃A�N�Z�X
            0,                         // ���L���[�h�Ȃ�
            NULL,                      // �Z�L�����e�B���� (�f�t�H���g)
            CREATE_NEW,                // �t�@�C�������݂��Ȃ��ꍇ�͍쐬
            FILE_ATTRIBUTE_NORMAL,     // �ʏ�̃t�@�C������
            NULL                       // �e���v���[�g�t�@�C���Ȃ�
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "�t�@�C���̍쐬�Ɏ��s���܂���: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "�V�����t�@�C�����쐬���܂���: " << filename << std::endl;
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
        filename.c_str(),          // �t�@�C����
        GENERIC_READ,              // �ǂݎ��A�N�Z�X
        0,                         // ���L���[�h�Ȃ�
        NULL,                      // �Z�L�����e�B���� (�f�t�H���g)
        OPEN_EXISTING,             // �����̃t�@�C�����J��
        FILE_ATTRIBUTE_NORMAL,     // �ʏ�̃t�@�C������
        NULL                       // �e���v���[�g�t�@�C���Ȃ�
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "�t�@�C�����J���܂���ł���: " << GetLastError() << std::endl;
        return "";
    }

    // �t�@�C���T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "�t�@�C���T�C�Y�̎擾�Ɏ��s���܂���: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return "";
    }

    // �t�@�C�����e���i�[����o�b�t�@���쐬
    std::vector<char> buffer(fileSize);

    // �t�@�C���̓��e��ǂݎ��
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL)) {
        std::cerr << "�t�@�C���̓ǂݎ��Ɏ��s���܂���: " << GetLastError() << std::endl;
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
