import os
import random
import numpy as np
from PIL import Image
import torch
from torchvision import transforms



# 输入和输出目录
input_base_dir = 'A_style_origin/00'
output_base_dir = 'A_style_augument/00'

# 创建输出目录（如果不存在）
os.makedirs(output_base_dir, exist_ok=True)

# 遍历输入目录中的所有子目录和图像
for root, _, files in os.walk(input_base_dir):
    for file in files:
        if file.endswith('.png'):
            input_image_path = os.path.join(root, file)
            relative_path = os.path.relpath(input_image_path, input_base_dir)
            print("rel", relative_path)
            output_dir = os.path.join(output_base_dir, os.path.dirname(relative_path))
            print(output_dir)
            base_filename = os.path.splitext(file)[0]
            print(os.path.splitext(file)[0])
            os.makedirs(output_dir, exist_ok=True)


