#pragma once

#include "definitions.h"
#include <string.h>
#include <stdint.h>

class StringView {
public:
    StringView()
     : StringView(nullptr)
    {}

    /**
     * @param[in] s
     * the string buffer to reference
     */
    StringView(const char *s)
     : StringView(s, (s ? strlen(s) : 0))
    {}

    /**
     * @param[in] s
     * the string buffer to reference
     * 
     * @param[in] len
     * the string's length as returned by an strlen(s) call
     */
    StringView(const char *s, size_t len)
     : buff_(s)
     , len_(len)
    {}

    const char *data() {return buff_;}
    char operator [](size_t i) {return buff_[i];}
    bool operator ==(const char *s) {return equals(s);}
    char charAt(size_t i) {return buff_[i];}
    bool startsWith(const char *str, size_t offset) {
        if ( ! buff_) {
            return false;
        }
        else if (offset >= len_) {
            return false;
        }

        const size_t str_len = strlen(str);
        if (str_len <= (len_ - offset)) {
            return strncmp(buff_+offset, str, str_len) == 0;
        }
        return false;
    }
    bool startsWith(const char *str) {return startsWith(str, 0);}
    bool equals(const char *str) {
        return strncmp(buff_, str, len_) == 0;
    }

    // return length of string in character (does not include null terminator)
    size_t length() {return len_;}

    StringView substring(size_t from) {
        return len_ == 0 ? StringView() : substring(from, len_ - 1);
    }

    StringView substring(size_t from, size_t to) {
        if (to < from || from >= len_ || to >= len_) {
            return StringView();
        }
        return StringView(buff_ + from, to - from + 1);
    }

    void dbgPrint() {
#if DO_DEBUG
        for (size_t i=0; i<len_; i++) {
            Serial.print(buff_[i]);
        }
#endif
    }

    void dbgPrint(const char *prefix) {
#if DO_DEBUG
        Serial.print(prefix);
        dbgPrint();
        Serial.println();
#endif
    }

private:
    const char *buff_;
    size_t len_;

};