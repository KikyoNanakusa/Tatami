#include "KeyUtils.h"

SHORT getVkCodeFromChar(char ch) {
    // ������啶���ɕϊ����Ă��牼�z�L�[�R�[�h���擾
    char upperCh = std::toupper(static_cast<unsigned char>(ch));
    return VkKeyScan(upperCh);
}