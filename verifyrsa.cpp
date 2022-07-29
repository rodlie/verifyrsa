// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#include "verifyrsa.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

//#include <assert.h>
#include <string.h>

RSA* createPrivateRSA(const std::string &key) {
  RSA *rsa = NULL;
  const char* c_string = key.c_str();
  BIO * keybio = BIO_new_mem_buf(reinterpret_cast<const void*>(c_string), -1);
  if (keybio == NULL) { return NULL; }
  rsa = PEM_read_bio_RSAPrivateKey(keybio,
                                   &rsa,
                                   NULL,
                                   NULL);
  BIO_free(keybio);
  return rsa;
}

RSA* createPublicRSA(const std::string &key) {
  RSA *rsa = NULL;
  BIO *keybio;
  const char* c_string = key.c_str();
  keybio = BIO_new_mem_buf(reinterpret_cast<const void*>(c_string), -1);
  if (keybio == NULL) { return NULL; }
  rsa = PEM_read_bio_RSAPublicKey(keybio,
                                  &rsa,
                                  NULL,
                                  NULL);
  BIO_free(keybio);
  return rsa;
}

bool RSASign( RSA* rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc) {
  EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
  EVP_PKEY* priKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(priKey, rsa);
  if (EVP_DigestSignInit(m_RSASignCtx,
                         NULL,
                         EVP_sha256(),
                         NULL,
                         priKey)<=0)
  {
      EVP_PKEY_free(priKey);
      return false;
  }
  if (EVP_DigestSignUpdate(m_RSASignCtx,
                           Msg,
                           MsgLen) <= 0)
  {
      EVP_PKEY_free(priKey);
      return false;
  }
  if (EVP_DigestSignFinal(m_RSASignCtx,
                          NULL,
                          MsgLenEnc) <=0)
  {
      EVP_PKEY_free(priKey);
      return false;
  }
  *EncMsg = reinterpret_cast<unsigned char*>(malloc(*MsgLenEnc));
  if (EVP_DigestSignFinal(m_RSASignCtx,
                          *EncMsg,
                          MsgLenEnc) <= 0)
  {
      EVP_PKEY_free(priKey);
      return false;
  }
  EVP_PKEY_free(priKey);
  EVP_MD_CTX_free(m_RSASignCtx);
  return true;
}

bool RSAVerifySignature( RSA* rsa,
                         unsigned char* MsgHash,
                         size_t MsgHashLen,
                         const char* Msg,
                         size_t MsgLen,
                         bool* Authentic) {
  *Authentic = false;
  EVP_PKEY* pubKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(pubKey, rsa);
  EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

  if (EVP_DigestVerifyInit(m_RSAVerifyCtx,
                           NULL,
                           EVP_sha256(),
                           NULL,
                           pubKey)<=0)
  {
    EVP_PKEY_free(pubKey);
    return false;
  }
  if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx,
                             Msg,
                             MsgLen) <= 0)
  {
    EVP_PKEY_free(pubKey);
    return false;
  }
  int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx,
                                         MsgHash,
                                         MsgHashLen);
  if (AuthStatus==1) {
    *Authentic = true;
    EVP_PKEY_free(pubKey);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return true;
  } else if(AuthStatus==0){
    *Authentic = false;
    EVP_PKEY_free(pubKey);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return true;
  } else{
    *Authentic = false;
    EVP_PKEY_free(pubKey);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return false;
  }
}

void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_write(bio, buffer, static_cast<int>(length));
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  *base64Text=(*bufferPtr).data;
}

size_t calcDecodeLength(const char* b64input) {
  size_t len = strlen(b64input), padding = 0;

  if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
    padding = 2;
  else if (b64input[len-1] == '=') //last char is =
    padding = 1;
  return (len*3)/4 - padding;
}

void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length) {
  BIO *bio, *b64;

  int decodeLen = static_cast<int>(calcDecodeLength(b64message));
  *buffer = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(decodeLen) + 1));
  (*buffer)[decodeLen] = '\0';

  bio = BIO_new_mem_buf(b64message, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  *length = BIO_read(bio, *buffer, strlen(b64message));
  BIO_free_all(bio);
}

char *VerifyRSA::sign(std::string privateKey, std::string plainText)
{
    if (privateKey.find("-----BEGIN RSA PRIVATE KEY-----",0)!=0 ||
        plainText.empty())
    {
        return NULL;
    }
    RSA* privateRSA = createPrivateRSA(privateKey);
    unsigned char* encMessage;
    char* base64Text;
    size_t encMessageLength;
    RSASign(privateRSA, (unsigned char*)plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
    Base64Encode(encMessage, encMessageLength, &base64Text);
    free(encMessage);
    return base64Text;
}

bool VerifyRSA::verify(std::string publicKey, std::string plainText, char *signatureBase64)
{
    if (publicKey.find("-----BEGIN RSA PUBLIC KEY-----",0)!=0 ||
        plainText.empty())
    {
        return false;
    }
    if (sizeof(signatureBase64)<1) {
        std::cerr << "Not a valid signature!" << std::endl;
        return false;
    }
    RSA* publicRSA = createPublicRSA(publicKey);
    unsigned char* encMessage;
    size_t encMessageLength;
    bool authentic;
    Base64Decode(signatureBase64, &encMessage, &encMessageLength);
    bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText.c_str(), plainText.length(), &authentic);
    free(encMessage);
    return result & authentic;
}

bool VerifyRSA::generate(std::string key, int bits)
{
    if (key.empty() || bits == 0 ) { return false; }

    int ret = 0;
    RSA *r = NULL;
    BIGNUM *bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    unsigned long e = RSA_F4;

    bne = BN_new();
    ret = BN_set_word(bne,e);
    if(ret != 1){
        BN_free(bne);
        return  false;
    }

    r = RSA_new();
    ret = RSA_generate_key_ex(r, bits, bne, NULL);
    if(ret != 1){
        RSA_free(r);
        BN_free(bne);
        return false;
    }

    std::string publicFile = key;
    publicFile.append("_public.pem");
    bp_public = BIO_new_file(publicFile.c_str(), "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, r);

    if(ret != 1){
        BIO_free_all(bp_public);
        RSA_free(r);
        BN_free(bne);
    }

    std::string privateFile = key;
    privateFile.append("_private.pem");
    bp_private = BIO_new_file(privateFile.c_str(), "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(r);
    BN_free(bne);

    return (ret == 1);
}
