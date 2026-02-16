# Author: Brenda Waiya
# Date:March 6th, 2023
# Purpose: Algorithms

from cs1lib4 import *

RADIUS = 5
STROKE_WIDTH = 5
r = 1
g = 1
b = 1

class Vertex:
    def __init__(self, vertex_name, x, y, adjacency_list):
        self.vertex_name = vertex_name
        self.x = int(x)
        self.y = int(y)
        self.adjacency_list = adjacency_list
        # self.radius = RADIUS

    def draws_vertices(self, r, g, b):
        set_fill_color(r, g, b)
        set_stroke_color(r, g, b)
        draw_circle(self.x, self.y, RADIUS)

    def draw_edge(self, point, r, g, b):
        set_stroke_color(r, g, b)
        set_stroke_width(STROKE_WIDTH)
        draw_line(self.x, self.y, point.x, point.y)
#draws the edge line connecting the vertices surrounding the current vertex
    def draw_adjacent_edges(self, r, g, b):
        for ele in self.adjacency_list:
            self.draw_edge(ele, r, g, b)

    # FUNCTION THAT CHECKS WHETHER X AND Y ARE CLOSE TO any VERTEX point around
    def edge_checker(self, x, y):
        return (self.x - RADIUS <= x <= self.x + RADIUS) and (self.y - RADIUS <= y <= self.y + RADIUS)


