import pygame
import sys
import os
from pygame.locals import *
from Chrono import Chrono
from Mallet import Mallet
from Goalpost import Goalpost
from Disc import Disc
from const import *
from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import threading
import socket

#keys = {K_LEFT : False, K_RIGHT : False, K_UP : False, K_DOWN : False,  \
#		K_a : False, K_d : False, K_w : False, K_s : False}
num = 0
actions = [ [0, 0, 0], [0, 0, 0] ]

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.settimeout(0.0003)
server.bind(('172.20.10.6', 6688))
#server.listen(5)
#connect, (host, port) = server.accept()
'''
while 1:
	#data = connect.recv(3).decode('utf-8')
	data, (remoteHost, remotePort) = server.recvfrom(3)
	data = data.decode('utf-8')
	print([data[0], data[1], data[2]])
'''

def update():
	'''
	player = int(data[2])
	actions[player][0] = int(data[0])
	actions[player][1] = int(data[1])
	'''
	while 1:
		#x1 = y1 = x2 = y2 = 0
		if pygame.key.get_pressed()[K_ESCAPE]:
			print(num)
			sys.exit()
		
		try:
			data, (remoteHost, remortPort) = server.recvfrom(12)
			data = data.decode('utf-8').split(',')
			player = int(data[2][0])
			x = int(data[0])
			y = int(data[1])
			if x > 128: x -= 256
			if y > 128: y -= 256
			actions[player][0] = x
			actions[player][1] = y

		except socket.timeout:
			pass
			

def init(angle=0):
	player1.set_pos_xy(PLAYER1_START)
	player1.set_speed_magnitude(0)
	player2.set_pos_xy(PLAYER2_START)
	player2.set_speed_magnitude(0)	
	disc.set_pos_xy((DISC_START_POS[0],DISC_START_POS[1]+disc.get_width()*0.5))
	disc.set_speed_angle(DISC_START_ANGLE+angle)
	disc.set_speed_magnitude(DISC_START_SPEED)
	if angle == 0:
		player1.set_point(0)
		player2.set_point(0)
		chrono.reset()
'''
def get_action0():
	data = connect.recv(3).decode('utf-8')
	data, (remoteHost, remortPort) = server.recvfrom(12)
			data = data.decode('utf-8').split(',')
			if data[2][0] == '0':
	updata(data)

def get_action1():
	while 1:
		pass
'''			
def game():

	init()

	while 1:
		screen.blit( bg, (0,0))

		for event in pygame.event.get():
			if event.type == QUIT:
				sys.exit()
		  
		keys = pygame.key.get_pressed()

		#get_action()
		if pygame.key.get_pressed()[K_ESCAPE]:
			print(num)
			sys.exit()
		
		'''
		x1 = x2 = y1 = y2 = 0
		#data = connect.recv(3).decode('utf-8')
		try:
			data, (remoteHost, remortPort) = server.recvfrom(12)
			data = data.decode('utf-8').split(',')
			if data[2][0] == '0':
				#if data[0] == '1': x1 = 1.0
				#elif data[0] == '2': x1 = -1.0
				#else: x1 = 0.0
				#if data[1] == '1': y1 = 1.0
				#elif data[1] == '2': y1 = -1.0
				#else: y1 = 0.0
				x1 = int(data[0])
				if x1 > 128:
					x1 -= 256
				y1 = int(data[1])
				if y1 > 128:
					y1 -= 256
			
			data, (remoteHost, remortPort) = server.recvfrom(12)
			data = data.decode('utf-8').split(',')
			if data[2][0] == '1':
				x2 = int(data[0])
				if x2 > 128:
					x2 -= 256
				y2 = int(data[1])
				if y2 > 128:
					y2 -= 256
			
		
		except socket.timeout:
			x1 = x2 = y1 = y2 = 0
		'''
		x1, y1, x2, y2 = actions[0][0], actions[0][1], actions[1][0], actions[1][1]

		if keys[ K_SPACE]: break
		
		dt = clock.tick(200)#(FPS)

		chrono.add_millisecond(dt)

		if chrono.get_minute() >= time:# and player1.get_point()<>player2.get_point():
			 end_game()
			 break

		player1.mod(x1,y1,dt)
		player2.mod(x2,y2,dt)
		disc.mod(dt)

		for p in posts:
			p.collision(disc,dt)
		if(disc.collision(player1,dt)): collision_sound.play()
		if(disc.collision(player2,dt)): collision_sound.play()

		player1.move(dt)
		player2.move(dt)
		result = disc.move(dt)

		if result > 0:#<> 0:
			goal(result)

		chrono.blit(screen,font1)
		disc.blit(screen)
		player1.blit(screen,font1)
		player2.blit(screen,font1)
		pygame.display.update()


def goal(result):

	goal_sound.play()

	if result == 1:
		player1.inc_point()
		goal_message = GOAL1
		goal_label = font2.render(goal_message, 1, (255,0,0))
		init(20)
	elif result == 2:
		player2.inc_point()
		goal_message = GOAL2
		goal_label = font2.render(goal_message, 1, (0,0,255))
		init(-20)

	screen.blit(goal_label,(WIDTH*0.5-goal_label.get_width()*0.5,HEIGHT*0.5))
	pygame.display.update()
	pygame.time.wait(2000)
	clock.tick(FPS) 
		

