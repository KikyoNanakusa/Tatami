#include "KeyUtils.h"

SHORT getVkCodeFromChar(char ch) {
    // 文字を大文字に変換してから仮想キーコードを取得
    char upperCh = std::toupper(static_cast<unsigned char>(ch));
    return VkKeyScan(upperCh);
}