with open('feed.txt', 'r') as input_file:
    input_data = input_file.readlines()

prev_clock = '0'
for i in range(0, len(input_data)):
    data = input_data[i].split('|')
    clock = '+' in data[1]
    test_in = data[2].strip()
    load = data[3].strip()
    addr = bin(int(data[4].strip())).replace("0b", "")
    addr = '000000000'[:9 - len(addr)] + addr
    expected_output = data[5].strip()

    # EXPECT_EQ(ram.setNewStateAndGetResult({ 0, 0, 0 }, 1, 0x1234, 1), 0x1234);
    print("EXPECT_EQ(ram.setNewStateAndGetResult({ ", end='')
    for j in range(0, len(addr)):
        print('{}'.format(addr[j]), end='')
        if j != len(addr) - 1:
            print(',', end='')
        print(' ', end='')
    print('}}, {load}, {test_in}, {clk}), {exc_out});'.format(load=load, test_in=test_in, clk=prev_clock, exc_out=expected_output))

    if clock is True:
        prev_clock = '1'
    else:
        prev_clock = '0'
