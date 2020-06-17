
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include<math.h>
#include <ctype.h>
#include <stdbool.h>
#define HI(num)	(((num) & 0x0000FF00) << 8)
#define LO(num)	((num) & 0x000000FF)
#define nullptr NULL
#define endl '/n'



typedef struct cords{
    int x;
    int y;
}cords;
typedef struct Node{

    int x;
    int y;
    bool isOpen:1;
    bool isClosed:1;
    int cost;
    int parentIndex;
} Node;

typedef struct cordpairs{
    cords c1;
    cords c2;
}cpairs;
typedef struct data{
    float *angles;
    int numofang;
    cpairs *linesegcords;
}data;

typedef struct line{
int a;
int b;
} line;

bool checkOpen(Node *list, int len){
    int i;
    bool ret=false;
    for(i=0 ; i<len ; i++){
        ret|=list[i].isOpen;
    }
    return ret;
}
int getMinIndex(Node *list, int len){
    int i, ind;
    for(i=0 ; i<len ; i++){
        if (list[i].isOpen==true){
            ind=i;
            break;
        }
    }
    for(i=0 ; i<len ; i++){
        if(list[i].isOpen==true && list[i].isClosed==false){
            if(list[i].cost<list[ind].cost){
                ind=i;
            }
        }
    }
    return ind;
}
int euclideanDistance(int x1, int y1, int x2, int y2){
    return (int)round(sqrt(pow(x1-x2, 2)+pow(y1-y2, 2)));
}

