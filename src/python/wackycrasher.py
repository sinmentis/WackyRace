#!/usr/bin/env python

"""Simple program to drive a wacky racer using the arrow keys.  This
program uses the pygame module for platform independence."""

from __future__ import print_function
import pygame
import sys
from wackyconnect import wacky_connect

__author__ = 'Michael Hayes'
__date__ = 'May 2017'


class Motor(object):

    def __init__(self, conn):
        self.conn = conn
        self.state = 'stopped'
        self.speed = 10
        self.speed_min = 10
        self.speed_max = 100
        self.speed_increment = 10        

    def send(self):
        if self.conn is not None:
            speed = self.speed
            if self.state == 'stopped':
                speed = 0
            elif self.state == 'reverse':
                speed = -speed
            self.conn.send_command('>MS%d' % speed)

    def speed_up(self):
        self.speed += self.speed_increment
        if self.speed > self.speed_max:
            self.speed = self.speed_max
        self.send()

    def speed_down(self):
        self.speed -= self.speed_increment
        if self.speed < self.speed_min:
            self.speed = self.speed_min
        self.send()        

    def stop(self):
        self.state = 'stopped'        
        self.send()

    def forward(self):
        self.state = 'forward'
        self.send()

    def reverse(self):
        self.state = 'reverse'        
        self.send()                


class Steering(object):

    def __init__(self, conn):
        self.conn = conn
        self.direction = 0

    def send(self):
        if self.conn is not None:
            self.conn.send_command('>MD%d' % self.direction)

    def turn_right(self):
        self.direction += 10
        if self.direction > 100:
            self.direction = 100
        self.send()

    def turn_left(self):
        self.direction -= 10
        if self.direction < -100:
            self.direction = -100
        self.send()        


class DisplayInfo(object):

    def __init__(self, screen):
        self.screen = screen
        self.font = pygame.font.Font(None, 50)

    def update(self, speed, direction):
        self.screen.fill(pygame.Color('blue'))
        text = self.font.render('Speed: %d' % speed, 1, pygame.Color('white'))
        self.screen.blit(text, (0, 0))
        text = self.font.render('Direction: %d' % direction, 1,
                                pygame.Color('white'))
        self.screen.blit(text, (0, 60))        
        pygame.display.update()        

        
def main():

    conn = wacky_connect(sys.argv, debug=False)
    if conn is None:
        sys.exit(1)    

    pygame.init()

    (width, height) = (300, 200)
    screen = pygame.display.set_mode((width, height))
    pygame.display.set_caption('Wacky crasher')
    
    motor = Motor(conn)
    steering = Steering(conn)    
    display_info = DisplayInfo(screen)

    display_info.update(motor.speed, steering.direction)
    
    while True:
        display_update = False
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit(); 
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_a:
                    motor.speed_up()
                    display_update = True
                if event.key == pygame.K_z:
                    motor.speed_down()
                    display_update = True
                if event.key == pygame.K_UP:
                    motor.forward()
                    display_update = True
                if event.key == pygame.K_DOWN:
                    motor.reverse()
                    display_update = True
                if event.key == pygame.K_SPACE:
                    motor.stop()
                    display_update = True
                if event.key == pygame.K_LEFT:
                    steering.turn_left()
                    display_update = True
                if event.key == pygame.K_RIGHT:
                    steering.turn_right()
                    display_update = True                                        
        if display_update:
            display_info.update(motor.speed, steering.direction)

        # Could periodically poll battery voltage here...

            
if __name__ == '__main__':
    main()

                
