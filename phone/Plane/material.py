# 键盘
pygame.key.set_repeat(1,50)
if event.type ==  KEYDOWN:
	keys = pygame.key.get_pressed()
        
	if event.key == K_j:
		x, y = pygame.mouse.get_pos()
		print('x: {}, y: {}'.format(x, y))
	if event.key == K_q:
		exit()
	if event.key == K_RIGHT:
		angle += 5
		row += 1
	elif event.key == K_LEFT:
		angle -= 5
		row -= 5
	if event.key == K_a:
		if keys[pygame.K_j]:
			pass



# 碰撞
hero = pygame.image.load("image/img-plane_3.png").convert_alpha()
herorect = hero.get_rect()

enemy_mask = pygame.mask.from_surface(enemy_).convert_alpha()
heromask = pygame.mask.from_surface(hero)

offsetX = enemy_rect.x - herorect.x
offsetY = enemy_rect.y - herorect.y
point = heromask.overlap(enemy_mask,(offsetX,offsetY))

if point:
	pass #碰撞反应



# 旋转
plane = pygame.image.load("image/bullet_10.png").convert()
plane_surface = pygame.transform.rotate(plane, angle)

screen.blit(plane_surface, plane.rect)



while not done:
	for event in pygame.event.get():
		if event.type ==QUIT:
			pygame.quit()
			exit()
	
		if event.type ==  MOUSEBUTTONDOWN:
			m_x, m_y = pygame.mouse.get_pos()
			if pygame.mouse.get_pressed() == (1, 0, 0):
				if (m_x > 170 and m_x < 221) and (m_y > 710 and m_y < 757):
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
			'''if event.key == K_UP:
				chat_rect.y -= 2
			if event.key == K_DOWN:
				chat_rect.y += 2
			if event.key == K_RIGHT:
				chat_rect.x += 2
			if event.key == K_LEFT:
				chat_rect.x -= 2
			if event.key == K_j:
				print('girl',girl_rect)'''

	mx,my = pygame.mouse.get_pos()
	screen.fill((255, 255, 255))
	screen.blit(phone, (0, 0))
	

	screen.blit(phone2, (0, 0))
	
	pygame.display.update()



pygame.display.set_caption(str(mx) + "," + str(my))



self.screen.fill((255, 255, 255))
self.screen.blit(self.phone, (0, 0))

self.screen.blit(self.phone2, (0, 0))


red
(234, 50, 50)
























