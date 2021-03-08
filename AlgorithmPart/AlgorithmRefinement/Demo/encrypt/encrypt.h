//
// Created by speauty on 2021/3/8.
//

#ifndef DEMO_ENCRYPT_H
#define DEMO_ENCRYPT_H

typedef unsigned long Huge;

/** RSA公钥结构 */
typedef struct __RsaPubKey
{
    Huge e;
    Huge n;
} RsaPubKey;
/** RSA私钥结构 */
typedef struct __RsaPriKey
{
    Huge d;
    Huge n;
} RsaPriKey;
void rsa_encipher(Huge plainText, Huge *cipherText, RsaPubKey pubKey);
void rsa_decipher(Huge cipherText, Huge *plainText, RsaPriKey priKey);

void des_encipher(const unsigned char *plainText, unsigned char *cipherText, const unsigned char *key);
void des_decipher(const unsigned char *cipherText, unsigned char *plainText, const unsigned char *key);




















#endif //DEMO_ENCRYPT_H
