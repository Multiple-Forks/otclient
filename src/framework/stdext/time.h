/*
 * Copyright (c) 2010-2020 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef STDEXT_TIME_H
#define STDEXT_TIME_H

#include "types.h"

namespace stdext
{
    ticks_t time();
    ticks_t millis();
    ticks_t micros();
    void millisleep(size_t ms);
    void microsleep(size_t us);

    struct timer
    {
    public:
        timer() { restart(); }
        float elapsed_seconds() { return (micros() - m_start) / 1000000.f; }
        ticks_t elapsed_millis() { return (micros() - m_start) / 1000; }
        ticks_t elapsed_micros() { return micros() - m_start; }
        void restart(int shift = 0) { m_start = micros() - shift; }

    private:
        ticks_t m_start;
    };
}

#endif
