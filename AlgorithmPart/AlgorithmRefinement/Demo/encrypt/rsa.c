//
// Created by speauty on 2021/3/8.
//
#include "encrypt.h"

static Huge modExp(Huge a, Huge b, Huge n)
{
    Huge y;
    y = 1;
    while (b != 0) {
        if (b&1) y = (y*a)%n;
        a = (a*a)%n;
        /** 右移 */
        b = b >> 1;
    }
    return y;
}

void rsa_encipher(Huge plainText, Huge *cipherText, RsaPubKey pubKey)
{
    *cipherText = modExp(plainText, pubKey.e, pubKey.n);
}

void rsa_decipher(Huge cipherText, Huge *plainText, RsaPriKey priKey)
{
    *plainText = modExp(cipherText, priKey.d, priKey.n);
}