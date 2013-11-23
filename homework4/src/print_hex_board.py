#!/bin/python

def print_hex_board(board_size, hexagon):

    for i in range(0, board_size):
        line = ''
        for j in range(0, board_size):
            if j is not (board_size - 1):
                line += hexagon[i][j] + ' - '
            else:
                line += hexagon[i][j]
        print('  ' * i + line)

        line = ''
        if i is not board_size - 1:
            for j in range(0, board_size):
                if j is not (board_size - 1):
                    line += '\\ / '
                else:
                    line += '\\'
            print (' ' * (2 * i + 1) + line)


if __name__ == "__main__":
    hexagon = []
    for i in range(0, 7):
        a_line_hexagon = []
        for j in range(0, 7):
            #a_line_hexagon.append('.')
            if j % 2 is 0:
                a_line_hexagon.append('B')
            else:
                a_line_hexagon.append('R')
        hexagon.append(a_line_hexagon)

    print_hex_board(7, hexagon)

#EOF
