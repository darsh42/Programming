
    def caster(self):
        # position inside of a sector
        rel_pos = (self.position[0] % self.map.segment_size[0],
                   self.position[1] % self.map.segment_size[1])

        # Line end point is sector
        center = [self.position[0], self.position[1] - rel_pos[1]]

        for point in self.screen(center, rel_pos):
            self.draw_ray(point, (0,255,0))

        #for x in screen:
        #    self.draw_ray(x, (0,255,0))

    
    def draw_ray(self, endpoint, colour):
        pygame.draw.line(self.surface,
                         colour,
                         self.position,
                         endpoint)

    def clip(self, endpoint):
        for sector in self.map.walls:
            if sector[1] != 0:
                clip = sector[0].clipline(self.position[0], self.position[1],
                                          endpoint[0], endpoint[1])
                if clip != ():
                    pygame.draw.line(self.surface,
                                    (0,0,255),
                                    clip[0],
                                    clip[1])
                    return clip[0]
            
        return endpoint

    # creates the screen
    def screen(self, center, rel_pos):
        points = []
        final_points = []
        angle_size = self.FOV/self.ray_count
        for ray in range(0,self.ray_count):
            # translates the x-point such that the screen can be made
            #                      screen
            #             -----------------------
            #                   \   |    /
            #                    \  |  /
            #                       x

            x_offset = rel_pos[1]*math.tan(math.radians(ray*angle_size))
            left = (center[0] - x_offset, center[1])
            right = (center[0] + x_offset, center[1])
            points.append(self.clip(left))
            points.append(self.clip(right))
        

        for point, angle_mult, in zip(points, range(0, self.ray_count)):
            angle = math.radians(angle_mult*(self.FOV/self.ray_count))

            # works
            while not self.collision(point):
                if point[0] < center[0]:
                    point = (point[0] - self.map.segment_size[1]*math.tan(angle),
                             point[1] - self.map.segment_size[1])
                else:
                    point = (point[0] + self.map.segment_size[1]*math.tan(angle),
                             point[1] - self.map.segment_size[1])

            final_points.append(self.clip(point))

        return final_points