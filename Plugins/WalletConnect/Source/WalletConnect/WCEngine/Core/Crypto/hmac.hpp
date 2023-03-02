#include "SHA256.hpp"
#include "SHA512.hpp"
#include <string>

namespace hmac {

	enum class TypeHash {
        SHA256 = 0,
        SHA512 = 1,
    };

    std::string to_hex(const std::string& input, const bool is_upper = false) {
        static const char *lut = "0123456789abcdef0123456789ABCDEF";
        const char *symbol = is_upper ? lut + 16 : lut;
        const size_t length = input.size();
        std::string output;
        output.reserve(2 * length);
        for (size_t i = 0; i < length; ++i) {
            const uint8_t c = input[i];
            output.push_back(symbol[c >> 4]);
            output.push_back(symbol[c & 15]);
        }
        return output;
    }

    std::string get_hash(const std::string &input, const TypeHash type) {
        switch(type) {
        case TypeHash::SHA256: {
                uint8_t digest[wc_hmac_hash::SHA256::DIGEST_SIZE];
                std::fill(digest, digest + wc_hmac_hash::SHA256::DIGEST_SIZE, '\0');
                wc_hmac_hash::SHA256 ctx = wc_hmac_hash::SHA256();
                ctx.init();
                ctx.update((uint8_t*)input.c_str(), input.size());
                ctx.final(digest);
                return std::string((const char*)digest, wc_hmac_hash::SHA256::DIGEST_SIZE);
            }
            break;
        case TypeHash::SHA512: {
                uint8_t digest[SHA512::DIGEST_SIZE];
                std::fill(digest, digest + SHA512::DIGEST_SIZE, '\0');
                SHA512 ctx = SHA512();
                ctx.init();
                ctx.update((uint8_t*)input.c_str(), input.size());
                ctx.final(digest);
                return std::string((const char*)digest, SHA512::DIGEST_SIZE);
            }
            break;
        default:
            return std::string();
            break;
        };
        return std::string();
    }

    std::string get_hmac(std::string key, const std::string &msg, const TypeHash type, const bool is_hex = true, const bool is_upper = false) {
        size_t block_size = 0;
        switch(type) {
        case TypeHash::SHA256:
            block_size = wc_hmac_hash::SHA256::SHA224_256_BLOCK_SIZE;
            break;
        case TypeHash::SHA512:
            block_size = SHA512::SHA384_512_BLOCK_SIZE;
            break;
        default:
            return std::string();
            break;
        };

        if(key.size() > block_size) {
            key = get_hash(key, type);
        }
        if(key.size() < block_size) {
            key.resize(block_size, '\0');
        }

        std::string ikeypad;
        std::string okeypad;
        ikeypad.reserve(block_size);
        okeypad.reserve(block_size);
        for(size_t i = 0; i < block_size; ++i) {
            ikeypad.push_back('\x36' ^ key[i]);
            okeypad.push_back('\x5c' ^ key[i]);
        }
        if(is_hex) return to_hex(get_hash(okeypad + get_hash(ikeypad + msg, type), type), is_upper);
        return get_hash(okeypad + get_hash(ikeypad + msg, type), type);
    }
}
