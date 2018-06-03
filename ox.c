#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 16

static struct {
    size_t len;
    uint8_t nibble[MAX_INPUT / 2];
} nibbles;

void usage();
void interactive_loop();
int parse_hex_string(char *str);
void print_result();

int main(int argc, char *argv[]) {

    if (argc != 2) {
        usage();
    }

    if (strncmp(argv[1], "-i", 2) == 0 || strncmp(argv[1], "--interactive", 13) == 0) {
        interactive_loop();
        exit(EXIT_SUCCESS);
    }

    if (parse_hex_string(argv[1])) {
        print_result();
    } else {
        usage();
    }
}

void usage() {
    printf("USAGE:\n\n");
    printf("    ox <hex-digits>\n");
    printf("    ox <-i | --interactive>\n\n");
    exit(EXIT_SUCCESS);
}

int parse_hex(char hex) {
    if (48 <= hex && hex <= 57) return (int)(hex - 48);
    if (65 <= hex && hex <= 70) return (int)(hex - 55);
    if (97 <= hex && hex <= 102) return (int)(hex - 87);
    return -1;
}

int parse_hex_string(char *str) {
    size_t len = strnlen(str, MAX_INPUT + 1);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    if (len <= 0) return 0;
    nibbles.len = len;
    for (size_t i = 0; i < len; i++) {
        int nibble = parse_hex(str[len - i - 1]);
        if (nibble == -1) return 0;
        nibbles.nibble[i] = (u_int8_t)nibble;
    }
    return 1;
}

void interactive_loop() {
    char buf[MAX_INPUT + 2];
    while (fgets(buf, MAX_INPUT + 2, stdin)) {
        if (parse_hex_string(buf)) {
            print_result();
        } else {
            printf("invalid hex string\n\n");
        }
    }
}

void print_nibble(uint8_t byte) {
    if (byte & 0b1000) putchar('1'); else putchar('0');
    if (byte & 0b0100) putchar('1'); else putchar('0');
    if (byte & 0b0010) putchar('1'); else putchar('0');
    if (byte & 0b0001) putchar('1'); else putchar('0');
}

void print_nibbles() {
    for (size_t i = nibbles.len; i > 0; i--) {
        print_nibble(nibbles.nibble[i - 1]);
        putchar(' ');
    }
}

uint64_t nibbles_to_uint64_t() {
    uint64_t u = 0;
    for (size_t i = nibbles.len; i > 0; i--)
    {
        u <<= 4;
        u |= (uint64_t)nibbles.nibble[i - 1];
    }
    return u;
}

void print_unsigned() {
    printf("%llu", nibbles_to_uint64_t());
}

void print_signed() {
    uint64_t u = nibbles_to_uint64_t();
    if (nibbles.nibble[nibbles.len - 1] & 0b1000)
    {
        uint64_t m = 0xFFFFFFFFFFFFFFF0;
        m <<= (nibbles.len - 1) * 4;
        u = m | u;
    }
    printf("%lld", (int64_t)u);
}

void print_result() {
    print_nibbles();
    printf("= ");
    print_unsigned();
    printf(" | ");
    print_signed();
    putchar('\n');
    putchar('\n');
}
