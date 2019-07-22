# coding:utf-8
import pygame
import random
import time

from pygame.locals import *
from random import randint
from sys import exit

#################################################
'''
					游戏成员
'''
#################################################

class Enemy:
	id_ = 0
	row = 0
	col = 0
	cursor = ''
	def __init__(self, name, num):
		self.row = randint(-70, 890) 
		self.col = randint(-100, -50)
		self.cursor = pygame.image.load(name)
		self.id_ = num


class Monster(Enemy):
	life = 15
	exist = False
	name = ('graph/big_tony.png', 'graph/zihan.png')
	def __init__(self):
		pass
	def set_data(self):
		self.life = 20
		self.row = randint(110, 700) 
		self.col = randint(-70, -30)
		self.cursor = pygame.image.load(self.name[randint(0,1)])
		self.exist = False
	def create(self):
		num = randint(1, 15)
		if num == 1:
			self.exist = True


class Bomb:
	row = 0
	col = 0
	def __init__(self, x, y):
		self.row = x
		self.col = y


class Timer:
	old_time = 0
	new_time = 0
	time_lag = 0
	def __init__(self, lag):
		self.time_lag = lag
		self.old_time = 0
		self.new_time = pygame.time.get_ticks()/1000
	def right_time(self):
		self.new_time = int(pygame.time.get_ticks()/1000)
		if(self.new_time - self.old_time == self.time_lag):
			self.old_time = self.new_time
			return True
		else:
			return False


class Bag:
	row = 0
	col = 0
	exist = False
	image = ''
	def __init__(self):
		self.row = 10
		self.col = 112
		self.image = pygame.image.load('graph/bag.png')
	def setdata(self):
		self.row = randint(0, 300)
		self.col = randint(0, 300)
		self.exist = False


if __name__ == "__main__":

# 初始化pygame，为使用硬件做准备
	pygame.init()
	pygame.mixer.init()
	
	pygame.display.set_caption("阳阳の奇幻冒险♂")

# 声音资源
	hit = pygame.mixer.Sound('music/hit.wav')
	hit.set_volume(0.1)
	gain = pygame.mixer.Sound('music/gain.wav')
	gain.set_volume(0.5)
	weige_music = pygame.mixer.Sound('music/weige.wav')
	weige_music.set_volume(1)


	############################################################################################
	'''
												游戏前
	'''
	############################################################################################



# 初始化数据
	font = pygame.font.Font('font/字魂48号-半天云魅黑手书.ttf', 70)

	caption_surface = font.render('阳阳的奇幻大冒险', True, (255, 187, 179))
	small_option = font.render('开始冒险', True, (255, 187, 179))
	big_option = font.render('开始冒险', True, (255, 187, 179), (255, 255, 240))

	text = pygame.font.Font('font/PPETRIAL.ttf', 50)
	line1 = text.render('q  quit', True, (255, 187, 179))
	line2 = text.render('p  pause', True, (255, 187, 179))
	line3 = text.render('j  superman', True, (255, 187, 179))

	x = 980-caption_surface.get_width()

	start = False

	screen = pygame.display.set_mode((1000, 750), 0, 32)
	background_before = pygame.image.load('graph/back.jpg').convert()


# 游戏前循环
	while start == False:
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()

			if event.type == KEYDOWN:
				if event.key == K_j:
					print('x: {}, y: {}'.format(m_x, m_y))
				if event.key == K_q:
					exit()

			if event.type ==  MOUSEBUTTONDOWN:
				if pygame.mouse.get_pressed() == (1, 0, 0):
					if (m_x > 377 and m_x < 657) and (m_y > 310 and m_y < 367):
						start = True


		screen.blit(background_before, (0, 0))
	# option
		m_x, m_y = pygame.mouse.get_pos()
		if (m_x > 377 and m_x < 657) and (m_y > 310 and m_y < 367):
			screen.blit(big_option, (355, 297))
	  
		else:
			screen.blit(small_option, (355, 297))

	# caption
		x -= 2
		if x < -caption_surface.get_width():
			x = 900

		screen.blit(caption_surface, (x, 40))

	# text
		screen.blit(line1, (434, 397))
		screen.blit(line2, (424, 467))
		screen.blit(line3, (394, 547))

		pygame.display.update()



	############################################################################################
	'''
												游戏中
	'''
	############################################################################################



