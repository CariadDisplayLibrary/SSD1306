#ifndef _SSD1306_H
#define _SSD1306_H
/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Cariad.h>

class SSD1306 : public Cariad {

    protected:
        static const int CMD_SET_CONTRAST      = 0x81;
        static const int CMD_SEG_REMAP         = 0xA1;
        static const int CMD_COM_DIR           = 0xC8;
        static const int CMD_COM_CONFIG        = 0xDA;
        static const int CMD_DISP_ON           = 0xAF;
        static const int CMD_DISP_OFF          = 0xAE;

        int _buffered;

        int _offset_x;
        int _offset_y;

        uint8_t _buffer[8*128]; // 8 pages of 128 bytes.
        uint8_t _dirty[8*16] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };
        void updateDisplay();

        virtual void command(uint8_t c) = 0;
        virtual void command(uint8_t c1, uint8_t c2) = 0;
        virtual void command(uint8_t c1, uint8_t c2, uint8_t c3) = 0;
        virtual void data(uint8_t d) = 0;

        void setPage(int p);
        void setY(int y);

        int _hw_width;
        int _hw_height;

    public:
        SSD1306(int w, int h) {
            _hw_width = _width = w;
            _hw_height = _height = h;
            _offset_x = 0;
            _offset_y = 0;
        }

        void setBacklight(uint8_t b);

        virtual void initializeDevice() = 0;

        void initDevice();

        void setPixel(int x, int y, color_t color);
        void displayOn();
        void displayOff();

        void setRotation(int r);
        void invertDisplay(bool i);

        void startBuffer() { _buffered++; }
        void endBuffer() { _buffered--; if (_buffered == 0) {updateDisplay();} }

        int getWidth() { return _width; }
        int getHeight() { return _height; }

        void fillScreen(color_t color);

        void setDisplayOffset(int x, int y);

        void drawLine(int x0, int y0, int x1, int y1, color_t color);
};

#endif
