#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define N 3
//#define TEST

char buf[N+1][N+1] = {0};
int step;

int x_min,x_max, y_min,y_max;

int num;
int bufsize;	//the crtct_rec count
/*
* shape's characteristic struct
* contains both variance an convariance
*/
typedef struct chart {
	float var_x;
	float var_y;	//variance
	float cov;	//convariance
}CHARTC;

/*
* struct 
*/
typedef struct point{
	int x;
	int y;
}POINT;

static POINT record[64] = {0};
CHARTC charater;
CHARTC *crtc_rec;


void do_print();
int checklegal(int x, int y);
void checksame();
void putblock(int x_min, int y_min, int x_max, int y_max);
void calchart(POINT *buf, int size);
float do_sqr(float x, float x_avr);
inline float do_cr(float x, float x_avr, float y, float y_avr);
CHARTC* get_chartc(POINT *buf, CHARTC *crt, int count);
int issame(CHARTC crt, CHARTC *rec);
void add_crtc(CHARTC crt);

/*
* put a block in area from (s_x, s_y) to (e_x, e_y)
*/
void putblock(int x_min, int y_min, int x_max, int y_max)
{
	int i, j;

	if(step == 0){
		i = j = (N>>1) + N%2;
		//printf("(%d,%d)\n", i, j);
		buf[i][j] = 1;
		record[step].x = i;
		record[step].y = j;
		step++;
		putblock(i, j, i, j);		
		step --;
		buf[i][j] = 0;
	}
	else if(step < N){
		for(i = x_min - 1; i <= x_max + 1; i++){
			for(j = y_min-1; j <= y_max+1; j++){
				if(checklegal(i,j)){
					buf[i][j] = 1;
					record[step].x = i;
					record[step].y = j;
					step++;
					x_min = (i < x_min) ? i : x_min;
					x_max = (i > x_max) ? i : x_max;
					y_min = (j < y_min) ? j : y_min;
					y_max = (j > y_max) ? j : y_max;

					putblock(x_min, y_min, x_max, y_max);	

					buf[i][j] = 0;
					step--;
				}	
			}
		}
	}
	else{
		CHARTC temp;
	#ifdef TEST
		printf("%d\n", ++num);
		calchart(record, N);
		do_print();	
		return;
	#endif
		get_chartc(record, &temp, N);
		if(!issame(temp, crtc_rec)){
			add_crtc(temp);
			printf("%d\n", num);
			do_print();
		}
			
	}
	
}

/*
*
*/
int issame(CHARTC crt, CHARTC *rec)
{
	int i;
	for(i = 1; i <= num; i++){
		//printf("var:%lf,%lf,cov:%lf\n",rec[i].var_x, rec[i].var_y, rec[i].cov);
		if((rec[i].var_x == crt.var_x && rec[i].var_y == crt.var_y)\
		|| (rec[i].var_x == crt.var_y && rec[i].var_y == crt.var_x)){
			//printf("variance match\n");
			if(rec[i].cov == crt.cov || rec[i].cov == -crt.cov){ 
				return 1;
			}	
		}
	}
	return 0;
}

/*
*
*/
void add_crtc(CHARTC crtc)
{
	num++;
	if(num > bufsize - 1){
		if((crtc_rec = (CHARTC *)realloc(crtc_rec, (sizeof(CHARTC)) * (bufsize + 4096))) == NULL){
			printf("realloc err\n");
			return;
		}
		bufsize += sizeof(CHARTC) * 4096;
	}
	crtc_rec[num].var_x = crtc.var_x;
	crtc_rec[num].var_y = crtc.var_y;
	crtc_rec[num].cov = crtc.cov;
}
/*
* check if point (x,y) is legal
*/
int checklegal(int x, int y)
{
	int i;
	if(buf[x][y] || x < 1 || x > N || y < 1 || y > N)
		return 0;
	for(i = 0; i < step; i++){
		if((abs(x-record[i].x) <=1) && (abs(y-record[i].y) <= 1))
			return 1;
	}
	return 0;
}

