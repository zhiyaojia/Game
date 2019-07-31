import pygame

from pygame.locals import *
from sys import exit
from item import *


black = (0,0,0)
white = (255,255,255)
green = (0,255,0)
red = (255,0,0)


# 主场景
class MainScene(object):
	# 初始化主场景
	def __init__(self):
		pygame.init()
		# 场景尺寸
		self.screen_size = (512, 768)
		# 场景对象
		self.screen = pygame.display.set_mode(self.screen_size, 0, 32)
		# 设置标题
		pygame.display.set_caption("飞机大战-v1.0")
		
		self.map = GameBackground(self.screen)
 	
 	
	# 绘制
	def draw_elements(self):
		self.map.draw()
		pygame.draw.circle(self.screen, green, (300, 300),10)
 

	# 动作
	def action_elements(self):
		self.map.action()
		pass
 

	# 处理事件
	def handle_event(self):
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()
 

	# 碰撞检测
	def detect_conlision(self):
		pass
 

	# 主循环,主要处理各种事件
	def run_scene(self):
		while True:
			# 计算元素坐标
			self.action_elements()
			# 绘制元素图片
			self.draw_elements()
			# 处理事件
			self.handle_event()
			# 碰撞检测
			self.detect_conlision()
			# 刷新显示
			pygame.display.update()


# 入口函数
if __name__ == "__main__":
	# 创建主场景
	mainScene = MainScene()
	# 开始游戏
	mainScene.run_scene()
	




















