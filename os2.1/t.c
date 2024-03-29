/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
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
********************************************************************/

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/*
UART0 base address: 0x101f1000;
UART1 base address: 0x101f2000;
UART2 base address: 0x101f3000;
UART3 base address: 0x10009000;

// flag register at 0x18
//  7    6    5    4    3    2   1   0
// TXFE RXFF TXFF RXFE BUSY
// TX FULL : 0x20
// TX empty: 0x80
// RX FULL : 0x40
// RX empty: 0x10
// BUSY=1 :  0x08
*/

int i;
int N = 10;
int v[] = {1,2,3,4,5,6,7,8,9,10};
int sum;
char string[50];
char *tab = "0123456789ABCDEF";
//#include "string.c"
#include "uart.c"

UART *up;

int main()
{
  uart_init();

  for (i=0; i<4; i++) {
    up = &uart[i];
    fuprints(up, "%s", "Enter the line for this UART: ");
    ugets(up, string);
    fuprints(up, "%s", "   Echo: ");fuprints(up, "%s", string);fuprints(up, "%s", "\n\r");
  }

  while(1){
    ugets(up, string);
    fuprints(up, "    ");
    if (strcmp(string, "quit")==0)
       break;
    fuprints(up, string);  fuprints(up, "\n\r");
  }


  fuprints(up, "%s", "Compute sum of array\n\r");
  sum = 0;
  for (i=0; i<N; i++)
    sum += v[i];
  uputc(up, (sum/10)+'0'); uputc(up, (sum%10)+'0');
  fuprints(up, "Sum is %d", sum);
  fuprints(up, "%s", "\n\rEND OF RUN\n\r");
 
}
