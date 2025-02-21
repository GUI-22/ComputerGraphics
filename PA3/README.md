### CGAN_jittor README

#### 1、项目简介

- 该项目为用jittor语言实现Conditional GAN模型。具体而言，模型运用数字图片数据集MNIST，训练一个将随机噪声和类别标签映射为数字图片的CGAN模型，最终可以生成一串指定数字的手写图样。

#### 2、环境配置

- 主要参考https://cg.cs.tsinghua.edu.cn/jittor/download/网站
- 笔者在Ubuntu系统下，使用GPU训练，此种情况可以如下方式配置环境

```bash
sudo apt install python3.7-dev libomp-dev
python3.7 -m pip install jittor
python3.7 -m jittor.test.test_example
# 如果您电脑包含Nvidia显卡，检查cudnn加速库
python3.7 -m jittor.test.test_cudnn_op
```

- 如果测试运行通过，恭喜你已经安装完成

#### 3、代码运行

- 在代码中可以指定number为目标生成的数字序列
- 在CGAN.py所在目录下，运行以下代码即可

```bash
python CGAN.py
```

- 可以在命令行指定参数，形如下

```bash
python CGAN.py --n_epochs 100 --batch_size 64
```

- 各参数含义：

  - n_epochs：number of epochs of training
  - batch_size：size of the batches
  - lr: adam learning rate (default: 0.0002)
  - b1: adam decay of first order momentum of gradient (default: 0.5)
  - b2: adam decay of first order momentum of gradient (default: 0.999)
  - n_cpu: number of CPU threads to use during batch generation (default: 8)
  - latent_dim: dimensionality of the latent space (default: 100)
  - n_classes: number of classes for the dataset (default: 10)
  - img_size: size of each image dimension (default: 32)
  - channels: number of image channels (default: 1)
  - sample_interval: interval between image sampling (default: 1000)
- 最终生成结果在result.png中
