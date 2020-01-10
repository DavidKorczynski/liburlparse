#ifndef _HOSTNORMALIZER_H
#define _HOSTNORMALIZER_H

#include <list>
#include <cstring>
#include <vector>

#include "./urlutil.h"
#include "./stringbuilder.h"
#include "./stringutils.h"
#include "./inetaddress.h"

class UrlUtil;
#define ubyte unsigned char

class HostNormalizer {
public:
    HostNormalizer(const string &host);

    vector<ubyte> getBytes();
    string getNormalizedHost();

private:
    static const long MAX_NUMERIC_DOMAIN_VALUE = 4294967295L;
    static const int MAX_IPV4_PART = 255;
    static const int MIN_IP_PART = 0;
    static const int MAX_IPV6_PART = 0xFFFF;
    static const int IPV4_MAPPED_IPV6_START_OFFSET = 12;
    static const int NUMBER_BYTES_IN_IPV4 = 4;
    string _host;

    vector<ubyte> _bytes;
    string _normalizedHost;
    bool _useIpv6Decoded;
    bool _useIpv4Decoded;
public:    
    /**
     * split ipv6 to vector array
     *
     * example a,b,c,d,e,,
     * splitchar -> ,
     * limit:0  
     *  try to split forever but remove the last ""
     * => ["a","b","c","d","e"] 
     * limit:n (n>0)        
     *  try to split n-1 times , parts = n
     *  n = 1 => ["a,b,c,d,e,,"] 
     *  n = 2 => ["a","b,c,d,e,,"]
     * limit:n (n<0)
     *  try to split forever ,parts = count(,) + 1
     * n = -1 => ["a","b","c","d","e","",""]
     *
     */
    static std::vector<std::string> splitbyDot(const std::string &src, const char c = ':', int nlimit = 0);

private:
    void normalizeHost();
    /**
     * Checks if the host is an ip address. Returns the byte representation of
     * it
     */
    std::vector<ubyte> &tryDecodeHostToIp(std::vector<ubyte> &bytes,const string &host);

    /**
     * This covers cases like:
     * Hexadecimal:        0x1283983
     * Decimal:            12839273
     * Octal:              037362273110
     * Dotted Decimal:     192.168.1.1
     * Dotted Hexadecimal: 0xfe.0x83.0x18.0x1
     * Dotted Octal:       0301.00.046.00
     * Dotted Mixed:       0x38.168.077.1
     *
     * if ipv4 was found, _bytes is set to the byte representation of the ipv4
     * address
     */
    vector<ubyte> &tryDecodeHostToIPv4(std::vector<ubyte> &sbytes,const string &host, bool &validEmpty);
    /**
     * Recommendation for IPv6 Address Text Representation
     * http://tools.ietf.org/html/rfc5952
     *
     * if ipv6 was found, _bytes is set to the byte representation of the ipv6 address
     */
    vector<ubyte> &tryDecodeHostToIPv6(std::vector<ubyte> &sbytes,const string &host);

    std::vector<ubyte> sectionToTwoBytes(int section);
    bool isHexSection(string &section);
};

#endif
