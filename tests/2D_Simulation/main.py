import pygame
import math

class World:
    def __init__(self, world_dimentions):
        self.black = (0,0,0)
        self.white = (255,255,255)
        self.green = (0,255,0)
        self.blue = (0,0,255)
        self.red = (255,0,0)
        self.yellow = (255,255,0)

        self.height = world_dimentions[0]
        self.width = world_dimentions[1]

        pygame.display.set_caption("Little Bot")
        self.map = pygame.display.set_mode((self.width, self.height))
    

class Robot:
    def __init__(self, start_position, robot_image, width):
        self.meterToPixel = 3779.52
        
        self.width = width

        self.x = start_position[0]
        self.y = start_position[1]
        self.theta = 0.0

        self.velocityLeft = 0.01 * self.meterToPixel # [m/s]
        self.velocityRight = 0.01 * self.meterToPixel # [m/s]

        self.maxSpeed = 0.02 * self.meterToPixel
        self.minSpeed = 0.02 * self.meterToPixel

        self.image = pygame.image.load(robot_image)
        self.image = pygame.transform.scale(self.image, (100, 100))
        self.rotatedImage = self.image
        self.rectangle = self.rotatedImage.get_rect(center = (self.x, self.y))

    def draw(self, map):
        map.blit(self.rotatedImage, self.rectangle)

    def move(self, event = None):
        if event is not None:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1:
                    self.velocityLeft += 0.001 * self.meterToPixel
                elif event.key == pygame.K_q:
                    self.velocityLeft -= 0.001 * self.meterToPixel
                elif event.key == pygame.K_0:
                    self.velocityRight += 0.001 * self.meterToPixel
                elif event.key == pygame.K_p:
                    self.velocityRight -= 0.001 * self.meterToPixel
                print("{}, {}\n".format(self.velocityLeft, self.velocityRight))
        self.x += ((self.velocityLeft + self.velocityRight)/2) * math.cos(self.theta) * dt
        self.y -= ((self.velocityLeft + self.velocityRight)/2) * math.sin(self.theta) * dt  
        self.theta += ((self.velocityRight - self.velocityLeft) / self.width) * dt

        self.rotatedImage = pygame.transform.rotozoom(self.image, math.degrees(self.theta), 1)
        self.rectangle = self.rotatedImage.get_rect(center=(self.x, self.y))         

pygame.init()
start_position = (200.0,200.0)
world_dimentions = (600,1200)
RUNNING = True
world = World(world_dimentions)
robot = Robot(start_position, "/Users/jeffreyfisher/Projects/little_bot/tests/2D_Simulation/little_bot_sprite.png", 0.01 * 3779.52)

dt = 0.0
lasttime = pygame.time.get_ticks()
while RUNNING:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            RUNNING = False
        robot.move(event)
    dt = (pygame.time.get_ticks() - lasttime) / 1000.0
    lasttime = pygame.time.get_ticks()
    pygame.display.update()
    world.map.fill(world.black)
    robot.move()
    robot.draw(world.map)

