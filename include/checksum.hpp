#ifndef FUNNEL_CHECKSUM_HPP
#define FUNNEL_CHECKSUM_HPP

#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <openssl/md5.h>

class checksum_md5
{
public:
    checksum_md5 ()
    {
        MD5_Init ( &handler );
    }
    
    void process_bytes ( const char* bytes, std::size_t length )
    {
        MD5_Update ( &handler, bytes, length );
    }

    void checksum ( unsigned char* result )
    {
        MD5_Final ( result, &handler );
    }
    
    bool compare_file ( const boost::filesystem::path& filename, unsigned char* prev_checksum )
    {
        bool result = false;

        boost::filesystem::ifstream file ( filename, std::ios::binary );

        if ( file.is_open () )
        {
            char buffer [ 1024 ];
            unsigned char new_checksum [ MD5_DIGEST_LENGTH ];

            do
            {
                file.read ( buffer, 1024 );
                process_bytes ( buffer, file.gcount () );
            }
            while ( !file.eof () );
        
            checksum ( new_checksum );

            if ( std::memcmp ( prev_checksum, new_checksum, MD5_DIGEST_LENGTH ) != 0 )
                memcpy ( prev_checksum, new_checksum, MD5_DIGEST_LENGTH );
            else
                result = true;
        }
    
        file.close ();
    
        return result;
    }
    
private:
    MD5_CTX handler;
};

#endif /* FUNNEL_CHECKSUM_HPP */