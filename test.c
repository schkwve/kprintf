#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_integer(char *buffer, uint64_t num, uint8_t base, bool is_signed)
{
	bool is_negative = false;
	int i = 0;
	char *digits = "0123456789abcdef";

	if (base == 10 && is_signed && (int64_t)num < 0) {
		num = -(int64_t)num;
		is_negative = true;
	}

	do {
		buffer[i++] = digits[num % base];
		num /= base;
	} while (num > 0);

	if (base == 10 && is_negative) {
		buffer[i++] = '-';
	}

	buffer[i--] = '\0';

	for (int j = 0; j < i; j++, i--) {
		char tmp = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = tmp;
	}
}

void custom_snprintf(char *buffer, size_t len, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int state = 0;
	size_t j = 0;
	for (size_t i = 0; fmt[i] != '\0' && i < len; i++, j++) {
		if (fmt[i] == '%') {

			i++;
			switch (fmt[i]) {
			case 'c': {
				buffer[j] = va_arg(args, int);
				break;
			}
			case 's': {
				const char *str = va_arg(args, const char *);
				while(*str != '\0') {
					buffer[j++] = *str++;
				}
				buffer[j--] = '\0';
				break;
			}
			case 'd': {
				int32_t n = va_arg(args, int32_t);
				char buf[32];
				print_integer(buf, n, 10, true);
				for (int k = 0; buf[k]; k++) {
					buffer[j++] = buf[k];
				}
				break;
			}
			case 'u': {
				uint32_t n = va_arg(args, uint32_t);
				char buf[32];
				print_integer(buf, n, 10, false);
				for (int k = 0; buf[k]; k++) {
					buffer[j++] = buf[k];
				}
				break;
			}
			case 'x': {
				uint64_t x = va_arg(args, uint64_t);
				char buf[32];
				print_integer(buf, x, 16, false);
				for (int k = 0; buf[k]; k++) {
					buffer[j++] = buf[k];
				}
				break;
			}
			case 'p': {
				buffer[j++] = '0';
				buffer[j++] = 'x';
				uint64_t x = va_arg(args, uint64_t);
				char buf[32];
				print_integer(buf, x, 16, false);
				for (int k = 0; buf[k]; k++) {
					buffer[j++] = buf[k];
				}
				break;
			}
			}
		} else {
			buffer[j] = fmt[i];
		}
	}

	va_end(args);
}

int main(void)
{
	char *buffer = (char *)malloc(256);

	int sposnum = 123;
	int snegnum = -123;
	int hexnum = 0xbadb33f;
	void *ptr = (void *)main;
	char c = 'A';
	char *str = "Hello!";
	
	custom_snprintf(buffer, -1, "signed positive number: %d", sposnum);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	custom_snprintf(buffer, -1, "signed negative number: %d", snegnum);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	custom_snprintf(buffer, -1, "hexadecimal number: %x", hexnum);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	custom_snprintf(buffer, -1, "pointer: %p", ptr);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	custom_snprintf(buffer, -1, "character: %c", c);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	custom_snprintf(buffer, -1, "string: %s", str);
	printf("%s\n", buffer);
	memset(buffer, 0, 256);
	
	free(buffer);
	return 0;
}