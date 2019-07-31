# coding:utf-8
import pygame
from pygame import *

from utility import *

white = (255, 255, 255)
black = (0, 0, 0)


class Phone(object):
	def __init__(self):
		self.screen = pygame.display.set_mode((394,784))
		self.phone = pygame.image.load('image/phone.jpg')
		self.phone2 = pygame.image.load('image/phone2.png').convert_alpha()



class Map(Phone):
	def __init__(self, image):
		super().__init__()

		self.image1 = image
		self.image2 = image
  
		self.y1 = 88
		self.y2 = -517

		self.speed = 2
 
	# 计算地图图片绘制坐标
	def action(self):
		self.y1 = self.y1 + self.speed
		self.y2 = self.y2 + self.speed
		#第一张图超出屏幕了，就接到第二张图的后面，以此类推。
		if self.y1 >= 693:
			self.y1 = 88
		if self.y2 >= 88:
			self.y2 = -517
 
	def update_speed(self, speed):
		self.speed = speed

	# 绘制地图的两张图片
	def draw(self, screen):
		screen.blit(self.image1, (22, self.y1))
		screen.blit(self.image2, (22, self.y2))



class Textline(object):
	def __init__(self, text, girl, color = (255, 255, 255), \
		 chat=pygame.image.load('Plane/image/chat1.png')):

		self.chat = chat
		self.girl = girl

		self.text = text[0]
		self.font = pygame.font.Font('HuaWenXiHei-1.ttf', 18)
		self.line = self.font.render(text[0], True, color)
		self.font2 = pygame.font.Font('HuaWenXiHei-1.ttf', 15)
		self.space = self.font2.render('（按空格键继续）', True, color)
		self.color = color

		if text[1] == 'None':
			self.icon_show = False
		else:
			self.icon_show = True
			self.icon = pygame.image.load(text[1])


	def handle_event(self, event):
		if event.type == pygame.KEYDOWN:
			if event.key == pygame.K_SPACE:
				return True
			return False


	def set_text(self, text):
		self.text = text[0]
		self.line = self.font.render(self.text, True, self.color)

		if text[1] == 'None':
			self.icon_show = False
		else:
			self.icon_show = True
			self.icon = pygame.image.load(text[1])


	def set_color(self, color):
		self.color = color


	def set_girl(self, path):
		self.girl = pygame.image.load(path)


	def set_chat(self, path):
		self.chat = pygame.image.load(path)


	def draw(self, screen):
		screen.blit(self.girl, (30, 208))
		screen.blit(self.chat, (-5, 515))
		screen.blit(self.line, (34, 530))
		screen.blit(self.space, (128, 589))
		if self.icon_show:
			screen.blit(self.icon, (279, 536))




class Conversation(Phone):
	def __init__(self, material, num, girl, chat='Plane/image/chat1.png'):
		super().__init__()
		self.name = ''
		self.textline = material
		path = 'Plane/level/level_' + num + '/background.png'
		self.background =pygame.image.load(path)
		self.chat = pygame.image.load(chat)
		self.girl = pygame.image.load(girl)

	def run(self):
		done = False
		
		index = 0 
		text = Textline(self.textline[index], self.girl)

		while not done:
			for event in pygame.event.get():
				if text.handle_event(event):
					index += 1
					if index == len(self.textline):
						done = True
					else:
						text.set_text(self.textline[index])

				if event.type ==QUIT:
					pygame.quit()
					exit()
			
				if event.type ==  MOUSEBUTTONDOWN:
					m_x, m_y = pygame.mouse.get_pos()
					if pygame.mouse.get_pressed() == (1, 0, 0):
						if position(170, 221, 710, 757, m_x, m_y):
							return False

				if event.type == KEYDOWN:
					if event.key == K_k:
						print(m_x, m_y)
					if event.key == K_q:
						exit()


			m_x,m_y = pygame.mouse.get_pos()
			self.screen.fill((255, 255, 255))
			self.screen.blit(self.phone, (0, 0))
			self.screen.blit(self.background, (22, 88))

			text.draw(self.screen)

			self.screen.blit(self.phone2, (0, 0))

			pygame.display.update()

		return True



class Information(Phone):
	def __init__(self, color):
		super().__init__()

		self.font = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 25)
		self.color = color
	
	def print(self, life, enemy, score):
		life_text = str(life) + '%'
		enemy_text = str(enemy) + '%'
		self.right_up = self.font.render(enemy_text, True, self.color)
		self.right_down = self.font.render(life_text, True, self.color)
		self.left_up = self.font.render('%d'%score, True, self.color)

		if life == 100:
			life_x = 290
		elif life < 100 and life > 9:
			life_x = 308
		elif life < 10:
			life_x = 326

		if enemy == 100:
			enemy_x = 290
		elif enemy < 100 and enemy > 9:
			enemy_x = 308
		elif enemy < 10:
			enemy_x = 326


		self.screen.blit(self.right_up, (enemy_x, 88))
		self.screen.blit(self.right_down, (life_x, 663))
			
		self.screen.blit(self.left_up, (25, 88))
		


class Caption(object):
	def __init__(self, text, x, y):
		self.font = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 65)
		self.text = self.font.render(text, True, (234, 50, 50))
		self.on = True
		self.x = x
		self.y = y

	
	def update(self, text, x, y):
		self.text = self.font.render(text, True, (234, 50, 50))
		self.x = x
		self.y = y


	def draw(self, screen):
		screen.blit(self.text, (self.x, self.y))


def grayscale(img):
    arr = pygame.surfarray.array3d(img)
    #luminosity filter
    avgs = [[(r*0.298 + g*0.587 + b*0.114) for (r,g,b) in col] for col in arr]
    arr = numpy.array([[[avg,avg,avg] for avg in col] for col in avgs])
    return pygame.surfarray.make_surface(arr)















