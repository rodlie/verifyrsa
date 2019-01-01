set OPENSSL_VERSION=1.0.2q
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set VS2013="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
set "PATH=%PATH%;C:\Program Files (x86)\NASM\"

IF NOT EXIST "openssl-src" GOTO NO_BUILD_DIR
DEL "openssl-src" /Q /F /S
RMDIR /S /Q "openssl-src"
:NO_BUILD_DIR

IF EXIST "openssl-%OPENSSL_VERSION%.tar" GOTO NO_TAR
%SEVENZIP% e openssl-%OPENSSL_VERSION%.tar.gz
:NO_TAR

%SEVENZIP% x openssl-%OPENSSL_VERSION%.tar
REN openssl-%OPENSSL_VERSION% openssl-src

CALL %VS2013%

CD openssl-src
perl Configure VC-WIN32 --prefix=..\openssl
call ms\do_ms.bat
call ms\do_nasm.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
