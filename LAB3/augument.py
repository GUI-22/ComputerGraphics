import os
import random
import numpy as np
from PIL import Image, ImageEnhance, ImageFilter
import torch
from torchvision import transforms
import torchvision.transforms.functional as F

def augment_image(image):
    original_size = image.size  # 获取原始大小 (width, height)
    transform = []
    label = []
    if random.randint(0, 9) % 2 == 0:
        transform.append(transforms.RandomHorizontalFlip(p=1))
        label.append("HorizontalFlipped")
    if random.randint(0, 11) % 4 == 0:
        transform.append(transforms.RandomVerticalFlip(p=1))
        label.append("VerticalFlipped")
    if random.randint(0, 9) % 2 == 0:
        transform.append(transforms.RandomRotation(degrees=(10, 10)))
        transform.append(transforms.CenterCrop((int(original_size[1] * 0.85), int(original_size[0] * 0.85))))  # 裁剪中间
        transform.append(transforms.Resize(original_size))       # 放大到原有大小
        label.append("Rotated")
    if random.randint(0, 9) % 2 == 0:
        transform.append(transforms.RandomResizedCrop(size=(image.size[1], image.size[0]), scale=(0.8, 1.0)))
        label.append("ResizedCropped")
    transform += [transforms.ToTensor(),transforms.ToPILImage()]
    transform = transforms.Compose(transform)
    augmented_image = transform(image)
    label = "_".join(label)
    return augmented_image, label



def augument_image_in_folder(input_dir, output_dir, folder_num):
    # 输入和输出目录
    input_base_dir = input_dir + str(folder_num)
    output_base_dir = output_dir + str(folder_num)

    # 创建输出目录（如果不存在）
    os.makedirs(output_base_dir, exist_ok=True)

    # 遍历输入目录中的所有子目录和图像
    for root, _, files in os.walk(input_base_dir):
        for file in files:
            if file.endswith('.png'):
                input_image_path = os.path.join(root, file)
                relative_path = os.path.relpath(input_image_path, input_base_dir)
                output_dir = os.path.join(output_base_dir, os.path.dirname(relative_path))
                os.makedirs(output_dir, exist_ok=True)



                image = Image.open(input_image_path).convert('RGB')

                base_filename = os.path.splitext(file)[0]

                num_augmented_images = 10

                for i in range(num_augmented_images):
                    augmented_image, label = augment_image(image)
                    output_image_path = os.path.join(output_dir, f'{base_filename}_{i+1}_{label}.png')
                    augmented_image.save(output_image_path)
                    print(f'Saved {output_image_path}')


#指定原始图像的文件夹
input_dir = 'A_style_origin/'
#指定输出图像的文件夹
output_dir = 'A_style_augument/'

for i in range(15):
    if i < 10:
        folder_num = f"0{i}"
    else:
        folder_num = str(i)
    augument_image_in_folder(input_dir, output_dir,folder_num=folder_num)