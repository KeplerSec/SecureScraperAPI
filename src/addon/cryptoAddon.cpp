#include <napi.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <string>
#include <vector>

// Fonction SHA-256 (réutilisée de SecureUserAPI)
Napi::Value HashPassword(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "Chaîne attendue").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string password = info[0].As<Napi::String>().Utf8Value();
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char*)password.c_str(), password.length(), hash);
  char hex[2 * SHA256_DIGEST_LENGTH + 1];
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    snprintf(hex + 2 * i, 3, "%02x", hash[i]);
  }
  hex[2 * SHA256_DIGEST_LENGTH] = '\0';
  return Napi::String::New(env, hex);
}

// Chiffrement AES-256
Napi::Value Encrypt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Deux chaînes attendues (data, key)").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string data = info[0].As<Napi::String>().Utf8Value();
  std::string key = info[1].As<Napi::String>().Utf8Value();

  // Clé doit être 32 bytes (256 bits) pour AES-256
  unsigned char aes_key[32];
  memset(aes_key, 0, 32);
  memcpy(aes_key, key.c_str(), std::min(key.length(), size_t(32)));

  // Initialisation AES
  AES_KEY enc_key;
  AES_set_encrypt_key(aes_key, 256, &enc_key);

  // Chiffrement
  std::vector<unsigned char> ciphertext(data.length() + AES_BLOCK_SIZE);
  int len = data.length();
  AES_encrypt((unsigned char*)data.c_str(), ciphertext.data(), &enc_key);

  // Convertir en hex
  char hex[2 * len + 1];
  for (int i = 0; i < len; i++) {
    snprintf(hex + 2 * i, 3, "%02x", ciphertext[i]);
  }
  hex[2 * len] = '\0';
  return Napi::String::New(env, hex);
}

// Déchiffrement AES-256 (simplifié, à compléter)
Napi::Value Decrypt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Deux chaînes attendues (ciphertext, key)").ThrowAsJavaScriptException();
    return env.Null();
  }
  // Implémenter déchiffrement (similaire à Encrypt, avec AES_decrypt)
  return Napi::String::New(env, "Not implemented");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("hashPassword", Napi::Function::New(env, HashPassword));
  exports.Set("encrypt", Napi::Function::New(env, Encrypt));
  exports.Set("decrypt", Napi::Function::New(env, Decrypt));
  return exports;
}

NODE_API_MODULE(cryptoAddon, Init)