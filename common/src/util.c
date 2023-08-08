#include "util.h"
#include "pwd_err.h"



pwd_err_et separators_find(uint8_t *buffer, uint16_t buffer_len,
                             uint16_t separator_count,
                             uint16_t separators[separator_count],
                             char separator, uint8_t fail_2many)
{
    uint16_t separator_idx = 0;
    uint16_t data_len = buffer_len;
    for (uint16_t data_idx = 0; data_idx < data_len; ++data_idx)
    {
        if (buffer[data_idx] == separator)
        {
            if (separator_idx == separator_count)
            {
                // Too many whitespaces.
                if (fail_2many)
                {
                    return PWD_FAILURE;
                }
                else
                {
                    return PWD_SUCCESS;
                }
            }
            else
            {
                    separators[separator_idx++] = data_idx;
            }
        }
    }
    if (separator_idx == separator_count)
    {
        return PWD_SUCCESS;
    }
    else
    {
        return PWD_FAILURE;
    }
}