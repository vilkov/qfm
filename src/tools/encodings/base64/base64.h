#ifndef BASE64_H_
#define BASE64_H_

#include "../encodings_ns.h"
#include "../../pbytearray/pbytearray.h"


ENCODINGS_NAMESPACE_BEGIN

struct Base64
{
	typedef PByteArray::value_type value_type;
	typedef PByteArray::size_type  size_type;
	typedef unsigned char          uchar;

	static PByteArray fromBase64(const PByteArray &base64)
	{
		unsigned int buf = 0;
		int nbits = 0;
		PByteArray tmp((base64.size() * 3) / 4);

		int offset = 0;
		for (size_type i = 0, size = base64.size(); i < size; ++i)
		{
			int ch = base64.at(i);
			int d;

			if (ch >= 'A' && ch <= 'Z')
				d = ch - 'A';
			else
				if (ch >= 'a' && ch <= 'z')
					d = ch - 'a' + 26;
				else
					if (ch >= '0' && ch <= '9')
						d = ch - '0' + 52;
					else
						if (ch == '+')
							d = 62;
						else
							if (ch == '/')
								d = 63;
							else
								d = -1;

			if (d != -1)
			{
				buf = (buf << 6) | d;
				nbits += 6;

				if (nbits >= 8)
				{
					nbits -= 8;
					tmp[offset++] = buf >> nbits;
					buf &= (1 << nbits) - 1;
				}
			}
		}

		tmp.truncate(offset);
		return tmp;
	}

	static PByteArray toBase64(const PByteArray &data)
	{
		static const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef" "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
		static const char padchar = '=';
		int padlen = 0;

		PByteArray tmp((data.size() * 4) / 3 + 3);

		char *out = tmp.data();
		for (size_type i = 0, size = data.size(); i < size;)
		{
			int chunk = 0;
			chunk |= int(uchar(data[i++])) << 16;

			if (i == size)
				padlen = 2;
			else
			{
				chunk |= int(uchar(data[i++])) << 8;

				if (i == size)
					padlen = 1;
				else
					chunk |= int(uchar(data[i++]));
			}

			int j = (chunk & 0x00fc0000) >> 18;
			int k = (chunk & 0x0003f000) >> 12;
			int l = (chunk & 0x00000fc0) >> 6;
			int m = (chunk & 0x0000003f);

			*out++ = alphabet[j];
			*out++ = alphabet[k];

			if (padlen > 1)
				*out++ = padchar;
			else
				*out++ = alphabet[l];

			if (padlen > 0)
				*out++ = padchar;
			else
				*out++ = alphabet[m];
		}

		tmp.truncate(out - tmp.data());
		return tmp;
	}
};

ENCODINGS_NAMESPACE_END

#endif /* BASE64_H_ */
