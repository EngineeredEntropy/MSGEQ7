/*
  MSGEQ7.h -  MSGEQ7 Graphic Equalizer library for Arduino 
  Version: 0.1
  
  Copyright (c) 2013 Joseph Diaz.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MSGEQ7_h
#define MSGEQ7_h

class MSGEQ7
{
  public:
    MSGEQ7(int reset, int strobe, int output);
	MSGEQ7(int reset, int strobe, int output, bool correctOffset);
    void reset();
    void strobe();
	void correctOffset(bool correct);
	int detectOffset();
	int getOffset();
	void setOffset(int offset);
    int read();
	void readAll();
	
	int get63();
	int get160();
	int get400();
	int get1000();
	int get2500();
	int get6250();
	int get16000();
	int getValue(int index);
	
	int read63();
	int read160();
	int read400();
	int read1000();
	int read2500();
	int read6250();
	int read16000();
	
  private:
    int _reset;
    int _strobe;
    int _output;
	int _offset;
	int _values[7];
	bool _correctOffset;
};

#endif