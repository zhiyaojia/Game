import pygame 
from pygame import *

from object import *
from helper import *

pygame.init()

width = 100
height = 68
x_speed = 8
y_speed = 12

path = []

white = (255, 255, 255)
black = (0, 0, 0)


award = pygame.image.load('image/hero2.png')
win_text = ['中士', award]


class temp(Phone):
	def __init__(self):
		super().__init__()

		self.background = pygame.image.load('level/level_1/background.png')
		self.gray_background = pygame.image.load('level/level_1/gray.png')

		self.information = Information((234, 50, 50))

		self.map = Map(self.background)

		self.timer_2000 = Timer(2000)

		self.clock = pygame.time.Clock()


	def print(self):
		x=137
		y=393
		pygame.key.set_repeat(1,50)

		lose = True
		win = not lose

		loop = True
		if win:

			font = pygame.font.Font('../HuaWenXiHei-1.ttf', 18)
			line1 = font.render('获得军衔: '+win_text[0], True, white)
			line2 = font.render('获得奖励:', True, white)

			font2 = pygame.font.Font('font/汉仪超级战甲.ttf', 30)
			start = font2.render('继续游戏', True, white)
			start_rect = start.get_rect()

			font3 = pygame.font.Font('font/汉仪超级战甲.ttf', 40)
			start_on = font3.render('继续游戏', True, white, (234, 50, 50))


			icon = pygame.image.load('image/win.png')
			conversation = pygame.image.load('image/conversation.png')


			while loop:
				self.clock.tick(60)

				#if self.timer_2000.right_time():
					#loop = False

				for event in pygame.event.get():
					if event.type ==QUIT:
						pygame.quit()
						exit()

					if event.type == KEYDOWN:
						if event.key == K_q:
							exit()
			
				self.screen.fill((255, 255, 255))
				self.screen.blit(self.phone, (0, 0))
				self.screen.blit(self.background, (22, 88))
				
				self.screen.blit(icon, (95, 160))

				self.screen.blit(conversation, (-1, 373))
				self.screen.blit(line1, (91, 397))
				self.screen.blit(line2, (91, 429))
				self.screen.blit(win_text[1], (181, 437))

				if position(135, 254, 613, 648, m_x, m_y):
					self.screen.blit(start_on, (117, 603))
				else:
					self.screen.blit(start, (135, 613))

				self.information.print(99, 8, 199)


				self.screen.blit(self.phone2, (0, 0))
				pygame.display.update()

		if lose:
			icon = pygame.image.load('image/lose.png')

			font2 = pygame.font.Font('font/汉仪超级战甲.ttf', 30)
			start = font2.render('重新开始', True, white)
			start_rect = start.get_rect()

			font3 = pygame.font.Font('font/汉仪超级战甲.ttf', 40)
			start_on = font3.render('重新开始', True, white, (234, 50, 50))


			while True:
				self.clock.tick(60)

				for event in pygame.event.get():
					if event.type ==QUIT:
						pygame.quit()
						exit()
				
					if event.type ==  MOUSEBUTTONDOWN:
						m_x, m_y = pygame.mouse.get_pos()
						if pygame.mouse.get_pressed() == (1, 0, 0):
							if position(170, 221, 710, 757, m_x, m_y):
								exit()
							

					if event.type == KEYDOWN:
						if event.key == K_q:
							exit()


				self.screen.fill((255, 255, 255))
				self.screen.blit(self.phone, (0, 0))
				self.screen.blit(self.gray_background, (22, 88))
				
				self.screen.blit(icon, (106, 244))

				if position(137, 256, 403, 437, m_x, m_y):
					self.screen.blit(start_on, (117, 393))
				else:
					self.screen.blit(start, (137, 403))


				self.information.print(99, 8, 199)

				self.screen.blit(self.phone2, (0, 0))
				pygame.display.update()


if __name__ == "__main__":
	pygame.init()
	x = temp()
	x.print()




















