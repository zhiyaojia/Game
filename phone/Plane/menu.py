# coding:utf-8
import pygame
from pygame import *

from Plane.helper import *

from utility import *


class Menu(Phone):
	def __init__(self):
		pygame.init()
		super().__init__()

		pygame.display.set_caption("飞机大战")

		self.m_x, self.m_y = pygame.mouse.get_pos()

		self.not_start = True

		self.background = pygame.image.load('Plane/image/menu.png').convert_alpha()

		self.font1 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 65)
		self.caption = self.font1.render('飞机大战', True, white)

		self.font2 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 30)
		self.start = self.font2.render('开始游戏', True, white)
		self.start_rect = self.start.get_rect()

		self.font3 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 40)
		self.start_on = self.font3.render('开始游戏', True, black, white)


	def draw(self):
		self.screen.fill(white)
		self.screen.blit(self.phone, (0, 0))
		self.screen.blit(self.background, (22, 78))
		
		self.screen.blit(self.caption, (65, 150))
		
		if position(135, 254, 308, 341, self.m_x, self.m_y):
			self.screen.blit(self.start_on, (122, 303))
		else:		
			self.screen.blit(self.start, (135, 308))

		self.screen.blit(self.phone2, (0, 0))


	def run(self):
		while self.not_start:
			self.m_x, self.m_y = pygame.mouse.get_pos()

			for event in pygame.event.get():
				if event.type ==QUIT:
					pygame.quit()
					exit()
				if event.type ==  MOUSEBUTTONDOWN:
					if pygame.mouse.get_pressed() == (1, 0, 0):
						if position(170, 221, 710, 757, self.m_x, self.m_y):
							return False

						if position(135, 254, 308, 341, self.m_x, self.m_y):
							return True

				if event.type == KEYDOWN:
					if event.key == K_q:
						exit()
					if event.key == K_p:
						print(self.mx, self.my)

			self.draw()
			pygame.display.update()






















