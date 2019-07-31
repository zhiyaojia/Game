# coding:utf-8
import pygame
from pygame import *

from Plane.object import *
from Plane.helper import *

from utility import *

white = (255, 255, 255)
black = (0, 0, 0)


class Guide(Phone):
	def __init__(self):
		super().__init__()
		self.name = ''

	def run(self):
		background = pygame.image.load('Plane/level/level_1/background.png')

		chat = pygame.image.load('Plane/image/chat1.png')
		girl = pygame.image.load('Plane/image/girl.png')


		font = pygame.font.Font('HuaWenXiHei-1.ttf', 18)
		textline = font.render('先给自己起个名字吧', True, white)


		box = InputBox(100, 570, 80, 32)
			
		done = False

		while not done:
			for event in pygame.event.get():
				self.name, done = box.handle_event(event)
				if event.type ==QUIT:
					pygame.quit()
					exit()
				
				if event.type ==  MOUSEBUTTONDOWN:
					m_x, m_y = pygame.mouse.get_pos()
					if pygame.mouse.get_pressed() == (1, 0, 0):
						if position(170, 221, 710, 757, m_x, m_y):
							return (False, '')


			m_x,m_y = pygame.mouse.get_pos()
			box.update()

			self.screen.fill((255, 255, 255))
			self.screen.blit(self.phone, (0, 0))
			self.screen.blit(background, (22, 88))

			self.screen.blit(girl, (30, 208))
			self.screen.blit(chat, (-5, 515))
			self.screen.blit(textline, (34, 530))
			box.draw(self.screen)

			self.screen.blit(self.phone2, (0, 0))
			pygame.display.update()


		done = False
		textline = [
				(self.name + '，欢迎你成为空军的列兵', 'None'),
				('接下来我会教你如何驾驶飞船', 'None'),
				('使用 W, S, A, D 来控制飞船移动', 'None'),
				('使用 J 来射击', 'None'),
				('使用 K 来呼叫炮火支援', 'None')
				]

		conversation = Conversation(textline, '1', 'Plane/image/girl.png')
		if not conversation.run():
			return (False, '')


		textline = [
				('记住， 炮火支援是有冷却时间的', 'None'),
				('使用 A+J 或 D+J 斜向射击', 'None'),
				('使用 P 暂停', 'None'),
				('战斗中收集补给箱可以获得特殊技能', 'None')
				]

		conversation = Conversation(textline, '1', 'Plane/image/girl2.png')
		if not conversation.run():
			return (False, '')


		textline = [
				('在战斗过程中你也会用到一些数据', 'None'),
				('右下角的是你的战机完整率', 'None'),
				('左下角的是你的弹药数', 'None'),
				('右上角的是敌机的完整率', 'None'),
				('左上角的是你的得分', 'None'),
				('接下来在模拟战斗中复习一下这些技巧吧', 'None'),
				('通关分数: 50 分', 'None')
				]

		conversation = Conversation(textline, '1', 'Plane/image/girl2.png')

		return (conversation.run(), self.name)




















