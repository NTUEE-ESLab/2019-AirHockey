#DIMENSIONS AND COORDINATES
WIDTH=800
HEIGHT=600
BORDER_TOP=154
BORDER_BOTTOM=40
BORDER_LEFT=40
BORDER_RIGHT=40
LIMIT_TOP = BORDER_TOP
LIMIT_BOTTOM = HEIGHT - BORDER_BOTTOM
LIMIT_LEFT = BORDER_LEFT
LIMIT_RIGHT = WIDTH - BORDER_RIGHT
FIELD_HEIGHT = LIMIT_BOTTOM - BORDER_TOP
FIELD_WIDTH = LIMIT_RIGHT - BORDER_LEFT

GOAL_WIDTH=134
GOAL_START = BORDER_TOP+0.5*(FIELD_HEIGHT-GOAL_WIDTH)
GOAL_END = BORDER_TOP+0.5*(FIELD_HEIGHT+GOAL_WIDTH)

PLAYER1_START = (BORDER_LEFT+FIELD_WIDTH*0.8,BORDER_TOP+FIELD_HEIGHT*0.5)
PLAYER2_START = (BORDER_LEFT+FIELD_WIDTH*0.2,BORDER_TOP+FIELD_HEIGHT*0.5)
PLAYER_LABEL_POS = ((400,20),(400,80))
PLAYER_LABEL_COLOR = ((255,0,0),(0,0,255))

CHRONO_LABEL_POS = (600,40)
CHRONO_LABEL_COLOR = (0,0,0)


#FPS
FPS=60

#MAIN RESOURCES FOLDER
RESOURCES = '../resources/'

#IMAGES
BG_PATH=RESOURCES+'img/bg.png'
BG_INSTRUCTION_PATH=RESOURCES+'img/bg_instruction.png'
ICON=RESOURCES+'img/icon.png'

#GOALPOST
GOALPOST_PATH=RESOURCES+'img/palo.png'
GOALPOST_MASS=100000

#MALLET
MALLET_RED_PATH=RESOURCES+'img/red_mallet.png'
MALLET_BLUE_PATH=RESOURCES+'img/blue_mallet.png'
MALLET_MAX_SPEED=0.4
MALLET_ACCELERATION=0.0025
MALLET_FRICTION=0.001
MALLET_MASS=10

#DISC
DISC_PATH=RESOURCES+'img/disc.png'
DISC_MAX_SPEED=0.6
DISC_FRICTION=0.0001
DISC_MASS=1
DISC_START_ANGLE=90
DISC_START_POS = (BORDER_LEFT+FIELD_WIDTH*0.5,BORDER_TOP)
DISC_START_SPEED = DISC_MAX_SPEED*0.5

#SOUNDS
COLLISION_SOUND=RESOURCES+'sounds/collision.wav'
END_SOUND=RESOURCES+'sounds/end.wav'
GOAL_SOUND=RESOURCES+'sounds/goal.wav'

#MESSAGES
GOAL1 = "PLAYER 1 SCORED!!!"
GOAL2 = "PLAYER 2 SCORED!!!"
WIN1 = "PLAYER 1 WON!!!"
WIN2 = "PLATER 2 WON!!!"
INSTR1 = "PLAYER 1: USE ARROWS"
INSTR2 = "PLAYER 2: USE W-A-S-D"
INSTR3 = "SPACE BAR STARTS A NEW GAME"
INSTR4 = "PRESS ESC TO EXIT INSTRUCTIONS"
TITLE = "Air Hockey"
SELECT_TIME = "CHOOSE GAME TIME [1,3,5]"
INSTRUCTION = "PRESS F1 FOR INSTRUCTIONS"