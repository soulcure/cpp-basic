#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xstring.h"

struct xstring {
    char *buf;
    int NUL;
    int buflen;
};

static unsigned
next_power(unsigned x)
{
	--x;
	x |= x >> 0x01;
	x |= x >> 0x02;
	x |= x >> 0x04;
	x |= x >> 0x08;
	x |= x >> 0x10;
	return ++x;
}

/* Extend the buffer in sb by at least len bytes.
 * Note len should include the space required for the NUL terminator */
static void
string_extendby(struct xstring *sb, int len)
{
	char *buf;

	len += sb->NUL;
	if (len <= sb->buflen)
		return;

	if (!sb->buflen)
		sb->buflen = 32;
	else
		sb->buflen = next_power(len);

	buf = (char *)realloc(sb->buf, sb->buflen);
	sb->buf = buf;
}

static void
string_vprintf(struct xstring *sb, const char *fmt,  va_list ap)
{
	int num_required;
	while ((num_required = vsnprintf(sb->buf + sb->NUL, sb->buflen - sb->NUL, fmt, ap)) >= sb->buflen - sb->NUL)
		string_extendby(sb, num_required + 1);
	sb->NUL += num_required;
}

////////////////////////////////////////////////////////////////////////////////

struct xstring *
string_new(size_t reserve)
{
	struct xstring *sb = (struct xstring *)malloc(sizeof(struct xstring));
	sb->buf = NULL;
	sb->NUL = 0;
	sb->buflen = 0;

	if (reserve)
		string_extendby(sb, reserve + 1);

	return sb;
}

void
string_free(struct xstring *sb)
{
	if (sb->buf)
		free(sb->buf);
	free(sb);
}

char *
string_detach(struct xstring *sb)
{
	char *buf = sb->buf;
	sb->buf = NULL;
	sb->NUL = 0;
	sb->buflen = 0;
	return buf;
}

void
string_clear(struct xstring *sb)
{
	sb->NUL = 0;
	if (sb->buf)
		sb->buf[sb->NUL] = '\0';
}

void
string_cat(struct xstring *sb, const char *str)
{
	size_t len = strlen(str);

	string_extendby(sb, len + 1);
	memcpy(&sb->buf[sb->NUL], str, len);
	sb->NUL += len;
	sb->buf[sb->NUL] = '\0';
}

void
string_putc(struct xstring *sb, const char c)
{
	string_extendby(sb, 2);
	sb->buf[sb->NUL++] = c;
	sb->buf[sb->NUL] = '\0';
}

void
string_ncat(struct xstring *sb, const char *str, size_t len)
{
	string_extendby(sb, len + 1);
	memcpy(&sb->buf[sb->NUL], str, len);
	sb->NUL += len;
	sb->buf[sb->NUL] = '\0';
}

void
string_scatf(struct xstring *sb, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	string_vprintf(sb, fmt, ap);
	va_end(ap);
}

const char *
string_raw(struct xstring *sb)
{
	return sb->NUL ? sb->buf : NULL;
}

size_t
string_len(struct xstring *sb)
{
	return sb->NUL;
}