int Astar(int **map,cords **rpath, int width, int height, int startx, int starty, int endx, int endy){
    if(map[starty][startx]>0 || map[endy][endx]>0){
        printf("GRESKA");
        
        return 0;
    }
   
   
    Node *heap = (Node*)calloc(width*height, sizeof(Node));
    int nodecounter=0, stepcounter=0;
    heap[nodecounter].cost=euclideanDistance(startx, starty, endx, endy);
    heap[nodecounter].isClosed=false;
    heap[nodecounter].isOpen=true;
    heap[nodecounter].parentIndex=-1;
    heap[nodecounter].x=startx;
    heap[nodecounter].y=starty;
    nodecounter++;
    while(checkOpen(heap, width*height)){
       // printf("looped");
        stepcounter++;
        int curindex=getMinIndex(heap, height*width);
       
        
        heap[curindex].isOpen=false;
        heap[curindex].isClosed=true;
        if(heap[curindex].x==endx && heap[curindex].y==endy){
           // printf("done");
            cords *path = (cords*)calloc(stepcounter, sizeof(cords));
            int ind=curindex, i=0;
            while(heap[ind].parentIndex>-1){
                path[i].x=heap[ind].x;
                path[i].y=heap[ind].y;
                ind=heap[ind].parentIndex;
                i++;
               
            }
            

            *rpath=path;
            free(heap);
            return i;
        }
        else {
           // printf("%i, %c", nodecounter, '_');
            //printf("line1");
            if(heap[curindex].y+1<height)
            {
            if (map[heap[curindex].y+1][heap[curindex].x]==0){
                heap[++nodecounter].x=heap[curindex].x;
                heap[nodecounter].y=heap[curindex].y+1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
           // printf("line2");
            if(heap[curindex].x+1<width)
            {
             if (map[heap[curindex].y][heap[curindex].x+1]==0){
                heap[++nodecounter].x=heap[curindex].x+1;
                heap[nodecounter].y=heap[curindex].y;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
             }
            }
//printf("line3");
           if(heap[curindex].y+1<height && heap[curindex].x+1<width)
           {
             if (map[heap[curindex].y+1][heap[curindex].x+1]==0){
                heap[++nodecounter].x=heap[curindex].x+1;
                heap[nodecounter].y=heap[curindex].y+1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
           }
           //printf("line4");
            if(heap[curindex].y-1>0)
            {
            
             if (map[heap[curindex].y-1][heap[curindex].x]==0){
                heap[++nodecounter].x=heap[curindex].x;
                heap[nodecounter].y=heap[curindex].y-1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
            //printf("line5");
            if(heap[curindex].x-1>0)
            {
             if (map[heap[curindex].y][heap[curindex].x-1]==0){
                heap[++nodecounter].x=heap[curindex].x-1;
                heap[nodecounter].y=heap[curindex].y;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
           // printf("line6");
            if(heap[curindex].y+1<height && heap[curindex].x-1>0){
             if (map[heap[curindex].y+1][heap[curindex].x-1]==0){
                heap[++nodecounter].x=heap[curindex].x-1;
                heap[nodecounter].y=heap[curindex].y+1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
          //  printf("line7");
            if(heap[curindex].y-1>0 && heap[curindex].x+1<width){

             if (map[heap[curindex].y-1][heap[curindex].x+1]==0){
                heap[++nodecounter].x=heap[curindex].x+1;
                heap[nodecounter].y=heap[curindex].y-1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
          // printf("line8");
            if(heap[curindex].y-1>0 && heap[curindex].x-1>0)
            {
             if (map[heap[curindex].y-1][heap[curindex].x-1]==0){
                heap[++nodecounter].x=heap[curindex].x-1;
                heap[nodecounter].y=heap[curindex].y-1;
                heap[nodecounter].parentIndex=curindex;
                heap[nodecounter].isOpen=true;
                heap[nodecounter].isClosed=false;
                  map[heap[nodecounter].y][heap[nodecounter].x]=255;
                heap[nodecounter].cost=stepcounter+euclideanDistance(heap[nodecounter].x, heap[nodecounter].y, endx, endy);
            }
            }
          //  printf("loopever");

        }
    }
}
int distP2line(cords *p1, cords *p2, cords *p3){
return abs((p2->y-p1->y)*p3->x-(p2->x-p1->x)*p3->y+p2->x*p1->y-p2->y*p1->x)/sqrt(pow(p2->y-p1->y, 2)+pow(p2->x-p1->x, 2));
}

line *equation(int x1, int x2, int y1, int y2){
int m=floor((y2-y1)/(x2-x1));
int c=y1-x1*m;
line *eq = (line*)calloc(1, sizeof(line));
eq->a=m;
eq->b=c;
return eq;
}
cords *transform(int *path, int len){
    cords *pathh=(cords*)calloc(len/2, sizeof(cords));
    int i, j=0;
    for(i=0 ; i<len/2 ; i++){
        pathh[i].x=path[i+j];
        pathh[i].y=path[i+(++j)];

    }
    return pathh;
}
int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;

    ret_val = (int **)calloc(row, sizeof(int *));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i) {
        ret_val[i] = (int *)calloc(col, sizeof(int));
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    return ret_val;
}

bool hit(line *eq,int x1, int x2, int **slika){
int a,b,y;
int x=x1;
a=eq->a;
b=eq->b;
while(x!=x2){
if(x<x2){
y=a*x+b;
if (slika[y][x]<1 && slika[y+1][x]<1 && slika[y][x+1]<1 && slika[y-1][x]<1 && slika[y][x-1]<1 && slika[y-1][x-1]<1 && slika[y+1][x+1]<1 && slika[y+1][x-1]<1 && slika[y-1][x+1]<1){
x++;
if(x==x2)
return false;
continue;
}
else
break;
}
else if(x>x2){
y=a*x+b;
if( slika[y][x]<1 && slika[y+1][x]<1 && slika[y][x+1]<1 && slika[y-1][x]<1 && slika[y][x-1]<1 && slika[y-1][x-1]<1 && slika[y+1][x+1]<1 && slika[y-1][x+1]<1 && slika[y+1][x-1]<1){
x--;
if(x==x2)
return false;
continue;
}
else
break;
}
}
return true;
}
int processPath(cords **npath, cords *path, int length, int **slika){
bool wall=false;

(*npath)=(cords*)calloc(length, sizeof(cords));
int i=0, x1, y1, x2, y2, len=0;
line *eq=nullptr;
while (i<length)
{
x1=path[i].x;
y1=path[i++].y;
while (i<length)
{
 x2=path[i].x;
 y2=path[i++].y;

 if (x1==x2){
 continue;
 }
 eq=equation(x1, x2, y1, y2);
 if(hit(eq, x1, x2, slika))
 {

 wall=true;
 break;
 }
}
free(eq);
if (wall)
{

//printf("%i", len);

(*npath)[len].x=x2;
(*npath)[len++].y=y2;
//printf("%i, %i", x2, y2);
// fflush(stdout);
}
else {

break;
}
}

return len;
}


int **maxPool(int **img1, int rows, int cols, int factor, int nrows, int ncols){
	int i,j,c1,c2;
    int **img2=allocate_dynamic_matrix(nrows, ncols);
	for (i=0 ; i<rows ; i++){
            c1=(int)(i/factor);
            if(c1>=nrows){
                c1=c1-1;
            }
        for(j=0 ; j<cols ; j++){
            c2=(int)(j/factor);
            if(c2>=ncols){
                c2=c2-1;
            }
            img2[c1][c2]|=img1[i][j];
        }
	}
	//	for (i=0; i<nrows ; i++){
     //       for(j=0 ; j<ncols ; j++){
      //          if  (img2[i][j]>0){
       //             img2[i][j]=255;
        //        }
         //       else
          //          img2[i][j]=0;
           // }
	//	}
return img2;
}


typedef struct _PGMData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int distance(int x1,int x2,int y1,int y2){
return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
}

data *pathwithangles(cords *path, int len){
data *retdata=(data*)calloc(1, sizeof(data));
retdata->linesegcords=(cpairs*)calloc(len-1, sizeof(cpairs)+4);

int i;
if (len<3){
   retdata->linesegcords->c1.x=path[0].x;
   retdata->linesegcords->c1.y=path[0].y;
   retdata->linesegcords->c2.x=path[1].x;
   retdata->linesegcords->c2.y=path[1].y;
   retdata->angles=nullptr;
    return retdata;
}



retdata->numofang=len-2;
retdata->angles=(float*)calloc(len-1, sizeof(float));
for (i=0 ; i<len ; i++){

retdata->linesegcords[i].c1.x=path[i].x;
retdata->linesegcords[i].c1.y=path[i].y;
retdata->linesegcords[i].c2.x=path[i+1].x;
retdata->linesegcords[i].c2.y=path[i+1].y;
//printf("%i, %c, %i", path[i].x, '-', path[i].y);

}
for(i=1 ; i<len ; i++){

retdata->angles[i-1]=(180.0+(180.0/3.14)*((atan2((double)path[i+1].y-(double)path[i].y, (double)path[i+1].x-(double)path[i].x)-atan2((double)path[i-1].y-(double)path[i].y, (double)path[i-1].x-(double)path[i].x))));


}
return retdata;
}
void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;

    for (i = 0; i < row; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch)) {
        ;
    }

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else {
        fseek(fp, -1, SEEK_CUR);
    }
}
PGMData* readPGM(const char *file_name, PGMData *data)
{
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);

    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i][j] = (hi << 8) + lo;
            }
        }
    }
    else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = fgetc(pgmFile);
                data->matrix[i][j] = lo;
            }
        }
    }

    fclose(pgmFile);
    return data;

}
void writePGM(const char *filename, const PGMData *data)
{
    FILE *pgmFile;
    int i, j;
    int hi, lo;

    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }

    fprintf(pgmFile, "P5 ");
    fprintf(pgmFile, "%d %d ", data->col, data->row);
    fprintf(pgmFile, "%d ", data->max_gray);

    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = HI(data->matrix[i][j]);
                lo = LO(data->matrix[i][j]);
                fputc(hi, pgmFile);
                fputc(lo, pgmFile);
            }

        }
    }
    else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = LO(data->matrix[i][j]);
                fputc(lo, pgmFile);
            }
        }
    }

    fclose(pgmFile);
    deallocate_dynamic_matrix(data->matrix, data->row);
}
void sendOverRS232(unsigned char *message, int port)
{
   int i=0 ;
   while(i<7){
   write(port, &message[i], 1);
   tcdrain(port);
   i++;
   }
   
}
bool sendToRobot(data *path, int len, int anglecorrection){
int uart0_filestream = -1;
int errorcount;
int anglecount=0;
__uint8_t speed=10;
	uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{

		printf("Error - Unable to open UART");
        return false;
	}
    struct termios options;
    if(!isatty(uart0_filestream)) {
    printf("bad port");
    return false;
    }
	if(tcgetattr(uart0_filestream, &options)<0){
        printf("bad port config");
        return false;
    }

	options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                    INLCR | PARMRK | INPCK | ISTRIP | IXON);
    options.c_oflag = 0;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    options.c_cflag &= ~(CSIZE | PARENB);
    options.c_cflag |= CS8;
    options.c_cc[VMIN]  = 1;
    options.c_cc[VTIME] = 0;
    if(cfsetispeed(&options, B9600) < 0 || cfsetospeed(&options, B9600) < 0) {
    printf("failed to set speed");
    return false;
}
	if(tcsetattr(uart0_filestream, TCSAFLUSH, &options) < 0) { 
    printf("failed to set config"); 
    return false; 
    }
   // if (ioctl(uart0_filestream, O_EXCL, NULL) < 0) {
   // printf("failed to gain exclusive access");
   // return false;
    

