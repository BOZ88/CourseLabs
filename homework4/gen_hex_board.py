#!/bin/python
'''
    generate a graph file which describes the hex board.

    first line is the number of vertices
    the following are edges as format of (from_vertex, to_vertex, cost)
    cost value simply choose 1

    for example: 7 X 7 hex board
    . - . - . - . - . - . - .
     \ / \ / \ / \ / \ / \ / \
      . - . - . - . - . - . - .
       \ / \ / \ / \ / \ / \ / \
        . - . - . - . - . - . - .
         \ / \ / \ / \ / \ / \ / \
          . - . - . - . - . - . - .
           \ / \ / \ / \ / \ / \ / \
            . - . - . - . - . - . - .
             \ / \ / \ / \ / \ / \ / \
              . - . - . - . - . - . - .
               \ / \ / \ / \ / \ / \ / \
                . - . - . - . - . - . - .
'''


def gen_hex_board(board_size):
    '''generate a bex board (board_size X board_size)'''

    assert(board_size > 0)
    vertex_num_tatal = board_size * board_size
    print vertex_num_tatal

    for i in range(0, vertex_num_tatal - 1):
        # the - style edges
        if (i + 1) % board_size is not 0:
            print "%d %d 1" % (i, i + 1)
            print "%d %d 1" % (i + 1, i)

        # the \ style edges
        if i < vertex_num_tatal - board_size:
            print "%d %d 1" % (i, i + board_size)
            print "%d %d 1" % (i + board_size, i)

        # the / style edges
        if i < vertex_num_tatal - board_size:
            if i % board_size is not 0:
                print "%d %d 1" % (i, i + board_size - 1)
                print "%d %d 1" % (i + board_size - 1, i)

if __name__ == "__main__":
    gen_hex_board(7)
