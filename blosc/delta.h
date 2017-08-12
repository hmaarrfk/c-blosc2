/*********************************************************************
  Blosc - Blocked Shuffling and Compression Library

  Author: Francesc Alted <francesc@blosc.org>

  See LICENSES/BLOSC.txt for details about copyright and rights to use.
**********************************************************************/

#ifndef BLOSC_DELTA_H
#define BLOSC_DELTA_H

void delta_encoder8(uint8_t* dref, int32_t offset, int32_t nbytes,
                    int32_t typesize, uint8_t* src, uint8_t* dest);

void delta_decoder8(uint8_t* dref, int32_t offset, int32_t nbytes,
                    int32_t typesize, uint8_t* dest);

#endif //BLOSC_DELTA_H
