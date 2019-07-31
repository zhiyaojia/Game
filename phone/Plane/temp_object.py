# coding:utf-8
import pygame
from pygame import *

from helper import *

import sys
sys.path.append("..")

from utility import *

class Object(Phone):
	def __init__(self, image, x, y, x_speed, y_speed, life, power):
		super().__init__()

		self.image = image

		self.x = x
		self.y = y

		self.life = life

		self.x_speed = x_speed
		self.y_speed = y_speed

		self.power = power

		self.mask = pygame.mask.from_surface(self.image)

	def action(self):
		self.x += self.x_speed
		self.y += self.y_speed


	def out_screen(self):
		if position(22, 372, 78, 693, self.x, self.y):
			return True
		else:
			return False


	def draw(self):
		self.screen.blit(self.image, (self.x, self.y))


	def set_speed(self, x_speed, y_speed):
		self.x_speed = x_speed
		self.y_speed = y_speed


	def whether_alive(self, life_change):
		if self.out_screen() is True:
			return False

		self.life += life_change
		if self.life <= =:
			return False

		return True



class Plane(Object):
	def __init__(self, x, y, x_speed, y_speed, life, power, bullet):
		super().__init__()

		self.bullet = bullet

	def action(self):
		super().action()


	def set_bullet(self, bullet):
		self.bullet = bullet


	def fire(self, bullet_list):
		for b in self.bullet:
			bullet_list.append(b)



class Enemy(Plane):
	def __init__(self, x, y, x_speed, y_speed, life, power, bullet, trigger):
		super().__init__()

		self.trigger_type = trigger[0]
		self.trigger = trigger[1]

		self.dead = False

	def action(self, score, time, distance, enemy_list, life_change = 0):
		if self.dead:
			pass

		if self in enemy_list:
			if not super().update():
				self.dead = True
				enemy_list.remove(self)
		else:
			if self.trigger_type == 'Score' and self.trigger == score:
				enemy_list.append(self)
			elif self.trigger_type == 'Time' and self.trigger == time:
				enemy_list.append(self)
			elif self.trigger_type == 'Distance' and self.trigger == distance:
				enemy_list.append(self)

	def super_action(self, life_change = 0):
		super().action()


class SmartEnemy(Enemy):
	def __init__(self, x, y, x_speed, y_speed, life, power, bullet, trigger, path):
		super().__init__()

		self.path = path
		self.node = 0

	def action(self, score, time, distance, enemy_list, life_change = 0):
		if self.dead:
			pass

		x, y = self.path[self.node]
		if self.x == x and self.y == y:
			self.node += 1
			if self.node == len(path):
				self.node = 0

			x, y = self.path[self.node]
			self.x_speed = int((x - self.x)/60)
			self.y_speed = int((y - self.y)/60)

		elif (self.x <= x + 5 and self.x >= x - 5) and \
			(self.y <= y + 5 and self.y >= y - 5):
			self.x = x
			self.y = y 



class Hero(Object):
	def __init__(self, x, y, x_speed, y_speed, life, power, bullet):
		super().__init__()

		self.on_fire = False
		self.on_assist = False


	def action(self):
		super().action()


	def set_bullet(self, bullet):
		self.bullet = bullet


	def fire(self, bullet_list):
		for b in self.bullet:
			bullet_list.append(b)


	def input(self, event):
		keys = pygame.key.get_pressed()

		if event.key == pygame.K_A:
			self.x_speed = -5

			if keys[pygame.K_w]:
				self.y_speed = -5
			elif keys[pygame.K_s]:
				self.y_speed = 5
			else:
				self.y_speed = 0

			if keys[pygame.K_j] and self.on_fire is False:
				self.on_fire = True

			if keys[pygame.K_k]:
				self.on_assist = True


		elif event.key == pygame.K_W:
			self.y_speed = -5

			if keys[pygame.K_a]:
				self.x_speed = -5
			elif keys[pygame.K_d]:
				self.x_speed = 5
			else:
				self.x_speed = 0

			if keys[pygame.K_j] and self.on_fire is False:
				self.on_fire = True

			if keys[pygame.K_k]:
				self.on_assist = True


		elif event.key == pygame.K_S:
			self.y_speed = 5

			if keys[pygame.K_a]:
				self.x_speed = -5
			elif keys[pygame.K_d]:
				self.x_speed = 5
			else:
				self.x_speed = 0

			if keys[pygame.K_j] and self.on_fire is False:
				self.on_fire = True

			if keys[pygame.K_k]:
				self.on_assist = True


		elif event.key == pygame.K_D:
			self.x_speed = 5

			if keys[pygame.K_w]:
				self.y_speed = -5
			elif keys[pygame.K_s]:
				self.y_speed = 5
			else:
				self.y_speed = 0

			if keys[pygame.K_j] and self.on_fire is False:
				self.on_fire = True

			if keys[pygame.K_k]:
				self.on_assist = True


		elif event.key == pygame.K_J:
			if self.on_fire is False:
				self.on_fire = True

			if keys[pygame.K_a]:
				self.x_speed = -5
			elif keys[pygame.K_d]:
				self.x_speed = 5
			else:
				self.x_speed = 0

			if keys[pygame.K_w]:
				self.y_speed = -5
			elif keys[pygame.K_s]:
				self.y_speed = 5
			else:
				self.y_speed = 0


		elif event.key == pygame.K_K:
			self.on_assist = True

			if keys[pygame.K_a]:
				self.x_speed = -5
			elif keys[pygame.K_d]:
				self.x_speed = 5
			else:
				self.x_speed = 0

			if keys[pygame.K_w]:
				self.y_speed = -5
			elif keys[pygame.K_s]:
				self.y_speed = 5
			else:
				self.y_speed = 0
















