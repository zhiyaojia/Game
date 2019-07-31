#!/usr/bin/python
# -*- coding: UTF-8 -*-
import time as Time
  
import pygame
from pygame import *

from Plane.plane import *
from utility import *


white = (255, 255, 255)
black = (0, 0, 0)

class iPhone(object):
	def __init__(self):
		pygame.init()
		pygame.mixer.init()

		self.screen = pygame.display.set_mode((394,784))
		pygame.display.set_caption("贾智尧的iPhone6 plus")

		self.m_x, self.m_y = pygame.mouse.get_pos()

		self.font1 = pygame.font.Font(None, 20)
		self.time = Time.strftime("%H:%M", Time.localtime())
		self.time_text = self.font1.render(self.time, True, black)

		self.not_start = True

		self.phone = pygame.image.load('image/phone.jpg').convert_alpha()
		self.phone2 = pygame.image.load('image/phone2.png').convert_alpha()
		self.phone3 = pygame.image.load('image/phone3.png').convert_alpha()
		self.background = pygame.image.load('image/back.png')

		self.font = pygame.font.Font('HuaWenXiHei-1.ttf', 12)
		self.font.set_bold(True)
		self.plane_name = self.font.render('飞机大战', True, black)
		self.plane_icon = pygame.image.load('image/plane_icon.png')

		self.clock = pygame.time.Clock()


	def draw(self):
		self.screen.fill(white)
		self.screen.blit(self.phone, (0, 0))
		self.screen.blit(self.background, (22, 78))
		
		self.screen.blit(self.time_text, (180, 95))

		self.screen.blit(self.plane_icon, (43, 124))
		self.screen.blit(self.plane_name, (53, 198))

		self.screen.blit(self.phone2, (0, 0))


	def update_time(self):
		self.time = Time.strftime("%H:%M", Time.localtime())
		self.time_text = self.font1.render(self.time, True, black)


	def wait(self, second):
		timer_1 = Timer(second)
		timer_1.reset()
		while True:
			for event in pygame.event.get():
				if event.type ==QUIT:
					pygame.quit()
					exit()
			if timer_1.right_time():
				break
			
			self.screen.blit(self.phone3, (0, 0))
			pygame.display.update()


	def run(self):
		while self.not_start:
			self.clock.tick(60)

			pygame.display.set_caption("贾智尧的iPhone6 plus")

			self.update_time()

			self.m_x, self.m_y = pygame.mouse.get_pos()

			for event in pygame.event.get():
				if event.type ==QUIT:
					pygame.quit()
					exit()
				if event.type ==  MOUSEBUTTONDOWN:
					if pygame.mouse.get_pressed() == (1, 0, 0):
						if position(170, 221, 710, 757, self.m_x, self.m_y):
							self.wait(2000)
							exit()

						if position(44, 111, 113, 182, self.m_x, self.m_y):
							self.wait(1000)
							self.plane = Planegame()
							self.plane.run()

				if event.type == KEYDOWN:
					if event.key == K_q:
						exit()
					if event.key == K_p:
						print(self.m_x, self.m_y)

			self.draw()
			pygame.display.update()



if __name__ == "__main__":
	phone = iPhone()
	phone.run()




















