# Author: Brenda Waiya
# Date:March 6th, 2023
#Purpose: Algorithms


# Project: Recursive Binary functions
from vertex import Vertex



def Load_graph(filename):

    vertex_dict = {}

        # Read the lines in the file into a list of lines:
    file = open(filename, "r")

    for l in file:

    #
        if len(l.split(";")) == 3:
                section_split = l.split(";")  #separates them based on a semi-colomn
                vertex_name = section_split[0].strip()  #using vertex_names as keys for the dictionary
                x = section_split[2].strip().split(',')[0]
                y = section_split[2].strip().split(',')[1]

                vert_object = Vertex(vertex_name, x, y, [])
                vertex_dict[vertex_name] = vert_object #vertex_name is the key



    file.close()


    file = open(filename, "r")

    for l in file:
#

        if len(l.split(";")) == 3:
            section_split = l.split(";")
            vertex_name = section_split[0].strip()
            adjacency_list = section_split[1].strip().split(', ')

            obj = vertex_dict[vertex_name]
            for vert in adjacency_list:
                a = vertex_dict[vert]
                obj.adjacency_list.append(a)
    file.close()
    return vertex_dict







