import pygame

from pygame.locals import *
from sys import exit


class GameBackground(object):
    # 初始化地图
    def __init__(self, scene):
        # 加载相同或者不同两种张图片资源,做交替实现地图滚动
        self.image1 = pygame.image.load("iamge/img_bg_level_5.jpg").convert()
        self.image2 = pygame.image.load("image/img_bg_level_5.jpg").convert()
        # 保存场景对象
        self.main_scene = scene
        # 辅助移动地图
       	self.y1 = 0
        self.y2 = -self.main_scene.get_size()[1]
 
    # 计算地图图片绘制坐标
    def action(self):
        self.y1 = self.y1 + 1
        self.y2 = self.y2 + 1
        #第一张图超出屏幕了，就接到第二张图的后面，以此类推。
        if self.y1 >= self.main_scene.get_size()[1]:
            self.y1 = 0
        if self.y2 >= self.main_scene.get_size()[1]:
            self.y2 = -self.main_scene.get_size()[1]
 
    # 绘制地图的两张图片
    def draw(self):
        self.main_scene.blit(self.image1, (0, self.y1))
        self.main_scene.blit(self.image2, (0, self.y2))



class Bullet(pygame.sprite.Sprite):
	def __init__(self, image_path, speed=5):      
		super().__init__()
		
		self.image = pygame.image.load(image_path).convert()
		
		self.rect = self.image.get_rect()
		
		self.speed = speed
 
	def update(self, *args):
		
		self.rect.y += self.speed







































