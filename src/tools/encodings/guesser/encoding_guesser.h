/**
 * http://www.haypocalc.com/tmp/unicode-2011-07-20/html/guess_encoding.html
 */

#ifndef ENCODING_GUESSER_H_
#define ENCODING_GUESSER_H_

#include <string.h>
#include <stdint.h>


const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";
const char *UTF_8_BOM = "\xEF\xBB\xBF";
const char *UTF_32_BE_BOM = "\x00\x00\xFE\xFF";
const char *UTF_32_LE_BOM = "\xFF\xFE\x00\x00";


const char *check_bom(const char *data, size_t size)
{
    if (size >= 3)
        if (memcmp(data, UTF_8_BOM, 3) == 0)
            return "UTF-8";

    if (size >= 4)
    {
        if (memcmp(data, UTF_32_LE_BOM, 4) == 0)
            return "UTF-32LE";

        if (memcmp(data, UTF_32_BE_BOM, 4) == 0)
            return "UTF-32BE";
    }

    if (size >= 2)
    {
        if (memcmp(data, UTF_16_LE_BOM, 2) == 0)
            return "UTF-16LE";

        if (memcmp(data, UTF_16_BE_BOM, 2) == 0)
            return "UTF-16BE";
    }

    return NULL;
}

bool isASCII(const char *data, size_t size)
{
    const unsigned char *str = (const unsigned char*)data;
    const unsigned char *end = str + size;

    for (; str != end; str++)
        if (*str & 0x80)
            return false;

    return true;
}

bool isUTF8(const char *data, size_t size)
{
    const unsigned char *str = (unsigned char*)data;
    const unsigned char *end = str + size;
    unsigned char byte;
    unsigned int code_length, i;
    uint32_t ch;

    while (str != end)
    {
        byte = *str;

        if (byte <= 0x7F)
        {
            /* 1 byte sequence: U+0000..U+007F */
            str += 1;
            continue;
        }

        if (0xC2 <= byte && byte <= 0xDF)
            /* 0b110xxxxx: 2 bytes sequence */
            code_length = 2;
        else
        	if (0xE0 <= byte && byte <= 0xEF)
        		/* 0b1110xxxx: 3 bytes sequence */
        		code_length = 3;
        	else
        		if (0xF0 <= byte && byte <= 0xF4)
        			/* 0b11110xxx: 4 bytes sequence */
        			code_length = 4;
        		else
        			/* invalid first byte of a multibyte character */
        			return false;

        if (str + (code_length - 1) >= end)
            /* truncated string or invalid byte sequence */
            return false;

        /* Check continuation bytes: bit 7 should be set, bit 6 should be
         * unset (b10xxxxxx). */
        for (i=1; i < code_length; i++)
            if ((str[i] & 0xC0) != 0x80)
                return false;

        if (code_length == 2)
        {
            /* 2 bytes sequence: U+0080..U+07FF */
            ch = ((str[0] & 0x1f) << 6) + (str[1] & 0x3f);

            /* ch cannot be greater than 0x07FF */
            if (ch < 0x0080)
                return false;
        }
        else
        	if (code_length == 3)
			{
				/* 3 bytes sequence: U+0800..U+FFFF */
				ch = ((str[0] & 0x0f) << 12) + ((str[1] & 0x3f) << 6) +
					  (str[2] & 0x3f);

				/* ch cannot be greater than 0xFFFF */
				if (ch < 0x0800)
					return 0;

				/* surrogates (U+D800-U+DFFF) are invalid in UTF-8 */
				if ((0xD800 <= ch) && (ch <= 0xDFFF))
					return 0;
			}
			else
				if (code_length == 4)
				{
					/* 4 bytes sequence: U+10000..U+10FFFF */
					ch = ((str[0] & 0x07) << 18) + ((str[1] & 0x3f) << 12) +
						 ((str[2] & 0x3f) << 6) + (str[3] & 0x3f);

					if ((ch < 0x10000) || (0x10FFFF < ch))
						return 0;
				}

        str += code_length;
    }

    return true;
}

#endif /* ENCODING_GUESSER_H_ */
