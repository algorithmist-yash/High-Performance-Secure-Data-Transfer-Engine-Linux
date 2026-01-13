#include "crypto.h"
#include <openssl/evp.h>
#include <stdexcept>

std::vector<unsigned char> aes_encrypt(
    const std::vector<unsigned char>& plaintext,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Encrypt ctx failed");

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key.data(), iv.data());

    std::vector<unsigned char> ciphertext;
    ciphertext.reserve(plaintext.size() + 16);

    int len = 0;
    ciphertext.resize(plaintext.size() + 16);

    EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                      plaintext.data(), plaintext.size());
    int total = len;

    EVP_EncryptFinal_ex(ctx, ciphertext.data() + total, &len);
    total += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(total);
    return ciphertext;
}

std::vector<unsigned char> aes_decrypt(
    const std::vector<unsigned char>& ciphertext,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Decrypt ctx failed");

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key.data(), iv.data());

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len = 0, total = 0;

    EVP_DecryptUpdate(ctx, plaintext.data(), &len,
                      ciphertext.data(), ciphertext.size());
    total += len;

    EVP_DecryptFinal_ex(ctx, plaintext.data() + total, &len);
    total += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(total);
    return plaintext;
}
