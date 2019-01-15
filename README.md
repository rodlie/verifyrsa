# RSA sign & verify

Very simple library used to sign and verify text files. Based on example by [Ian Bull](https://github.com/irbull). 

Requires CMake and OpenSSL. Tested on Windows with MSVC 2013 and Linux with GCC 5.5.

```
$ echo "Hello World!" > world.txt
$ rsa-generate my 4096
$ rsa-sign my_private.pem world.txt
$ rsa-verify my_public.pem world.txt
Valid signature!
```
