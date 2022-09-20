import math
import random
import matplotlib.pyplot as plt


# params: 生成随机数的阈值
def RandomGenerator(fMin, fMax):
    return fMin + random.random() * (fMax - fMin)


# 待优化函数 取(x - 1) * (x - 1) + (y - 2) * (y - 2) + (z - 3) * (z - 3)
def objectfunction(x, y, z):
    return pow(x - 1, 2) + pow(y - 2, 2) + pow(z - 3, 2)


# 取邻近解
def Neighbor(x, fMin, fMax):
    return x + RandomGenerator(fMin, fMax)


class Simple_SA:
    def __init__(self, params: int):
        self._params = params
        # 生成 (-30, 30)间的随机值
        self._x = RandomGenerator(-30, 30)
        self._y = RandomGenerator(-30, 30)
        self._z = RandomGenerator(-30, 30)
        self._optimal = objectfunction(self._x, self._y, self._z)
        self._loss_list = [self._optimal]
        # 初始 T 以及T的衰减速率
        self._T = 1e3
        self._decay = 0.98
        # 保留迭代次数的列表
        self._iterations = [0]

    # 有时即使函数值没下降也会考虑接受
    def accept(self, nextval):
        p = -(nextval - self._optimal)
        p /= self._T
        return math.exp(p)

    def optimize(self):
        cnt = 0
        # 第一重循环，frozen，即temperature足够低了
        while self._T > 1e-5:
            x, y, z = self._x, self._y, self._z
            # 第二重循环用function evaluation来控制，实际上应该是在当前T下函数值应该稳定
            i = 0
            while i <= 30:
                # 内层循环都是从当前最优解四处找邻近解，以达到当前T下的一个稳定状态！
                cnt += 1
                nextx, nexty, nextz = Neighbor(x, -0.5, 0.5), Neighbor(y, -0.5, 0.5), Neighbor(z, -0.5, 0.5)
                nextval = objectfunction(nextx, nexty, nextz)
                # 这个接不接受的条件也是有自由度的，或许可以把概率阈值调更大些，即不要轻易放弃目前最优解，也有可能得调低些
                if nextval < self._optimal or self.accept(nextval) > 0.5:
                    self._optimal = nextval
                    self._loss_list.append(nextval)
                    self._iterations.append(i)
                    self._x, self._y, self._z = nextx, nexty, nextz
                    # 符合条件进来了，就全部修改一遍
                i += 1
            self._T *= self._decay

    def showResult(self):
        print("Expect Solution is x: %d, y: %d, z: %d\nExpected Minimum Value is %d" % (1, 2, 3, 0))
        print("My Result is x: %f, y: %f, z: %f\nMy Minimum Value is %f" % (self._x, self._y, self._z, self._optimal))
        print("Iteration nums: %d" % len(self._iterations))
        # finally plot the process
        """
        plt.plot(self._iterations, self._loss_list)
        plt.show()
        """


if __name__ == "__main__":
    sa_model = Simple_SA(3)
    sa_model.optimize()
    sa_model.showResult()
