#include "rcwpt/credentials.hpp"
#include <wincred.h>
#include <wincrypt.h>
#include <vector>

namespace rcwpt::credentials {

bool save_credential(const std::string& target_name, const std::string& secret) {
    std::wstring wtarget(target_name.begin(), target_name.end());
    CREDENTIALW cred{};
    cred.Type = CRED_TYPE_GENERIC;
    cred.TargetName = const_cast<LPWSTR>(wtarget.c_str());
    cred.CredentialBlobSize = static_cast<DWORD>(secret.size());
    cred.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<char*>(secret.data()));
    cred.Persist = CRED_PERSIST_LOCAL_MACHINE;

    return CredWriteW(&cred, 0) == TRUE;
}

std::optional<std::string> get_credential(const std::string& target_name) {
    // Check environment variable first
    const char* env_val = std::getenv(target_name.c_str());
    if (env_val && strlen(env_val) > 0) {
        return std::string(env_val);
    }

    std::wstring wtarget(target_name.begin(), target_name.end());
    PCREDENTIALW pCred = nullptr;
    if (CredReadW(wtarget.c_str(), CRED_TYPE_GENERIC, 0, &pCred)) {
        std::string secret(reinterpret_cast<char*>(pCred->CredentialBlob), pCred->CredentialBlobSize);
        CredFree(pCred);
        return secret;
    }
    return std::nullopt;
}

bool delete_credential(const std::string& target_name) {
    std::wstring wtarget(target_name.begin(), target_name.end());
    return CredDeleteW(wtarget.c_str(), CRED_TYPE_GENERIC, 0) == TRUE;
}

std::vector<uint8_t> protect_data(const std::vector<uint8_t>& plaintext) {
    DATA_BLOB inBlob{};
    inBlob.pbData = const_cast<BYTE*>(plaintext.data());
    inBlob.cbData = static_cast<DWORD>(plaintext.size());

    DATA_BLOB outBlob{};
    if (CryptProtectData(&inBlob, L"RCWPT Credential Protection", nullptr, nullptr, nullptr, 0, &outBlob)) {
        std::vector<uint8_t> result(outBlob.pbData, outBlob.pbData + outBlob.cbData);
        LocalFree(outBlob.pbData);
        return result;
    }
    return {};
}

std::vector<uint8_t> unprotect_data(const std::vector<uint8_t>& ciphertext) {
    DATA_BLOB inBlob{};
    inBlob.pbData = const_cast<BYTE*>(ciphertext.data());
    inBlob.cbData = static_cast<DWORD>(ciphertext.size());

    DATA_BLOB outBlob{};
    if (CryptUnprotectData(&inBlob, nullptr, nullptr, nullptr, nullptr, 0, &outBlob)) {
        std::vector<uint8_t> result(outBlob.pbData, outBlob.pbData + outBlob.cbData);
        LocalFree(outBlob.pbData);
        return result;
    }
    return {};
}

} // namespace rcwpt::credentials
