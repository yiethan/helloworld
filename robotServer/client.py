# Echo client program  
import socket  
import sys  
  
HOST = '127.0.0.1'    # The remote host  
PORT = 3000              # The same port as used by the server  
s = None  
for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM):  
    af, socktype, proto, canonname, sa = res  
    try:  
        s = socket.socket(af, socktype, proto)  
    except socket.error, msg:  
        s = None  
        continue  
    try:  
        s.connect(sa)  
    except socket.error, msg:  
        s.close()  
        s = None  
        continue  
    break  
if s is None:  
    print 'could not open socket'  
    sys.exit(1)  
else:      
	s.sendall('2111111345')
	print 'senddata', 2111111345
	data = s.recv(1024)
	s.close()
	print 'Received', repr(data)