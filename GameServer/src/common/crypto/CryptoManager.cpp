#include "pch.h"
#include "CryptoManager.h"
#include <openssl/rand.h>


thread_local EVP_MD_CTX* CryptoManager::LMdctx = nullptr;
thread_local EVP_CIPHER_CTX* CryptoManager::LCipherctx = nullptr;
std::once_flag CryptoManager::_initFlag;

void CryptoManager::Init()
{
    std::call_once(_initFlag, []() 
    {
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_LOAD_CONFIG, NULL);
    });

    // 알고리즘 초기화
    if (LMdctx == nullptr)
    {
        LMdctx = EVP_MD_CTX_new();
    }
    else
    {
        EVP_MD_CTX_reset(LMdctx);
    }

    // 암호화 컨텍스트 초기화
    if (LCipherctx == nullptr)
    {
        LCipherctx = EVP_CIPHER_CTX_new();
    }
    else
    {
        EVP_CIPHER_CTX_reset(LCipherctx);
    }
}

void CryptoManager::CleanUp()
{
    if (LMdctx != nullptr)
    {
        EVP_MD_CTX_free(LMdctx);

        LMdctx = nullptr;
    }
    if (LCipherctx != nullptr)
    {
        EVP_CIPHER_CTX_free(LCipherctx);

        LCipherctx = nullptr;
    }
}

