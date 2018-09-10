import random

# ------ INPUT DEFINITIONS -------

table_width = 100
table_height = 100
number_of_bodies = 100
radius = 1
mu = 0.01
speed_range = 6
mass = 1
mass_range = 0

# --------------------------------

file = open("input.txt", "w+")
file.write("%f %f %f\n" % (table_width,table_height,number_of_bodies))
file.write("%f 1 1\n" % (mu))

ball_pos = []

nx = int((table_width - 1)/(radius*2 + 1))

ny = 0
while ny*nx < number_of_bodies:
    ny += 1

count = 0
for j in range(0,ny):
    for i in range(0,nx):
        if count == number_of_bodies:
            break
        else:
            x = radius+1 + (radius*2+1)*i
            y = radius+1 + ((table_height-2-radius*2)/ny)*j
            ball_pos.append((x,y)) 
            count += 1

    if count == number_of_bodies:
            break

for ball_id in range(0,number_of_bodies):
    
    vx = float(random.randint(-speed_range,speed_range))
    vy = float(random.randint(-speed_range,speed_range))

    if mass_range != 0:
        mass = float(random.randint(1,mass_range))

    file.write("%f %f %f %f %f %f %f\n" % (ball_id, radius, mass, ball_pos[ball_id][0], ball_pos[ball_id][1], vx, vy))