/*
 bit_array.c
 project: bit array library
 author: Isaac Turner <turner.isaac@gmail.com>
 Copyright (C) 23-Dec-2011
 
 Project adapted from:
 http://stackoverflow.com/questions/2633400/c-c-efficient-bit-array
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>

#include "bit_array.h"

// For internal use
// sizeof gives size in bytes (8 bits per byte)
int WORD_SIZE = sizeof(word_t) * 8;

inline word_addr_t bindex(bit_index_t b) { return b / WORD_SIZE; }
inline unsigned int boffset(bit_index_t b) { return b % WORD_SIZE; }

// Number of words required to store so many bits
inline word_addr_t nwords(bit_index_t b) { return b / WORD_SIZE + 1; }

// Constructor
BIT_ARRAY* bit_array_create(bit_index_t nbits)
{
  BIT_ARRAY* bitarr = (BIT_ARRAY*) malloc(sizeof(BIT_ARRAY));
  word_addr_t num_of_words = nwords(nbits);

  #ifdef DEBUG
  printf("Creating BIT_ARRAY (bits: %lu; words: %lu; WORD_SIZE: %i)\n",
         (unsigned long)nbits, (unsigned long)num_of_words, WORD_SIZE);
  #endif

  //bitarr->num_of_words = num_of_words;
  bitarr->num_of_bits = nbits;
  bitarr->words = (word_t*) malloc(sizeof(word_t) * num_of_words);

  // Initialise to zero
  bit_array_fill_zeros(bitarr);

  return bitarr;
}

// Destructor
void bit_array_free(BIT_ARRAY* bitarr)
{
  free(bitarr->words);
  free(bitarr);
}

// Methods
void bit_array_set_bit(BIT_ARRAY* bitarr, bit_index_t b)
{
  bitarr->words[bindex(b)] |= ((word_t)1 << (boffset(b)));
}

void bit_array_clear_bit(BIT_ARRAY* bitarr, bit_index_t b)
{
  bitarr->words[bindex(b)] &= ~((word_t)1 << (boffset(b)));
}

char bit_array_get_bit(BIT_ARRAY* bitarr, bit_index_t b)
{
  return (bitarr->words[bindex(b)] >> (boffset(b))) & 1;
}

/* set all elements of data to zero */
void bit_array_fill_zeros(BIT_ARRAY* bitarr)
{
  unsigned long num_of_words = nwords(bitarr->num_of_bits);

  int i;
  for(i = 0; i < num_of_words; i++)
  {
    bitarr->words[i] = 0;
  }
}

/* set all elements of data to one */
void bit_array_fill_ones(BIT_ARRAY* bitarr)
{
  unsigned long num_of_words = nwords(bitarr->num_of_bits);

  int i;
  for(i = 0; i < num_of_words; i++)
  {
    bitarr->words[i] = ~0;
  }
}

char* bit_array_to_string(BIT_ARRAY* bitarr)
{
  char* str = (char*) malloc(sizeof(char) * (bitarr->num_of_bits + 1));

  unsigned long i;
  
  for(i = 0; i < bitarr->num_of_bits; i++)
  {
    str[i] = bit_array_get_bit(bitarr, i) ? '1' : '0';
  }

  str[bitarr->num_of_bits] = '\0';

  return str;
}