# 初始化游戏数据
	SCREEN_SIZE = (1000, 800)
	acceleration = 0
	enemy_num = 5
	score = 0
	weige_num = 1
	weige_start_time = 0
	weige_curr_time = 0

	x, y = (423, 627)
	row, col = pygame.mouse.get_pos()

	strong = False
	strong_assist = False
	random = True
	live = True
	pause = False
	pause_assist = False    #取消暂停时飞机不会移到暂停光标处

	name_list = ('graph/kangmin.png', 'graph/boyuan.png', 'graph/bruce.png',\
				'graph/bob.png', 'graph/tony.png')
	kill_num = [0, 0, 0, 0, 0]


# 初始化游戏对象
	enemy_list=[]
	for i in range(5):
		num = randint(0,4)
		enemy_name = name_list[num]
		e = Enemy(enemy_name, num)
		enemy_list.append(e)

	bomb_list = []

	monster = Monster()
	monster.set_data()

	timer_8s = Timer(8)

	bag = Bag()


# 初始化字体
	font_score = pygame.font.Font('font/GUEVARA_.TTF', 50)
	font_weige = pygame.font.Font(None, 30)
	font_time = pygame.font.Font('font/FZZH-CJTJW.TTF', 30)
	weige_text = font_weige.render('× %d'%weige_num, True, (0, 0, 0))
	weige_time = font_time.render('无敌时间: %d秒'%(weige_curr_time-weige_start_time)\
									, True, (0, 0, 0))


# 准备图像资源
	background_game = pygame.image.load('graph/background.jpg')
	yang = pygame.image.load('graph/yang.png')
	big_yang = pygame.image.load('graph/big_yang.png')
	bomb_image = pygame.image.load('graph/finger.png')
	pause_1 = pygame.image.load('graph/pause1.png')
	pause_2 = pygame.image.load('graph/pause2.png')
	weigeoff = pygame.image.load('graph/capsule_off.png')
	weigeon = pygame.image.load('graph/capsule_on.png')
	weige_image = weigeoff

	screen = pygame.display.set_mode(SCREEN_SIZE, RESIZABLE, 32)

