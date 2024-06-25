# run via python3.8 ipkd.py
from socketserver import TCPServer, UDPServer, StreamRequestHandler, BaseRequestHandler
import logging
import re
import threading
import signal
import sys
LISP_RE = rb'\(([+\-*/]) (\d+) (\d+)\)'

def expr(data):
    op, a, b = data
    a = int(a)
    b = int(b)
    if op == b'+': return a + b
    if op == b'-': return a - b
    if op == b'*': return a * b
    if op == b'/': return a / b
    raise SyntaxError(f'unknown operator: {op!a}')


def lisp(data):
    while not re.fullmatch(b'\d+', data):
        subexpr = re.search(LISP_RE, data)
        if not subexpr:
            raise SyntaxError(f'not a valid lisp expression: {data!a}')
        subres = expr(subexpr.groups())
        data = re.sub(LISP_RE, str(subres).encode('ascii'), data, 1)
    return int(data)


class TCPIPK(StreamRequestHandler):
    def error_out(self, msg):
        self.wfile.write(b'BYE\n')
        raise SyntaxError(msg)
    def handle(self):
        logging.info('new connection from %s', self.request)
        hello = self.rfile.readline()
        if hello != b'HELLO\n':
            self.error_out(f'Did not read HELLO as first message, got {hello!a}')
        self.wfile.write(b'HELLO\n')
        logging.info('connection %s said hello', self.request)
        for line in self.rfile:
            command, space, data = line.rstrip(b'\n').partition(b' ')
            logging.info('connection %s gave command %a', self.request, command)
            if command == b'BYE':
                if space:
                    self.error_out(f'got bye command: {line!a}, but BYE takes no extra data')
                self.wfile.write(b'BYE\n')
                logging.info('connection %a said bye', self.request)
                return
            if command == b'SOLVE':
                if not space:
                    self.error_out(f'got solve command {line!a}, but SOLVE requires an expression to solve')
                try:
                    res = lisp(data)
                except Exception as e:
                    self.error_out(str(e))
                else:
                    self.wfile.write(b'RESULT ')
                    self.wfile.write(str(res).encode('ascii'))
                    self.wfile.write(b'\n')
                    logging.info('connection %s was sent %s, the solution to %a', command, res, data)
                    continue
            self.error_out(f'got command {line!a}, but command {command!a} is not implemented')

class UDPIPK(BaseRequestHandler):
    def handle(self):
        datagram, request = self.request
        logging.info('new connection from %s', request)
        logging.info('got datagram from %s: %a', request, datagram)
        if datagram[0] != 0:
            raise SyntaxError('Got non-zero opcode from request')
        length = datagram[1]
        if length + 2 != len(datagram):
            raise SyntaxError(f'Incorrect length, expected {len(datagram)-2}, got {length}')
        data = datagram[2:]
        logging.info('connection %s: parsed request %a', request, data)
        try:
            res = str(lisp(data))
        except Exception as e:
            status = 1
            res = str(e)
            logging.info('connection %s: computed error %a', request, res)
        else:
            status = 0
            logging.info('connection %s: computed answer %a', request, res)
        print(repr(res))
        res = res.encode('ascii')
        print(repr(res))

        msg = bytes([1, status, len(res)]) + res
        request.sendto(msg, self.client_address)

HOST = '0.0.0.0'
PORT = 2023

def serve_tcp():
    with TCPServer((HOST, PORT), TCPIPK) as server:
        server.serve_forever()

def serve_udp():
    with UDPServer((HOST, PORT), UDPIPK) as server:
        print('started', repr(server))
        server.serve_forever()

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    logging.info('started on %s:%s', HOST, PORT)
    threading.Thread(target=serve_tcp, daemon=True).start()
    threading.Thread(target=serve_udp, daemon=True).start()
    input('press enter to exit\n')