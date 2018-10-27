//
// Created by josef on 27.10.18.
//


unsigned char hexToNum(char hex) {
    if (hex >= 'a' && hex <= 'f') {
        return static_cast<unsigned char>(hex - 'a' + 10);
    } else if (hex >= 'A' && hex <= 'F') {
        return static_cast<unsigned char>(hex - 'A' + 10);
    } else if (hex >= '0' && hex <= '9') {
        return static_cast<unsigned char>(hex - '0');
    } else {
        return 0;
    }
}

