import socket

keymap = {
 ord('a')        : [0x04] , # Keyboard a and A
 ord('b')        : [0x05] , # Keyboard b and B
 ord('c')        : [0x06] , # Keyboard c and C
 ord('d')        : [0x07] , # Keyboard d and D
 ord('e')        : [0x08] , # Keyboard e and E
 ord('f')        : [0x09] , # Keyboard f and F
 ord('g')        : [0x0A] , # Keyboard g and G
 ord('h')        : [0x0B] , # Keyboard h and H
 ord('i')        : [0x0C] , # Keyboard i and I
 ord('j')        : [0x0D] , # Keyboard j and J
 ord('k')        : [0x0E] , # Keyboard k and K
 ord('l')        : [0x0F] , # Keyboard l and L
 ord('m')        : [0x10] , # Keyboard m and M
 ord('n')        : [0x11] , # Keyboard n and N
 ord('o')        : [0x12] , # Keyboard o and O
 ord('p')        : [0x13] , # Keyboard p and P
 ord('q')        : [0x14] , # Keyboard q and Q
 ord('r')        : [0x15] , # Keyboard r and R
 ord('s')        : [0x16] , # Keyboard s and S
 ord('t')        : [0x17] , # Keyboard t and T
 ord('u')        : [0x18] , # Keyboard u and U
 ord('v')        : [0x19] , # Keyboard v and V
 ord('w')        : [0x1A] , # Keyboard w and W
 ord('x')        : [0x1B] , # Keyboard x and X
 ord('y')        : [0x1C] , # Keyboard y and Y
 ord('z')        : [0x1D] , # Keyboard z and Z
 ord('1')        : [0x1E] , # Keyboard 1 and !
 ord('2')        : [0x1F] , # Keyboard 2 and @
 ord('3')        : [0x20] , # Keyboard 3 and, #
 ord('4')        : [0x21] , # Keyboard 4 and $
 ord('5')        : [0x22] , # Keyboard 5 and %
 ord('6')        : [0x23] , # Keyboard 6 and ^
 ord('7')        : [0x24] , # Keyboard 7 and &
 ord('8')        : [0x25] , # Keyboard 8 and *
 ord('9')        : [0x26] , # Keyboard 9 and (
 ord('0')        : [0x27] , # Keyboard 0 and )
 ord('\n')        : [0x28] , # Keyboard Return (ENTER)
 ord(' ')        : [0x2C] , # Keyboard Spacebar
 ord('-')        : [0x2D] , # Keyboard - and (underscore)
 ord('=')        : [0x2E] , # Keyboard = and +
 ord('[')        : [0x2F] , # Keyboard [ and {
 ord(']')        : [0x30] , # Keyboard ] and }
 ord('\\')        : [0x31] , # Keyboard \ and |
 ord(';')        : [0x33] , # Keyboard ; and :
 ord('\'')        : [0x34] , # Keyboard ' and "
 ord('`')        : [0x35] , # Keyboard Grave Accent and Tilde
 ord(',')        : [0x36] , # Keyboard, and <
 ord('.')        : [0x37] , # Keyboard . and >
 ord('/')        : [0x38] , # Keyboard / and ?
 ord('A')        : [0xE1, 0x04] , # Keyboard a and A
 ord('B')        : [0xE1, 0x05] , # Keyboard b and B
 ord('C')        : [0xE1, 0x06] , # Keyboard c and C
 ord('D')        : [0xE1, 0x07] , # Keyboard d and D
 ord('E')        : [0xE1, 0x08] , # Keyboard e and E
 ord('F')        : [0xE1, 0x09] , # Keyboard f and F
 ord('G')        : [0xE1, 0x0A] , # Keyboard g and G
 ord('H')        : [0xE1, 0x0B] , # Keyboard h and H
 ord('I')        : [0xE1, 0x0C] , # Keyboard i and I
 ord('J')        : [0xE1, 0x0D] , # Keyboard j and J
 ord('K')        : [0xE1, 0x0E] , # Keyboard k and K
 ord('L')        : [0xE1, 0x0F] , # Keyboard l and L
 ord('M')        : [0xE1, 0x10] , # Keyboard m and M
 ord('N')        : [0xE1, 0x11] , # Keyboard n and N
 ord('O')        : [0xE1, 0x12] , # Keyboard o and O
 ord('P')        : [0xE1, 0x13] , # Keyboard p and P
 ord('Q')        : [0xE1, 0x14] , # Keyboard q and Q
 ord('R')        : [0xE1, 0x15] , # Keyboard r and R
 ord('S')        : [0xE1, 0x16] , # Keyboard s and S
 ord('T')        : [0xE1, 0x17] , # Keyboard t and T
 ord('U')        : [0xE1, 0x18] , # Keyboard u and U
 ord('V')        : [0xE1, 0x19] , # Keyboard v and V
 ord('W')        : [0xE1, 0x1A] , # Keyboard w and W
 ord('X')        : [0xE1, 0x1B] , # Keyboard x and X
 ord('Y')        : [0xE1, 0x1C] , # Keyboard y and Y
 ord('Z')        : [0xE1, 0x1D] , # Keyboard z and Z
 ord('!')        : [0xE1, 0x1E] , # Keyboard 1 and !
 ord('@')        : [0xE1, 0x1F] , # Keyboard 2 and @
 ord('#')        : [0xE1, 0x20] , # Keyboard 3 and, #
 ord('$')        : [0xE1, 0x21] , # Keyboard 4 and $
 ord('%')        : [0xE1, 0x22] , # Keyboard 5 and %
 ord('^')        : [0xE1, 0x23] , # Keyboard 6 and ^
 ord('&')        : [0xE1, 0x24] , # Keyboard 7 and &
 ord('(')        : [0xE1, 0x25] , # Keyboard 8 and *
 ord(')')        : [0xE1, 0x26] , # Keyboard 9 and (
 ord('}')        : [0xE1, 0x27] , # Keyboard 0 and )
 ord('_')        : [0xE1, 0x2D] , # Keyboard - and (underscore)
 ord('+')        : [0xE1, 0x2E] , # Keyboard = and +
 ord('{')        : [0xE1, 0x2F] , # Keyboard [ and {
 ord('}')        : [0xE1, 0x30] , # Keyboard ] and }
 ord('|')        : [0xE1, 0x31] , # Keyboard \ and |
 ord(':')        : [0xE1, 0x33] , # Keyboard ; and :
 ord('"')        : [0xE1, 0x34] , # Keyboard ' and "
 ord('~')        : [0xE1, 0x35] , # Keyboard Grave Accent and Tilde
 ord('<')        : [0xE1, 0x36] , # Keyboard, and <
 ord('>')        : [0xE1, 0x37] , # Keyboard . and >
 ord('?')        : [0xE1, 0x38] , # Keyboard / and ?
}

def main():
    s = socket.create_connection(('localhost', 31337))
    buf = None
    with open('input.txt', 'rb') as f:
        buf = f.read()
    for b in buf:
        cmd = None
        b = ord(b)
        if b in keymap:
            if len(keymap[b]) == 2:
                cmd = '\x04\x80\x02\r\n'
                cmd += '\x01\x00' + chr(keymap[b][1]) + '\r\n'
                cmd += '\x02\x00' + chr(keymap[b][1]) + '\r\n'
                cmd += '\x04\x00\x00\r\n'
            else:
                cmd = '\x01\x00' + chr(keymap[b][0]) + '\r\n'
                cmd += '\x02\x00' + chr(keymap[b][0]) + '\r\n'
            s.send(cmd)

if __name__ == '__main__':
    main()
