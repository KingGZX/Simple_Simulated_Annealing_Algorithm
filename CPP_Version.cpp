#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define frozen 1e-6

//   设计匆忙不是很完善，其中objectfunction，直接写进去就完事了
//  原本打算保留一个函数指针作为类成员的。。。
class Simple_SA{
private:
    double _T;                       //  as temperature
    vector<double> _Variables;       // 目标优化函数的自变量
    vector<double> _Neighbor;        // 采样邻近点作为solution
    int _num;                        // 参数数量
    double _optimal;
    double _decay;
public:
    Simple_SA(int n);
    double object_function(double x, double y, double z);       // 目标函数
    double Neiborhood();    // 对单点进行采样
    double accept(double val);                // 即：有时候即使目标函数值并没有降低，但仍有一定的概率会接受更新值
    void optimize();                // 退火算法的主题部分
    void showResult();              // 最后结果展示
};

double Simple_SA::object_function(double x, double y, double z){
    // 模拟一个 (x -1)(x - 1) + (y - 2)(y - 2) + (z - 3)(z - 3)
    return pow(x - 1, 2) + pow(y - 2, 2) + pow(z - 3, 2);
    // intuition : optimal is (1, 2, 3)
}

Simple_SA::Simple_SA(int n){
    _num = n;
    _Variables.resize(_num);
    _Neighbor.resize(_num);    
    // 初始的T和自变量参数都随机生成
    _T = 1e3;                    // 我记得老师上课一直说1000的啥的，直接定死不随机了
    srand(time(NULL));
    for(int i = 0 ; i < _num ; i ++){
        // 产生一个 -30 - 30之间的随机数
        double f = (double)rand() / RAND_MAX;       // 获得一个 0 ～ 1 之间的双精度浮点系数
        double var = -30.0 + f * 60.0;
        _Variables[i] = var;
    }
    // 初始optimal就是初始变量代入的值
    _optimal = object_function(_Variables[0], _Variables[1], _Variables[2]);
    cout << "initial setting is x: " << _Variables[0] << "    " << "y: " << _Variables[1] << "    " << "z: " << _Variables[2] << endl;
    cout << "initial optimal is :" << _optimal << endl;
    // 衰减系数，用于T的迭代更新
    _decay = 0.98;
}

// 采样邻近点
double Simple_SA::Neiborhood(){
    // 假设每次都采用(+- 0.5)邻域
    double f = (double)rand() / RAND_MAX;
    return -0.5 + f * 1.0;
}

//计算概率
double Simple_SA::accept(double val){
    double diff = val - _optimal;
    double p = diff / _T;
    return exp(-p);
}

void Simple_SA::optimize(){
    double x = _Variables[0];
    double y = _Variables[1];
    double z = _Variables[2];

    // 第一层循环停止条件是not yet frozen,frozen的含义是自己定义的，我这里就简单模仿原例，控制损失到一个精度内
    while(_T > frozen){
        // 第二层循环就是看看在当前T下是不是稳定了
        // 这里用function evaluation来替代，就是控制循环次数算了
        int i = 1;
        while(i < 40){
            // 根据当前的解进行邻近点采样
            double nextx = x + Neiborhood();
            double nexty = y + Neiborhood();
            double nextz = z + Neiborhood();
            double nextval = object_function(nextx, nexty, nextz);
            if(nextval < _optimal || accept(nextval) > 0.5){
                x = nextx, y = nexty, z = nextz;
                _optimal = nextval;
                _Variables[0] = nextx;
                _Variables[1] = nexty;
                _Variables[2] = nextz;
            }
            ++i;
        }
        // 此时的Variable存储了之前一个T的最优解了，所以之后的迭代从这里开始
        x = _Variables[0], y = _Variables[1],z = _Variables[2];
        _T *= _decay;
    }
}

void Simple_SA::showResult(){
    cout << "best solution is x: " << _Variables[0] << "    " << "y: " << _Variables[1] << "    " << "z: " << _Variables[2] << endl;
    cout << "current optimal value is: " << _optimal << endl;

    cout << "Expected Solution is x: " << 1 << "    " << "y: " << 2 << "    " << "z: " << 3 << endl;
    cout << "Expected optimal value is: " << 0 << endl; 
}

int main(){
    Simple_SA obj(3);
    obj.optimize();
    obj.showResult();
    return 0;
}