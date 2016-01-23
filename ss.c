#include <stdio.h>

int A[3][3] = {
    0,1,0,
    0,1,1,
    0,0,0
};
int B[3][3] = {
    0,0,0,
    1,1,0,
    0,1,0
};

float getNum (int x, int y, float m_x, float m_y) {
    float a = x-m_x;
    float b = y-m_y;
    return a*b;
}

float calI2 (int A[][3], int size, int num) {

    int i=0, j=0;
    float total;
    float avl;
    float i2;
    float m_x;     // x重心
    float m_y;     // y重心

    // 获取重心(m_x, m_y)
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++) {

            if (A[i][j] == 1) {
               // total = total + getNum (i, j);
                m_x = m_x + i;
                m_y = m_y + j;
            }
        }
    }
    m_x = m_x / num;
    m_y = m_y / num;

    printf ("重心:(%lf,%lf)\n", m_x, m_y);

    // 总权重
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++) {

            if (A[i][j] == 1) {
               total = total + getNum (i, j, m_x, m_y);
            }
        }
    }

    // 平均值
    avl = total / num;
    printf ("avl:%lf\n", avl);

    // 获取方差
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++) {

            if (1 == A[i][j]) {
               i2 = i2 + (getNum (i,j, m_x, m_y) + avl)*(getNum (i, j, m_x, m_y) + avl);
            }
        }
    }

    return i2;
}

int main () {

    printf ("A:%lf\n", calI2(A, 3, 3));
    printf ("B:%lf\n", calI2(B, 3, 3));
    return 0;
}
