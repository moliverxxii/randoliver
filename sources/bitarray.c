/*
 * bitarray.c
 *
 *  Created on: 12 juil. 2026
 *      Author: moliver
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "bitarray.h"

#define BITARRAY_BITS_PER_BYTE 8
#define BITARRAY_BITS_PER_WORD (BITARRAY_BITS_PER_BYTE * sizeof(uint32_t))
#define DIV_CEIL(X, Y) (((X) + (Y) - 1)/(Y))

static uint32_t
bit_reverse_32(uint32_t bits)
{
    bits = (bits & 0x0000FFFF) << (BITARRAY_BITS_PER_BYTE<<1)
         | (bits & 0xFFFF0000) >> (BITARRAY_BITS_PER_BYTE<<1);
    bits = (bits & 0x00FF00FF) << (BITARRAY_BITS_PER_BYTE)
         | (bits & 0xFF00FF00) >> (BITARRAY_BITS_PER_BYTE);
    bits = (bits & 0x0F0F0F0F) << (BITARRAY_BITS_PER_BYTE>>1)
         | (bits & 0xF0F0F0F0) >> (BITARRAY_BITS_PER_BYTE>>1);
    bits = (bits & 0x33333333) << (BITARRAY_BITS_PER_BYTE>>2)
         | (bits & 0xCCCCCCCC) >> (BITARRAY_BITS_PER_BYTE>>2);
    bits = (bits & 0x55555555) << (BITARRAY_BITS_PER_BYTE>>3)
         | (bits & 0xAAAAAAAA) >> (BITARRAY_BITS_PER_BYTE>>3);
    return bits;
}

static uint32_t
bit_reverse_24(uint32_t bits)
{
    bits = (bits & 0x000FFF) << 12
         | (bits & 0xFFF000) >> 12;
    bits = (bits & 0x03F03F) << 6
         | (bits & 0xFC0FC0) >> 6;
    bits = (bits & 0x1C71C7) << 3
         | (bits & 0xE38E38) >> 3;
    bits = (bits & 0x249249) << 2
         | (bits & 0x492492)
         | (bits & 0x924924) >> 2;
    return bits;
}

typedef struct bitarray_t
{
    uint32_t width;
    uint32_t height;
    uint32_t* bytes_p;
} bitarray_t;

uint32_t
bitarray_row_words(const bitarray_t* bitarray_p)
{
    return DIV_CEIL(bitarray_p->width, BITARRAY_BITS_PER_WORD);
}

uint32_t
bitarray_words(const bitarray_t* bitarray_p)
{
    return bitarray_p->height * bitarray_row_words(bitarray_p);
}

uint32_t*
bitarray_row(bitarray_t* bitarray_p, uint32_t y)
{
    uint32_t row_words = bitarray_row_words(bitarray_p);
    return &bitarray_p->bytes_p[y * row_words];
}

bitarray_t*
bitarray_init(uint32_t width, uint32_t height)
{
    bitarray_t* bitarray_p = malloc(sizeof(bitarray_t));
    if(bitarray_p != NULL)
    {
        bitarray_t array =
        {
            width, height, NULL
        };
        *bitarray_p = array;

        uint32_t word_count = bitarray_words(bitarray_p);
        bitarray_p->bytes_p = malloc(sizeof(uint32_t)*word_count);
        for(uint32_t word = 0; word < word_count; ++word)
        {
            uint32_t half_word_1 = ((uint64_t) UINT16_MAX * rand()) / RAND_MAX;
            uint32_t half_word_2 = ((uint64_t) UINT16_MAX * rand()) / RAND_MAX;

            uint32_t word_value  = half_word_2 << (BITARRAY_BITS_PER_WORD/2);
            word_value          |= half_word_1;

            uint32_t remaining_bits_row = bitarray_p->width - (BITARRAY_BITS_PER_WORD * (word%bitarray_row_words(bitarray_p)));
            if(remaining_bits_row < BITARRAY_BITS_PER_WORD)
            {
                uint32_t MASK = 0xFFFFFFFF >> (BITARRAY_BITS_PER_WORD - remaining_bits_row);
                word_value &= MASK;
            }

            bitarray_p->bytes_p[word] = word_value;
        }
    }

    return bitarray_p;
}

void
bitarray_free(bitarray_t* bitarray_p)
{
    free(bitarray_p->bytes_p);
    free(bitarray_p);
}

#define BOUCKLE

void
bitarray_extract(bitarray_t* destination_p, const bitarray_t* source_p, int32_t position_x, int32_t position_y)
{
    uint32_t offset = position_x % BITARRAY_BITS_PER_WORD;
    int32_t word_s0 = position_x / BITARRAY_BITS_PER_WORD;
    int32_t row_words_s = bitarray_row_words(source_p);
    memset(bitarray_row(destination_p, 0), 0, bitarray_words(destination_p) * sizeof(uint32_t));
    for(uint32_t y_d = 0; y_d < destination_p->height; ++y_d)
    {
        int32_t y_s = (int32_t) y_d + position_y;
#ifdef BOUCKLE
        y_s = modulo(y_s, source_p->height);
#else
        if(y_s < 0)
        {
            continue;
        }
        if(y_s >= (int32_t) source_p->height)
        {
            break;
        }
#endif

#define BIT_PER_BIT

#ifdef BIT_PER_BIT
        for(uint32_t x_d = 0; x_d < destination_p->width; ++x_d)
        {
            int32_t x_s = (int32_t) x_d + position_x;
#ifdef BOUCKLE
            x_s = modulo(x_s, source_p->width);
#else
            if(x_s < 0)
            {
                continue;
            }
            if(x_s >= (int32_t) source_p->width)
            {
                break;
            }
#endif

            if(bitarray_bit_get(source_p, x_s, y_s))
            {
                bitarray_bit_set(destination_p, x_d, y_d);
            }
            else
            {
                bitarray_bit_reset(destination_p, x_d, y_d);
            }
        }

#else //BIT_PER_BIT
        uint32_t* row_d       = bitarray_row(destination_p, y_d);
        const uint32_t* row_s = bitarray_row((bitarray_t*)source_p, y_d + position_y);

        int32_t word_s = word_s0;
        for(uint32_t word_d = 0; word_d < bitarray_row_words(destination_p); ++word_d, ++word_s)
        {
            if(word_s >= row_words_s)
            {
                break;
            }

            if(word_s >= 0)
            {
                row_d[word_d] |= row_s[word_s]     >> offset;
            }

            if(word_s >= -1)
            {
                if(word_s < row_words_s - 1)
                {
                    row_d[word_d] |= row_s[word_s + 1] << (BITARRAY_BITS_PER_WORD - offset);
                }
            }
        }
#endif //BIT_PER_BIT
    }
}

int
bitarray_is_in(const bitarray_t* array_p, uint32_t x, uint32_t y)
{
    return x < array_p->width && y < array_p->height;
}

void
bitarray_bit_set(bitarray_t* array_p, uint32_t x, uint32_t y)
{
    if(bitarray_is_in(array_p, x, y))
    {
        uint32_t* word_p = &bitarray_row(array_p, y)[x / BITARRAY_BITS_PER_WORD];
        *word_p |= 1 << (x % BITARRAY_BITS_PER_WORD);
    }
}

void
bitarray_bit_reset(bitarray_t* array_p, uint32_t x, uint32_t y)
{
    if(bitarray_is_in(array_p, x, y))
    {
        uint32_t* word_p = &bitarray_row(array_p, y)[x / BITARRAY_BITS_PER_WORD];
        *word_p &= ~(1 << (x % BITARRAY_BITS_PER_WORD));
    }
}

int
bitarray_bit_get(const bitarray_t* array_p, uint32_t x, uint32_t y)
{
    int bit = 0;
    if(bitarray_is_in(array_p, x, y))
    {
        const uint32_t* word_p = &bitarray_row((bitarray_t*) array_p, y)[x / BITARRAY_BITS_PER_WORD];
        bit = 0 != (*word_p & (1 << (x % BITARRAY_BITS_PER_WORD)));
    }
    return bit;
}

void
bitarray_and(bitarray_t* a_p, const bitarray_t* b_p)
{
    uint32_t* a_words_p = bitarray_row(a_p, 0);
    const uint32_t* b_words_p = bitarray_row((bitarray_t*) b_p, 0);
    for(uint32_t word = 0; word < bitarray_words(b_p); ++word)
    {
        a_words_p[word] &= b_words_p[word];
    };

}

uint32_t
bitarray_on_get(const bitarray_t* array_p)
{
    uint32_t count = 0;
    const uint32_t* words_p = bitarray_row((bitarray_t*) array_p, 0);
    for(uint32_t word = 0; word < bitarray_words(array_p); ++word)
    {
        uint32_t value = words_p[word];
        while(value > 0)
        {
            count += value & 1;
            value >>= 1;
        }
    };
    return count;
}

void
bitarray_print(const bitarray_t* bitarray_p)
{
    uint32_t row_words = bitarray_row_words(bitarray_p);
    for(uint32_t y = 0; y < bitarray_p->height; ++y)
    {
        for(uint32_t x_word = 0; x_word < row_words; ++x_word)
        {
            uint32_t x = x_word * BITARRAY_BITS_PER_WORD;
            uint32_t word_value = bitarray_p->bytes_p[row_words * y + x_word];
            uint32_t remaining_bits_row = bitarray_p->width - x;
            if(remaining_bits_row < BITARRAY_BITS_PER_WORD)
            {
                uint32_t MASK = 0xFFFFFFFF >> (BITARRAY_BITS_PER_WORD - remaining_bits_row);
                word_value &= MASK;
            }
            printf("%08x", bit_reverse_32(word_value));
        }
        printf("\n");
    }
}
