/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PERCENT_H_
#define PERCENT_H_

#include "../encodings_ns.h"
#include "../../pbytearray/pbytearray.h"


ENCODINGS_NAMESPACE_BEGIN

struct Percent
{
	static PByteArray fromUtf8Percent(const PByteArray &percent)
	{
		if (!percent.isEmpty())
		{
			PByteArray tmp(percent.size());
			char *out = tmp.data();
			const char *in = percent.constData();

			for (const char *end = in + percent.size() - 3; in < end;)
				if (*in == '%')
					if (in[1] == '%')
						++in;
					else
						if (
								(
									(in[1] >= '0' && in[1] <= '9') ||
									(in[1] >= 'a' && in[1] <= 'f') ||
									(in[1] >= 'A' && in[1] <= 'F')
								)
								&&
								(
									(in[2] >= '0' && in[2] <= '9') ||
									(in[2] >= 'a' && in[2] <= 'f') ||
									(in[2] >= 'A' && in[2] <= 'F')
								)
							)
						{
							*out++ = (unsigned char)((charToHex(in[1]) << 4) | charToHex(in[2]));
							in += 3;
						}
						else
							*out++ = *in++;
				else
					*out++ = *in++;

			if (	in[0] == '%' && in + 2 < percent.constData() + percent.size()
					&&
					(
						(in[1] >= '0' && in[1] <= '9') ||
						(in[1] >= 'a' && in[1] <= 'f') ||
						(in[1] >= 'A' && in[1] <= 'F')
					)
					&&
					(
						(in[2] >= '0' && in[2] <= '9') ||
						(in[2] >= 'a' && in[2] <= 'f') ||
						(in[2] >= 'A' && in[2] <= 'F')
					)
				)
			{
				*out++ = (unsigned char)((charToHex(in[1]) << 4) | charToHex(in[2]));
			}
			else
				for (const char *end = percent.constData() + percent.size(); in < end;)
					*out++ = *in++;

			tmp.truncate(out - tmp.data());
			return tmp;
		}

		return PByteArray();
	}

	static PByteArray fromPercent(const PByteArray &percent)
	{
		if (!percent.isEmpty())
		{
			PByteArray tmp(percent.size());
			char *out = tmp.data();
			const char *in = percent.constData();

			for (const char *end = in + percent.size() - 3; in < end;)
				if (*in == '%')
					if (in[1] == '%')
						++in;
					else
						if (
								(
									(in[1] >= '0' && in[1] <= '9') ||
									(in[1] >= 'a' && in[1] <= 'f') ||
									(in[1] >= 'A' && in[1] <= 'F')
								)
								&&
								(
									(in[2] >= '0' && in[2] <= '9') ||
									(in[2] >= 'a' && in[2] <= 'f') ||
									(in[2] >= 'A' && in[2] <= 'F')
								)
							)
						{
							const unsigned char c = (charToHex(in[1]) << 4) | charToHex(in[2]);

							if (c < 127)
								*out++ = c;
							else
							{
								*out++ = in[0];
								*out++ = in[1];
								*out++ = in[2];
							}

							in += 3;
						}
						else
							*out++ = *in++;
				else
					*out++ = *in++;

			if (	in[0] == '%' && in + 2 < percent.constData() + percent.size()
					&&
					(
						(in[1] >= '0' && in[1] <= '9') ||
						(in[1] >= 'a' && in[1] <= 'f') ||
						(in[1] >= 'A' && in[1] <= 'F')
					)
					&&
					(
						(in[2] >= '0' && in[2] <= '9') ||
						(in[2] >= 'a' && in[2] <= 'f') ||
						(in[2] >= 'A' && in[2] <= 'F')
					)
				)
			{
				const unsigned char c = (charToHex(in[1]) << 4) | charToHex(in[2]);

				if (c < 127)
					*out++ = c;
				else
				{
					*out++ = in[0];
					*out++ = in[1];
					*out++ = in[2];
				}
			}
			else
				for (const char *end = percent.constData() + percent.size(); in < end;)
					*out++ = *in++;

			tmp.truncate(out - tmp.data());
			return tmp;
		}

		return PByteArray();
	}

private:
	static char charToHex(const char token)
	{
		if (token >= '0' && token <= '9')
			return (token - '0');
		else
			if (token >= 'a' && token <= 'f')
				return (token - 'a' + 10);
			else
				return (token - 'A' + 10);
	}
	static char hexToChar(const char token)
	{
		if (token <= 9)
			return (token + '0');
		else
			if (token >= 10 && token <= 15)
				return (token + 'A' - 10);
	}
};

ENCODINGS_NAMESPACE_END

#endif /* PERCENT_H_ */
