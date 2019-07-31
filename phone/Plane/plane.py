# coding:utf-8
import pygame
from pygame import *

from Plane.menu import *
from Plane.guide import *
from Plane.object import *
from Plane.helper import *
from Plane.level_1 import *


class Planegame(object):
	def __init__(self):
		pygame.init()
		self.menu = Menu()
		self.guide = Guide()
		self.name = ''
		self.level_1 = Level_1(1)


	def run(self):
		if not self.menu.run():
			return

		guide = self.guide.run()
		if not guide[0]:
			return
		self.name = guide[1]

		if not self.level_1.run():
			return


