tcflush(uart0_filestream, TCIOFLUSH);
    unsigned char msg[7];
    msg[0]='2';
    msg[1]='0';
    int i;
    __int8_t startangle=180+(180/3.14)*(atan2(path->linesegcords[0].c1.y-path->linesegcords[0].c2.y, path->linesegcords[0].c1.x-path->linesegcords[0].c2.x))+anglecorrection;
    if(startangle>0){
        msg[2]='r';
    }
    else {
        msg[2]='l';
    }
if (startangle%3!=0){
startangle++;
}
printf("%i\n", startangle);
fflush(stdout);
    msg[3]=0;
    msg[4]=0;
    msg[5]='+';
    msg[6]=abs(startangle);
    
    while(1)
     {
         printf("sending");
         fflush(stdout);
         sendOverRS232(msg, uart0_filestream);
         char x;
         
         fflush(stdout);
         int chars=read(uart0_filestream, &x, 1);
         if(chars==0)
         {
             printf("error");
             
             fflush(stdout);
        errorcount++;
      }
      else{
      printf("_recieved_");
        printf("%c, %c, %c", '-', x, '-');
	}
	  chars=read(uart0_filestream, &x, 1);

       if(errorcount>=10){
            break;
        }
     if(chars>0 && x=='e')
     {
		 printf("success");
		 break;
     }
     else if(x!='e'){
		
		 errorcount++;
	 }
	 
  if(errorcount>=10){
      return false;
  }
}
  errorcount=0;
    

    for(i=0 ; i<len; i++){
        int dist=(distance(path->linesegcords[i].c1.x, path->linesegcords[i].c2.x, path->linesegcords[i].c1.y, path->linesegcords[i].c2.y))*200;
        msg[2]='f';
        msg[3]=(dist>>8)&0xff;
        msg[4]=(dist)&0xff;
        msg[5]=140;
        msg[6]=0;
        printf("%i\n", dist);
     while(1)
     {
         printf("sending");
         fflush(stdout);
         sendOverRS232(msg, uart0_filestream);
         char x;
         
         fflush(stdout);
         int chars=read(uart0_filestream, &x, 1);
         if(chars==0)
         {
             printf("error");
             
             fflush(stdout);
        errorcount++;
      }
      else{
      printf("_recieved_");
        printf("%c, %c, %c", '-', x, '-');
	}
       if(errorcount>=10){
            break;
        }
          chars=read(uart0_filestream, &x, 1);
     if(chars>0 && x=='e')
     {
		 printf("success");
		 break;
     }
     else if(x!='e'){
		
		 errorcount++;
	 }
	 
  if(errorcount>=10){
      return false;
  }
}
 
  errorcount=0;
if(anglecount<path->numofang){
     if(path->angles[anglecount]>0){
        msg[2]='r';
    }
    else {
        msg[2]='l';
    }
int angle=abs(path->angles[anglecount+1]);
if (angle%3!=0){
angle++;
}
    msg[3]=0;
    msg[4]=0;
    msg[5]='+';
    msg[6]=angle;
    while(1)
     {
         printf("sending");
         fflush(stdout);
         sendOverRS232(msg, uart0_filestream);
         char x;
         
         fflush(stdout);
         int chars=read(uart0_filestream, &x, 1);
         if(chars==0)
         {
             printf("error");
             
             fflush(stdout);
        errorcount++;
      }
      else{
      printf("_recieved_");
        printf("%c, %c, %c", '-', x, '-');
	}
       if(errorcount>=10){
            break;
        }
          chars=read(uart0_filestream, &x, 1);
     if(chars>0 && x=='e')
     {
		 printf("success");
		 break;
     }
     else if(x!='e'){
		
		 errorcount++;
	 }
	 
  if(errorcount>=10){
      return false;
  }
}
  errorcount=0;
    

}
    }
