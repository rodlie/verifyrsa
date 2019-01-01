# RSA sign & verify - C++ library and utilities

Very simple library used to sign and verify text files. Based on example by [Ian Bull](https://github.com/irbull). 

Requires CMake and OpenSSL. Tested on Windows with MSVC 2013 and Linux with GCC 5.5.

## Keys

A private and public key is required for usage, example:

```
$ openssl genrsa -out private.pem 4096
$ openssl rsa -in private.pem -pubout > public.pem
```

## Applications

A couple of examples are included, they can be used to sign and verify a text file.

```
$ echo "Hello World!" > world.txt
$ rsa-sign private.pem world.txt
$ rsa-verify public.pem world.txt
Valid signature!
```

## Library

The library is very simple and includes two functions:

```
static char* sign(std::string privateKey,
                  std::string plainText);
static bool verify(std::string publicKey,
                   std::string plainText,
                   char* signatureBase64);
```

