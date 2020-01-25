# UDP-Client-Server
An example of creating a 4-way handshake using the User Datagram Protocol (UDP). This example goes over client/server program code, as well as screen captures on Wireshark for proving tests by observing traffic on a localhost.

The program was built using Visual Studio, and is a Windows console application. The application was written in C, but built using Visual Studio and so it appears as a CPP project.

The 4-way handshake is as follows:
- Client sends "knock knock" on program launch
- The server replys with "dochira sama deshou ko"
- The client sends back "KanfyooSHas" 
- The server ends the handshake by responding with "o nkgx gtj o luxmkz o ykk gtj o xkskshkx o ju gtj o atjkxyzgtj"

Running the program:
You must edit the deault IP-address in the client program, and re-build it (either through Visual Studio console application, or through command line) as the address was hard coded. Once re-built, launch the udpserver executable - the server will listen for any connections. Then, launch the udpclient executable, and the handshake will start. You can test the handshake is working by observing traffic in wireshark over localhost.
