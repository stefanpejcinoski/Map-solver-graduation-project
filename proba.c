#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>


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



void Read_image(char *filename, double *imHi)
{
    int rows;
    int cols;
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

    __u_char *imORG = calloc(rows*cols, sizeof(__u_char));
    if( (int)fread((void *)imORG, sizeof(__u_char), rows*cols, fp ) != rows*cols )
    {
        printf("\nCannot finish reading input file!");
        exit(1);
    }
    fclose( fp );
    for( cnt = 0 ; cnt<rows*cols ; cnt++ ) imHi[cnt] = (double)imORG[cnt]; // Cast image into double

    free(imORG);

}

/* FLAG ZA MULTITHREAD JOIN RACNA IMPLEMENTACIJA */
volatile bool ex_flag[] = {false,false,false,false};

typedef struct Image{
    int Rows;
    int Cols;
    u_int8_t **img;
    u_int8_t *data;

}Image;

typedef struct Queue{
    Image *queue;
    int length;
}Queue;

void maxPoolResizeFunc(Image *inputimage, Image *outputimage, int factor, int flag){
    int r=0;
    int c=0;
    for (; c<inputimage->Cols; c++){
        for (; r<inputimage->Rows; r++){

            outputimage->img[(int)(r/factor)][(int)(c/factor)]|=inputimage->img[r][c];
        }
    }
    ex_flag[flag]=true;
}

