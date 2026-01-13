#ifndef CRYPTO_H
#define CRYPTO_H

#include <vector>

std::vector<unsigned char> aes_encrypt(
    const std::vector<unsigned char>& plaintext,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv
);

std::vector<unsigned char> aes_decrypt(
    const std::vector<unsigned char>& ciphertext,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv
);

#endif