def end_game():

	end_sound.play()

	if player1.get_point()>player2.get_point():
		end = WIN1
		end_label = font2.render(end, 1, (255,0,0))
	else:
		end = WIN2
		end_label = font2.render(end, 1, (0,0,255))

	screen.blit(end_label,(WIDTH*0.5-end_label.get_width()*0.5,HEIGHT*0.5))
	pygame.display.update()
	pygame.time.wait(3000)	 
	clock.tick(FPS)	


def instruction_screen():

	while 1:
		instr1_label = font1.render(INSTR1, 1, (0,0,0))
		instr2_label = font1.render(INSTR2, 1, (0,0,0))
		instr3_label = font1.render(INSTR3, 1, (0,0,0))
		instr4_label = font1.render(INSTR4, 1, (0,0,0))
		
		screen.blit( bg_instruction, (0,0))
		screen.blit(instr1_label,(WIDTH*0.5-instr1_label.get_width()*0.5,200))
		screen.blit(instr2_label,(WIDTH*0.5-instr2_label.get_width()*0.5,250))
		screen.blit(instr3_label,(WIDTH*0.5-instr3_label.get_width()*0.5,450))
		screen.blit(instr4_label,(WIDTH*0.5-instr4_label.get_width()*0.5,500))
		
		pygame.display.update()
		for event in pygame.event.get():
			if event.type == QUIT:
				sys.exit()
	  
		keys = pygame.key.get_pressed()
		if keys[ K_ESCAPE]:
			break
'''
while 1:		
	data = connect.recv(3).decode('utf-8')
	print(data)	
'''
pygame.init()

mixer = pygame.mixer
mixer.init()
collision_sound=mixer.Sound(COLLISION_SOUND)
end_sound=mixer.Sound(END_SOUND)
goal_sound=mixer.Sound(GOAL_SOUND)

os.environ['SDL_VIDEO_CENTERED'] = '1'

icon = pygame.image.load(ICON)
pygame.display.set_icon(icon)
pygame.display.set_caption(TITLE)

screen = pygame.display.set_mode((WIDTH,HEIGHT), 0, 32)

bg = pygame.image.load(BG_PATH).convert()
bg_instruction = pygame.image.load(BG_INSTRUCTION_PATH).convert()

clock = pygame.time.Clock()
chrono = Chrono(0,0,0)

player1 = Mallet(PLAYER1_START,MALLET_MASS,MALLET_RED_PATH,MALLET_MAX_SPEED,MALLET_ACCELERATION,MALLET_FRICTION,1)
player2 = Mallet(PLAYER2_START,MALLET_MASS,MALLET_BLUE_PATH,MALLET_MAX_SPEED,MALLET_ACCELERATION,MALLET_FRICTION,2)

disc = Disc(DISC_START_POS,DISC_START_ANGLE,DISC_START_SPEED,DISC_MASS,DISC_PATH,DISC_MAX_SPEED,DISC_FRICTION)

posts = []
posts.append(Goalpost((BORDER_LEFT,GOAL_START), (-1,-1), GOALPOST_MASS, GOALPOST_PATH))
posts.append(Goalpost((BORDER_LEFT,GOAL_END), (-1,1), GOALPOST_MASS, GOALPOST_PATH))
posts.append(Goalpost((WIDTH-BORDER_RIGHT,GOAL_START), (1,-1), GOALPOST_MASS, GOALPOST_PATH))
posts.append(Goalpost((WIDTH-BORDER_RIGHT,GOAL_END), (1,1), GOALPOST_MASS, GOALPOST_PATH))

font1 = pygame.font.SysFont("Verdana", 24,True)
font2 = pygame.font.SysFont("Verdana", 40,True)

title_game = TITLE.upper()
title_game_label = font2.render(title_game, 1, (0,0,0))
select_time = SELECT_TIME
select_time_label = font1.render(select_time, 1, (0,0,0))
instruction = INSTRUCTION
instruction_label = font1.render(instruction, 1, (0,0,0))

'''
t0 = threading.Thread(target = get_action0)
t1 = threading.Thread(target = get_action1)
t0.start()
t1.start()
'''

t0 = threading.Thread( target = update )
t0.start()

t1 = threading.Thread( target = update )
t1.start()

t2 = threading.Thread( target = update )
t2.start()
while 1:
	screen.blit( bg, (0,0))
	screen.blit(title_game_label,(WIDTH*0.5-title_game_label.get_width()*0.5,200))
	screen.blit(select_time_label,(WIDTH*0.5-select_time_label.get_width()*0.5,300))
	screen.blit(instruction_label,(WIDTH*0.5-instruction_label.get_width()*0.5,400))
	pygame.display.update()
	
	for event in pygame.event.get():
		if event.type == QUIT:
			sys.exit()
	  
	keys = pygame.key.get_pressed()
	
	dt = clock.tick(FPS)
	
	time = 0
	if keys[K_ESCAPE]: sys.exit()
	if keys[ K_1]: time = 1
	elif keys[ K_3]: time = 3
	elif keys[ K_5]: time = 5
	if time>0:#<>0:
		game()
	elif keys[ K_F1]:
		instruction_screen()
