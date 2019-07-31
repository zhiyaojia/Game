import pygame
import numpy

from pygame import *


def add(x):
	x = 3


def grayscale(img):
    arr = pygame.surfarray.array3d(img)
    #luminosity filter
    avgs = [[(r*0.298 + g*0.587 + b*0.114) for (r,g,b) in col] for col in arr]
    arr = numpy.array([[[avg,avg,avg] for avg in col] for col in avgs])
    return pygame.surfarray.make_surface(arr)

if __name__ == "__main__":
	'''
	image = pygame.image.load('image/assist.jpg')
	image = grayscale(image)
	pygame.image.save(image, 'assist_gray.jpg')
	'''

	x = 2
	print(x)
	add(x)
	print(x)
	









