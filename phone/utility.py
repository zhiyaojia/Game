# coding:utf-8
import pygame
from pygame import *

pygame.init()

def position(x1, x2, y1, y2, x, y):
	if (x > x1 and x < x2) and (y > y1 and y < y2):
		return True
	else:
		return False


class Timer:
	def __init__(self, lag):
		self.time_lag = lag
		self.old_time = 0
		self.new_time = pygame.time.get_ticks()

	def right_time(self):
		self.new_time = pygame.time.get_ticks()
		time_diff = self.new_time - self.old_time
		if(time_diff <= self.time_lag + 100 and time_diff >= self.time_lag - 100):
			self.old_time = self.new_time
			return True
		else:
			return False

	def update_time(self, time):
		self.time_lag = time

	def reset(self):
		self.old_time = pygame.time.get_ticks()

	def print(self):
		self.new_time = pygame.time.get_ticks()
		time_diff = self.new_time - self.old_time
		print('time diff is: %d'%time_diff)
		


COLOR_INACTIVE = pygame.Color('lightskyblue3')
COLOR_ACTIVE = pygame.Color('dodgerblue2')
FONT = pygame.font.Font(None, 32)

class InputBox:

	def __init__(self, x, y, w = 140, h =32, text='', color = (255, 255, 255)):
		self.rect = pygame.Rect(x, y, w, h)
		self.color = COLOR_INACTIVE
		self.text = text
		self.txt_surface = FONT.render(text, True, color)
		self.active = False


	def handle_event(self, event):
		if event.type == pygame.MOUSEBUTTONDOWN:
			# If the user clicked on the input_box rect.
			if self.rect.collidepoint(event.pos):
				
				# Toggle the active variable.
				self.active = not self.active
			else:
				self.active = False
			# Change the current color of the input box.
			self.color = COLOR_ACTIVE if self.active else COLOR_INACTIVE
		if event.type == pygame.KEYDOWN:
			if self.active:
				if event.key == pygame.K_RETURN:
				   return self.text, True
					
				elif event.key == pygame.K_BACKSPACE:
					self.text = self.text[:-1]
				else:
					if len(self.text) < 15:
						self.text += event.unicode
				# Re-render the text.
				self.txt_surface = FONT.render(self.text, True, self.color)
		return '', False

	def update(self):
		# Resize the box if the text is too long.
		width = max(200, self.txt_surface.get_width()+10)
		self.rect.w = width

	def draw(self, screen):
		# Blit the text.
		screen.blit(self.txt_surface, (self.rect.x+5, self.rect.y+5))
		# Blit the rect.
		pygame.draw.rect(screen, self.color, self.rect, 2)



