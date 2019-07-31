import pygame

from pygame.locals import *

from Plane.object import *
from Plane.helper import *

import os

from utility import *


# 英雄
left_image = pygame.image.load('Plane/image/bullet_left.png')
single_image = pygame.image.load('Plane/image/bullet_straight.png')
right_image = pygame.image.load('Plane/image/bullet_right.png')
triple_left = pygame.image.load('Plane/image/triple_left.png')
triple_right = pygame.image.load('Plane/image/triple_right.png')


left_fire = [[left_image, 0, 0, -15, -20, 100, -20]]
single_fire = [[single_image, 0, 0, 0, -25, 100, -20]]
right_fire = [[right_image, 0, 0, 15, -20, 100, -20]]
double_fire = [[single_image, 0, 0, 0, -25, 100, -20], \
				[single_image, 0, 0, 0, -25, 100, -20]]
triple_fire = [[triple_left, 0, 0, -10, -20, 100, -20],\
				[single_image, 0, 0, 0, -25, 100, -20],\
				[triple_right, 0, 0, 10, -20, 100, -20]]


assist_1 = [[single_image, x, 700, 0, -25, 100, -20] for x in range(40, 390, 50)]
assist_2 = [[single_image, x, 800, 0, -25, 100, -20] for x in range(40, 390, 50)]
assist = assist_1 + assist_2


hero_image = pygame.image.load('Plane/image/hero.png')
hero_info = [hero_image, 142, 549, 0, 0, 100, -100, single_fire, assist]
hero_bullet = []


# 敌人
down_bullet_image_1 = pygame.image.load('Plane/image/down_bullet.png')
down_left_bullet_image_1 = pygame.image.load('Plane/image/triple_left_down.png')
down_right_bullet_image_1 = pygame.image.load('Plane/image/triple_right_down.png')
down_bullet_image_2 = pygame.image.load('Plane/image/bullet_04.png')


down_fire_1 = [[down_bullet_image_1, 0, 0, 0, 25, 100, -20]]
down_fire_2 = [[down_bullet_image_2, 0, 0, 0, 25, 100, -30]]
down_double_fire = [[down_bullet_image_1, 0, 0, 0, 25, 100, -20],
					[down_bullet_image_1, 0, 0, 0, 25, 100, -20]]
down_triple_fire =[[down_left_bullet_image_1, 0, 0, -10, 20, 100, -10],
				[down_bullet_image_1, 0, 0, 0, 25, 100, -10],
				[down_right_bullet_image_1, 0, 0, 10, 20, 100, -10]]

enemy_bullet = []
enemy_list = []

boss_image = pygame.image.load('Plane/image/boss.png')
enemy_image_1 = pygame.image.load('Plane/image/img-plane_3.png')
enemy_image_2 = pygame.image.load('Plane/image/img-plane_7.png')
enemy_image_3 = pygame.image.load('Plane/image/img-plane_1.png')

#image, x, y, x_speed, y_speed, life, power, bullet, trigger, path, score, fire_lag