/* KORISTI PAREN FAKTOR PLSS */
void threadedMaxPoolResize(u_int8_t *inputimg, int rows, int cols, u_int8_t **outputbuffer, int factor){

    /* SVEDUVANJE NA SLIKA NA PAREN BROJ REDICI I KOLONI */
    u_int8_t *newinputbuffer;
    int nrows=rows, ncols=cols;
    if (rows%2!=0 || cols%2!=0) {
        if (rows % 2 != 0) {
            nrows=rows-1;
            newinputbuffer = calloc((rows - 1) * (cols), sizeof(u_int8_t));
            int r, c;
            for (c = 0; c < cols; c++) {
                for (r = 0; r < rows; r++) {
                    if (r == rows - 1) {
                        continue;
                    }
                    *(newinputbuffer + c * rows + r) = *(inputimg + c * rows + r);
                }
            }

        } else if (cols % 2 != 0) {
            ncols=cols-1;
            newinputbuffer = (u_int8_t*) calloc((rows) * (cols - 1), sizeof(u_int8_t));
            int r, c;
            for (c = 0; c < cols; c++) {
                for (r = 0; r < rows; r++) {
                    if (c == cols - 1) {
                        continue;
                    }
                    *(newinputbuffer + c * rows + r) = *(inputimg + c * rows + r);
                }
            }
        }
        free(inputimg);
    }

    /* KREIRANJE NA 4 THREADOVI (SAMO THREAD ID) I DELENJE NA SLIKATA NA 4 DELA*/
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    Image *input1 = calloc(1, sizeof(Image));
    Image *input2 = calloc(1, sizeof(Image));;
    Image *input3 = calloc(1, sizeof(Image));;
    Image *input4 = calloc(1, sizeof(Image));;
    Image *output1 = calloc(1, sizeof(Image));
    Image *output2 = calloc(1, sizeof(Image));;
    Image *output3 = calloc(1, sizeof(Image));;
    Image *output4 = calloc(1, sizeof(Image));;
    u_int8_t **dptr = calloc(cols, sizeof(__uint8_t));
    int i,j,iret1,iret2,iret3,iret4;
    for(i=0; i<cols; i++) {
        dptr[i]=newinputbuffer+rows*i;
    }
    input1->Cols=ncols/2;
    input2->Cols=ncols/2;
    input3->Cols=ncols/2;
    input4->Cols=ncols/2;
    input1->Rows=nrows/2;
    input2->Rows=nrows/2;
    input3->Rows=nrows/2;
    input4->Rows=nrows/2;
    input1->data=calloc(input1->Rows*input1->Cols, sizeof(u_int8_t));
    input1->img=calloc(input1->Cols, sizeof(u_int8_t*));
    input2->data=calloc(input2->Rows*input2->Cols, sizeof(u_int8_t));
    input2->img=calloc(input2->Cols, sizeof(u_int8_t*));
    input3->data=calloc(input3->Rows*input3->Cols, sizeof(u_int8_t));
    input3->img=calloc(input3->Cols, sizeof(u_int8_t*));
    input4->data=calloc(input4->Rows*input4->Cols, sizeof(u_int8_t));
    input4->img=calloc(input4->Cols, sizeof(u_int8_t*));
    output1->Cols=ncols/2;
    output2->Cols=ncols/2;
    output3->Cols=ncols/2;
    output4->Cols=ncols/2;
    output1->Rows=nrows/2;
    output2->Rows=nrows/2;
    output3->Rows=nrows/2;
    output4->Rows=nrows/2;
    output1->data=calloc(output1->Rows*output1->Cols, sizeof(u_int8_t));
    output1->img=calloc(output1->Cols, sizeof(u_int8_t*));
    output2->data=calloc(output2->Rows*output2->Cols, sizeof(u_int8_t));
    output2->img=(output2->Cols, sizeof(u_int8_t*));
    output3->data=calloc(output3->Rows*output3->Cols, sizeof(u_int8_t));
    output3->img=calloc(output3->Cols, sizeof(u_int8_t*));
    output4->data=calloc(output4->Rows*output4->Cols, sizeof(u_int8_t));
    output4->img=calloc(output4->Cols, sizeof(u_int8_t*));

    for (i=0; i<input1->Cols; i++){
        for(j=0; j<input1->Rows; j++){
            input1->img[i][j]=dptr[i][j];
        }
    }
    for (i=input2->Cols; i<ncols; i++){
        for(j=0; j<input2->Rows; j++){
            input2->img[i][j]=dptr[i][j];
        }
    }
    for (i=0; i<input3->Cols; i++){
        for(j=input3->Rows; j<nrows; j++){
            input3->img[i][j]=dptr[i][j];
        }
    }
    for (i=input3->Cols; i<ncols; i++){
        for(j=input3->Rows; j<nrows; j++){
            input3->img[i][j]=dptr[i][j];
        }
    }
    free(dptr);

    iret1=pthread_create(&thread1, NULL, maxPoolResizeFunc, (*input1, *output1, factor, 0));
    iret2=pthread_create(&thread2, NULL, maxPoolResizeFunc, (*input2, *output2, factor, 1));
    iret3=pthread_create(&thread3, NULL, maxPoolResizeFunc, (*input3, *output3, factor, 2));
    iret4=pthread_create(&thread4, NULL, maxPoolResizeFunc, (*input4, *output4, factor, 3));

/* IMPLEMENTACIJA NA JOIN KADE SITE 4 THREADOVI RABOTAT PARALELNO (MOZE POSTOI I GOTOVO NEZNAM) */
    while (!(ex_flag[0]&&ex_flag[1]&&ex_flag[2]&&ex_flag[3]));

/* IZLEZ AKO IMALO GRESKA VO NEKOJ THREAD STO NE PREDIZIVIKALA SEGFAULT */
    if(!(iret1 && iret2 && iret3 && iret4)){
        exit(-1);
    }
    nrows=nrows/factor;
    ncols=ncols/factor;
/* SPOJUVANJE NA 4 TE SLIKI VO 1 OUTPUT BUFFER */

/* KREIRANJE NA BUFFER OT */
    __uint8_t *newimbuffer = calloc(nrows*ncols, sizeof(u_int8_t));
    __uint8_t **newim = calloc(ncols, sizeof(u_int8_t *));
    for(i=0; i<ncols; i++) {
        newim[i]=newimbuffer+nrows*i;
    }

/* KOPIRANJE VO BUFFER OT */
    for (i=0 ; i<ncols/2 ; i++){
        for (j=0 ; j<nrows/2 ; j++){
            newim[i][j]=output1->img[i][j];
        }
    }
    for (i=0 ; i<ncols/2 ; i++){
        for (j=0 ; j<nrows/2 ; j++){
            newim[i+ncols/2][j]=output2->img[i][j];
        }
    }

    for (i=0 ; i<ncols/2 ; i++){
        for (j=0 ; j<nrows/2 ; j++){
            newim[i][j+nrows/2]=output3->img[i][j];
        }
    }

    for (i=0 ; i<ncols/2 ; i++){
        for (j=0 ; j<nrows/2 ; j++){
            newim[i+ncols/2][j+nrows/2]=output4->img[i][j];
        }
    }

    memcpy(outputbuffer, newim, ncols* sizeof(u_int8_t*));
    free(newim);
}

int main(){

    printf("%c","h");
    double *image = calloc(1284*2035, sizeof(double));
    char filename[]="/home/stefan/untitled/mapa.pgm";
    Read_image(filename, image);

    __uint8_t **imdata = calloc(((int)2035/10), sizeof(u_int8_t*));
    threadedMaxPoolResize(image, 1284, 2035, imdata, 10);

    FILE *fp=fopen("home/stefan/img.raw", "rb");
    fwrite((void*)*imdata, sizeof(u_int8_t), (int)(2035/10)*(int)(1284/10), fp);
    fclose(fp);
}