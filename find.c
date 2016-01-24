#include <stdio.h>

#define N 3

extern struct point;

char buf[N+1][N+1] = {0};
/*
*struct of point set
*/
typedef struct point_set {
	POINT *set;		//a complete shape's point set whit unique
	CHARTC crtc;		//shape's charateristic
	POINT_SET *next;	//next point set
}POINT_SET;

void find(POINT_SET *pre, POINT_SET *next, int size);
inline int chartc_compare(CHARTC res, CHARTC dst);
int isexist(CHARTC crt, POINT_SET *pst);
void add_set(CHARTC crtc, POINT *buf, POINT_SET *set);
/*
*find next unique n+1 point sets base on previous n point sets
*/
void find(POINT_SET *pre, POINT_SET *next, int size, int N)
{
	int i, j, k;
	POINT_SET *cur;
	POINT_SET *tmp;
	if(pre == NULL || next == NULL){
		return;	
	}
	if(size < N){
		for(cur = pre; cur != NULL; cur = cur->next){
			memset(buf, 0, (N+1)*(N+1));
			for(i = 0; i < size; i++){
				buf[cur->set[i].x][cur->set[i].y] = 1;
			}
			for(i = 0; i < size; i++){
				for(j = cur->set[i].x-1; j <= cur->set[i].x+1, j++){
					for(k = cur->set[i].y-1; k <= cur->set[i].y+1; k++){
						if(islegal(j, k, cur->set, size)){
							POINT *temp;
							CHARTC crtc;
							buf[j][k] = 1;
							cur->set[size].x = j;
							cur->set[size].y = k;
							//temp = (POINT *)malloc(sizeof(POINT) * size+1);
							get_chartc(cur->set, &crtc, size+1);
							if(!isexist(crtc, next)){
								add_set(crtc, cur->set, next);
							}	
							buf[j][k] = 0;
						}
					}
				}
			}
		}	
	POINT_SET ps;		
	ps.set = NULL;
	ps.next = NULL;
	find(next, &set, size+1);
	}
	else{
		do_printf();
	}
}

/*
*
*/
inline int islegal(int x, int y, POINT *set, int size)
{
	int i;
	if(buf[x][y] || x < 1 || y < 1 || x > N || y > N)
		return 0;
	for(i = 0; i < size; i++){
		if((abs(x-set[i].x)) <= 1 && (abs(y-set[i].y) <= 1)){
			return 1;
		}	
	}
	return 0;
}
/*
*
*/
int isexist(CHARTC crt, POINT_SET *pst)
{
	POINT_SET *ptmp;
	for(ptmp = pst; ptmp != NULL; ptmp = ptmp->next){
		if(crtc_compare(crt, ptmp->crtc)){
			return 1;
		}
	}
	return 0;
}
/*
* check if the two shape is same
*/
inline int chartc_compare(CHARTC res, CHARTC dst)
{
	
	if((res.var_x == dst.var_x && res.var_y == dst.var_y)\
	|| (res.var_x == dst.var_y && res.var_y == dst.var_x)){
		//printf("variance match\n");
		if(res.cov == dst.cov || res.cov == -dst.cov){ 
			return 1;
		}	
	}
	else
		return 0;
}