#pragma region SHA256
bool CryptoManager::Sha256Encrypt(const char* data, unsigned char* hash)
{
    if (LMdctx == nullptr || data == nullptr)
    {
        return false;
    }

    if (EVP_DigestInit_ex(LMdctx, EVP_sha256(), NULL) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    if (EVP_DigestUpdate(LMdctx, data, strlen(data)) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    unsigned int hashLen{};

    if (EVP_DigestFinal_ex(LMdctx, hash, &hashLen) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    EVP_MD_CTX_reset(LMdctx);

    return true;
}
bool CryptoManager::Sha256Encrypt(const std::string& data, std::string& hash)
{
    unsigned char hashBuffer[EVP_MAX_MD_SIZE];
    unsigned int hashLen{ 32 };

    if (!Sha256Encrypt(data.c_str(), hashBuffer))
    {
        return false;
    }

    hash.assign(reinterpret_cast<const char*>(hashBuffer), hashLen);

    return true;
}
bool CryptoManager::Sha256Encrypt(const std::wstring& data, std::wstring& hash)
{
	unsigned char hashBuffer[EVP_MAX_MD_SIZE];
    unsigned int hashLen{ 32 };

	if (!Sha256Encrypt(reinterpret_cast<const char*>( data.c_str() ), hashBuffer))
	{
		return false;
	}

	hash.assign(reinterpret_cast<const wchar_t*>( hashBuffer ), hashLen);
 
	return true;
}

bool CryptoManager::Sha256Verify(const char* data, const unsigned char* hash)
{
    if (data == nullptr || hash == nullptr)
    {
        return false;
    }

    unsigned char compareHash[EVP_MAX_MD_SIZE];
    unsigned int hashLen{ 32 };

    if (!Sha256Encrypt(data, compareHash))
    {
        return false;
    }

    return (CRYPTO_memcmp(compareHash, hash, hashLen) == 0);
}
bool CryptoManager::Sha256Verify(const std::string& data, const std::string& hash)
{
    return Sha256Verify(data.c_str(), reinterpret_cast<const unsigned char*>(hash.c_str()));
}
bool CryptoManager::Sha256Verify(const std::wstring& data, const std::wstring& hash)
{
	return Sha256Verify(reinterpret_cast<const char*>( data.c_str() ), reinterpret_cast<const unsigned char*>( hash.c_str() ));
}
#pragma endregion

#pragma region SHA512
bool CryptoManager::Sha512Encrypt(const char* data, unsigned char* hash)
{
    if (LMdctx == nullptr || data == nullptr)
    {
        return false;
    }

    if (EVP_DigestInit_ex(LMdctx, EVP_sha512(), NULL) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    if (EVP_DigestUpdate(LMdctx, data, strlen(data)) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    unsigned int hashLen{};

    if (EVP_DigestFinal_ex(LMdctx, hash, &hashLen) != 1)
    {
        EVP_MD_CTX_reset(LMdctx);

        return false;
    }

    EVP_MD_CTX_reset(LMdctx);

    return true;
}
bool CryptoManager::Sha512Encrypt(const std::string& data, std::string& hash)
{
    unsigned char hashBuffer[EVP_MAX_MD_SIZE];
    unsigned int hashLen{};

    if (!Sha512Encrypt(data.c_str(), hashBuffer))
    {
        return false;
    }

    hash.assign(reinterpret_cast<const char*>(hashBuffer), EVP_MAX_MD_SIZE);

    return true;
}
bool CryptoManager::Sha512Encrypt(const std::wstring& data, std::wstring& hash)
{
    unsigned char hashBuffer[EVP_MAX_MD_SIZE]{};
    unsigned int hashLen{ 32 };

    const string& str{ StringUtils::WstringToString(data) };

	if (!Sha512Encrypt(str.c_str(), hashBuffer))
	{
		return false;
	}

	hash.assign(reinterpret_cast<const wchar_t*>( hashBuffer ), hashLen);

	return true;
}

bool CryptoManager::Sha512Verify(const char* data, const unsigned char* hash)
{
    if (data == nullptr || hash == nullptr)
    {
        return false;
    }

    unsigned char compareHash[EVP_MAX_MD_SIZE];
    unsigned int hashLen{};

    if (!Sha512Encrypt(data, compareHash))
    {
        return false;
    }

    // 실제 hashLen을 사용하여 비교
    return (CRYPTO_memcmp(compareHash, hash, hashLen) == 0);
}
bool CryptoManager::Sha512Verify(const std::string& data, const std::string& hash)
{
    return Sha512Verify(data.c_str(), reinterpret_cast<const unsigned char*>( hash.c_str() ));
}
bool CryptoManager::Sha512Verify(const std::wstring& data, const std::wstring& hash)
{
    return Sha512Verify(reinterpret_cast<const char*>( data.c_str() ), reinterpret_cast<const unsigned char*>( hash.c_str() ));
}
#pragma endregion

#pragma region AES256
bool CryptoManager::Aes256Encrypt(const unsigned char* plaintext, int plaintextLen, const unsigned char* key, const unsigned char* iv, unsigned char* ciphertext)
{
    if (LCipherctx == nullptr || plaintext == nullptr || key == nullptr || iv == nullptr || ciphertext == nullptr || plaintextLen <= 0)
    {
        return false;
    }

    const int keyLength{ 32 };

    int len{};

    if (EVP_EncryptInit_ex(LCipherctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    if (EVP_EncryptUpdate(LCipherctx, ciphertext, &len, plaintext, plaintextLen) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    if (EVP_EncryptFinal_ex(LCipherctx, ciphertext + len, &len) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    EVP_CIPHER_CTX_reset(LCipherctx);

    return true;
}

bool CryptoManager::Aes256Encrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext)
{
    if (key.size() != 32)
    {
        return false;
    }

    unsigned char iv[16];

    if (RAND_bytes(iv, sizeof(iv)) != 1)
    {
        return false;
    }

    // 암호화 결과 버퍼 (평문 크기 + 패딩 공간(블록 크기) + IV 크기)
    const int maxCiphertextLen{ static_cast<int>(plaintext.size() + 16 + 16 )};

    std::vector<unsigned char> ciphertextBuffer(maxCiphertextLen);

    if (!Aes256Encrypt(reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size(), 
        reinterpret_cast<const unsigned char*>(key.data()), iv, ciphertextBuffer.data()))
    {
        return false;
    }

    const int ciphertextLen{ static_cast<int>(( ( plaintext.size() / 16 ) + 1 ) * 16 )};

    ciphertext.assign(reinterpret_cast<char*>(ciphertextBuffer.data()), ciphertextLen);
    ciphertext.append(reinterpret_cast<char*>(iv), sizeof(iv));

    return true;
}

bool CryptoManager::Aes256Encrypt(const std::wstring& plaintext, const std::wstring& key, std::wstring& ciphertext)
{
    std::string plaintextBytes(reinterpret_cast<const char*>(plaintext.data()), plaintext.size() * sizeof(wchar_t));
    std::string keyBytes(reinterpret_cast<const char*>(key.data()), key.size() * sizeof(wchar_t));
    std::string ciphertextBytes;

    if (!Aes256Encrypt(plaintextBytes, keyBytes, ciphertextBytes))
    {
        return false;
    }

    ciphertext.assign(reinterpret_cast<const wchar_t*>( ciphertextBytes.data() ), ciphertextBytes.size() / sizeof(wchar_t));

    return true;
}


bool CryptoManager::Aes256Decrypt(const unsigned char* ciphertext, int ciphertextLen, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext)
{
    if (LCipherctx == nullptr || ciphertext == nullptr || key == nullptr || iv == nullptr || plaintext == nullptr || ciphertextLen <= 0)
    {
        return false;
    }

    int len{};

    if (EVP_DecryptInit_ex(LCipherctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    if (EVP_DecryptUpdate(LCipherctx, plaintext, &len, ciphertext, ciphertextLen) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    if (EVP_DecryptFinal_ex(LCipherctx, plaintext + len, &len) != 1)
    {
        EVP_CIPHER_CTX_reset(LCipherctx);

        return false;
    }

    EVP_CIPHER_CTX_reset(LCipherctx);

    return true;
}

bool CryptoManager::Aes256Decrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext)
{
    if (key.size() != 32)
    {
        return false;
    }

    const size_t ivSize{ 16 };

    if (ciphertext.size() <= ivSize)
    {
        return false;
    }

    const unsigned char* iv{ reinterpret_cast<const unsigned char*>( ciphertext.data() + ciphertext.size() - ivSize ) };
    const unsigned char* ciphertextData{ reinterpret_cast<const unsigned char*>( ciphertext.data() ) };
    const int ciphertextLen{ static_cast<int>(ciphertext.size() - ivSize) };

    std::vector<unsigned char> plaintextBuffer(ciphertextLen);

    if (!Aes256Decrypt(ciphertextData, ciphertextLen, reinterpret_cast<const unsigned char*>(key.data()),
            iv, plaintextBuffer.data()))
    {
        return false;
    }

    int actualPlaintextLen{};

    for (int i = 0; i < ciphertextLen; ++i)
    {
        if (plaintextBuffer[i] == 0) 
        {
            actualPlaintextLen = i;

            break;
        }

        actualPlaintextLen = i + 1;
    }

    plaintext.assign(reinterpret_cast<char*>(plaintextBuffer.data()), actualPlaintextLen);
    
    return true;
}

bool CryptoManager::Aes256Decrypt(const std::wstring& ciphertext, const std::wstring& key, std::wstring& plaintext)
{
    std::string ciphertextBytes(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size() * sizeof(wchar_t));
    std::string keyBytes(reinterpret_cast<const char*>(key.data()), key.size() * sizeof(wchar_t));
    std::string plaintextBytes;

    if (!Aes256Decrypt(ciphertextBytes, keyBytes, plaintextBytes))
    {
        return false;
    }

    plaintext.assign(reinterpret_cast<const wchar_t*>( plaintextBytes.data() ), plaintextBytes.size() / sizeof(wchar_t));

    return true;
}