/*
* printf the shape
*/
void do_print()
{
	int i,j;
	for(i = 1; i <= N; i++){
		for(j = 1; j <= N; j++){
			if(buf[i][j])
				printf("*  ");
			else
				printf(".  ");
		}
		
		printf("\n");
	}
}

/*
* caculate the shape characteristic
*/
void calchart(POINT *buf, int size)
{
	int i;
	float x_total = 0.0, y_total = 0.0;
	float x_avr = 0.0;
	float y_avr = 0.0;
	float cov = 0.0;
	for(i = 0; i < size; i++){
		//printf("(%d,%d)\n", buf[i].x, buf[i].y);
		x_total += buf[i].x * size;
		y_total += buf[i].y * size;		
	}
	x_avr = x_total/size;
	y_avr = y_total/size;
	printf("average: (%lf, %lf)\n", x_avr, y_avr);
	
	for(i = 0; i < size; i++){
		//printf("(%d, %d)\n", buf[i].x, buf[i].y);
		cov += do_cr(buf[i].x * size, x_avr, buf[i].y * size, y_avr);
		//printf("point: (%d,%d) cov: %lf\n", buf[i].x, buf[i].y, cov);
		
	}
	printf("协方差: %lf\n", cov/N);

	x_total = y_total = 0;
	for(i = 0; i < size; i++){
		x_total += do_sqr(buf[i].x * size, x_avr);
		y_total += do_sqr(buf[i].y * size, y_avr);
	}
	x_avr = x_total/size;
	y_avr = y_total/size;
	printf("方差:(%lf,%lf)\n", x_avr, y_avr);

	
}

inline float do_cr(float x, float x_avr, float y, float y_avr)
{
	float temp;
	float t, t1;
	t = x-x_avr;
	t1 = y-y_avr;
	temp = t * t1/*((x-x_avr) * (y-y_avr))*/;
	//printf("temp = %lf, t:%lf t2:%lf\n", temp, t, t1);
	return temp; 
}

/*
*计算放置点集的方差和协方差
*由方差和协方差两个特征能唯一确定图形的摆放情况
*上下左右翻转造成协方差正负对调
*图形旋转一个90度造成X,Y坐标方差相互对调
*/
CHARTC* get_chartc(POINT *buf, CHARTC *crt, int size)
{
	int i;
	float x_total = 0.0, y_total = 0.0;
	float x_avr = 0.0;
	float y_avr = 0.0;
	float cov = 0.0;
	for(i = 0; i < size; i++){
		//printf("(%d,%d)\n", buf[i].x, buf[i].y);
		x_total += buf[i].x * size;		//坐标点乘以size已使得到的方差和协方差为整数
		y_total += buf[i].y * size;		//防止结果为小数时因为精度截断导致相同的结果产生误差,下同	
	}
	x_avr= x_total/size;
	y_avr = y_total/size;
	//printf("average: (%lf, %lf)\n", x_avr, y_avr);
	
	for(i = 0; i < size; i++){
		//printf("(%d, %d)\n", buf[i].x, buf[i].y);
		cov += do_cr(buf[i].x * size, x_avr, buf[i].y * size, y_avr);
		//printf("point: (%d,%d) cov: %lf\n", buf[i].x, buf[i].y, cov);
		
	}
	crt->cov = cov/size;
	//printf("协方差: %lf\n", crt->cov);

	x_total = y_total = 0;
	for(i = 0; i < size; i++){
		x_total += do_sqr(buf[i].x * size, x_avr);
		y_total += do_sqr(buf[i].y * size, y_avr);
	}
	crt->var_x = x_total/size;
	crt->var_y = y_total/size;
	//printf("方差:(%lf,%lf)\n", crt->var_x, crt->var_y);
	
	return crt;
}


float do_sqr(float x, float x_avr)
{
	float temp;
	temp = x - x_avr;
	return (temp*temp);
}
int main()
{
	step = 0;
	crtc_rec = (CHARTC *)malloc(sizeof(CHARTC)* 4096);
	if(crtc_rec == NULL){
		printf("malloc error\n");
		return 0;
	}
	putblock(1,1,1,1);
	printf("total match num:%d\n", num);
	return 1;
}
