# Author: Brenda Waiya
# Date:March 6th, 2023
# Purpose: Algorithms

from cs1lib4 import *
from vertex import Vertex
from load_graph import Load_graph
from Breadth_first import *

vert_dict = Load_graph("dartmouth_graph.txt")
mousepressed = False
goal = None
start = None


# mouse_press, mouse_move, draw_path, draw_vertices, main(call all the functioons
def mouse_press(vx, vy):
    global mousepressed, start
    mousepressed = True

    for ele in vert_dict:
        if vert_dict[ele].edge_checker(vx, vy):
            start = vert_dict[ele]


def my_mouse_move(vx, vy):
    global goal
    if mousepressed:  # when the mouse is not pressed, let that the vertex be the goal
        for ele in vert_dict:
            if vert_dict[ele].edge_checker(vx, vy):
                goal = vert_dict[ele]


# draw the map background function
img = load_image("dartmouth_map.png")





# draws tyhe vertices and the linesd connecting them
def draw_function():
    global start, goal
    clear()
    draw_image(img, 0, 0)

    for ele in vert_dict:
        vert_dict[ele].draws_vertices(1, 0.5, 0)
        vert_dict[ele].draw_adjacent_edges(1, 0.5, 0)

    path = input_taker(start, goal)
    for i in range(len(path) - 1):
        path[i].draw_edge(path[i + 1], 0, 0, 0)
        path[i].draws_vertices(0, 0, 0)
        path[i + 1].draws_vertices(0, 0, 0)


start_graphics(draw_function, width=1012, height=811, mouse_press=mouse_press, mouse_move=my_mouse_move)

# start_graphics(draw_func, frames, data, framerate, title, 1012,811,                 mouse_press, mouse_release, mouse_move,key_press, key_release)
