### PA0 报告

#### 代码逻辑：

##### 1、直线：

（1）主要使用Bresenham算法：对于从左下指向右上，而且0 <= k < 1的线段，记录一个误差累积项（通分、减去偏置后的e）

 - e >=0 时，则y+=1, x+=1，更新e
 - e < 0 时，则x+=1，更新e

（2）为了统一，先视情况交换$x_1$和$x_2$（$y_1$和$y_2$），保证$x_1 < x_2$

（3）注意用对称思想处理其他k的情况

 - 对于k>1的线段，则先沿着y=x对称后，按上述逻辑求得每个(x, y)，再逐点按y=x对称以画最终的图
 - 对于k<0的线段（即$x_1 < x_2$但$y_1 > y_2$）先关于$y=y_1$对称，转化为上述k>=0情况，求得(x, y)，再做一次对称以画图

（4）注意特殊判断k不存在的情况

##### 2、圆：

（1）主要用中点画圆法，即使用“判别式”$F(x_p +1, y_p-0.5) = (x_p + 1)^2 + (y_p - 0.5)^2 - R^2$

 - 若d >= 0，则下一个点应该判断$F(x_p + 2, y_p - 1.5)$ ，y-=1，x+=1
 - 若d < 0，则下一个点应该判断$F(x_p + 2, y_p - 0.5)$ ，x+=1

（2）利用对称性，只用计算八分之一圆即可（代码中set_circle_pixel函数即完成所有对称点的绘制）

##### 3、区域填充：

用四联通方法，结合队列的数据结构：

 - 先将初始点涂色、入队
 - 每次取出队首元素，判断其上、下、左、右是否有符合要求的像素（像素在合法位置、且颜色为填充之前的颜色old_color），将其涂为新color，入队
 - 一直循环，直到队空

#### 代码参考：

1、和万子豪同学讨论：

- 遇到的问题：区域缝合不严，导致区域填充时队长度非常大，绘图错误
- 得到的帮助：考虑0 < k < 1以外的情况
- 我的思考：借助对称性以解决0 < k < 1以外的情况，最后代码通过测试

2、未参考网上代码

3、参考了教材、习题课代码、思路

#### 其它问题和思考：

1、如果old_color==color则跳过填充直接return，减少不必要的计算

2、用栈、队列实现时，刚入队/栈要立刻染色，否则再次轮到这个点时，会重复入队/栈，导致超时、MLE

3、对比stack与queue
- （1）stack因为“刚刚探测到的可染色点”会很快接受考察，因此容易形成“向一个方向拓展，难以在比较近的区域内迂回”（DFS形式的扩展），于是入栈的最大长度会比queue更大，空间更容易爆炸（但也说明，重复考察的点会少一些）
- （2）queue容易“在同一个地区内迂回”，是BFS形式的扩展，因此重复考察的点多，但“队的最大长度”更小