enemy_info =\
[
	[enemy_image_1, 22, 26, 8, 8, 100, -20, down_fire_1, ('Score', 0, 2, 0), 
	[[22, 114], [22, 354], [270, 354], [270, 114]], 5, (2000, 1000)],

	[enemy_image_1, 20, 24, 10, 10, 100, -20, down_fire_1, ('Score', 5, 0, 5), 
	[[20, 114], [270, 114], [20, 364], [270, 364]], 5, (2000, 800)],

	[enemy_image_2, 80, 24, 5, 10, 100, -20, down_fire_1, ('Score', 10, 2, 10), 
	[[80, 114], [20, 114], [20, 234], [80, 234]], 5, (3000, 800)],

	[enemy_image_2, 210, 24, 5, 10, 100, -20, down_fire_1, ('Score', 10, 2, 10), 
	[[210, 114], [270, 114], [270, 234], [210, 234]], 5, (3000, 800)],

	[enemy_image_3, 70, 24, 0, 3, 100, -20, down_double_fire, ('Score', 20, 2, 10), 
	[[70, 114], [70, 1114]], 10, (2000, 800)],

	[enemy_image_3, 220, 24, 0, 3, 100, -20, down_double_fire, ('Score', 20, 2, 10), 
	[[220, 114], [220, 1114]], 10, (2000, 800)],

	[enemy_image_3, 145, 24, 10, 10, 100, -20, down_triple_fire, ('Distance', 0, 2, 40),
	[[145, 314], [45, 314], [45, 114], [245, 114], [245, 314]], 10, (1500, 800)],

	[boss_image, 140, 12, 10, 10, 500, -20, down_fire_2, ('Distance', 0, 2, 50), 
	[[140, 112], [70, 112], [20, 112], [260, 112], [200, 112]], 10, (4000, 1500)]
]


# 其他

box_shield = pygame.image.load('Plane/image/box_shield.png')
box_double = pygame.image.load('Plane/image/box_double.png')
box_triple = pygame.image.load('Plane/image/box_triple.png')
box_life = pygame.image.load('Plane/image/box_life.png')

box_info =\
[
	[box_shield, 242, 26, 0, 5, 100, 0, ['box_shield'], ('Time', 999, 8, 999), 
	[], 5, (2000, 1000)],
	[box_double, 142, 26, 0, 5, 100, 0, ['box_double'], ('Time', 20, 30, 999), 
	[], 5, (2000, 1000)],
	[box_shield, 242, 26, 0, 5, 100, 0, ['box_shield'], ('Time', 30, 50, 999), 
	[], 5, (2000, 1000)]
]
box_list = []

award = pygame.image.load('Plane/image/hero2.png')
win_text = ['中士', award]




