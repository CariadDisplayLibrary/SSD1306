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

#include <SSD1306.h>

void SSD1306::initDevice() {

    command(0x8D, 0x14);
    command(0xD9, 0xF1);

    command(CMD_SEG_REMAP);
    command(CMD_COM_DIR);

    if (_hw_width == 128 && _hw_height == 32) {
        command(CMD_COM_CONFIG);
        command(0x20);
    }

    command(CMD_SET_CONTRAST, 0x80);

    displayOn();
}

#define C2B(X, Y) (((Y) << 7) + (X))

void SSD1306::setDisplayOffset(int x, int y) {
    _offset_x = x;
    _offset_y = y;
}

void SSD1306::setPixel(int x, int y, color_t color) {
    uint8_t row;
    uint8_t pixel;
    uint8_t mask;

    translateCoordinates(&x, &y);

    x += _offset_x;
    y += _offset_y;

    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height))
        return;

    row = y>>3;
    pixel = y & 0x07;
    mask = 1<<pixel;
    if (color) {
        _buffer[C2B(x, row)] |= mask;
    } else {
        _buffer[C2B(x, row)] &= ~mask;
    }

    if (_buffered == 0) {
        setPage(row);
        setY(x);
        data(_buffer[C2B(x, row)]);
    }

    
}

void SSD1306::displayOn() {
    command(CMD_DISP_ON);
}

void SSD1306::displayOff() {
    command(CMD_DISP_OFF);
}

void SSD1306::setPage(int page) {
    command(0x22, page & 0x07, page & 0x07);
}

void SSD1306::setY(int y) {
    command(0x00 | (y & 0x0F)); // Low nibble
    command(0x10 | ((y >> 4) & 0x0F)); // High nibble
}

void SSD1306::updateDisplay() {
    for (int p = 0; p < _hw_height/8; p++) {
        setPage(p);
        setY(0);
        for (int y = 0; y < _hw_width; y++) {
            data(_buffer[C2B(y, p)]);
        }
    }
}

void SSD1306::fillScreen(color_t color) {
    if (color) { 
        memset(_buffer, 0xFF, 128*(_height / 8));
    } else {
        memset(_buffer, 0x00, 128*(_height / 8));
    }

    if (_buffered == 0) {
        updateDisplay();
    }
}

void SSD1306::setRotation(int r) {

    rotation = r & 0x03;

    switch (r & 0x03) {
        case 0:
        case 2:
            _width = _hw_width;
            _height = _hw_height;
            break;
        case 1:
        case 3:
            _width = _hw_height;
            _height = _hw_width;
            break;
    }
    clearClipping();
}

void SSD1306::setBacklight(uint8_t b) {
    command(CMD_SET_CONTRAST, b);
}

void SSD1306::invertDisplay(bool i) {
    command(i ? 0xA7 : 0xA6);
}