return true;
}
int main(int argc, char **argv) {
int xscord, yscord, xecord, yecord, anglecorrection;
if(geteuid() != 0)
{
  printf("Ne e root");
  exit(1);
}
if (argc<4){
printf("Nemas argumenti");
return 0;
}

if(argc==5){
anglecorrection=strtol(argv[5], nullptr, 10);
}
else{
anglecorrection=0;
}
xscord=(int)strtol(argv[1], nullptr, 10);
yscord=(int)strtol(argv[2], nullptr, 10);
xecord=(int)strtol(argv[3], nullptr, 10);
yecord=(int)strtol(argv[4], nullptr, 10);

	int factor = 20;
	char filename[]="mapac.pgm";
	PGMData *slika = calloc(1, sizeof(PGMData));
	slika = readPGM(filename, slika);
	FILE *fp1=fopen("izlez2.raw", "wb");
	int i,j;
	for(i=0 ; i<slika->row ; i++){
        for(j=0 ; j<slika->col ; j++){
            fputc(slika->matrix[i][j], fp1);
        }
	}
	fclose(fp1);

	int nrows = (int)floor(((double)slika->row/(double)factor));
	int ncols = (int)floor(((double)slika->col/(double)factor));

	int **nslika = nullptr;
	nslika=maxPool(slika->matrix, slika->row, slika->col, factor, nrows, ncols);


    int **tmp=allocate_dynamic_matrix(nrows, ncols);
    for(i=0 ; i<nrows ; i++){
	for(j=0 ; j<ncols ; j++){
		tmp[i][j]=nslika[i][j];
	}
}
   /* FILE *fp=fopen("izlez.raw", "wb");
	for(i=0 ; i<nrows ; i++){
        for(j=0 ; j<ncols ; j++){
            fputc(tmp[i][j], fp);
        }
	}
	fclose(fp);
  */
  
    cords *path=nullptr;
    int len=Astar(nslika, &path,ncols, nrows, xscord, yscord, xecord, yecord);
   // printf("DONE");
   int nlen;
   // uchar format za prakanje

cords *npath=(cords*)calloc(len, sizeof(cords));
for (i=0 ; i <len ; i++){
    npath[i]=path[len-i];
}

cords *npathh=nullptr;
nlen=processPath(&npathh, npath, len, tmp);



data *points=pathwithangles(npathh, nlen);








    for(i=0 ; i<len ; i++){
       
         tmp[npath[i].y][npath[i].x]=255;

    }
    for(i=0 ; i<points->numofang ; i++){
        printf("%f, %c", points->angles[i], '_');
    }
 /*    FILE *fp3=fopen("resena.raw", "wb");
     FILE *fp4=fopen("koordinati.txt", "w");
     for(i=0 ; i<nlen ; i++){
         fprintf(fp4, "%i, %c", npathh[i].x, ',');
         fprintf(fp4, "%i, %c", npathh[i].y, ',');
     }
     fclose(fp4);
	for(i=0 ; i<nrows ; i++){
        for(j=0 ; j<ncols ; j++){

            fputc(tmp[i][j], fp3);
        }
	}
	fclose(fp3);
*/
    if(!sendToRobot(points, nlen/2, anglecorrection))
    {
        printf("ERROR");
    }
    deallocate_dynamic_matrix(slika->matrix, slika->row);
    free(slika);
    deallocate_dynamic_matrix(nslika, nrows);
    free(path);
    free(npathh);
    free(points->angles);
    free(points->linesegcords);
    free(points);
    deallocate_dynamic_matrix(tmp, nrows);
    free(npath);
}

