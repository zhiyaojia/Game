# coding:utf-8
import pygame
from pygame import *

from Plane.helper import *

from utility import *

class Object(object):

	def __init__(self, array):
		#image, x, y, x_speed, y_speed, life, power
		super().__init__()

		self.image = array[0]

		self.rect = self.image.get_rect()
		self.width = self.rect.width
		self.height = self.rect.height

		self.x = array[1]
		self.y = array[2]

		self.center = (self.x + int(self.width/2), self.y + int(self.height/2))

		self.true_life = array[5]
		self.life = array[5]

		self.x_speed = array[3]
		self.y_speed = array[4]

		self.power = array[6]

		self.mask = pygame.mask.from_surface(self.image)


	def action(self):
		self.x += self.x_speed
		self.y += self.y_speed


	def out_screen(self):
		if position(22 - self.width, 372, 88 - self.height, 784, self.x, self.y):
			return False
		else:
			return True


	def draw(self, screen):
		screen.blit(self.image, (self.x, self.y))


	def set_rect(self, x, y):
		self.x = x
		self.y = y


	def set_speed(self, x_speed, y_speed):
		self.x_speed = x_speed
		self.y_speed = y_speed


	def alive(self):
		if self.out_screen() is True:
			return False

		if self.life <= 0:
			return False

		return True


	def update_life(self, life_change):
		self.life += life_change

		if self.life <= 0:
			self.life = 0



class SmartObject(Object):
	def __init__(self, array):
		super().__init__(array)

		self.bullet = array[7]

		self.trigger = array[8]

		self.path = array[9]
		self.index = 0

		self.move_timer = Timer(array[11][0])
		self.static = True

		self.true_x_speed = self.x_speed
		self.true_y_speed = self.y_speed

		if self.path:
			self.x_speed = 0
			self.y_speed = 0

		self.fire_timer = Timer(array[11][1])
		self.fire_ready = True

		self.score = array[10]

	def reset(self):
		self.index = 0
		self.x_speed = 0
		self.y_speed = 0

		self.move_timer.reset()
		self.fire_timer.reset()


	def action(self):
		super().action()


	def out_screen(self):
		if position(0, 394, 9, 784, self.x, self.y):
			return False
		else:
			return True


	def whether_trigger(self, score, time, distance):
		if self.trigger[0] == 'Score':
			if self.trigger[1] == score and \
				(self.trigger[2] <= time or self.trigger[3] <= distance):
				trigger = True
			else:
				trigger = False

		if self.trigger[0] == 'Time':
			if self.trigger[2] == time or \
				self.trigger[1] <= score or self.trigger[3] <= distance:
				trigger = True
			else:
				trigger = False

		if self.trigger[0] == 'Distance':
			if self.trigger[3] == distance and \
				(self.trigger[1] <= score or self.trigger[2] <= time):
				trigger = True
			else:
				trigger = False

		if trigger:
			self.fire_timer.reset()
			self.move_timer.reset()
		return trigger


	def update_speed(self):
		if not self.path:  
			return

		if self.static is True:
			if self.move_timer.right_time() is True:
				self.static = False
			
				x, y = self.path[self.index]
				if self.x > x:
					self.x_speed = -self.true_x_speed
				elif self.x < x:
					self.x_speed = self.true_x_speed
				else:
					self.x_speed = 0

				if self.y > y:
					self.y_speed = -self.true_y_speed
				elif self.y < y:
					self.y_speed = self.true_y_speed
				else:
					self.y_speed = 0

			else:
				self.x_speed, self.y_speed = (0, 0)

		else:
			x, y = self.path[self.index]
			if (self.x == x and self.y == y):
				self.static = True
				self.x_speed, self.y_speed = (0, 0)

				self.move_timer.reset() 

				self.index += 1
				if self.index == len(self.path):
					self.index = 0


	def fire(self, enemy_bullet):
		if not self.bullet:
			return

		if self.fire_ready:
			self.fire_ready = False

			if len(self.bullet) == 1:
				width = self.bullet[0][0].get_rect().width
				self.bullet[0][1] = self.x + (self.width/2) - (width/2) 
				self.bullet[0][2] = self.y
				enemy_bullet.append(Object(self.bullet[0]))


			if len(self.bullet) == 2:
				width = self.bullet[0][0].get_rect().width

				self.bullet[0][1] = self.x + (self.width/2) - width  
				self.bullet[0][2] = self.y
				enemy_bullet.append(Object(self.bullet[0]))

				self.bullet[1][1] = self.x + (self.width/2) 
				self.bullet[1][2] = self.y
				enemy_bullet.append(Object(self.bullet[1]))


			if len(self.bullet) == 3:
				width = self.bullet[1][0].get_rect().width
				center = self.x + (self.width/2) - (width/2)

				self.bullet[0][1] = center - width
				self.bullet[0][2] = self.y
				enemy_bullet.append(Object(self.bullet[0]))

				self.bullet[1][1] = center
				self.bullet[1][2] = self.y
				enemy_bullet.append(Object(self.bullet[1]))

				self.bullet[2][1] = center + width - 5
				self.bullet[2][2] = self.y
				enemy_bullet.append(Object(self.bullet[2]))

			self.fire_timer.reset()



