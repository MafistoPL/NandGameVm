with open('feed.txt', 'r') as input_file:
    input_data = input_file.readlines()

for i in range(0, len(input_data)):
    data = input_data[i].split('|')
    x = data[1].strip()
    y = data[2].strip()
    zx = data[3].strip()
    nx = data[4].strip()
    zy = data[5].strip()
    ny = data[6].strip()
    f = data[7].strip()
    no = data[8].strip()
    out = data[9].strip()
    print("EXPECT_EQ(ALU::Compute({{ {no}, {f}, {ny}, {zy}, {nx}, {zx} }}, 0b{x}, 0b{y}), 0b{out});".format(
        no=no, f=f, ny=ny, zy=zy, nx=nx, zx=zx, x=x, y=y, out=out
    ))

