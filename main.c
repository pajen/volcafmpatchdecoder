/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Console mode (command line) program.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>

/****************************************************************************
 *                                                                          *
 * Function: main                                                           *
 *                                                                          *
 * Purpose : Main entry point.                                              *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"

struct wav_header
{
  char          riff[4];
  uint32_t      size;
  char          wave[4];
  char          fmt[4];
  uint32_t      chunk_size;
  uint16_t      encode; 
  uint16_t      channels;
  uint32_t      fs;
  uint32_t      Bps;
  uint16_t      block;
  uint16_t      bps;
  char          data[4];
  uint32_t      data_size;
};

struct fw_block
{
  short         block;
  short         last;
  short         bytes;
};

#define TXHEADER_STR	 "KORG SYSTEM FILE"
#define TXHEADER_STR_LEN 16

struct tx_header
{
  uint8_t       Header[TXHEADER_STR_LEN];
  uint32_t      DeviceID;
  uint8_t       BlockCode;
  uint8_t       Num;
  uint8_t       Version[2];
  uint32_t      Size;
  uint16_t      m_Reserved;
  uint16_t      m_Speed;
};

unsigned short crc16_table[] = {	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,	
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };

unsigned short crc16(unsigned char * buf, int len)
{ 
  unsigned short crc = 0xffff;
	int j;
  for( j = 0; j < len; j++ )
 	     crc = crc16_table[(crc >> 8) ^ buf[j]] ^ ((crc & 0xff) << 8 );
  return crc;  
}

const char op_byte[] = {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  11,  12,  13,  13,  14,  15,  15,  16,  12};   
const char op_bits[] = {   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,0x02,0x22,0x03,0x02,0x23,   7,0x01,0x15,   7,0x34};
//FIXED OPERATOR LEVEL BUG MAR 22 !

const unsigned char par_byte[]= { 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 111, 112, 113, 114, 115, 116, 116, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};
const unsigned char par_bits[]= {   7,   7,   7,   7,   7,   7,   7,   7,   5,0x03,0x31,   7,   7,   7,   7,0x01,0x13,0x43,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7};

int DEBUG;


void set_bits(unsigned char *addr, int val, int bits)
{
  unsigned char mask, shifted_mask;

  mask = ( 0x7f >> (7 - (bits&7)));
  shifted_mask = mask << (bits >> 4) ;
  *addr = (*addr & (~shifted_mask)) | ((val & mask)<<(bits >> 4));
 
} 

unsigned char get_bits(unsigned char *addr, char bits)
{
  unsigned char mask, shifted_mask;
 mask = ( 0x7f >> (7 - (bits&7)));
  shifted_mask = mask << (bits >> 4) ;
  return (*addr & shifted_mask) >> ( bits>>4);
} 

void convert_127_to_155( unsigned char *dst, unsigned char *src)
{
  int op, i, j=0 ;
  
  for (op=0; op<6; op++)
  {
    for ( i = 0; i < 21; i++)
      {
              dst[j++] = get_bits( src + op_byte[i] + op*17 , op_bits[i] ); 
      }    
  }
  for ( i = 0; i < (155-126); i++)
      {
              dst[j++] = get_bits( src + par_byte[i], par_bits[i] ); 
      }    
   
}

unsigned char SYSEX_HEADER_155[] = {0xf0, 0x43, 0x00, 0x00, 0x01, 0x1b};
unsigned char SYSEX_HEADER_4096[] = {0xf0, 0x43, 0x00, 0x09, 0x20, 0x00};
unsigned char SYSEX_FOOTER[] = {0xf7};


void export_sysex_155(FILE * outfile, char * buf)
{
  int i;
  char sum;

  fwrite(SYSEX_HEADER_155, sizeof(SYSEX_HEADER_155), 1, outfile);
  fwrite(buf, 155, 1, outfile);
  for (sum=0, i=0; i<155; i++){
    sum+= buf[i];
  }
  sum = (128 - (sum & 0x7f));
  fwrite(&sum, 1, 1, outfile);
  fwrite(SYSEX_FOOTER, sizeof(SYSEX_FOOTER), 1, outfile);
  
}

#define BUFFER_SIZE 0x4000
#define COUNT_THRESHOLD 15
#define SIGN (1)
#define AMP_MINIMUM 4096

char buffer[BUFFER_SIZE];
unsigned char  * res;
short * samples;


int decode_preamble( FILE * infile)
{
  int count, amplitude_ok;
  unsigned int word;
  short s, new_s;

  s=1;
  word=0;
  count=0;
  amplitude_ok=0;
  for( ;  ;  )
  {
    if( !fread(&new_s, sizeof(short), 1, infile) )
      { 
        printf("File end error\n");
        exit(-1);
      }
       
    if ( s*SIGN < 0 && new_s*SIGN >= 0 && amplitude_ok)
      {
          word >>= 1;
          if ( count < COUNT_THRESHOLD)
            word |= 1 << 31;
          if (count > COUNT_THRESHOLD*2)
            word=0;
          if (word == 0x7fffffff)
            return 0;      
      count = 0;  
      amplitude_ok=0;
      }
    else
      {
        count++;
        if (abs(new_s) > AMP_MINIMUM)
          amplitude_ok = 1;    
      }
    s = new_s;
  }
  return 1;
}


int decode(unsigned char * result, int len, FILE * infile)
{
  int  bits, count, r;
  unsigned int word;
  short s, new_s, amplitude_min, amplitude_max;
  
  s=1;
  bits = 0;
  word=0;
  count=0;
  r = 0;
  amplitude_min = 0;
  amplitude_max = 0;
  for( ; r < len;  )
  {
    if( !fread(&new_s, sizeof(short), 1, infile) )
      { 
        printf("File end error\n");
        exit(-1);
      }
    if ( s*SIGN < 0 && new_s*SIGN >= 0 && amplitude_min && amplitude_max )
      {
          bits++;
          word >>= 1;
          if ( count < COUNT_THRESHOLD)
            word |= 1 << 31;
          if ((count<8) || (count>20))
              printf("Weird signal shape:%d ,%d \n", count, (ftell(infile) - sizeof( struct wav_header ))/2);

          if (bits == 8) {
            result[r++] = word >> 24;
            bits = 0;
          }  
      count = 0;  
      amplitude_min = 0;
      amplitude_max = 0;

      }
    else
      {
         count++;
         if (new_s > AMP_MINIMUM)
           amplitude_max = 1; 
         if (new_s < -AMP_MINIMUM)
           amplitude_min = 1; 
   
      }
    s =  new_s;
  }
  return r;
}

void printhex(unsigned char *r, int len)
{
  int i;
  for (i=0; i < len; i++)
  {
      if (i && !(i%16))
        printf("\n");
      printf("%02x ", r[i]);
  } 
  printf("\n");
}
 
void assert_sequence(char * seq, int len, FILE * infile,  char * message )
{
  unsigned char * buf;
  buf = malloc(len);
  decode(buf, len, infile);
  if (strncmp((char *)buf, seq, len))
    {
    printf("ASSERT ERROR :%s\n", message);
    exit(0);
    }
  free(buf);
}




int main(int argc, char *argv[])
{
  int fa, frames;
  int sample_i;
  FILE * infile, * outfile;
  char fname[100];
  char *p;
  char sysex_155[156];
  unsigned short crc;

  struct wav_header * wh;

  printf( "Volca Decode Sysex\n" );
  if( argc < 2 )
  {
    printf( "Usage: %s wav_file(s)\n", argv[0] );
    exit( -1 );
  }
  for( fa = 1; fa < argc; fa++ )
  {
    strcpy( fname, argv[fa] );
    if( ( infile = fopen( fname, "rb" ) ) == NULL )
    {
      printf( "Cannot open file %s for reading\n", fname );
      exit( -3 );
    }
    fread( buffer, 1, sizeof( struct wav_header ), infile );
    wh = ( struct wav_header * ) buffer;
	  if(  strncmp( wh->riff, "RIFF", 4 ) ||
	       strncmp( wh->wave, "WAVE", 4 ) ||
	       strncmp( wh->fmt, "fmt ", 4 ) ||
	       strncmp( wh->data, "data", 4 ) || wh->encode != 1 )
          {
            printf( "Invalid wave file format.\n" );
            exit( 0 );
          }
    if (wh->channels != 1)
    {
      printf("Wav file needs to be 1 channel (MONO)!\n");
      exit(0);
    }
    if (wh->fs != 44100)
    {
      printf("Wav file needs to be 44100 sample rate!\n");
      exit(0);
    }
    if (wh->bps != 16)
    {
      printf("Wav file needs to be 16 bits per sample!\n");
      exit(0);
    }
    
    printf("FILE: %s, %d channel(s), rate:%d, width:%d, block:%d\n", fname, wh->channels, wh->fs, wh->bps, wh->block);
    if( wh->block )
      frames = wh->data_size / wh->block;
    else
      frames = 0;
    if( ( res = malloc( frames / 8 ) ) == NULL ||
	     ( samples = malloc( frames * wh->channels * sizeof( short ) ) ) == NULL )
      {
        printf( "Memory allocation error.\n" );
        exit( 0 );
      }
        
        {
          sample_i = 0;
          decode_preamble( infile );
          
          
          assert_sequence("\xa9" ,1, infile, "Frame start(a9)");  
          assert_sequence(TXHEADER_STR ,TXHEADER_STR_LEN, infile, "Frame KORG Header");  
          decode(res,  33, infile);
          
          if (res[5] == 0xff) // SINGLE PATCH
          {
            decode(res,  2, infile);
            decode_preamble( infile );
            decode(res,  1, infile);
            decode(res,  140, infile);
            crc = crc16(res, 140);
  
	            
            assert_sequence( (char*) &crc, 2, infile, "CRC");
              
            convert_127_to_155(sysex_155, res);
            printf("Name:%.10s\n", &sysex_155[9*16+1]);
            p = fname + strlen( fname );
            strcpy( p, ".syx" );
            outfile = fopen( fname, "wb" );
            export_sysex_155(outfile, sysex_155);
          }
          else if (res[5] == 0x00) // CARTRIDGE
          {
            int i, j;
            signed char sum;
            decode(res,  2, infile);
            p = fname + strlen( fname );
            strcpy( p, "_cart.syx" );
            outfile = fopen( fname, "wb" );    
            fwrite(&SYSEX_HEADER_4096, sizeof(SYSEX_HEADER_4096), 1, outfile);
            sum=0;
            for (i= 0; i<32; i++) {
  
              decode_preamble( infile );
              assert_sequence("\xa9" ,1, infile, "Frame start(a9)");  
          
              
              decode(res,  140, infile);
              crc = crc16(res, 140);
    
              assert_sequence( (char*) &crc, 2, infile, "CRC");
              
              fwrite(res, 128, 1, outfile);
              for(j=0; j<128;j++)
                sum += res[j];
              convert_127_to_155(sysex_155, res);
              
  
              printf("%.10s\n", &sysex_155[9*16+1]);
              decode_preamble( infile );
              decode(res,  1, infile);
              decode(res,  128, infile);
              
            }  
            sum = (128 - (sum &0x7f)); 
            fwrite(&sum, 1, 1, outfile);
            fwrite(&SYSEX_FOOTER, sizeof(SYSEX_FOOTER), 1, outfile);
            
            
          }            

        }
    fclose( infile );
  

  }  
    return 0;
}

