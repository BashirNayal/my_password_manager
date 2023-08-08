#pragma once

#include "pwd_err.h"

#include <netdb.h>
#include <stdint.h>


pwd_err_et separators_find(uint8_t *buffer, uint16_t buffer_len,
                             uint16_t separator_count,
                             uint16_t separators[separator_count],
                             char separator, uint8_t fail_2many);