# 游戏主循环
	while live is True:
	# 事件列表
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()

			if event.type == VIDEORESIZE:
				SCREEN_SIZE = event.size
				screen = pygame.display.set_mode(SCREEN_SIZE, RESIZABLE, 32)

			if event.type == KEYDOWN:
				if event.key == K_k:
					print('x: {}, y: {}'.format(row, col))
				if event.key == K_q:
					exit()
				if event.key == K_p:
					if pause == True:
						pause = False
						pause_assist = True
					else:
						pause = True
				if event.key == K_j:
					pygame.mixer.Channel(0).play(weige_music)
					strong = True
					weige_num -= 1
					weige_start_time = int(pygame.time.get_ticks()/1000)


			if event.type ==  MOUSEBUTTONDOWN:
			# 暂停
				if (row > 930 and row < 990) and (col > 733 and col < 793):
					if pause == True:
						pause = False
						pause_assist = True
					else:
						pause = True
			# 伟哥
				if ((row > 769 and row < 862) and (col > 741 and col < 773)) or \
				pygame.mouse.get_pressed() == (0, 0, 1):
					strong_assist = True
					if(weige_num > 0):
						pygame.mixer.Channel(0).play(weige_music)
						strong = True
						weige_num -= 1
						weige_start_time = int(pygame.time.get_ticks()/1000)


	# 调整屏幕
		screen_width, screen_height = SCREEN_SIZE
		for i in range(0, screen_height, background_game.get_height()):
			for j in range(0, screen_width, background_game.get_width()):
				screen.blit(background_game, (i, j))  
		
		if pause is False:
		# 玩家移动
			if pygame.mouse.get_pressed() == (1, 0, 0) and pause is False:
				if pause_assist is False and strong_assist is False:
					x, y = pygame.mouse.get_pos()
					x -= yang.get_width() / 2
					y -= yang.get_height() / 2
				elif pause_assist is True:
					pause_assist = False
				elif strong_assist is True:
					strong_assist = False


		# 变身
			if strong is True:
				weige_curr_time = int(pygame.time.get_ticks()/1000)
				if weige_curr_time - weige_start_time == 4:
					strong = False


		# 伟哥按钮
			if (row > 769 and row < 862) and (col > 741 and col < 773):
				weige_image = weigeon
			else:
				weige_image = weigeoff


		# 空投箱
			if bag.exist is False:
				if randint(1, 100) == 15:
					bag.exist = True

			if bag.exist is True:
				if(x < bag.row+150 and x > bag.row-180) \
				and (y < bag.col+180 and y > bag.row-230):
					pygame.mixer.Channel(1).play(gain)
					weige_num += 1
					bag.setdata()
				else:	
					bag.row += 5
					bag.col += 5
					if(bag.col > 700):
						bag.exist = False


		# 时间
			if timer_8s.right_time() == True:
				acceleration += 1
				enemy_num += 5


		# 炸弹
			if random:
				bomb = Bomb(x+60, y+80)
				random=False
			else:
				bomb = Bomb(x+60, y-230)
				random=True

			bomb_list.append(bomb)
			temp_bomb = bomb_list[:]
			for b in bomb_list:
				if b.col<-75:
					temp_bomb.remove(b)
				else:
					b.col -= 200

			bomb_list = temp_bomb


		# 敌人
			if(len(enemy_list) <= enemy_num):
				temp = randint(0,4)
				e = Enemy(name_list[temp], temp)
				enemy_list.append(e)

			for e in enemy_list:
				if e.col > 734:
					enemy_list.remove(e)
					continue
				for b in bomb_list:
					if((b.row <= e.row+125 and b.row >= e.row+55) and \
						(b.col <= e.col+200 and b.col >= e.col-200)):
						pygame.mixer.Channel(2).play(hit)
						kill_num[e.id_] += 1
						score += 1
						e.cursor = pygame.image.load('graph/bang.png')
						screen.blit(e.cursor, (e.row, e.col))
						enemy_list.remove(e)
						break

				e.col += 7+acceleration


		# 怪兽
			if monster.exist == False:
				monster.create()
			else:
				if monster.col >734:
					monster.set_data()

			if monster.exist == True:
				for b in bomb_list:
					if monster.life == 0:
						score += 5
						monster.cursor=pygame.image.load('graph/big_bang.png')
						screen.blit(monster.cursor, (monster.row, monster.col)) 
						monster.set_data()
						break
					if((b.row<=monster.row+400 and b.row>=monster.row) and \
						(b.col<=monster.col+200 and b.col>=monster.col)):
						monster.life -= 1
						pygame.mixer.Channel(2).play(hit)
					else:
						monster.col += 2+acceleration
				

	# 暂停
		row, col = pygame.mouse.get_pos()
		if (row > 930 and row < 990) and (col > 733 and col < 793):
			screen.blit(pause_2, (930, 733))  
		else:
			screen.blit(pause_1, (930, 733)) 


	# 碰撞
		for e in enemy_list:
			if((x<=e.row+150 and x>=e.row-150) and (y<=e.col+85 and y>=e.col-80)):
				if strong is False:
					live = False
					break
				elif strong is True:
					kill_num[e.id_] += 1
					score += 1
					e.cursor = pygame.image.load('graph/bang.png')
					screen.blit(e.cursor, (e.row, e.col))
					enemy_list.remove(e)

				
		if((x<=monster.row+370 and x>=monster.row-180) \
			and (y<=monster.col+200 and y>=monster.col-120)):
			if strong is False:
				live = False
			elif strong is True:
				score += 5
				monster.cursor=pygame.image.load('graph/big_bang.png')
				screen.blit(monster.cursor, (monster.row, monster.col)) 
				monster.set_data()


	# 画图
		for b in bomb_list:
			screen.blit(bomb_image, (b.row, b.col))

		for e in enemy_list:
			screen.blit(e.cursor, (e.row+50, e.col+50))

		if monster.exist is True:
			screen.blit(monster.cursor, (monster.row, monster.col))

		if bag.exist is True:
			screen.blit(bag.image, (bag.row, bag.col))

		score_surface = font_score.render('Score: %d'%score, True, (255, 68, 68))
		screen.blit(score_surface, (15, 733))

		if strong is False:
			screen.blit(yang, (x, y))
		else:
			screen.blit(big_yang, (x, y))

		if strong is True:
			weige_time = font_time.render('无敌时间: %d秒'%(4-(weige_curr_time-weige_start_time))\
									, True, (0, 0, 0))
			screen.blit(weige_time, (740, 743))
		else:
			weige_text = font_weige.render('× %d'%weige_num, True, (0, 0, 0))
			screen.blit(weige_image, (771, 738))
			screen.blit(weige_text, (870, 747))

	# 刷新画面
		pygame.display.update()


	# 数据台
	

