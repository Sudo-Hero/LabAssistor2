#include "Crypto.h"


std::string encrypt(SecByteBlock& key, SecByteBlock& iv, std::string& plain) {
    std::string cipher;
    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            )
        );
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    //HexEncoder encoder(new FileSink(std::cout));

   // encoder.Put((const byte*)&cipher[0], cipher.size());
   // encoder.MessageEnd();
    //std::cout << std::endl;

    //std::cout  << "CText: " << cipher << std::endl;
    //std::cout << std::endl;
    return cipher;
}