class Level_1(Phone):
	def __init__(self, x):
		super().__init__()

		# 数字资源
		self.background = pygame.image.load('Plane/level/level_'+str(x)+'/background.png')
		self.gray_background = pygame.image.load('Plane/level/level_'+str(x)+'/gray.png')
		self.pause_on = pygame.image.load('Plane/image/pause_on.png')
		self.pause_off = pygame.image.load('Plane/image/pause_off.png')
		self.assist_image = pygame.image.load('Plane/image/assist.png')
		self.assist_gray = pygame.image.load('Plane/image/assist_gray.png')
		self.bomb = pygame.mixer.Sound('Plane/image/bomb.wav')
		self.bomb.set_volume(0.3)
		self.background_music = pygame.mixer.Sound('Plane/image/bg.wav')
		self.background_music.set_volume(0.2)
		self.hit = pygame.mixer.Sound('Plane/image/hit.wav')
		self.hit.set_volume(0.3)
		self.gain = pygame.mixer.Sound('Plane/image/gain.wav')
		self.gain.set_volume(0.5)
	
		# 成员
		self.map = Map(self.background)
		self.hero = Hero(hero_info)
		self.timer_2000 = Timer(2000)
		self.caption = Caption('Level 1', 60, 305)
		self.information = Information((234, 50, 50))
		self.bombs = [Bomb() for _ in range(5)]
		self.all_enemy = []
		self.all_box = []
		self.count_shield = Count()

		# 游戏数据
		self.clock = pygame.time.Clock()
		self.score = 0
		self.distance = 0
		self.start_time = int(pygame.time.get_ticks()/1000)
		self.curr_time = int(pygame.time.get_ticks()/1000)
		self.enemy_life = 100
		self.straight_fire = single_fire

		# Boolean 数据
		self.game_on = True
		self.win = False
		self.lose = False
		self.pause = False
		self.quit = False

		# 字体
		self.font2 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 20)
		self.start = self.font2.render('开始游戏', True, white)
		self.start_rect = self.start.get_rect()

		self.font3 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 30)
		self.start_on = self.font3.render('开始游戏', True, black, white)


	def reset(self):
		self.timer_2000.reset()
		self.count_shield.reset()
		self.hero.reset()
		self.hero.set_bullet(single_fire)
		self.all_enemy.clear()
		for e in enemy_info:
			enemy = SmartObject(e)
			self.all_enemy.append(enemy)
		self.all_box.clear()
		for b in box_info:
			box = SmartObject(b)
			self.all_box.append(box)

		for b in self.bombs:
			b.reset()


		self.game_on = True
		self.win = False
		self.lose = False
		self.pause = False
		self.caption.on = True

		self.score = 0
		self.distance = 0
		self.start_time = int(pygame.time.get_ticks()/1000)
		self.curr_time = int(pygame.time.get_ticks()/1000)
		self.enemy_life = 100
		self.straight_fire = single_fire


	def peace(self):
		for b in self.bombs:
			if b.visible:
				return False
		return True


	def set_bomb(self, x, y):
		for b in self.bombs:
			if not b.visible:
				b.set_pos(x, y)
				b.visible = True
				return


	def run(self):
		pygame.key.set_repeat(1,50)

		pygame.mixer.Channel(1).play(self.background_music)

		while not self.quit:
			self.reset()

			num = 0  # over game
			while self.game_on:
				self.clock.tick(60)
				self.curr_time = int(pygame.time.get_ticks()/1000)

			# action
				if not self.pause:
					self.map.action()

					self.hero.action()

					for b in hero_bullet:
						b.action()

					for e in enemy_list:
						e.action()

					for b in enemy_bullet:
						b.action()

					for bomb in self.bombs:
						if bomb.visible:
							bomb.action()

					for box in box_list:
						box.action()

					self.count_shield.action()


			# collision
					for b in hero_bullet:
						for e in enemy_list:
							offsetX = int(b.x - e.x)
							offsetY = int(b.y - e.y)
							point = e.mask.overlap(b.mask,(offsetX,offsetY))

							if point:
								if position(10, 392, 68, 693, e.x, e.y):
									pygame.mixer.Channel(2).play(self.hit)
									e.update_life(b.power)
									hero_bullet.remove(b)
									self.enemy_life = int((e.life/e.true_life)*100)

					for b in enemy_bullet:
						offsetX = int(b.x - self.hero.x)
						offsetY = int(b.y - self.hero.y)
						point = self.hero.mask.overlap(b.mask,(offsetX,offsetY))

						if point:
							if not self.count_shield.visible:
								pygame.mixer.Channel(2).play(self.hit)
								self.hero.update_life(b.power)
							enemy_bullet.remove(b)


					for e in enemy_list:
						offsetX = int(e.x - self.hero.x)
						offsetY = int(e.y - self.hero.y)
						point = self.hero.mask.overlap(e.mask,(offsetX,offsetY))

						if point:
							self.hero.update_life(e.power)
							e.update_life(self.hero.power)


					for box in box_list:
						offsetX = int(box.x - self.hero.x)
						offsetY = int(box.y - self.hero.y)
						point = self.hero.mask.overlap(box.mask,(offsetX,offsetY))

						if point:
							pygame.mixer.Channel(3).play(self.gain)
							if box.bullet == ['box_shield']:
								self.count_shield.reset()
								self.count_shield.visible = True
							if box.bullet == ['box_double']:
								self.straight_fire = double_fire
							if box.bullet == ['box_triple']:
								self.straight_fire = triple_fire
							if box.bullet == ['box_life']:
								self.hero.life += 50
							box_list.remove(box)



			# event
				# 操作
				m_x,m_y = pygame.mouse.get_pos()

				self.hero.reset_function()
				for event in pygame.event.get():
					
					if event.type ==QUIT:
						pygame.quit()
						exit()
				
					if event.type ==  MOUSEBUTTONDOWN:
						m_x, m_y = pygame.mouse.get_pos()
						if pygame.mouse.get_pressed() == (1, 0, 0):
							if position(170, 221, 710, 757, m_x, m_y):
								return False

							if position(27, 59, 660, 695, m_x, m_y):
								if self.pause:
									self.pause = False
								else:
									self.pause = True

					if event.type == KEYDOWN:
						if event.key == K_q:
							exit()
						if event.key == K_p:
							if self.pause:
								self.pause = False
							else:
								self.pause = True

						self.hero.input(event)

					else:
						self.hero.reset_speed(self.hero.x, self.hero.y)

				
				# 输赢
				if not self.all_enemy and not enemy_list:
					self.win = True

				if not self.hero.alive():
					pygame.mixer.Channel(2).play(self.bomb)
					self.set_bomb(self.hero.x, self.hero.y)
					self.lose = True

				if (self.win is True or self.lose is True):
					num += 1
					if num == 40:
						self.game_on = False
					   

				# 英雄
				for b in hero_bullet:
					if not b.alive():
						hero_bullet.remove(b)


				if len(hero_bullet) <= len(self.hero.bullet) * 2:
					if self.hero.whether_fire() is True:

						if self.hero.left_fire is True:
							self.hero.set_bullet(left_fire)

						if self.hero.straight_fire is True:
							self.hero.set_bullet(self.straight_fire)

						if self.hero.right_fire is True:
							self.hero.set_bullet(right_fire)
						 
						self.hero.fire(hero_bullet)

				if not self.hero.assist_ready:
					if self.hero.timer.right_time():
						self.hero.assist_ready = True

				if self.hero.on_assist:
					self.hero.assist(hero_bullet)

				self.hero.shield_on = self.count_shield.visible

				# 敌人
				for e in self.all_enemy:
					if e in enemy_list:
						continue
					if e.whether_trigger(self.score, self.curr_time - self.start_time,\
										self.distance):
						enemy_list.append(e)
						self.all_enemy.remove(e)


				for e in enemy_list:
					if not e.alive():
						if e.life <= 0:
							self.score += e.score
						enemy_list.remove(e)
						self.distance += e.score
						self.set_bomb(e.x, e.y)
						pygame.mixer.Channel(2).play(self.bomb)
						continue

					e.update_speed()

					if e.fire_timer.right_time():
						e.fire_ready = True
					if e.x_speed == 0 or (e.x_speed != 0 and e.y_speed != 0):
						if position(18, 380, 68, 693, e.x, e.y):
							e.fire(enemy_bullet)

				for b in enemy_bullet:
					if not b.alive():
						enemy_bullet.remove(b)


				# Caption
				if self.timer_2000.right_time():
					self.caption.on = False

				if len(self.all_enemy) == 1 and not enemy_list:
					self.caption.update('Boss', 100, 305)
					self.timer_2000.update_time(3500)
					self.timer_2000.reset()
					self.caption.on = True

				# Box
				for box in self.all_box:
					if box in box_list:
						continue
					if box.whether_trigger(self.score, self.curr_time - self.start_time,\
										self.distance):
						box_list.append(box)
						self.all_box.remove(box)


			# draw
				self.screen.fill((255, 255, 255))
				self.screen.blit(self.phone, (0, 0))

				
				self.map.draw(self.screen)

				for b in hero_bullet:
					b.draw(self.screen)

				for b in enemy_bullet:
					b.draw(self.screen)

				for e in enemy_list:
					e.draw(self.screen)

				for box in box_list:
					box.draw(self.screen)

				if self.hero.alive():
					self.hero.draw(self.screen)

				for bomb in self.bombs:
					if bomb.visible:
						bomb.draw(self.screen)

				if self.caption.on:
					self.caption.draw(self.screen)

				self.information.print(self.hero.life, self.enemy_life, self.score)

				if self.hero.assist_ready:
					self.screen.blit(self.assist_image, (28, 605))
				else:
					self.screen.blit(self.assist_gray, (28, 605))

				if self.pause:
					self.screen.blit(self.pause_off, (26, 650))
				else:
					self.screen.blit(self.pause_on, (26, 650))

				self.count_shield.draw(self.screen)


				self.screen.blit(self.phone2, (0, 0))


				pygame.display.update()
				


		# 游戏结束
			enemy_list.clear()
			hero_bullet.clear()
			enemy_bullet.clear()
			box_list.clear()
			self.all_enemy.clear()
			self.all_box.clear()

			loop = True

			if self.win:
				font = pygame.font.Font('HuaWenXiHei-1.ttf', 18)
				line1 = font.render('获得军衔: '+win_text[0], True, white)
				line2 = font.render('获得奖励:', True, white)

				font2 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 30)
				start = font2.render('继续游戏', True, white)
				start_rect = start.get_rect()

				font3 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 40)
				start_on = font3.render('继续游戏', True, white, (234, 50, 50))


				icon = pygame.image.load('Plane/image/win.png')
				conversation = pygame.image.load('Plane/image/conversation.png')


				while loop:
					self.clock.tick(60)

					for event in pygame.event.get():
						if event.type ==QUIT:
							pygame.quit()
							exit()

						if event.type == KEYDOWN:
							if event.key == K_q:
								exit()

						if event.type ==  MOUSEBUTTONDOWN:
							m_x, m_y = pygame.mouse.get_pos()
							if pygame.mouse.get_pressed() == (1, 0, 0):
								if position(170, 221, 710, 757, m_x, m_y):
									return False
								if position(135, 254, 613, 648, m_x, m_y):
									loop = False
									self.quit = True

				
					m_x, m_y = pygame.mouse.get_pos()

					self.screen.fill((255, 255, 255))
					self.screen.blit(self.phone, (0, 0))
					self.screen.blit(self.background, (22, 88))
					
					self.screen.blit(icon, (95, 160))

					self.screen.blit(conversation, (-1, 373))
					self.screen.blit(line1, (91, 397))
					self.screen.blit(line2, (91, 429))
					self.screen.blit(win_text[1], (195, 437))

					if position(135, 254, 613, 648, m_x, m_y):
						self.screen.blit(start_on, (117, 603))
					else:
						self.screen.blit(start, (135, 613))

					self.information.print(self.hero.life, 0, self.score)


					self.screen.blit(self.phone2, (0, 0))
					pygame.display.update()
					

			if self.lose:
				icon = pygame.image.load('Plane/image/lose.png')

				font2 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 30)
				start = font2.render('重新开始', True, white)
				start_rect = start.get_rect()

				font3 = pygame.font.Font('Plane/font/汉仪超级战甲.ttf', 40)
				start_on = font3.render('重新开始', True, white, (234, 50, 50))


				while loop:
					self.clock.tick(60)

					for event in pygame.event.get():
						if event.type ==QUIT:
							pygame.quit()
							exit()
					
						if event.type ==  MOUSEBUTTONDOWN:
							m_x, m_y = pygame.mouse.get_pos()
							if pygame.mouse.get_pressed() == (1, 0, 0):
								if position(170, 221, 710, 757, m_x, m_y):
									return False
								if position(137, 256, 403, 437, m_x, m_y):
									loop = False
								

						if event.type == KEYDOWN:
							if event.key == K_q:
								exit()


					self.screen.fill((255, 255, 255))
					self.screen.blit(self.phone, (0, 0))
					self.screen.blit(self.gray_background, (22, 88))
					
					self.screen.blit(icon, (106, 234))

					m_x, m_y = pygame.mouse.get_pos()
					if position(137, 256, 403, 437, m_x, m_y):
						self.screen.blit(start_on, (117, 393))
					else:
						self.screen.blit(start, (137, 403))


					self.information.print(0, self.enemy_life, self.score)

					self.screen.blit(self.phone2, (0, 0))
					pygame.display.update()

		return True	


		





