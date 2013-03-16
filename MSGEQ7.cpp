/*
  MSGEQ7.cpp -  MSGEQ7 Graphic Equalizer library for Arduino 
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

#define NOP __asm__ __volatile__ ("nop\n\t")
#include "MSGEQ7.h"
#include "Arduino.h"
// NOP is 62.5ns @ 16MHz

//TODO Timings for multiple processor speeds

//TODO Change offset detect to a minimum detect?

//Constructor
MSGEQ7::MSGEQ7(int reset, int strobe, int output){
	_reset=reset;	//Reset Digital Output
	_strobe=strobe;	//Strobe Digital Output
	_output=output;	//Analog "Output" Input
	
	_correctOffset=false;	//Do not adjust offset by default
	//Initialize Measurement Variables
	for(int i=0;i<7;i++){
		_values[i]=0;
	}
	
	pinMode(reset, OUTPUT);
	pinMode(strobe, OUTPUT);
	pinMode(output, INPUT);
}
//Constructor with offset Correct
MSGEQ7::MSGEQ7(int reset, int strobe, int output, bool correctOffset){
	_reset=reset;	//Reset Digital Output
	_strobe=strobe;	//Strobe Digital Output
	_output=output;	//Analog "Output" Input
	
	_correctOffset=correctOffset;	//Offset adjustment Flag
	
	//Initialize Measurement Variables
	for(int i=0;i<7;i++){
		_values[i]=0;
	}
	
	pinMode(reset, OUTPUT);
	pinMode(strobe, OUTPUT);
	pinMode(output, INPUT);
}

//Send a Reset Pulse to the MSGEQ7
void MSGEQ7::reset(){
  digitalWrite(_reset, HIGH);
  //100nS minimum reset pulse
  NOP;
  NOP;
  digitalWrite(_reset, LOW);
}	

//Send a Strobe Pulse to the MSGEQ7
void MSGEQ7::strobe(){
	//Strobe to Strobe and Reset to Strobe delay 72uS Min
	//76.8uS @ 16MHz
	for(int i=0;i<165;i++){
		NOP;
	}
	digitalWrite(_strobe, HIGH);
  
	//Strobe Pulse 18uS Min.
	//21.6uS @ 16MHz
  	for(int i=0;i<40;i++){
		NOP;
	}
	digitalWrite(_strobe, LOW);
}

//Detect the Average Output DC Offset.
int MSGEQ7::detectOffset(){
	//Must be run with no Signal Input
	
	//Reset Offset Variable
	_offset = read63();
	
	//Cycle through all input signals.
	for(int i=0;i<5;i++){
		readAll();
		for(int j=0;j<7;j++){
			_offset = (_offset+_values[j])/2;
		}
	}
	return _offset;
}

//Set the Offset Value
void MSGEQ7::setOffset(int offset){
	_offset=offset;
}

//Get the Offset Value
int MSGEQ7::getOffset(){
	return _offset;
}

//Set offset adjusmtent flag
void MSGEQ7::correctOffset(bool value){
	_correctOffset=value;
}

//Read the Active Frequency - May return negative values if offset correction is used.
int MSGEQ7::read(){
	//Output Settling Time 36uS Min
	//40.8uS @ 16MHz
  	for(int i=0;i<80;i++){
		NOP;
	}
	if(_correctOffset)
		return analogRead(_output)-_offset;	
	return analogRead(_output);	
}

//Updates all frequency values.
void MSGEQ7::readAll(){
	reset();

	for(int i=0;i<7;i++){
		strobe();
		_values[i]=read();
	}
}

//Read Frequency 1 (63Hz)
int MSGEQ7::read63(){
	reset();
	strobe();
	
	_values[0] = read();
	
	if(_correctOffset)
		if((_values[0]-_offset)>0)
			return _values[0]-_offset;
		else 
			return 0;
	return _values[0];
}

//Read Frequency 2 (160Hz)
int MSGEQ7::read160(){
	reset();
	strobe();
	strobe();
	
	_values[1] = read();
	
	if(_correctOffset)
		if((_values[1]-_offset)>0)
			return _values[1]-_offset;
		else 
			return 0;
	return _values[1];
}

//Read Frequency 3 (400Hz)
int MSGEQ7::read400(){
	reset();
	strobe();
	strobe();
	strobe();

	_values[2] = read();
	
	if(_correctOffset)
		if((_values[2]-_offset)>0)
			return _values[2]-_offset;
		else 
			return 0;
	return _values[2];	
}

//Read Frequency 4 (1kHz)
int MSGEQ7::read1000(){
	reset();
	strobe();
	strobe();
	strobe();
	strobe();

	_values[3] = read();
	
	if(_correctOffset)
		if((_values[3]-_offset)>0)
			return _values[3]-_offset;
		else 
			return 0;
	return _values[3];
}

//Read Frequency 5 (2.5kHz)
int MSGEQ7::read2500(){
	reset();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	
	_values[4] = read();
	
	if(_correctOffset)
		if((_values[4]-_offset)>0)
			return _values[4]-_offset;
		else 
			return 0;
	return _values[4];
}

//Read Frequency 6 (6.25kHz)
int MSGEQ7::read6250(){
	reset();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	
	_values[5] = read();
	
	if(_correctOffset)
		if((_values[5]-_offset)>0)
			return _values[5]-_offset;
		else 
			return 0;
	return _values[5];
}

//Read Frequency 7 (16kHz)
int MSGEQ7::read16000(){
	reset();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	strobe();
	
	_values[6] = read();
	
	if(_correctOffset)
		if((_values[6]-_offset)>0)
			return _values[6]-_offset;
		else 
			return 0;
	return _values[6];
}

//Return Frequency at specified index
int MSGEQ7::getValue(int index){
	if(index>6 || index<0)
		return 0;
		
	if(_correctOffset)
		if((_values[index]-_offset)>0)
			return _values[index]-_offset;
		else 
			return 0;
	return _values[index];
}
//Return Frequency 1 stored value.
int MSGEQ7::get63(){
	if(_correctOffset)
		if((_values[0]-_offset)>0)
			return _values[0]-_offset;
		else 
			return 0;
	return _values[0];
}

//Return Frequency 2 stored value.
int MSGEQ7::get160(){
	if(_correctOffset)
		if((_values[1]-_offset)>0)
			return _values[1]-_offset;
		else 
			return 0;
	return _values[1];
}

//Return Frequency 3 stored value.
int MSGEQ7::get400(){
	if(_correctOffset)
		if((_values[2]-_offset)>0)
			return _values[2]-_offset;
		else 
			return 0;
	return _values[2];
}

//Return Frequency 4 stored value.
int MSGEQ7::get1000(){
	if(_correctOffset)
		if((_values[3]-_offset)>0)
			return _values[3]-_offset;
		else 
			return 0;
	return _values[3];
}

//Return Frequency 5 stored value.
int MSGEQ7::get2500(){
	if(_correctOffset)
		if((_values[4]-_offset)>0)
			return _values[4]-_offset;
		else 
			return 0;
	return _values[4];
}

//Return Frequency 6 stored value.
int MSGEQ7::get6250(){
	if(_correctOffset)
		if((_values[5]-_offset)>0)
			return _values[5]-_offset;
		else 
			return 0;
	return _values[5];
}

//Return Frequency 7 stored value.
int MSGEQ7::get16000(){
	if(_correctOffset)
		if((_values[6]-_offset)>0)
			return _values[6]-_offset;
		else 
			return 0;
	return _values[6];
}




