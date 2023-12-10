/*#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <openssl/sha.h>
#include "set_const.h"

// Prototyping the functions
void generateSalt(unsigned char *salt);
void hash_password(const char *password, const unsigned char *salt, unsigned char *hash);
int verify_password(const char *password, const unsigned char *salt, const unsigned char *storedHash);

// Function to generate a random salt
void generateSalt(unsigned char *salt) {
    FILE *urandom = fopen("/dev/urandom", "rb");
    fread(salt, 1, SALT_SIZE, urandom);
    fclose(urandom);
}

// Function to hash the password with the salt
void hash_password(const char *password, const unsigned char *salt, unsigned char *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salt, SALT_SIZE);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
}

// Function to verify a user's password during authentication
int verify_password(const char *password, const unsigned char *salt, const unsigned char *storedHash) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    hashPassword(password, salt, hash);
    return memcmp(hash, storedHash, SHA256_DIGEST_LENGTH) == 0;
}*/

/* demo
int main() {
    const char *userPassword = "user_password";
    unsigned char salt[SALT_SIZE];
    unsigned char storedHash[SHA256_DIGEST_LENGTH];

    // Generate a random salt
    generateSalt(salt);

    // Hash the password with the salt
    hashPassword(userPassword, salt, storedHash);

    // Simulate storing the hash (storedHash) without storing the actual password

    // Simulate authentication
    const char *userInputPassword = "user_password";
    if (verifyPassword(userInputPassword, salt, storedHash)) {
        printf("Authentication successful!\n");
    } else {
        printf("Authentication failed.\n");
    }

    return 0;
}

#endif*/