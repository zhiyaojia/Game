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


class Count(object):
	def __init__(self):
		self.image = [pygame.image.load("image/" + str(v) + ".png") \
						for v in range(5, -1, -1)]
		
		self.index = 0
		
		self.timer = Timer(1000)
		
		self.visible = False
 
	def action(self):
		if not self.visible:
			return
 
		if self.timer.right_time():
			self.index += 1
			self.timer.reset()

		if self.index == len(self.image):
			self.visible = False
 
	def draw(self, screen):
		if not self.visible:
			return
		screen.blit(self.image[self.index], (30, 552))

	def reset(self):
		self.index = 0
		self.timer.reset()
		self.visible = False


class temp(Phone):
	def __init__(self):
		super().__init__()

		self.font = pygame.font.Font('font/汉仪超级战甲.ttf', 25)
		self.right_up = self.font.render('0%', True, (234, 50, 50))
		self.right_down = self.font.render('1%', True, (234, 50, 50))
		self.left_up = self.font.render('100', True, (234, 50, 50))
		self.left_down = self.font.render('50', False, (234, 50, 50))
		self.clock = pygame.time.Clock()
		self.background = pygame.image.load('level/level_1/background.png')

		self.information = Information((234, 50, 50))

		self.map = Map(self.background)

		self.bomb = Bomb()

		self.pause_on = pygame.image.load('image/pause_on.png')

		self.assist = pygame.image.load('image/assist.png')

		self.num = pygame.image.load('image/0.png')

		self.count = Count()

	def print(self):
		x=26
		y=570
		pygame.key.set_repeat(1,50)

		self.bomb.set_pos(100, 100)

		while True:
			self.clock.tick(60)
			for event in pygame.event.get():
				if event.type ==QUIT:
					pygame.quit()
					exit()
			
				if event.type == KEYDOWN:
					if event.key == K_q:
						exit()
					if event.key == K_w:
						y -= 2
					if event.key == K_s:
						y += 2
					if event.key == K_a:
						x -= 2
					if event.key == K_d:
						x += 2
					if event.key == K_j:
						print('x: %d, y: %d'%(x, y))
					if event.key == K_k:
						self.count.reset()
						self.count.visible = True
						

			m_x,m_y = pygame.mouse.get_pos()
			pygame.display.set_caption(str(m_x) + "," + str(m_y))

			self.screen.fill((255, 255, 255))
			self.screen.blit(self.phone, (0, 0))
			
			self.map.action()
			self.map.draw(self.screen)
			self.count.action()

			self.screen.blit(self.pause_on, (26, 650))
			self.screen.blit(self.assist, (28, 605))
			self.count.draw(self.screen)

			self.screen.blit(self.phone2, (0, 0))
			pygame.display.update()


if __name__ == "__main__":
	pygame.init()
	x = temp()
	x.print()




















