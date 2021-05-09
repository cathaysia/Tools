#include <curl/curl.h>
#include <curl/mprintf.h>

static const char base64[]    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char base64url[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static CURLcode base64_encode(const char *table64, const char *inputbuff, size_t insize,
                              char **outptr, size_t *outlen) {
    unsigned char ibuf[3];
    unsigned char obuf[4];
    int i;
    int inputparts;
    char *output;
    char *base64data;
    char *convbuf      = NULL;

    const char *indata = inputbuff;

    *outptr            = NULL;
    *outlen            = 0;

    if(!insize) insize = strlen(indata);

#if SIZEOF_SIZE_T == 4
    if(insize > UINT_MAX / 4) return CURLE_OUT_OF_MEMORY;
#endif

    base64data = output = (char *)malloc(insize * 4 / 3 + 4);
    if(!output) return CURLE_OUT_OF_MEMORY;
    while(insize > 0) {
        for(i = inputparts = 0; i < 3; i++) {
            if(insize > 0) {
                inputparts++;
                ibuf[i] = (unsigned char)*indata;
                indata++;
                insize--;
            } else
                ibuf[i] = 0;
        }

        obuf[0] = (unsigned char)((ibuf[0] & 0xFC) >> 2);
        obuf[1] = (unsigned char)(((ibuf[0] & 0x03) << 4) | ((ibuf[1] & 0xF0) >> 4));
        obuf[2] = (unsigned char)(((ibuf[1] & 0x0F) << 2) | ((ibuf[2] & 0xC0) >> 6));
        obuf[3] = (unsigned char)(ibuf[2] & 0x3F);

        switch(inputparts) {
            case 1: curl_msnprintf(output, 5, "%c%c==", table64[obuf[0]], table64[obuf[1]]); break;

            case 2: curl_msnprintf(output, 5, "%c%c%c=", table64[obuf[0]], table64[obuf[1]], table64[obuf[2]]); break;
            default:
                curl_msnprintf(output, 5, "%c%c%c%c", table64[obuf[0]], table64[obuf[1]], table64[obuf[2]],
                               table64[obuf[3]]);
                break;
        }
        output += 4;
    }
    *output = '\0';
    *outptr = base64data;

    free(convbuf);
    *outlen = strlen(base64data);

    return CURLE_OK;
}

CURLcode Curl_base64_encode(const char *inputbuff, size_t insize, char **outptr,
                            size_t *outlen) {
    return base64_encode(base64, inputbuff, insize, outptr, outlen);
}

/*
 * Curl_base64url_encode()
 *
 * Given a pointer to an input buffer and an input size, encode it and
 * return a pointer in *outptr to a newly allocated memory area holding
 * encoded data. Size of encoded data is returned in variable pointed by
 * outlen.
 *
 * Input length of 0 indicates input buffer holds a NUL-terminated string.
 *
 * Returns CURLE_OK on success, otherwise specific error code. Function
 * output shall not be considered valid unless CURLE_OK is returned.
 *
 * When encoded data length is 0, returns NULL in *outptr.
 *
 * @unittest: 1302
 */
CURLcode Curl_base64url_encode( const char *inputbuff, size_t insize, char **outptr, size_t *outlen) {
    return base64_encode(base64url, inputbuff, insize, outptr, outlen);
}
