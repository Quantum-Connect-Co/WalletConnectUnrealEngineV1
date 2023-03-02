#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include "openssl/evp.h"
#include <openssl/aes.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
THIRD_PARTY_INCLUDES_END
#undef UI
#include <list>
#define KEY_LENGHT 32
#define IV_LENGHT 16

namespace crypto {

	 static const std::vector<uint8_t> generate_key() noexcept {
		std::vector<uint8_t> key;
		key.resize(KEY_LENGHT);
		RAND_bytes(&key[0], KEY_LENGHT);
		return key;
	}

	 static const std::vector<uint8_t> generate_iv() noexcept {
		std::list<uint8_t> iv;
		for (size_t i = 0; i < IV_LENGHT; i++) 
			iv.push_back(static_cast<uint8_t>
				(utils::generate_random_integer(0, 255)));

		return std::vector<uint8_t>(iv.begin(), iv.end());
	}

	 static const std::string generate_guid() noexcept {
		return std::string(TCHAR_TO_UTF8(*FGuid::NewGuid().
			ToString(EGuidFormats::DigitsWithHyphensLower)));
	};

	 static std::vector<uint8_t> encrypt_aes_256_cbc_PKCS7(const std::vector<uint8_t>& key,
		const std::vector<uint8_t>& iv, std::string str) noexcept {

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		EVP_CIPHER_CTX_init(ctx);

		assert(key.size() == 32);  // AES256 key size
		assert(iv.size() == 16);   // IV is always the AES block size
		assert(ctx != nullptr);    
		// If data isn't a multiple of 16, the default behavior is to pad with
		// n bytes of value n, where n is the number of padding bytes required
		// to make data a multiple of the block size.  This is PKCS7 padding.
		// The output then will be a multiple of the block size.

		std::vector<uint8_t> encrypted;
		size_t max_output_len = str.length() + 16 - (str.length() % 16);

		encrypted.resize(max_output_len);
		// Enc is 1 to encrypt, 0 to decrypt, or -1 (see documentation).
		EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, &key[0], &iv[0], 1);

		// EVP_CipherUpdate can encrypt all your data at once, or you can do
		// small chunks at a time.
		int actual_size = 0;
		EVP_CipherUpdate(ctx, &encrypted[0], &actual_size, reinterpret_cast<unsigned char*>(&str[0]), str.size());

		// EVP_CipherFinal_ex is what applies the padding.  If your data is
		// a multiple of the block size, you'll get an extra AES block filled
		// with nothing but padding.
		int final_size;
		EVP_CipherFinal_ex(ctx, &encrypted[actual_size], &final_size);

		actual_size += final_size;
		encrypted.resize(actual_size);

		EVP_CIPHER_CTX_cleanup(ctx);
		EVP_CIPHER_CTX_free(ctx);

		return encrypted;
	}

	 static std::string decrypt_aes_256_cbc_PKCS7(const std::string& cipher_str,
		const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv) noexcept {
		
		auto ciphertext = utils::convert_hex_string_to_vector_uchar(cipher_str);

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		EVP_CIPHER_CTX_init(ctx);

		assert(key.size() == 32 && iv.size() == 16 && ctx != nullptr);

		std::vector<uint8_t> plaintext;
		size_t max_output_len = ciphertext.size();
		plaintext.resize(max_output_len);
		
		EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, &key[0], &iv[0]);

		// EVP_DecryptUpdate can decrypt all your data at once, or you can do
		// small chunks at a time.
		int actual_size = 0;
		EVP_DecryptUpdate(ctx, &plaintext[0], &actual_size, &ciphertext[0], ciphertext.size());

		int final_size = 0;
		EVP_DecryptFinal_ex(ctx, &plaintext[actual_size], &final_size);


		actual_size += final_size;
		plaintext.resize(actual_size);

		EVP_CIPHER_CTX_cleanup(ctx);
		EVP_CIPHER_CTX_free(ctx);

		return std::string(plaintext.begin(), plaintext.end());
	}

	 static std::string create_hmmac_sha256(const std::vector<uint8_t>& key,
		std::vector<uint8_t>& data) noexcept {
	
		unsigned int hmac_size = 32;
		std::vector<uint8_t> result;
		result.resize(hmac_size);
		HMAC_CTX* hmac = HMAC_CTX_new();
		assert(hmac != nullptr);
		HMAC_CTX_reset(hmac);
		HMAC_Init(hmac, &key[0], key.size(), EVP_sha256());
		HMAC_Update(hmac, &data.front(), data.size());
		HMAC_Final(hmac, &result[0], &hmac_size);
		HMAC_CTX_free(hmac);

		return std::string(result.begin(), result.end());
	}

	 static models::encrypted_payload generate_encrypted_payload(const std::vector<uint8_t>& key,
		const std::vector<uint8_t>& iv, std::string str) noexcept {
		
		auto aes_msg = encrypt_aes_256_cbc_PKCS7(key, iv, str);
		std::vector<uint8_t> signature = aes_msg;
		signature.insert(signature.end(), iv.begin(), iv.end());

		return models::encrypted_payload(
			utils::convert_string_to_hex(std::string(iv.begin(), iv.end())),
			utils::convert_string_to_hex(create_hmmac_sha256(key, signature)),
			utils::convert_string_to_hex(std::string(aes_msg.begin(), aes_msg.end()))
		);
	}
}