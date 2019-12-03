#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
void maxPoolResize(uint8_t *imdata, int rows, int cols, int factor, uint8_t *buffer){
    int i,j,f;
    for(i=0 ; i<rows ; i++ ){
        for(j=0 ; j<cols ; j++){

           *(buffer+((int)floor((i/factor))*(int)floor(cols/factor)+(int)floor(j/factor)))=*(imdata+i*cols+j);
        }

    }
}


static void eat_white(FILE *fp)
{
short int ch;

while ((ch=getc(fp)) != EOF)
if (ch == '#')
do {
ch = getc(fp);
} while ((ch != '\n') && (ch != EOF));
else if ((ch != ' ')  && (ch != '\t') && (ch != '\n') && (ch != '\r'))
{
ungetc(ch,fp);
return;
}
}



void Read_image(char *filename, uint8_t *imHi)
{
int rows, cols;
char magic[3];
int bitdepth, cnt;
FILE *fp;

//im.flt = flt;
fp = fopen(filename,"rb");
if (fp == NULL)
//cout<<"Unable to open input image file "<<filename<<NLN,
exit(1);

//file_name = new SINT8[strlen(filename)+1];
//strcpy(file_name,filename);


if ( strstr(filename,".pgm")!=NULL )
{

magic[0] = magic[1] = magic[2] = '\0';
fread(magic,1,2,fp);

if (strcmp(magic,"P5") == 0)
{ /* PGM file format. */
eat_white(fp);
if ((fscanf(fp,"%d",&cols) != 1) || (cols < 1))
//cout<<"Invalid PNM header in input image"<<filename<<NLN,
exit(1);
eat_white(fp);
if ((fscanf(fp,"%d",&rows) != 1) || (rows < 1))
//cout<<"Invalid PNM header in input image" << filename << NLN,
exit(1);
eat_white(fp);
if ((fscanf(fp,"%d",&bitdepth) < 1) ||
(bitdepth > 255) || (bitdepth < 0))
//cout<<"Invalid PNM header in input image"<< filename << NLN,
exit(1);
fgetc(fp);
bitdepth = 8;
}

}

unsigned char *imORG = calloc(rows*cols, sizeof(unsigned char));
if( (int)fread( imORG, sizeof(unsigned char), rows*cols, fp ) != rows*cols )
{
printf("\nCannot finish reading input file!");
exit(1);
}
fclose( fp );
for( cnt = 0 ; cnt<rows*cols ; cnt++ ) imHi[cnt] = (uint8_t)imORG[cnt];

free(imORG);
}

int main()
{
    uint8_t *image=calloc(1284*2035, sizeof(uint8_t));
    Read_image("mapa.pgm", image);
    uint8_t *nbuffer = calloc((int)floor(1284/40)*(int)floor(2035/40), sizeof(uint8_t));
    FILE *fp1 = fopen("img1.raw", "wb");
    if(fp1==NULL)
    {
        exit(-1);
    }
    fwrite(image,sizeof(uint8_t), (int)floor((1284)*(2035)), fp1);
    fclose(fp1);
    maxPoolResize(image, 2035, 1284, 40, nbuffer);
    FILE *fp0=fopen("img.raw", "wb");
    if(fp0==NULL)
    {
        exit(-1);
    }
    putchar('G');
    fwrite(nbuffer, 1600, sizeof(uint8_t), fp0);
    putchar('B');
    fclose(fp0);
    return 0;
}