# 游戏失败
	start = int(pygame.time.get_ticks()/1000)
	font_gg = pygame.font.Font('font/字魂48号-半天云魅黑手书.ttf', 130)

	while True:
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()

			if event.type == KEYDOWN:
				if event.key == K_q:
						exit()

		curr = int(pygame.time.get_ticks()/1000)
		if curr-start == 4:
			break
		#print('start: %d, curr: %d'%(start, curr))

		screen.blit(yang, (x, y))

		for e in enemy_list:
			screen.blit(e.cursor, (e.row+50, e.col+50))

		if monster.exist is True:
			screen.blit(monster.cursor, (monster.row, monster.col))

		screen.blit(score_surface, (15, 733))

		gg_surface = font.render('阳阳凉了', True, (255, 68, 68))
		screen.blit(gg_surface, (340, 258))

		pygame.display.update()



	############################################################################################
	'''
												游戏完
	'''
	############################################################################################



	# 初始数据
	nickname = [('烟台克星', 'kangmin_most.png'), 
				('蒙古克星', 'boyuan_most.png'), 
				('册那克星', 'bruce_most.png'), 
				('北京克星', 'bob_most.png'), 
				('上海克星', 'tony_most.png')]
	index = kill_num.index(max(kill_num))
	killer = nickname[index]
	killer_name = killer[0]
	killer_image = killer[1]
	killer_cursor = pygame.image.load('graph/'+killer_image)

	font_3 = pygame.font.Font('font/FZZH-CJTJW.TTF', 75)
	score_text = font_3.render('阳阳得分： %d'%score, True, (255, 68, 68))
	killer_text = font_3.render('获得称号： '+killer_name, True, (255, 68, 68))

	background_after = pygame.image.load('graph/back.jpg').convert()

	screen = pygame.display.set_mode((1000, 750), 0, 32)


	# 画图
	while True:
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()

			if event.type == KEYDOWN:
				if event.key == K_j:
					x, y = pygame.mouse.get_pos()
					print('x: {}, y: {}'.format(x, y))

		screen.blit(background_after, (0, 0))
		screen.blit(score_text, (274, 140))
		screen.blit(killer_text, (174, 246))
		screen.blit(killer_cursor, (370, 350))
		pygame.display.update()