class Hero(Object):
	def __init__(self, array):
		super().__init__(array)

		self.bullet = array[7]

		self.assist_bullet = array[8]

		self.left_fire = False
		self.straight_fire = False
		self.right_fire = False

		self.on_assist = False
		self.assist_ready = True

		self.timer = Timer(10000)

		self.shield_image = pygame.image.load('Plane/image/shield.png')
		self.shield_on = False
		self.shield_width = self.shield_image.get_rect().width
		self.shield_height = self.shield_image.get_rect().height


	def draw(self, screen):
		super().draw(screen)
		if self.shield_on:
			row = self.x + int(self.width/2) - int(self.shield_width/2)
			col = self.y + int(self.height/2) - int(self.shield_height/2)

			screen.blit(self.shield_image, (row, col))


	def action(self):
		super().action()

		if self.x > 372 - self.width:
			self.x = 372 - self.width
		if self.x < 22:
			self.x = 22

		if self.y < 88:
			self.y = 88
		if self.y > 693 - self.height:
			self.y = 693 - self.height


	def out_screen(self, x, y):
		if position(22, 372 - self.width, 88, 693 - self.height, self.x + x, self.y + y):
			return False
		else:
			return True


	def whether_fire(self):
		if self.straight_fire is True or self.left_fire is True or self.right_fire is True:
			return True
		else:
			return False


	def set_bullet(self, bullet):
		self.bullet = bullet


	def fire(self, hero_bullet):

		if hero_bullet and hero_bullet[len(hero_bullet)-1].y - self.y > -150:
			return 
		if self.whether_fire() is True:

			if len(self.bullet) == 1:

				if self.left_fire is True:
					self.bullet[0][1] = self.x 
					self.bullet[0][2] = self.y
					hero_bullet.append(Object(self.bullet[0]))

				if self.straight_fire is True:
					width = self.bullet[0][0].get_rect().width
					self.bullet[0][1] = self.x + (self.width/2) - (width/2) 
					self.bullet[0][2] = self.y
					hero_bullet.append(Object(self.bullet[0]))

				if self.right_fire is True:
					width = self.bullet[0][0].get_rect().width
					self.bullet[0][1] = self.x + self.width - width
					self.bullet[0][2] = self.y
					hero_bullet.append(Object(self.bullet[0]))


			if len(self.bullet) == 2:
				width = self.bullet[0][0].get_rect().width

				self.bullet[0][1] = self.x + (self.width/2) - width  
				self.bullet[0][2] = self.y
				hero_bullet.append(Object(self.bullet[0]))

				self.bullet[1][1] = self.x + (self.width/2) 
				self.bullet[1][2] = self.y
				hero_bullet.append(Object(self.bullet[1]))


			if len(self.bullet) == 3:
				width = self.bullet[1][0].get_rect().width
				center = self.x + (self.width/2) - (width/2)

				self.bullet[0][1] = center - width
				self.bullet[0][2] = self.y
				hero_bullet.append(Object(self.bullet[0]))

				self.bullet[1][1] = center
				self.bullet[1][2] = self.y
				hero_bullet.append(Object(self.bullet[1]))

				self.bullet[2][1] = center + width - 5
				self.bullet[2][2] = self.y
				hero_bullet.append(Object(self.bullet[2]))


	def assist(self, bullet_list):
		if self.assist_ready is True:
			for b in self.assist_bullet:
				bullet = Object(b)
				bullet_list.append(bullet)
			self.timer.reset()
			self.assist_ready = False


	def update_life(self, life_change):
		super().update_life(life_change)


	def alive(self):
		if self.life <= 0:
			return False
		return True


	def set_x_speed(self, x_speed):
		if self.x + x_speed >= 22 and self.x + x_speed <= 372 - self.width:
			self.x_speed = x_speed
		else:
			self.x_speed = 0

	def set_y_speed(self, y_speed):
		if self.y + y_speed >= 88 and self.y + y_speed <= 693 - self.height:
			self.y_speed = y_speed
		else:
			self.y_speed = 0


	def reset_speed(self, x, y):
		self.x = x
		self.y = y
		self.x_speed = 0
		self.y_speed = 0


	def reset_function(self):
		self.left_fire = False
		self.straight_fire = False
		self.right_fire = False

		self.on_assist = False


	def reset(self):
		self.reset_speed(142, 549)
		self.reset_function()
		self.life = 100
		self.timer.reset()


	def input(self, event):
		keys = pygame.key.get_pressed()

		if event.key == K_a:

			if keys[K_j]:
				self.left_fire = True

			elif keys[K_k]:
				self.on_assist = True

			else:
				self.set_x_speed(-8)

				if keys[K_w]:
					self.set_y_speed(-12)
				elif keys[K_s]:
					self.set_y_speed(12)
				


		elif event.key == K_w:

			self.set_y_speed(-12)

			if keys[K_a]:
				self.set_x_speed(-8)
			elif keys[K_d]:
				self.set_x_speed(8)
			


			if keys[K_j]:
				self.straight_fire = True

			elif keys[K_k]:
				self.on_assist = True

				

		elif event.key == K_s:

			self.set_y_speed(12)

			if keys[K_a]:
				self.set_x_speed(-8)
			elif keys[K_d]:
				self.set_x_speed(8)
			

			if keys[K_j]:
				self.straight_fire = True

			elif keys[K_k]:
				self.on_assist = True

				


		elif event.key == K_d:

			if keys[K_j]:
				self.right_fire = True

			elif keys[K_k]:
				self.on_assist = True

			else:
				self.set_x_speed(8)

				if keys[K_w]:
					self.set_y_speed(-12)
				elif keys[K_s]:
					self.set_y_speed(12)
				


		elif event.key == K_j:
			self.straight_fire = True

			if keys[K_a]:
				self.set_x_speed(-8)
			elif keys[K_d]:
				self.set_x_speed(8)
			else:
				self.x_speed = 0

			if keys[K_w]:
				self.set_y_speed(-12)
			elif keys[K_s]:
				self.set_y_speed(12)
			else:
				self.y_speed = 0


		elif event.key == K_k:
			self.on_assist = True

			if keys[K_a]:
				self.set_x_speed(-8)
			elif keys[K_d]:
				self.set_x_speed(8)
			else:
				self.x_speed = 0

			if keys[K_w]:
				self.set_y_speed(-12)
			elif keys[K_s]:
				self.set_y_speed(12)
			else:
				self.y_speed = 0



