#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

#define BASE64_ENCODE_LEN(len) (((len + 2) / 3 * 4) + 1)

#ifdef __cplusplus
extern "C" {
#endif

int Base64EncodeLength(int len);
int Base64Encode(char *encoded, const char *string, int len);
    
#ifdef __cplusplus
}
#endif

#endif
