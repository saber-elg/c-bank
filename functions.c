#include<openssl/sha.h>
#include
/*Prototype of functions*/
void hash_password(const char *password, char *hash);

/*Creation of functions*/
void hash_password(const char *password, char *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final((unsigned char*)hash, &sha256);
}