class Bomb(object):
	# 初始化爆炸
	def __init__(self):
		# 加载爆炸资源
		self.image = [pygame.image.load("Plane/image/bomb-" + str(v) + ".png") \
						for v in range(1, 5)]
		# 设置当前爆炸播放索引
		self.index = 0
		# 图片爆炸播放间隔
		self.interval = 3
		self.interval_index = 0
		# 爆炸位置
		self.position = [0, 0]
		# 是否可见
		self.visible = False
 
	# 设置爆炸播放的位置
	def set_pos(self, x, y):
		self.position[0] = x
		self.position[1] = y
 
	# 爆炸播放
	def action(self):
		# 如果爆炸对象状态不可见，则不计算坐标
		if not self.visible:
			return
 
		# 控制每一帧图片的播放间隔
		self.interval_index += 1
		if self.interval_index < self.interval:
			return
		self.interval_index = 0
 
		self.index = self.index + 1
		if self.index >= len(self.image):
			self.index = 0
			self.visible = False
 
	# 绘制爆炸
	def draw(self, screen):
		# 如果对象不可见，则不绘制
		if not self.visible:
			return
		screen.blit(self.image[self.index], \
			(self.position[0], self.position[1]))


	def reset(self):
		self.index = 0
		self.interval = 3
		self.interval_index = 0
		self.position = [0, 0]
		self.visible = False




class Count(object):
	def __init__(self):
		self.image = [pygame.image.load("Plane/image/" + str(v) + ".png") \
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





